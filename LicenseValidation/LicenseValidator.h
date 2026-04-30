// LicenseValidator.h
#ifndef LICENSEVALIDATOR_H
#define LICENSEVALIDATOR_H

#include <QObject>
#include <QDate>

class LicenseValidator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid NOTIFY licenseChanged)
    Q_PROPERTY(int daysRemaining READ daysRemaining NOTIFY licenseChanged)
    Q_PROPERTY(QString message READ message NOTIFY licenseChanged)
    Q_PROPERTY(QString serialNumber READ serialNumber NOTIFY licenseChanged)

public:
    explicit LicenseValidator(QObject *parent = nullptr);

    bool isValid() const { return m_isValid; }
    int daysRemaining() const { return m_daysRemaining; }
    QString message() const { return m_message; }
    QString serialNumber() const { return m_serialNumber; }

    Q_INVOKABLE bool loadAndValidateLicense();
    Q_INVOKABLE bool validateLicense(const QString &licenseKey, const QString &serialNumber);
    Q_INVOKABLE void setSerialNumber(const QString &serial);

signals:
    void licenseChanged();

private:
    bool loadLicenseFromFile();
    bool validateKey(const QString &serialNumber, const QString &licenseKey);
    QString decryptKey(const QString &encryptedKey);
    QString getApplicationPath();

private:
    bool m_isValid;
    int m_daysRemaining;
    QString m_message;
    QString m_serialNumber;
    QString m_licenseKey;
    QDate m_expiryDate;
};

#endif
