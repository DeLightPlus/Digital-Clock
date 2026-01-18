#pragma once
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class AudioManager {
public:
    AudioManager(uint8_t rxPin, uint8_t txPin);
    bool begin();
    void playStartupSound();    // Plays track 1 (for backward compat)
    void playTrack(uint8_t track);  // Play any track number
private:
    SoftwareSerial mp3Serial;
    DFRobotDFPlayerMini player;
    bool ready = false;
};
