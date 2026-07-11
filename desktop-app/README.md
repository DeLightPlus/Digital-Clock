# iGO-Desktop (Electron + React)

## Purpose
iGO-Desktop is the desktop companion for iGO Core. It mirrors the web UI and supports both WiFi/AP and BLE transports.

## Setup
```bash
cd desktop-app
npm install
```

## Development
```bash
npm run dev
```

## Build (renderer only)
```bash
npm run build
```

## Current State
- Electron + React scaffold only
- No transport logic wired yet
- UI mirrors the intended layout direction

## Planned Transports
- WiFi/AP: REST calls to device IP (AP or STA)
- BLE: JSON RPC over SerialBT
- Auto-select based on availability

## Next Steps
- Implement transport auto-detect (WiFi first, BLE fallback)
- Add shared API client with REST + BLE adapters
- Mirror the web UI panels and routing
