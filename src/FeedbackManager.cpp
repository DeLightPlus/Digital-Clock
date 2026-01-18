#include "FeedbackManager.h"
#include <Arduino.h>

FeedbackManager::FeedbackManager(uint8_t buzzerPin, uint8_t vibratorPin)
    : buzzerPin(buzzerPin), vibratorPin(vibratorPin) {}

void FeedbackManager::begin() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(vibratorPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    analogWrite(vibratorPin, 0);  // PWM off
}

void FeedbackManager::update() {
    // Handle non-blocking vibration
    if (isVibrating && millis() >= vibrateEndTime) {
        analogWrite(vibratorPin, 0);  // PWM off
        isVibrating = false;

        // Handle multi-pulse pattern
        if (vibrationPulsesRemaining > 0 && pulseOffTime > 0) {
            vibrationPulsesRemaining--;
            if (vibrationPulsesRemaining > 0) {
                nextPulseTime = millis() + pulseOffTime;
            }
        }
    }

    // Handle next pulse in pattern
    if (vibrationPulsesRemaining > 0 && pulseOffTime > 0) {
        if (millis() >= nextPulseTime) {
            analogWrite(vibratorPin, 200);  // PWM on (0-255)
            vibrateEndTime = millis() + pulseOnTime;
            isVibrating = true;
            pulseOffTime = 0;  // Prevent re-triggering
        }
    }
}

void FeedbackManager::playBootTone() {
    // Play a pleasant ascending arpeggio
    beep(NOTE_C5, 100);
    delay(120);
    beep(NOTE_E5, 100);
    delay(120);
    beep(NOTE_G5, 100);
    delay(120);
    beep(NOTE_C6, 200);
}

void FeedbackManager::playFlashTone() {
    // Quick double beep for flash event
    beep(NOTE_G5, 80);
    delay(100);
    beep(NOTE_G5, 80);
}

void FeedbackManager::playAlarmTone() {
    // Alarm-style pattern: alternating high-low
    beep(1000, 200);
    delay(100);
    beep(800, 200);
}

void FeedbackManager::beep(uint16_t frequency, uint16_t duration_ms) {
    tone(buzzerPin, frequency, duration_ms);
}

void FeedbackManager::vibrate(uint16_t duration_ms) {
    analogWrite(vibratorPin, 200);  // PWM on (0-255)
    isVibrating = true;
    vibrateEndTime = millis() + duration_ms;
    vibrationPulsesRemaining = 0;
    pulseOffTime = 0;
}

void FeedbackManager::vibratePulse(uint16_t on_ms, uint16_t off_ms, uint8_t pulses) {
    if (pulses == 0) return;

    pulseOnTime = on_ms;
    pulseOffTime = off_ms;
    vibrationPulsesRemaining = pulses - 1;  // First pulse starts now

    analogWrite(vibratorPin, 200);  // PWM on (0-255)
    isVibrating = true;
    vibrateEndTime = millis() + on_ms;
}

void FeedbackManager::vibratePattern(const uint16_t* pattern, uint8_t count) {
    // pattern format: ON, OFF, ON, OFF, ..., ON
    // count must be odd (ends with ON)
    if (count < 3 || (count % 2) == 0) return;

    pulseOnTime = pattern[0];
    pulseOffTime = pattern[1];
    vibrationPulsesRemaining = (count - 1) / 2 - 1;  // Subtract first ON

    analogWrite(vibratorPin, 200);  // PWM on (0-255)
    isVibrating = true;
    vibrateEndTime = millis() + pattern[0];
}

