const cardData = [
    { title: 'LED RGB', mode: 'UNACTIVATE', state: 'OFF'},
    { title: 'LED',mode: 'UNACTIVATE', state: 'OFF' },
    // { title: 'FAN', mode: 'UNACTIVATE', state: 'OFF' },
  ];
const container = document.getElementById('cardContainer');
const customContainer = document.getElementById('customContainer');
let selectedColor = 'ffffff';

document.addEventListener('DOMContentLoaded', () => {
  cardData.forEach(data => {
    const card = document.createElement('div');
    card.className = 'card';
    card.id = data.title.replace(/\s+/g, '-').toLowerCase(); 
    const title = document.createElement('h3');
    title.textContent = data.title;
    const mode = document.createElement('h4');
    mode.textContent = data.mode;
    card.appendChild(title);
    card.appendChild(mode);
    if (data.title === 'LED RGB') {
      const colorPalette = document.createElement('div');
      colorPalette.id = 'colorWheel';
      card.appendChild(colorPalette); 
      const colorPicker = new iro.ColorPicker(colorPalette, {
        width: 180,
        color: "#ff0000",
        borderWidth: 0,
        borderColor: "#000",
      });
      colorPicker.on("color:change", function(color) {
        selectedColor = color.hexString;
        console.log("Selected color:", selectedColor);
      });
    }
    const button = document.createElement('button');
    button.className = 'button';
    button.textContent = 'TURN ON';
    if (data.title === 'LED RGB') {
      button.style.borderRadius = '30px';
    }
    button.addEventListener('click', () => {
        if (data.title === 'LED RGB') {
          if(data.state === 'OFF') {
            data.state = 'ON';
            data.mode = 'AUTOMATIC MODE';
            mode.textContent = data.mode;
            button.textContent = 'SWITCH MODE';
          }else if(data.state === 'ON' && data.mode === 'AUTOMATIC MODE') {
            data.state = 'ON';
            data.mode = 'MANUAL MODE';
            mode.textContent = data.mode;
            button.textContent = 'TURN OFF';
          }else if(data.state === 'ON' && data.mode === 'MANUAL MODE') {
            data.state = 'OFF';
            data.mode = 'UNACTIVATE';
            mode.textContent = data.mode;
            button.textContent = 'TURN ON';
          }
        }else{
          if(data.state === 'OFF') {
            data.state = 'ON';
            data.mode = 'ACTIVATE';
            mode.textContent = data.mode;
            button.textContent = 'TURN OFF';
          }else if(data.state === 'ON') {
            data.state = 'OFF';
            data.mode = 'UNACTIVATE';
            mode.textContent = data.mode;
            button.textContent = 'TURN ON';
          }
        }
    });
    card.appendChild(button);
    container.appendChild(card);
    setInterval(() =>{
      if (data.title === 'LED RGB') {
        msg = {
          title: data.title,
          mode: data.mode,
          state: data.state,
          color: selectedColor
        }
      } else {
        msg = {
          title: data.title,
          mode: data.mode,
          state: data.state
        }
      }
      sendMessage(msg);
    },100);
  });
});

document.addEventListener('DOMContentLoaded', () => {
  const sensorCard = document.createElement('div');
  sensorCard.className = 'card';

  const sensorTitle = document.createElement('h3');
  sensorTitle.textContent = 'Sensor Data';

  const temperature = document.createElement('p');
  const humidity = document.createElement('p');
  const light = document.createElement('p');
  const soil = document.createElement('p');
  const distance = document.createElement('p');
  temperature.innerHTML = `<p><span>Temperature:</span> <span>-- °C</span></p>`;
  humidity.innerHTML = `<p><span>Humidity:</span> <span>-- %</span></p>`;
  light.innerHTML = `<p><span>Light:</span> <span>-- lx</span></p>`;
  soil.innerHTML = `<p><span>Soil Moisture:</span> <span>-- %<span></p>`;
  distance.innerHTML = `<p><span>Distance:</span> <span>-- cm</span></p>`;

  sensorCard.appendChild(sensorTitle);
  sensorCard.appendChild(temperature);
  sensorCard.appendChild(humidity);
  sensorCard.appendChild(light);
  sensorCard.appendChild(soil);
  sensorCard.appendChild(distance);
  container.appendChild(sensorCard);
  socket.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log("Received data:", data);
    if (data !== null) {
      temperature.innerHTML = `<p><span>Temperature:</span> <span>${data.temperature} °C</span></p>`;
      humidity.innerHTML = `<p><span>Humidity:</span> <span>${data.humidity} %</span></p>`;
      light.innerHTML = `<p><span>Light:</span> <span>${data.light} lx</span></p>`;
      soil.innerHTML = `<p><span>Soil Moisture:</span> <span>${data.moisture} %<span></p>`;
      distance.innerHTML = `<p><span>Distance:</span> <span>${data.distance} cm</span></p>`;
    }
  };
});
