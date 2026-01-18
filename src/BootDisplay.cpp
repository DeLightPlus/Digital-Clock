#include "BootDisplay.h"
#include <Arduino.h>
#include "FaceAnimator.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

BootDisplay::BootDisplay(Adafruit_SSD1306 &oled)
    : display(oled) {}

void BootDisplay::begin() {
    finished = false;
}

bool BootDisplay::isFinished() const {
    return finished;
}

void BootDisplay::play() {
    int cx = SCREEN_WIDTH / 2;
    int cy = SCREEN_HEIGHT / 2;

    display.clearDisplay();
    display.display();
    delay(100);

    // 1️⃣ Center dot
    display.clearDisplay();
    display.drawPixel(cx, cy, SSD1306_WHITE);
    display.drawPixel(cx - 1, cy, SSD1306_WHITE);
    display.drawPixel(cx + 1, cy, SSD1306_WHITE);
    display.drawPixel(cx, cy - 1, SSD1306_WHITE);
    display.drawPixel(cx, cy + 1, SSD1306_WHITE);
    display.display();
    delay(900);

    // 2️⃣ Small expanding line
    for (int w = 2; w <= 8; w++) {
        display.clearDisplay();
        display.drawFastHLine(cx - w, cy, w * 2 + 1, SSD1306_WHITE);
        display.drawFastHLine(cx - w, cy - 1, w * 2 + 1, SSD1306_WHITE);
        display.drawFastHLine(cx - w, cy + 1, w * 2 + 1, SSD1306_WHITE);
        display.display();
        delay(180);
    }

    // 3️⃣ Full expansion
    for (int w = 9; w <= cx; w += 2) {
        display.clearDisplay();
        display.drawFastHLine(cx - w, cy - 1, w * 2, SSD1306_WHITE);
        display.drawFastHLine(cx - w, cy,     w * 2, SSD1306_WHITE);
        display.drawFastHLine(cx - w, cy + 1, w * 2, SSD1306_WHITE);
        display.display();
        delay(70);
    }

    delay(600);

    // 4️⃣ White flash
    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    display.display();
    delay(1000);

    display.clearDisplay();
    display.display();
    delay(400);

    // 5️⃣ Face animation using modular FaceAnimator
    FaceAnimator faceAnim(display);
    faceAnim.setDelay(1250);
    faceAnim.setTextSize(4);
    faceAnim.animate();

    display.clearDisplay();
    display.display();
    delay(700);

    finished = true;
}

void BootDisplay::drawCenterDot() {
    // Legacy method - not used
}

void BootDisplay::drawExpandingLine() {
    // Legacy method - not used
}

void BootDisplay::whiteFlash() {
    // Legacy method - not used
}

void BootDisplay::faceAnimation() {
    // Legacy method - not used (now using FaceAnimator)
}

