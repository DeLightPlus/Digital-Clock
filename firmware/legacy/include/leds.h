// leds.h - NeoPixel LED control interface
#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

// Initialize NeoPixel strip
void initLEDs();

// Update LED breathing animation
void updateLEDs();

// Set all LEDs to a specific color
void setLEDColor(uint8_t r, uint8_t g, uint8_t b);

// Turn off all LEDs
void clearLEDs();

#endif // LEDS_H
