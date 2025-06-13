const express = require('express');
const bcrypt = require('bcrypt');
const db = require('./database/connect'); 

const router = express.Router();

router.post('/', async (req, res) => {
  const { username, password } = req.body;

  console.log('Login attempt:', { username, password });

  try {
    const sql = 'SELECT * FROM users WHERE username = ?';
    const results = await new Promise((resolve, reject) => {
      db.query(sql, [username], (err, results) => {
        if (err) {
          return reject(err);
        }
        resolve(results);
      });
    });

    console.log('Database query results:', results);

    if (results.length === 0) {
      return res.status(400).json({ message: 'User not found' });
    }

    const user = results[0];
    const passwordMatch = await bcrypt.compare(password, user.password);

    if (!passwordMatch) {
      return res.status(400).json({ message: 'Incorrect password' });
    }

    res.redirect(`/vpn_interface?username=${encodeURIComponent(username)}`);
 
  } catch (err) {
    console.error('Error during login:', err);
    res.status(500).json({ message: 'Error logging in user' });
  }
});

module.exports = router;
