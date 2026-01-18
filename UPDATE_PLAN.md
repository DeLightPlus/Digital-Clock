# Feedback Haptic Branch Plan

## Branch: `blackboxai/feedback-haptic`

This branch replaces the MP3-based AudioManager with a simpler buzzer + vibrator feedback system.

## Changes Made

### New Files Created:
- `include/FeedbackManager.h` - Header for buzzer/vibrator control
- `src/FeedbackManager.cpp` - Implementation with:
  - `playBootTone()` - Startup melody (C5-E5-G5-C6 arpeggio)
  - `playFlashTone()` - Quick double beep
  - `playAlarmTone()` - Alarm pattern
  - `vibrate(duration)` - Single vibration
  - `vibratePulse(on, off, pulses)` - Pattern vibration
  - `update()` - Non-blocking vibration handling

### Files Modified:
- `src/main.cpp` - Uses FeedbackManager instead of AudioManager
- `include/DisplayManager.h` - Changed to use FeedbackManager*
- `src/DisplayManager.cpp` - Calls `feedback->playBootTone()` at boot start
- `include/BootDisplay.h` - Removed AudioManager reference
- `src/BootDisplay.cpp` - Removed AudioManager usage

### AudioManager Removed From:
- `include/BootDisplay.h` - AudioManager* member deleted
- `src/BootDisplay.cpp` - Constructor updated, audio calls removed
- `include/FeedbackManager.h` - Comment updated (no longer mentions AudioManager)

## Pin Assignment
| Component | Arduino Pin |
|-----------|-------------|
| Buzzer (PWM) | 3 |
| Vibrator | 11 |
| LEDs | 6, 5 |
| OLED | I2C |
| RTC | I2C |

## MP3 Version Archived
The original AudioManager files have been moved to:
- `_/vanilla/AudioManager/AudioManager.h`
- `_/vanilla/AudioManager/AudioManager.cpp`

These can be restored later if needed.

## To Test:
1. Connect buzzer to pin 3 (PWM)
2. Connect vibrator to pin 11 (with transistor driver)
3. Build and upload
4. Boot sequence should play melody on buzzer

