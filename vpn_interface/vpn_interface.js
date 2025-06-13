// Initialize Socket.IO
const socket = io();

// Get references to DOM elements
const sendButton = document.getElementById('send-button');
const messageInput = document.getElementById('message-input');
const messagesDiv = document.querySelector('.chat-messages');

// Handle VPN connection status
window.addEventListener("DOMContentLoaded", () => {
    const statusDiv = document.querySelector(".status");
    statusDiv.textContent = "Connecting to VPN...";

    // Simulate VPN connecting, then show "Connected"
    setTimeout(() => {
        statusDiv.textContent = "Connected";
        statusDiv.style.color = "#45a247"; // Green for connected
    }, 5000);
});

// Toggle the visibility of the side container
function showContainer() {
    const sideContainer = document.getElementById('sideContainer');
    sideContainer.classList.toggle('show');
}

// Function to send a message
function sendMessage() {
    const message = messageInput.value.trim();

    if (message) {
        // Emit the message to the server
        socket.emit('chat message', message);

        // Display the sent message in the container
        displayMessage(message, "outgoing");

        // Clear input and maintain focus
        messageInput.value = '';
        messageInput.focus();
    }
}

// Function to display a message in the container
function displayMessage(message, type) {
    const messageElement = document.createElement('div');
    messageElement.textContent = message;
    messageElement.classList.add('message', type); // 'outgoing' or 'incoming'
    messagesDiv.appendChild(messageElement);

    // Auto-scroll to the latest message
    messagesDiv.scrollTop = messagesDiv.scrollHeight;
}

// Listen for incoming messages from the server
socket.on('chat message', (msg) => {
    displayMessage(msg, "incoming");
});

// Allow pressing Enter key to send a message
messageInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        sendMessage();
    }
});

// Attach click event to the send button
sendButton.addEventListener('click', sendMessage);