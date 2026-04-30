// LicenseValidator.cpp
#include "LicenseValidator.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

LicenseValidator::LicenseValidator(QObject *parent)
    : QObject(parent)
    , m_isValid(false)
    , m_daysRemaining(0)
{
    loadAndValidateLicense();
}

QString LicenseValidator::getApplicationPath()
{
    return QCoreApplication::applicationDirPath();
}

bool LicenseValidator::loadLicenseFromFile()
{
    QString licensePath = getApplicationPath() + "/license.dat";
    QFile file(licensePath);

    if (!file.exists()) {
        m_message = "License file not found at: " + licensePath;
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_message = "Cannot open license file";
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Parse license file
    QStringList lines = content.split("\n", Qt::SkipEmptyParts);
    for(QString line : lines) {
        line = line.trimmed();
        if(line.startsWith("Key=")) {
            m_licenseKey = line.mid(4);
            return true;
        }
    }

    m_message = "Invalid license file format";
    return false;
}

bool LicenseValidator::loadAndValidateLicense()
{
    // Load license from file
    if(!loadLicenseFromFile()) {
        m_isValid = false;
        emit licenseChanged();
        return false;
    }

    // Check if serial number is saved
    QSettings settings("MyCompany", "MyApp");
    m_serialNumber = settings.value("SerialNumber").toString();

    if(m_serialNumber.isEmpty()) {
        m_isValid = false;
        m_message = "Please enter your PC serial number";
        emit licenseChanged();
        return false;
    }

    // Validate license with serial number
    return validateLicense(m_licenseKey, m_serialNumber);
}

bool LicenseValidator::validateLicense(const QString &licenseKey, const QString &serialNumber)
{
    m_serialNumber = serialNumber;

    // Decrypt license key
    QString decryptedKey = decryptKey(licenseKey);

    // Parse license: SERIAL|EXPIRY_DATE|CHECKSUM
    QStringList parts = decryptedKey.split("|");

    if(parts.size() != 3) {
        m_message = "Invalid license key format";
        m_isValid = false;
        emit licenseChanged();
        return false;
    }

    QString storedSerial = parts[0];
    QDate expiryDate = QDate::fromString(parts[1], "yyyyMMdd");
    QString providedChecksum = parts[2];

    // Verify serial number
    if(storedSerial != m_serialNumber) {
        m_message = "License key does not match this PC serial number";
        m_isValid = false;
        emit licenseChanged();
        return false;
    }

    // Verify checksum
    QString data = storedSerial + parts[1];
    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
    QString expectedChecksum = hash.toHex().left(16);

    if(providedChecksum != expectedChecksum) {
        m_message = "Invalid license key (corrupted)";
        m_isValid = false;
        emit licenseChanged();
        return false;
    }

    // Check expiry
    QDate today = QDate::currentDate();
    if(today > expiryDate) {
        m_daysRemaining = 0;
        m_message = QString("License expired on %1").arg(expiryDate.toString("dd/MM/yyyy"));
        m_isValid = false;
        emit licenseChanged();
        return false;
    }

    // Calculate days remaining
    m_daysRemaining = today.daysTo(expiryDate);
    m_expiryDate = expiryDate;
    m_message = QString("License valid until %1 (%2 days left)").arg(
                    expiryDate.toString("dd/MM/yyyy")).arg(m_daysRemaining);
    m_isValid = true;

    // Save serial number
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("SerialNumber", m_serialNumber);

    emit licenseChanged();
    return true;
}

void LicenseValidator::setSerialNumber(const QString &serial)
{
    if(m_serialNumber != serial) {
        m_serialNumber = serial;
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("SerialNumber", serial);

        // Re-validate with new serial number
        if(!m_licenseKey.isEmpty()) {
            validateLicense(m_licenseKey, serial);
        }
        emit licenseChanged();
    }
}

QString LicenseValidator::decryptKey(const QString &encryptedKey)
{
    QByteArray encrypted = QByteArray::fromBase64(encryptedKey.toUtf8());
    QByteArray decrypted;
    int xorKey = 0x7F;

    for(int i = 0; i < encrypted.size(); i++) {
        decrypted.append(encrypted[i] ^ xorKey);
        xorKey = (xorKey + i) & 0xFF;
    }

    return QString::fromUtf8(decrypted);
}
