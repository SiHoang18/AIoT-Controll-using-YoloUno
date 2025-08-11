let socket = new WebSocket(`ws://${location.host}/ws`);

const listeners = [];

socket.onopen = function() {
    console.log('WebSocket connected');
};

socket.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log('Message from server:', data);
    listeners.forEach(callback => callback(data)); 
};

socket.onclose = function() {
    console.log('WebSocket disconnected');
};

export function sendMessage(message) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(message));
    } else {
        console.error('WebSocket is not open. Ready state:', socket.readyState);
    }
}

export function onMessage(callback) {
    listeners.push(callback);
}
