#include "RawImageEncryptor.h"
#include <random>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <iostream>

// Simple crypto implementation (for demonstration)
// In production, use OpenSSL or Crypto++

RawImageEncryptor::RawImageEncryptor(const std::string& key) {
    if (!key.empty()) {
        setKey(key);
    } else {
        this->key.resize(32);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        for (size_t i = 0; i < this->key.size(); ++i) {
            this->key[i] = static_cast<unsigned char>(dis(gen));
        }
    }
}

bool RawImageEncryptor::encryptFile(const std::string& inputPath,
                                   const std::string& outputPath,
                                   EncryptionMethod method) {

    std::cout << "Reading file: " << inputPath << std::endl;

    // Read raw image
    std::vector<unsigned char> imageData;
    if (!readFile(inputPath, imageData)) {
        std::cerr << "Failed to read input file!" << std::endl;
        return false;
    }

    std::cout << "File size: " << imageData.size() << " bytes" << std::endl;

    // Encrypt data
    std::vector<unsigned char> encryptedData;

    switch (method) {
        case AES_256_CBC:
            encryptedData = aesEncrypt(imageData);
            break;
        case XOR_SIMPLE:
            encryptedData = xorEncrypt(imageData);
            break;
        case CHACHA20:
            encryptedData = chacha20Encrypt(imageData);
            break;
        default:
            std::cerr << "Unknown encryption method!" << std::endl;
            return false;
    }

    if (encryptedData.empty()) {
        std::cerr << "Encryption failed!" << std::endl;
        return false;
    }

    // Write encrypted file
    if (!writeFile(outputPath, encryptedData)) {
        std::cerr << "Failed to write output file!" << std::endl;
        return false;
    }

    std::cout << "Encryption successful!" << std::endl;
    std::cout << "Output: " << outputPath << std::endl;
    std::cout << "Key: " << vectorToString(key) << std::endl;

    return true;
}

bool RawImageEncryptor::decryptFile(const std::string& inputPath,
                                   const std::string& outputPath,
                                   EncryptionMethod method) {

    std::cout << "Reading encrypted file: " << inputPath << std::endl;

    // Read encrypted data
    std::vector<unsigned char> encryptedData;
    if (!readFile(inputPath, encryptedData)) {
        std::cerr << "Failed to read input file!" << std::endl;
        return false;
    }

    std::cout << "Encrypted file size: " << encryptedData.size() << " bytes" << std::endl;

    // Decrypt data
    std::vector<unsigned char> decryptedData;

    switch (method) {
        case AES_256_CBC:
            decryptedData = aesDecrypt(encryptedData);
            break;
        case XOR_SIMPLE:
            decryptedData = xorDecrypt(encryptedData);
            break;
        case CHACHA20:
            decryptedData = chacha20Decrypt(encryptedData);
            break;
        default:
            std::cerr << "Unknown encryption method!" << std::endl;
            return false;
    }

    if (decryptedData.empty()) {
        std::cerr << "Decryption failed! Check your key." << std::endl;
        return false;
    }

    // Write decrypted raw image
    if (!writeFile(outputPath, decryptedData)) {
        std::cerr << "Failed to write output file!" << std::endl;
        return false;
    }

    std::cout << "Decryption successful!" << std::endl;
    std::cout << "Output: " << outputPath << std::endl;

    return true;
}

// AES Encryption (simplified - use OpenSSL for production)
std::vector<unsigned char> RawImageEncryptor::aesEncrypt(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> result = data;

    // Generate IV (Initialization Vector)
    std::vector<unsigned char> iv(16);
    generateIV(iv);

    // Simple CBC mode simulation
    std::vector<unsigned char> prevBlock = iv;

    for (size_t i = 0; i < result.size(); i += 16) {
        // XOR with previous block
        size_t blockSize = std::min<size_t>(16, result.size() - i);
        for (size_t j = 0; j < blockSize; ++j) {
            result[i + j] ^= prevBlock[j];
        }

        // Simple "encryption" (XOR with key)
        for (size_t j = 0; j < blockSize; ++j) {
            result[i + j] ^= key[j % key.size()];
        }

        // Store this block as previous for next iteration
        prevBlock.assign(result.begin() + i,
                        result.begin() + i + blockSize);
    }

    // Prepend IV to result
    result.insert(result.begin(), iv.begin(), iv.end());

    return result;
}

