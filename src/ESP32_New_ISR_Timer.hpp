/****************************************************************************************************************************
  ESP32_New_ISR_Timer.hpp
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_New_TimerInterrupt
  Licensed under MIT license

  The ESP32, ESP32_S2, ESP32_S3, ESP32_C3 have two timer groups, TIMER_GROUP_0 and TIMER_GROUP_1
  1) each group of ESP32, ESP32_S2, ESP32_S3 has two general purpose hardware timers, TIMER_0 and TIMER_1
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

  Version: 1.5.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      15/08/2021 Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
  1.0.1   K.Hoang      14/11/2021 Avoid using float and D1 in examples due to issue with core v2.0.0 and v2.0.1
  1.1.0   K.Hoang      18/01/2022 Fix `multiple-definitions` linker error.
  1.2.0   K Hoang      12/02/2022 Add support to new ESP32-S3
  1.2.1   K Hoang      16/06/2022 Add support to new Adafruit boards
  1.3.0   K Hoang      03/08/2022 Suppress errors and warnings for new ESP32 core
  1.4.0   K Hoang      11/08/2022 Add support and suppress warnings for ESP32_C3, ESP32_S2 and ESP32_S3 boards
  1.5.0   K Hoang      16/11/2022 Fix doubled time for ESP32_C3, ESP32_S2 and ESP32_S3
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_GENERIC_HPP
#define ISR_TIMER_GENERIC_HPP

#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#ifndef ESP32_NEW_TIMERINTERRUPT_VERSION
  #define ESP32_NEW_TIMERINTERRUPT_VERSION          "ESP32_New_TimerInterrupt v1.5.0"
  
  #define ESP32_NEW_TIMERINTERRUPT_VERSION_MAJOR     1
  #define ESP32_NEW_TIMERINTERRUPT_VERSION_MINOR     5
  #define ESP32_NEW_TIMERINTERRUPT_VERSION_PATCH     0

  #define ESP32_NEW_TIMERINTERRUPT_VERSION_INT      1005000
#endif

#include "TimerInterrupt_Generic_Debug.h"

#define CONFIG_ESP32_APPTRACE_ENABLE

#if 0
  #ifndef configMINIMAL_STACK_SIZE
    #define configMINIMAL_STACK_SIZE    2048
  #else
    #undef configMINIMAL_STACK_SIZE
    #define configMINIMAL_STACK_SIZE    2048
  #endif
#endif

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#define ESP32_ISR_Timer ESP32_ISRTimer

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

class ESP32_ISR_Timer 
{

  public:
    // maximum number of timers
#define MAX_NUMBER_TIMERS        16
#define TIMER_RUN_FOREVER       0
#define TIMER_RUN_ONCE          1

    // constructor
    ESP32_ISR_Timer();

    void init();

    // this function must be called inside loop()
    void IRAM_ATTR run();

    // Timer will call function 'callback' every 'delay' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setInterval(const unsigned long& delay, timer_callback callback);

    // Timer will call function 'callback' with parameter 'param' every 'delay' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setInterval(const unsigned long& delay, timer_callback_p callback, void* param);

    // Timer will call function 'callback' after 'delay' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setTimeout(const unsigned long& delay, timer_callback callback);

    // Timer will call function 'callback' with parameter 'param' after 'delay' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setTimeout(const unsigned long& delay, timer_callback_p callback, void* param);

    // Timer will call function 'callback' every 'delay' milliseconds 'numRuns' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setTimer(const unsigned long& delay, timer_callback callback, const uint32_t& numRuns);

    // Timer will call function 'callback' with parameter 'param' every 'delay' milliseconds 'numRuns' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (callback == NULL) or no free timers
    int setTimer(const unsigned long& delay, timer_callback_p callback, void* param, const uint32_t& numRuns);

    // updates interval of the specified timer
    bool changeInterval(const uint8_t& numTimer, const unsigned long& delay);

    // destroy the specified timer
    void deleteTimer(const uint8_t& numTimer);

    // restart the specified timer
    void restartTimer(const uint8_t& numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(const uint8_t& numTimer);

    // enables the specified timer
    void enable(const uint8_t& numTimer);

    // disables the specified timer
    void disable(const uint8_t& numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled, and vice-versa
    void toggle(const uint8_t& numTimer);

    // returns the number of used timers
    int8_t getNumTimers();

    // returns the number of available timers
    uint8_t getNumAvailableTimers() __attribute__((always_inline))
    {
      if (numTimers <= 0)
        return MAX_NUMBER_TIMERS;
      else 
        return MAX_NUMBER_TIMERS - numTimers;
    };

  private:
    // deferred call constants
#define TIMER_DEFCALL_DONTRUN   0       // don't call the callback function
#define TIMER_DEFCALL_RUNONLY   1       // call the callback function but don't delete the timer
#define TIMER_DEFCALL_RUNANDDEL 2       // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setupTimer(const unsigned long& delay, void* callback, void* param, bool hasParam, const uint32_t& numRuns) ;

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct 
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void*         callback;           // pointer to the callback function
      void*         param;              // function parameter
      bool          hasParam;           // true if callback takes a parameter
      unsigned long delay;              // delay value
      uint32_t      maxNumRuns;         // number of runs to be executed
      uint32_t      numRuns;            // number of executed runs
      bool          enabled;            // true if enabled
      unsigned      toBeCalled;         // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[MAX_NUMBER_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int8_t numTimers;

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
};

#endif    // ISR_TIMER_GENERIC_HPP


