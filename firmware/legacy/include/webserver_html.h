#ifndef WEBSERVER_HTML_H
#define WEBSERVER_HTML_H

// Terminal SPA served directly from flash (HTML + CSS + JS endpoints)
const char HTML_DASHBOARD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>iGO-Buddy Terminal</title>
    <link rel="stylesheet" href="/spa-demo.css">
</head>
<body>
    <header>
        <div class="brand">
            <div class="brand-badge">iGO</div>
            <div>
                <div class="brand-title">iGO-Buddy Control Terminal</div>
                <div class="muted">ESP32 Smart Clock • WiFi • BT • REST API</div>
            </div>
        </div>
        <div class="pill" id="status-pill">● READY</div>
    </header>

    <main>
        <nav id="nav"></nav>
        <div id="views"></div>
    </main>

    <footer>
        <span id="footer-ip">192.168.4.1</span> • 
        <span id="footer-uptime">Uptime: 0s</span> • 
        <span>ESP32-D0WD-V3</span>
    </footer>
    <script src="/spa-demo.js"></script>
</body>
</html>
)rawliteral";

const char SPA_CSS[] PROGMEM = R"rawliteral(
/* Terminal-inspired green/black theme - Mobile first */
:root {
    --bg: #000000;
    --panel: #0a0f0a;
    --accent: #00ff41;
    --accent-dim: #00aa2b;
    --text: #00ff41;
    --text-dim: #00aa2b;
    --muted: #006622;
    --warn: #ffaa00;
    --danger: #ff3333;
    --border: #003311;
    --shadow: 0 0 20px rgba(0, 255, 65, 0.1);
    --glow: 0 0 10px rgba(0, 255, 65, 0.3);
}

* { 
    box-sizing: border-box; 
    margin: 0; 
    padding: 0; 
}

body {
    background: var(--bg);
    color: var(--text);
    font-family: "Courier New", "Monaco", "Consolas", monospace;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    font-size: 14px;
    line-height: 1.5;
}

/* Header */
header {
    padding: 12px 16px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    gap: 12px;
    position: sticky;
    top: 0;
    background: var(--bg);
    z-index: 10;
    border-bottom: 2px solid var(--accent);
    box-shadow: var(--shadow);
}

.brand {
    display: flex;
    align-items: center;
    gap: 10px;
    font-weight: 700;
}

.brand-badge {
    width: 36px;
    height: 36px;
    border: 2px solid var(--accent);
    display: grid;
    place-items: center;
    font-weight: 800;
    color: var(--accent);
    background: var(--bg);
    text-shadow: var(--glow);
}

.brand-title {
    font-size: 14px;
    letter-spacing: 1px;
}

.pill {
    padding: 4px 10px;
    border: 1px solid var(--accent);
    background: var(--panel);
    color: var(--accent);
    font-size: 11px;
    letter-spacing: 0.5px;
    text-shadow: var(--glow);
}

.muted { 
    color: var(--muted); 
    font-size: 11px; 
    letter-spacing: 0.5px;
}

/* Navigation - Mobile first */
nav {
    padding: 12px;
    border-bottom: 1px solid var(--border);
    background: var(--panel);
    overflow-x: auto;
    white-space: nowrap;
    -webkit-overflow-scrolling: touch;
}

.nav-title { 
    font-size: 11px; 
    letter-spacing: 1px; 
    color: var(--muted); 
    margin-bottom: 8px; 
    display: none;
}

.nav-group {
    display: flex;
    gap: 8px;
}

.nav-item {
    display: inline-block;
    padding: 6px 12px;
    color: var(--text-dim);
    text-decoration: none;
    transition: all 0.2s ease;
    font-size: 12px;
    border: 1px solid var(--border);
    background: var(--bg);
    white-space: nowrap;
    letter-spacing: 0.5px;
}

.nav-item:hover { 
    color: var(--accent);
    border-color: var(--accent);
    box-shadow: var(--glow);
}

.nav-item.active {
    background: var(--panel);
    border-color: var(--accent);
    color: var(--accent);
    box-shadow: var(--glow);
}

/* Main content */
main {
    flex: 1;
    display: flex;
    flex-direction: column;
}

