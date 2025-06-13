const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');
const userControlRoute = require('./user_contol'); 
const verificationRoute = require('./verification');
const db = require('./database/connect'); 
const { Server } = require('socket.io');
const http = require('http');

const app = express();
const server = http.createServer(app);
const io = new Server(server); // Attach Socket.IO to the server

// Middleware to parse form data
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Serve static files and routes
app.use(express.static(path.join(__dirname, 'login')));
app.use(express.static(path.join(__dirname, 'register')));
app.use(express.static(path.join(__dirname, 'vpn_interface')));
app.use('/register', userControlRoute);
app.use('/login', verificationRoute);

// POST endpoint to receive encrypted data
app.post("/encrypt", (req, res) => {
    const { encryptedData } = req.body;

    console.log("Received encrypted data:", encryptedData);
    // Respond back with an acknowledgment
    res.json({ message: "Data received successfully" });
});

// Serve HTML files
app.get('/login', (req, res) => {
    res.sendFile(path.join(__dirname, 'login', 'login.html'));
});
app.get('/register', (req, res) => {
    res.sendFile(path.join(__dirname, 'register', 'register.html'));
});
app.get('/vpn_interface', (req, res) => {
    res.sendFile(path.join(__dirname, 'vpn_interface', 'vpn_interface.html'));
});

// Socket.IO setup
io.on('connection', (socket) => {
    console.log('A user connected');
  
    // Handle user disconnection
    socket.on('disconnect', () => {
        console.log('User disconnected');
    });
  
    // Listen for chat messages
    socket.on('chat message', (msg) => {
        console.log(`Received message: ${msg}`);
        // Broadcast the message to all clients except the sender
        socket.broadcast.emit('chat message', msg);
    });
});

// Start the server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});