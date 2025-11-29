#include "DisplayManager.h"
#include <Arduino.h>

DisplayManager::DisplayManager(Adafruit_SSD1306 &oled, RTC_DS3231 &r)
    : display(oled), rtc(r), clockUI(oled, r) {}

void DisplayManager::begin() {
    clockUI.begin();
    currentMode = DisplayMode::Splash;
    modeStart = millis();
    colonVisible = true;
}

void DisplayManager::update() {
    unsigned long nowMs = millis();

    // Splash → Clock after 3 s
    if (currentMode == DisplayMode::Splash && nowMs - modeStart > 3000) {
        currentMode = DisplayMode::Clock;
    }

    // Blink colon for all modes
    if (nowMs - lastColonBlink > 500) {
        colonVisible = !colonVisible;
        lastColonBlink = nowMs;
    }

    switch (currentMode) {
        case DisplayMode::Splash:
            showSplash();
            break;

        case DisplayMode::Clock:
            // clockUI.update(colonVisible);
            break;

        // placeholders for expansion
        case DisplayMode::Alarm:     /* ... */ break;
        case DisplayMode::Settings:  /* ... */ break;
    }
}

void DisplayManager::showSplash() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(20, 24);
    display.println(F("Smart Clock"));
    display.display();
}