section {
    padding: 16px;
    display: none;
    gap: 12px;
}

section.active { 
    display: grid;
    grid-template-columns: 1fr;
}

/* Cards */
.card {
    background: var(--panel);
    border: 1px solid var(--border);
    padding: 16px;
    box-shadow: var(--shadow);
}

.card h3 { 
    font-size: 13px; 
    margin-bottom: 12px; 
    color: var(--accent);
    letter-spacing: 1px;
    text-transform: uppercase;
    border-bottom: 1px solid var(--border);
    padding-bottom: 6px;
}

.stat { 
    font-size: 24px; 
    font-weight: 700; 
    margin: 8px 0; 
    color: var(--accent);
    text-shadow: var(--glow);
    font-family: "Courier New", monospace;
}

.row { 
    display: flex; 
    gap: 8px; 
    flex-wrap: wrap; 
    margin: 8px 0;
}

.badge { 
    padding: 4px 8px; 
    font-size: 11px; 
    background: var(--bg); 
    border: 1px solid var(--border);
    color: var(--text-dim);
    letter-spacing: 0.5px;
}

.progress { 
    width: 100%; 
    height: 8px; 
    background: var(--border); 
    overflow: hidden; 
    margin: 8px 0; 
    border: 1px solid var(--border);
}

.progress span { 
    display: block; 
    height: 100%; 
    background: var(--accent);
    box-shadow: 0 0 10px var(--accent);
}

.btn-row { 
    display: flex; 
    gap: 8px; 
    flex-wrap: wrap; 
    margin: 8px 0;
}

button {
    background: var(--panel);
    color: var(--accent);
    border: 1px solid var(--accent);
    padding: 8px 14px;
    font-weight: 700;
    cursor: pointer;
    transition: all 0.2s ease;
    font-family: inherit;
    font-size: 12px;
    letter-spacing: 0.5px;
    text-transform: uppercase;
}

button:hover { 
    background: var(--accent);
    color: var(--bg);
    box-shadow: var(--glow);
}

button.alt { 
    background: var(--bg); 
    color: var(--text-dim); 
    border-color: var(--border);
}

button.alt:hover {
    color: var(--accent);
    border-color: var(--accent);
}

button:disabled {
    opacity: 0.3;
    cursor: not-allowed;
}

button.danger {
    border-color: var(--danger);
    color: var(--danger);
}

button.danger:hover {
    background: var(--danger);
    color: var(--bg);
}

ul { 
    padding-left: 18px; 
    margin: 8px 0; 
    color: var(--text-dim); 
    font-size: 12px;
    line-height: 1.6;
}

.sub { 
    font-size: 11px; 
    color: var(--muted); 
    margin: 4px 0;
    letter-spacing: 0.5px;
}

/* Terminal/Log styles */
.log { 
    background: var(--bg); 
    border: 1px solid var(--border);
    padding: 12px; 
    font-family: inherit;
    font-size: 11px; 
    height: 160px; 
    overflow-y: auto; 
    color: var(--text-dim);
    line-height: 1.5;
    white-space: pre-wrap;
    word-wrap: break-word;
}

.log::-webkit-scrollbar {
    width: 8px;
}

.log::-webkit-scrollbar-track {
    background: var(--bg);
}

.log::-webkit-scrollbar-thumb {
    background: var(--border);
    border: 1px solid var(--accent-dim);
}

.chip { 
    display: inline-block; 
    padding: 4px 8px; 
    background: var(--bg); 
    margin: 3px; 
    font-size: 11px;
    border: 1px solid var(--border);
    color: var(--text-dim);
    letter-spacing: 0.5px;
}

.hero-time { 
    font-size: 32px; 
    letter-spacing: 2px;
    color: var(--accent);
    text-shadow: var(--glow);
    font-weight: 700;
    margin: 12px 0;
}

/* LED strip */
.led-strip { 
    display: grid; 
    grid-template-columns: repeat(auto-fit, minmax(24px, 1fr)); 
    gap: 6px; 
    margin: 10px 0; 
}

.led { 
    height: 14px; 
    background: var(--border); 
    border: 1px solid var(--border);
}

