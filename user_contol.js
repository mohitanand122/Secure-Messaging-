const express = require('express');
const bcrypt = require('bcrypt');
const db = require('./database/connect'); // Ensure this imports correctly

const router = express.Router();

// Handle POST request for user registration
router.post('/', async (req, res) => {
  const { username, password, repeatPassword } = req.body;

  // Log the received registration data for debugging
  console.log('Received registration data:', req.body);

  // Check if all fields are provided
  if (!username || !password || !repeatPassword) {
    return res.status(400).send('All fields are required');
  }

  // Check if passwords match
  if (password !== repeatPassword) {
    console.log('Passwords do not match');
    return res.status(400).send('Passwords do not match');
  }

  try {
    // Check if username already exists
    const userCheckSql = 'SELECT * FROM users WHERE username = ?';
    db.query(userCheckSql, [username], async (err, results) => {
      if (err) {
        console.error('Error checking username:', err);
        return res.status(500).send('Server error');
      }
      if (results.length > 0) {
        console.log('Username already exists');
        return res.status(409).send('Username already taken');
      }

      // Hash the password
      const passwordHash = await bcrypt.hash(password, 10);
      console.log('Password hashed successfully:', passwordHash);

      const sql = 'INSERT INTO users (username, password) VALUES (?, ?)';
      
      // Insert user into the database
      db.query(sql, [username, passwordHash], (err, result) => {
        if (err) {
          console.error('Error inserting into database:', err);
          return res.status(500).send('Error registering user');
        }
        console.log('New user registered successfully:', result);
        res.status(201).send('New record created successfully');
      });
    });
  } catch (error) {
    console.error('Error processing registration:', error);
    res.status(500).send('Error registering user');
  }
});

module.exports = router;