std::vector<unsigned char> RawImageEncryptor::aesDecrypt(const std::vector<unsigned char>& data) {
    if (data.size() < 16) return {};

    // Extract IV (first 16 bytes)
    std::vector<unsigned char> iv(data.begin(), data.begin() + 16);
    std::vector<unsigned char> result(data.begin() + 16, data.end());

    // Simple CBC mode decryption
    std::vector<unsigned char> prevBlock = iv;

    for (size_t i = 0; i < result.size(); i += 16) {
        std::vector<unsigned char> currentBlock(result.begin() + i,
                                               result.begin() + i + std::min<size_t>(16, result.size() - i));

        // Simple "decryption" (XOR with key)
        std::vector<unsigned char> decryptedBlock = currentBlock;
        for (size_t j = 0; j < decryptedBlock.size(); ++j) {
            decryptedBlock[j] ^= key[j % key.size()];
        }

        // XOR with previous block
        for (size_t j = 0; j < decryptedBlock.size(); ++j) {
            decryptedBlock[j] ^= prevBlock[j];
        }

        // Copy back to result
        for (size_t j = 0; j < decryptedBlock.size(); ++j) {
            result[i + j] = decryptedBlock[j];
        }

        prevBlock = currentBlock;
    }

    return result;
}

// XOR Encryption (simple but fast)
std::vector<unsigned char> RawImageEncryptor::xorEncrypt(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> result = data;

    // Simple XOR with key
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] ^= key[i % key.size()];
    }

    return result;
}

std::vector<unsigned char> RawImageEncryptor::xorDecrypt(const std::vector<unsigned char>& data) {
    // XOR is symmetric, same operation for decryption
    return xorEncrypt(data);
}

// ChaCha20-like stream cipher
std::vector<unsigned char> RawImageEncryptor::chacha20Encrypt(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> result = data;

    // Generate nonce
    std::vector<unsigned char> nonce(12);
    generateIV(nonce);

    size_t counter = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        // Simple key stream simulation
        std::vector<unsigned char> keyStream = key;
        for (auto& byte : keyStream) {
            byte ^= nonce[counter % nonce.size()];
            byte = (byte << 3) | (byte >> 5); // Simple rotation
            byte ^= static_cast<unsigned char>(counter);
            counter++;
        }

        result[i] ^= keyStream[i % keyStream.size()];
    }

    // Prepend nonce
    result.insert(result.begin(), nonce.begin(), nonce.end());

    return result;
}

std::vector<unsigned char> RawImageEncryptor::chacha20Decrypt(const std::vector<unsigned char>& data) {
    if (data.size() < 12) return {};

    // Extract nonce
    std::vector<unsigned char> nonce(data.begin(), data.begin() + 12);
    std::vector<unsigned char> result(data.begin() + 12, data.end());

    // Same operation for decryption (stream cipher)
    size_t counter = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        std::vector<unsigned char> keyStream = key;
        for (auto& byte : keyStream) {
            byte ^= nonce[counter % nonce.size()];
            byte = (byte << 3) | (byte >> 5);
            byte ^= static_cast<unsigned char>(counter);
            counter++;
        }

        result[i] ^= keyStream[i % keyStream.size()];
    }

    return result;
}

// File operations
bool RawImageEncryptor::readFile(const std::string& path, std::vector<unsigned char>& buffer) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return false;
    }

    file.close();
    return true;
}

bool RawImageEncryptor::writeFile(const std::string& path, const std::vector<unsigned char>& buffer) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    file.close();
    return true;
}

// Key management
void RawImageEncryptor::setKey(const std::string& key) {
    this->key = stringToVector(key);
}

std::string RawImageEncryptor::getKey() const {
    return vectorToString(key);
}

bool RawImageEncryptor::saveKeyToFile(const std::string& keyPath) const {
    return writeFile(keyPath, key);
}

bool RawImageEncryptor::loadKeyFromFile(const std::string& keyPath) {
    return readFile(keyPath, key);
}

// Utility functions
std::string RawImageEncryptor::generateRandomKey(size_t length) {
    std::string key;
    key.reserve(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126); // Printable ASCII

    for (size_t i = 0; i < length; ++i) {
        key.push_back(static_cast<char>(dis(gen)));
    }

    return key;
}

std::vector<unsigned char> RawImageEncryptor::stringToVector(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

std::string RawImageEncryptor::vectorToString(const std::vector<unsigned char>& vec) {
    return std::string(vec.begin(), vec.end());
}

void RawImageEncryptor::generateIV(std::vector<unsigned char>& iv) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (auto& byte : iv) {
        byte = static_cast<unsigned char>(dis(gen));
    }
}
