#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "ClockDisplay.h"
#include "LEDAnimator.h"
#include "AudioManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// --- Hardware Objects ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;
ClockDisplay clockUI(display, rtc);
LEDAnimator leds(6, 5);             // pin 6, 5 LEDs
AudioManager audio(8, 9);           // RX=8, TX=9

// --- Time Blinker ---
unsigned long lastBlink = 0;
bool colonVisible = true;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println(F("Booting..."));

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not detected."));
  }

  if (!rtc.begin()) {
    Serial.println(F("RTC not found."));
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(F("RTC reset to compile time."));
  }

  clockUI.begin();
  leds.begin();
  if (audio.begin()) audio.playStartupSound();

  Serial.println(F("System Ready."));
}

void loop() {
  unsigned long nowMs = millis();

  if (nowMs - lastBlink >= 500) {
    colonVisible = !colonVisible;
    lastBlink = nowMs;
  }

  clockUI.showTime(colonVisible);
  leds.update();
  delay(40);
}