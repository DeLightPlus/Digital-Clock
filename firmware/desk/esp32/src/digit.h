#pragma once

#include <Adafruit_NeoPixel.h>

void showDigit(
    Adafruit_NeoPixel& strip,
    int digit,
    int offset = 0
);

void showSegment(
    Adafruit_NeoPixel& strip,
    int segment,
    int offset = 0
);
