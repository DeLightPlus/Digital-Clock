#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Pins (match the working setup)
#define TFT_DC    2
#define TFT_RST   4
#define TFT_CS    -1      // Tie CS to GND if your breakout has no CS
#define TFT_MOSI  23
#define TFT_SCLK  18
#define TFT_BL    32

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

// RTC support with software fallback
RTC_DS3231 rtc;
bool rtcOk = false;
DateTime baseCompile(F(__DATE__), F(__TIME__));
unsigned long startMs = 0;

// UI state
bool colonOn = true;
unsigned long lastBlink = 0;
unsigned long lastDraw = 0;

DateTime nowTime() {
  if (rtcOk) {
    return rtc.now();
  }
  uint32_t elapsed = (millis() - startMs) / 1000;
  return DateTime(baseCompile.unixtime() + elapsed);
}

bool initRTC() {
  if (!rtc.begin()) return false;
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  return true;
}

void loader() {
  tft.fillScreen(ST77XX_BLACK);
  int x = 20, y = 110, w = 200, h = 16;
  tft.drawRoundRect(x, y, w, h, 4, ST77XX_CYAN);
  for (int i = 0; i <= w - 4; i += 8) {
    tft.fillRoundRect(x + 2, y + 2, i, h - 4, 3, ST77XX_CYAN);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(90, 90);
    tft.print("Loading...");
    tft.setCursor(110, 130);
    tft.print((i * 100) / (w - 4));
    tft.print("%");
    delay(35);
  }
  delay(350);
}

void drawClock(const DateTime &now) {
  tft.fillScreen(ST77XX_BLACK);

  // Frame
  tft.drawRoundRect(10, 20, 220, 200, 8, ST77XX_CYAN);
  tft.drawRoundRect(14, 24, 212, 192, 8, ST77XX_CYAN);

  // Time string
  char timeStr[6];
  snprintf(timeStr, sizeof(timeStr), "%02d%c%02d", now.hour(), colonOn ? ':' : ' ', now.minute());
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(4);
  tft.setCursor(40, 60);
  tft.print(timeStr);

  // Seconds
  char secStr[4];
  snprintf(secStr, sizeof(secStr), "%02d", now.second());
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(185, 75);
  tft.print(secStr);

  // Date
  char dateStr[20];
  snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", now.year(), now.month(), now.day());
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 130);
  tft.print(dateStr);

  // Label
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(18, 30);
  tft.print(rtcOk ? "RTC" : "SOFT CLOCK");

  // Simple graphic
  tft.fillCircle(200, 170, 12, ST77XX_MAGENTA);
  tft.drawCircle(200, 170, 14, ST77XX_WHITE);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== ST7789 Graphics + Clock Test ===");

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  Serial.println("Init display (SPI mode 3)...");
  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  rtcOk = initRTC();
  startMs = millis();
  Serial.print("RTC: ");
  Serial.println(rtcOk ? "OK" : "FALLBACK");

  loader();
  drawClock(nowTime());
}

void loop() {
  unsigned long ms = millis();
  if (ms - lastBlink >= 500) {
    colonOn = !colonOn;
    lastBlink = ms;
  }
  if (ms - lastDraw >= 500) {
    drawClock(nowTime());
    lastDraw = ms;
  }
}
