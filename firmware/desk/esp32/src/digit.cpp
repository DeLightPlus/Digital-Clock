#include "digit.h"

// Physical LED order:
//
// 0 = C
// 1 = D
// 2 = E
// 3 = F
// 4 = A
// 5 = B
// 6 = G
//
// Segment order:
//
// C D E F A B G

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


void showDigit(
    Adafruit_NeoPixel& strip,
    int digit,
    int offset
) {

    for (int i = 0; i < 7; i++) {

        if (DIGITS[digit][i]) {

            strip.setPixelColor(
                offset + i,
                strip.Color(0, 0, 255)
            );

        } else {

            strip.setPixelColor(
                offset + i,
                0
            );
        }
    }

    strip.show();
}


void showSegment(
    Adafruit_NeoPixel& strip,
    int segment,
    int offset
) {

    for (int i = 0; i < 7; i++) {
        strip.setPixelColor(offset + i, 0);
    }

    strip.setPixelColor(
        offset + segment,
        strip.Color(255, 0, 0)
    );

    strip.show();
}
