#define TFT_DC    7
#define TFT_RST   8
#define SCR_WD   240
#define SCR_HT   240

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <Wire.h>
#include <RTClib.h>

// --- Display object ---
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

// --- RTC object ---
RTC_DS3231 rtc;

// --- Temperature tracking ---
float previousTemp = -100.0;
float tempC = 0;
float minTemp = 200;
float maxTemp = -200;

// --- Time tracking ---
int prevHour = -1;
int prevMinute = -1;
int prevSecond = -1;

// --- Display coordinates ---
#define TEMP_X   20
#define TEMP_Y   85
#define TIME_X   10
#define TIME_Y   20

void setup() {
  Serial.begin(9600);
  Serial.println("Starting RTC temperature + clock display...");

  // --- Init display with reset toggle ---
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  delay(50);
  digitalWrite(TFT_RST, HIGH);
  delay(50);

  tft.init(SCR_WD, SCR_HT);
  tft.fillScreen(BLACK);

  // --- Init RTC ---
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- Static display headers ---
  tft.setCursor(0, 0);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.println("Temp / Clock");

  // MIN/MAX labels
  tft.setCursor(40, 180);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("MIN");
  printMinTempDegreesSymbol();

  tft.setCursor(170, 180);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println("MAX");
  printMaxTempDegreesSymbol();
}

void loop() {
  delay(1000);

  // --- Read RTC temperature ---
  previousTemp = tempC;
  tempC = rtc.getTemperature();

  // --- Update min/max ---
  if(tempC < minTemp) { deleteMinTemp(); minTemp = tempC; }
  if(tempC > maxTemp) { deleteMaxTemp(); maxTemp = tempC; }

  // --- Update temperature display ---
  if(previousTemp != tempC) {
    deletePreviousTemp();
    printTemp();
    printMinTemp();
    printMaxTemp();
  }

  // --- Read current time ---
  DateTime now = rtc.now();
  int h = now.hour();
  int m = now.minute();
  int s = now.second();

  // --- Update clock only if changed ---
  if(h != prevHour || m != prevMinute || s != prevSecond) {
    prevHour = h;
    prevMinute = m;
    prevSecond = s;
    displayTime(h, m, s);
  }
}

// ---------------- Display helper functions ----------------
void deletePreviousTemp() {
  tft.setCursor(TEMP_X, TEMP_Y);
  tft.setTextSize(5);
  tft.setTextColor(BLACK);
  tft.println(previousTemp, 1);
}

void printTemp() {
  tft.setCursor(TEMP_X, TEMP_Y);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.println(tempC, 1);

  // Degree symbol
  tft.setCursor(TEMP_X + 140, TEMP_Y);
  tft.println((char)247);
  tft.setCursor(TEMP_X + 160, TEMP_Y);
  tft.println("C");
}

void printMinTemp() {
  tft.setCursor(10, 210);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println(minTemp,1);
}

void printMaxTemp() {
  tft.setCursor(150, 210);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println(maxTemp,1);
}

void deleteMaxTemp() {
  tft.setCursor(150, 210);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.println(maxTemp,1);
}

void deleteMinTemp() {
  tft.setCursor(10, 210);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.println(minTemp,1);
}

void printMinTempDegreesSymbol() {
  tft.setCursor(70, 210);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println((char)247);
  tft.setCursor(85, 210);
  tft.println("C");
}

void printMaxTempDegreesSymbol() {
  tft.setCursor(210, 210);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println((char)247);
  tft.setCursor(225, 210);
  tft.println("C");
}

void displayTime(int h, int m, int s) {
  tft.setCursor(TIME_X, TIME_Y);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK); // White text, black background to erase previous time

  if(h < 10) tft.print('0');
  tft.print(h);
  tft.print(':');

  if(m < 10) tft.print('0');
  tft.print(m);
  tft.print(':');

  if(s < 10) tft.print('0');
  tft.print(s);
}
