// DisplaySettings.h - Time and date format settings management
#ifndef DISPLAY_SETTINGS_H
#define DISPLAY_SETTINGS_H

#include <Preferences.h>

// Time format options
enum TimeFormat {
  TIME_24H = 0,  // 24-hour format (e.g., 23:45)
  TIME_12H = 1   // 12-hour format with AM/PM (e.g., 11:45 PM)
};

// Date format options
enum DateFormat {
  DATE_DDMMYYYY = 0,  // 25/01/2026
  DATE_MMDDYYYY = 1,  // 01/25/2026
  DATE_YYYYMMDD = 2,  // 2026-01-25
  DATE_DAYNAME = 3    // Sat 25 Jan
};

// Initialize settings from persistent storage
void initDisplaySettings();

// Get current time format
TimeFormat getTimeFormat();

// Get current date format
DateFormat getDateFormat();

// Set time format and save to preferences
void setTimeFormat(TimeFormat format);

// Set date format and save to preferences
void setDateFormat(DateFormat format);

#endif // DISPLAY_SETTINGS_H
