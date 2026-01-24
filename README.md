# iGO-ESP32: Modular Desk Companion Platform

**Version:** 1.0 MVP  
**Author:** Kabelo Matlakala  
**Platform:** ESP32 with TFT Display, RTC, and NeoPixel LEDs

---

## 🎯 Project Vision

**iGO** (inspectiGO) is a modular platform that starts as a smart desk companion and scales to specialized applications in healthcare, automotive, and home automation. The core system provides universal features everyone needs—time management, notifications, task tracking, and load-shedding resilience—while domain-specific add-ons extend functionality for specific use cases.

### Universal Core (Everyone)
- ⏰ **Time & Alarms**: RTC-backed precision timing, recurring alarms, Pomodoro timer
- 📅 **Calendar Sync**: Google/Outlook integration with offline local storage
- ✅ **Task Management**: Simple to-dos with priorities and due dates
- 🔔 **Notifications**: Meeting reminders, important notes with visual + audio alerts
- 🔋 **Load-Shedding Management**: Battery backup, Eskom schedule integration, pre-outage alerts
- 🎨 **Display Customization**: Multiple clock faces, themes, and layouts
- 🔊 **Audio System**: Voice alerts (DFPlayer), multilingual support, Do Not Disturb mode
- 📱 **Multi-Device Connectivity**: USB (desktop), WiFi (mobile/web), BLE (offline mode)

### Domain Add-Ons (Optional)
- 🏥 **iGO-Health**: Medication reminders, vital signs tracking, clinic monitoring
- 🚗 **iGO-Drive**: OBD-II diagnostics, GPS tracking, crash detection
- 🏡 **iGO-Home**: Smart home automation, energy monitoring, security

---

## 🔧 Current Implementation (MVP)

This MVP demonstrates the **Universal Core** foundation with a beautiful clock display and modular architecture ready for expansion.

### Hardware Components
- **ESP32 DevKit**: Main microcontroller (WiFi + BLE capable)
- **ST7789 TFT Display**: 240x240 color screen for rich UI
- **DS3231 RTC Module**: Precision timekeeping + temperature sensor
- **WS2812B NeoPixel Strip**: 5 LEDs for ambient lighting (breathing animation)
- **DFPlayer Mini** (planned): MP3 audio playback for voice alerts
- **BMP280 Sensor** (planned): Additional environmental monitoring
- **Power Bank Module** (planned): Battery backup for load-shedding

### Current Features ✅
- Beautiful analog-style clock face with digital time display
- Real-time date and temperature display from DS3231
- Blinking colon animation (heartbeat indicator)
- Smooth cyan breathing LED animation
- Modular code architecture (DisplayManager, ClockManager, RTC, LEDs)
- Serial debugging and status output

### In Development 🚧
- WiFi Access Point mode with web dashboard
- REST API for remote control and notifications
- Desktop/mobile app connectivity
- Calendar event display and countdown
- Audio alerts via DFPlayer Mini
- Battery management and load-shedding automation
- Task and notification system

---

## 📁 Project Structure

```
iGO-ESP32/
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── DisplayManager.cpp       # TFT display control
│   ├── ClockManager.cpp         # Clock logic and timing
│   ├── rtc_time.cpp            # DS3231 RTC interface
│   └── leds.cpp                # NeoPixel LED animations
├── include/
│   ├── config.h                # Pin definitions and constants
│   ├── DisplayManager.h        # Display API
│   ├── ClockManager.h          # Clock state management
│   ├── rtc_time.h              # RTC functions
│   ├── leds.h                  # LED control
│   └── User_Setup.h            # TFT configuration
├── docs/
│   ├── iGO-Vision.md           # Platform vision and architecture
│   ├── iGO-Demo-Plan.md        # 3-day demo preparation roadmap
│   ├── Protocol.md             # API and communication protocols
│   └── Web-Demo-Guide.md       # Web interface implementation guide
├── platformio.ini              # Build configuration
└── README.md                   # This file
```

---

## ⚡ Quick Start

