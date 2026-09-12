// leds.cpp - NeoPixel LED control implementation
#include "leds.h"
#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <math.h>

// NeoPixel strip object
static Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void initLEDs() {
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  Serial.println("NeoPixel LEDs initialized");
}

void updateLEDs() {
  // Smooth breathing animation in cyan to match clock theme
  float phase = (millis() % LED_BREATHE_CYCLE_MS) / (float)LED_BREATHE_CYCLE_MS;
  float brightness = 10.0f + 90.0f * (0.5f * (1.0f + sinf(phase * 6.28318530718f)));
  uint8_t b = (uint8_t)brightness;
  
  // Cyan breathing (matches clock theme)
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, b, b));
  }
  strip.show();
}

void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void clearLEDs() {
  strip.clear();
  strip.show();
}