.led.on { 
    background: var(--accent); 
    border-color: var(--accent);
    box-shadow: 0 0 12px var(--accent);
}

/* Input fields */
input[type="text"],
input[type="number"],
input[type="time"] {
    background: var(--bg);
    border: 1px solid var(--border);
    color: var(--accent);
    padding: 8px 12px;
    font-family: inherit;
    font-size: 12px;
    width: 100%;
    margin: 4px 0;
}

input:focus {
    outline: none;
    border-color: var(--accent);
    box-shadow: var(--glow);
}

/* Form groups */
.form-group {
    margin: 10px 0;
}

.form-group label {
    display: block;
    margin-bottom: 4px;
    color: var(--text-dim);
    font-size: 11px;
    letter-spacing: 0.5px;
    text-transform: uppercase;
}

.grid-2 { 
    display: grid; 
    grid-template-columns: 1fr 1fr; 
    gap: 12px; 
}

/* Timer/Stopwatch display */
.time-display {
    font-size: 36px;
    font-weight: 700;
    text-align: center;
    color: var(--accent);
    text-shadow: var(--glow);
    padding: 16px;
    border: 2px solid var(--accent);
    background: var(--bg);
}

.text-center { text-align: center; }

/* Modal */
.modal { position: fixed; inset: 0; background: rgba(0,0,0,0.7); display: flex; align-items: center; justify-content: center; z-index: 50; padding: 16px; }
.modal.hidden { display: none; }
.modal-card { background: #0d140d; border: 1px solid var(--border); box-shadow: var(--shadow); padding: 16px; width: min(420px, 100%); }
.modal-card h4 { margin-bottom: 10px; letter-spacing: 1px; color: var(--accent); text-transform: uppercase; font-size: 13px; }
.modal-actions { display: flex; gap: 8px; flex-wrap: wrap; margin-top: 10px; }

.hidden { display: none; }

@media (min-width: 700px) {
    section.active {
        grid-template-columns: repeat(2, minmax(0, 1fr));
    }
}
)rawliteral";

const char SPA_JS[] PROGMEM = R"rawliteral(
// iGO-Buddy Terminal Control Panel with Live API Integration
const API_BASE = window.location.origin;

const sections = [
    { id: "clock", label: "CLOCK", cards: () => clockCards() },
    { id: "alarms", label: "ALARMS", cards: () => alarmsCards() },
    { id: "timer", label: "TIMER", cards: () => timerCards() },
    { id: "stopwatch", label: "STOPWATCH", cards: () => stopwatchCards() },
    { id: "wifi", label: "WIFI", cards: () => wifiCards() },
    { id: "led", label: "LED", cards: () => ledCards() },
    { id: "system", label: "SYSTEM", cards: () => systemCards() }
];

const state = {
    time: "--:--:--",
    date: "---",
    timeFormat: 0,
    dateFormat: 0,
    alarms: [],
    timer: { running: false, remaining: 0, set: 0 },
    stopwatch: { running: false, elapsed: 0, laps: [] },
    wifi: { ssid: "---", ip: "192.168.4.1", connected: false },
    uptime: 0,
    log: []
};

let timerInterval, stopwatchInterval, statusInterval;

// === Navigation ===
function buildNav() {
    const nav = document.getElementById("nav");
    const group = document.createElement("div");
    group.className = "nav-group";
    const title = document.createElement("div");
    title.className = "nav-title";
    title.textContent = "MODULES";
    group.appendChild(title);
  
    sections.forEach((s, idx) => {
        const a = document.createElement("a");
        a.href = "#" + s.id;
        a.className = "nav-item" + (idx === 0 ? " active" : "");
        a.textContent = s.label;
        a.onclick = (e) => { e.preventDefault(); activate(s.id); };
        group.appendChild(a);
    });
    nav.appendChild(group);
}

function buildViews() {
    const views = document.getElementById("views");
    sections.forEach((s, idx) => {
        const sec = document.createElement("section");
        sec.id = s.id;
        sec.className = idx === 0 ? "active" : "";
        views.appendChild(sec);
        renderSection(s.id);
    });
}

