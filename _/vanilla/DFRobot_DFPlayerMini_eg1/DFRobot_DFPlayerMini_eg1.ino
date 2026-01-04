#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // TX =10, RX =11
DFRobotDFPlayerMini player;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Initializing DFPlayer...");

  if (!player.begin(mySerial)) {
    Serial.println("DFPlayer not responding!");
    while (true);
  }

  Serial.println("DFPlayer OK!");
  player.volume(30);  // 0-30
  player.play(2);     // Plays 0001.mp3
}

void loop() {
}
