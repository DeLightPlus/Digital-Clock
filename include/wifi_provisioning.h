#ifndef WIFI_PROVISIONING_H
#define WIFI_PROVISIONING_H

// WiFi Setup page now points users to the SPA WiFi section
const char HTML_SETTINGS[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="refresh" content="0; url=/index.html#wifi">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>WiFi Setup Redirect</title>
	<style>
		body { font-family: Arial, sans-serif; background: #0b0f11; color: #b4f08b; display: grid; place-items: center; height: 100vh; margin: 0; }
		a { color: #8be28b; }
		.card { padding: 24px; border: 1px solid #1f2a2f; border-radius: 8px; background: #10161a; text-align: center; }
	</style>
</head>
<body>
	<div class="card">
		<p>WiFi setup moved to the main control panel.</p>
		<p>If you are not redirected, open <a href="/index.html#wifi">/index.html#wifi</a>.</p>
	</div>
</body>
</html>
)rawliteral";

#endif // WIFI_PROVISIONING_H
