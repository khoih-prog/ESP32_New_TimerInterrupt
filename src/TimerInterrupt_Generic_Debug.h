/****************************************************************************************************************************
  TimerInterrupt_Generic_Debug.h
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

#ifndef TIMERINTERRUPT_GENERIC_DEBUG_H
#define TIMERINTERRUPT_GENERIC_DEBUG_H

#ifdef TIMERINTERRUPT_DEBUG_PORT
  #define TISR_DBG_PORT      TIMERINTERRUPT_DEBUG_PORT
#else
  #define TISR_DBG_PORT      Serial
#endif

// Change _TIMERINTERRUPT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _TIMERINTERRUPT_LOGLEVEL_
  #define _TIMERINTERRUPT_LOGLEVEL_       1
#endif

/////////////////////////////////////////////////////////

const char TISR_MARK[] = "[TISR] ";

#define TISR_PRINT_MARK   TISR_PRINT(TISR_MARK)
#define TISR_PRINT_SP     TISR_DBG_PORT.print(" ")

#define TISR_PRINT        TISR_DBG_PORT.print
#define TISR_PRINTLN      TISR_DBG_PORT.println

/////////////////////////////////////////////////////////

#define TISR_LOGERROR(x)         if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGERROR0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT(x); }
#define TISR_LOGERROR1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGERROR2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGERROR3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGWARN(x)          if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGWARN0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT(x); }
#define TISR_LOGWARN1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGWARN2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGWARN3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGINFO(x)          if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGINFO0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT(x); }
#define TISR_LOGINFO1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGINFO2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGINFO3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define TISR_LOGDEBUG(x)         if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINTLN(x); }
#define TISR_LOGDEBUG0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT(x); }
#define TISR_LOGDEBUG1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINTLN(y); }
#define TISR_LOGDEBUG2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINTLN(z); }
#define TISR_LOGDEBUG3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_PRINT_MARK; TISR_PRINT(x); TISR_PRINT_SP; TISR_PRINT(y); TISR_PRINT_SP; TISR_PRINT(z); TISR_PRINT_SP; TISR_PRINTLN(w); }

/////////////////////////////////////////////////////////


#endif    //TIMERINTERRUPT_GENERIC_DEBUG_H
