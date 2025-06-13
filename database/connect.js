// ./database/connect.js
const mysql = require('mysql');

// Create a connection to the database
const db = mysql.createConnection({
  host: 'localhost',  
  user: 'root',       
  password: 'Mohit2023',       
  database: 'vpn'  
});

// Connect to the database
db.connect((err) => {
  if (err) {
    console.error('Error connecting to the database:', err.message);
  } else {
    console.log('Connected to the MySQL database.');
  }
});

module.exports = db;
