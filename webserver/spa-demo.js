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
      <li>Setup WiFi at /setup</li>
      <li>Scan networks via API</li>
      <li>AP: iGO-Buddy (default)</li>
      <li>REST endpoints active</li>
    </ul>
    <div class="btn-row" style="margin-top: 10px;">
      <button class="alt" onclick="window.location.href='/setup'">SETUP</button>
    </div>
  `;

  return [card1, card2];
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
    state.timer.remaining = total;
    logMsg(`Timer set: ${mins}m ${secs}s`);
    updateTimerDisplay();
  }
}

async function startTimer() {
  const res = await apiPost('/api/timer/start', {});
  if (res?.success) {
    state.timer.running = true;
    logMsg('Timer started');
    updateTimerDisplay();
    if (!timerInterval) {
      timerInterval = setInterval(pollTimerStatus, 1000);
    }
  }
}

async function stopTimer() {
  const res = await apiPost('/api/timer/stop', {});
  if (res?.success) {
    state.timer.running = false;
    logMsg('Timer stopped');
    updateTimerDisplay();
  }
}

async function pollTimerStatus() {
  const data = await apiGet('/api/timer/status');
  if (data) {
    state.timer.running = data.running;
    state.timer.remaining = data.remaining;
    updateTimerDisplay();
  }
}

function updateTimerDisplay() {
  const disp = document.getElementById('timer-display');
  const prog = document.getElementById('timer-progress');
  if (!disp) return;
  
  const mins = Math.floor(state.timer.remaining / 60);
  const secs = state.timer.remaining % 60;
  disp.textContent = `${String(mins).padStart(2,'0')}:${String(secs).padStart(2,'0')}`;
  disp.classList.toggle('running', state.timer.running);
  
  if (prog && state.timer.set > 0) {
    const pct = ((state.timer.set - state.timer.remaining) / state.timer.set) * 100;
    prog.style.width = pct + '%';
  }
}

// === Stopwatch Functions ===
async function startStopwatch() {
  const res = await apiPost('/api/stopwatch/start', {});
  if (res?.success) {
    state.stopwatch.running = true;
    logMsg('Stopwatch started');
    if (!stopwatchInterval) {
      stopwatchInterval = setInterval(pollStopwatchStatus, 100);
    }
  }
}

async function stopStopwatch() {
  const res = await apiPost('/api/stopwatch/stop', {});
  if (res?.success) {
    state.stopwatch.running = false;
    logMsg('Stopwatch stopped');
    await pollStopwatchStatus();
  }
}

async function resetStopwatch() {
  const res = await apiPost('/api/stopwatch/reset', {});
  if (res?.success) {
    state.stopwatch = { running: false, elapsed: 0, laps: [] };
    logMsg('Stopwatch reset');
    updateStopwatchDisplay();
    renderLaps();
  }
}

async function lapStopwatch() {
  const res = await apiPost('/api/stopwatch/lap', {});
  if (res?.success) {
    logMsg('Lap recorded');
    await pollStopwatchStatus();
  }
}

async function pollStopwatchStatus() {
  const data = await apiGet('/api/stopwatch/status');
  if (data) {
    state.stopwatch.running = data.running;
    state.stopwatch.elapsed = data.elapsed_ms;
    state.stopwatch.laps = data.laps || [];
    updateStopwatchDisplay();
    renderLaps();
  }
}

function updateStopwatchDisplay() {
  const disp = document.getElementById('stopwatch-display');
  if (!disp) return;
  
  const ms = state.stopwatch.elapsed;
  const mins = Math.floor(ms / 60000);
  const secs = Math.floor((ms % 60000) / 1000);
  const millis = ms % 1000;
  
  disp.textContent = `${String(mins).padStart(2,'0')}:${String(secs).padStart(2,'0')}.${String(millis).padStart(3,'0')}`;
  disp.classList.toggle('running', state.stopwatch.running);
}

function renderLaps() {
  const list = document.getElementById('lap-list');
  if (!list) return;
  if (state.stopwatch.laps.length === 0) {
    list.textContent = '> No laps recorded';
  } else {
    list.textContent = state.stopwatch.laps.map((lap, i) => {
      const ms = lap;
      const mins = Math.floor(ms / 60000);
      const secs = Math.floor((ms % 60000) / 1000);
      const millis = ms % 1000;
      return `LAP ${i+1}: ${String(mins).padStart(2,'0')}:${String(secs).padStart(2,'0')}.${String(millis).padStart(3,'0')}`;
    }).join('\n');
  }
}

// === WiFi Status ===
async function fetchStatus() {
  const data = await apiGet('/api/status');
  if (data) {
    state.uptime = Math.floor(data.uptime_ms / 1000);
    state.wifi.connected = data.home_wifi_connected;
    state.wifi.ip = data.home_wifi_connected ? (data.home_wifi_ip || data.ap_ip) : data.ap_ip;
    
    const statusEl = document.getElementById('wifi-status');
    if (statusEl) {
      statusEl.textContent = data.home_wifi_connected ? 'CONNECTED' : 'AP ONLY';
      statusEl.className = 'badge ' + (data.home_wifi_connected ? 'text-success' : 'text-warn');
    }
    
    const ipEl = document.getElementById('wifi-ip');
    if (ipEl) ipEl.textContent = state.wifi.ip;
    
    const footerIp = document.getElementById('footer-ip');
    if (footerIp) footerIp.textContent = state.wifi.ip;
    
    const uptime = document.getElementById('uptime');
    if (uptime) uptime.textContent = formatUptime(state.uptime);
    
    const footerUptime = document.getElementById('footer-uptime');
    if (footerUptime) footerUptime.textContent = `Uptime: ${formatUptime(state.uptime)}`;
    
    const pill = document.getElementById('status-pill');
    if (pill) pill.textContent = data.home_wifi_connected ? '● ONLINE' : '● AP MODE';
  }
}

function formatUptime(seconds) {
  if (seconds < 60) return `${seconds}s`;
  if (seconds < 3600) return `${Math.floor(seconds/60)}m ${seconds%60}s`;
  return `${Math.floor(seconds/3600)}h ${Math.floor((seconds%3600)/60)}m`;
}

// === LED Ring ===
function renderLedStrip() {
  const strip = document.getElementById("led-strip");
  if (!strip) return;
  strip.innerHTML = "";
  for (let i = 0; i < 10; i++) {
    const led = mk("div", "led" + (i === (Date.now() / 600 | 0) % 10 ? " on" : ""));
    strip.appendChild(led);
  }
}

// === Logging ===
function logMsg(msg) {
  const ts = new Date().toLocaleTimeString();
  state.log.unshift(`[${ts}] ${msg}`);
  if (state.log.length > 50) state.log.pop();
  renderLogs();
}

function renderLogs() {
  const log = document.getElementById("sys-log");
  if (!log) return;
  log.textContent = state.log.slice(0, 20).join("\n");
}

// === Initialization ===
buildNav();
buildViews();
renderLogs();
renderLedStrip();
logMsg('iGO-Buddy Terminal initialized');

// Fetch initial data
fetchSettings();
fetchStatus();
refreshAlarms();
pollTimerStatus();
pollStopwatchStatus();

// Update intervals
setInterval(renderLedStrip, 600);
setInterval(fetchStatus, 5000);
