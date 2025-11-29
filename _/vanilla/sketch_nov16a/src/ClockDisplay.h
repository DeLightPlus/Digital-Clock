#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

class ClockDisplay {
public:
    ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &rtc);

    void begin();
    void showTime(bool showColon = true);

private:
    Adafruit_SSD1306 &display;
    RTC_DS3231 &rtc;

    String dayToString(uint8_t day);
};