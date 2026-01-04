#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "DisplayManager.h"
#include "LEDAnimator.h"
#include "AudioManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// --- Hardware Objects ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;
LEDAnimator leds(6, 5);   // pin 6, 5 LEDs
AudioManager audio(10, 11); // RX=10, TX=11
DisplayManager displayManager(display, rtc, leds, &audio);

void setup()
{
    Serial.begin(9600);
    delay(300);
    Serial.println(F("InspectiGO Booting..."));

    // =============================
    // 1️⃣ CRITICAL HARDWARE INIT
    // =============================
    Wire.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("OLED not detected"));
        while (true);
    }

    // Improve contrast (important on 3.3V)
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(0xFF);

    display.clearDisplay();
    display.display();

    // LEDs are safe to init early (visual feedback)
    leds.begin();

    // =============================
    // 2️⃣ RTC INIT
    // =============================
    if (!rtc.begin())
    {
        Serial.println(F("RTC not found"));
        while (true);
    }

    if (rtc.lostPower())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        Serial.println(F("RTC reset to compile time"));
    }

    // =============================
    // 3️⃣ AUDIO INIT (no playback yet)
    // =============================
    audio.begin();  // Initialize only - audio triggers happen in BootDisplay

    // =============================
    // 4️⃣ START DISPLAY MANAGER
    // =============================
    displayManager.begin();  // Sets mode to Boot, boot animation will play in loop()

    Serial.println(F("System Ready"));
}

void loop()
{
    displayManager.update(); // Handles boot → clock transition
    leds.update();           // Breathing animation in clock mode
    delay(40);               // ~25 FPS
}

