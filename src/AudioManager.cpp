#include "AudioManager.h"
#include <Arduino.h>

AudioManager::AudioManager(uint8_t rxPin, uint8_t txPin)
    : mp3Serial(rxPin, txPin) {}

bool AudioManager::begin() {
    mp3Serial.begin(9600);
    delay(200);
    if (!player.begin(mp3Serial)) {
        Serial.println(F("DFPlayer not found!"));
        return false;
    }
    player.volume(20);
    ready = true;
    Serial.println(F("DFPlayer initialized."));
    return true;
}

void AudioManager::playStartupSound() {
    playTrack(1);
}

void AudioManager::playTrack(uint8_t track) {
    if (ready) {
        player.play(track);
        Serial.print(F("Playing track "));
        Serial.println(track);
    }
}
