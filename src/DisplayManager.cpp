#include <Arduino.h>
#include "DisplayManager.h"

DisplayManager::DisplayManager(
    Adafruit_SSD1306 &d,
    RTC_DS3231 &r,
    LEDAnimator &l,
    FeedbackManager *fb)
    : display(d), rtc(r), leds(l), feedback(fb), boot(d), clockUI(d, r) {}

void DisplayManager::begin() {
    boot.begin();
    // Don't initialize clockUI here - boot animation will clear the display
    // Clock will be initialized after boot completes
    mode = DisplayMode::Boot;
    bootPlayed = false;
}

void DisplayManager::setMode(DisplayMode newMode) {
    mode = newMode;
}

void DisplayManager::update() {
    unsigned long nowMs = millis();

    // Colon blink (used by clock)
    if (nowMs - lastColonBlink > 500) {
        colonVisible = !colonVisible;
        lastColonBlink = nowMs;
    }

    switch (mode) {
        case DisplayMode::Boot:
            if (!bootPlayed) {
                Serial.println(F("🎬 Starting boot animation..."));

                // Play boot feedback at start of boot
                if (feedback) {
                    feedback->playBootTone();
                }

                boot.play();      // blocking (for now, by design)
                bootPlayed = true;

                Serial.println(F("✅ Boot animation complete"));

                // Initialize clock UI after boot animation completes
                clockUI.begin();

                mode = DisplayMode::Clock;
                Serial.println(F("🔄 Transitioning to Clock mode"));
            }
            break;

        case DisplayMode::Clock:
            clockUI.update();
            break;

        case DisplayMode::Alarm:
            // future
            break;

        case DisplayMode::Settings:
            // future
            break;
    }
}

