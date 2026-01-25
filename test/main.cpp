#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <ArduinoJson.h>

// TFT pins
#define TFT_DC    2
#define TFT_RST   4
#define TFT_CS    -1
#define TFT_MOSI  23
#define TFT_SCLK  18
#define TFT_BL    32

// WiFi AP settings
const char* ssid = "iGO-Buddy";
const char* password = "";

// Device state
int battery = 85;
bool charging = false;
String nextEvent = "Team Meeting";
int countdown = 15;
String lastNotification = "";

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
WiFiServer server(80);

// Simple HTML page
const char html_page[] = R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
Content-Length: 2400
Connection: close

<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>iGO-Buddy</title>
<style>
body { font-family: sans-serif; background: #667eea; color: white; margin: 0; padding: 20px; }
.container { max-width: 600px; margin: 0 auto; }
h1 { text-align: center; }
.card { background: rgba(255,255,255,0.2); padding: 20px; border-radius: 10px; margin: 15px 0; }
.status-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
.status-value { font-size: 2em; font-weight: bold; }
button { width: 100%; padding: 12px; margin: 8px 0; background: rgba(255,255,255,0.3); border: 1px solid white; color: white; font-size: 1em; cursor: pointer; border-radius: 5px; }
button:hover { background: rgba(255,255,255,0.5); }
</style>
</head>
<body>
<div class="container">
<h1>🎯 iGO-Buddy</h1>
<div class="card">
  <div class="status-grid">
    <div><div class="status-label">Battery</div><div class="status-value" id="battery">85%</div></div>
    <div><div class="status-label">Status</div><div class="status-value">OK</div></div>
  </div>
</div>
<div class="card">
  <h3>📅 Next Event</h3>
  <div id="event" style="font-size: 1.3em;">Team Meeting</div>
  <div style="font-size: 2.5em; color: #ffd700;">15<span style="font-size: 0.5em;"> min</span></div>
</div>
<div class="card">
  <h3>🔔 Actions</h3>
  <button onclick="location.href='/notify'">Send Notification</button>
  <button onclick="location.href='/alarm'">Test Alarm</button>
  <button onclick="location.href='/status'">Refresh</button>
</div>
</div>
<script>
setInterval(() => { location.reload(); }, 5000);
</script>
</body>
</html>
)";

void updateTFTDashboard() {
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(3);
  tft.setCursor(30, 20);
  tft.print("iGO-Buddy");
  
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 70);
  tft.print("Battery: ");
  tft.setTextColor(battery > 20 ? ST77XX_GREEN : ST77XX_RED);
  tft.print(battery);
  tft.print("%");
  
  tft.setTextColor(ST77XX_MAGENTA);
  tft.setTextSize(2);
  tft.setCursor(20, 130);
  tft.print("Event:");
  
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(20, 155);
  tft.print(nextEvent);
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(70, 185);
  tft.print(countdown);
  tft.setTextSize(2);
  tft.print(" m");
  
  if (lastNotification.length() > 0) {
    tft.setTextColor(ST77XX_ORANGE);
    tft.setTextSize(1);
    tft.setCursor(10, 220);
    tft.print(lastNotification.substring(0, 25));
  }
}

void sendStatusJSON(WiFiClient& client) {
  JsonDocument doc;
  doc["battery"] = battery;
  doc["charging"] = charging;
  doc["nextEvent"] = nextEvent;
  doc["countdown"] = countdown;
  
  String json;
  serializeJson(doc, json);
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + String(json.length()));
  client.println("Connection: close");
  client.println();
  client.print(json);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  
  Serial.println("\niGO-Buddy Web Demo Starting...");
  
  // Initialize TFT
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(3);
  tft.setCursor(20, 100);
  tft.print("iGO-Buddy");
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 140);
  tft.print("Starting AP...");
  
  // Start WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP: ");
  Serial.println(IP);
  
  server.begin();
  Serial.println("Web server started on port 80");
  
  // Show dashboard
  updateTFTDashboard();
}

void loop() {
  static unsigned long lastUpdate = 0;
  
  // Check for web client
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    
    Serial.println(request);
    
    if (request.indexOf("/status") > -1) {
      sendStatusJSON(client);
    } 
    else if (request.indexOf("/notify") > -1) {
      lastNotification = "Meeting Alert";
      updateTFTDashboard();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("Notification sent!");
    }
    else if (request.indexOf("/alarm") > -1) {
      lastNotification = "ALARM!";
      updateTFTDashboard();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("Alarm triggered!");
    }
    else {
      client.print(html_page);
    }
    delay(1);
    client.stop();
  }
  
  // Simulate countdown every 60 seconds
  if (millis() - lastUpdate > 60000) {
    lastUpdate = millis();
    if (countdown > 0) {
      countdown--;
      updateTFTDashboard();
    }
    if (!charging && battery > 0) {
      battery--;
    }
  }
  
  delay(10);
}
