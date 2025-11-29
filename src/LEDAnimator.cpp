#include "LEDAnimator.h"
#include <Arduino.h>
#include <math.h>

LEDAnimator::LEDAnimator(uint8_t pin, uint16_t count)
    : strip(count, pin, NEO_GRB + NEO_KHZ800) {}

void LEDAnimator::begin() {
    strip.begin();
    strip.show();
    strip.setBrightness(50);
}

void LEDAnimator::update() {
    unsigned long t = millis();
    float phase = t / 2000.0 * TWO_PI;   // 2 s full cycle
    int brightness = 10 + (int)(100 * (1 + sin(phase)) / 2);  // breathing curve
    uint32_t color = strip.Color(0, brightness, brightness);  // cyan
    for (uint8_t i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, color);
    strip.show();
}