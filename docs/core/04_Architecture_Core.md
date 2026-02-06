# Architecture Overview (Core)
System: iGO Core Smart Dock / Desktop Buddy
Version: 1.0
Date: 2026-02-06

## 1. High-Level Components
- Embedded device (ESP32 + peripherals)
- Desktop app (USB)
- Mobile app (BLE/WiFi)
- Web UI (served from device)

## 2. Embedded Firmware Modules
- WebManager: HTTP server, routes, WiFi provisioning
- ControlManager: alarms, timer, stopwatch, tasks, notes
- DisplayManager: TFT rendering
- LEDAnimator: LED effects
- AudioManager: DFPlayer control

## 3. Data Flow
1. User action on app
2. API request to device
3. ControlManager updates local storage
4. Device updates display and alerts
5. Optional sync to other clients on next connection

## 4. Storage
- Preferences (key-value) for settings and state
- Optional file storage for themes and assets

## 5. Connectivity
- USB: configuration and firmware updates
- WiFi: local web UI and REST API
- BLE: mobile sync for offline scenarios

## 6. Security
- Local network access only
- Optional basic authentication
- No cloud dependency for core

## 7. Scalability Path
- Add-on modules integrate via API extensions
- Sensor nodes can be added without replacing core device
