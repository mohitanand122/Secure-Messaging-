#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <ctime>
#include <cstdlib>


// AES constants
const int Nb = 4;  // Number of columns in the state
const int Nk = 4;  // Number of 32-bit words in the key (AES-128)
const int Nr = 10; // Number of rounds (AES-128)

// S-box for SubBytes step
const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};


// Inverse S-box for InvSubBytes step
// Inverse S-box for InvSubBytes step
const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Rcon array for key expansion
    const uint8_t Rcon[11] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };

// Galois field multiplication
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1)
            p ^= a;
        bool hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1b; // 0x1b is the irreducible polynomial
        b >>= 1;
    }
    return p;
}

void SubBytes(uint8_t state[4][Nb]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            state[i][j] = sbox[state[i][j]];
}

void InvSubBytes(uint8_t state[4][Nb]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            state[i][j] = inv_sbox[state[i][j]];
}

void ShiftRows(uint8_t state[4][Nb]) {
    uint8_t temp;
    // Row 1: shift left by 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    // Row 2: shift left by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    // Row 3: shift left by 3 (or right by 1)
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void InvShiftRows(uint8_t state[4][Nb]) {
    uint8_t temp;
    // Row 1: shift right by 1
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    // Row 2: shift right by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    // Row 3: shift right by 3 (or left by 1)
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

void MixColumns(uint8_t state[4][Nb]) {
    uint8_t temp[4];
    for (int c = 0; c < Nb; c++) {
        for (int i = 0; i < 4; i++)
            temp[i] = state[i][c];
        state[0][c] = gmul(0x02, temp[0]) ^ gmul(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state[1][c] = temp[0] ^ gmul(0x02, temp[1]) ^ gmul(0x03, temp[2]) ^ temp[3];
        state[2][c] = temp[0] ^ temp[1] ^ gmul(0x02, temp[2]) ^ gmul(0x03, temp[3]);
        state[3][c] = gmul(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ gmul(0x02, temp[3]);
    }
}

void InvMixColumns(uint8_t state[4][Nb]) {
    uint8_t temp[4];
    for (int c = 0; c < Nb; c++) {
        for (int i = 0; i < 4; i++)
            temp[i] = state[i][c];
        state[0][c] = gmul(0x0e, temp[0]) ^ gmul(0x0b, temp[1]) ^ gmul(0x0d, temp[2]) ^ gmul(0x09, temp[3]);
        state[1][c] = gmul(0x09, temp[0]) ^ gmul(0x0e, temp[1]) ^ gmul(0x0b, temp[2]) ^ gmul(0x0d, temp[3]);
        state[2][c] = gmul(0x0d, temp[0]) ^ gmul(0x09, temp[1]) ^ gmul(0x0e, temp[2]) ^ gmul(0x0b, temp[3]);
        state[3][c] = gmul(0x0b, temp[0]) ^ gmul(0x0d, temp[1]) ^ gmul(0x09, temp[2]) ^ gmul(0x0e, temp[3]);
    }
}

void AddRoundKey(uint8_t state[4][Nb], const uint32_t* round_key) {
    for (int c = 0; c < Nb; c++) {
        uint32_t word = round_key[c];
        state[0][c] ^= (word >> 24) & 0xFF;
        state[1][c] ^= (word >> 16) & 0xFF;
        state[2][c] ^= (word >> 8) & 0xFF;
        state[3][c] ^= word & 0xFF;
    }
}
// Key scheduling algorithm
void KeyExpansion(const uint8_t* key, uint32_t* w) {
    // Initialize the first Nk words of the key schedule with the cipher key
    for (int i = 0; i < Nk; i++) {
        w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | 
               (key[4 * i + 2] << 8) | key[4 * i + 3];
    }

    // Generate the remaining words of the key schedule
    for (int i = Nk; i < Nb * (Nr + 1); i++) {
        uint32_t temp = w[i - 1];  // Previous word

        if (i % Nk == 0) {
            // Rotate the bytes of temp: [a0, a1, a2, a3] -> [a1, a2, a3, a0]
            temp = (temp << 8) | (temp >> 24);

            // Apply S-box transformation to each byte of temp
            temp = (sbox[(temp >> 24) & 0xFF] << 24) |
                   (sbox[(temp >> 16) & 0xFF] << 16) |
                   (sbox[(temp >> 8) & 0xFF] << 8) |
                   sbox[temp & 0xFF];

            // XOR with the appropriate Rcon value
            temp ^= (Rcon[i / Nk] << 24);
        }

        // If Nk > 6, perform additional transformation for AES-256 (not used here)
        w[i] = w[i - Nk] ^ temp;
    }
}

void AES_Encrypt(const uint8_t* input, uint8_t* output, const uint32_t* round_keys) {
    uint8_t state[4][Nb];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            state[j][i] = input[i*4 + j];

    AddRoundKey(state, round_keys);

    for (int round = 1; round < Nr; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, round_keys + round * Nb);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, round_keys + Nr * Nb);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            output[i*4 + j] = state[j][i];
}

void AES_Decrypt(const uint8_t* input, uint8_t* output, const uint32_t* round_keys) {
    uint8_t state[4][Nb];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            state[j][i] = input[i*4 + j];

    AddRoundKey(state, round_keys + Nr * Nb);

    for (int round = Nr - 1; round > 0; round--) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, round_keys + round * Nb);
        InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, round_keys);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < Nb; j++)
            output[i*4 + j] = state[j][i];
}

    void generate_random_key(uint8_t* key, int length) {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < length; ++i) {
            key[i] = static_cast<uint8_t>(rand() % 256);
        }
    }

