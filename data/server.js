let socket = new WebSocket(`ws://${location.host}/ws`);

socket.onopen = function() {
    console.log('WebSocket connected');
};

socket.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log('Message from server:', data);
};

socket.onclose = function() {
    console.log('WebSocket disconnected');
};

function sendMessage(message) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(message));
    } else {
        console.error('WebSocket is not open. Ready state:', socket.readyState);
    }
}
