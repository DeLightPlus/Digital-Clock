// DisplayManager.cpp - TFT display control implementation
#include "DisplayManager.h"
#include "config.h"
#include <SPI.h>

// TFT display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void initDisplay() {
  // Initialize backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Turn on backlight before init
  
  // Initialize SPI with explicit pins
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  
  // Initialize display with SPI_MODE3 (critical for this panel)
  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(ST77XX_BLACK);
  
  Serial.println("Display initialized");
}

void showLoader() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.setCursor(60, 100);
  tft.print("Loading...");
  
  // Animated progress bar
  for (int i = 0; i <= 100; i += 10) {
    int barWidth = map(i, 0, 100, 0, 180);
    tft.fillRoundRect(30, 130, barWidth, 20, 5, ST77XX_CYAN);
    tft.setCursor(220, 130);
    tft.setTextSize(1);
    tft.print(i);
    tft.print("%");
    delay(100);
  }
  delay(500);
}

void initClockDisplay() {
  // Clear screen and draw static frame
  tft.fillScreen(ST77XX_BLACK);
  
  // Decorative elements
  tft.drawCircle(120, 120, 110, ST77XX_MAGENTA);
  tft.drawCircle(120, 120, 108, ST77XX_WHITE);
  
  Serial.println("Clock display initialized");
}

void updateClockDisplay(const DateTime &now, bool colonVisible, float temperature) {
  char timeBuffer[6];
  char dateBuffer[12];
  char secBuffer[3];
  char tempBuffer[10];
  
  // Format time HH:MM with blinking colon
  sprintf(timeBuffer, "%02d%c%02d", now.hour(), colonVisible ? ':' : ' ', now.minute());
  
  // Format date
  sprintf(dateBuffer, "%02d/%02d/%04d", now.day(), now.month(), now.year());
  
  // Format seconds
  sprintf(secBuffer, "%02d", now.second());
  
  // Format temperature (DS3231 provides temp in Celsius)
  sprintf(tempBuffer, "%.1fC", temperature);
  
  // Clear areas for dynamic content (time, seconds, date, temperature)
  tft.fillRect(20, 90, 200, 40, ST77XX_BLACK);  // Time area
  tft.fillRect(70, 145, 100, 20, ST77XX_BLACK); // Seconds area
  tft.fillRect(40, 170, 160, 20, ST77XX_BLACK); // Date area
  tft.fillRect(60, 200, 120, 20, ST77XX_BLACK); // Temperature area
  
  // Display time (HH:MM)
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(35, 95);
  tft.print(timeBuffer);
  
  // Display seconds
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(100, 145);
  tft.print(secBuffer);
  
  // Display date
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(50, 170);
  tft.print(dateBuffer);
  
  // Display temperature
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_ORANGE);
  tft.setCursor(85, 200);
  tft.print(tempBuffer);
}

Adafruit_ST7789& getTFT() {
  return tft;
}
