// WebManager.cpp - WiFi AP and REST API server with web-based provisioning
#include "WebManager.h"
#include "DisplayManager.h"
#include "DisplaySettings.h"
#include "ClockManager.h"
#include "rtc_time.h"
#include "ControlManager.h"
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
static bool isPaired = false;
static unsigned long lastWiFiCheckTime = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;  // Check every 5 seconds

// Events (ring buffer)
struct DeviceEvent {
  uint32_t id;
  const char* type;
  String message;
  unsigned long timestampMs;
};

static const uint8_t kMaxEvents = 10;
static DeviceEvent eventBuffer[kMaxEvents];
static uint8_t eventHead = 0;
static uint32_t eventCounter = 0;

static void pushEvent(const char* type, const String& message) {
  DeviceEvent& ev = eventBuffer[eventHead];
  ev.id = ++eventCounter;
  ev.type = type;
  ev.message = message;
  ev.timestampMs = millis();
  eventHead = (eventHead + 1) % kMaxEvents;
}

// Web server
WiFiServer webServer(80);

// Bluetooth Serial
BluetoothSerial SerialBT;
static bool bluetoothEnabled = false;

void recordEvent(const char* type, const String& message) {
  pushEvent(type, message);
}

static void sendJsonResponse(WiFiClient& client, int code, const String& body) {
  client.print("HTTP/1.1 ");
  client.print(code);
  client.print(code == 200 ? " OK\r\n" : " Error\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Connection: close\r\n\r\n");
  client.print(body);
}

static String getAuthPin() {
  return preferences.getString("pin", "");
}

static bool isAuthorized(const JsonDocument* doc, const String& headerPin, String* errorOut) {
  String pin = getAuthPin();
  if (pin.length() == 0) {
    return true;
  }
  String provided = headerPin;
  if (provided.length() == 0 && doc) {
    if (doc->containsKey("pin")) {
      provided = (*doc)["pin"].as<String>();
    } else if ((*doc).containsKey("params") && (*doc)["params"].is<JsonObject>()) {
      provided = (*doc)["params"]["pin"].as<String>();
    }
  }
  if (provided == pin) {
    return true;
  }
  if (errorOut) {
    *errorOut = "unauthorized";
  }
  return false;
}

static String buildStatusJson() {
  String json = "{\"status\":\"ok\",\"uptime_ms\":" + String(millis()) +
                ",\"ap_active\":" + (apActive ? "true" : "false") +
                ",\"home_wifi_connected\":" + (homeWiFiConnected ? "true" : "false") +
                ",\"has_wifi_config\":" + (hasStoredConfig ? "true" : "false") +
                ",\"paired\":" + (isPaired ? "true" : "false") +
                ",\"ap_ip\":\"" + getAPIP() + "\"";
  if (homeWiFiConnected) {
    json += ",\"home_wifi_ip\":\"" + getHomeWiFiIP() + "\"";
  }
  json += "}";
  return json;
}

static String buildClockJson() {
  DateTime now = getCurrentTime();
  char timeStr[16];
  char dateStr[16];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", now.day(), now.month(), now.year());
  String json = String("{\"time\":\"") + timeStr + "\",\"date\":\"" + dateStr + "\",\"uptime_ms\":" + String(millis()) + "}";
  return json;
}