std::vector<uint8_t> pad_message(const std::vector<uint8_t>& message) {
    int padding_length = 16 - (message.size() % 16);
    std::vector<uint8_t> padded = message;
    padded.insert(padded.end(), padding_length, static_cast<uint8_t>(padding_length));
    return padded;
}

std::vector<uint8_t> unpad_message(const std::vector<uint8_t>& padded_message) {
    if (padded_message.empty()) return padded_message;
    uint8_t padding_length = padded_message.back();
    if (padding_length == 0 || padding_length > padded_message.size()) {
        // Invalid padding
        return padded_message;
    }
    return std::vector<uint8_t>(padded_message.begin(), padded_message.end() - padding_length);
}

void print_hex(const uint8_t* buf, int len) {
    for (int i = 0; i < len; i++)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buf[i]) << " ";
    std::cout << std::dec << std::endl;
}

int main() {
    // Use current time as seed for random generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Generate a random 16-byte key
    uint8_t key[16];
    generate_random_key(key, 16);

    std::cout << "Generated Key: ";
    print_hex(key, 16);

    // Expand the key
    uint32_t round_keys[Nb * (Nr + 1)];
    KeyExpansion(key, round_keys);

    // Simulate a message from client to server
    std::string input_message;
    std::cout << "\nEnter your message: ";
    std::getline(std::cin, input_message);

     std::vector<uint8_t> message(input_message.begin(), input_message.end());
    
    std::cout << "Original message: ";
    for (uint8_t c : message) std::cout << c;
    std::cout << std::endl;

    // Pad the message
    std::vector<uint8_t> padded_message = pad_message(message);

    // Encrypt the message
    std::vector<uint8_t> encrypted_message(padded_message.size());
    for (size_t i = 0; i < padded_message.size(); i += 16) {
        AES_Encrypt(&padded_message[i], &encrypted_message[i], round_keys);
    }

    std::cout << "Encrypted message: ";
    print_hex(encrypted_message.data(), encrypted_message.size());

    // Decrypt the message
    std::vector<uint8_t> decrypted_message(encrypted_message.size());
    for (size_t i = 0; i < encrypted_message.size(); i += 16) {
        AES_Decrypt(&encrypted_message[i], &decrypted_message[i], round_keys);
    }

    // Unpad the decrypted message
    std::vector<uint8_t> unpadded_message = unpad_message(decrypted_message);

    std::cout << "Decrypted message: ";
    for (uint8_t c : unpadded_message) std::cout << c;
    std::cout << std::endl;

    return 0;
}