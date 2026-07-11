#include "ControlManager.h"
#include "rtc_time.h"
#include "DisplayManager.h"
#include "WebManager.h"
#include <ArduinoJson.h>

extern String lastNotification;

static const uint8_t kMaxAlarms = 6;
static AlarmEntry alarms[kMaxAlarms];
static TimerState timerState = {false, 0, 0};
static StopwatchState stopwatchState = {false, 0, 0, {0}, 0};

static uint8_t lastDay = 0;  // Track last day of month for daily reset

void initControlManager() {
  for (uint8_t i = 0; i < kMaxAlarms; i++) {
    alarms[i].active = false;
    alarms[i].hour = 0;
    alarms[i].minute = 0;
    alarms[i].label[0] = '\0';
    alarms[i].triggeredToday = false;
  }
  timerState.running = false;
  stopwatchState.running = false;
  stopwatchState.elapsedMillis = 0;
  stopwatchState.lapCount = 0;
  lastDay = getCurrentTime().day();
}

void resetAlarmDailyFlags() {
  for (uint8_t i = 0; i < kMaxAlarms; i++) {
    alarms[i].triggeredToday = false;
  }
}

bool addAlarm(uint8_t hour, uint8_t minute, const char* label, uint8_t &outIndex) {
  for (uint8_t i = 0; i < kMaxAlarms; i++) {
    if (!alarms[i].active) {
      alarms[i].active = true;
      alarms[i].hour = hour;
      alarms[i].minute = minute;
      strncpy(alarms[i].label, label ? label : "", sizeof(alarms[i].label) - 1);
      alarms[i].label[sizeof(alarms[i].label) - 1] = '\0';
      alarms[i].triggeredToday = false;
      outIndex = i;
      return true;
    }
  }
  return false;
}

bool deleteAlarm(uint8_t index) {
  if (index >= kMaxAlarms) return false;
  alarms[index].active = false;
  alarms[index].label[0] = '\0';
  alarms[index].triggeredToday = false;
  return true;
}

void listAlarms(JsonDocument &doc) {
  JsonArray arr = doc.createNestedArray("alarms");
  for (uint8_t i = 0; i < kMaxAlarms; i++) {
    if (!alarms[i].active) continue;
    JsonObject o = arr.add<JsonObject>();
    o["index"] = i;
    o["hour"] = alarms[i].hour;
    o["minute"] = alarms[i].minute;
    o["label"] = alarms[i].label;
  }
}

// Timer
void setTimer(uint32_t seconds) {
  timerState.durationSec = seconds;
  timerState.running = false;
  timerState.endMillis = millis() + seconds * 1000UL;
}

void startTimer() {
  if (timerState.durationSec == 0) return;
  timerState.endMillis = millis() + timerState.durationSec * 1000UL;
  timerState.running = true;
}

void stopTimer() {
  timerState.running = false;
}

bool isTimerRunning() {
  return timerState.running;
}

uint32_t getTimerRemaining() {
  if (!timerState.running) return timerState.durationSec;
  long remaining = (long)(timerState.endMillis - millis());
  if (remaining < 0) remaining = 0;
  return (uint32_t)(remaining / 1000);
}

uint32_t getTimerDuration() {
  return timerState.durationSec;
}

// Stopwatch
void startStopwatch() {
  if (stopwatchState.running) return;
  stopwatchState.running = true;
  stopwatchState.startMillis = millis();
}

void stopStopwatch() {
  if (!stopwatchState.running) return;
  stopwatchState.elapsedMillis += millis() - stopwatchState.startMillis;
  stopwatchState.running = false;
}

void resetStopwatch() {
  stopwatchState.running = false;
  stopwatchState.elapsedMillis = 0;
  stopwatchState.lapCount = 0;
}

bool lapStopwatch() {
  if (!stopwatchState.running && stopwatchState.elapsedMillis == 0) return false;
  if (stopwatchState.lapCount >= StopwatchState::kMaxLaps) return false;
  unsigned long currentElapsed = stopwatchState.elapsedMillis;
  if (stopwatchState.running) {
    currentElapsed += millis() - stopwatchState.startMillis;
  }
  stopwatchState.laps[stopwatchState.lapCount++] = currentElapsed;
  return true;
}

bool isStopwatchRunning() {
  return stopwatchState.running;
}

uint32_t getStopwatchElapsed() {
  unsigned long currentElapsed = stopwatchState.elapsedMillis;
  if (stopwatchState.running) {
    currentElapsed += millis() - stopwatchState.startMillis;
  }
  return (uint32_t)currentElapsed;
}

void getStopwatchLaps(JsonDocument &doc) {
  JsonArray arr = doc.createNestedArray("laps");
  for (uint8_t i = 0; i < stopwatchState.lapCount; i++) {
    arr.add((uint32_t)stopwatchState.laps[i]);
  }
}

void updateControlManager() {
  DateTime now = getCurrentTime();
  uint8_t today = now.day();
  if (today != lastDay) {
    resetAlarmDailyFlags();
    lastDay = today;
  }

  // Alarm checking (minute-level)
  for (uint8_t i = 0; i < kMaxAlarms; i++) {
    if (!alarms[i].active || alarms[i].triggeredToday) continue;
    if (alarms[i].hour == now.hour() && alarms[i].minute == now.minute()) {
      alarms[i].triggeredToday = true;
      lastNotification = String("ALARM: ") + alarms[i].label;
      recordEvent("alarm", lastNotification);
      // Hook points for LED/audio can be added here
    }
  }

  // Timer completion
  if (timerState.running) {
    long remaining = (long)(timerState.endMillis - millis());
    if (remaining <= 0) {
      timerState.running = false;
      lastNotification = "TIMER DONE";
      recordEvent("timer", lastNotification);
    }
  }

  // Stopwatch accumulation is handled lazily in getters
}
