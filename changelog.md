# ESP32_New_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_New_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_New_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_New_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_New_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_New_TimerInterrupt/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-ESP32_New_TimerInterrupt/count.svg" title="ESP32_New_TimerInterrupt Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-ESP32_New_TimerInterrupt/count.svg" style="height: 30px;width: 200px;"></a>

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.0](#releases-v140)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.1](#releases-v121)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.1](#releases-v101)
  * [Releases v1.0.0](#releases-v100)


---
---

## Changelog

### Releases v1.5.0

1. Fix doubled time for ESP32_C3, ESP32_S2 and ESP32_S3. Check [Error in the value defined by TIMER0_INTERVAL_MS #28](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/issues/28)
2. Modify examples to avoid using `LED_BUILTIN` / `GPIO2` as it can cause crash in some boards, such as `ESP32_C3`
3. Use `allman astyle` and add `utils`

### Releases v1.4.0

1. Suppress warnings for ESP32_C3, ESP32_S2 and ESP32_S3
2. Add support to 
  - ESP32_S2 : ESP32S2 Native USB, UM FeatherS2 Neo, UM TinyS2, UM RMP, microS2, LOLIN_S2_MINI, LOLIN_S2_PICO, ADAFRUIT_FEATHER_ESP32S2, ADAFRUIT_FEATHER_ESP32S2_TFT, ATMegaZero ESP32-S2, Deneyap Mini, FRANZININHO_WIFI, FRANZININHO_WIFI_MSC
  - ESP32_S3 : UM TinyS3, UM PROS3, UM FeatherS3, ESP32_S3_USB_OTG, ESP32S3_CAM_LCD, DFROBOT_FIREBEETLE_2_ESP32S3, ADAFRUIT_FEATHER_ESP32S3_TFT
  - ESP32_C3 : LOLIN_C3_MINI, DFROBOT_BEETLE_ESP32_C3, ADAFRUIT_QTPY_ESP32C3, AirM2M_CORE_ESP32C3, XIAO_ESP32C3

### Releases v1.3.0

1. Suppress errors and warnings for new ESP32 core v2.0.4+

### Releases v1.2.1

1. Add support to new Adafruit boards such as QTPY_ESP32S2, FEATHER_ESP32S3_NOPSRAM and QTPY_ESP32S3_NOPSRAM

### Releases v1.2.0

1. Add support to new `ESP32-S3`
2. Optimize library code by using `reference-passing` instead of `value-passing`


### Releases v1.1.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories

### Releases v1.0.1

1. Avoid using `PIN_D1 (GPIO1)` in your code due to issue with core v2.0.0 and v2.0.1. Check [ESP32 Core v2.0.1 / 2.0.1 RC1 crashes if using pinMode with GPIO1 #5868](https://github.com/espressif/arduino-esp32/issues/5868). Only OK with core v1.0.6-
2. Don't use `float` in `ISR` due to issue with core v2.0.0 and v2.0.1. Only OK with core v1.0.6-.
3. Delete Blynk-related examples
4. Add changelog.md

### Releases v1.0.0

1. Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with [ESP32 core v2.0.0-rc1+](https://github.com/espressif/arduino-esp32/releases/tag/2.0.0-rc1)