function activate(id) {
    document.querySelectorAll("section").forEach(sec => sec.classList.toggle("active", sec.id === id));
    document.querySelectorAll(".nav-item").forEach(a => a.classList.toggle("active", a.getAttribute("href") === "#" + id));
  
    // Auto-refresh data when switching views
    if (id === 'alarms') refreshAlarms();
    if (id === 'timer') pollTimerStatus();
    if (id === 'stopwatch') pollStopwatchStatus();
}

function renderSection(id) {
    const sec = document.getElementById(id);
    if (!sec) return;
    sec.innerHTML = "";
    const cards = sections.find(s => s.id === id)?.cards();
    if (cards) cards.forEach(el => sec.appendChild(el));
}

const mk = (tag, cls, text) => { 
    const el = document.createElement(tag); 
    if (cls) el.className = cls; 
    if (text) el.textContent = text; 
    return el; 
};

// === API Calls ===
async function apiGet(endpoint) {
    try {
        const res = await fetch(`${API_BASE}${endpoint}`);
        return await res.json();
    } catch (e) {
        logMsg(`ERROR: GET ${endpoint} failed`);
        return null;
    }
}

async function apiPost(endpoint, data) {
    try {
        const res = await fetch(`${API_BASE}${endpoint}`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        return await res.json();
    } catch (e) {
        logMsg(`ERROR: POST ${endpoint} failed`);
        return null;
    }
}

// === Clock Cards ===
function clockCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>Live Clock</h3>
        <div class="hero-time" id="clock-time">${state.time}</div>
        <div class="sub text-center" id="clock-date">${state.date}</div>
        <div class="row" style="margin-top: 12px;">
            <span class="badge" id="clock-format">24H</span>
            <span class="badge" id="date-format">DD/MM/YYYY</span>
        </div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Display Settings</h3>
        <div class="form-group">
            <label>Time Format</label>
            <div class="btn-row">
                <button onclick="setTimeFormat(1)">12-HOUR</button>
                <button onclick="setTimeFormat(0)">24-HOUR</button>
            </div>
        </div>
        <div class="form-group">
            <label>Date Format</label>
            <div class="btn-row">
                <button class="alt" onclick="setDateFormat(0)">DD/MM/YYYY</button>
                <button class="alt" onclick="setDateFormat(1)">MM/DD/YYYY</button>
                <button class="alt" onclick="setDateFormat(2)">YYYY-MM-DD</button>
            </div>
        </div>
    `;

    return [card1, card2];
}

// === Alarms Cards ===
function alarmsCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>Active Alarms</h3>
        <div id="alarms-list" class="log" style="height: 200px;"></div>
        <div class="btn-row" style="margin-top: 10px;">
            <button onclick="refreshAlarms()">REFRESH</button>
            <button class="danger" onclick="deleteAlarmPrompt()">DELETE</button>
        </div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Add Alarm</h3>
        <div class="form-group">
            <label>Time</label>
            <input type="time" id="alarm-time" value="07:00">
        </div>
        <div class="form-group">
            <label>Label</label>
            <input type="text" id="alarm-label" placeholder="Morning wake-up" maxlength="15">
        </div>
        <button onclick="addAlarm()">ADD ALARM</button>
    `;

    return [card1, card2];
}

// === Timer Cards ===
function timerCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>Timer</h3>
        <div class="time-display" id="timer-display">00:00</div>
        <div class="btn-row">
            <button onclick="startTimer()" id="timer-start">START</button>
            <button class="alt" onclick="stopTimer()" id="timer-stop">STOP</button>
        </div>
        <div class="progress" style="margin-top: 10px;">
            <span id="timer-progress" style="width: 0%;"></span>
        </div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Set Timer</h3>
        <div class="form-group">
            <label>Minutes</label>
            <input type="number" id="timer-minutes" value="5" min="0" max="1440">
        </div>
        <div class="form-group">
            <label>Seconds</label>
            <input type="number" id="timer-seconds" value="0" min="0" max="59">
        </div>
        <button onclick="setTimerDuration()">SET TIMER</button>
    `;

    return [card1, card2];
}

// === Stopwatch Cards ===
function stopwatchCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>Stopwatch</h3>
        <div class="time-display" id="stopwatch-display">00:00.000</div>
        <div class="btn-row">
            <button onclick="startStopwatch()" id="sw-start">START</button>
            <button class="alt" onclick="stopStopwatch()" id="sw-stop">STOP</button>
            <button class="alt" onclick="resetStopwatch()">RESET</button>
            <button class="alt" onclick="lapStopwatch()">LAP</button>
        </div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Lap Times</h3>
        <div id="lap-list" class="log" style="height: 200px;"></div>
    `;

    return [card1, card2];
}

