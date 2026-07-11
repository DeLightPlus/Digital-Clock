# Protocol (Core)
Date: 2026-02-06

## Goal
Define a minimal, consistent protocol for desktop/web/mobile clients to control the device.

## HTTP REST (WiFi/AP)
Base: http://<device-ip>

Core endpoints:
- GET /api/status
- GET /api/clock
- GET /api/settings
- POST /api/settings/time-format
- POST /api/settings/date-format
- POST /api/wifi/configure
- POST /api/wifi/reset
- POST /api/pair/complete
- GET /api/wifi/scan
- POST /api/alarm/add
- POST /api/alarm/delete
- GET /api/alarm/list
- POST /api/timer/set
- POST /api/timer/start
- POST /api/timer/stop
- GET /api/timer/status
- POST /api/stopwatch/start
- POST /api/stopwatch/stop
- POST /api/stopwatch/reset
- POST /api/stopwatch/lap
- GET /api/stopwatch/status

## BLE JSON (SerialBT)
Line-delimited JSON requests and responses.

Request example:
```
{"id":"req-1","method":"wifi.configure","params":{"ssid":"...","password":"...","ap_name":"iGO-Buddy"}}
```

Response example:
```
{"id":"req-1","ok":true,"result":{"message":"WiFi configured"}}
```

Method mapping mirrors REST:
- status.get -> /api/status
- clock.get -> /api/clock
- settings.get -> /api/settings
- settings.timeFormat -> /api/settings/time-format
- settings.dateFormat -> /api/settings/date-format
- wifi.configure -> /api/wifi/configure
- wifi.reset -> /api/wifi/reset
- pair.complete -> /api/pair/complete
- wifi.scan -> /api/wifi/scan
- alarm.add -> /api/alarm/add
- alarm.delete -> /api/alarm/delete
- alarm.list -> /api/alarm/list
- timer.set -> /api/timer/set
- timer.start -> /api/timer/start
- timer.stop -> /api/timer/stop
- timer.status -> /api/timer/status
- stopwatch.start -> /api/stopwatch/start
- stopwatch.stop -> /api/stopwatch/stop
- stopwatch.reset -> /api/stopwatch/reset
- stopwatch.lap -> /api/stopwatch/lap
- stopwatch.status -> /api/stopwatch/status

## Notes
- BLE is command-first; web UI is WiFi/AP only.
- Keep payloads aligned to avoid dual documentation.
