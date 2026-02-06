# Software Requirements Specification (SRS)
System: iGO Core Smart Dock / Desktop Buddy
Version: 1.0
Date: 2026-02-06

## 1. System Overview
The system consists of embedded firmware on the device and companion clients (desktop, mobile, web). The device provides offline-first functionality and syncs when connected.

## 2. Functional Requirements

### 2.1 Device Core
- FR-1: The device shall maintain time using an RTC during power loss.
- FR-2: The device shall support multiple alarms (one-time and recurring).
- FR-3: The device shall support a timer with start/stop/reset.
- FR-4: The device shall support a stopwatch with lap support.
- FR-5: The device shall store tasks and notes locally.
- FR-6: The device shall display battery state and power mode.

### 2.2 Alerts and Notifications
- FR-7: The device shall provide visual alerts for alarms and timers.
- FR-8: The device shall provide audio alerts when audio module is present.
- FR-9: The device shall support quiet hours configuration.

### 2.3 Connectivity
- FR-10: The device shall provide a local web UI over WiFi.
- FR-11: The device shall accept BLE connections for mobile sync.
- FR-12: The device shall support USB for desktop configuration.

### 2.4 Data Sync
- FR-13: The device shall store data locally and sync changes when a client connects.
- FR-14: The device shall support conflict resolution with last-write-wins.
- FR-15: The device shall expose an API for alarms, timer, stopwatch, and settings.

### 2.5 Firmware Update
- FR-16: The device shall support firmware updates via USB and web.
- FR-17: The device shall validate update packages before applying.

## 3. API Requirements (Local HTTP)
Endpoints (base /api):
- GET /clock
- GET/POST /settings
- GET/POST /alarm/add
- POST /alarm/delete
- GET /alarm/list
- GET/POST /timer/set
- POST /timer/start
- POST /timer/stop
- GET /timer/status
- POST /stopwatch/start
- POST /stopwatch/stop
- POST /stopwatch/reset
- POST /stopwatch/lap
- GET /stopwatch/status
- GET /wifi/scan
- POST /wifi/connect
- POST /wifi/clear
- GET /status

## 4. Data Model (Core)
- Alarm: id, time, days, enabled, label
- Timer: set_seconds, remaining_seconds, running
- Stopwatch: running, elapsed_ms, laps[]
- Task: id, title, due_at, completed
- Note: id, text, created_at
- Settings: timezone, brightness, volume, quiet_hours

## 5. Non-Functional Requirements
- NFR-1: Alarm accuracy within 1 second.
- NFR-2: Device uptime >= 99% during pilot.
- NFR-3: Web UI must load in <= 2 seconds on LAN.
- NFR-4: Data retention at least 30 days offline.

## 6. Constraints
- ESP32 memory and storage limits
- Power constraints during battery mode

## 7. Compliance and Privacy
- Local-only storage by default
- Optional basic auth for web UI
- No cloud dependency for core functions

## 8. Assumptions
- RTC is installed and calibrated
- Battery backup is present
