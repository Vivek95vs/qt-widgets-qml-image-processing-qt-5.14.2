// LicenseGenerator.h
#ifndef LICENSEGENERATOR_H
#define LICENSEGENERATOR_H

#include <QString>
#include <QDate>

class LicenseGenerator
{
public:
    static QString generateLicenseKey(const QString &serialNumber, const QDate &expiryDate);
    static bool saveLicenseToFile(const QString &filePath, const QString &licenseKey);
    static QString encryptKey(const QString &key);
};

#endif