// === WiFi Cards ===
function wifiCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>WiFi Status</h3>
        <div class="stat" id="wifi-ip">${state.wifi.ip}</div>
        <div class="sub">SSID: <span id="wifi-ssid">---</span></div>
        <div class="sub">Mode: <span id="wifi-mode">AP</span></div>
        <div class="row" style="margin-top: 10px;">
            <span class="badge" id="wifi-status">CHECKING...</span>
        </div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Commands</h3>
        <ul>
            <li>Configure WiFi without leaving the app</li>
            <li>Scan networks via API</li>
            <li>AP: iGO-Buddy (default)</li>
            <li>REST endpoints active</li>
        </ul>
        <div class="btn-row" style="margin-top: 10px;">
            <button class="alt" onclick="openWifiModal()">CONFIGURE</button>
            <button class="danger" onclick="resetWiFi()">CLEAR WIFI</button>
        </div>
    `;

    return [card1, card2];
}

// WiFi modal helpers
function buildWifiModal() {
    const modal = document.createElement('div');
    modal.id = 'wifi-modal';
    modal.className = 'modal hidden';
    modal.innerHTML = `
        <div class="modal-card">
            <h4>WiFi Setup</h4>
            <div class="form-group">
                <label>SSID</label>
                <input type="text" id="wifi-ssid-input" placeholder="Network name">
            </div>
            <div class="form-group">
                <label>Password</label>
                <input type="text" id="wifi-pass-input" placeholder="Password">
            </div>
            <div class="form-group">
                <label>AP Name</label>
                <input type="text" id="wifi-ap-input" placeholder="iGO-Buddy" value="iGO-Buddy">
            </div>
            <div class="sub" id="wifi-modal-msg"></div>
            <div class="log" id="wifi-networks" style="height: 140px; margin-top: 8px;">Scan to list networks</div>
            <div class="modal-actions">
                <button onclick="loadWiFiNetworks()">SCAN</button>
                <button onclick="submitWifiConfig()">SAVE & RESTART</button>
                <button class="alt" onclick="closeWifiModal()">CLOSE</button>
            </div>
        </div>`;
    document.body.appendChild(modal);
}

function openWifiModal() {
    const modal = document.getElementById('wifi-modal');
    if (!modal) return;
    modal.classList.remove('hidden');
    loadWiFiNetworks();
}

function closeWifiModal() {
    const modal = document.getElementById('wifi-modal');
    if (modal) modal.classList.add('hidden');
}

function setWifiModalMsg(msg) {
    const el = document.getElementById('wifi-modal-msg');
    if (el) el.textContent = msg || '';
}

async function loadWiFiNetworks() {
    const list = document.getElementById('wifi-networks');
    if (list) list.textContent = 'Scanning...';
    const data = await apiGet('/api/wifi/scan');
    if (!list) return;
    if (data?.networks?.length) {
        list.innerHTML = data.networks.map(n => `${n.ssid} (${n.rssi} dBm)`).join('\n');
        list.onclick = (e) => {
            if (!e.target) return;
            const text = e.target.textContent || '';
            const ssid = text.split(' (')[0];
            const input = document.getElementById('wifi-ssid-input');
            if (input) input.value = ssid;
        };
    } else {
        list.textContent = 'No networks found';
    }
}

async function submitWifiConfig() {
    const ssid = document.getElementById('wifi-ssid-input')?.value || '';
    const password = document.getElementById('wifi-pass-input')?.value || '';
    const ap = document.getElementById('wifi-ap-input')?.value || 'iGO-Buddy';
    if (!ssid) { setWifiModalMsg('Enter SSID'); return; }
    setWifiModalMsg('Saving...');
    const res = await apiPost('/api/wifi/configure', { ssid, password, ap_name: ap });
    if (res?.success) {
        setWifiModalMsg('Saved. Device will restart...');
        logMsg('WiFi configured, restarting');
    } else {
        setWifiModalMsg('Failed to save WiFi');
    }
}

async function resetWiFi() {
    if (!confirm('Clear saved WiFi credentials?')) return;
    const res = await apiPost('/api/wifi/reset', {});
    logMsg(res?.success ? 'WiFi settings cleared' : 'Failed to clear WiFi');
}

// === LED Cards ===
function ledCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>LED Ring (WS2812B)</h3>
        <div class="sub">10-LED NeoPixel Ring</div>
        <div class="led-strip" id="led-strip"></div>
        <div class="sub" style="margin-top: 10px;">Animations tied to seconds/events</div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Patterns</h3>
        <ul>
            <li>Seconds orbit around dial</li>
            <li>Breathing pulse</li>
            <li>Alarm flash sequence</li>
            <li>Timer countdown glow</li>
        </ul>
    `;

    return [card1, card2];
}

