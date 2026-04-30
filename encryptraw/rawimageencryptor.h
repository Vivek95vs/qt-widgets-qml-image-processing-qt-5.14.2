// Change this line:
std::string getKey() const;  // Add 'const' here

// Also add const to these helper functions:
std::string vectorToString(const std::vector<unsigned char>& vec) const;
std::vector<unsigned char> stringToVector(const std::string& str) const;

// The complete fixed header:
#ifndef RAWIMAGEENCRYPTOR_H
#define RAWIMAGEENCRYPTOR_H

#include <string>
#include <vector>
#include <fstream>

class RawImageEncryptor {
public:
    enum EncryptionMethod {
        AES_256_CBC,
        XOR_SIMPLE,
        CHACHA20
    };

    RawImageEncryptor(const std::string& key = "");

    bool encryptFile(const std::string& inputPath, const std::string& outputPath,
                     EncryptionMethod method = AES_256_CBC);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath,
                     EncryptionMethod method = AES_256_CBC);

    void setKey(const std::string& key);
    std::string getKey() const;  // Added 'const' here
    bool saveKeyToFile(const std::string& keyPath) const;  // Also add 'const' here
    bool loadKeyFromFile(const std::string& keyPath);

    static std::string generateRandomKey(size_t length = 32);

private:
    std::vector<unsigned char> key;

    std::vector<unsigned char> aesEncrypt(const std::vector<unsigned char>& data);
    std::vector<unsigned char> aesDecrypt(const std::vector<unsigned char>& data);

    std::vector<unsigned char> xorEncrypt(const std::vector<unsigned char>& data);
    std::vector<unsigned char> xorDecrypt(const std::vector<unsigned char>& data);

    std::vector<unsigned char> chacha20Encrypt(const std::vector<unsigned char>& data);
    std::vector<unsigned char> chacha20Decrypt(const std::vector<unsigned char>& data);

    bool readFile(const std::string& path, std::vector<unsigned char>& buffer);
    bool writeFile(const std::string& path, const std::vector<unsigned char>& buffer) const;  // Add const

    // Helper functions - add const qualifiers
    std::vector<unsigned char> stringToVector(const std::string& str) const;
    std::string vectorToString(const std::vector<unsigned char>& vec) const;

    void generateIV(std::vector<unsigned char>& iv);
};

#endif
