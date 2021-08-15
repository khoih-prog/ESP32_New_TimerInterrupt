# ESP32_New_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_New_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_New_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_New_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_New_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_New_TimerInterrupt/issues)

---
---

## Table of Contents

* [Why do we need this ESP32_New_TimerInterrupt library](#why-do-we-need-this-esp32_new_timerinterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](#changelog)
  * [Releases v1.0.0](#releases-v100)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [More useful Information](#more-useful-information)
* [How to use](#how-to-use)
* [Examples](#examples)
  * [  1. Argument_None](examples/Argument_None)
  * [  2. Change_Interval](examples/Change_Interval).
  * [  3. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  4. ISR_Switch](examples/ISR_Switch)
  * [  5. ISR_Timer_Complex](examples/ISR_Timer_Complex)
  * [  6. ISR_Timer_Switch](examples/ISR_Timer_Switch)
  * [  7. ISR_Timer_4_Switches](examples/ISR_Timer_4_Switches)
  * [  8. ISR_Timer_Switches](examples/ISR_Timer_Switches)
  * [  9. RPM_Measure](examples/RPM_Measure)
  * [ 10. SwitchDebounce](examples/SwitchDebounce)
  * [ 11. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 12. ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
  * [ 13. ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_Timer_Complex on ESP32_S2_DEV](#1-isr_timer_complex-on-esp32_s2_dev)
  * [2. TimerInterruptTest on ESP32_S2_DEV](#2-timerinterrupttest-on-esp32_s2_dev)
  * [3. Change_Interval on ESP32_S2_DEV](#3-change_interval-on-esp32_s2_dev)
  * [4. Argument_None on ESP32_S2_DEV](#4-argument_none-on-esp32_s2_dev)
  * [5. ISR_16_Timers_Array_Complex on ESP32_S2_DEV](#5-isr_16_timers_array_complex-on-esp32_s2_dev)
  * [6. ISR_16_Timers_Array on ESP32_S2_DEV](#6-isr_16_timers_array-on-esp32_s2_dev)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Releases](#releases)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [ESP32_New_TimerInterrupt library](https://github.com/khoih-prog/ESP32_New_TimerInterrupt)

## Features

This library enables you to use Interrupt from Hardware Timers on an ESP32-S2-based board.

The ESP32-S2 timer Interrupt control is different from that of ESP32, at least with the ESP32-S2 core v1.0.6. While the examples of current [ESP32TimerInterrupt library](https://github.com/khoih-prog/ESP32TimerInterrupt) can be compiled OK, they can't run yet. That's why this new [ESP32_New_TimerInterrupt library](https://github.com/khoih-prog/ESP32_New_TimerInterrupt) has been created.

---

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. ESP32 boards, such as ESP32_DEV, etc.
2. ESP32_S2-based boards, such as ESP32S2_DEV, ESP32_S2 Saola, etc.
3. ESP32_C3-based boards, such as ESP32C3_DEV, etc.

---

### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

## Changelog

### Releases v1.0.0

1. Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+


---
---

## Prerequisites

1. [`Arduino IDE 1.8.15+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`ESP32 Core v2.0.0-rc1+`](https://github.com/espressif/arduino-esp32) for ESP32, ESP32_S2/C3-based boards.
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**ESP32_New_TimerInterrupt**](https://github.com/khoih-prog/ESP32_New_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_New_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_New_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**ESP32_New_TimerInterrupt**](https://github.com/khoih-prog/ESP32_New_TimerInterrupt) page.
2. Download the latest release `ESP32_New_TimerInterrupt-master.zip`.
3. Extract the zip file to `ESP32_New_TimerInterrupt-master` directory 
4. Copy whole `ESP32_New_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP32_New_TimerInterrupt** library](https://platformio.org/lib/show/xxxxx/ESP32_New_TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/xxxxx/ESP32_New_TimerInterrupt/installation). Search for **ESP32_New_TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---


### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---
---

## More useful Information

The ESP32 has two timer groups, each one with two general purpose hardware timers.  All the timers are based on 64 bits counters and 16 bit prescalers. 

The timer counters can be configured to count up or down and support automatic reload and software reload.

They can also generate alarms when they reach a specific value, defined by the software. The value of the counter can be read by 
the software program.

---

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---
---

## How to use

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1, Timer2 and Timer3` are supported for ESP32-S2.

---
---

### Examples: 

 1. [Argument_None](examples/Argument_None)
 2. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 3. [ISR_Switch](examples/ISR_Switch) 
 4. [ISR_Timer_4_Switches](examples/ISR_Timer_4_Switches) 
 5. [ISR_Timer_Complex](examples/ISR_Timer_Complex)
 6. [ISR_Timer_Switch](examples/ISR_Timer_Switch)
 7. [ISR_Timer_Switches](examples/ISR_Timer_Switches) 
 8. [RPM_Measure](examples/RPM_Measure)
 9. [SwitchDebounce](examples/SwitchDebounce)
10. [TimerInterruptTest](examples/TimerInterruptTest)
11. [**Change_Interval**](examples/Change_Interval).
12. [**ISR_16_Timers_Array**](examples/ISR_16_Timers_Array)
13. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).

---
---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

```
#if !defined( ESP32 )
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "ESP32_New_TimerInterrupt.h"
#include "ESP32_New_ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2
#endif

#ifndef LED_BLUE
  #define LED_BLUE          25
#endif

#ifndef LED_RED
  #define LED_RED           27
#endif

#define HW_TIMER_INTERVAL_US      10000L

volatile uint32_t startMillis = 0;

// Init ESP32 timer 1
ESP32Timer ITimer(1);

// Init ESP32_ISR_Timer
ESP32_ISR_Timer ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

bool IRAM_ATTR TimerHandler(void * timerNo)
{ 
  static bool toggle  = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 2000ms = 2s
  if (++timeRun == ((LED_TOGGLE_INTERVAL_MS * 1000) / HW_TIMER_INTERVAL_US) )
  {
    timeRun = 0;

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }

  return true;
}

/////////////////////////////////////////////////

#define NUMBER_ISR_TIMERS         16

typedef void (*irqCallback)  ();

/////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#if USE_COMPLEX_STRUCT

typedef struct
{
  irqCallback   irqCallbackFunc;
  uint32_t      TimerInterval;
  unsigned long deltaMillis;
  unsigned long previousMillis;
} ISRTimerData;

// In NRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomething(int index);

#else

volatile unsigned long deltaMillis    [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMillis [NUMBER_ISR_TIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
  5000L,  10000L,  15000L,  20000L,  25000L,  30000L,  35000L,  40000L,
  45000L, 50000L,  55000L,  60000L,  65000L,  70000L,  75000L,  80000L
};

void doingSomething(int index)
{
  unsigned long currentMillis  = millis();

  deltaMillis[index]    = currentMillis - previousMillis[index];
  previousMillis[index] = currentMillis;
}

#endif

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomething0()
{
  doingSomething(0);
}

void doingSomething1()
{
  doingSomething(1);
}

void doingSomething2()
{
  doingSomething(2);
}

void doingSomething3()
{
  doingSomething(3);
}

void doingSomething4()
{
  doingSomething(4);
}

void doingSomething5()
{
  doingSomething(5);
}

void doingSomething6()
{
  doingSomething(6);
}

void doingSomething7()
{
  doingSomething(7);
}

void doingSomething8()
{
  doingSomething(8);
}

void doingSomething9()
{
  doingSomething(9);
}

void doingSomething10()
{
  doingSomething(10);
}

void doingSomething11()
{
  doingSomething(11);
}

void doingSomething12()
{
  doingSomething(12);
}

void doingSomething13()
{
  doingSomething(13);
}

void doingSomething14()
{
  doingSomething(14);
}

void doingSomething15()
{
  doingSomething(15);
}

#if USE_COMPLEX_STRUCT

ISRTimerData curISRTimerData[NUMBER_ISR_TIMERS] =
{
  //irqCallbackFunc, TimerInterval, deltaMillis, previousMillis
  { doingSomething0,    5000L, 0, 0 },
  { doingSomething1,   10000L, 0, 0 },
  { doingSomething2,   15000L, 0, 0 },
  { doingSomething3,   20000L, 0, 0 },
  { doingSomething4,   25000L, 0, 0 },
  { doingSomething5,   30000L, 0, 0 },
  { doingSomething6,   35000L, 0, 0 },
  { doingSomething7,   40000L, 0, 0 },
  { doingSomething8,   45000L, 0, 0 },
  { doingSomething9,   50000L, 0, 0 },
  { doingSomething10,  55000L, 0, 0 },
  { doingSomething11,  60000L, 0, 0 },
  { doingSomething12,  65000L, 0, 0 },
  { doingSomething13,  70000L, 0, 0 },
  { doingSomething14,  75000L, 0, 0 },
  { doingSomething15,  80000L, 0, 0 }
};

void doingSomething(int index)
{
  unsigned long currentMillis  = millis();

  curISRTimerData[index].deltaMillis    = currentMillis - curISRTimerData[index].previousMillis;
  curISRTimerData[index].previousMillis = currentMillis;
}

#else

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
  doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3,
  doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7,
  doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
  doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

#endif
///////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;

  unsigned long currMillis = millis();

  Serial.print(F("SimpleTimer : ")); Serial.print(SIMPLE_TIMER_MS / 1000);
  Serial.print(F(", ms : ")); Serial.print(currMillis);
  Serial.print(F(", Dms : ")); Serial.println(currMillis - previousMillis);

  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(curISRTimerData[i].TimerInterval);
    Serial.print(F(", actual : ")); Serial.println(curISRTimerData[i].deltaMillis);
#else
    Serial.print(F("Timer : ")); Serial.print(i);
    Serial.print(F(", programmed : ")); Serial.print(TimerInterval[i]);
    Serial.print(F(", actual : ")); Serial.println(deltaMillis[i]);
#endif
  }

  previousMillis = currMillis;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_16_Timers_Array_Complex on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_NEW_TIMERINTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(startMillis);
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  startMillis = millis();

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (uint16_t i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISRTimerData[i].previousMillis = startMillis;
    ISR_Timer.setInterval(curISRTimerData[i].TimerInterval, curISRTimerData[i].irqCallbackFunc);
#else
    previousMillis[i] = millis();
    ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]);
#endif
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

### 1. ISR_Timer_Complex on ESP32_S2_DEV

The following is the sample terminal output when running example [ISR_Timer_Complex](examples/ISR_Timer_Complex) to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 3.435s !!!**

```
Starting ISR_Timer_Complex on ESP32S2_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 240 MHz
Starting ITimer OK, millis() = 3015
[4415] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on ESP32

[4450] Ready (ping: 4ms).
Blynk connected
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 6508
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
11s: D ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 5005
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 5006
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 5005
2s: D ms = 2000
11s: D ms = 11000
2s: D ms = 2000
5s: D ms = 5000
2s: D ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 5006
2s: D ms = 2000
2s: D ms = 2000
5s: D ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 5005
2s: D ms = 2000
11s: D ms = 11000
```

---

### 2. TimerInterruptTest on ESP32C3_DEV

The following is the sample terminal output when running example [TimerInterruptTest](examples/TimerInterruptTest) to demonstrate how to start/stop Hardware Timers.

```
Starting TimerInterruptTest on ESP32C3_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 160 MHz
[TISR] _timerIndex = 0 , _timerGroup = 0
[TISR] _count = 0 - 1000000
[TISR] timer_set_alarm_value = 1000000.00
ITimer0 called, millis() = 307
Starting  ITimer0 OK, millis() = 310
[TISR] _timerIndex = 0 , _timerGroup = 1
[TISR] _count = 0 - 3000000
[TISR] timer_set_alarm_value = 3000000.00
ITimer1 called, millis() = 333
Starting  ITimer1 OK, millis() = 337
ITimer0 called, millis() = 1305
ITimer0 called, millis() = 2305
ITimer0 called, millis() = 3305
ITimer1 called, millis() = 3331
ITimer0 called, millis() = 4305
Stop ITimer0, millis() = 5001
ITimer1 called, millis() = 6331
ITimer1 called, millis() = 9331
Start ITimer0, millis() = 10002
ITimer0 called, millis() = 11002
ITimer0 called, millis() = 12002
ITimer1 called, millis() = 12331
ITimer0 called, millis() = 13002
ITimer0 called, millis() = 14002
Stop ITimer1, millis() = 15001
ITimer0 called, millis() = 15002
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
ITimer0 called, millis() = 21004
ITimer0 called, millis() = 22004
ITimer0 called, millis() = 23004
ITimer0 called, millis() = 24004
ITimer0 called, millis() = 25004
Stop ITimer0, millis() = 25005
```

---


### 3. Change_Interval on ESP32_DEV

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on ESP32_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 240 MHz
ITimer0: millis() = 129
Starting  ITimer0 OK, millis() = 132
ITimer1: millis() = 134
Starting  ITimer1 OK, millis() = 137
ITimer0: millis() = 2129
ITimer0: millis() = 4129
ITimer1: millis() = 5132
ITimer0: millis() = 6129
ITimer0: millis() = 8129
Time = 10001, Timer0Count = 5, Timer1Count = 2
ITimer0: millis() = 10129
ITimer1: millis() = 10132
ITimer0: millis() = 12129
ITimer0: millis() = 14129
ITimer1: millis() = 15132
ITimer0: millis() = 16129
ITimer0: millis() = 18129
Time = 20002, Timer0Count = 10, Timer1Count = 4
Changing Interval, Timer0 = 4000,  Timer1 = 10000
ITimer0: millis() = 24002
ITimer0: millis() = 28002
ITimer1: millis() = 30002
Time = 30003, Timer0Count = 12, Timer1Count = 5
ITimer0: millis() = 32002
ITimer0: millis() = 36002
ITimer0: millis() = 40002
ITimer1: millis() = 40002
Time = 40004, Timer0Count = 15, Timer1Count = 6
Changing Interval, Timer0 = 2000,  Timer1 = 5000

```

---

### 4. Argument_None on ESP32_S2_DEV

The following is the sample terminal output when running example [Argument_None](examples/Argument_None)

```
Starting Argument_None on ESP32S2_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 240 MHz
[TISR] ESP32_S2_TimerInterrupt: _timerNo = 0 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 0 , _timerGroup = 0
[TISR] _count = 0 - 1000000
[TISR] timer_set_alarm_value = 1000000.00
ITimer0: millis() = 656
Starting  ITimer0 OK, millis() = 660
[TISR] ESP32_S2_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
[TISR] _timerIndex = 1 , _timerGroup = 0
[TISR] _count = 0 - 5000000
[TISR] timer_set_alarm_value = 5000000.00
ITimer1: millis() = 682
Starting  ITimer1 OK, millis() = 685
ITimer0: millis() = 1651
ITimer0: millis() = 2651
ITimer0: millis() = 3651
ITimer0: millis() = 4651
ITimer0: millis() = 5651
ITimer1: millis() = 5676
ITimer0: millis() = 6651
ITimer0: millis() = 7651
ITimer0: millis() = 8651
ITimer0: millis() = 9651
ITimer0: millis() = 10651
ITimer1: millis() = 10676
ITimer0: millis() = 11651
ITimer0: millis() = 12651
ITimer0: millis() = 13651
ITimer0: millis() = 14651
ITimer0: millis() = 15651
ITimer1: millis() = 15676
ITimer0: millis() = 16651
ITimer0: millis() = 17651
ITimer0: millis() = 18651
ITimer0: millis() = 19651
ITimer0: millis() = 20651
ITimer1: millis() = 20676
ITimer0: millis() = 21651
ITimer0: millis() = 22651
```

---

### 5. ISR_16_Timers_Array_Complex on ESP32C3_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **ESP32C3_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!

```
Starting ISR_16_Timers_Array_Complex on ESP32C3_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 160 MHz
Starting ITimer OK, millis() = 2187
SimpleTimer : 2, ms : 12193, Dms : 10004
Timer : 0, programmed : 5000, actual : 5008
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 22251, Dms : 10058
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15008
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 32310, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25008
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 42369, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25008
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35008
Timer : 7, programmed : 40000, actual : 40008
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 52429, Dms : 10060
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30008
Timer : 6, programmed : 35000, actual : 35008
Timer : 7, programmed : 40000, actual : 40008
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 62490, Dms : 10061
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35008
Timer : 7, programmed : 40000, actual : 40008
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60008
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 72551, Dms : 10061
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40008
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60008
Timer : 12, programmed : 65000, actual : 65008
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 82613, Dms : 10062
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50008
Timer : 10, programmed : 55000, actual : 55008
Timer : 11, programmed : 60000, actual : 60008
Timer : 12, programmed : 65000, actual : 65008
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 75008
Timer : 15, programmed : 80000, actual : 80008

```

---

### 6. ISR_16_Timers_Array on ESP32_S2_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) on **ESP32S2_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!


```
Starting ISR_16_Timers_Array on ESP32S2_DEV
ESP32_New_TimerInterrupt v1.0.0
CPU Frequency = 240 MHz
Starting ITimer OK, millis() = 2538
1s: Delta ms = 1002, ms = 3540
1s: Delta ms = 999, ms = 4539
2s: Delta ms = 2002, ms = 4540
1s: Delta ms = 1000, ms = 5539
3s: Delta ms = 3002, ms = 5540
1s: Delta ms = 1000, ms = 6539
2s: Delta ms = 2000, ms = 6540
4s: Delta ms = 4002, ms = 6540
1s: Delta ms = 1000, ms = 7539
5s: Delta ms = 5002, ms = 7540
1s: Delta ms = 1000, ms = 8539
2s: Delta ms = 2000, ms = 8540
3s: Delta ms = 3000, ms = 8540
6s: Delta ms = 6002, ms = 8540
1s: Delta ms = 1000, ms = 9539
7s: Delta ms = 7002, ms = 9540
1s: Delta ms = 1000, ms = 10539
2s: Delta ms = 2000, ms = 10540
4s: Delta ms = 4000, ms = 10540
8s: Delta ms = 8002, ms = 10540
1s: Delta ms = 1000, ms = 11539
3s: Delta ms = 3000, ms = 11540
9s: Delta ms = 9002, ms = 11540
1s: Delta ms = 1000, ms = 12539
2s: Delta ms = 2000, ms = 12540
5s: Delta ms = 5000, ms = 12540
10s: Delta ms = 10002, ms = 12540
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10008
1s: Delta ms = 1000, ms = 13539
11s: Delta ms = 11002, ms = 13540
1s: Delta ms = 1000, ms = 14539
2s: Delta ms = 2000, ms = 14540
3s: Delta ms = 3000, ms = 14540
4s: Delta ms = 4000, ms = 14540
6s: Delta ms = 6000, ms = 14540
12s: Delta ms = 12005, ms = 14543
1s: Delta ms = 1000, ms = 15539
13s: Delta ms = 13001, ms = 15539
1s: Delta ms = 1000, ms = 16539
2s: Delta ms = 1999, ms = 16539
7s: Delta ms = 6999, ms = 16539
14s: Delta ms = 14001, ms = 16539
1s: Delta ms = 1000, ms = 17539
3s: Delta ms = 2999, ms = 17539
5s: Delta ms = 4999, ms = 17539
15s: Delta ms = 15001, ms = 17539
1s: Delta ms = 1000, ms = 18539
2s: Delta ms = 2000, ms = 18539
4s: Delta ms = 3999, ms = 18539
8s: Delta ms = 7999, ms = 18539
16s: Delta ms = 16001, ms = 18539
1s: Delta ms = 1000, ms = 19539
1s: Delta ms = 1000, ms = 20539
2s: Delta ms = 2000, ms = 20539
3s: Delta ms = 3000, ms = 20539
6s: Delta ms = 5999, ms = 20539
9s: Delta ms = 9000, ms = 20540
1s: Delta ms = 1001, ms = 21540
1s: Delta ms = 1000, ms = 22540
2s: Delta ms = 2001, ms = 22540
4s: Delta ms = 4001, ms = 22540
5s: Delta ms = 5001, ms = 22540
10s: Delta ms = 10000, ms = 22540
simpleTimerDoingSomething2s: Delta programmed ms = 2000, actual = 10005
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level (_TIMERINTERRUPT_LOGLEVEL_) from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "ESP32_New_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

## Releases

### Releases v1.0.0

1. Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
---
---

### Issues

Submit issues to: [ESP32_New_TimerInterrupt issues](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/issues)

---

## TO DO

1. Search for bug and improvement.



## DONE

1. Basic hardware timers for ESP32, ESP32-S2 and ESP32_C3 for ESP32 core v2.0.0-rc1+.
2. More hardware-initiated software-enabled timers
3. Longer time interval
4. Similar features for remaining Arduino boards such as SAMD21, SAMD51, SAM-DUE, nRF52, ESP8266, STM32, etc.
5. Fix compiler errors due to conflict to some libraries.
6. Add complex examples.

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


