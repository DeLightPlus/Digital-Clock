#include "ClockDisplay.h"
#include <math.h>

ClockDisplay::ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &r)
    : display(oled), rtc(r) {}

void ClockDisplay::begin() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

String ClockDisplay::dayToString(uint8_t day) {
    static const char* days[] = {"Su","Mo","Tu","We","Th","Fr","Sa"};
    return days[day];
}

void ClockDisplay::showTime(bool showColon) {
    DateTime now = rtc.now();
    float tempC = rtc.getTemperature();

    display.clearDisplay();

    // --- Top header: date + temperature ---
    display.setTextSize(2);
    String day2 = dayToString(now.dayOfTheWeek());
    String compact = day2 + String(now.day());

    char tempBuf[10];
    snprintf(tempBuf, sizeof(tempBuf), "%dC", (int)round(tempC));

    // Line: Sa29   [28°C]
    display.setCursor(0, 0);
    display.print(compact);
    display.print("   ");
    display.print(tempBuf);

    // --- Main time HH:MM:ss ---
    display.setTextSize(3);
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d%c%02d", now.hour(), showColon ? ':' : ' ', now.minute());

    int16_t cursorX = 8;
    int16_t cursorY = 45;
    display.setCursor(cursorX, cursorY);
    display.print(buf);

    display.setTextSize(2);
    char sec[3];
    snprintf(sec, sizeof(sec), "%02d", now.second());
    display.setCursor(100, cursorY + 6);
    display.print(sec);

    display.display();
}