void initWebServer() {
  Serial.println("\n=== Initializing iGO-Buddy WiFi System ===");
  
  // Load WiFi credentials from Preferences (persistent storage like .env)
  preferences.begin("wifi-config", false);  // false = read/write mode
  savedSSID = preferences.getString("ssid", "");
  savedPassword = preferences.getString("password", "");
  savedAPName = preferences.getString("ap_name", "iGO-Buddy");
  bool allowEnvDefaults = preferences.getBool("use_env", false);
  isPaired = preferences.getBool("paired", false);
  hasStoredConfig = (savedSSID.length() > 0);
  
  Serial.println("\nStep 1: Loading WiFi configuration...");
  if (hasStoredConfig) {
    Serial.println("✓ Found saved WiFi credentials");
    Serial.print("  SSID: ");
    Serial.println(savedSSID);
  } else {
    Serial.println("⚠ No saved WiFi credentials");
    if (allowEnvDefaults) {
      Serial.println("→ Using .env defaults (opt-in)");
      savedSSID = HOME_WIFI_SSID;
      savedPassword = HOME_WIFI_PASSWORD;
      savedAPName = IGO_AP_NAME;
      
      if (savedSSID.length() > 0) {
        hasStoredConfig = true;
        Serial.println("✓ Using credentials from .env file");
        Serial.print("  SSID: ");
        Serial.println(savedSSID);
      }
    } else {
      Serial.println("→ Skipping .env defaults");
    }
  }
  
  // Step 2: Try to connect to home WiFi (if configured)
  if (hasStoredConfig && savedSSID.length() > 0) {
    Serial.println("\nStep 2: Connecting to home network...");
    Serial.print("SSID: ");
    Serial.println(savedSSID);
    
    WiFi.mode(WIFI_STA);  // Station-only; AP is for provisioning
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
      Serial.println("\n✗ Could not connect to home WiFi (falling back to AP-only)");
    }
  } else {
    // No home WiFi configured, AP-only mode
    Serial.println("\nStep 2: No home WiFi configured - AP-only mode");
  }
  
  // Step 3: Start iGO-Buddy AP only when provisioning is needed
  if (!homeWiFiConnected) {
    Serial.println("\nStep 3: Starting iGO-Buddy AP (provisioning mode)...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(savedAPName.c_str(), "");  // Open AP (no password)
    IPAddress apIP = WiFi.softAPIP();
    
    Serial.println("✓ iGO-Buddy AP Active");
    Serial.print("  SSID: ");
    Serial.println(savedAPName);
    Serial.print("  IP Address: ");
    Serial.println(apIP);
    apActive = true;
  } else {
    apActive = false;
  }
  
  // Step 4: Start web server
  Serial.println("\nStep 4: Starting HTTP server...");
  webServer.begin();
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

  if (homeWiFiConnected) {
    Serial.println("WiFi mode: STA (home network)");
  } else if (apActive) {
    Serial.println("WiFi mode: AP (provisioning)");
  } else {
    Serial.println("WiFi mode: OFFLINE");
  }
  Serial.println("Pairing state: " + String(isPaired ? "paired" : "unpaired"));
}

