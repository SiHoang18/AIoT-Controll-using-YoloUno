import { sendMessage } from "./server.js";

export const ports = {
  "YoloUno": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 17, 18, 21, 38, 43, 44, 45, 47, 48],
  "ESP32_4IN4OUT": [1, 2, 3, 4, 5, 6]
};
const peripherals = [
  "LED_PORT", "NEOPIXEL_PORT", "RGB_PORT", "LIGHT_PORT", "MOISTURE_PORT",
  "RELAY_PORT", "FAN_PORT", "ULTRASONIC_TRIG_PORT", "ULTRASONIC_ECHO_PORT",
  "MY_SCL", "MY_SDA"
];

const dynamicDevicePorts = [];
let saveButton = null;
let settings;
let maxIndex = -1;

function getUsedDevices() {
  return dynamicDevicePorts.map(entry => entry.deviceSelect.value);
}

function getUsedPorts() {
  return dynamicDevicePorts.map(entry => parseInt(entry.portSelect.value));
}

function createOptionElements(device, usedPorts) {
  return ports[device]
    .filter(port => !usedPorts.includes(port))
    .map(port => {
      const option = document.createElement("option");
      option.value = port;
      option.textContent = port;
      return option;
    });
}

function createDevicePortRow(initialDevice = null, initialPort = null, existingKey = null, initialMode = "0x03") {
  const row = document.createElement("div");
  row.className = "device-port-row";

  const usedDevices = getUsedDevices();
  const usedPorts = getUsedPorts();

  // Device Select
  const deviceSelect = document.createElement("select");
  deviceSelect.className = "device-select";

  peripherals.forEach(peripheral => {
    if (!usedDevices.includes(peripheral) || peripheral === initialDevice) {
      const option = document.createElement("option");
      option.value = peripheral;
      option.textContent = peripheral.replace(/_/g, ' ').toLowerCase().replace(/\b\w/g, c => c.toUpperCase());
      if (peripheral === initialDevice || (!initialDevice && !deviceSelect.value)) {
        option.selected = true;
        if (!initialDevice) initialDevice = peripheral;
      }
      deviceSelect.appendChild(option);
    }
  });

  // Port Select
  const portSelect = document.createElement("select");
  portSelect.className = "port-select";

  const portOptions = createOptionElements("YoloUno", usedPorts.filter(p => p !== initialPort));
  if (!initialPort && portOptions.length > 0) {
    initialPort = portOptions[0].value;
  }

  portOptions.forEach(option => {
    if (String(option.value) === String(initialPort)) option.selected = true;
    portSelect.appendChild(option);
  });

  // Mode Select
  const modeSelect = document.createElement("select");
  modeSelect.className = "mode-select";

  const inputOption = document.createElement("option");
  inputOption.value = "0x01";
  inputOption.textContent = "INPUT";

  const outputOption = document.createElement("option");
  outputOption.value = "0x03";
  outputOption.textContent = "OUTPUT";

  modeSelect.appendChild(inputOption);
  modeSelect.appendChild(outputOption);
  modeSelect.value = initialMode;

  // Remove Button
  const removeBtn = document.createElement("span");
  removeBtn.textContent = "✖";
  removeBtn.className = "remove-btn";
  removeBtn.style.cursor = "pointer";
  removeBtn.style.marginLeft = "10px";
  removeBtn.addEventListener("click", () => {
    row.remove();
    const index = dynamicDevicePorts.findIndex(entry => entry.row === row);
    if (index !== -1) {
      dynamicDevicePorts.splice(index, 1);
    }
    refreshAllSelectOptions();
    if (dynamicDevicePorts.length === 0 && saveButton) {
      saveButton.remove();
      saveButton = null;
    }
  });

  row.appendChild(deviceSelect);
  row.appendChild(portSelect);
  row.appendChild(modeSelect);
  row.appendChild(removeBtn);
  settings.appendChild(row);

  const key = existingKey !== null ? parseInt(existingKey) : ++maxIndex;
  dynamicDevicePorts.push({ row, deviceSelect, portSelect, modeSelect, key });
  refreshAllSelectOptions();

  if (!saveButton) {
    saveButton = document.createElement("button");
    saveButton.textContent = "Save";
    saveButton.addEventListener("click", saveAction);
    saveButton.addEventListener("keydown", event => {
      if (event.key === "Enter") saveAction();
    });
    settings.appendChild(saveButton);
  } else {
    settings.appendChild(saveButton);
  }
}

