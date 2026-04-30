#ifndef IMAGEENCRYPTOR_H
#define IMAGEENCRYPTOR_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QThread>

class ImageEncryptor : public QObject
{
    Q_OBJECT

public:
    enum Algorithm {
        AES_256,
        ChaCha20,
        XOR
    };

    explicit ImageEncryptor(QObject *parent = nullptr);

    // Key-based encryption
    bool encryptWithKeyFile(const QString &inputPath, const QString &outputPath,
                           const QString &keyPath, bool compress = true);
    bool decryptWithKeyFile(const QString &inputPath, const QString &outputPath,
                           const QString &keyPath);

    // Password-based encryption
    bool encryptWithPassword(const QString &inputPath, const QString &outputPath,
                            const QString &password, bool compress = true);
    bool decryptWithPassword(const QString &inputPath, const QString &outputPath,
                            const QString &password);

    // Key management
    bool generateKeyFile(const QString &path);
    QByteArray generateKeyFromPassword(const QString &password, const QByteArray &salt);

    // Settings
    void setAlgorithm(Algorithm algo);
    Algorithm algorithm() const { return m_algorithm; }

signals:
    void progressChanged(int percent);
    void operationComplete(bool success, const QString &message);

private slots:
    void performEncryption();
    void performDecryption();

private:
    // Core encryption functions
    QByteArray encryptData(const QByteArray &data, const QByteArray &key);
    QByteArray decryptData(const QByteArray &data, const QByteArray &key);

    // Compression
    QByteArray compressData(const QByteArray &data);
    QByteArray decompressData(const QByteArray &data);

    // File operations
    bool readFile(const QString &path, QByteArray &data);
    bool writeFile(const QString &path, const QByteArray &data);

    // Crypto functions
    QByteArray aesEncrypt(const QByteArray &data, const QByteArray &key);
    QByteArray aesDecrypt(const QByteArray &data, const QByteArray &key);
    QByteArray chachaEncrypt(const QByteArray &data, const QByteArray &key);
    QByteArray chachaDecrypt(const QByteArray &data, const QByteArray &key);
    QByteArray xorEncrypt(const QByteArray &data, const QByteArray &key);

    // Member variables
    Algorithm m_algorithm;
    QThread *workerThread;
    struct Operation {
        bool isEncrypt;
        QString inputPath;
        QString outputPath;
        QString keyPath;
        QString password;
        bool usePassword;
        bool compress;
    } currentOperation;
};
#endif // IMAGEENCRYPTOR_H
