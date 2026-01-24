#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_DC    2      // TFT DC pin
#define TFT_RST   4      // TFT RST pin
#define TFT_CS    -1     // TFT CS pin (tie to GND if the breakout has no CS)
#define TFT_MOSI  23     // TFT data in
#define TFT_SCLK  18     // TFT clock
#define TFT_BL    32     // Backlight control (set HIGH to turn on)

// Initialize ST7789 TFT module
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void displayText(int x, int y, const char* text, int size, uint16_t color);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== Text Display Test ===");

  // Ensure backlight is on
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Bring up hardware SPI explicitly on VSPI pins
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  
  // Initialize the display
  Serial.println("Initializing display...");
  // Most ST7789 panels expect SPI mode 3; mode 2 failed in your last test
  tft.init(240, 240, SPI_MODE3);
  Serial.println("Display initialized");
  
  tft.setRotation(1);

  // Set the display to black
  Serial.println("Filling screen black...");
  tft.fillScreen(ST77XX_BLACK);
  delay(500);
  
  Serial.println("Displaying text...");
  
  // Display text in different font sizes and colors
  displayText(0, 0, "Small", 1, ST77XX_WHITE);
  delay(1000);
  
  displayText(0, 30, "Medium", 2, ST77XX_RED);
  delay(1000);
  
  displayText(0, 70, "Large", 3, ST77XX_GREEN);
  delay(1000);
  
  displayText(0, 120, "X-Large", 4, ST77XX_BLUE);
  delay(1000);
  
  displayText(0, 180, "XX-Large", 5, ST77XX_CYAN);
  delay(1000);
  
  Serial.println("Text display cycle complete");
}

void loop() {
  // Nothing to do in the loop
  delay(100);
}

void displayText(int x, int y, const char* text, int size, uint16_t color) {
  tft.setTextSize(size);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
  Serial.print("Displayed: ");
  Serial.print(text);
  Serial.print(" at size ");
  Serial.print(size);
  Serial.print(" at (");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(")");
}
