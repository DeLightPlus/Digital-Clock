import React, { useEffect, useState } from "react";

const transportLabels = {
  idle: "Idle",
  wifi: "WiFi/AP",
  ble: "Bluetooth",
  fallback: "Fallback"
};

export default function App() {
  const [appInfo, setAppInfo] = useState({ name: "iGO-Desktop", version: "0.1.0" });
  const [transport, setTransport] = useState("idle");
  const [status, setStatus] = useState("Not connected");

  useEffect(() => {
    if (window.igoDesktop?.getAppInfo) {
      window.igoDesktop.getAppInfo().then(setAppInfo);
    }
  }, []);

  return (
    <div className="app">
      <header className="app__header">
        <div>
          <div className="app__title">iGO-Desktop</div>
          <div className="app__subtitle">React + Electron scaffold</div>
        </div>
        <div className="app__meta">
          <span>{appInfo.name}</span>
          <span>v{appInfo.version}</span>
        </div>
      </header>

      <section className="panel">
        <div className="panel__row">
          <span className="label">Transport</span>
          <span className={`pill pill--${transport}`}>{transportLabels[transport]}</span>
        </div>
        <div className="panel__row">
          <span className="label">Status</span>
          <span className="value">{status}</span>
        </div>
        <div className="panel__actions">
          <button className="button" onClick={() => setTransport("wifi")}>
            Use WiFi/AP
          </button>
          <button className="button" onClick={() => setTransport("ble")}>
            Use Bluetooth
          </button>
          <button className="button ghost" onClick={() => {
            setTransport("idle");
            setStatus("Not connected");
          }}>
            Reset
          </button>
        </div>
        <p className="note">
          This is a scaffold. Next step: implement auto-select between WiFi/AP and BLE,
          and wire the UI to the shared API layer.
        </p>
      </section>

      <section className="panel panel--grid">
        <div>
          <h3>Provisioning</h3>
          <p>WiFi setup will be supported via:</p>
          <ul>
            <li>HTTP (AP or STA)</li>
            <li>BLE JSON RPC (SerialBT)</li>
          </ul>
        </div>
        <div>
          <h3>Next Milestones</h3>
          <ul>
            <li>Transport auto-detect</li>
            <li>Shared API client (REST + BLE)</li>
            <li>Mirror web UI layout</li>
          </ul>
        </div>
      </section>
    </div>
  );
}
