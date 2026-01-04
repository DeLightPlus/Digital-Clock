#include <Arduino.h>
#include "DisplayManager.h"


DisplayManager::DisplayManager( Adafruit_SSD1306 &d,
    RTC_DS3231 &r,
    LEDAnimator &l,
    AudioManager *a)
    : display(d), rtc(r), leds(l), audio(a), boot(d, a), clockUI(d, r) {}

void DisplayManager::begin() {
    boot.begin();
    // clockUI.begin() is called after boot completes
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
                boot.play();      // blocking - plays boot animation with audio
                bootPlayed = true;
                clockUI.begin();  // Init clock UI after boot completes
                mode = DisplayMode::Clock;
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