// === System Cards ===
function systemCards() {
    const card1 = mk("div", "card");
    card1.innerHTML = `
        <h3>System Log</h3>
        <div class="log" id="sys-log"></div>
    `;

    const card2 = mk("div", "card");
    card2.innerHTML = `
        <h3>Status</h3>
        <div class="row">
            <span class="badge">ESP32-D0WD-V3</span>
            <span class="badge">240MHz</span>
            <span class="badge">WiFi+BT</span>
        </div>
        <div class="sub" style="margin-top: 10px;">
            Uptime: <span id="uptime">0s</span><br>
            Flash: 82% used<br>
            RAM: 18% used
        </div>
    `;

    return [card1, card2];
}

// === Clock Functions ===
async function setTimeFormat(format) {
    const res = await apiPost('/api/settings/time-format', { format });
    if (res?.success) {
        logMsg(`Time format set to ${format === 1 ? '12H' : '24H'}`);
        await fetchSettings();
    }
}

async function setDateFormat(format) {
    const res = await apiPost('/api/settings/date-format', { format });
    if (res?.success) {
        logMsg(`Date format updated`);
        await fetchSettings();
    }
}

async function fetchSettings() {
    const data = await apiGet('/api/settings');
    if (data) {
        state.timeFormat = data.time_format;
        state.dateFormat = data.date_format;
        updateClockDisplay();
    }
}

function updateClockDisplay() {
    const formats = ['DD/MM/YYYY', 'MM/DD/YYYY', 'YYYY-MM-DD', 'DAY NAME'];
    const el = document.getElementById('clock-format');
    if (el) el.textContent = state.timeFormat === 1 ? '12H' : '24H';
    const el2 = document.getElementById('date-format');
    if (el2) el2.textContent = formats[state.dateFormat] || 'DD/MM/YYYY';
}

// === Alarm Functions ===
async function refreshAlarms() {
    const data = await apiGet('/api/alarm/list');
    if (data?.alarms) {
        state.alarms = data.alarms;
        renderAlarmsList();
        logMsg(`Loaded ${state.alarms.length} alarm(s)`);
    }
}

function renderAlarmsList() {
    const list = document.getElementById('alarms-list');
    if (!list) return;
    if (state.alarms.length === 0) {
        list.textContent = '> No alarms set';
    } else {
        list.textContent = state.alarms.map(a => 
            `[${a.index}] ${String(a.hour).padStart(2,'0')}:${String(a.minute).padStart(2,'0')} - ${a.label || 'Alarm'}`
        ).join('\n');
    }
}

async function addAlarm() {
    const time = document.getElementById('alarm-time')?.value || '07:00';
    const label = document.getElementById('alarm-label')?.value || 'Alarm';
    const [hour, minute] = time.split(':').map(Number);
  
    const res = await apiPost('/api/alarm/add', { hour, minute, label });
    if (res?.success) {
        logMsg(`Alarm added: ${time} - ${label}`);
        await refreshAlarms();
        document.getElementById('alarm-label').value = '';
    } else {
        logMsg(`ERROR: Failed to add alarm (${res?.message || 'unknown'})`);
    }
}

