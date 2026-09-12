iGO Digital Clock — Desk

Firmware for the Desk variant of the iGO Digital Clock.

The Desk clock is being developed as a modular platform so that different microcontrollers can be used without changing the fundamental display design.

Firmware Targets

The Desk variant may support multiple MCU platforms:

firmware/
└── desk/
    ├── arduino/
    ├── esp32/
    ├── pico/
    ├── shared/
    └── README.md


Not every target needs to be implemented. Directories should only be added when a platform is actually supported.

Display Architecture

The Desk display is based on addressable LEDs arranged into 7-segment digits.

Each digit contains 7 addressable LEDs.

The physical LED order is:

C → D → E → F → A → B → G

Segment Layout
        A
     ───────
   F │       │ B
     │   G   │
     ───────
   E │       │ C
     │       │
     ───────
        D


Therefore:

LED 0 = C
LED 1 = D
LED 2 = E
LED 3 = F
LED 4 = A
LED 5 = B
LED 6 = G


The LEDs physically follow the perimeter clockwise and finish at the centre segment.

The software handles the mapping between the physical LED order and the conventional 7-segment representation.

Hours Module

The current hardware plan is to build the hours display as:

7 LEDs + 7 LEDs + 1 blink LED


Therefore:

Digit 0 = LEDs 0–6
Digit 1 = LEDs 7–13
Blink   = LED 14


Total:

15 addressable LEDs


The intended arrangement is:

┌───────────────┐
│   HOUR DIGITS │
│               │
│     1  2      │
│               │
│       •       │
└───────────────┘


The final physical appearance will depend on the enclosure/display construction.

Minutes Module

Minutes are planned to use an equivalent module:

7 LEDs + 7 LEDs + 1 blink LED


Giving:

15 LEDs


for the minutes section.

The planned complete Desk display is therefore:

HOURS                  MINUTES

7 + 7 + 1              7 + 7 + 1
   │                       │
   └──────────┐ ┌──────────┘
              │ │
           30 LEDs


The exact GPIO arrangement is still to be determined.

Current Development Milestone

The display is being developed incrementally.

Completed
 Addressable LED prototype
 Single 7-segment digit tested
 Physical LED order established
 C → D → E → F → A → B → G mapping confirmed
 Digit 0–9 mapping established
 Arduino/ESP32-style NeoPixel test working
Current
 Add second digit
 Add blink/colon LED
 Test complete 15-LED hours module
 Migrate prototype into PlatformIO
 Separate display logic into .h / .cpp
 Test two-digit hours
Next
 Build minutes module
 Test complete 30-LED display
 Implement clock/timekeeping
 Determine RTC requirements
 Add user controls
 Finalise MCU GPIO allocation
 Extract reusable Desk display code
Software Architecture

The Desk firmware is expected to have three levels of code.

Product-independent code

Code that is genuinely useful across the entire iGO project may eventually live under:

firmware/shared/


Examples could include:

clock/time models
date/time calculations
generic digit representations
common utility code
Desk-specific shared code

Code shared between Desk MCU implementations belongs under:

firmware/desk/shared/


Examples:

Desk 7-segment display mapping
Desk LED animations
Desk display layout
hours/minutes display logic

This code should not be forced into the global firmware/shared/ directory if Wall or Wrist products have no use for it.

MCU-specific code

Hardware-specific implementation belongs under the relevant target:

firmware/desk/arduino/
firmware/desk/esp32/
firmware/desk/pico/


This includes:

GPIO configuration
MCU-specific peripherals
board configuration
hardware initialisation
PlatformIO configuration
Design Principle

The physical Desk display should remain consistent across MCU implementations wherever practical.

For example:

Desk Arduino
Desk ESP32
Desk Pico


should all use the same conceptual display:

C → D → E → F → A → B → G


The MCU should not determine the meaning of the segments.

Instead:

              Desk Display
                    │
          ┌─────────┴─────────┐
          │                   │
     Display Logic       Hardware Layer
          │                   │
       segments          Arduino / ESP32 / Pico


This allows the display logic to be reused while keeping hardware-specific details isolated.

PlatformIO

Each supported MCU is intended to be a separate PlatformIO project.

For example:

firmware/
└── desk/
    ├── arduino/
    │   └── platformio.ini
    │
    ├── esp32/
    │   └── platformio.ini
    │
    └── pico/
        └── platformio.ini


This avoids trying to force unrelated boards into a single PlatformIO environment.

Current Prototype Naming

The initial display test is referred to as:

desk_digit_mapping_test


Its purpose is to validate:

physical LED
      ↓
segment mapping
      ↓
digit 0–9


It is deliberately separate from the eventual clock firmware.

Planned Progression
Single digit
    │
    ▼
7 LEDs
    │
    ▼
Two digits
    │
    ▼
7 + 7 LEDs
    │
    ▼
+ blink LED
    │
    ▼
15-LED hours module
    │
    ▼
15-LED minutes module
    │
    ▼
30-LED Desk display
    │
    ▼
Clock logic
    │
    ▼
Complete Desk firmware

Status

Product: iGO Digital Clock — Desk
Stage: Display hardware/prototype development

The immediate objective is to complete and validate the 15-LED hours module before expanding to the minutes module and full clock functionality.