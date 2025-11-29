#include "ClockDisplay.h"
#include <Arduino.h>

ClockDisplay::ClockDisplay(Adafruit_SSD1306 &oled, RTC_DS3231 &r)
    : display(oled), rtc(r) {}

void ClockDisplay::begin() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Draw time once at startup (colon visible)
    drawTimeMain();
    drawHeader();
    display.display(); // full refresh on boot
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

void ClockDisplay::drawTimeMain() {
    DateTime now = rtc.now();

    // Clear only the time area if needed? Or just redraw — it's stable.
    // We'll redraw time only when colon changes or on boot.
    display.setTextSize(3);
    char timeBuf[6];
    snprintf(timeBuf, sizeof(timeBuf), "%02d%c%02d",
             now.hour(), colonVisible ? ':' : ' ', now.minute());
    display.setCursor(8, 30);
    display.print(timeBuf);

    display.setTextSize(2);
    char secBuf[3];
    snprintf(secBuf, sizeof(secBuf), "%02d", now.second());
    display.setCursor(100, 36);
    display.print(secBuf);
}

void ClockDisplay::drawHeader() {
    // Erase header area first
    display.fillRect(HEADER_X, HEADER_Y, HEADER_W, HEADER_H, SSD1306_BLACK);

    display.setTextSize(2);
    display.setCursor(HEADER_X, HEADER_Y);

    if (currentHeaderMode == TEMP_HEADER) {
        float tempC = rtc.getTemperature();
        display.print(tempC, 0); // no decimal for compactness
        display.print(F("C"));   // saves space vs "°C"
    } else {
        DateTime now = rtc.now();
        display.print(dayToString(now.dayOfTheWeek()));
        if (now.day() < 10) display.print('0');
        display.print(now.day());
    }
}

void ClockDisplay::update() {
    unsigned long now = millis();
    DateTime rtcNow = rtc.now();
    int currentSecond = rtcNow.second();

    // --- Handle slideshow timing ---
    bool shouldShowTemp = (currentSecond % TEMP_INTERVAL_SEC == 0);
    bool inTempPhase = (now - lastTempSwitch < TEMP_DURATION_MS);

    Mode newMode = shouldShowTemp && inTempPhase ? TEMP_HEADER : TIME_HEADER;

    if (newMode != currentHeaderMode) {
        currentHeaderMode = newMode;
        if (newMode == TEMP_HEADER) {
            lastTempSwitch = now; // reset timer when entering temp
        }
        drawHeader();
        display.display(); // partial update: only header changed
        return; // no need to update time yet
    }

    // --- Handle colon blinking (only if mode is TIME) ---
    if (currentHeaderMode == TIME_HEADER) {
        if (now - lastColonToggle >= 500) {
            colonVisible = !colonVisible;
            lastColonToggle = now;

            // Only redraw time area (not header)
            // But Adafruit_GFX doesn't support "dirty rectangles" natively,
            // so we clear & redraw time section manually.
            display.fillRect(0, 30, 128, 34, SSD1306_BLACK); // clear time+seconds area
            drawTimeMain();
            display.display(); // partial update of lower part
            return;
        }

        // Also update seconds every second (optional but recommended)
        static uint8_t lastSecond = 99;
        if (rtcNow.second() != lastSecond) {
            lastSecond = rtcNow.second();
            display.fillRect(100, 36, 20, 16, SSD1306_BLACK); // clear seconds area
            display.setTextSize(2);
            char secBuf[3];
            snprintf(secBuf, sizeof(secBuf), "%02d", rtcNow.second());
            display.setCursor(100, 36);
            display.print(secBuf);
            display.display();
            return;
        }
    }
}