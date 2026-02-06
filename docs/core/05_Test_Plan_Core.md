# Test Plan (Core)
System: iGO Core Smart Dock / Desktop Buddy
Version: 1.0
Date: 2026-02-06

## 1. Test Objectives
- Validate core reliability during power loss
- Verify alarm/timer/stopwatch accuracy
- Confirm sync across USB/WiFi/BLE
- Ensure web UI is functional and responsive

## 2. Test Types
- Unit tests (logic and data)
- Integration tests (API + storage)
- System tests (end-to-end flows)
- Reliability tests (power loss, long runtime)

## 3. Test Environments
- ESP32 device with RTC and battery
- Desktop client (Windows)
- Mobile client (Android)
- Local WiFi network

## 4. Key Test Cases
- Alarm fires during 8-hour power outage
- Timer countdown accuracy within 1 second
- Stopwatch start/stop/reset and lap tracking
- WiFi provisioning, reconnect, and credential clearing
- API returns expected JSON for status and clock
- Task sync between desktop and device
- Device boot time <= 6 seconds

## 5. Performance Tests
- Web UI load time on LAN <= 2 seconds
- BLE sync of 50 tasks <= 5 seconds

## 6. Reliability Tests
- 72-hour continuous run without crash
- 20 power cycles without corruption
- OTA update success rate >= 95%

## 7. Acceptance Criteria
- All high-priority tests pass
- No data loss after power cycling
- Alarm accuracy within 1 second
