#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cassert>
#include <cstring>

using namespace std;

// Helper function for modular exponentiation
long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Function to calculate GCD
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Modular inverse using Extended Euclidean Algorithm
long long modInverse(long long a, long long m) {
    long long m0 = m;
    long long y = 0, x = 1;

    if (m == 1) return 0;

    while (a > 1) {
        long long q = a / m;
        long long t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0) x += m0;

    return x;
}

// RSA Key Generation
void generateRSAKeys(long long &n, long long &e, long long &d) {
    long long p = 61, q = 53; // Prime numbers for RSA
    n = p * q;
    long long phi = (p - 1) * (q - 1); // Euler's Totient

    e = 17; // A common choice for e
    while (gcd(e, phi) != 1) {
        e++;
    }

    d = modInverse(e, phi); // Calculate d using modular inverse
}

// RSA Encryption (handles AES key as bytes)
vector<long long> rsaEncrypt(const unsigned char* aesKey, size_t keyLength, long long e, long long n) {
    vector<long long> encryptedKey;
    for (size_t i = 0; i < keyLength; i++) {
        encryptedKey.push_back(modExp(aesKey[i], e, n)); // Encrypt each byte of AES key
    }
    return encryptedKey;
}

// RSA Decryption (handles AES key as bytes)
void rsaDecrypt(const vector<long long>& encryptedKey, unsigned char* decryptedKey, long long d, long long n) {
    for (size_t i = 0; i < encryptedKey.size(); i++) {
        decryptedKey[i] = static_cast<unsigned char>(modExp(encryptedKey[i], d, n)); // Decrypt each byte
    }
}

// AES Encryption (simple AES 128 in ECB mode)
void aesEncrypt(const unsigned char* key, unsigned char* data, size_t dataLength) {
    for (size_t i = 0; i < dataLength; i++) {
        data[i] ^= key[i % 16]; // Simple XOR with AES key (ECB Mode)
    }
}

// AES Decryption (simple AES 128 in ECB mode)
void aesDecrypt(const unsigned char* key, unsigned char* data, size_t dataLength) {
    for (size_t i = 0; i < dataLength; i++) {
        data[i] ^= key[i % 16]; // Simple XOR with AES key (ECB Mode)
    }
}

int main() {
    // Step 1: Generate RSA Keys
    long long n, e, d;
    generateRSAKeys(n, e, d);

    cout << "RSA Public Key (n, e): (" << n << ", " << e << ")\n";
    cout << "RSA Private Key (n, d): (" << n << ", " << d << ")\n";

    // Step 2: Take AES Key (128-bit) from user
    unsigned char aesKey[16]; // 128-bit AES key

    cout << "Enter a 128-bit AES key (16 bytes as hex values, e.g., ff76a212...):\n";
    for (int i = 0; i < 16; i++) {
        int temp;
        cin >> hex >> temp;
        aesKey[i] = static_cast<unsigned char>(temp);
    }

    cout << "AES Key: ";
    for (int i = 0; i < 16; i++) {
        cout << hex << (int)aesKey[i] << " ";
    }
    cout << endl;

    // Step 3: Encrypt AES Key using RSA
    vector<long long> encryptedKey = rsaEncrypt(aesKey, 16, e, n);
    cout << "Encrypted AES Key: ";
    for (auto val : encryptedKey) {
        cout << val << " ";
    }
    cout << endl;

    // Step 4: Decrypt AES Key using RSA Private Key
    unsigned char decryptedAesKey[16];
    rsaDecrypt(encryptedKey, decryptedAesKey, d, n);

    cout << "Decrypted AES Key: ";
    for (int i = 0; i < 16; i++) {
        cout << hex << (int)decryptedAesKey[i] << " ";
    }
    cout << endl;

    // Step 5: Encrypt and Decrypt some data using AES
   

    return 0;
}
