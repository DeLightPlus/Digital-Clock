#include "ClockDisplay.h"
#include <math.h>

ClockDisplay::ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &r)
    : display(oled), rtc(r) {}

void ClockDisplay::begin()
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

String ClockDisplay::dayToString(uint8_t day)
{
    static const char *days[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    return days[day];
}

void ClockDisplay::showTime(bool showColon)
{
    DateTime now = rtc.now();
    float tempC = rtc.getTemperature();

    display.clearDisplay();

    // --- Top header: day + date only ---
    display.setTextSize(2);
    String dayLabel = dayToString(now.dayOfTheWeek());
    String header = dayLabel.substring(0, 2);
    header.setCharAt(0, toupper(header.charAt(0)));
    header.setCharAt(1, tolower(header.charAt(1)));
    header += String(now.day());

    display.setCursor(75, 0);          // right-aligned look
    display.print(header);

    // --- Main time HH:MM:ss ---
    display.setTextSize(3);
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d%c%02d",
             now.hour(), showColon ? ':' : ' ', now.minute());
    int16_t cursorX = 8, cursorY = 30;
    display.setCursor(cursorX, cursorY);
    display.print(buf);

    display.setTextSize(2);
    char sec[3];
    snprintf(sec, sizeof(sec), "%02d", now.second());
    display.setCursor(100, cursorY + 6);
    display.print(sec);

    display.display();
}