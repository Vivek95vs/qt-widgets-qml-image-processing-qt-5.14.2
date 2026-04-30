// main.cpp - License Generator Tool
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QDate>
#include <QDir>
#include "LicenseGenerator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);
    QTextStream in(stdin);

    out << "\n";
    out << "╔══════════════════════════════════════════════════════════╗\n";
    out << "║           License Key Generator Tool                     ║\n";
    out << "╚══════════════════════════════════════════════════════════╝\n";
    out.flush();

    QString serialNumber;
    QString dateStr;
    QString filePath;

    // Get PC Serial Number
    out << "\nEnter PC Serial Number: ";
    out.flush();
    serialNumber = in.readLine().trimmed();

    if(serialNumber.isEmpty()) {
        out << "\n✗ Error: Serial number cannot be empty!\n";
        out.flush();
        return 1;
    }

    // Get Expiry Date
    out << "Enter Expiry Date (YYYY-MM-DD): ";
    out.flush();
    dateStr = in.readLine().trimmed();

    QDate expiryDate = QDate::fromString(dateStr, "yyyy-MM-dd");

    if(!expiryDate.isValid()) {
        out << "\n✗ Error: Invalid date format! Use YYYY-MM-DD (e.g., 2025-12-31)\n";
        out.flush();
        return 1;
    }

    // Get file path to save license
    out << "Enter path to save license file (press Enter for current directory): ";
    out.flush();
    filePath = in.readLine().trimmed();

    if(filePath.isEmpty()) {
        filePath = QDir::currentPath() + "/license.dat";
    }

    // Generate License Key
    QString licenseKey = LicenseGenerator::generateLicenseKey(serialNumber, expiryDate);

    // Save to file
    if(LicenseGenerator::saveLicenseToFile(filePath, licenseKey)) {
        int daysRemaining = QDate::currentDate().daysTo(expiryDate);

        out << "\n";
        out << "╔══════════════════════════════════════════════════════════╗\n";
        out << "║           License Generated Successfully!               ║\n";
        out << "╚══════════════════════════════════════════════════════════╝\n";
        out << "\n";
        out << "PC Serial Number: " << serialNumber << "\n";
        out << "Expiry Date:      " << expiryDate.toString("dd/MM/yyyy") << "\n";
        out << "Days Valid:       " << daysRemaining << " days\n";
        out << "\n";
        out << "License Key:\n";
        out << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        out << licenseKey << "\n";
        out << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        out << "\n";
        out << "License file saved to:\n";
        out << filePath << "\n";
        out << "\n";
        out << "Copy this license file to your application's directory!\n";
        out.flush();
    } else {
        out << "\n✗ Error: Failed to save license file!\n";
        out.flush();
        return 1;
    }

    return 0;
}
