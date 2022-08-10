/****************************************************************************************************************************
  ESP32_New_ISR_Servo_Debug.h
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.0+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_New_ISR_Servo
  Licensed under MIT license

  The ESP32, ESP32_S2, ESP32_S3, ESP32_C3 have two timer groups, TIMER_GROUP_0 and TIMER_GROUP_1
  1) each group of ESP32, ESP32_S2, ESP32_S3 has two general purpose hardware timers, TIMER_0 and TIMER_1
  2) each group of ESP32_C3 has ony one general purpose hardware timer, TIMER_0
  
  All the timers are based on 64-bit counters (except 54-bit counter for ESP32_S3 counter) and 16 bit prescalers. 
  The timer counters can be configured to count up or down and support automatic reload and software reload. 
  They can also generate alarms when they reach a specific value, defined by the software. 
  The value of the counter can be read by the software program.

  Now these new 16 ISR-based PWM servo contro uses only 1 hardware timer.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.3.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      15/08/2021 Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
  1.1.0   K Hoang      12/02/2022 Add support to new ESP32-S3. Convert to h-only library. Optimize code
  1.2.0   K Hoang      08/05/2022 Fix issue with core v2.0.1+
  1.2.1   K Hoang      16/06/2022 Add support to new Adafruit boards
  1.3.0   K Hoang      03/08/2022 Suppress errors and warnings for new ESP32 core
 *****************************************************************************************************************************/

#pragma once

#ifndef ESP32_New_ISR_Servo_Debug_h
#define ESP32_New_ISR_Servo_Debug_h

//////////////////////////////////////////

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG         1
#endif

#ifndef ISR_SERVO_DEBUG
  #define ISR_SERVO_DEBUG               1
#endif

//////////////////////////////////////////

#if !defined(ISR_SERVO_DEBUG_OUTPUT)
  #define ISR_SERVO_DEBUG_OUTPUT    Serial
#endif

//////////////////////////////////////////////////////

const char ISR_SERVO_MARK[]  = "[ISR_SERVO] ";
const char ISR_SERVO_SP[]    = " ";

#define ISR_SERVO_PRINT          ISR_SERVO_DEBUG_OUTPUT.print
#define ISR_SERVO_PRINTLN        ISR_SERVO_DEBUG_OUTPUT.println
#define ISR_SERVO_FLUSH          ISR_SERVO_DEBUG_OUTPUT.flush

#define ISR_SERVO_PRINT_MARK     ISR_SERVO_PRINT(ISR_SERVO_MARK)
#define ISR_SERVO_PRINT_SP       ISR_SERVO_PRINT(ISR_SERVO_SP)

//////////////////////////////////////////////////////

#define ISR_SERVO_LOGERROR(x)         if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGERROR0(x)        if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_LOGERROR1(x,y)      if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_LOGERROR2(x,y,z)    if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGERROR3(x,y,z,w)  if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }


///////////////////////////////////////

#define ISR_SERVO_LOGWARN(x)          if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGWARN0(x)         if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_LOGWARNLN0(x)       if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGWARN1(x,y)       if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_LOGWARN2(x,y,z)     if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGWARN3(x,y,z,w)   if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }

///////////////////////////////////////

#define ISR_SERVO_LOGINFO(x)          if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGINFO0(x)         if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_LOGINFOLN0(x)       if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGINFO1(x,y)       if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_LOGINFO2(x,y,z)     if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGINFO3(x,y,z,w)   if(ISR_SERVO_DEBUG>2) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }

//////////////////////////////////////////////////////

#define ISR_SERVO_LOGDEBUG(x)         if(ISR_SERVO_DEBUG>3) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_LOGDEBUG0(x)        if(ISR_SERVO_DEBUG>3) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_LOGDEBUG1(x,y)      if(ISR_SERVO_DEBUG>3) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_LOGDEBUG2(x,y,z)    if(ISR_SERVO_DEBUG>3) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGDEBUG3(x,y,z,w)  if(ISR_SERVO_DEBUG>3) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }

//////////////////////////////////////////


#endif      // ESP32_New_ISR_Servo_Debug_h
