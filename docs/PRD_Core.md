# Product Requirements Document (PRD)
Product: iGO Core Smart Dock / Desktop Buddy
Version: 1.0
Date: 2026-02-06
Owner: Kabelo Matlakala

## 1. Product Summary
iGO Core is a compact, always-on desktop device that provides time, alarms, reminders, tasks, and notifications. It remains reliable during power outages and syncs with desktop/mobile apps when connected.

## 2. Goals
- Deliver a reliable, offline-capable daily assistant
- Provide a clean, glanceable UI with voice and visual alerts
- Support USB, WiFi, and BLE connectivity
- Enable customization and future add-ons

## 3. Non-Goals
- Full smart home control
- Advanced health diagnostics
- City-scale infrastructure integration

## 4. Personas
- Student: needs alarms, timers, task list, Pomodoro
- Professional: needs calendar sync, meeting reminders
- Household user: needs power-outage aware reminders

## 5. User Journeys
- Set an alarm on mobile, device rings even if internet is down.
- Power goes out, device switches to battery and continues alerts.
- User adds a task on desktop; device shows pending task count.

## 6. MVP Features
Device:
- Clock display with date
- Multiple alarms (recurring and one-time)
- Timer and stopwatch
- Task list (basic: add, complete, delete)
- Notes (short text)
- Load-shedding alerts (manual schedule or API sync)
- Battery status and power mode

Apps:
- Desktop app (USB): setup, alarm/task management, firmware update
- Mobile app (BLE/WiFi): view status, manage alarms/tasks
- Web app (WiFi): quick dashboard, settings

Connectivity:
- USB for configuration and updates
- WiFi for local web dashboard
- BLE for offline mobile access

## 7. vNext Features
- Calendar read-only sync (Google/Outlook) to device
- Voice alerts library management
- Theme customization via app
- Basic notification rules (priority, quiet hours)

## 8. User Experience
- Glanceable home screen: time, next alarm, battery, alerts count
- Simple navigation via buttons or web UI
- Low cognitive load, minimal steps for core tasks

## 9. Hardware Requirements
- ESP32-class MCU with WiFi/BLE
- RTC module (DS3231 or equivalent)
- 240x240 TFT display
- Optional audio module (DFPlayer) and speaker
- Battery and charging module

## 10. Performance Requirements
- Device boot to home screen: <= 6 seconds
- Alarm trigger latency: <= 1 second
- Web UI load on LAN: <= 2 seconds
- BLE sync of tasks: <= 5 seconds

## 11. Security and Privacy
- Local-first data storage
- Optional authentication for web UI
- No mandatory cloud dependency

## 12. Acceptance Criteria
- Alarms fire correctly during 8-hour power outage
- Tasks and notes sync across at least two clients
- Device remains usable without WiFi or BLE
- Firmware update completes without bricking
