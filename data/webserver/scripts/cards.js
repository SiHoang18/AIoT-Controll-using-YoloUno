import { sendMessage } from "./server.js";

export class buttonWidget {
  constructor(title, container, name, port, macAddress, mode = "UNACTIVATE", state = "OFF") {
    this.title = title; 
    this.name = name;  
    this.port = port;
    this.macAddress = macAddress;
    this.mode = mode;
    this.state = state;
    
    this.element = document.createElement('div');
    this.element.className = 'card myCard';
    this.element.id = title.replace(/\s+/g, '-').toLowerCase();
    this.element.style.position = 'relative';

    // Widget Title
    this.titleElem = document.createElement('h3');
    this.titleElem.textContent = this.name; 

    // Mode Display
    this.modeElem = document.createElement('h4');
    this.modeElem.textContent = this.mode;

    // Action Button
    this.button = document.createElement('button');
    this.button.className = 'button';

    // Remove Button
    this.removeBtn = document.createElement("button");
    this.removeBtn.textContent = "✖";
    this.removeBtn.className = "widget-remove-btn";
    this.removeBtn.title = "Remove widget";
    this.removeBtn.addEventListener("click", () => {
      this.element.remove();     
      location.reload();         
    });

    this.element.append(this.titleElem, this.modeElem, this.button, this.removeBtn);
    container.appendChild(this.element);
  }

  updateDisplay() {
    this.modeElem.textContent = this.mode;
  }

  send() {
    const msg = {
      title: this.title,
      name: this.name,
      port: this.port,
      mac_address: this.macAddress,
      mode: this.mode,
      state: this.state
    };
    sendMessage(msg);
  }
}

export class LedWidget extends buttonWidget {
  constructor(title, container, name, port, macAddress) {
    super(title, container, name, port, macAddress);

    this.button.textContent = 'TURN ON';

    this.button.addEventListener('click', async () => {
      if (this.state === 'OFF') {
        this.state = 'ON';
        this.mode = 'ACTIVATE';
        this.button.textContent = 'TURN OFF';
      } else {
        this.state = 'OFF';
        this.mode = 'UNACTIVATE';
        this.button.textContent = 'TURN ON';
      }
      this.send();
      this.updateDisplay();
    });

    // Optional: send repeatedly
    // setInterval(() => this.send(), 100);
  }
}

export class RGBWidget extends buttonWidget {
  constructor(title, container, name, port, macAddress) {
    super(title, container, name, port, macAddress);
    this.selectedColor = "#FF0000";
    this.button.textContent = 'TURN ON';
    this.button.style.borderRadius = '30px';

    // Color Picker
    const colorPalette = document.createElement('div');
    colorPalette.id = 'colorWheel';
    this.element.insertBefore(colorPalette, this.button);

    this.colorPicker = new iro.ColorPicker(colorPalette, {
      width: 180,
      color: this.selectedColor,
    });

    this.colorPicker.on("color:change", color => {
      this.selectedColor = color.hexString;
      this.send();
    });

    this.button.addEventListener('click', () => {
      if (this.state === 'OFF') {
        this.state = 'ON';
        this.mode = 'AUTOMATIC MODE';
        this.button.textContent = 'SWITCH MODE';
      } else if (this.mode === 'AUTOMATIC MODE') {
        this.mode = 'MANUAL MODE';
        this.button.textContent = 'TURN OFF';
      } else {
        this.state = 'OFF';
        this.mode = 'UNACTIVATE';
        this.button.textContent = 'TURN ON';
      }
      this.updateDisplay();
      this.send();
    });

    // Optional: send repeatedly
    // setInterval(() => this.send(), 100);
  }

  send() {
    const msg = {
      title: this.title,
      name: this.name,
      port: this.port,
      mac_address: this.macAddress,
      mode: this.mode,
      state: this.state,
      color: this.selectedColor,
    };
    sendMessage(msg);
  }
}
