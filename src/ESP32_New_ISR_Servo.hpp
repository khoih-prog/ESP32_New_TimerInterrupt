/****************************************************************************************************************************
  ESP32_New_ISR_Servo.hpp
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

#ifndef ESP32_New_ISR_Servo_HPP
#define ESP32_New_ISR_Servo_HPP

#if ( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_ESP32S2_THING_PLUS || ARDUINO_MICROS2 || \
        ARDUINO_METRO_ESP32S2 || ARDUINO_MAGTAG29_ESP32S2 || ARDUINO_FUNHOUSE_ESP32S2 || \
        ARDUINO_ADAFRUIT_FEATHER_ESP32S2_NOPSRAM || ARDUINO_ADAFRUIT_QTPY_ESP32S2)
  #warning Using ESP32_S2-based board
  #define USING_ESP32_S2_ISR_SERVO         true
#elif ( defined(ARDUINO_ESP32S3_DEV) || defined(ARDUINO_ESP32_S3_BOX) || defined(ARDUINO_TINYS3) || \
        defined(ARDUINO_PROS3) || defined(ARDUINO_FEATHERS3) || defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3_NOPSRAM) || \
        defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3_NOPSRAM))
  #warning Using ESP32_S3-based board      
  #define USING_ESP32_S3_ISR_SERVO         true    
#elif ( ARDUINO_ESP32C3_DEV )
  #warning Using ESP32_C3-based board
  #define USING_ESP32_C3_ISR_SERVO         true
#elif defined(ESP32)
  #warning Using ESP32-based board
  #define USING_ESP32_ISR_SERVO            true
#else
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.  
#endif

#ifndef ESP32_NEW_ISR_SERVO_VERSION
  #define ESP32_NEW_ISR_SERVO_VERSION           "ESP32_New_ISR_Servo v1.3.0"
  
  #define ESP32_NEW_ISR_SERVO_VERSION_MAJOR     1
  #define ESP32_NEW_ISR_SERVO_VERSION_MINOR     3
  #define ESP32_NEW_ISR_SERVO_VERSION_PATCH     0

  #define ESP32_NEW_ISR_SERVO_VERSION_INT       1003000
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

#include "ESP32_New_ISR_Servo_Debug.h"
#include "ESP32_New_FastTimerInterrupt.h"

#if defined(NUM_DIGITAL_PINS)
  #if (ISR_SERVO_DEBUG > 1)
    #warning Using default NUM_DIGITAL_PINS
  #endif
  #define ESP32_MAX_PIN           NUM_DIGITAL_PINS
#else
  #if USING_ESP32_S2_ISR_SERVO
    #define ESP32_MAX_PIN         48
  #elif USING_ESP32_S3_ISR_SERVO
    #define ESP32_MAX_PIN         48
  #elif USING_ESP32_C3_ISR_SERVO
    #define ESP32_MAX_PIN         22
  #else
    #define ESP32_MAX_PIN         40
  #endif
#endif

#define ESP32_WRONG_PIN         255

// From Servo.h - Copyright (c) 2009 Michael Margolis.  All right reserved.

#define MIN_PULSE_WIDTH         544       // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH         2400      // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH     1500      // default pulse width when servo is attached
#define REFRESH_INTERVAL        20000     // minumim time to refresh servos in microseconds 

extern bool IRAM_ATTR ESP32_ISR_Servo_Handler(void * timerNo);

class ESP32_ISR_Servo
{

  public:
    // maximum number of servos
    const static uint8_t MAX_SERVOS = 16;

    // constructor
    ESP32_ISR_Servo();

    // destructor
    ~ESP32_ISR_Servo()
    {
      if (ESP32_ITimer)
      {
        ESP32_ITimer->detachInterrupt();
        delete ESP32_ITimer;
      }
    }

    void IRAM_ATTR run();

    // useTimer select which timer (0-3) of ESP32 to use for Servos
    //Return true if timerN0 in range
    bool useTimer(const uint8_t& timerNo)
    {
      if (timerNo < MAX_ESP32_NUM_TIMERS)
      {
        _timerNo = timerNo;
        return true;
      }
      
      return false;
    }

    // Bind servo to the timer and pin, return servoIndex
    int8_t setupServo(const uint8_t& pin, const uint16_t& min = MIN_PULSE_WIDTH, const uint16_t& max = MAX_PULSE_WIDTH);

    // setPosition will set servo to position in degrees
    // by using PWM, turn HIGH 'duration' microseconds within REFRESH_INTERVAL (20000us)
    // returns true on success or false on wrong servoIndex
    bool setPosition(const uint8_t& servoIndex, const float& position);

    // returns last position in degrees if success, or -1 on wrong servoIndex
    float getPosition(const uint8_t& servoIndex);

    // setPulseWidth will set servo PWM Pulse Width in microseconds, correcponding to certain position in degrees
    // by using PWM, turn HIGH 'pulseWidth' microseconds within REFRESH_INTERVAL (20000us)
    // min and max for each individual servo are enforced
    // returns true on success or false on wrong servoIndex
    bool setPulseWidth(const uint8_t& servoIndex, uint32_t& pulseWidth);

    // returns pulseWidth in microsecs (within min/max range) if success, or 0 on wrong servoIndex
    uint32_t getPulseWidth(const uint8_t& servoIndex);

    // destroy the specified servo
    void deleteServo(const uint8_t& servoIndex);

    // returns true if the specified servo is enabled
    bool isEnabled(const uint8_t& servoIndex);

    // enables the specified servo
    bool enable(const uint8_t& servoIndex);

    // disables the specified servo
    bool disable(const uint8_t& servoIndex);

    // enables all servos
    void enableAll();

    // disables all servos
    void disableAll();

    // enables the specified servo if it's currently disabled,
    // and vice-versa
    bool toggle(const uint8_t& servoIndex);

    // returns the number of used servos
    int8_t getNumServos();

    // returns the number of available servos
    int8_t getNumAvailableServos() 
    {
      if (numServos <= 0)
        return MAX_SERVOS;
      else 
        return MAX_SERVOS - numServos;
    }

  private:

    // Use 10 microsecs timer => not working from core v2.0.1+
    // Use 12 microsecs timer now, just fine enough to control Servo, normally requiring pulse width (PWM) 500-2000us in 20ms.
#define TIMER_INTERVAL_MICRO        12

    void init()
    {
      ESP32_ITimer = new ESP32FastTimer(_timerNo);

      // Interval in microsecs
      if ( ESP32_ITimer && ESP32_ITimer->attachInterruptInterval(TIMER_INTERVAL_MICRO, (esp32_timer_callback) ESP32_ISR_Servo_Handler ) )
      {
        ISR_SERVO_LOGERROR("Starting  ITimer OK");
      }
      else
      {
        ISR_SERVO_LOGERROR("Fail setup ESP32_ITimer");      }

      for (int servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
      {
        memset((void*) &servo[servoIndex], 0, sizeof (servo_t));
        servo[servoIndex].count    = 0;
        servo[servoIndex].enabled  = false;
        // Intentional bad pin
        servo[servoIndex].pin      = ESP32_WRONG_PIN;
      }

      numServos   = 0;

      // Init timerCount
      timerCount  = 1;

      timerMux = portMUX_INITIALIZER_UNLOCKED;
    }

    // find the first available slot
    int8_t findFirstFreeSlot();

    typedef struct
    {
      uint8_t       pin;                  // pin servo connected to
      uint32_t      count;                // In microsecs
      float         position;             // In degrees
      bool          enabled;              // true if enabled
      uint16_t      min;
      uint16_t      max;
    } servo_t;

    volatile servo_t servo[MAX_SERVOS];

    // actual number of servos in use (-1 means uninitialized)
    volatile int8_t numServos;

    // timerCount starts at 1, and counting up to (REFRESH_INTERVAL / TIMER_INTERVAL_MICRO) = (20000 / 10) = 2000
    // then reset to 1. Use this to calculate when to turn ON / OFF pulse to servo
    // For example, servo1 uses pulse width 1000us => turned ON when timerCount = 1, turned OFF when timerCount = 1000 / TIMER_INTERVAL_MICRO = 100
    volatile uint32_t timerCount;

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

    // For ESP32 timer
    uint8_t _timerNo;
    ESP32FastTimer* ESP32_ITimer;
};


#endif      // ESP32_New_ISR_Servo_HPP
