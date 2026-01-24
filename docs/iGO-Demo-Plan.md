# iGO-Buddy 3-Day Demo Plan (Mon Presentation)

Goal: Show iGO-Buddy core with desktop app control, notifications, alarms, and battery/load-shedding behavior; highlight upgrade paths to Health/Drive/Home.

## Day 1 (Today)
- Scope demo features and wireframes.
- Create web-based interface hosted on ESP32 (responsive HTML/CSS/JS).
- Define REST API endpoints for status, notifications, alarms.
- Test WiFi AP mode and browser access.

## Day 2 (Tomorrow)
- Implement TFT dashboard screens (clock, status, next event).
- Add basic audio alerts via DFPlayer (meeting soon, battery low).
- Integrate calendar stub (upload sample events.json to SD or SPIFFS).
- Test battery failover behavior (simulate power loss).

## Day 3 (Sunday)
- Polish UI themes and add one custom clock face.
- Prepare demo content: sample notifications, alarms, Pomodoro timer.
- Write presentation outline and record a short demo video clip.
- Smoke test on desk + capture screenshots.

## Monday Presentation Flow (5–7 minutes)
1. Introduce iGO-Buddy and show hardware (TFT, speaker, ports).
2. Connect laptop/phone to iGO WiFi AP → open browser to 192.168.4.1.
3. Show web dashboard: live status updates (battery, next event, countdown).
4. Send notification from web UI → appears on TFT + audio alert.
5. Test alarm → voice + visual alert demo.
6. Switch to phone browser → same interface works seamlessly.
7. Simulate load-shedding → iGO on battery, web still accessible via AP.
8. Show upgrade packs briefly: Health (med reminders), Drive (vehicle), Home (automation).
9. Wrap with roadmap and potential partners.

## Checklist
- ESP32 creates WiFi AP with web server.
- Web dashboard loads on desktop and mobile browsers.
- REST API returns device status (battery, event, countdown).
- Notification endpoint triggers TFT update and DFPlayer alert.
- Audio works for at least two alerts.
- Calendar countdown updates on TFT.
- Battery indicator updates dynamically.
- Slides and demo script ready.
