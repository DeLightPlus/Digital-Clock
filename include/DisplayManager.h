#pragma once
#include <RTClib.h>
#include "ClockDisplay.h"

// Enum for different display modes
enum class DisplayMode {
    Splash,
    Clock,
    Alarm,
    Settings
};

class DisplayManager {
public:
    DisplayManager(Adafruit_SSD1306 &oled, RTC_DS3231 &rtc);

    void begin();
    void update();

private:
    Adafruit_SSD1306 &display;
    RTC_DS3231 &rtc;
    ClockDisplay clockUI;

    DisplayMode currentMode = DisplayMode::Splash;
    unsigned long modeStart = 0;
    bool colonVisible = true;
    unsigned long lastColonBlink = 0;

    void showSplash();
};