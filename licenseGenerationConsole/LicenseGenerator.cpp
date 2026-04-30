// LicenseGenerator.cpp
#include "LicenseGenerator.h"
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString LicenseGenerator::generateLicenseKey(const QString &serialNumber, const QDate &expiryDate)
{
    // Format: SERIAL|EXPIRY_DATE|CHECKSUM
    QString expiryStr = expiryDate.toString("yyyyMMdd");
    QString data = serialNumber + expiryStr;

    // Generate checksum using SHA256
    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
    QString checksum = hash.toHex().left(16);

    // Create raw license key
    QString rawKey = QString("%1|%2|%3").arg(serialNumber, expiryStr, checksum);

    // Encrypt the key
    return encryptKey(rawKey);
}

QString LicenseGenerator::encryptKey(const QString &key)
{
    // Simple XOR encryption with rotating key
    QByteArray data = key.toUtf8();
    QByteArray encrypted;
    int xorKey = 0x7F; // Fixed seed

    for(int i = 0; i < data.size(); i++) {
        encrypted.append(data[i] ^ xorKey);
        xorKey = (xorKey + i) & 0xFF;
    }

    // Convert to base64 for easy storage
    return encrypted.toBase64();
}

bool LicenseGenerator::saveLicenseToFile(const QString &filePath, const QString &licenseKey)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        return false;
    }

    QTextStream out(&file);
    out << "[LICENSE]\n";
    out << "Key=" << licenseKey << "\n";
    out << "Generated=" << QDate::currentDate().toString("yyyy-MM-dd") << "\n";
    out.flush();
    file.close();  // Close the file, not the QTextStream

    return true;
}
