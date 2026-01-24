# 3-Day Demo Sprint - Quick Reference

**Goal**: Working iGO-Buddy demo for Monday presentation  
**Start**: Friday (Today) | **Demo**: Monday Morning

---

## 📅 Day-by-Day Breakdown

### 🔷 Friday (Day 1) - Foundation
**Focus**: Get WiFi and web interface running

#### Morning (3-4 hours)
- [ ] **WiFi AP Setup**
  - Add WiFi.h and WebServer.h includes to main.cpp
  - Create WiFi AP with SSID "iGO-Buddy"
  - Verify connection from phone/laptop
  - Print IP address (should be 192.168.4.1)

- [ ] **Basic Web Server**
  - Implement handleRoot() for homepage
  - Implement handleStatus() for /api/status JSON
  - Test in browser: http://192.168.4.1

#### Afternoon (3-4 hours)
- [ ] **HTML Dashboard**
  - Create simple responsive dashboard
  - Display device status (battery, time, next event)
  - Add "Send Notification" test button
  - Style with basic CSS (mobile-friendly)

- [ ] **AJAX Status Updates**
  - JavaScript to poll /api/status every 2 seconds
  - Update dashboard elements dynamically
  - Test on both phone and laptop browsers

**Evening Goal**: Can access dashboard on phone, see live clock updates

---

### 🔷 Saturday (Day 2) - Features
**Focus**: Notifications, audio, and interactivity

#### Morning (3-4 hours)
- [ ] **Notification System**
  - Create NotificationManager module
  - Implement showNotification() function
  - Display notification on TFT (overlay on clock)
  - Add /api/notify endpoint to trigger from web

- [ ] **Calendar/Event System**
  - Create simple event structure (title, time, countdown)
  - Hard-code 2-3 sample events
  - Display next event on TFT
  - Show countdown timer

#### Afternoon (3-4 hours)
- [ ] **Audio Integration** (if DFPlayer available)
  - Wire DFPlayer Mini
  - Test basic playback
  - Create 2-3 alert sounds (meeting.mp3, notification.mp3)
  - Play sound when notification triggered

- [ ] **Battery Monitoring** (if hardware available)
  - Add voltage divider circuit
  - Read battery percentage
  - Display on TFT and web dashboard
  - Add low battery warning

**Evening Goal**: Send notification from web → shows on TFT + plays sound

---

### 🔷 Sunday (Day 3) - Polish
**Focus**: Professional appearance and demo prep

#### Morning (2-3 hours)
- [ ] **UI Polish**
  - Refine TFT layout (clean, professional)
  - Improve web dashboard styling
  - Add loading indicators
  - Handle errors gracefully

- [ ] **Demo Content**
  - Create realistic sample events
    - "Team Standup" in 15 minutes
    - "Client Call" in 1 hour
    - "Lunch Break" in 3 hours
  - Prepare notification examples
  - Test notification priorities (info/warning/critical)

#### Afternoon (2-3 hours)
- [ ] **Testing**
  - Full system test (all features)
  - Test on multiple devices
  - Simulate load-shedding (unplug power)
  - Fix any bugs discovered

- [ ] **Presentation Prep**
  - Write demo script (5-7 minutes)
  - Create PowerPoint slides (5-7 slides)
  - Practice presentation twice
  - Record backup demo video

**Evening Goal**: Confident in demo, all features working reliably

---

## 🎯 Minimum Viable Demo (If Time Runs Short)

### Absolute Must-Haves
1. ✅ Clock display working (already done!)
2. ⏳ WiFi AP accessible
3. ⏳ Web dashboard loads
4. ⏳ Can send notification from web → appears on TFT

### Nice-to-Haves
5. ⏳ Audio alert plays
6. ⏳ Battery percentage shown
7. ⏳ Event countdown visible
8. ⏳ Professional styling

### Stretch Goals
9. ⏳ Multiple themes
10. ⏳ Load-shedding simulation
11. ⏳ Voice alerts

**If behind schedule**: Focus on items 1-4, skip 5-11

---

## 📝 Code Snippets (Copy-Paste Ready)

### WiFi AP Setup
```cpp
// Add to top of main.cpp
#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "iGO-Buddy";
const char* ap_password = "igo12345";
WebServer server(80);

// Add to setup()
void setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

// Call in setup():
setupWiFi();
```

### Web Endpoints
```cpp
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>iGO-Buddy</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; max-width: 600px; margin: 50px auto; padding: 20px; }
    .status { background: #f0f0f0; padding: 15px; margin: 10px 0; border-radius: 8px; }
    button { background: #4CAF50; color: white; padding: 12px 24px; border: none; 
             border-radius: 4px; font-size: 16px; cursor: pointer; }
    button:hover { background: #45a049; }
  </style>
</head>
<body>
  <h1>🎯 iGO-Buddy Dashboard</h1>
  <div class="status">
    <h3>Device Status</h3>
    <p id="time">Loading...</p>
    <p id="battery">Battery: --</p>
    <p id="event">Next Event: --</p>
  </div>
  <button onclick="sendNotification()">📢 Send Test Notification</button>
  
  <script>
    function updateStatus() {
      fetch('/api/status')
        .then(r => r.json())
        .then(data => {
          document.getElementById('battery').innerText = 'Battery: ' + data.battery + '%';
          document.getElementById('event').innerText = 'Next: ' + data.nextEvent + ' in ' + data.countdown + ' min';
        });
    }
    
    function sendNotification() {
      fetch('/api/notify?title=Test&body=Hello from web!')
        .then(() => alert('Notification sent!'));
    }
    
    setInterval(updateStatus, 2000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

void handleStatus() {
  DateTime now = getCurrentTime();
  
  String json = "{";
  json += "\"battery\": 85,";
  json += "\"charging\": false,";
  json += "\"nextEvent\": \"Team Meeting\",";
  json += "\"countdown\": 15";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleNotify() {
  String title = server.arg("title");
  String body = server.arg("body");
  
  // Display on TFT
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.println(title);
  tft.setCursor(10, 100);
  tft.println(body);
  
  // TODO: Play audio alert
  
  server.send(200, "text/plain", "OK");
}

// In setup():
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/api/status", handleStatus);
  server.on("/api/notify", handleNotify);
  server.begin();
  Serial.println("Web server started");
}

// In loop():
server.handleClient();
```

