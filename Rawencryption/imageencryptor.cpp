#include "imageencryptor.h"
#include <QDataStream>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <zlib.h>
#include <QDebug>

// Simple crypto functions for demonstration
// In production, use proper crypto libraries like Botan or OpenSSL

ImageEncryptor::ImageEncryptor(QObject *parent)
    : QObject(parent)
    , m_algorithm(AES_256)
    , workerThread(new QThread(this))
{
    moveToThread(workerThread);
    workerThread->start();
}

bool ImageEncryptor::encryptWithKeyFile(const QString &inputPath, const QString &outputPath,
                                       const QString &keyPath, bool compress)
{
    currentOperation = {true, inputPath, outputPath, keyPath, "", false, compress};
    QMetaObject::invokeMethod(this, "performEncryption", Qt::QueuedConnection);
    return true; // Operation started asynchronously
}

bool ImageEncryptor::decryptWithKeyFile(const QString &inputPath, const QString &outputPath,
                                       const QString &keyPath)
{
    currentOperation = {false, inputPath, outputPath, keyPath, "", false, false};
    QMetaObject::invokeMethod(this, "performDecryption", Qt::QueuedConnection);
    return true;
}

bool ImageEncryptor::encryptWithPassword(const QString &inputPath, const QString &outputPath,
                                        const QString &password, bool compress)
{
    currentOperation = {true, inputPath, outputPath, "", password, true, compress};
    QMetaObject::invokeMethod(this, "performEncryption", Qt::QueuedConnection);
    return true;
}

bool ImageEncryptor::decryptWithPassword(const QString &inputPath, const QString &outputPath,
                                        const QString &password)
{
    currentOperation = {false, inputPath, outputPath, "", password, true, false};
    QMetaObject::invokeMethod(this, "performDecryption", Qt::QueuedConnection);
    return true;
}

void ImageEncryptor::performEncryption()
{
    try {
        // Read input file
        QByteArray inputData;
        if (!readFile(currentOperation.inputPath, inputData)) {
            emit operationComplete(false, "Failed to read input file");
            return;
        }

        emit progressChanged(10);

        // Compress if requested
        QByteArray dataToEncrypt = inputData;
        if (currentOperation.compress) {
            dataToEncrypt = compressData(inputData);
            emit progressChanged(30);
        }

        // Get encryption key
        QByteArray key;
        if (currentOperation.usePassword) {
            // Generate key from password
            QByteArray salt(16, 0);
            for (int i = 0; i < salt.size(); ++i) {
                salt[i] = QRandomGenerator::global()->generate() % 256;
            }
            key = generateKeyFromPassword(currentOperation.password, salt);

            // Prepend salt to encrypted data
            dataToEncrypt = salt + dataToEncrypt;
        } else {
            // Read key from file
            if (!readFile(currentOperation.keyPath, key)) {
                emit operationComplete(false, "Failed to read key file");
                return;
            }
        }

        emit progressChanged(50);

        // Encrypt data
        QByteArray encryptedData = encryptData(dataToEncrypt, key);
        if (encryptedData.isEmpty()) {
            emit operationComplete(false, "Encryption failed");
            return;
        }

        emit progressChanged(70);

        // Add header with metadata
        QByteArray outputData;
        QDataStream stream(&outputData, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_15);

        // File format version
        stream << (quint8)0x01;

        // Algorithm
        stream << (quint8)m_algorithm;

        // Compression flag
        stream << (quint8)(currentOperation.compress ? 1 : 0);

        // Password flag
        stream << (quint8)(currentOperation.usePassword ? 1 : 0);

        // Data size
        stream << (quint64)encryptedData.size();

        // Encrypted data
        stream.writeRawData(encryptedData.constData(), encryptedData.size());

        emit progressChanged(90);

        // Write output file
        if (!writeFile(currentOperation.outputPath, outputData)) {
            emit operationComplete(false, "Failed to write output file");
            return;
        }

        emit progressChanged(100);
        emit operationComplete(true, "Encryption completed successfully");

    } catch (const std::exception &e) {
        emit operationComplete(false, QString("Encryption error: %1").arg(e.what()));
    }
}

void ImageEncryptor::performDecryption()
{
    try {
        // Read encrypted file
        QByteArray inputData;
        if (!readFile(currentOperation.inputPath, inputData)) {
            emit operationComplete(false, "Failed to read input file");
            return;
        }

        emit progressChanged(10);

        // Parse header
        QDataStream stream(inputData);
        stream.setVersion(QDataStream::Qt_5_15);

        quint8 version, algo, compressed, passwordFlag;
        quint64 dataSize;

        stream >> version;
        stream >> algo;
        stream >> compressed;
        stream >> passwordFlag;
        stream >> dataSize;

        // Validate
        if (version != 0x01) {
            emit operationComplete(false, "Unsupported file format version");
            return;
        }

        // Read encrypted data
        QByteArray encryptedData(dataSize, 0);
        if (stream.readRawData(encryptedData.data(), dataSize) != dataSize) {
            emit operationComplete(false, "Invalid file format");
            return;
        }

        emit progressChanged(30);

        // Get key
        QByteArray key;
        if (passwordFlag) {
            if (currentOperation.password.isEmpty()) {
                emit operationComplete(false, "Password required");
                return;
            }

            // Extract salt (first 16 bytes of encrypted data)
            QByteArray salt = encryptedData.left(16);
            encryptedData = encryptedData.mid(16);
            key = generateKeyFromPassword(currentOperation.password, salt);
        } else {
            if (!readFile(currentOperation.keyPath, key)) {
                emit operationComplete(false, "Failed to read key file");
                return;
            }
        }

        emit progressChanged(50);

        // Decrypt data
        QByteArray decryptedData = decryptData(encryptedData, key);
        if (decryptedData.isEmpty()) {
            emit operationComplete(false, "Decryption failed - wrong key or corrupted data");
            return;
        }

        emit progressChanged(70);

        // Decompress if needed
        QByteArray finalData = decryptedData;
        if (compressed) {
            finalData = decompressData(decryptedData);
            if (finalData.isEmpty()) {
                emit operationComplete(false, "Decompression failed");
                return;
            }
        }

        emit progressChanged(90);

        // Write output file
        if (!writeFile(currentOperation.outputPath, finalData)) {
            emit operationComplete(false, "Failed to write output file");
            return;
        }

        emit progressChanged(100);
        emit operationComplete(true, "Decryption completed successfully");

    } catch (const std::exception &e) {
        emit operationComplete(false, QString("Decryption error: %1").arg(e.what()));
    }
}

