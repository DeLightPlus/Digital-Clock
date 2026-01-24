# iGO PC ↔ Device Protocol

## Web API (HTTP REST)

iGO-Buddy hosts a WiFi access point and web server. Connect to the AP and access `http://192.168.4.1` (default AP IP).

### Endpoints

- `GET /` — Web dashboard (HTML interface)
- `GET /api/status` — Device status JSON:
  ```json
  {
    "battery": 75,
    "charging": false,
    "nextEvent": "Team Meeting",
    "countdown": 15
  }
  ```
- `GET /api/notify?title=X&body=Y` — Send notification, plays audio alert
- `GET /api/alarm` — Trigger test alarm sound

## Serial (115200 baud, legacy)

Commands from PC to iGO:
- CONFIG:key=value — update a single setting.
- NOTIFY:app|title|body — display a notification with source app.
- ALARM:add|time=HH:MM|label=Text — add an alarm.
- ALARM:list — request current alarms.
- THEME:apply|id=theme_id — switch theme.
- FILE:START:name.ext|size=N — begin file transfer.
- FILE:CHUNK:<bytes> — subsequent chunks (512B default) with checksum header.
- FILE:END|md5=... — finalize transfer.
- QUERY:battery — request battery status.
- TIME:sync:epoch — set RTC time.

Responses from iGO:
- ACK — command received.
- ERROR:reason — invalid or failed command.
- STATUS:battery=78%,charging=1 — periodic device status.
- EVENT:alarm_triggered:07:00 — emitted on events.
- DATA:sensor:temp=24.5,pressure=1013.2 — sensor snapshot.

Notes:
- Binary file chunks include a short header with seq number and CRC32.
- Large transfers pause UI updates to ensure stable writes.
