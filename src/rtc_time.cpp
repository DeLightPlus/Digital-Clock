// rtc_time.cpp - Real-time clock management implementation
#include "rtc_time.h"
#include "DisplayManager.h"
#include "config.h"
#include <Wire.h>

// RTC globals
static RTC_DS3231 rtc;
static bool rtcOk = false;
static DateTime baseCompile(F(__DATE__), F(__TIME__));
static unsigned long startMs = 0;

bool initRTC() {
  Wire.begin(I2C_SDA, I2C_SCL);
  
  if (!rtc.begin()) {
    Serial.println("RTC: Not found, using software clock");
    rtcOk = false;
  } else {
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      Serial.println("RTC: Power lost, reset to compile time");
    } else {
      Serial.println("RTC: OK");
    }
    rtcOk = true;
  }
  
  startMs = millis();
  return rtcOk;
}

DateTime getCurrentTime() {
  if (rtcOk) {
    return rtc.now();
  }
  // Software fallback clock
  uint32_t elapsed = (millis() - startMs) / 1000;
  return DateTime(baseCompile.unixtime() + elapsed);
}

float getCurrentTemperature() {
  if (rtcOk) {
    // DS3231 has built-in temperature sensor (±3°C accuracy)
    return rtc.getTemperature();
  } else {
    // Return placeholder if RTC unavailable
    return 0.0;
  }
}

bool isRTCAvailable() {
  return rtcOk;
}

void displayRTCStatus() {
  Adafruit_ST7789& tft = getTFT();
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(18, 30);
  tft.print(rtcOk ? "RTC" : "SOFT CLOCK");
}
