#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QThread>
#include <QDataStream>

// Worker thread for encryption/decryption
class FolderWorker : public QThread {
    Q_OBJECT
public:
    enum Operation { Encrypt, Decrypt };

    FolderWorker(Operation op, const QString &source, const QString &dest,
                 const QString &key, QObject *parent = nullptr)
        : QThread(parent), operation(op), sourcePath(source),
          destPath(dest), hexKey(key) {}

signals:
    void progress(int percent);
    void status(const QString &message);
    void finished(bool success, const QString &message);
    void error(const QString &error);

protected:
    void run() override;

private:
    Operation operation;
    QString sourcePath;
    QString destPath;
    QString hexKey;

    bool compressAndEncryptFolder(const QString &folderPath, const QString &outputFile,
                                 const QByteArray &key);
    bool decryptAndExtractFolder(const QString &encryptedFile, const QString &outputFolder,
                                const QByteArray &key);
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_raw_encrypt_clicked();
    void on_raw_decrypt_clicked();

    void onEncryptionProgress(int percent);
    void onEncryptionStatus(const QString &message);
    void onEncryptionFinished(bool success, const QString &message);
    void onEncryptionError(const QString &error);

private:
    Ui::MainWindow *ui;
    FolderWorker *worker;

    static const int IV_SIZE = 16;
    static const int KEY_SIZE = 32;

    // Helper methods
    bool encryptFolderStreaming(const QString &folderPath, const QString &outputFile,
                               const QByteArray &key);
    bool decryptFolderStreaming(const QString &encryptedFile, const QString &outputFolder,
                               const QByteArray &key);
    qint64 calculateTotalSize(const QString &folderPath);
};

#endif // MAINWINDOW_H
