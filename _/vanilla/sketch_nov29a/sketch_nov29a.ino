/*
  🎯 Best-Practice Modular Startup Example
  - OLED (SSD1306) via I2C
  - DS3231 RTC
  - DFPlayer Mini (SoftwareSerial)
  - NeoPixel breathing LED

  ✅ Stable startup order to prevent I2C lock or serial freeze
  ✅ Non-blocking animation & display updates
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;
Adafruit_NeoPixel strip(5, 6, NEO_GRB + NEO_KHZ800);

SoftwareSerial mp3Serial(8, 9);  // RX, TX
DFRobotDFPlayerMini mp3;

// State tracking
unsigned long lastToggle = 0, lastColonBlink = 0;
bool showDate = false, colonVisible = true;

void safeInitOLED() {
  Serial.print(F("Initializing OLED... "));
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("⚠️  OLED not detected"));
  } else {
    Serial.println(F("OLED ready"));
    display.clearDisplay();
    display.display();
  }
}

void safeInitRTC() {
  Serial.print(F("Initializing RTC... "));
  if (!rtc.begin()) {
    Serial.println(F("❌ RTC not found"));
  } else {
    Serial.println(F("RTC ready"));
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      Serial.println(F("RTC reset to compile time"));
    }
  }
}

void safeInitMP3() {
  Serial.print(F("Initializing DFPlayer... "));
  if (mp3.begin(mp3Serial)) {
    Serial.println(F("DFPlayer ready, playing track 1"));
    mp3.volume(20);
    mp3.play(1);
  } else {
    Serial.println(F("❌ DFPlayer init failed"));
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("Booting..."));

  Wire.begin();                   // 1️⃣ I2C first
  safeInitOLED();                 // 2️⃣ OLED next
  safeInitRTC();                  // 3️⃣ RTC next
  strip.begin();                  // 4️⃣ LED init
  strip.show();
  delay(100);

  mp3Serial.begin(9600);          // 5️⃣ Serial last
  safeInitMP3();

  Serial.println(F("Initialization complete.\n---"));
}

void loop() {
  unsigned long nowMs = millis();
  DateTime now = rtc.now();

  // Toggle between date/temp every 15s
  if (nowMs - lastToggle > 15000) {
    showDate = !showDate;
    lastToggle = nowMs;
  }

  // Blink colon every 500ms
  if (nowMs - lastColonBlink > 500) {
    colonVisible = !colonVisible;
    lastColonBlink = nowMs;
  }

  // --- OLED Display ---
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 24);
  char timeStr[6];
  sprintf(timeStr, "%02d%c%02d", now.hour(), colonVisible ? ':' : ' ', now.minute());
  display.print(timeStr);

  display.setTextSize(1);
  display.setCursor(98, 8);
  if (showDate) {
    const char* days[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    display.print(days[now.dayOfTheWeek()]);
    display.print(now.day());
  } else {
    float temp = rtc.getTemperature();
    char tempStr[8];
    sprintf(tempStr, "%2.1fC", temp);
    display.print(tempStr);
  }

  display.setCursor(106, 50);
  display.setTextSize(1);
  display.print(now.second());
  display.display();

  // --- NeoPixel Breathing (Cyan) ---
  float phase = nowMs / 2000.0 * TWO_PI;
  int brightness = 10 + (int)(45 * (1 + sin(phase)) / 2);
  strip.setPixelColor(0, strip.Color(0, brightness, brightness));
  strip.show();

  delay(40);
}