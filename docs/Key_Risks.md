# ⚠️ InspectiGO – Key Technical Risks & Mitigations

This document highlights known technical risks in the InspectiGO smartwatch project
and records recommended mitigation strategies for future reference.

---

## DFPlayer Audio Sync Drift

### Risk
The DFPlayer Mini does **not start audio playback deterministically**.
Even when using a single continuous boot audio file, playback may start
~50–150ms late relative to visual animations.

This can cause:
- Visuals appearing slightly ahead of sound
- Perceived desynchronization during the boot sequence

### Mitigation
- Use **one continuous boot audio file** (`0001.mp3`, 11.6s)
- Drive all visual timing using **`millis()` offsets**, not audio events
- Treat audio as a background layer, not a timing authority

#### Recommended Pattern
```cpp
bootStartTime = millis();
audio.play(1);  // 0001.mp3

if (elapsed >= PHASE_1_START) {
    // Trigger visual phase
}

```

### Status
✔️ Known
🛠️ Mitigation planned / in progress
---

## OLED Burn-In Risk (Static Clock UI)
### Risk

SSD1306 OLED displays are susceptible to image retention / burn-in,
especially when showing:
- Large static text
- Always-on clock UI
- High contrast for long durations

### Mitigation Options
Implement at least two of the following:
- Slight clock position jitter (±1px every minute)
- Automatic dimming during night hours
- Periodic display inversion (brief)
- Lower contrast after inactivity
- Sleep mode with motion/button wake

### Status
⚠️ Not yet implemented
🛠️ Planned for optimization phase

---

## NeoPixel Power Spikes During Boot
### Risk
The white flash phase (RGB(200,200,200)) across 5 WS2812B LEDs can briefly
draw 300mA+, which may:
- Cause brownouts on LiPo power
- Stress voltage regulators
- Reset the MCU during boot

### Mitigation
- Reduce white flash brightness (e.g. RGB(120,120,120))
- Cap global NeoPixel brightness (strip.setBrightness())
- Avoid full-white frames on battery power

### Status
✔️ Identified
🛠️ Mitigation recommended before enclosure build

## Single-Point Boot Failure Recovery
### Risk
A power brownout or reset during the boot animation could leave the system
in an undefined visual or audio state.

### Mitigation
- Always allow boot animation to restart safely
- Ensure all managers reset state on setup()
- Avoid blocking calls during boot phases

### Status
✔️ Architecture supports recovery
🛠️ Stress testing recommended

## I2C Bus Stability (OLED + RTC Shared Bus)
### Risk
OLED (0x3C) and RTC (0x68) share the same I2C bus.
Poor wiring or noise may cause:
- Display freezes
- RTC read failures
- Missed acknowledgements

### Mitigation
- Short I2C wiring
- Common ground
- Add bus scan at startup (debug builds)
- Fail gracefully if RTC is unavailable

### Status
✔️ Known
🛠️ Monitor during hardware testing

---

## 📌 Summary
These risks are expected in embedded wearable systems and do not
indicate design flaws. They are documented here to ensure:
- Future-proofing
- Easier debugging
- Informed hardware revisions

Addressing them incrementally will significantly improve system robustness,
battery life, and user experience.

---

If you want, next we can:
- Split this into **DEV NOTES vs USER SAFETY**
- Add a **“Resolved / Deferred”** checkbox system
- Turn this into a **pre-release checklist**

Just say the word 👌

---
