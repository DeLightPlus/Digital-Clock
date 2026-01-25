# WiFi Configuration Guide

## Overview
Your iGO-Buddy now supports **dual-mode WiFi**:
- **Scenario 1:** Connect ESP32 to your home WiFi + keep iGO-Buddy AP active
- **Scenario 2:** Use iGO-Buddy AP only (offline mode)

---

## How to Configure

### Step 1: Edit `include/wifi_config.h`

Open the file in VS Code and update your credentials:

```cpp
// Home WiFi Network Credentials
const char* HOME_WIFI_SSID = "YOUR_HOME_WIFI_NAME";      // ← Your WiFi network name
const char* HOME_WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";   // ← Your WiFi password

// iGO-Buddy AP Settings (always active)
const char* IGO_AP_SSID = "iGO-Buddy";                   // ← Keep this or customize
const char* IGO_AP_PASSWORD = "";                        // ← Empty = open (no password)
```

**Example:**
```cpp
const char* HOME_WIFI_SSID = "Guest WiFi";
const char* HOME_WIFI_PASSWORD = "SecurePassword123";
```

### Step 2: Build and Upload

```powershell
# Build
C:\Users\KPMatlakala\.platformio\penv\Scripts\platformio.exe run -e esp32dev

# Upload
C:\Users\KPMatlakala\.platformio\penv\Scripts\platformio.exe run -e esp32dev -t upload
```

### Step 3: Check Serial Monitor

```powershell
C:\Users\KPMatlakala\.platformio\penv\Scripts\platformio.exe device monitor -e esp32dev
```

Expected output:
```
=== Initializing iGO-Buddy WiFi System ===

Step 1: Connecting to home network...
SSID: Guest WiFi
.........
✓ Connected to home WiFi!
  IP Address: 192.168.1.45

Step 2: Starting iGO-Buddy AP...
✓ iGO-Buddy AP Active
  SSID: iGO-Buddy
  Password: (none - open network)
  IP Address: 192.168.4.1

Step 3: Starting HTTP server...
✓ HTTP server running on port 80

=== System Ready ===
Modes: Clock Display + Web Control
Local AP: http://192.168.4.1 (or iGO-Buddy SSID)
Home WiFi: Configured in wifi_config.h
```

---

## Usage Scenarios

### Scenario A: Home Network Only (Recommended)
All devices connected to home WiFi:

```
Home WiFi Router (192.168.1.0/24)
├── Phone (192.168.1.50)
├── PC (192.168.1.100)
└── iGO-Buddy (192.168.1.45) ← Also broadcasts iGO-Buddy AP
```

**Access dashboard:**
- From phone/PC: `http://192.168.4.1` (via iGO-Buddy AP)
- Or: `http://192.168.1.45` (via home WiFi, if you add mDNS)

**Advantages:**
- ✅ All devices have internet
- ✅ iGO-Buddy can upload data to cloud
- ✅ Can control from anywhere in home network range

---

### Scenario B: AP Only (Offline/Temporary)
Leave `HOME_WIFI_SSID = ""` empty:

```cpp
const char* HOME_WIFI_SSID = "";  // Empty = AP-only mode
```

**Result:**
- Only iGO-Buddy AP visible (192.168.4.1)
- Works without home WiFi
- Useful for demo, camping, remote location

---

### Scenario C: Both Networks Active (Current)
Configured in both `home` and `iGO` networks:

```
┌─────────────────────────────────┐
│   Home WiFi Router              │
│  (192.168.1.0/24)              │
├─────────────────────────────────┤
│ Phone: 192.168.1.50            │
│ PC: 192.168.1.100              │
│ iGO: 192.168.1.45 (STA mode)  │
└─────────────────────────────────┘

  Also broadcasts:
  ┌─────────────────────────────────┐
  │   iGO-Buddy AP                  │
  │   (192.168.4.0/24)              │
  ├─────────────────────────────────┤
  │ iGO: 192.168.4.1 (AP mode)     │
  │ Can connect phone/PC here too   │
  └─────────────────────────────────┘
```

**Benefits:**
- Phone/PC on home WiFi = internet access
- Phone/PC can ALSO see iGO-Buddy AP for direct control
- iGO acts as bridge between networks (future router capability)

---

## API Endpoints

Check WiFi status via REST API:

```bash
# Get status (returns JSON with WiFi info)
curl http://192.168.4.1/api/status

# Response example:
{
  "status": "ok",
  "uptime_ms": 125430,
  "ap_active": true,
  "home_wifi_connected": true,
  "ap_ip": "192.168.4.1",
  "home_wifi_ip": "192.168.1.45"
}
```

---

## Troubleshooting

### WiFi doesn't connect
- **Check:** Credentials in `wifi_config.h` are correct (case-sensitive)
- **Check:** WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- **Check:** Password doesn't have special characters that confuse parser
- **Fix:** Leave `HOME_WIFI_SSID = ""` to use AP-only mode

### iGO-Buddy AP doesn't appear
- **Check:** Serial monitor shows "✓ iGO-Buddy AP Active"
- **Check:** ESP32 is powered on and running
- **Check:** Your phone/PC WiFi is scanning for networks
- **Fix:** Restart ESP32 (unplug/replug)

### Can't reach `192.168.4.1`
- **Check:** Connected to iGO-Buddy AP (not home WiFi)
- **Check:** Try `http://192.168.4.1:80` explicitly
- **Fix:** Restart browser, clear cache

### Device IP address keeps changing
- **Normal:** DHCP assigns new IPs, but AP IP stays at `192.168.4.1`
- **Note:** iGO always responds on `192.168.4.1` from its AP

---

## Configuration File Reference

```cpp
// include/wifi_config.h

// Home WiFi Network Credentials
const char* HOME_WIFI_SSID = "";          // WiFi SSID (empty = AP-only mode)
const char* HOME_WIFI_PASSWORD = "";      // WiFi password

// iGO-Buddy AP Settings (always active)
const char* IGO_AP_SSID = "iGO-Buddy";    // AP name visible to devices
const char* IGO_AP_PASSWORD = "";         // AP password (empty = open)

// WiFi Connection Timeout (milliseconds)
#define WIFI_CONNECT_TIMEOUT_MS 10000     // Max 10 seconds to connect

// Debug mode
#define WIFI_DEBUG 1                       // 1 = verbose serial output
```

---

## Next Steps

1. **Update credentials** in `wifi_config.h`
2. **Build and upload** firmware
3. **Check serial monitor** for connection status
4. **Test from phone/PC:**
   - Connect to iGO-Buddy AP or home WiFi
   - Open `http://192.168.4.1` in browser
   - Click "Send Notification" button
   - Watch TFT display update

5. **Future enhancements:**
   - Add mDNS (access as `http://igo-buddy.local`)
   - Store credentials in SPIFFS (editable via web UI)
   - Add WiFi provisioning app
   - Implement WiFi bridge/router mode for true internet sharing
