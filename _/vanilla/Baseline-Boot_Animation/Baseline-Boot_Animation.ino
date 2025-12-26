/*
  InspectiGO – Cinematic CRT Boot Animation (FULLY FIXED)
  --------------------------------------------------------
  - SSD1306 OLED 128x64
  - DS3231 RTC
  - NeoPixel breathing (safe)
  
  Boot Sequence:
  1. Center dot appears
  2. Dot becomes small line
  3. Line expands slowly (with LED glow)
  4. FULL WHITE FLASH (1 second)
  5. Face animation (5 seconds total)
  6. Fade to clock display
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- RTC -----------
RTC_DS3231 rtc;

// ---------- NeoPixel ------
#define LED_PIN   6
#define LED_COUNT 5
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---------- Clock state ---
unsigned long lastBlink = 0;
bool colonVisible = true;

// =======================================================
// BOOT ANIMATION - FULLY FIXED
// =======================================================
void bootAnimation() {
  
  int cx = SCREEN_WIDTH / 2;   // Center X = 64
  int cy = SCREEN_HEIGHT / 2;  // Center Y = 32

  // Force display to be ready
  display.clearDisplay();
  display.display();
  delay(100);

  // ========================================
  // 1️⃣ CENTER DOT (900ms)
  // ========================================
  display.clearDisplay();
  display.drawPixel(cx, cy, SSD1306_WHITE);
  display.drawPixel(cx-1, cy, SSD1306_WHITE);  // Make it slightly bigger
  display.drawPixel(cx+1, cy, SSD1306_WHITE);
  display.drawPixel(cx, cy-1, SSD1306_WHITE);
  display.drawPixel(cx, cy+1, SSD1306_WHITE);
  display.display();
  delay(900);

  // ========================================
  // 2️⃣ SMALL LINE AROUND DOT (expand 1-6 pixels)
  // ========================================
  for (int w = 2; w <= 8; w++) {
    display.clearDisplay();
    // Draw horizontal line
    display.drawFastHLine(cx - w, cy, w * 2 + 1, SSD1306_WHITE);
    // Add vertical thickness
    display.drawFastHLine(cx - w, cy - 1, w * 2 + 1, SSD1306_WHITE);
    display.drawFastHLine(cx - w, cy + 1, w * 2 + 1, SSD1306_WHITE);
    display.display();
    delay(180);
  }

  // ========================================
  // 3️⃣ EXPANDING LINE (SLOW + LED GLOW)
  // ========================================
  for (int w = 9; w <= cx; w += 2) {
    display.clearDisplay();
    // Draw thicker line for visibility
    display.drawFastHLine(cx - w, cy - 1, w * 2, SSD1306_WHITE);
    display.drawFastHLine(cx - w, cy, w * 2, SSD1306_WHITE);
    display.drawFastHLine(cx - w, cy + 1, w * 2, SSD1306_WHITE);
    display.display();

    // Subtle cyan glow building up
    int glow = constrain(5 + w / 4, 5, 50);
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(0, glow, glow));
    }
    strip.show();

    delay(70);
  }

  delay(600);  // Pause at full line

  // ========================================
  // 4️⃣ FULL WHITE FLASH (1 SECOND)
  // ========================================
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  
  // Brighten ALL LEDs during flash
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(200, 200, 200));
  }
  strip.show();
  
  delay(1000);  // 1 second white flash

  // ========================================
  // 5️⃣ CLEAR TO BLACK
  // ========================================
  display.clearDisplay();
  display.display();
  strip.clear();
  strip.show();
  delay(400);

  // ========================================
  // 6️⃣ FACE ANIMATION (5 SECONDS TOTAL)
  // ========================================
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);

  // Boot sequence faces (first 3 are fixed)
  const char* bootFaces[] = {
    "- _ -",     // Sleepy
    "o _ o",     // Waking
    "- _ -"      // Blink
  };

  // Random final faces (pick one for 4th frame)
  const char* finalFaces[] = {
    "O _ O",     // Awake
    "> _ <",     // Happy
    "^ _ ^",     // Joy
    "@ _ @",     // Surprised
    "* _ *"      // Sparkle
  };

  // Seed random with current time
  randomSeed(millis());
  int randomIndex = random(0, 5);

  // Show first 3 faces (fixed sequence)
  for (int i = 0; i < 3; i++) {
    display.clearDisplay();

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(bootFaces[i], 0, 0, &x1, &y1, &w, &h);

    display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
    display.print(bootFaces[i]);
    display.display();

    // Pulse LED with face
    int brightness = (i + 1) * 10;
    for (int j = 0; j < LED_COUNT; j++) {
      strip.setPixelColor(j, strip.Color(0, brightness, brightness));
    }
    strip.show();

    delay(1250);
  }

  // Show random final face (4th frame)
  display.clearDisplay();
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(finalFaces[randomIndex], 0, 0, &x1, &y1, &w, &h);
  
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
  display.print(finalFaces[randomIndex]);
  display.display();

  // Bright LED for final face
  for (int j = 0; j < LED_COUNT; j++) {
    strip.setPixelColor(j, strip.Color(0, 40, 40));
  }
  strip.show();

  delay(1250);

  // ========================================
  // 7️⃣ FADE OUT PAUSE
  // ========================================
  display.clearDisplay();
  display.display();
  strip.clear();
  strip.show();
  delay(700);
}

// =======================================================
// SETUP
// =======================================================
void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize OLED with explicit settings
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  // CRITICAL: Set contrast high for visibility
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0xFF);  // Max contrast

  // Clear any garbage
  display.clearDisplay();
  display.display();
  delay(200);

  // Initialize NeoPixel
  strip.begin();
  strip.setBrightness(35);
  strip.clear();
  strip.show();

  // 🎬 PLAY BOOT ANIMATION
  bootAnimation();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (true);
  }

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, setting time"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

// =======================================================
// LOOP – CLOCK DISPLAY
// =======================================================
void loop() {
  DateTime now = rtc.now();
  unsigned long ms = millis();

  // Colon blink every 500ms
  if (ms - lastBlink > 500) {
    colonVisible = !colonVisible;
    lastBlink = ms;
  }

  // ========================================
  // OLED CLOCK DISPLAY
  // ========================================
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Main time (HH:MM)
  display.setTextSize(2);
  display.setCursor(18, 22);

  char timeStr[6];
  sprintf(timeStr, "%02d%c%02d",
          now.hour(),
          colonVisible ? ':' : ' ',
          now.minute());
  display.print(timeStr);

  // Seconds (top right)
  display.setTextSize(1);
  display.setCursor(102, 8);
  display.print(now.second());

  // Date (bottom)
  display.setCursor(34, 48);
  display.print(now.day());
  display.print("/");
  display.print(now.month());
  display.print("/");
  display.print(now.year());

  display.display();

  // ========================================
  // BREATHING LED (SUBTLE CYAN)
  // ========================================
  float phase = ms / 2500.0 * TWO_PI;
  int brightness = 5 + (int)(20 * (1 + sin(phase)) / 2);
  strip.setPixelColor(0, strip.Color(0, brightness, brightness));
  strip.show();

  delay(30);
}