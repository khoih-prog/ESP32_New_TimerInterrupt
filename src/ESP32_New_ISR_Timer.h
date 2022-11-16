/****************************************************************************************************************************
  ESP32_New_ISR_Timer.h
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

#ifndef ISR_TIMER_GENERIC_H
#define ISR_TIMER_GENERIC_H

#include "ESP32_New_ISR_Timer.hpp"
#include "ESP32_New_ISR_Timer-Impl.h"


#endif    // ISR_TIMER_GENERIC_H


