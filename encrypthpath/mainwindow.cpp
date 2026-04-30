#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Encrypt a string (path) to file
bool encryptPathToFile(const QString &path, const QString &outputFilePath, const QString &hexKey) {
    QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
    if (key.size() != 32) {
        qDebug() << "Key must be 32 bytes (64 hex chars)";
        return false;
    }

    // Convert path to bytes
    QByteArray data = path.toUtf8();

    unsigned char iv[16];
    RAND_bytes(iv, 16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qDebug() << "Failed to create cipher context";
        return false;
    }

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                           (unsigned char*)key.constData(), iv)) {
        qDebug() << "Encrypt init failed";
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int len, cipherLen = data.size() + 16;
    QByteArray encrypted(cipherLen, 0);

    if (!EVP_EncryptUpdate(ctx, (unsigned char*)encrypted.data(), &len,
                          (unsigned char*)data.constData(), data.size())) {
        qDebug() << "Encrypt update failed";
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    cipherLen = len;

    if (!EVP_EncryptFinal_ex(ctx, (unsigned char*)encrypted.data() + len, &len)) {
        qDebug() << "Encrypt final failed";
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    cipherLen += len;
    encrypted.resize(cipherLen);

    EVP_CIPHER_CTX_free(ctx);

    // Write to file
    QFile output(outputFilePath);
    if (!output.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open output file";
        return false;
    }
    output.write((char*)iv, 16);
    output.write(encrypted);
    output.close();

    qDebug() << "Path encrypted to file:" << outputFilePath;
    return true;
}

// Decrypt path from file
QString decryptPathFromFile(const QString &inputFilePath, const QString &hexKey) {
    QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
    if (key.size() != 32) {
        qDebug() << "Key must be 32 bytes (64 hex chars)";
        return QString();
    }

    QFile input(inputFilePath);
    if (!input.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open input file";
        return QString();
    }

    char iv[16];
    if (input.read(iv, 16) != 16) {
        qDebug() << "Failed to read IV";
        input.close();
        return QString();
    }

    QByteArray encrypted = input.readAll();
    input.close();

    if (encrypted.isEmpty()) {
        qDebug() << "No encrypted data";
        return QString();
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qDebug() << "Failed to create cipher context";
        return QString();
    }

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                           (unsigned char*)key.constData(), (unsigned char*)iv)) {
        qDebug() << "Decrypt init failed";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    int len, plainLen = encrypted.size();
    QByteArray decrypted(plainLen + 16, 0);

    if (!EVP_DecryptUpdate(ctx, (unsigned char*)decrypted.data(), &len,
                          (unsigned char*)encrypted.constData(), encrypted.size())) {
        qDebug() << "Decrypt update failed";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }
    plainLen = len;

    if (!EVP_DecryptFinal_ex(ctx, (unsigned char*)decrypted.data() + len, &len)) {
        qDebug() << "Decrypt final failed";
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }
    plainLen += len;
    decrypted.resize(plainLen);

    EVP_CIPHER_CTX_free(ctx);

    // Convert back to string (path)
    QString path = QString::fromUtf8(decrypted);
    qDebug() << "Decrypted path:" << path;
    return path;
}

// Encrypt path to string (Base64 encoded)
QString encryptPathToString(const QString &path, const QString &hexKey) {
    QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
    if (key.size() != 32) {
        qDebug() << "Key must be 32 bytes (64 hex chars)";
        return QString();
    }

    QByteArray data = path.toUtf8();

    unsigned char iv[16];
    RAND_bytes(iv, 16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QString();

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                      (unsigned char*)key.constData(), iv);

    int len, cipherLen = data.size() + 16;
    QByteArray encrypted(cipherLen, 0);

    EVP_EncryptUpdate(ctx, (unsigned char*)encrypted.data(), &len,
                     (unsigned char*)data.constData(), data.size());
    cipherLen = len;

    EVP_EncryptFinal_ex(ctx, (unsigned char*)encrypted.data() + len, &len);
    cipherLen += len;
    encrypted.resize(cipherLen);

    EVP_CIPHER_CTX_free(ctx);

    // Combine IV and encrypted data, then Base64 encode
    QByteArray combined;
    combined.append((char*)iv, 16);
    combined.append(encrypted);

    return combined.toBase64();
}

// Decrypt path from string (Base64 encoded)
QString decryptPathFromString(const QString &encryptedString, const QString &hexKey) {
    QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
    if (key.size() != 32) {
        qDebug() << "Key must be 32 bytes (64 hex chars)";
        return QString();
    }

    // Decode Base64
    QByteArray combined = QByteArray::fromBase64(encryptedString.toLatin1());
    if (combined.size() < 16) {
        qDebug() << "Invalid encrypted string";
        return QString();
    }

    // Extract IV and encrypted data
    QByteArray iv = combined.left(16);
    QByteArray encrypted = combined.mid(16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return QString();

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                      (unsigned char*)key.constData(), (unsigned char*)iv.constData());

    int len, plainLen = encrypted.size();
    QByteArray decrypted(plainLen + 16, 0);

    EVP_DecryptUpdate(ctx, (unsigned char*)decrypted.data(), &len,
                     (unsigned char*)encrypted.constData(), encrypted.size());
    plainLen = len;

    EVP_DecryptFinal_ex(ctx, (unsigned char*)decrypted.data() + len, &len);
    plainLen += len;
    decrypted.resize(plainLen);

    EVP_CIPHER_CTX_free(ctx);

    return QString::fromUtf8(decrypted);
}


void MainWindow::on_Encrypthpath_clicked()
{

    QString key = "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff";

    QString originalPath = "D:\\Encrypt path\\Dicom";

        // Option 1: Encrypt path to file
    encryptPathToFile(originalPath, "D:\\Encrypt path\\encrypted_path.bin", key);


}


void MainWindow::on_decryptpath_clicked()
{
    QString key = "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff";
    QString decryptedPath = decryptPathFromFile("D:\\Encrypt path\\encrypted_path.bin", key);
//        if (decryptedPath == originalPath) {
//            qDebug() << "Path decrypted successfully!";
//        }

        // Option 2: Encrypt path to string (for storing in config, database, etc.)
        QString encryptedString = encryptPathToString("D:\\Encrypt path", key);
        qDebug() << "Encrypted string (Base64):" << encryptedString;

        QString decryptedFromString = decryptPathFromString(encryptedString, key);
//        if (decryptedFromString == originalPath) {
//            qDebug() << "String decrypted successfully!";
//        }

}
