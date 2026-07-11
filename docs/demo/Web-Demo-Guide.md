# iGO-Buddy Web Demo Guide

Quick reference for running the web-based demo.

## Hardware Setup

1. **ESP32 connections:**
   - TFT ST7789 (240×240): SCLK→GPIO18, MOSI→GPIO23, DC→GPIO2, RST→GPIO4, CS→GND
   - DFPlayer Mini: TX→GPIO16, RX→GPIO17, 5V, GND; microSD with `0001.mp3`, `0002.mp3`
   - Power via USB or 5V supply

2. **Build and upload:**
   ```powershell
   pio run -e esp32buddy
   pio run -e esp32buddy -t upload
   pio device monitor -b 115200 -e esp32buddy
   ```

3. **Find the AP IP** (default: `192.168.4.1`):
   - Serial monitor shows: `AP IP: 192.168.4.1`

## Using the Web Interface

### Desktop Demo
1. Connect to WiFi network: **iGO-Buddy** (no password)
2. Open browser: `http://192.168.4.1`
3. Dashboard shows battery, next event, countdown
4. Send notification → TFT updates + DFPlayer plays track 1
5. Test alarm → DFPlayer plays track 2

### Mobile Demo
1. Same steps on phone browser
2. Responsive layout adapts to screen size
3. Touch-friendly buttons

## Demo Flow (5 min)
1. Show ESP32 hardware with TFT displaying dashboard
2. Connect laptop to iGO-Buddy WiFi → open web UI
3. Live status updates every 5 seconds
4. Send "Meeting Alert" notification → observe TFT + audio
5. Switch to phone → same interface, fully functional
6. Mention: works during load-shedding (battery backup)
7. Upgrade path: add Health/Drive/Home feature packs

## Customization

Edit `src/esp32_buddy/main.cpp`:
- Change WiFi AP name: `const char* ssid = "iGO-Buddy";`
- Add password: `const char* password = "yourpass";`
- Adjust TFT pins if wiring differs
- Change DFPlayer volume: `dfplayer.volume(20);` (0-30)

## Troubleshooting

- **Can't connect to AP**: Check ESP32 serial output for IP; ensure WiFi is enabled on device
- **No audio**: Verify DFPlayer wiring, SD card inserted, files named `0001.mp3`, `0002.mp3`
- **TFT blank**: Check SPI connections; try different CS/DC/RST pins
- **Web page doesn't load**: Confirm browser connected to iGO-Buddy network; try `192.168.4.1` directly
