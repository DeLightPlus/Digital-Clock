#include "FaceAnimator.h"
#include <Arduino.h>

FaceAnimator::FaceAnimator(Adafruit_SSD1306 &display)
    : display(display) {}

void FaceAnimator::setDelay(uint16_t ms) {
    frameDelay = ms;
}

void FaceAnimator::setTextSize(uint8_t size) {
    textSize = size;
}

void FaceAnimator::drawFace(const char* face) {
    display.clearDisplay();
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(face, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
    display.print(face);
    display.display();
    delay(frameDelay);
}

void FaceAnimator::animate() {
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);

    // Standard boot faces
    const char* bootFaces[] = {
        "- -",
        "o o",
        "- -"
    };

    // Random final face options
    const char* finalFaces[] = {
        "O O",
        "> <",
        "^ ^",
        "@ @",
        "* *"
    };

    randomSeed(millis());
    int finalIndex = random(0, 5);

    // First 3 faces
    for (int i = 0; i < 3; i++) {
        drawFace(bootFaces[i]);
    }

    // Random final face
    drawFace(finalFaces[finalIndex]);
}

