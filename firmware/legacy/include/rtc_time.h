// rtc_time.h - Real-time clock management interface
#ifndef RTC_TIME_H
#define RTC_TIME_H

#include <RTClib.h>

// Initialize RTC with fallback to software clock
bool initRTC();

// Get current time (from RTC or software fallback)
DateTime getCurrentTime();
// Get current temperature from DS3231 RTC (in Celsius)
float getCurrentTemperature();
// Check if RTC hardware is available
bool isRTCAvailable();

// Display RTC status label on screen
void displayRTCStatus();

#endif // RTC_TIME_H
