# ESP32_New_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_New_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_New_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_New_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_New_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_New_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_New_TimerInterrupt/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.0.1](#releases-v101)
  * [Releases v1.0.0](#releases-v100)


---
---

## Changelog

### Releases v1.0.1

1. Avoid using `PIN_D1 (GPIO1)` in your code due to issue with core v2.0.0 and v2.0.1. Check [ESP32 Core v2.0.1 / 2.0.1 RC1 crashes if using pinMode with GPIO1 #5868](https://github.com/espressif/arduino-esp32/issues/5868). Only OK with core v1.0.6-
2. Don't use `float` in `ISR` due to issue with core v2.0.0 and v2.0.1. Only OK with core v1.0.6-.
3. Delete Blynk-related examples
4. Add changelog.md

### Releases v1.0.0

1. Initial coding for ESP32, ESP32_S2, ESP32_C3 boards with [ESP32 core v2.0.0-rc1+](https://github.com/espressif/arduino-esp32/releases/tag/2.0.0-rc1)

