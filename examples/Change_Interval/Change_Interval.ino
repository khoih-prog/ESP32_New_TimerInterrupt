/****************************************************************************************************************************
  Change_Interval.ino
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

/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/

#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     1

#include "ESP32_New_TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define PIN_D1              1         // Pin D1 mapped to pin GPIO1 of ESP32

volatile uint32_t Timer0Count = 0;
volatile uint32_t Timer1Count = 0;

bool IRAM_ATTR TimerHandler0(void * timerNo)
{ 
  static bool toggle0 = false;

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  Timer0Count++;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer0: millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;

  return true;
}

bool IRAM_ATTR TimerHandler1(void * timerNo)
{  
  static bool toggle1 = false;

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  Timer1Count++;

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer1: millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles PIN_D1
  digitalWrite(PIN_D1, toggle1);
  toggle1 = !toggle1;

  return true;
}

void printResult(uint32_t currTime)
{
  Serial.print(F("Time = ")); Serial.print(currTime); 
  Serial.print(F(", Timer0Count = ")); Serial.print(Timer0Count);
  Serial.print(F(", Timer1Count = ")); Serial.println(Timer1Count);
}

#define TIMER0_INTERVAL_MS        2000

#define TIMER1_INTERVAL_MS        5000

// Init ESP32 timer 0
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

void setup()
{
  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(PIN_D1,       OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  delay(100);

  Serial.print(F("\nStarting Change_Interval on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_NEW_TIMERINTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.print(F("Starting  ITimer0 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

#define CHECK_INTERVAL_MS     10000L
#define CHANGE_INTERVAL_MS    20000L

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t lastChangeTime = 0;
  static uint32_t currTime;
  static uint32_t multFactor = 0;

  currTime = millis();

  if (currTime - lastTime > CHECK_INTERVAL_MS)
  {
    printResult(currTime);
    lastTime = currTime;

    if (currTime - lastChangeTime > CHANGE_INTERVAL_MS)
    {
      //setInterval(unsigned long interval, timerCallback callback)
      multFactor = (multFactor + 1) % 2;
      
      ITimer0.setInterval(TIMER0_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler0);
      ITimer1.setInterval(TIMER1_INTERVAL_MS * 1000 * (multFactor + 1), TimerHandler1);

      Serial.print(F("Changing Interval, Timer0 = ")); Serial.print(TIMER0_INTERVAL_MS * (multFactor + 1));
      Serial.print(F(",  Timer1 = ")); Serial.println(TIMER1_INTERVAL_MS * (multFactor + 1)); 

      lastChangeTime = currTime;
    }
  }
}