async function deleteAlarmPrompt() {
    const idx = prompt('Enter alarm index to delete (check list):');
    if (idx !== null && !isNaN(idx)) {
        const res = await apiPost('/api/alarm/delete', { index: parseInt(idx) });
        if (res?.success) {
            logMsg(`Alarm #${idx} deleted`);
            await refreshAlarms();
        } else {
            logMsg(`ERROR: Failed to delete alarm #${idx}`);
        }
    }
}

// === Timer Functions ===
async function setTimerDuration() {
    const mins = parseInt(document.getElementById('timer-minutes')?.value || 0);
    const secs = parseInt(document.getElementById('timer-seconds')?.value || 0);
    const total = mins * 60 + secs;
  
    const res = await apiPost('/api/timer/set', { seconds: total });
    if (res?.success) {
        state.timer.set = total;
        logMsg(`Timer set to ${total}s`);
        pollTimerStatus();
    }
}

async function startTimer() {
    const res = await apiPost('/api/timer/start', {});
    if (res?.success) {
        state.timer.running = true;
        logMsg('Timer started');
        pollTimerStatus();
    }
}

async function stopTimer() {
    const res = await apiPost('/api/timer/stop', {});
    if (res?.success) {
        state.timer.running = false;
        logMsg('Timer stopped');
        pollTimerStatus();
    }
}

async function pollTimerStatus() {
    const data = await apiGet('/api/timer/status');
    if (!data) return;
    state.timer.running = data.running;
    state.timer.remaining = data.remaining_seconds;
    const disp = document.getElementById('timer-display');
    if (disp) disp.textContent = formatMMSS(data.remaining_seconds);
    const bar = document.getElementById('timer-progress');
    if (bar && data.set_seconds > 0) {
        const pct = Math.max(0, Math.min(100, (data.remaining_seconds / data.set_seconds) * 100));
        bar.style.width = `${pct}%`;
    }
}

function formatMMSS(totalSeconds) {
    const m = Math.floor(totalSeconds / 60);
    const s = totalSeconds % 60;
    return `${String(m).padStart(2,'0')}:${String(s).padStart(2,'0')}`;
}

// === Stopwatch Functions ===
async function startStopwatch() {
    const res = await apiPost('/api/stopwatch/start', {});
    if (res?.success) {
        state.stopwatch.running = true;
        pollStopwatchStatus();
    }
}

async function stopStopwatch() {
    const res = await apiPost('/api/stopwatch/stop', {});
    if (res?.success) {
        state.stopwatch.running = false;
        pollStopwatchStatus();
    }
}

async function resetStopwatch() {
    const res = await apiPost('/api/stopwatch/reset', {});
    if (res?.success) {
        state.stopwatch.elapsed = 0;
        state.stopwatch.laps = [];
        pollStopwatchStatus();
    }
}

async function lapStopwatch() {
    const res = await apiPost('/api/stopwatch/lap', {});
    if (res?.success) {
        logMsg('Lap added');
        pollStopwatchStatus();
    }
}

async function pollStopwatchStatus() {
    const data = await apiGet('/api/stopwatch/status');
    if (!data) return;
    state.stopwatch.running = data.running;
    state.stopwatch.elapsed = data.elapsed_ms;
    state.stopwatch.laps = data.laps || [];
    const disp = document.getElementById('stopwatch-display');
    if (disp) disp.textContent = formatMS(state.stopwatch.elapsed);
    const list = document.getElementById('lap-list');
    if (list) list.textContent = state.stopwatch.laps.map((lap, i) => `#${i+1}: ${formatMS(lap)}`).join('\n');
}

function formatMS(ms) {
    const totalSeconds = Math.floor(ms / 1000);
    const minutes = Math.floor(totalSeconds / 60);
    const seconds = totalSeconds % 60;
    const millis = ms % 1000;
    return `${String(minutes).padStart(2,'0')}:${String(seconds).padStart(2,'0')}.${String(millis).padStart(3,'0')}`;
}

