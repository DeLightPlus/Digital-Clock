/**
 * iGO ESP32 Clock - Main Application
 * 
 * Modular clock system with:
 * - ST7789 TFT display (240x240)
 * - DS3231 RTC with temperature sensor
 * - WS2812B NeoPixel LED strip
 */

#include <Arduino.h>
#include "config.h"
#include "DisplayManager.h"
#include "ClockManager.h"
#include "rtc_time.h"
#include "leds.h"

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== iGO ESP32 Clock v1.0 ===");

  // Initialize hardware managers
  initDisplay();
  initRTC();
  initLEDs();
  initClockManager();

  // Show startup animation
  showLoader();
  
  // Initialize clock interface
  initClockDisplay();
  displayRTCStatus();
  
  // Initial display with temperature
  DateTime now = getCurrentTime();
  float temp = getCurrentTemperature();
  updateClockDisplay(now, true, temp);
  
  Serial.println("System ready\n");
}

void loop() {
  // Clock manager handles timing and display updates
  updateClock();
  
  // Update LED breathing animation
  updateLEDs();
  
  delay(10);  // Small delay for stability
}
