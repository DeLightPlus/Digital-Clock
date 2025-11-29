#pragma once
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class AudioManager {
public:
    AudioManager(uint8_t rxPin, uint8_t txPin);
    bool begin();
    void playStartupSound();
private:
    SoftwareSerial mp3Serial;
    DFRobotDFPlayerMini player;
    bool ready = false;
};