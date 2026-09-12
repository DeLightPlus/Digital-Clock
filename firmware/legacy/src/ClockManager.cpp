// ClockManager.cpp - Clock logic and state management implementation
#include "ClockManager.h"
#include "DisplayManager.h"
#include "rtc_time.h"
#include "config.h"

// Clock state
static bool colonOn = true;
static unsigned long lastBlink = 0;
static unsigned long lastDraw = 0;

void initClockManager() {
  lastBlink = millis();
  lastDraw = millis();
  Serial.println("Clock manager initialized");
}

void updateClock() {
  unsigned long currentMs = millis();
  
  // Blink colon every 500ms
  if (currentMs - lastBlink >= COLON_BLINK_MS) {
    colonOn = !colonOn;
    lastBlink = currentMs;
  }
  
  // Update display every 500ms
  if (currentMs - lastDraw >= DISPLAY_UPDATE_MS) {
    DateTime now = getCurrentTime();
    float temp = getCurrentTemperature();
    updateClockDisplay(now, colonOn, temp);
    lastDraw = currentMs;
  }
}

bool getColonVisible() {
  return colonOn;
}
