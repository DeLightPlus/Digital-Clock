// ClockManager.h - Clock logic and state management interface
#ifndef CLOCK_MANAGER_H
#define CLOCK_MANAGER_H

#include <RTClib.h>

// Initialize clock manager
void initClockManager();

// Update clock state (called in loop)
void updateClock();

// Get current colon state (for display)
bool getColonVisible();

#endif // CLOCK_MANAGER_H
