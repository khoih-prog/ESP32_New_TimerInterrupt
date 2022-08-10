/****************************************************************************************************************************
  multiFileProject.ino
  
  For ESP32, ESP32_S2, ESP32_C3 boards with ESP32 core v2.0.0-rc1+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_New_ISR_Servo
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !defined(ESP32)
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define ESP32_NEW_ISR_SERVO_VERSION_MIN_TARGET      "ESP32_New_ISR_Servo v1.3.0"
#define ESP32_NEW_ISR_SERVO_VERSION_MIN             1003000

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32_New_ISR_Servo.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(ESP32_NEW_ISR_SERVO_VERSION);

#if defined(ESP32_NEW_ISR_SERVO_VERSION_MIN)
  if (ESP32_NEW_ISR_SERVO_VERSION_INT < ESP32_NEW_ISR_SERVO_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(ESP32_NEW_ISR_SERVO_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
