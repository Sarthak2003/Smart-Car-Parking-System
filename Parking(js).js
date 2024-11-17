const WebSocket = require('ws');
const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(bodyParser.json());

const PORT = 3000;
const wsServer = new WebSocket.Server({ port: 8080 });

let currentVehicle = null;

// WebSocket for real-time communication
wsServer.on('connection', (socket) => {
    console.log('WebSocket connection established.');

    // Simulate receiving payment status
    app.post('/payment-success', (req, res) => {
        const { vehicleNumber } = req.body;
        if (vehicleNumber === currentVehicle) {
            socket.send(JSON.stringify({ paymentStatus: 'success' }));
        }
        res.sendStatus(200);
    });

    // Simulate vehicle detection (replace with Python integration)
    setInterval(() => {
        const simulatedVehicleNumber = `DL${Math.floor(1000 + Math.random() * 9000)}`;
        currentVehicle = simulatedVehicleNumber;
        socket.send(JSON.stringify({ vehicleNumber: simulatedVehicleNumber }));
    }, 5000);

    socket.on('close', () => {
        console.log('WebSocket connection closed.');
    });
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