QByteArray ImageEncryptor::encryptData(const QByteArray &data, const QByteArray &key)
{
    switch (m_algorithm) {
    case AES_256:
        return aesEncrypt(data, key);
    case ChaCha20:
        return chachaEncrypt(data, key);
    case XOR:
        return xorEncrypt(data, key);
    default:
        return QByteArray();
    }
}

QByteArray ImageEncryptor::decryptData(const QByteArray &data, const QByteArray &key)
{
    switch (m_algorithm) {
    case AES_256:
        return aesDecrypt(data, key);
    case ChaCha20:
        return chachaDecrypt(data, key);
    case XOR:
        return xorEncrypt(data, key); // XOR is symmetric
    default:
        return QByteArray();
    }
}

QByteArray ImageEncryptor::aesEncrypt(const QByteArray &data, const QByteArray &key)
{
    // Simplified AES implementation
    // In production, use OpenSSL or Botan

    QByteArray result = data;
    QByteArray realKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);

    // Simple XOR with key for demonstration
    // Replace with proper AES implementation
    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ realKey[i % realKey.size()];
    }

    return result;
}

QByteArray ImageEncryptor::aesDecrypt(const QByteArray &data, const QByteArray &key)
{
    // AES decryption is same as encryption for this simplified version
    return aesEncrypt(data, key);
}

QByteArray ImageEncryptor::chachaEncrypt(const QByteArray &data, const QByteArray &key)
{
    QByteArray result = data;
    QByteArray realKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);

    // Simple stream cipher simulation
    quint32 counter = 0;
    for (int i = 0; i < result.size(); ++i) {
        if (i % 4 == 0) {
            counter++;
            // Simulate key stream generation
            QByteArray keyStream = QCryptographicHash::hash(
                realKey + QByteArray::number(counter), QCryptographicHash::Md5);
            realKey = keyStream;
        }
        result[i] = result[i] ^ realKey[i % realKey.size()];
    }

    return result;
}

QByteArray ImageEncryptor::chachaDecrypt(const QByteArray &data, const QByteArray &key)
{
    return chachaEncrypt(data, key); // Stream ciphers are symmetric
}

QByteArray ImageEncryptor::xorEncrypt(const QByteArray &data, const QByteArray &key)
{
    QByteArray result = data;
    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ key[i % key.size()];
    }
    return result;
}

QByteArray ImageEncryptor::compressData(const QByteArray &data)
{
    // Using zlib for compression
    ulong sourceLen = data.size();
    ulong destLen = compressBound(sourceLen);
    QByteArray compressed(destLen, 0);

    if (compress((Bytef*)compressed.data(), &destLen,
                 (Bytef*)data.constData(), sourceLen) != Z_OK) {
        return QByteArray();
    }

    compressed.resize(destLen);
    return compressed;
}

QByteArray ImageEncryptor::decompressData(const QByteArray &data)
{
    // Estimate decompressed size (for raw images, assume 2x compression ratio)
    ulong destLen = data.size() * 3;
    QByteArray decompressed(destLen, 0);

    while (true) {
        ulong actualDestLen = destLen;
        int result = uncompress((Bytef*)decompressed.data(), &actualDestLen,
                               (Bytef*)data.constData(), data.size());

        if (result == Z_OK) {
            decompressed.resize(actualDestLen);
            return decompressed;
        } else if (result == Z_BUF_ERROR) {
            // Buffer too small, increase size
            destLen *= 2;
            decompressed.resize(destLen);
        } else {
            // Error
            return QByteArray();
        }
    }
}

bool ImageEncryptor::readFile(const QString &path, QByteArray &data)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    data = file.readAll();
    file.close();
    return !data.isEmpty();
}

bool ImageEncryptor::writeFile(const QString &path, const QByteArray &data)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    qint64 bytesWritten = file.write(data);
    file.close();
    return bytesWritten == data.size();
}

bool ImageEncryptor::generateKeyFile(const QString &path)
{
    QByteArray key(32, 0); // 256-bit key
    for (int i = 0; i < key.size(); ++i) {
        key[i] = QRandomGenerator::global()->generate() % 256;
    }

    return writeFile(path, key);
}

QByteArray ImageEncryptor::generateKeyFromPassword(const QString &password, const QByteArray &salt)
{
    // Use PBKDF2-like key derivation
    QByteArray data = password.toUtf8() + salt;
    for (int i = 0; i < 10000; ++i) {
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    }
    return data;
}

void ImageEncryptor::setAlgorithm(Algorithm algo)
{
    m_algorithm = algo;
}