---

## 🧪 Testing Checklist

### Hardware Tests
- [ ] TFT displays correctly
- [ ] RTC time accurate
- [ ] LEDs animate smoothly
- [ ] DFPlayer plays audio (if connected)
- [ ] Battery reading accurate (if connected)

### Software Tests
- [ ] WiFi AP broadcasts
- [ ] Can connect from phone
- [ ] Can connect from laptop
- [ ] Web page loads on both
- [ ] Status updates every 2 seconds
- [ ] Notification appears on TFT
- [ ] Audio plays (if DFPlayer working)

### Demo Rehearsal
- [ ] 5-minute presentation timed
- [ ] All features demonstrated
- [ ] No crashes during run-through
- [ ] Backup demo video recorded
- [ ] Slides ready and polished

---

## 🎤 Presentation Structure (5-7 minutes)

### Slide 1: Title (30 seconds)
- "iGO Platform: Universal Desk Companion"
- Your name and project context
- Quick hook: "Solves load-shedding + notifications + health reminders"

### Slide 2: Problem (1 minute)
- Show pain points:
  - Missed meetings during load-shedding
  - No offline desk notifications
  - Medication reminders need internet
  - No modular solution exists

### Slide 3: Solution - iGO Core (1 minute)
- Universal features everyone needs
- Offline-capable, battery backup
- Multi-device connectivity
- Show architecture diagram

### Slide 4: Live Demo (2-3 minutes)
- Connect phone to iGO WiFi
- Open dashboard
- Send notification → appears on device + sound
- Show calendar countdown
- Simulate load-shedding (unplug power)

### Slide 5: Upgrade Path (1 minute)
- Show add-on modules (Health, Drive, Home)
- Quick use cases for each
- Mention Limpopo pilot

### Slide 6: Roadmap & Ask (1 minute)
- Phase 1: MVP done (this demo)
- Phase 2: Desktop/mobile apps
- Phase 3: Pilot deployment
- Ask: Feedback, partners, pilot sites

### Slide 7: Thank You + Q&A
- Contact info
- GitHub link (if public)
- Open for questions

---

## ⚠️ Common Pitfalls to Avoid

### Technical
- ❌ Don't forget `server.handleClient()` in loop()
- ❌ Don't block loop() with delays (use millis())
- ❌ Don't forget CORS headers if testing from different origin
- ❌ Don't hardcode IPs (use `WiFi.softAPIP()`)

### Demo
- ❌ Don't assume WiFi will work perfectly
- ❌ Don't skip testing on actual demo devices
- ❌ Don't forget to charge batteries
- ❌ Don't go over 7 minutes (practice timing!)

### Presentation
- ❌ Don't read slides word-for-word
- ❌ Don't dive too deep into technical details
- ❌ Don't ignore backup plan if demo fails
- ❌ Don't forget to smile and make eye contact

---

## 🎁 Bonus Features (If Ahead of Schedule)

### Friday Bonus
- QR code on TFT for easy WiFi connection
- Multiple color themes (toggle via web)

### Saturday Bonus
- Voice recording via web (family custom messages)
- Multiple languages for alerts

### Sunday Bonus
- Load-shedding schedule integration (real API)
- Weather display (from online API)
- GitHub stats or crypto ticker

---

## 📞 Emergency Contacts

### If Stuck on Code
- ESP32 Arduino Forums
- PlatformIO Community Discord
- Stack Overflow (esp32 tag)

### Hardware Issues
- Double-check wiring diagram
- Test components individually
- Use spare ESP32 if available

### Time Pressure
- **Priority**: Web interface working
- **Fallback**: Visual demo only (no audio)
- **Nuclear option**: Pre-recorded video + hardware display

---

## ✅ Friday Evening Checkpoint

By end of Friday, you should have:
- [ ] WiFi AP broadcasting "iGO-Buddy"
- [ ] Web dashboard accessible at 192.168.4.1
- [ ] Live status updates working
- [ ] Basic notification trigger from web

**If not all done**: Don't panic, Saturday can catch up. Focus on WiFi + web first.

---

## ✅ Saturday Evening Checkpoint

By end of Saturday, you should have:
- [ ] Notifications displaying on TFT
- [ ] Audio alert playing (or skip if DFPlayer issues)
- [ ] Event countdown working
- [ ] Battery display (or mock data if no hardware)

**If not all done**: Sunday is for polish, not features. Lock in what works.

---

## ✅ Sunday Evening Checkpoint

By end of Sunday, you should have:
- [ ] Demo script written and practiced
- [ ] Slides complete (7 slides max)
- [ ] Backup demo video recorded
- [ ] Hardware packed and ready
- [ ] Confident in presentation

**Sleep well Sunday night** - you've got this! 🚀

---

**Remember**: Perfect is the enemy of done. A working 80% demo beats a broken 100% demo every time.

**Good luck!** 🎯
