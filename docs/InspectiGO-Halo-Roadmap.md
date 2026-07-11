# InspectiGO Halo
*A Minimalist Smart LED Clock Platform*

> Status: Planning & Hardware Prototyping
>
> Target 1: Arduino Nano
>
> Target 2: ESP32-C3 Mini
>
> Current Phase: LED Configuration & Animation Prototype

---

# Vision

InspectiGO Halo is a minimalist smart clock platform that combines ambient lighting,
time visualization, and subtle character animations.

Unlike a traditional digital clock, Halo should feel **alive**.

Its purpose is to be:

- Desk Clock
- Wall Clock
- Ambient Light
- Smart Notification Device
- Alarm Clock
- Timer / Stopwatch

while remaining elegant when idle.

---

# Design Goals

- Minimalist appearance
- Soft ambient lighting
- Smooth animations
- No visible electronics
- Modular software architecture
- Shared codebase across devices

---

# Editions

## 1. Halo Edition

Primary Hardware

- ESP32-C3 Mini
- WS2812B LED Ring / Strip
- Optional RTC
- USB-C

Features

- LED Clock
- Ambient Mode
- Alarm
- Timer
- Stopwatch
- Wi-Fi Time
- OTA Updates

---

## 2. Desk Edition

Primary Hardware

- Arduino Nano
- OLED
- NeoPixels
- DFPlayer
- RTC
- Buttons

Features

- Character Face
- Clock
- Alarm
- Music Player
- Timer
- Stopwatch

---

# Shared Software Architecture

InspectiGO/

    Core/
        ClockEngine
        AlarmEngine
        TimerEngine
        StopwatchEngine
        SettingsManager

    Drivers/
        RTC
        LEDStrip
        OLED
        Audio
        Buttons

    Renderers/
        RingRenderer
        TimeRenderer
        FaceRenderer

    Managers/
        DisplayManager
        ModeManager
        AnimationManager

    Devices/
        NanoDesk
        ESP32Halo

---

# Development Roadmap

## Phase 1
Hardware Prototype

Goal

Light LEDs reliably.

Tasks

- LED strip test
- Brightness control
- Power measurement
- Boot animation
- FPS testing

Deliverable

Working LED animation.

---

## Phase 2
Clock Engine

Goal

Display time using LEDs.

Tasks

- Hour indicator
- Minute indicator
- Second indicator
- Brightness scaling

Deliverable

Functional LED clock.

---

## Phase 3
Animation Engine

Goal

Smooth reusable effects.

Animations

- Breathing
- Sweep
- Rainbow
- Pulse
- Wake
- Shutdown
- Alarm
- Notification

Deliverable

Animation library.

---

## Phase 4
Mode Manager

Modes

Clock

Ambient

Night

Alarm

Timer

Stopwatch

Settings

Deliverable

Working state machine.

---

## Phase 5
User Interaction

Arduino

- Push Buttons

ESP32

- Buttons
- BLE
- Web UI

Deliverable

Interactive clock.

---

# LED Configuration Research

This phase intentionally comes before designing the enclosure.

Questions

## Ring Size

Investigate

- 12 LEDs
- 24 LEDs
- 36 LEDs
- 48 LEDs
- 60 LEDs
- 96 LEDs
- 120 LEDs

Evaluate

- Smoothness
- Readability
- Cost
- Power
- Ease of assembly

---

## Clock Representation

Possible methods

### Method A

One LED per hour.

Simple.

Lowest power.

---

### Method B

60 LEDs

Every minute has one LED.

Hours occupy multiple LEDs.

Highest precision.

---

### Method C

Continuous sweep

Minute hand moves smoothly.

Second hand fades.

Most premium appearance.

---

# Hardware Evaluation

Arduino Nano

Pros

- Simple
- Cheap
- Low power

Cons

- Limited RAM
- No Wi-Fi

Use

Desk Edition

---

ESP32-C3

Pros

- Wi-Fi
- BLE
- More RAM
- OTA Updates

Cons

- Slightly higher power

Use

Halo Edition

---

# Future Features

- Weather colours
- Calendar reminders
- Pomodoro mode
- Sunrise alarm
- Sleep mode
- Phone notifications
- Home Assistant integration

---

# Immediate Next Step

Prototype only.

No enclosure.

No alarm.

No buttons.

No Wi-Fi.

Goals

✓ Choose LED count

✓ Test animations

✓ Measure power

✓ Decide wall vs desk layout

Only after these are complete should the CAD enclosure begin.

---

# Success Criteria

The first prototype should be able to:

✓ Boot with animation

✓ Display current time

✓ Run smoothly at 30+ FPS

✓ Scale brightness

✓ Run for hours without instability

Everything else builds on this foundation.