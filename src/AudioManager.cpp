#include "AudioManager.h"
#include <Arduino.h>

AudioManager::AudioManager(uint8_t rxPin, uint8_t txPin)
    : mp3Serial(rxPin, txPin) {}

bool AudioManager::begin() {
    mp3Serial.begin(9600);
    delay(200);
    if (!player.begin(mp3Serial)) {
        Serial.println(F("⚠️ DFPlayer not found!"));
        return false;
    }
    player.volume(5);
    ready = true;
    Serial.println(F("DFPlayer initialized."));
    return true;
}

void AudioManager::playStartupSound() {
    if (ready) {
        player.play(1);     // Play file 0001.mp3
        Serial.println(F("Playing startup sound..."));
    }
}