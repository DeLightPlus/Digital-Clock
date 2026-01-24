// config.h - Pin definitions and hardware constants
#ifndef CONFIG_H
#define CONFIG_H

// TFT Display Pins (ST7789 240x240)
#define TFT_DC    2
#define TFT_RST   4
#define TFT_CS    -1      // Tie CS to GND if breakout has no CS
#define TFT_MOSI  23
#define TFT_SCLK  18
#define TFT_BL    32

// RTC Pins (DS3231 I2C)
#define I2C_SDA   21
#define I2C_SCL   22

// NeoPixel LED Pins
#define LED_PIN   16
#define NUM_LEDS  5

// Timing constants
#define COLON_BLINK_MS  500
#define DISPLAY_UPDATE_MS 500
#define LED_BREATHE_CYCLE_MS 4000

#endif // CONFIG_H