function refreshAllSelectOptions() {
  const usedDevices = getUsedDevices();
  const usedPorts = getUsedPorts();

  dynamicDevicePorts.forEach(entry => {
    const currentDevice = entry.deviceSelect.value;
    const currentPort = parseInt(entry.portSelect.value);

    // Refresh device options
    while (entry.deviceSelect.firstChild) entry.deviceSelect.removeChild(entry.deviceSelect.firstChild);
    peripherals.forEach(peripheral => {
      if (!usedDevices.includes(peripheral) || peripheral === currentDevice) {
        const option = document.createElement("option");
        option.value = peripheral;
        option.textContent = peripheral.replace(/_/g, ' ').toLowerCase().replace(/\b\w/g, c => c.toUpperCase());
        if (peripheral === currentDevice) option.selected = true;
        entry.deviceSelect.appendChild(option);
      }
    });

    // Refresh port options
    while (entry.portSelect.firstChild) entry.portSelect.removeChild(entry.portSelect.firstChild);
    ports["YoloUno"].forEach(port => {
      if (!usedPorts.includes(port) || port === currentPort) {
        const option = document.createElement("option");
        option.value = port;
        option.textContent = port;
        if (port === currentPort) option.selected = true;
        entry.portSelect.appendChild(option);
      }
    });
  });
}

async function saveAction() {
  const data = {};

  dynamicDevicePorts.forEach(({ deviceSelect, portSelect, modeSelect, key }) => {
    data[key] = {
      device: deviceSelect.value,
      port: parseInt(portSelect.value),
      mode: parseInt(modeSelect.value)
    };
  });

  const msg = {
    title: "ports",
    data: data
  };

  console.log("Saving message:", msg);

  try {
    sendMessage(msg);
    setTimeout(() => location.reload(), 500);
  } catch (error) {
    alert("Failed to save.");
    console.error("Save error:", error);
  }
}

async function loadConfigAndBuildUI() {
  try {
    const response = await fetch("ports.dat");
    if (!response.ok) throw new Error("Failed to fetch config");

    const json = await response.json();
    console.log("Parsed config:", json);

    if (json.title === "ports" && typeof json.data === "object") {
      maxIndex = -1;
      for (const key in json.data) {
        const entry = json.data[key];
        const numericKey = parseInt(key);
        if (!isNaN(numericKey) && numericKey > maxIndex) {
          maxIndex = numericKey;
        }
        if (entry.device && entry.port && peripherals.includes(entry.device)) {
          const mode = entry.mode ? "0x" + Number(entry.mode).toString(16).padStart(2, '0') : "0x03";
          createDevicePortRow(entry.device, entry.port, key, mode);
        }
      }
    }
  } catch (err) {
    console.error("Error loading config:", err);
  }
}

function createAddDeviceButton() {
  const button = document.createElement("button");
  button.textContent = "Add Device";
  button.classList.add("add-device-btn");
  button.addEventListener("click", () => createDevicePortRow());
  settings.appendChild(button);

  const divider = document.createElement("hr");
  divider.className = "section-divider";
  settings.appendChild(divider);
}

async function initSettingUI() {
  settings = document.querySelector(".setting-box");
  if (!settings) {
    console.error("Element with class 'setting-box' not found.");
    return;
  }
  createAddDeviceButton();
}

document.addEventListener("DOMContentLoaded", async () => {
  await initSettingUI();
  await loadConfigAndBuildUI();
});