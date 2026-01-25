// WebManager.cpp - WiFi AP and REST API server with web-based provisioning
#include "WebManager.h"
#include "DisplayManager.h"
#include "DisplaySettings.h"
#include "ClockManager.h"
#include "rtc_time.h"
#include "config.h"
#include "wifi_config.h"
#include "webserver_html.h"
#include "wifi_provisioning.h"
#include <WiFi.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>

// External variables
extern String lastNotification;

// Preferences for persistent storage (like .env)
Preferences preferences;

// WiFi credentials loaded from Preferences
String savedSSID = "";
String savedPassword = "";
String savedAPName = "iGO-Buddy";

// WiFi state tracking
static bool apActive = false;
static bool homeWiFiConnected = false;
static bool hasStoredConfig = false;
static unsigned long lastWiFiCheckTime = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;  // Check every 5 seconds

// Web server
WiFiServer webServer(80);

// Bluetooth Serial
BluetoothSerial SerialBT;
static bool bluetoothEnabled = false;

void initWebServer() {
  Serial.println("\n=== Initializing iGO-Buddy WiFi System ===");
  
  // Load WiFi credentials from Preferences (persistent storage like .env)
  preferences.begin("wifi-config", false);  // false = read/write mode
  savedSSID = preferences.getString("ssid", "");
  savedPassword = preferences.getString("password", "");
  savedAPName = preferences.getString("ap_name", "iGO-Buddy");
  hasStoredConfig = (savedSSID.length() > 0);
  
  Serial.println("\nStep 1: Loading WiFi configuration...");
  if (hasStoredConfig) {
    Serial.println("✓ Found saved WiFi credentials");
    Serial.print("  SSID: ");
    Serial.println(savedSSID);
  } else {
    Serial.println("⚠ No saved WiFi credentials - checking .env defaults");
    // Fallback to .env/wifi_config.h if no saved config
    // Try to use compiled-in defaults from .env
    savedSSID = HOME_WIFI_SSID;
    savedPassword = HOME_WIFI_PASSWORD;
    savedAPName = IGO_AP_NAME;
    
    if (savedSSID.length() > 0) {
      hasStoredConfig = true;
      Serial.println("✓ Using credentials from .env file");
      Serial.print("  SSID: ");
      Serial.println(savedSSID);
    }
  }
  
  // Step 2: Try to connect to home WiFi (if configured)
  if (hasStoredConfig && savedSSID.length() > 0) {
    Serial.println("\nStep 2: Connecting to home network...");
    Serial.print("SSID: ");
    Serial.println(savedSSID);
    
    WiFi.mode(WIFI_AP_STA);  // Dual mode: AP + Station
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
    
    // Wait for connection with timeout
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT_MS) {
      delay(500);
      Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      homeWiFiConnected = true;
      Serial.println("\n✓ Connected to home WiFi!");
      Serial.print("  IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      homeWiFiConnected = false;
      Serial.println("\n✗ Could not connect to home WiFi (using AP-only)");
    }
  } else {
    // No home WiFi configured, AP-only mode
    WiFi.mode(WIFI_AP);
    Serial.println("\nStep 2: No home WiFi configured - AP-only mode");
  }
  
  // Step 3: Start iGO-Buddy AP
  Serial.println("\nStep 3: Starting iGO-Buddy AP...");
  WiFi.softAP(savedAPName.c_str(), "");  // Open AP (no password)
  IPAddress apIP = WiFi.softAPIP();
  
  Serial.println("✓ iGO-Buddy AP Active");
  Serial.print("  SSID: ");
  Serial.println(savedAPName);
  Serial.print("  IP Address: ");
  Serial.println(apIP);
  
  // Step 4: Start web server
  Serial.println("\nStep 4: Starting HTTP server...");
  webServer.begin();
  apActive = true;
  Serial.println("✓ HTTP server running on port 80");
  
  // Step 5: Start Bluetooth Serial
  Serial.println("\nStep 5: Starting Bluetooth...");
  if (SerialBT.begin("iGO-Buddy")) {
    bluetoothEnabled = true;
    Serial.println("✓ Bluetooth Serial active");
    Serial.println("  Name: iGO-Buddy");
    Serial.println("  Connect via Bluetooth terminal app");
  } else {
    Serial.println("⚠ Bluetooth initialization failed");
  }
  
  if (!hasStoredConfig) {
    Serial.println("\n⚠ WiFi Setup Required!");
    Serial.println("  Connect to: " + savedAPName);
    Serial.println("  Open browser: http://192.168.4.1/setup");
    Serial.println("  Configure WiFi from web interface");
  }
}

