# Connectivity Notes (Core)
Date: 2026-02-06

## Current Provisioning Behavior

### WiFi AP Portal
- The local setup portal is available only over WiFi AP.
- BLE cannot serve the web portal.
- Access: connect to the device AP, then open /index.html#wifi.

### BLE Provisioning (Serial Command)
- BLE is a command channel, not a web UI.
- Provisioning via BLE uses a JSON command:

```
WIFI JSON {"ssid":"YourSSID","password":"YourPass","ap_name":"iGO-Buddy"}
```

- On success, the device saves credentials and reboots into STA.

### Desktop Usage
- A desktop app or BLE terminal can send the BLE JSON command.
- USB serial can be used for similar command-based provisioning.

## Desktop App Goal (Summary)
- One desktop UI that mirrors the web app.
- Two transports (WiFi/AP + BLE) with auto-select.
- Full control and provisioning even when PC WiFi is unavailable.

Details:
- docs/core/08_Desktop_Transport.md
- docs/reference/Protocol_Core.md

## UI-less vs Desktop vs Web vs Mobile (Quick Matrix)

UI-less server (headless + minimal admin):
- Best for: B2B deployments, automation, set-and-forget
- Pros: fastest to ship, lowest UI maintenance
- Cons: weak onboarding, requires another client

Desktop app:
- Best for: full control, provisioning, firmware updates
- Pros: BLE fallback when WiFi is busy, power-user workflows
- Cons: install/update overhead

Web app (on-device/LAN):
- Best for: quick setup and demos
- Pros: zero install, simplest onboarding
- Cons: needs WiFi/AP access

Mobile app:
- Best for: daily usage and notifications
- Pros: always available, BLE works anywhere
- Cons: highest build/maintenance cost

Recommended rollout:
1) Headless core + minimal admin
2) Web UI for setup/demos
3) Desktop app for power users
4) Mobile app for daily use

## Open Questions
- Should we add a desktop UI for WiFi setup (fields + apply button)?
- Should BLE use a GATT config screen in the mobile app?
- Should AP stay available after STA connects (for local management)?
