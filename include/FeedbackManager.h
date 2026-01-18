#pragma once

#include <Arduino.h>

// FeedbackManager - Handles buzzer tones and vibrator haptic feedback
// Replaces AudioManager for simpler audio without MP3 module

class FeedbackManager {
public:
    // Default pins (can be changed)
    static constexpr uint8_t DEFAULT_BUZZER_PIN = 3;    // PWM-capable
    static constexpr uint8_t DEFAULT_VIBRATOR_PIN = 17; // A3 - PWM-capable

    FeedbackManager(
        uint8_t buzzerPin = DEFAULT_BUZZER_PIN,
        uint8_t vibratorPin = DEFAULT_VIBRATOR_PIN);

    void begin();
    void update();  // Call in loop() for non-blocking vibration

    // Sound feedback
    void playBootTone();   // Startup melody
    void playFlashTone();  // For white flash event
    void playAlarmTone();  // Alarm sound
    void beep(uint16_t frequency, uint16_t duration_ms);

    // Haptic feedback
    void vibrate(uint16_t duration_ms);           // Single vibration
    void vibratePulse(uint16_t on_ms, uint16_t off_ms, uint8_t pulses);
    void vibratePattern(const uint16_t* pattern, uint8_t count);

private:
    uint8_t buzzerPin;
    uint8_t vibratorPin;

    // Vibration state
    bool isVibrating = false;
    unsigned long vibrateEndTime = 0;
    uint8_t vibrationPulsesRemaining = 0;
    unsigned long nextPulseTime = 0;
    uint16_t pulseOnTime = 0;
    uint16_t pulseOffTime = 0;

    // Note frequencies (Hz)
    static constexpr uint16_t NOTE_C5 = 523;
    static constexpr uint16_t NOTE_E5 = 659;
    static constexpr uint16_t NOTE_G5 = 784;
    static constexpr uint16_t NOTE_C6 = 1047;
};

