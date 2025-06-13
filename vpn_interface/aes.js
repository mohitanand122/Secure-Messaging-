const crypto = require('crypto');

// The key and iv should be kept safe, and should be 32 bytes for AES-256-CBC
const algorithm = 'aes-256-cbc';
const key = Buffer.from('thisisaverysecretkey12345'); // Example key (32 bytes)
const iv = Buffer.from('thisisaverysecretiv123');   // Example IV (16 bytes)

// Function to encrypt a message
function encrypt(text) {
    const cipher = crypto.createCipheriv(algorithm, key, iv);
    let encrypted = cipher.update(text, 'utf8', 'hex');
    encrypted += cipher.final('hex');
    return encrypted;
}

// Function to decrypt a message
function decrypt(encryptedText) {
    const decipher = crypto.createDecipheriv(algorithm, key, iv);
    let decrypted = decipher.update(encryptedText, 'hex', 'utf8');
    decrypted += decipher.final('utf8');
    return decrypted;
}

// Command-line arguments
const args = process.argv.slice(2); // Get command line arguments (for encrypt/decrypt)

if (args.length < 2) {
    console.error('Usage: node encryption_decryption.js <encrypt/decrypt> <message>');
    process.exit(1);
}

const command = args[0]; // 'encrypt' or 'decrypt'
const message = args[1]; // The message to encrypt or decrypt

if (command === 'encrypt') {
    const encryptedMessage = encrypt(message);
    console.log('Encrypted Message:', encryptedMessage);
} else if (command === 'decrypt') {
    const decryptedMessage = decrypt(message);
    console.log('Decrypted Message:', decryptedMessage);
} else {
    console.error('Invalid command. Use "encrypt" or "decrypt".');
    process.exit(1);
}
