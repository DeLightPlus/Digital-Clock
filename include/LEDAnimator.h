#pragma once
#include <Adafruit_NeoPixel.h>

class LEDAnimator {
public:
    LEDAnimator(uint8_t pin, uint16_t count);
    void begin();
    void update();
private:
    Adafruit_NeoPixel strip;
};