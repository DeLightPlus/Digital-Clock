#pragma once
#include <Adafruit_SSD1306.h>

class FaceAnimator {
public:
    FaceAnimator(Adafruit_SSD1306 &display);
    
    void animate();           // Run one full cycle of 3 faces + random final
    void setDelay(uint16_t ms);
    void setTextSize(uint8_t size);
    
private:
    Adafruit_SSD1306 &display;
    uint16_t frameDelay = 1250;
    uint8_t textSize = 4;
    
    static constexpr uint8_t SCREEN_WIDTH = 128;
    static constexpr uint8_t SCREEN_HEIGHT = 64;
    
    void drawFace(const char* face);
};