### Prerequisites
- [PlatformIO IDE](https://platformio.org/) (VS Code extension recommended)
- ESP32 board with USB cable
- Components listed in Hardware section

### Wiring Diagram

| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| **ST7789 TFT** | | |
| VCC | 3.3V | Power supply |
| GND | GND | Ground |
| SCL/SCK | GPIO 18 | SPI Clock |
| SDA/MOSI | GPIO 23 | SPI Data |
| RES/RST | GPIO 4 | Reset |
| DC | GPIO 2 | Data/Command |
| BL | GPIO 32 | Backlight (PWM capable) |
| | | |
| **DS3231 RTC** | | |
| VCC | 3.3V | I²C compatible |
| GND | GND | Ground |
| SDA | GPIO 21 | I²C Data |
| SCL | GPIO 22 | I²C Clock |
| | | |
| **WS2812B LEDs** | | |
| VCC | 5V | External power recommended for >5 LEDs |
| GND | GND | Common ground |
| DIN | GPIO 16 | Data input |

### Installation

1. **Clone the repository** (or extract files):
   ```bash
   cd Documents/PlatformIO/Projects/iGO-ESP32
   ```

2. **Install dependencies** (PlatformIO auto-installs from platformio.ini):
   - Adafruit GFX Library
   - Adafruit ST7789 Driver
   - RTClib
   - Adafruit NeoPixel

3. **Upload firmware**:
   ```bash
   pio run -t upload
   ```

4. **Monitor serial output**:
   ```bash
   pio device monitor
   ```

### First Boot
On first boot, the RTC will sync to compile time if not already set. You should see:
```
=== iGO ESP32 Clock v1.0 ===
Display initialized
RTC initialized
RTC time: 2026/01/24 23:15:00
Temperature: 24.5°C
LEDs initialized
Clock manager initialized
System ready
```

---

## 🎨 Customization

### Changing Display Theme
Edit `src/DisplayManager.cpp` to modify colors and layout:
```cpp
// Current theme uses:
ST77XX_WHITE   // Time digits
ST77XX_CYAN    // Date
ST77XX_YELLOW  // Seconds
ST77XX_ORANGE  // Temperature
ST77XX_MAGENTA // Decorative circle
```

### Adjusting Update Rates
Edit `include/config.h`:
```cpp
#define COLON_BLINK_MS 500        // Colon blink speed
#define DISPLAY_UPDATE_MS 500     // Screen refresh rate
#define LED_BREATHE_CYCLE_MS 4000 // LED breathing cycle
```

### LED Animation
Modify `src/leds.cpp` to change LED behavior (color, pattern, speed).

---

## 📡 Connectivity Roadmap

### Planned Connection Methods

#### USB (Desktop App)
- Full-featured desktop application (Windows/Mac/Linux)
- Firmware updates via serial
- Theme and audio file uploads
- Complete configuration management

#### WiFi (Mobile/Web App)
- ESP32 as Access Point (192.168.4.1)
- RESTful API for status and control
- Responsive web dashboard
- Calendar sync and notification push

#### BLE (Mobile App - Offline)
- Low-power connection for mobile app
- Basic notifications and alarm management
- Works without internet/WiFi

#### Charging Dock (Physical Connection)
- Simultaneous data + power for mobile devices
- USB hub functionality (1x input, 2-3x output ports)

---

## 🚀 Next Development Phases

### Phase 1: Web Interface (Current Sprint)
- [x] Core clock and display system
- [ ] WiFi AP mode and web server
- [ ] REST API implementation
- [ ] Mobile-responsive dashboard
- [ ] Notification system
- [ ] Calendar event countdown

### Phase 2: Desktop App Integration
- [ ] Python/Qt desktop application
- [ ] Serial communication protocol
- [ ] Theme designer and uploader
- [ ] Calendar sync (Google/Outlook)
- [ ] Audio file management
- [ ] Firmware updater

### Phase 3: Hardware Expansion
- [ ] DFPlayer Mini integration
- [ ] BMP280 environmental sensor
- [ ] Battery management system
- [ ] Power bank USB hub
- [ ] Load-shedding automation

### Phase 4: Domain Add-Ons
- [ ] iGO-Health firmware module
- [ ] iGO-Drive firmware module
- [ ] iGO-Home firmware module
- [ ] Accessory kits and sensors
- [ ] Commercial licensing

---

## 🏥 Use Cases

### Personal Desk Companion
- Never miss meetings with countdown alerts
- Pomodoro timer for focused work sessions
- Stay informed during load-shedding
- Charge devices via USB hub

### Healthcare (iGO-Health)
- Medication reminder system for elderly
- Vaccine fridge temperature monitoring (clinics)
- Vital signs tracking via BLE sensors
- Caregiver remote monitoring

### Automotive (iGO-Drive)
- Vehicle diagnostics dashboard
- Fuel economy tracking
- Maintenance schedule reminders
- Crash detection and emergency alerts

### Home Automation (iGO-Home)
- Load-shedding pre-cooling automation
- Energy consumption monitoring
- Security system integration
- Smart lighting and occupancy detection

---

## 🛠️ Development

### Building from Source
```bash
# Clean build
pio run -t clean

# Build firmware
pio run

# Upload and monitor
pio run -t upload && pio device monitor
```

### Code Style
- Modular architecture with clear separation of concerns
- Manager classes for each hardware subsystem
- Minimal global state (encapsulated in managers)
- Descriptive function names and comments where needed

### Contributing
This is currently a personal/research project. Feature suggestions and bug reports welcome via issues.

---

## 📊 Technical Specifications

### Performance
- **Display Refresh**: 2 FPS (500ms updates, non-blocking)
- **RTC Accuracy**: ±2ppm (DS3231 crystal)
- **LED Animation**: Smooth 60fps breathing effect
- **Power Consumption**: ~200mA active (TFT + ESP32 + LEDs)

### Memory Usage
- **Flash**: ~350KB / 1.2MB (30% used)
- **RAM**: ~60KB / 320KB (19% used)
- **Headroom**: Plenty of space for features and assets

### Connectivity
- **WiFi**: 2.4GHz 802.11 b/g/n
- **BLE**: Bluetooth 4.2 LE
- **Serial**: 115200 baud UART

---

## 📜 License

This firmware is open-source under the **MIT License**. See LICENSE file for details.

Hardware design and commercial add-ons may use separate licensing.

---

## 🙏 Acknowledgments

Built with:
- **PlatformIO**: Professional embedded development
- **Adafruit Libraries**: Excellent hardware drivers
- **ESP32 Community**: Arduino framework support

Special thanks to the open-source community for making projects like this possible.

---

## 📞 Contact

**Author**: Kabelo Matlakala  
**Project**: iGO Platform (inspectiGO)  
**Focus**: Rural healthcare solutions (Limpopo) + load-shedding resilience  

---

**Last Updated**: January 24, 2026  
**Status**: Active Development (MVP Complete)