// === WiFi ===
async function fetchWiFi() {
    const data = await apiGet('/api/status');
    if (!data) return;
    state.wifi.connected = data.home_wifi_connected;
    state.wifi.ip = data.home_wifi_connected ? data.home_wifi_ip : data.ap_ip;
    const ssidEl = document.getElementById('wifi-ssid');
    if (ssidEl) ssidEl.textContent = state.wifi.connected ? 'Home WiFi' : 'AP';
    const modeEl = document.getElementById('wifi-mode');
    if (modeEl) modeEl.textContent = state.wifi.connected ? 'STA+AP' : 'AP';
    const ipEl = document.getElementById('wifi-ip');
    if (ipEl) ipEl.textContent = state.wifi.ip;
    const statusBadge = document.getElementById('wifi-status');
    if (statusBadge) {
        statusBadge.textContent = state.wifi.connected ? 'ONLINE' : 'AP ONLY';
        statusBadge.style.borderColor = state.wifi.connected ? '#00ff41' : '#ffaa00';
        statusBadge.style.color = state.wifi.connected ? '#00ff41' : '#ffaa00';
    }
    const footerIp = document.getElementById('footer-ip');
    if (footerIp) footerIp.textContent = state.wifi.ip;
}

// === System / Clock ===
async function fetchClock() {
    const data = await apiGet('/api/clock');
    if (!data) return;
    state.time = data.time;
    state.date = data.date;
    state.uptime = data.uptime_ms;
    updateClockRender();
}

function updateClockRender() {
    const t = document.getElementById('clock-time');
    if (t) t.textContent = state.time;
    const d = document.getElementById('clock-date');
    if (d) d.textContent = state.date;
    const up = document.getElementById('uptime');
    if (up) up.textContent = formatUptime(state.uptime);
    const footerUp = document.getElementById('footer-uptime');
    if (footerUp) footerUp.textContent = `Uptime: ${formatUptime(state.uptime)}`;
}

function formatUptime(ms) {
    const sec = Math.floor(ms / 1000);
    const min = Math.floor(sec / 60);
    const hr = Math.floor(min / 60);
    const days = Math.floor(hr / 24);
    const h = hr % 24;
    const m = min % 60;
    const s = sec % 60;
    if (days > 0) return `${days}d ${h}h ${m}m`;
    if (hr > 0) return `${h}h ${m}m ${s}s`;
    if (min > 0) return `${m}m ${s}s`;
    return `${s}s`;
}

// === LED Mock ===
function initLEDMock() {
    const strip = document.getElementById('led-strip');
    if (!strip) return;
    strip.innerHTML = '';
    for (let i = 0; i < 10; i++) {
        const led = mk('div', 'led');
        strip.appendChild(led);
    }
    let idx = 0;
    setInterval(() => {
        const leds = strip.querySelectorAll('.led');
        leds.forEach(l => l.classList.remove('on'));
        leds[idx % leds.length].classList.add('on');
        idx++;
    }, 1000);
}

// === Logging ===
function logMsg(msg) {
    const log = document.getElementById('sys-log');
    if (log) {
        const stamp = new Date().toLocaleTimeString();
        log.textContent = `${stamp} | ${msg}\n${log.textContent}`.slice(0, 4000);
    }
}

function tickStatus() {
    const pill = document.getElementById('status-pill');
    if (!pill) return;
    const now = new Date();
    const seconds = now.getSeconds();
    pill.textContent = seconds % 2 === 0 ? '● READY' : '● RUN';
}

function startIntervals() {
    statusInterval = setInterval(() => {
        fetchClock();
        fetchWiFi();
        tickStatus();
    }, 2000);
}

// === Init ===
window.onload = async () => {
    buildNav();
    buildViews();
    buildWifiModal();
    initLEDMock();
    await fetchSettings();
    await fetchClock();
    await fetchWiFi();
    refreshAlarms();
    pollTimerStatus();
    pollStopwatchStatus();
    startIntervals();
};
)rawliteral";

#endif // WEBSERVER_HTML_H
