// DisplaySettings.cpp - Time and date format settings implementation
#include "DisplaySettings.h"

static Preferences preferences;
static TimeFormat currentTimeFormat = TIME_24H;
static DateFormat currentDateFormat = DATE_DDMMYYYY;

void initDisplaySettings() {
  preferences.begin("display-settings", false);
  
  // Load saved settings or use defaults
  currentTimeFormat = (TimeFormat)preferences.getUChar("time_format", TIME_24H);
  currentDateFormat = (DateFormat)preferences.getUChar("date_format", DATE_DDMMYYYY);
  
  Serial.println("\n=== Display Settings ===");
  Serial.print("Time format: ");
  Serial.println(currentTimeFormat == TIME_12H ? "12-hour (AM/PM)" : "24-hour");
  Serial.print("Date format: ");
  switch (currentDateFormat) {
    case DATE_DDMMYYYY: Serial.println("DD/MM/YYYY"); break;
    case DATE_MMDDYYYY: Serial.println("MM/DD/YYYY"); break;
    case DATE_YYYYMMDD: Serial.println("YYYY-MM-DD"); break;
    case DATE_DAYNAME: Serial.println("Day Name"); break;
  }
}

TimeFormat getTimeFormat() {
  return currentTimeFormat;
}

DateFormat getDateFormat() {
  return currentDateFormat;
}

void setTimeFormat(TimeFormat format) {
  currentTimeFormat = format;
  preferences.putUChar("time_format", format);
  Serial.print("Time format updated to: ");
  Serial.println(format == TIME_12H ? "12-hour" : "24-hour");
}

void setDateFormat(DateFormat format) {
  currentDateFormat = format;
  preferences.putUChar("date_format", format);
  Serial.print("Date format updated to: ");
  Serial.println(format);
}
