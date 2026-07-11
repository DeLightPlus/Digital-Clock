# Desktop Transport Plan (Core)
Date: 2026-02-06

## Goal
Provide a single desktop app that can fully control the ESP32 whether the PC is on WiFi/AP or only has Bluetooth.

## Principle
- One UI (same as web app)
- Two transports (WiFi/AP + BLE)
- Identical payloads and responses where possible

## Transport Modes

### 1) WiFi / AP Mode
- Target: http://192.168.4.1 (AP) or device STA IP
- Protocol: HTTP REST
- Responses: JSON

### 2) BLE Mode
- Target: SerialBT channel
- Protocol: JSON RPC over BLE serial
- Responses: JSON

## BLE JSON RPC Schema (Proposed)
Requests:
```
{"id":"req-1","method":"wifi.configure","params":{"ssid":"...","password":"...","ap_name":"iGO-Buddy"}}
{"id":"req-2","method":"timer.set","params":{"seconds":120}}
{"id":"req-3","method":"alarm.add","params":{"hour":7,"minute":30,"label":"Wake up"}}
```

Responses:
```
{"id":"req-1","ok":true,"result":{"message":"WiFi configured"}}
{"id":"req-2","ok":true}
{"id":"req-3","ok":true,"result":{"index":2}}
```

## Method Mapping (Source of Truth)
- See docs/reference/Protocol_Core.md

## Desktop App Behavior
- On launch:
  - Try WiFi/AP reachability
  - If not reachable, try BLE scan + connect
- Keep one active transport at a time
- Show transport status in UI (WiFi/AP/BLE)

## Implementation Notes
- BLE messages are line-delimited JSON
- Device replies with line-delimited JSON
- If BLE is active, disable web-only features (file uploads, OTA)

## Open Questions
- Should BLE allow OTA updates?
- Should BLE include file transfer for themes?
- What is the max payload size for BLE serial?
