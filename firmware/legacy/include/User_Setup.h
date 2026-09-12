// User_Setup.h - TFT_eSPI Configuration for ESP32 + ST7789
// This file configures TFT_eSPI for your GMT130-V1.0 display

#ifndef USER_SETUP_H
#define USER_SETUP_H

// ==========================================
// Display Driver IC Configuration
// ==========================================
#define ST7789_DRIVER  // 240x240 ST7789V display

// ==========================================
// Display Resolution
// ==========================================
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// ==========================================
// PIN DEFINITIONS FOR ESP32
// ==========================================
#define TFT_MOSI 23  // SDA - MOSI (GPIO23)
#define TFT_SCLK 18  // SCK - Clock (GPIO18)
#define TFT_CS   -1  // CS - Not used (always selected)
#define TFT_DC   2   // DC - Data/Command (GPIO2)
#define TFT_RST  4   // RES - Reset (GPIO4)
#define TFT_BL   32  // BLK - Backlight PWM (GPIO32)

// ==========================================
// SPI Speed Configuration
// ==========================================
#define SPI_FREQUENCY 40000000  // 40 MHz for fast updates
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000

// ==========================================
// Display Orientation & Color
// ==========================================
#define TFT_BL_ON HIGH           // Backlight on level
#define TFT_BACKLIGHT_ON HIGH
#define TFT_RGB_ORDER TFT_BGR    // ST7789 panels expect BGR order (fixes swapped/odd colors)
#define TFT_INVERT_COLORS 0      // Set to 1 if your panel ships inverted

// ==========================================
// Rotation
// ==========================================
// 0 = Portrait
// 1 = Landscape (90°)
// 2 = Portrait Inverted (180°)
// 3 = Landscape Inverted (270°)
#define TFT_ROTATION 0

// ==========================================
// Smooth Font Support
// ==========================================
#define SMOOTH_FONT

// ==========================================
// Hardware SPI Configuration
// ==========================================
#define USE_HSPI_PORT

#endif // USER_SETUP_H
