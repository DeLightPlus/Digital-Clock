/**
 * iGO ESP32 Clock - Main Application
 * 
 * Modular clock system with:
 * - ST7789 TFT display (240x240)
 * - DS3231 RTC with temperature sensor
 * - WS2812B NeoPixel LED strip (cyan breathing)
 * - WiFi AP with REST API for remote control
 */

#include <Arduino.h>
#include "config.h"
#include "DisplayManager.h"
#include "ClockManager.h"
#include "rtc_time.h"
#include "leds.h"
#include "WebManager.h"

// Shared state variable for notifications
String lastNotification = "";

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== iGO ESP32 Clock v2.0 ===");

  // Initialize hardware managers
  initDisplay();
  initRTC();
  initLEDs();
  initClockManager();

  // Show startup animation
  showLoader();
  
  // Initialize clock interface
  initClockDisplay();
  
  // Initial display with temperature
  DateTime now = getCurrentTime();
  float temp = getCurrentTemperature();
  updateClockDisplay(now, true, temp);
  
  // Initialize WiFi AP and REST API server
  // Reads credentials from wifi_config.h
  // Supports dual mode: connects to home WiFi while also broadcasting iGO-Buddy AP
  initWebServer();
  
  // Display WiFi connection status after WiFi initialization
  displayWiFiStatus();
  
  Serial.println("\n=== System Ready ===");
  Serial.println("Modes: Clock Display + Web Control");
  Serial.println("Local AP: http://192.168.4.1 (or iGO-Buddy SSID)");
  Serial.println("Home WiFi: Configure in wifi_config.h");
  Serial.println("====================\n");
}

void loop() {
  // Clock manager handles timing and display updates
  updateClock();
  
  // Update LED breathing animation
  updateLEDs();
  
  // Handle incoming web requests (non-blocking)
  updateWebServer();
  
  delay(10);  // Small delay for stability
}
