#include <Adafruit_NeoPixel.h>

#define LED_PIN   4
#define NUM_LEDS  7

Adafruit_NeoPixel strip(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// Physical LED order:
//
// LED 0 = C
// LED 1 = D
// LED 2 = E
// LED 3 = F
// LED 4 = A
// LED 5 = B
// LED 6 = G
//
//        A
//     ───────
//   F │       │ B
//     │   G   │
//     ───────
//   E │       │ C
//     │       │
//     ───────
//        D

const bool DIGITS[10][7] = {

  // C D E F A B G

  // 0
  {1, 1, 1, 1, 1, 1, 0},

  // 1
  {1, 0, 0, 0, 0, 1, 0},

  // 2
  {0, 1, 1, 0, 1, 1, 1},

  // 3
  {1, 1, 0, 0, 1, 1, 1},

  // 4
  {1, 0, 0, 1, 0, 1, 1},

  // 5
  {1, 1, 0, 1, 1, 0, 1},

  // 6
  {1, 1, 1, 1, 1, 0, 1},

  // 7
  {1, 0, 0, 0, 1, 1, 0},

  // 8
  {1, 1, 1, 1, 1, 1, 1},

  // 9
  {1, 1, 0, 1, 1, 1, 1}
};


void clearDisplay() {
  strip.clear();
  strip.show();
}


void showSegment(int led) {

  strip.clear();

  strip.setPixelColor(
    led,
    strip.Color(255, 0, 0)
  );

  strip.show();

  delay(700);
}


void showDigit(int digit) {

  strip.clear();

  for (int led = 0; led < 7; led++) {

    if (DIGITS[digit][led]) {

      strip.setPixelColor(
        led,
        strip.Color(0, 0, 255)
      );
    }
  }

  strip.show();

  delay(1000);
}


void setup() {

  strip.begin();

  strip.setBrightness(40);

  strip.show();
}


void loop() {

  // --------------------------------
  // TEST 1
  // Walk physical LEDs
  // C → D → E → F → A → B → G
  // --------------------------------

  for (int led = 0; led < 7; led++) {

    showSegment(led);
  }

  delay(500);


  // --------------------------------
  // TEST 2
  // Display 0 → 9
  // --------------------------------

  for (int digit = 0; digit <= 9; digit++) {

    showDigit(digit);
  }

  delay(1000);
}
