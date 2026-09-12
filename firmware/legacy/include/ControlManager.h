#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <Arduino.h>
#include <RTClib.h>
#include <ArduinoJson.h>

struct AlarmEntry {
  bool active;
  uint8_t hour;
  uint8_t minute;
  char label[16];
  bool triggeredToday;
};

struct TimerState {
  bool running;
  uint32_t durationSec;
  unsigned long endMillis;
};

struct StopwatchState {
  bool running;
  unsigned long startMillis;
  unsigned long elapsedMillis;
  static const uint8_t kMaxLaps = 6;
  unsigned long laps[kMaxLaps];
  uint8_t lapCount;
};

void initControlManager();
void updateControlManager();

// Alarms
bool addAlarm(uint8_t hour, uint8_t minute, const char* label, uint8_t &outIndex);
bool deleteAlarm(uint8_t index);
void listAlarms(JsonDocument &doc);
void resetAlarmDailyFlags();

// Timer
void setTimer(uint32_t seconds);
void startTimer();
void stopTimer();
bool isTimerRunning();
uint32_t getTimerRemaining();
uint32_t getTimerDuration();

// Stopwatch
void startStopwatch();
void stopStopwatch();
void resetStopwatch();
bool lapStopwatch();
bool isStopwatchRunning();
uint32_t getStopwatchElapsed();
void getStopwatchLaps(JsonDocument &doc);

#endif // CONTROL_MANAGER_H