void updateWebServer() {
  // Handle Bluetooth Serial commands
  if (bluetoothEnabled && SerialBT.available()) {
    String btCommand = SerialBT.readStringUntil('\n');
    btCommand.trim();
    
    Serial.println("BT Command: " + btCommand);
    
    // JSON RPC over BLE (line-delimited)
    if (btCommand.startsWith("{")) {
      StaticJsonDocument<512> req;
      DeserializationError error = deserializeJson(req, btCommand);
      StaticJsonDocument<512> resp;
      String id = req["id"].as<String>();
      const char* method = req["method"] | "";
      JsonObject params = req["params"].as<JsonObject>();
      resp["id"] = id;

      String authError;
      auto replyError = [&](const char* msg) {
        resp["ok"] = false;
        resp["error"] = msg;
      };

      if (error) {
        replyError("invalid_json");
      } else if (strcmp(method, "status.get") == 0) {
        resp["ok"] = true;
        resp["result"] = serialized(buildStatusJson());
      } else if (strcmp(method, "clock.get") == 0) {
        resp["ok"] = true;
        resp["result"] = serialized(buildClockJson());
      } else if (strcmp(method, "wifi.configure") == 0) {
        if (!isAuthorized(&req, "", &authError)) {
          replyError(authError.c_str());
        } else {
          String ssid = params["ssid"].as<String>();
          String password = params["password"].as<String>();
          String apName = params["ap_name"] | "iGO-Buddy";
          if (ssid.length() == 0) {
            replyError("missing_ssid");
          } else {
            preferences.putString("ssid", ssid);
            preferences.putString("password", password);
            preferences.putString("ap_name", apName);
            preferences.putBool("paired", false);
            preferences.putBool("use_env", false);
            resp["ok"] = true;
            resp["result"]["message"] = "WiFi configured";
            String out;
            serializeJson(resp, out);
            SerialBT.println(out);
            delay(200);
            ESP.restart();
            return;
          }
        }
      } else if (strcmp(method, "wifi.reset") == 0) {
        if (!isAuthorized(&req, "", &authError)) {
          replyError(authError.c_str());
        } else {
          preferences.clear();
          preferences.putBool("use_env", false);
          preferences.putBool("paired", false);
          savedSSID = "";
          savedPassword = "";
          savedAPName = IGO_AP_NAME;
          hasStoredConfig = false;
          homeWiFiConnected = false;
          isPaired = false;
          WiFi.disconnect(true, true);
          WiFi.mode(WIFI_AP);
          WiFi.softAP(savedAPName.c_str(), "");
          apActive = true;
          resp["ok"] = true;
          resp["result"]["message"] = "WiFi reset";
        }
      } else if (strcmp(method, "pair.complete") == 0) {
        if (!isAuthorized(&req, "", &authError)) {
          replyError(authError.c_str());
        } else {
          preferences.putBool("paired", true);
          isPaired = true;
          resp["ok"] = true;
          resp["result"]["message"] = "Device paired";
        }
      } else if (strcmp(method, "alarm.add") == 0) {
        uint8_t hour = params["hour"] | 0;
        uint8_t minute = params["minute"] | 0;
        const char* label = params["label"] | "";
        uint8_t idx = 0;
        bool ok = addAlarm(hour, minute, label, idx);
        resp["ok"] = ok;
        if (ok) resp["result"]["index"] = idx;
        else replyError("alarm_full");
      } else if (strcmp(method, "alarm.delete") == 0) {
        uint8_t idx = params["index"] | 0;
        bool ok = deleteAlarm(idx);
        resp["ok"] = ok;
        if (!ok) replyError("invalid_index");
      } else if (strcmp(method, "alarm.list") == 0) {
        StaticJsonDocument<256> doc;
        listAlarms(doc);
        resp["ok"] = true;
        resp["result"] = doc["alarms"];
      } else if (strcmp(method, "timer.set") == 0) {
        uint32_t sec = params["seconds"] | 0;
        setTimer(sec);
        resp["ok"] = true;
      } else if (strcmp(method, "timer.start") == 0) {
        startTimer();
        resp["ok"] = true;
      } else if (strcmp(method, "timer.stop") == 0) {
        stopTimer();
        resp["ok"] = true;
      } else if (strcmp(method, "timer.status") == 0) {
        resp["ok"] = true;
        resp["result"]["running"] = isTimerRunning();
        resp["result"]["remaining_seconds"] = getTimerRemaining();
        resp["result"]["set_seconds"] = getTimerDuration();
      } else if (strcmp(method, "stopwatch.start") == 0) {
        startStopwatch();
        resp["ok"] = true;
      } else if (strcmp(method, "stopwatch.stop") == 0) {
        stopStopwatch();
        resp["ok"] = true;
      } else if (strcmp(method, "stopwatch.reset") == 0) {
        resetStopwatch();
        resp["ok"] = true;
      } else if (strcmp(method, "stopwatch.lap") == 0) {
        bool ok = lapStopwatch();
        resp["ok"] = ok;
        if (!ok) replyError("lap_failed");
      } else if (strcmp(method, "stopwatch.status") == 0) {
        StaticJsonDocument<192> doc;
        doc["running"] = isStopwatchRunning();
        doc["elapsed_ms"] = getStopwatchElapsed();
        getStopwatchLaps(doc);
        resp["ok"] = true;
        resp["result"] = doc;
      } else if (strcmp(method, "settings.get") == 0) {
        resp["ok"] = true;
        resp["result"]["time_format"] = (int)getTimeFormat();
        resp["result"]["date_format"] = (int)getDateFormat();
      } else if (strcmp(method, "settings.timeFormat") == 0) {
        int format = params["format"] | -1;
        if (format == 0 || format == 1) {
          setTimeFormat((TimeFormat)format);
          resp["ok"] = true;
        } else {
          replyError("invalid_format");
        }
      } else if (strcmp(method, "settings.dateFormat") == 0) {
        int format = params["format"] | -1;
        if (format >= 0 && format <= 3) {
          setDateFormat((DateFormat)format);
          resp["ok"] = true;
        } else {
          replyError("invalid_format");
        }
      } else if (strcmp(method, "auth.pin.set") == 0) {
        String pin = params["pin"].as<String>();
        String currentPin = params["current_pin"].as<String>();
        String storedPin = getAuthPin();
        if (storedPin.length() > 0 && storedPin != currentPin) {
          replyError("invalid_pin");
        } else {
          preferences.putString("pin", pin);
          resp["ok"] = true;
          resp["result"]["message"] = pin.length() == 0 ? "PIN cleared" : "PIN set";
        }
      } else {
        replyError("unknown_method");
      }

      String out;
      serializeJson(resp, out);
      SerialBT.println(out);
    }
    // Process simple text commands
    else if (btCommand.startsWith("TIME:")) {
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
    else if (btCommand.startsWith("ALARM ADD ")) {
      String payload = btCommand.substring(10);
      int colonPos = payload.indexOf(':');
      if (colonPos > 0 && colonPos + 3 <= payload.length()) {
        int hour = payload.substring(0, colonPos).toInt();
        int minute = payload.substring(colonPos + 1, colonPos + 3).toInt();
        String label = payload.substring(colonPos + 3);
        label.trim();
        uint8_t idx = 0;
        if (addAlarm(hour, minute, label.c_str(), idx)) {
          SerialBT.println("OK: Alarm added at " + String(hour) + ":" + String(minute) + " (#" + String(idx) + ")");
        } else {
          SerialBT.println("ERROR: Alarm list full");
        }
      } else {
        SerialBT.println("ERROR: Use ALARM ADD HH:MM Label");
      }
    }
    else if (btCommand == "ALARM LIST") {
      StaticJsonDocument<256> doc;
      listAlarms(doc);
      String json; serializeJson(doc, json);
      SerialBT.println(json);
    }
    else if (btCommand.startsWith("ALARM DEL ")) {
      uint8_t idx = btCommand.substring(10).toInt();
      if (deleteAlarm(idx)) SerialBT.println("OK: Alarm deleted");
      else SerialBT.println("ERROR: Invalid index");
    }
    else if (btCommand.startsWith("TIMER SET ")) {
      uint32_t sec = btCommand.substring(10).toInt();
      setTimer(sec);
      SerialBT.println("OK: Timer set to " + String(sec) + "s");
    }
    else if (btCommand == "TIMER START") {
      startTimer();
      SerialBT.println("OK: Timer started");
    }
    else if (btCommand == "TIMER STOP") {
      stopTimer();
      SerialBT.println("OK: Timer stopped");
    }
    else if (btCommand == "STOPWATCH START") { startStopwatch(); SerialBT.println("OK: Stopwatch start"); }
    else if (btCommand == "STOPWATCH STOP") { stopStopwatch(); SerialBT.println("OK: Stopwatch stop"); }
    else if (btCommand == "STOPWATCH RESET") { resetStopwatch(); SerialBT.println("OK: Stopwatch reset"); }
    else if (btCommand == "STOPWATCH LAP") { SerialBT.println(lapStopwatch() ? "OK: Lap" : "ERROR: Lap failed"); }
    else if (btCommand.startsWith("WIFI JSON ")) {
      String payload = btCommand.substring(10);
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        String ssid = doc["ssid"].as<String>();
        String password = doc["password"].as<String>();
        String apName = doc["ap_name"] | "iGO-Buddy";
        if (ssid.length() == 0) {
          SerialBT.println("ERROR: Missing ssid");
        } else {
          preferences.putString("ssid", ssid);
          preferences.putString("password", password);
          preferences.putString("ap_name", apName);
          preferences.putBool("paired", false);
          preferences.putBool("use_env", false);
          SerialBT.println("OK: WiFi configured, rebooting...");
          delay(200);
          ESP.restart();
        }
      } else {
        SerialBT.println("ERROR: Invalid JSON");
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
      SerialBT.println("ALARM ADD HH:MM Label | ALARM LIST | ALARM DEL N");
      SerialBT.println("TIMER SET seconds | TIMER START | TIMER STOP");
      SerialBT.println("STOPWATCH START|STOP|RESET|LAP");
      SerialBT.println("WIFI JSON {\"ssid\":\"...\",\"password\":\"...\",\"ap_name\":\"...\"}");
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
  String pinHeader = "";
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
      if (line.startsWith("X-IGO-PIN:")) {
        pinHeader = line.substring(10);
        pinHeader.trim();
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
  if (request.indexOf("GET /spa-demo.css") > -1 || request.indexOf("GET /index.css") > -1) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/css\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(SPA_CSS);
    Serial.println(">> Served SPA CSS");
  }
  else if (request.indexOf("GET /spa-demo.js") > -1 || request.indexOf("GET /index.js") > -1) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/javascript\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(SPA_JS);
    Serial.println(">> Served SPA JS");
  }
  else if (request.indexOf("GET /setup") > -1) {
    // Serve WiFi setup page
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(HTML_SETTINGS);
    Serial.println(">> Setup page served");
  }
  else if (request.indexOf("GET /api/v1/status") > -1) {
    String json = String("{\"ok\":true,\"result\":") + buildStatusJson() + "}";
    sendJsonResponse(client, 200, json);
  }
  else if (request.indexOf("GET /api/v1/clock") > -1) {
    String json = String("{\"ok\":true,\"result\":") + buildClockJson() + "}";
    sendJsonResponse(client, 200, json);
  }
  else if (request.indexOf("GET /api/v1/events") > -1) {
    int sincePos = request.indexOf("since=");
    uint32_t sinceId = 0;
    if (sincePos > -1) {
      sinceId = request.substring(sincePos + 6).toInt();
    }
    StaticJsonDocument<384> doc;
    JsonArray arr = doc.createNestedArray("events");
    for (uint8_t i = 0; i < kMaxEvents; i++) {
      const DeviceEvent& ev = eventBuffer[i];
      if (ev.id == 0 || ev.id <= sinceId) continue;
      JsonObject obj = arr.add<JsonObject>();
      obj["id"] = ev.id;
      obj["type"] = ev.type;
      obj["message"] = ev.message;
      obj["ts_ms"] = ev.timestampMs;
    }
    String payload;
    serializeJson(doc, payload);
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
  }
  else if (request.indexOf("GET /api/v1/wifi/scan") > -1) {
    int n = WiFi.scanNetworks();
    StaticJsonDocument<384> doc;
    JsonArray networks = doc.createNestedArray("networks");
    for (int i = 0; i < n && i < 10; i++) {
      JsonObject network = networks.add<JsonObject>();
      network["ssid"] = WiFi.SSID(i);
      network["rssi"] = WiFi.RSSI(i);
    }
    String payload;
    serializeJson(doc, payload);
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
    WiFi.scanDelete();
  }
  else if (request.indexOf("POST /api/v1/wifi/configure") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      String authError;
      if (!isAuthorized(&doc, pinHeader, &authError)) {
        sendJsonResponse(client, 401, String("{\"ok\":false,\"error\":\"") + authError + "\"}");
        return;
      }
      String ssid = doc["ssid"].as<String>();
      String password = doc["password"].as<String>();
      String apName = doc["ap_name"] | "iGO-Buddy";
      if (ssid.length() == 0) {
        sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"missing_ssid\"}");
        return;
      }
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      preferences.putString("ap_name", apName);
      preferences.putBool("paired", false);
      preferences.putBool("use_env", false);
      sendJsonResponse(client, 200, "{\"ok\":true,\"result\":{\"message\":\"WiFi configured\"}}");
      delay(500);
      client.stop();
      delay(1500);
      ESP.restart();
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/wifi/reset") > -1) {
    String authError;
    if (!isAuthorized(nullptr, pinHeader, &authError)) {
      sendJsonResponse(client, 401, String("{\"ok\":false,\"error\":\"") + authError + "\"}");
      return;
    }
    preferences.clear();
    preferences.putBool("use_env", false);
    preferences.putBool("paired", false);
    savedSSID = "";
    savedPassword = "";
    savedAPName = IGO_AP_NAME;
    hasStoredConfig = false;
    homeWiFiConnected = false;
    isPaired = false;
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(savedAPName.c_str(), "");
    apActive = true;
    sendJsonResponse(client, 200, "{\"ok\":true,\"result\":{\"message\":\"WiFi reset\"}}");
  }
  else if (request.indexOf("POST /api/v1/pair/complete") > -1) {
    String authError;
    if (!isAuthorized(nullptr, pinHeader, &authError)) {
      sendJsonResponse(client, 401, String("{\"ok\":false,\"error\":\"") + authError + "\"}");
      return;
    }
    preferences.putBool("paired", true);
    isPaired = true;
    sendJsonResponse(client, 200, "{\"ok\":true,\"result\":{\"message\":\"Device paired\"}}");
  }
  else if (request.indexOf("GET /api/v1/settings") > -1) {
    String payload = String("{\"time_format\":") + String((int)getTimeFormat()) +
                     ",\"date_format\":" + String((int)getDateFormat()) + "}";
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
  }
  else if (request.indexOf("POST /api/v1/settings/time-format") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error && doc.containsKey("format")) {
      int format = doc["format"].as<int>();
      if (format == 0 || format == 1) {
        setTimeFormat((TimeFormat)format);
        sendJsonResponse(client, 200, "{\"ok\":true}");
      } else {
        sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_format\"}");
      }
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/settings/date-format") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error && doc.containsKey("format")) {
      int format = doc["format"].as<int>();
      if (format >= 0 && format <= 3) {
        setDateFormat((DateFormat)format);
        sendJsonResponse(client, 200, "{\"ok\":true}");
      } else {
        sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_format\"}");
      }
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/auth/pin") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      String pin = doc["pin"].as<String>();
      String currentPin = doc["current_pin"].as<String>();
      String storedPin = getAuthPin();
      if (storedPin.length() > 0 && storedPin != currentPin) {
        sendJsonResponse(client, 401, "{\"ok\":false,\"error\":\"invalid_pin\"}");
      } else {
        preferences.putString("pin", pin);
        sendJsonResponse(client, 200, pin.length() == 0 ? "{\"ok\":true,\"result\":{\"message\":\"PIN cleared\"}}" : "{\"ok\":true,\"result\":{\"message\":\"PIN set\"}}");
      }
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/alarm/add") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint8_t hour = doc["hour"].as<uint8_t>();
      uint8_t minute = doc["minute"].as<uint8_t>();
      const char* label = doc["label"] | "";
      uint8_t idx = 0;
      bool ok = addAlarm(hour, minute, label, idx);
      sendJsonResponse(client, 200, ok ? String("{\"ok\":true,\"result\":{\"index\":") + String(idx) + "}}" : "{\"ok\":false,\"error\":\"alarm_full\"}");
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/alarm/delete") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint8_t idx = doc["index"].as<uint8_t>();
      bool ok = deleteAlarm(idx);
      sendJsonResponse(client, 200, ok ? "{\"ok\":true}" : "{\"ok\":false,\"error\":\"invalid_index\"}");
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("GET /api/v1/alarm/list") > -1) {
    StaticJsonDocument<256> doc;
    listAlarms(doc);
    String payload;
    serializeJson(doc, payload);
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
  }
  else if (request.indexOf("POST /api/v1/timer/set") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint32_t sec = doc["seconds"].as<uint32_t>();
      setTimer(sec);
      sendJsonResponse(client, 200, "{\"ok\":true}");
    } else {
      sendJsonResponse(client, 400, "{\"ok\":false,\"error\":\"invalid_json\"}");
    }
  }
  else if (request.indexOf("POST /api/v1/timer/start") > -1) {
    startTimer();
    sendJsonResponse(client, 200, "{\"ok\":true}");
  }
  else if (request.indexOf("POST /api/v1/timer/stop") > -1) {
    stopTimer();
    sendJsonResponse(client, 200, "{\"ok\":true}");
  }
  else if (request.indexOf("GET /api/v1/timer/status") > -1) {
    String payload = String("{\"running\":") + (isTimerRunning() ? "true" : "false") +
                     ",\"remaining_seconds\":" + String(getTimerRemaining()) +
                     ",\"set_seconds\":" + String(getTimerDuration()) + "}";
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
  }
  else if (request.indexOf("POST /api/v1/stopwatch/start") > -1) {
    startStopwatch();
    sendJsonResponse(client, 200, "{\"ok\":true}");
  }
  else if (request.indexOf("POST /api/v1/stopwatch/stop") > -1) {
    stopStopwatch();
    sendJsonResponse(client, 200, "{\"ok\":true}");
  }
  else if (request.indexOf("POST /api/v1/stopwatch/reset") > -1) {
    resetStopwatch();
    sendJsonResponse(client, 200, "{\"ok\":true}");
  }
  else if (request.indexOf("POST /api/v1/stopwatch/lap") > -1) {
    bool ok = lapStopwatch();
    sendJsonResponse(client, 200, ok ? "{\"ok\":true}" : "{\"ok\":false,\"error\":\"lap_failed\"}");
  }
  else if (request.indexOf("GET /api/v1/stopwatch/status") > -1) {
    StaticJsonDocument<192> doc;
    doc["running"] = isStopwatchRunning();
    doc["elapsed_ms"] = getStopwatchElapsed();
    getStopwatchLaps(doc);
    String payload;
    serializeJson(doc, payload);
    sendJsonResponse(client, 200, String("{\"ok\":true,\"result\":") + payload + "}");
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
      String authError;
      if (!isAuthorized(&doc, pinHeader, &authError)) {
        sendJsonResponse(client, 401, String("{\"success\":false,\"message\":\"") + authError + "\"}");
        return;
      }
      String ssid = doc["ssid"].as<String>();
      String password = doc["password"].as<String>();
      String apName = doc["ap_name"] | "iGO-Buddy";
      
      // Save to Preferences (persistent storage)
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      preferences.putString("ap_name", apName);
      preferences.putBool("paired", false);
      preferences.putBool("use_env", false);
      
      sendJsonResponse(client, 200, "{\"success\":true,\"message\":\"WiFi configured\"}");
      
      Serial.println("✓ WiFi config saved!");
      Serial.println("  SSID: " + ssid);
      Serial.println("  AP Name: " + apName);
      Serial.println("  Restarting in 3 seconds...");
      
      delay(500);
      client.stop();
      delay(2500);
      ESP.restart();  // Reboot to apply new settings
    } else {
      sendJsonResponse(client, 400, "{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("POST /api/wifi/reset") > -1) {
    String authError;
    if (!isAuthorized(nullptr, pinHeader, &authError)) {
      sendJsonResponse(client, 401, String("{\"success\":false,\"message\":\"") + authError + "\"}");
      return;
    }
    // Clear saved WiFi configuration
    preferences.clear();
    preferences.putBool("use_env", false);
    preferences.putBool("paired", false);

    savedSSID = "";
    savedPassword = "";
    savedAPName = IGO_AP_NAME;
    hasStoredConfig = false;
    homeWiFiConnected = false;
    isPaired = false;

    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(savedAPName.c_str(), "");
    apActive = true;
    
    sendJsonResponse(client, 200, "{\"success\":true,\"message\":\"WiFi config cleared\"}");
    
    Serial.println("✓ WiFi config cleared (AP-only mode)!");
  }
  else if (request.indexOf("POST /api/pair/complete") > -1) {
    String authError;
    if (!isAuthorized(nullptr, pinHeader, &authError)) {
      sendJsonResponse(client, 401, String("{\"success\":false,\"message\":\"") + authError + "\"}");
      return;
    }
    preferences.putBool("paired", true);
    isPaired = true;
    sendJsonResponse(client, 200, "{\"success\":true,\"message\":\"Device paired\"}");
    Serial.println("✓ Device marked as paired");
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
  else if (request.indexOf("POST /api/auth/pin") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      String pin = doc["pin"].as<String>();
      String currentPin = doc["current_pin"].as<String>();
      String storedPin = getAuthPin();
      if (storedPin.length() > 0 && storedPin != currentPin) {
        sendJsonResponse(client, 401, "{\"success\":false,\"message\":\"Invalid PIN\"}");
      } else {
        preferences.putString("pin", pin);
        sendJsonResponse(client, 200, pin.length() == 0 ? "{\"success\":true,\"message\":\"PIN cleared\"}" : "{\"success\":true,\"message\":\"PIN set\"}");
      }
    } else {
      sendJsonResponse(client, 400, "{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("POST /api/alarm/add") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint8_t hour = doc["hour"].as<uint8_t>();
      uint8_t minute = doc["minute"].as<uint8_t>();
      const char* label = doc["label"] | "";
      uint8_t idx = 0;
      bool ok = addAlarm(hour, minute, label, idx);
      client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print(ok ? String("{\"success\":true,\"index\":" + String(idx) + "}") : "{\"success\":false,\"message\":\"Full\"}");
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print("{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("POST /api/alarm/delete") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint8_t idx = doc["index"].as<uint8_t>();
      bool ok = deleteAlarm(idx);
      client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print(ok ? "{\"success\":true}" : "{\"success\":false,\"message\":\"Invalid index\"}");
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print("{\"success\":false,\"message\":\"Invalid JSON\"}");
    }
  }
  else if (request.indexOf("GET /api/alarm/list") > -1) {
    StaticJsonDocument<256> doc;
    listAlarms(doc);
    String json; serializeJson(doc, json);
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print(json);
  }
  else if (request.indexOf("GET /api/clock") > -1) {
    DateTime now = getCurrentTime();
    char timeStr[16];
    char dateStr[16];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", now.day(), now.month(), now.year());
    String json = String("{\"time\":\"") + timeStr + "\",\"date\":\"" + dateStr + "\",\"uptime_ms\":" + String(millis()) + "}";
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print(json);
  }
  else if (request.indexOf("POST /api/timer/set") > -1) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (!error) {
      uint32_t sec = doc["seconds"].as<uint32_t>();
      setTimer(sec);
      client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print("{\"success\":true}\n");
    } else {
      client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
      client.print("{\"success\":false}\n");
    }
  }
  else if (request.indexOf("POST /api/timer/start") > -1) {
    startTimer();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"success\":true}");
  }
  else if (request.indexOf("POST /api/timer/stop") > -1) {
    stopTimer();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"success\":true}");
  }
  else if (request.indexOf("GET /api/timer/status") > -1) {
    String json = "{\"running\":" + String(isTimerRunning() ? "true" : "false") +
            ",\"remaining_seconds\":" + String(getTimerRemaining()) +
            ",\"set_seconds\":" + String(getTimerDuration()) + "}";
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print(json);
  }
  else if (request.indexOf("POST /api/stopwatch/start") > -1) {
    startStopwatch();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"success\":true}");
  }
  else if (request.indexOf("POST /api/stopwatch/stop") > -1) {
    stopStopwatch();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"success\":true}");
  }
  else if (request.indexOf("POST /api/stopwatch/reset") > -1) {
    resetStopwatch();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"success\":true}");
  }
  else if (request.indexOf("POST /api/stopwatch/lap") > -1) {
    bool ok = lapStopwatch();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print(ok ? "{\"success\":true}" : "{\"success\":false}");
  }
  else if (request.indexOf("GET /api/stopwatch/status") > -1) {
    StaticJsonDocument<192> doc;
    doc["running"] = isStopwatchRunning();
    doc["elapsed_ms"] = getStopwatchElapsed();
    getStopwatchLaps(doc);
    String json; serializeJson(doc, json);
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
    client.print(json);
  }
  else if (request.indexOf("GET /api/notify") > -1) {
    // Send notification
    lastNotification = "Web Notification";
    recordEvent("notify", lastNotification);
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
    recordEvent("alarm", lastNotification);
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
    String json = buildStatusJson();
    sendJsonResponse(client, 200, json);
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
  if (isHomeWiFiConnected() && isPaired) {
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    tft.print("WIFI");
  } else if (isHomeWiFiConnected() && !isPaired) {
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.print("SETUP");
  } else if (isAPActive()) {
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.print("AP ONLY");
  } else {
    tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
    tft.print("OFFLINE");
  }
}
