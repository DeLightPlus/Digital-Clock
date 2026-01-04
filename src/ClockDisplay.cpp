#include "ClockDisplay.h"
#include <Arduino.h>

ClockDisplay::ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &r)
    : display(oled), rtc(r) {}

void ClockDisplay::begin() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    
    // Draw time and date once
    drawTime();
    drawDate();
    display.display();
}

const __FlashStringHelper* ClockDisplay::dayToString(uint8_t day) {
    if (day == 0) return F("Su");
    if (day == 1) return F("Mo");
    if (day == 2) return F("Tu");
    if (day == 3) return F("We");
    if (day == 4) return F("Th");
    if (day == 5) return F("Fr");
    if (day == 6) return F("Sa");
    return F("??");
}

void ClockDisplay::drawTime() {
    DateTime now = rtc.now();
    
    display.setTextSize(3);
    char timeBuf[6];
    snprintf(timeBuf, sizeof(timeBuf), "%02d%c%02d",
             now.hour(), colonVisible ? ':' : ' ', now.minute());
    display.setCursor(TIME_X, TIME_Y);
    display.print(timeBuf);

    display.setTextSize(2);
    char secBuf[3];
    snprintf(secBuf, sizeof(secBuf), "%02d", now.second());
    display.setCursor(SEC_X, SEC_Y);
    display.print(secBuf);
}

void ClockDisplay::drawDate() {
    DateTime now = rtc.now();
    
    display.setTextSize(2);
    display.setCursor(DATE_X, DATE_Y);
    display.print(dayToString(now.dayOfTheWeek()));
    if (now.day() < 10) display.print('0');
    display.print(now.day());
}

void ClockDisplay::update() {
    unsigned long now = millis();
    DateTime nowRTC = rtc.now();

    // Update seconds every second
    if (now - lastSecond >= 1000) {
        lastSecond = now;
        
        // Clear and redraw seconds area
        display.fillRect(SEC_X, SEC_Y, 20, 16, SSD1306_BLACK);
        display.setTextSize(2);
        char secBuf[3];
        snprintf(secBuf, sizeof(secBuf), "%02d", nowRTC.second());
        display.setCursor(SEC_X, SEC_Y);
        display.print(secBuf);
        
        display.display();
    }

    // Toggle colon every 500ms
    if (now - lastColonToggle >= 500) {
        lastColonToggle = now;
        colonVisible = !colonVisible;
        
        // Clear and redraw time (HH:MM)
        display.fillRect(TIME_X, TIME_Y, 92, 30, SSD1306_BLACK);
        display.setTextSize(3);
        char timeBuf[6];
        snprintf(timeBuf, sizeof(timeBuf), "%02d%c%02d",
                 nowRTC.hour(), colonVisible ? ':' : ' ', nowRTC.minute());
        display.setCursor(TIME_X, TIME_Y);
        display.print(timeBuf);
        
        display.display();
    }
}