void updateWebServer() {
  // Handle Bluetooth Serial commands
  if (bluetoothEnabled && SerialBT.available()) {
    String btCommand = SerialBT.readStringUntil('\n');
    btCommand.trim();
    
    Serial.println("BT Command: " + btCommand);
    
    // Process simple text commands
    if (btCommand.startsWith("TIME:")) {
      int format = btCommand.substring(5).toInt();
      if (format == 12 || format == 24) {
        setTimeFormat(format == 12 ? TIME_12H : TIME_24H);
        SerialBT.println("OK: Time format set to " + String(format) + "-hour");
      } else {
        SerialBT.println("ERROR: Use TIME:12 or TIME:24");
      }
    }
    else if (btCommand.startsWith("DATE:")) {
      int format = btCommand.substring(5).toInt();
      if (format >= 0 && format <= 3) {
        setDateFormat((DateFormat)format);
        const char* formats[] = {"DD/MM/YYYY", "MM/DD/YYYY", "YYYY-MM-DD", "Day Name"};
        SerialBT.println("OK: Date format set to " + String(formats[format]));
      } else {
        SerialBT.println("ERROR: Use DATE:0 to DATE:3");
      }
    }
    else if (btCommand == "STATUS") {
      SerialBT.println("=== iGO-Buddy Status ===");
      SerialBT.println("WiFi: " + String(isHomeWiFiConnected() ? "Connected" : "AP Only"));
      SerialBT.println("IP: " + (isHomeWiFiConnected() ? getHomeWiFiIP() : getAPIP()));
      SerialBT.println("Time Format: " + String(getTimeFormat() == TIME_12H ? "12-hour" : "24-hour"));
      SerialBT.println("Date Format: " + String((int)getDateFormat()));
    }
    else if (btCommand == "HELP") {
      SerialBT.println("=== iGO-Buddy Commands ===");
      SerialBT.println("TIME:12 or TIME:24 - Set time format");
      SerialBT.println("DATE:0-3 - Set date format");
      SerialBT.println("STATUS - Show system status");
      SerialBT.println("HELP - Show this help");
    }
    else {
      SerialBT.println("ERROR: Unknown command. Send HELP for commands.");
    }
  }
  
  // Check for incoming HTTP client
  WiFiClient client = webServer.available();
  if (!client) {
    return;
  }
  
  // Read full HTTP request
  String request = "";
  String body = "";
  bool isPost = false;
  int contentLength = 0;
  
  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      
      if (line.startsWith("POST")) {
        isPost = true;
      }
      
      if (line.startsWith("Content-Length:")) {
        contentLength = line.substring(16).toInt();
      }
      
      if (line.length() == 0) {
        // End of headers, read body if POST
        if (isPost && contentLength > 0) {
          for (int i = 0; i < contentLength; i++) {
            if (client.available()) {
              body += (char)client.read();
            }
          }
        }
        break;
      }
      
      if (request.length() == 0) {
        request = line;
      }
    }
  }
  
  Serial.print("HTTP Request: ");
  Serial.println(request);
  
  // Route requests
  if (request.indexOf("GET /setup") > -1) {
    // Serve WiFi setup page
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(HTML_SETTINGS);
    Serial.println(">> Setup page served");
  }
  else if (request.indexOf("GET /api/wifi/scan") > -1) {
    // Scan for WiFi networks
    Serial.println(">> Scanning WiFi networks...");
    int n = WiFi.scanNetworks();
    
    JsonDocument doc;
    JsonArray networks = doc["networks"].to<JsonArray>();
    
    for (int i = 0; i < n && i < 10; i++) {  // Limit to 10 networks
      JsonObject network = networks.add<JsonObject>();
      network["ssid"] = WiFi.SSID(i);
      network["rssi"] = WiFi.RSSI(i);
    }
    
    String json;
    serializeJson(doc, json);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(json);
    
    WiFi.scanDelete();  // Clean up
    Serial.println(">> Scan complete, found " + String(n) + " networks");
  }
  else if (request.indexOf("POST /api/wifi/configure") > -1) {
    // Save WiFi configuration
    Serial.println(">> Saving WiFi config...");
    Serial.println("Body: " + body);
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error) {
      String ssid = doc["ssid"].as<String>();
      String password = doc["password"].as<String>();
      String apName = doc["ap_name"] | "iGO-Buddy";
      
      // Save to Preferences (persistent storage)
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      preferences.putString("ap_name", apName);
      
      client.print("HTTP/1.1 200 OK\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Connection: close\r\n\r\n");
      client.print("{\"success\":true,\"message\":\"WiFi configured\"}");
      
      Serial.println("✓ WiFi config saved!");
      Serial.println("  SSID: " + ssid);
      Serial.println("  AP Name: " + apName);
      Serial.println("  Restarting in 3 seconds...");
      
      delay(500);
      client.stop();
      delay(2500);
      ESP.restart();  // Reboot to apply new settings
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Connection: close\r\n\r\n");
      client.print("{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("POST /api/wifi/reset") > -1) {
    // Clear saved WiFi configuration
    preferences.clear();
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print("{\"success\":true,\"message\":\"WiFi config cleared\"}");
    
    Serial.println("✓ WiFi config cleared!");
  }
  else if (request.indexOf("POST /api/settings/time-format") > -1) {
    // Set time format (12h/24h)
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error && doc.containsKey("format")) {
      int format = doc["format"].as<int>();
      if (format == 0 || format == 1) {
        setTimeFormat((TimeFormat)format);
        
        client.print("HTTP/1.1 200 OK\r\n");
        client.print("Content-Type: application/json\r\n");
        client.print("Connection: close\r\n\r\n");
        client.print("{\"success\":true,\"message\":\"Time format updated\"}");
        
        Serial.print("✓ Time format set to: ");
        Serial.println(format == 1 ? "12-hour" : "24-hour");
      } else {
        client.print("HTTP/1.1 400 Bad Request\r\n");
        client.print("Content-Type: application/json\r\n");
        client.print("Connection: close\r\n\r\n");
        client.print("{\"success\":false,\"message\":\"Invalid format value\"}");
      }
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Connection: close\r\n\r\n");
      client.print("{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("POST /api/settings/date-format") > -1) {
    // Set date format
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error && doc.containsKey("format")) {
      int format = doc["format"].as<int>();
      if (format >= 0 && format <= 3) {
        setDateFormat((DateFormat)format);
        
        client.print("HTTP/1.1 200 OK\r\n");
        client.print("Content-Type: application/json\r\n");
        client.print("Connection: close\r\n\r\n");
        client.print("{\"success\":true,\"message\":\"Date format updated\"}");
        
        Serial.print("✓ Date format set to: ");
        Serial.println(format);
      } else {
        client.print("HTTP/1.1 400 Bad Request\r\n");
        client.print("Content-Type: application/json\r\n");
        client.print("Connection: close\r\n\r\n");
        client.print("{\"success\":false,\"message\":\"Invalid format value\"}");
      }
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Connection: close\r\n\r\n");
      client.print("{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("GET /api/settings") > -1) {
    // Get current settings
    String json = "{";
    json += "\"time_format\":" + String((int)getTimeFormat()) + ",";
    json += "\"date_format\":" + String((int)getDateFormat());
    json += "}";
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(json);
  }
  else if (request.indexOf("GET /api/notify") > -1) {
    // Send notification
    lastNotification = "Web Notification";
    updateClockDisplay(getCurrentTime(), getColonVisible(), getCurrentTemperature());
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Notification sent!");
    Serial.println(">> Notification displayed on TFT");
  }
  else if (request.indexOf("GET /api/alarm") > -1) {
    // Trigger alarm
    lastNotification = "ALARM!";
    updateClockDisplay(getCurrentTime(), getColonVisible(), getCurrentTemperature());
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Alarm triggered!");
    Serial.println(">> Alarm triggered on TFT");
  }
  else if (request.indexOf("GET /api/status") > -1) {
    // Check WiFi status
    if (millis() - lastWiFiCheckTime > WIFI_CHECK_INTERVAL) {
      homeWiFiConnected = (WiFi.status() == WL_CONNECTED);
      lastWiFiCheckTime = millis();
    }
    
    // Return JSON status with network info
    String json = "{\"status\":\"ok\",\"uptime_ms\":" + String(millis()) + 
                  ",\"ap_active\":" + (apActive ? "true" : "false") + 
                  ",\"home_wifi_connected\":" + (homeWiFiConnected ? "true" : "false") +
                  ",\"has_wifi_config\":" + (hasStoredConfig ? "true" : "false") +
                  ",\"ap_ip\":\"" + getAPIP() + "\"";
    
    if (homeWiFiConnected) {
      json += ",\"home_wifi_ip\":\"" + getHomeWiFiIP() + "\"";
    }
    json += "}";
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(json);
  }
  else {
    // Serve dashboard HTML
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html; charset=utf-8\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");
    client.print(HTML_DASHBOARD);
    Serial.println(">> Dashboard served");
  }
  
  delay(1);
  client.stop();
}

String getAPIP() {
  return WiFi.softAPIP().toString();
}

String getHomeWiFiIP() {
  if (isHomeWiFiConnected()) {
    return WiFi.localIP().toString();
  }
  return "Not connected";
}

bool isAPActive() {
  return apActive;
}

bool isHomeWiFiConnected() {
  // Update status periodically
  if (millis() - lastWiFiCheckTime > WIFI_CHECK_INTERVAL) {
    homeWiFiConnected = (WiFi.status() == WL_CONNECTED);
    lastWiFiCheckTime = millis();
  }
  return homeWiFiConnected;
}

bool hasWiFiConfig() {
  return hasStoredConfig;
}

void displayWiFiStatus() {
  Adafruit_ST7789& tft = getTFT();
  tft.setTextSize(1);
  tft.setCursor(18, 30);
  
  // Show WiFi connection status
  if (isHomeWiFiConnected()) {
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    tft.print("WIFI");
  } else if (isAPActive()) {
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.print("AP ONLY");
  } else {
    tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
    tft.print("OFFLINE");
  }
}
