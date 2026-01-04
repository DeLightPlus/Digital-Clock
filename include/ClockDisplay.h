#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

class ClockDisplay {
public:
    ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &rtc);
    void begin();
    void update(); // handles blinking and seconds update

private:
    Adafruit_SSD1306 &display;
    RTC_DS3231 &rtc;

    bool colonVisible = true;
    unsigned long lastColonToggle = 0;
    unsigned long lastSecond = 0;

    // Display regions
    static constexpr int TIME_X = 8;
    static constexpr int TIME_Y = 30;
    static constexpr int SEC_X = 100;
    static constexpr int SEC_Y = 36;
    static constexpr int DATE_X = 75;
    static constexpr int DATE_Y = 0;

    void drawTime();
    void drawDate();
    const __FlashStringHelper* dayToString(uint8_t day);
};

