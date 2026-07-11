const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("igoDesktop", {
  getAppInfo: () => ipcRenderer.invoke("app:info")
});
