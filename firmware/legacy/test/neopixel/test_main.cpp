#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ============================================
// ESP32 NeoPixel Configuration - TEST FILE
// ============================================
// GPIO16 (RXD_2) - Safe for ESP32 NeoPixel data line
// Located on right side of board, near bottom
#define LED_PIN     16      // GPIO16 (RXD_2) - Safe choice near boot button area
#define NUM_LEDS    5

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  strip.begin();
  strip.show(); // All off
  
  Serial.println("\n====================================");
  Serial.println("NeoPixel Strip Test - GPIO16");
  Serial.println("====================================");
}

/* ===========================
   BOOTUP - Eye Catching Startup
   =========================== */
void bootupSequence() {
  Serial.println("▶ BOOTUP SEQUENCE");
  // All LEDs light up in red one by one
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    strip.show();
    delay(100);
  }
  delay(300);
  // All LEDs pulse together
  for (int pulse = 0; pulse < 3; pulse++) {
    for (int brightness = 0; brightness <= 255; brightness += 25) {
      strip.setBrightness(brightness);
      strip.show();
      delay(20);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 25) {
      strip.setBrightness(brightness);
      strip.show();
      delay(20);
    }
  }
  strip.setBrightness(255);
  strip.clear();
  strip.show();
}

/* ===========================
   FIRE EFFECT - Red/Orange Flames
   =========================== */
void fireEffect() {
  Serial.println("▶ FIRE EFFECT");
  for (int frame = 0; frame < 15; frame++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Random brightness for flicker effect
      int brightness = random(100, 256);
      int red = 255;
      int green = random(50, 150);  // Orange flicker
      int blue = 0;
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
    delay(100);
  }
}

/* ===========================
   POLICE FLASHER - Red & Blue
   =========================== */
void policeFlasher() {
  Serial.println("▶ POLICE FLASHER");
  for (int cycle = 0; cycle < 6; cycle++) {
    // Red pulse
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    }
    strip.show();
    delay(200);
    
    strip.clear();
    strip.show();
    delay(100);
    
    // Blue pulse
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
    }
    strip.show();
    delay(200);
    
    strip.clear();
    strip.show();
    delay(100);
  }
}

/* ===========================
   1. Solid Colors
   =========================== */
void solidColors() {
  Serial.println("▶ Solid Colors");
  uint32_t colors[] = {
    strip.Color(255, 0, 0),    // Red
    strip.Color(0, 255, 0),    // Green
    strip.Color(0, 0, 255),    // Blue
    strip.Color(255, 255, 0),  // Yellow
    strip.Color(255, 0, 255),  // Purple
    strip.Color(0, 255, 255),  // Cyan
    strip.Color(255, 255, 255) // White
  };

  for (int c = 0; c < 7; c++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, colors[c]);
    }
    strip.show();
    delay(500);
  }
}

/* ===========================
   2. Blink Colors
   =========================== */
void blinkColors() {
  Serial.println("▶ Blink Colors");
  uint32_t colors[] = {
    strip.Color(255, 0, 0),
    strip.Color(0, 255, 0),
    strip.Color(0, 0, 255)
  };

  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 3; i++) {
      for (int p = 0; p < NUM_LEDS; p++) {
        strip.setPixelColor(p, colors[c]);
      }
      strip.show();
      delay(300);

      strip.clear();
      strip.show();
      delay(300);
    }
  }
}

/* ===========================
   3. Breathing Effect
   =========================== */
void breathingEffect() {
  Serial.println("▶ Breathing Effect");
  for (int b = 0; b <= 255; b += 5) {
    uint32_t col = strip.Color(0, 0, b);
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, col);
    }
    strip.setBrightness(b);
    strip.show();
    delay(30);
  }

  for (int b = 255; b >= 0; b -= 5) {
    uint32_t col = strip.Color(0, 0, b);
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, col);
    }
    strip.setBrightness(b);
    strip.show();
    delay(30);
  }

  strip.setBrightness(255); // reset
}

/* ===========================
   4. Color Wipe
   =========================== */
void colorWipe() {
  Serial.println("▶ Color Wipe");
  uint32_t colors[] = {
    strip.Color(255, 0, 0),
    strip.Color(0, 255, 0),
    strip.Color(0, 0, 255)
  };

  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, colors[c]);
      strip.show();
      delay(150);
    }
    strip.clear();
    strip.show();
    delay(200);
  }
}

/* ===========================
   5. Rainbow Cycle
   =========================== */
void rainbowCycle() {
  Serial.println("▶ Rainbow Cycle");
  for (int j = 0; j < 256; j += 8) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.ColorHSV(((i * 40) + j) * 256, 255, 255));
    }
    strip.show();
    delay(50);
  }
}

/* ===========================
   6. Theater Chase
   =========================== */
void theaterChase() {
  Serial.println("▶ Theater Chase");
  uint32_t white = strip.Color(255, 255, 255);

  for (int repeat = 0; repeat < 10; repeat++) {
    for (int offset = 0; offset < 3; offset++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        if ((i + offset) % 3 == 0) {
          strip.setPixelColor(i, white);
        } else {
          strip.setPixelColor(i, 0);
        }
      }
      strip.show();
      delay(200);
    }
  }
}

void loop() {
  bootupSequence();      // Eye-catching startup
  fireEffect();          // Red/orange flames
  policeFlasher();       // Red & blue police lights
  rainbowCycle();
  theaterChase();
  solidColors();
  breathingEffect();
  colorWipe();
  blinkColors();
}
