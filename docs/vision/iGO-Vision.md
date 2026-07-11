# iGO Platform Vision

This document summarizes the universal core of iGO-Buddy and the domain add-ons (Health, Drive, Home) based on the discussion in notes.

## Universal Core (for everyone)
- Time and alarms: RTC-backed time, recurring alarms, timers, stopwatch, Pomodoro.
- Notifications and reminders: meetings, deadlines, important notes with visual/audio tiers.
- Tasks and notes: simple to-dos, quick capture, categories and priorities.
- Calendar sync: Google/Outlook import with offline local storage and agenda display.
- Power and load-shedding management: battery status, outage schedule pre-alerts, seamless failover.
- Display customization: clock faces, themes, layouts, brightness, screen rotation.
- Audio system: voice alerts, sound effects, multilingual packs, DND.
- Multi-device connectivity: USB (desktop), WiFi (mobile/web), BLE (offline mobile).

## Add-ons (field-specific feature packs)

### iGO-Health
- Medication schedules, pill photos, interaction warnings.
- Vital signs via BLE devices (BP, SpO2, glucose).
- Caregiver alerts and clinic modes (fridge monitoring, checklists).

### iGO-Drive
- OBD-II diagnostics, GPS trip logging, fuel economy coach.
- Maintenance reminders, crash detection + SOS, parking assist.

### iGO-Home
- Automation via relays, occupancy lighting, energy monitoring.
- Security (arm/disarm, alerts), leak detection, HVAC scheduling.

## Connectivity Matrix
- USB → desktop app (full control, firmware updates).
- WiFi → mobile/web apps (wireless convenience).
- BLE → mobile app offline (low power tether).
- Charging dock → mobile physical connection (data + power).

## Data Model (high level)
- Core: alarms, calendar, tasks, notes, notifications.
- Add-ons: health/drive/home sections enabled per feature pack.

## Next steps
- Build iGO-Buddy MVP with modular firmware and desktop app.
- Layer add-ons via firmware feature flags and accessory kits.
