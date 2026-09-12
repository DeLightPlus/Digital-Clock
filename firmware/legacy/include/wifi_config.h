/**
 * wifi_config.h - WiFi Configuration
 * 
 * Home WiFi credentials should be set in .env file:
 *   HOME_WIFI_SSID=YourNetworkName
 *   HOME_WIFI_PASSWORD=YourPassword
 * 
 * Build system reads .env and passes as compile-time defines.
 * These defaults are used if not provided via build flags.
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// Home WiFi Network Credentials (from .env file via build flags)
// These are set during compilation via -D flags in platformio.ini
#ifndef HOME_WIFI_SSID
  #define HOME_WIFI_SSID "VC-2043-84"  // Default: set in .env
#endif

#ifndef HOME_WIFI_PASSWORD
  #define HOME_WIFI_PASSWORD "f939b82250"  // Default: set in .env
#endif

#ifndef IGO_AP_NAME
  #define IGO_AP_NAME "iGO-Buddy"  // AP name
#endif

// iGO-Buddy AP Settings (always active)
#define IGO_AP_SSID IGO_AP_NAME
#define IGO_AP_PASSWORD ""  // Open AP (no password)

// WiFi Connection Timeout (milliseconds)
#define WIFI_CONNECT_TIMEOUT_MS 10000

// Debug mode
#define WIFI_DEBUG 1

// Helper: stringify macros (used if needed for const char*)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#endif // WIFI_CONFIG_H
