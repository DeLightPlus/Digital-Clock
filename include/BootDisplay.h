#pragma once

#include <Adafruit_SSD1306.h>
#include "LEDAnimator.h"

class BootDisplay {
public:
    BootDisplay(Adafruit_SSD1306 &oled);

    void begin();
    void play();          // blocking cinematic boot
    bool isFinished() const;

private:
    Adafruit_SSD1306 &display;

    bool finished = false;

    void drawCenterDot();
    void drawExpandingLine();
    void whiteFlash();
    void faceAnimation();
};

