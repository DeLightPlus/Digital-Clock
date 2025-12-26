/*
  InspectiGO – Digital Clock (Stable Baseline)
  -------------------------------------------
  - SSD1306 OLED (I2C)
  - DS3231 RTC (RTClib)
  - NeoPixel breathing (low current)
  - Boot animation on power-up

  ❌ No buttons
  ❌ No DFPlayer
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// -------------------- OLED --------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------------------- RTC ---------------------
RTC_DS3231 rtc;

// -------------------- NeoPixel ----------------
#define LED_PIN   6
#define LED_COUNT 5
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------------------- State -------------------
unsigned long lastBlink = 0;
bool colonVisible = true;

// ================= BOOT ANIMATION =================
void bootAnimation() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);

  const char* frames[] = {
    "-------",
    "- _ -",
    "o _ o",
    "- _ -",
    "O _ O"
  };

  for (int i = 0; i < 5; i++) {
    display.clearDisplay();

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(frames[i], 0, 0, &x1, &y1, &w, &h);

    display.setCursor((SCREEN_WIDTH - w) / 2,
                      (SCREEN_HEIGHT - h) / 2);

    display.print(frames[i]);
    display.display();

    // subtle NeoPixel pulse (safe brightness)
    strip.setPixelColor(0, strip.Color(0, 10 + i * 8, 10 + i * 8));
    strip.show();

    delay(220);
  }

  strip.clear();
  strip.show();

  delay(300);
  display.clearDisplay();
  display.display();
}

// ================= SETUP =================
void setup() {
  Wire.begin();

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // OLED is mandatory
  }

  // NeoPixel
  strip.begin();
  strip.setBrightness(40);  // LOW current
  strip.show();

  // Boot animation ✨
  bootAnimation();

  // RTC
  if (!rtc.begin()) {
    while (true); // RTC is mandatory
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

// ================= LOOP =================
void loop() {
  DateTime now = rtc.now();
  unsigned long ms = millis();

  // Blink colon every 500 ms
  if (ms - lastBlink > 500) {
    colonVisible = !colonVisible;
    lastBlink = ms;
  }

  // -------- OLED --------
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Time
  display.setTextSize(2);
  display.setCursor(18, 22);

  char timeStr[6];
  sprintf(timeStr, "%02d%c%02d",
          now.hour(),
          colonVisible ? ':' : ' ',
          now.minute());

  display.print(timeStr);

  // Seconds
  display.setTextSize(1);
  display.setCursor(102, 8);
  display.print(now.second());

  // Date
  display.setCursor(34, 48);
  display.print(now.day());
  display.print("/");
  display.print(now.month());
  display.print("/");
  display.print(now.year());

  display.display();

  // -------- NeoPixel Breathing --------
  float phase = ms / 2000.0 * TWO_PI;
  int breath = 5 + (int)(25 * (1 + sin(phase)) / 2);

  strip.setPixelColor(0, strip.Color(0, breath, breath));
  strip.show();

  delay(30);
}
