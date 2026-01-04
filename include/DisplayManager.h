#pragma once
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "BootDisplay.h"
#include "ClockDisplay.h"
#include "LEDAnimator.h"
#include "AudioManager.h"

enum class DisplayMode
{
    Boot,
    Clock,
    Alarm,
    Settings
};

class DisplayManager
{
public:
    DisplayManager(
        Adafruit_SSD1306 &display,
        RTC_DS3231 &rtc,
        LEDAnimator &leds,
        AudioManager *audio = nullptr);

    void begin();
    void update();

    void setMode(DisplayMode mode);

private:
    Adafruit_SSD1306 &display;
    RTC_DS3231 &rtc;

    LEDAnimator &leds;
    AudioManager *audio;

    BootDisplay boot;
    ClockDisplay clockUI;

    DisplayMode mode = DisplayMode::Boot;

    bool bootPlayed = false;
    bool colonVisible = true;
    unsigned long lastColonBlink = 0;
};
