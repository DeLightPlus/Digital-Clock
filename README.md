# ⏰ OLED Clock Display with DS3231 RTC + Temperature + LED Breathing Animation

A clean, modular digital desktop clock built on the **Arduino Nano**.

---

## 🧩 Modules

- 🖥 **SSD1306 OLED** – I²C display shows time and alternates header between date and temperature  
- ⏱ **DS3231 RTC** – Precise real-time clock with onboard temperature sensor  
- 🔊 **DFPlayer Mini** – Startup sound and audio feedback (pins 8 = RX, 9 = TX)  
- 💡 **NeoPixel LED strip** – Smooth cyan breathing animation (5 LEDs on D6)

---

## ⚙️ Electrical Connection (5 V Safe)

| Module           | Pin on Nano            | Notes                     |
|------------------|------------------------|---------------------------|
| OLED SSD1306     | SDA → A4 / SCL → A5    | I²C address: `0x3C`       |
| DS3231 RTC       | SDA → A4 / SCL → A5    | Shared I²C bus            |
| NeoPixel LED     | D6                     | 5 LEDs, 5 V, common GND   |
| DFPlayer Mini    | RX → 9 / TX → 8        | Uses AltSoftSerial        |

> 💡 All I²C devices share the same SDA/SCL lines — no conflict.

---

## 🧠 Features

- Big centered time `HH:MM` with **blinking colon**
- Seconds displayed in small font (bottom-right)
- **Top-right header alternates** every 15 seconds:
  - `Sa29` (day + date)
  - `23C` (temperature from RTC, rounded to integer)
- NeoPixel **cyan breathing animation** (non-blocking, smooth)
- DFPlayer plays **startup sound** (`0001.mp3`) at low volume
- **5 V-safe** operation for Arduino Nano
- **Partial screen updates** to minimize OLED flicker

---

## 💻 Libraries

Install via Arduino Library Manager or PlatformIO:

- Adafruit GFX Library
- Adafruit SSD1306
- RTClib (by Adafruit)
- Adafruit NeoPixel
- DFRobot DFPlayer Mini

---

## 📁 Folder Structure (PlatformIO)
```
src/
├── main.cpp
├── ClockDisplay.cpp
├── LEDAnimator.cpp
└── AudioManager.cpp
include/
├── ClockDisplay.h
├── LEDAnimator.h
└── AudioManager.h
```

---

## 🚀 Quick Start

1. Install required libraries
2. Wire components per connection table above
3. Place a `0001.mp3` file on your DFPlayer microSD card (optional but recommended)
4. Upload code to Arduino Nano
5. On first boot, RTC syncs to compile time (`__DATE__`, `__TIME__`)
6. Enjoy your sleek, animated desk clock!

> 🔌 Power via USB or external 5 V — both work safely.

---

## 📸 Preview

![OLED Clock Display](assets/clock_demo.jpg)  
*Time with blinking colon, seconds, and alternating header (Sa29 ↔ 23C)*

> 📷 *Tip: Add a real photo of your build to `assets/clock_demo.jpg`!*

---

## 📜 License

Firmware is open-source under the **MIT License** — see [`LICENSE`](LICENSE).

Hardware design (if published separately) may use CERN OHL, but this repository contains **firmware only**.

---

> **Author**: Kabelo Matlakala  
> **Version**: 1.0 (Stable Release — 29 November 2025)