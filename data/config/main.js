const elements = [
    "Name Device", "WiFi SSID", "WiFi Password", "MQTT SSID", "MQTT Password" 
];
const container = document.getElementsByClassName("container")[0];
const configTab = container.querySelector("#config"); 

const socket = new WebSocket(`ws://192.168.4.1/ws`);


document.addEventListener("DOMContentLoaded", () => {
    const title = document.createElement("h2");
    title.textContent = "ESP32 Configuration";
    configTab.appendChild(title);

    const inputs = [];
    elements.forEach(element => {
        const tab = document.createElement("div");
        const label = document.createElement("label");
        label.textContent = `${element}: `;
        const input = document.createElement("input");
        input.type = "text";
        tab.appendChild(label);
        tab.appendChild(input);
        configTab.appendChild(tab);
        inputs.push({ key: element, input });
    });

    const button = document.createElement("button");
    button.textContent = "Save";
    button.addEventListener("click", () => {
        const data = {};
        let hasEmpty = false;
        inputs.forEach(({ key, input }) => {
            data[key] = input.value;
            if (input.value.trim() === "") {
                hasEmpty = true;
            }
        });
        if (hasEmpty) {
            alert("Please fill in all fields.");
            return;
        }
        console.log(JSON.stringify(data, null, 2));
        if (socket.readyState === WebSocket.OPEN) {
            socket.send(JSON.stringify(data));
            inputs.forEach(({ input }) => {
                input.value = "";
            });
            alert("Data Send");
        } else {
            alert("WebSocket not connected.");
        }
    });
    configTab.appendChild(button);
});