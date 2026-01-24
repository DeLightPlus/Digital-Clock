// DisplayManager.h - TFT display control interface
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_ST7789.h>
#include <RTClib.h>

// Initialize TFT display hardware
void initDisplay();

// Show loading animation
void showLoader();

// Initialize static clock elements (frame, labels, etc.)
void initClockDisplay();

// Update clock display with current time and temperature
void updateClockDisplay(const DateTime &now, bool colonVisible, float temperature);

// Get reference to TFT object for external use
Adafruit_ST7789& getTFT();

#endif // DISPLAY_MANAGER_H
