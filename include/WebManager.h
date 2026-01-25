// WebManager.h - WiFi AP and REST API server interface
#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <WiFi.h>

// Initialize WiFi in dual mode: AP + Station (home WiFi)
// Loads credentials from Preferences (non-volatile storage)
void initWebServer();

// Handle incoming web requests (call from loop)
void updateWebServer();

// Get WiFi AP IP address
String getAPIP();

// Get home WiFi IP address (if connected)
String getHomeWiFiIP();

// Check if WiFi AP is active
bool isAPActive();

// Check if connected to home WiFi
bool isHomeWiFiConnected();

// Check if WiFi credentials are configured
bool hasWiFiConfig();

// Display WiFi connection status on TFT screen
void displayWiFiStatus();

#endif // WEB_MANAGER_H
