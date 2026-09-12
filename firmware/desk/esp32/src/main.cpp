#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "config.h"
#include "digit.h"

Adafruit_NeoPixel strip(
    NUM_LEDS,
    LED_PIN,
    NEO_GRB + NEO_KHZ800
);


void setup() {

    strip.begin();

    strip.setBrightness(LED_BRIGHTNESS);

    strip.show();
}


void loop() {

    // Physical LED test
    for (int i = 0; i < 7; i++) {

        showSegment(strip, i);

        delay(700);
    }

    delay(500);


    // Digit test
    for (int digit = 0; digit <= 9; digit++) {

        showDigit(strip, digit);

        delay(1000);
    }

    delay(1000);
}
