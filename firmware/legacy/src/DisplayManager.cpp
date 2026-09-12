// DisplayManager.cpp - TFT display control implementation
#include "DisplayManager.h"
#include "DisplaySettings.h"
#include "config.h"
#include <SPI.h>
#include <math.h>

// TFT display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Track previous second indicator position
static int lastSecondX = -1;
static int lastSecondY = -1;

// Day names for date formatting
static const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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
  char timeBuffer[12];
  char dateBuffer[20];
  char tempBuffer[10];
  char ampmBuffer[4] = "";
  
  // Clock center and radius for second indicator
  const int centerX = 120;
  const int centerY = 120;
  const int radius = 110;  // Same as decorative circle
  const int dotRadius = 5;  // Size of the moving dot
  
  // Format time based on user preference (12h or 24h)
  TimeFormat timeFormat = getTimeFormat();
  int displayHour = now.hour();
  
  if (timeFormat == TIME_12H) {
    // 12-hour format with AM/PM
    bool isPM = displayHour >= 12;
    if (displayHour == 0) displayHour = 12;  // Midnight is 12 AM
    else if (displayHour > 12) displayHour -= 12;  // Convert to 12-hour
    
    sprintf(timeBuffer, "%2d%c%02d", displayHour, colonVisible ? ':' : ' ', now.minute());
    sprintf(ampmBuffer, "%s", isPM ? "PM" : "AM");
  } else {
    // 24-hour format
    sprintf(timeBuffer, "%02d%c%02d", displayHour, colonVisible ? ':' : ' ', now.minute());
  }
  
  // Format date based on user preference
  DateFormat dateFormat = getDateFormat();
  switch (dateFormat) {
    case DATE_DDMMYYYY:
      sprintf(dateBuffer, "%02d/%02d/%04d", now.day(), now.month(), now.year());
      break;
    case DATE_MMDDYYYY:
      sprintf(dateBuffer, "%02d/%02d/%04d", now.month(), now.day(), now.year());
      break;
    case DATE_YYYYMMDD:
      sprintf(dateBuffer, "%04d-%02d-%02d", now.year(), now.month(), now.day());
      break;
    case DATE_DAYNAME:
      sprintf(dateBuffer, "%s %02d %s", dayNames[now.dayOfTheWeek()], now.day(), monthNames[now.month() - 1]);
      break;
  }
  
  // Format temperature (DS3231 provides temp in Celsius)
  sprintf(tempBuffer, "%.1fC", temperature);
  
  // Clear areas for dynamic content (time, date, temperature, AM/PM)
  tft.fillRect(20, 90, 200, 40, ST77XX_BLACK);  // Time area
  tft.fillRect(40, 140, 160, 20, ST77XX_BLACK); // AM/PM area
  tft.fillRect(30, 170, 180, 20, ST77XX_BLACK); // Date area
  tft.fillRect(60, 200, 120, 20, ST77XX_BLACK); // Temperature area
  
  // Erase previous second indicator dot
  if (lastSecondX >= 0 && lastSecondY >= 0) {
    tft.fillCircle(lastSecondX, lastSecondY, dotRadius + 1, ST77XX_BLACK);
  }
  
  // Calculate new second indicator position
  // Angle: 0 degrees = top (12 o'clock), rotates clockwise
  float angle = (now.second() / 60.0) * 2.0 * PI - (PI / 2.0);  // -90° offset to start at top
  int secondX = centerX + (int)(radius * cos(angle));
  int secondY = centerY + (int)(radius * sin(angle));
  
  // Draw new second indicator dot
  tft.fillCircle(secondX, secondY, dotRadius, ST77XX_YELLOW);
  tft.drawCircle(secondX, secondY, dotRadius + 1, ST77XX_ORANGE);  // Outline for visibility
  
  // Store position for next erase
  lastSecondX = secondX;
  lastSecondY = secondY;
  
  // Display time (HH:MM)
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(35, 95);
  tft.print(timeBuffer);
  
  // Display AM/PM if 12-hour format
  if (timeFormat == TIME_12H && strlen(ampmBuffer) > 0) {
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_MAGENTA);
    tft.setCursor(100, 145);
    tft.print(ampmBuffer);
  }
  
  // Display date
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(dateFormat == DATE_DAYNAME ? 40 : 50, 170);
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
