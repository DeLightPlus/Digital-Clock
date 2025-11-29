/************************************************************************************
# ⏰ OLED Clock Display with DS3231 RTC + Temperature + LED Breathing Animation

A clean, modular digital desktop clock built on the **Arduino Nano**.

🧩 **Modules**
- 🖥 **SSD1306 OLED** – I²C display shows time and temperature/date alternately  
- ⏱ **DS3231 RTC** – precise real‑time clock & on‑board temperature sensor  
- 🔊 **DFPlayer Mini** – startup sound / audio feedback (pins 8 = RX, 9 = TX)  
- 💡 **NeoPixel LED strip** – smooth cyan breathing animation (5 LEDs, D6)  

⚙️ **Electrical Connection (5 V Safe)**
<div class="my-4 w-full overflow-x-auto">
<table class="min-w-full border-collapse text-sm">
<thead>
<tr>
<th class="whitespace-nowrap border-b border-zinc-200 px-3 py-2 text-left text-xs font-medium text-zinc-900 dark:border-zinc-700 dark:text-zinc-100 sm:px-4 sm:text-sm">Module</th>
<th class="whitespace-nowrap border-b border-zinc-200 px-3 py-2 text-left text-xs font-medium text-zinc-900 dark:border-zinc-700 dark:text-zinc-100 sm:px-4 sm:text-sm">Pin on Nano</th>
<th class="whitespace-nowrap border-b border-zinc-200 px-3 py-2 text-left text-xs font-medium text-zinc-900 dark:border-zinc-700 dark:text-zinc-100 sm:px-4 sm:text-sm">Notes</th>
</tr>
</thead>
<tbody>
<tr class="border-b border-zinc-200 hover:bg-zinc-50 dark:border-zinc-800 dark:hover:bg-zinc-900">
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">OLED SSD1306</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">SDA → A4 / SCL → A5</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">0x3C or 0x3D I²C address</td>
</tr>
<tr class="border-b border-zinc-200 hover:bg-zinc-50 dark:border-zinc-800 dark:hover:bg-zinc-900">
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">DS3231 RTC</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">SDA → A4 / SCL → A5</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">Shared I²C bus</td>
</tr>
<tr class="border-b border-zinc-200 hover:bg-zinc-50 dark:border-zinc-800 dark:hover:bg-zinc-900">
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">NeoPixel LED</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">D6</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">5 LEDs, 5 V, common GND</td>
</tr>
<tr class="border-b border-zinc-200 hover:bg-zinc-50 dark:border-zinc-800 dark:hover:bg-zinc-900">
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">DFPlayer Mini</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">RX → 9 / TX → 8</td>
<td class="px-3 py-2 text-xs text-zinc-700 dark:text-zinc-300 sm:px-4 sm:text-base">AltSoftSerial (avoid USB conflict)</td>
</tr>
</tbody>
</table>
</div>

🧠 **Features**
- Big centered time `HH:MM` with blinking colon  
- Seconds in small font  
- Alternating bottom line: date ↔ temperature (q15 s)  
- NeoPixel cyan breathing light (non‑blocking)  
- DFPlayer low‑volume startup sound (track 0001.mp3)  
- 5 V‑safe operation for Nano VCC  

💻 **Libraries (Arduino Library Manager / PlatformIO lib_deps)**
Adafruit GFX Library
Adafruit SSD1306
RTClib (by Adafruit)
Adafruit NeoPixel
DFRobot DFPlayer Mini
🧾 **Folder Structure (for PlatformIO)**
```
src/ 
├─ main.cpp 
├─ ClockDisplay.cpp 
├─ LEDAnimator.cpp 
└─ AudioManager.cpp 
include/ 
├─ ClockDisplay.h 
├─ LEDAnimator.h 
└─ AudioManager.h
```

**Design Goals** 
- Modular / reusable modules with clear interfaces 
- Non‑blocking updates using `millis()` 
- Reliable startup sequence to avoid I²C lock and serial freeze 
---

* Author  : Kabelo Matlakala 
* Version : 1.0  (Stable Release — 2025‑01) 
* License : Open Hardware / MIT ************************************************************************************