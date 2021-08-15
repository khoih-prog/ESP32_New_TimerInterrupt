/****************************************************************************************************************************
  ESP32_New_ISR_Timer.cpp
  For ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_New_TimerInterrupt
  Licensed under MIT license

  The ESP32, ESP32_S2, ESP32_C3 have two timer groups, TIMER_GROUP_0 and TIMER_GROUP_1
  1) each group of ESP32, ESP32_S2 has two general purpose hardware timers, TIMER_0 and TIMER_1
  2) each group of ESP32_C3 has ony one general purpose hardware timer, TIMER_0
  
  All the timers are based on 64 bits counters and 16 bit prescalers. The timer counters can be configured to count up or down 
  and support automatic reload and software reload. They can also generate alarms when they reach a specific value, defined by 
  the software. The value of the counter can be read by the software program.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one ESP32-S2 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      15/08/2021 Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
*****************************************************************************************************************************/

#include "ESP32_New_ISR_Timer.h"
#include <string.h>

ESP32_ISR_Timer::ESP32_ISR_Timer()
  : numTimers (-1)
{
}

void ESP32_ISR_Timer::init() 
{
  unsigned long current_millis = millis();   //elapsed();

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    memset((void*) &timer[i], 0, sizeof (timer_t));
    timer[i].prev_millis = current_millis;
  }

  numTimers = 0;

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  timerMux = portMUX_INITIALIZER_UNLOCKED;
}

void IRAM_ATTR ESP32_ISR_Timer::run() 
{
  uint8_t i;
  unsigned long current_millis;

  // get current time
  current_millis = millis();   //elapsed();

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portENTER_CRITICAL_ISR(&timerMux);

  for (i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {

    timer[i].toBeCalled = TIMER_DEFCALL_DONTRUN;

    // no callback == no timer, i.e. jump over empty slots
    if (timer[i].callback != NULL) 
    {

      // is it time to process this timer ?
      // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

      if ((current_millis - timer[i].prev_millis) >= timer[i].delay) 
      {
        unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
        
        // update time
        timer[i].prev_millis += timer[i].delay * skipTimes;

        // check if the timer callback has to be executed
        if (timer[i].enabled) 
        {

          // "run forever" timers must always be executed
          if (timer[i].maxNumRuns == TIMER_RUN_FOREVER) 
          {
            timer[i].toBeCalled = TIMER_DEFCALL_RUNONLY;
          }
          // other timers get executed the specified number of times
          else if (timer[i].numRuns < timer[i].maxNumRuns) 
          {
            timer[i].toBeCalled = TIMER_DEFCALL_RUNONLY;
            timer[i].numRuns++;

            // after the last run, delete the timer
            if (timer[i].numRuns >= timer[i].maxNumRuns) 
            {
              timer[i].toBeCalled = TIMER_DEFCALL_RUNANDDEL;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].toBeCalled == TIMER_DEFCALL_DONTRUN)
      continue;

    if (timer[i].hasParam)
      (*(timer_callback_p)timer[i].callback)(timer[i].param);
    else
      (*(timer_callback)timer[i].callback)();

    if (timer[i].toBeCalled == TIMER_DEFCALL_RUNANDDEL)
      deleteTimer(i);
  }

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  portEXIT_CRITICAL_ISR(&timerMux);

}


// find the first available slot
// return -1 if none found
int ESP32_ISR_Timer::findFirstFreeSlot() 
{
  // all slots are used
  if (numTimers >= MAX_NUMBER_TIMERS) 
  {
    return -1;
  }

  // return the first slot with no callback (i.e. free)
  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback == NULL) 
    {
      return i;
    }
  }

  // no free slots found
  return -1;
}


int ESP32_ISR_Timer::setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n) 
{
  int freeTimer;

  if (numTimers < 0) 
  {
    init();
  }

  freeTimer = findFirstFreeSlot();
  if (freeTimer < 0) 
  {
    return -1;
  }

  if (f == NULL) 
  {
    return -1;
  }

  timer[freeTimer].delay = d;
  timer[freeTimer].callback = f;
  timer[freeTimer].param = p;
  timer[freeTimer].hasParam = h;
  timer[freeTimer].maxNumRuns = n;
  timer[freeTimer].enabled = true;
  timer[freeTimer].prev_millis = millis();

  numTimers++;

  return freeTimer;
}


int ESP32_ISR_Timer::setTimer(unsigned long d, timer_callback f, unsigned n) 
{
  return setupTimer(d, (void *)f, NULL, false, n);
}

int ESP32_ISR_Timer::setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n) 
{
  return setupTimer(d, (void *)f, p, true, n);
}

int ESP32_ISR_Timer::setInterval(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, TIMER_RUN_FOREVER);
}

int ESP32_ISR_Timer::setInterval(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, TIMER_RUN_FOREVER);
}

int ESP32_ISR_Timer::setTimeout(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, TIMER_RUN_ONCE);
}

int ESP32_ISR_Timer::setTimeout(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, TIMER_RUN_ONCE);
}

bool IRAM_ATTR ESP32_ISR_Timer::changeInterval(unsigned numTimer, unsigned long d) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return false;
  }

  // Updates interval of existing specified timer
  if (timer[numTimer].callback != NULL) 
  {
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    portENTER_CRITICAL(&timerMux);

    timer[numTimer].delay = d;
    timer[numTimer].prev_millis = millis();

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    portEXIT_CRITICAL(&timerMux);

    return true;
  }
  
  // false return for non-used numTimer, no callback
  return false;
}

void ESP32_ISR_Timer::deleteTimer(unsigned timerId) 
{
  if (timerId >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numTimers == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the specified slot is already empty
  if (timer[timerId].callback != NULL) 
  {
    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    portENTER_CRITICAL(&timerMux);

    memset((void*) &timer[timerId], 0, sizeof (timer_t));
    timer[timerId].prev_millis = millis();

    // update number of timers
    numTimers--;

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    portEXIT_CRITICAL(&timerMux);

  }
}

// function contributed by code@rowansimms.com
void ESP32_ISR_Timer::restartTimer(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portENTER_CRITICAL(&timerMux);

  timer[numTimer].prev_millis = millis();

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portEXIT_CRITICAL(&timerMux);
}


bool ESP32_ISR_Timer::isEnabled(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return false;
  }

  return timer[numTimer].enabled;
}


void ESP32_ISR_Timer::enable(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = true;
}


void ESP32_ISR_Timer::disable(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = false;
}

void ESP32_ISR_Timer::enableAll() 
{
  // Enable all timers with a callback assigned (used)

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portENTER_CRITICAL(&timerMux);

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == TIMER_RUN_FOREVER) 
    {
      timer[i].enabled = true;
    }
  }

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portEXIT_CRITICAL(&timerMux);
}

void ESP32_ISR_Timer::disableAll() 
{
  // Disable all timers with a callback assigned (used)

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portENTER_CRITICAL(&timerMux);

  for (uint8_t i = 0; i < MAX_NUMBER_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == TIMER_RUN_FOREVER) 
    {
      timer[i].enabled = false;
    }
  }

  // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  portEXIT_CRITICAL(&timerMux);

}

void ESP32_ISR_Timer::toggle(unsigned numTimer) 
{
  if (numTimer >= MAX_NUMBER_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = !timer[numTimer].enabled;
}


unsigned ESP32_ISR_Timer::getNumTimers() 
{
  return numTimers;
}


