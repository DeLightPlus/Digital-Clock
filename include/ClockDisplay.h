#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

class ClockDisplay {
public:
    ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &rtc);
    void begin();
    void update(); // handles blinking + slideshow (partial update)

private:
    Adafruit_SSD1306 &display;
    RTC_DS3231 &rtc;

    enum Mode { TIME_HEADER, TEMP_HEADER };
    Mode currentHeaderMode = TIME_HEADER;

    unsigned long lastTempSwitch = 0;
    unsigned long lastColonToggle = 0;
    bool colonVisible = true;

    static constexpr unsigned long TEMP_DURATION_MS = 5000;
    static constexpr unsigned long TEMP_INTERVAL_SEC = 15;

    // Header region bounds (adjust if needed)
    static constexpr int HEADER_X = 75;
    static constexpr int HEADER_Y = 0;
    static constexpr int HEADER_W = 53; // 128 - 75 = 53px wide
    static constexpr int HEADER_H = 16; // text size 2 → ~16px tall

    void drawTimeMain();      // draws HH:MM:ss (once, unless colon changes)
    void drawHeader();        // draws only Sa29 or 30°C
    const __FlashStringHelper* dayToString(uint8_t day);
};