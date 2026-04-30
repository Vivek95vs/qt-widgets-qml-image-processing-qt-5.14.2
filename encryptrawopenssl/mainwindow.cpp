#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDataStream>
#include <QMessageBox>
#include <QElapsedTimer>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <QProgressDialog>

// Worker thread implementation
void FolderWorker::run() {
    try {
        QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
        if (key.size() != 32) {
            emit error("Invalid key size (must be 64 hex characters)");
            return;
        }

        if (operation == Encrypt) {
            emit status("Calculating total size...");

            // Calculate total size for progress
            qint64 totalSize = 0;
            QDirIterator it(sourcePath, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                QFileInfo info = it.fileInfo();
                if (info.isFile()) {
                    totalSize += info.size();
                }
            }

            emit status(QString("Total size: %1 bytes").arg(totalSize));

            if (compressAndEncryptFolder(sourcePath, destPath, key)) {
                emit finished(true, "Encryption completed successfully");
            } else {
                emit finished(false, "Encryption failed");
            }
        } else {
            if (!QFile::exists(sourcePath)) {
                emit error("Encrypted file does not exist");
                return;
            }

            if (decryptAndExtractFolder(sourcePath, destPath, key)) {
                emit finished(true, "Decryption completed successfully");
            } else {
                emit finished(false, "Decryption failed");
            }
        }
    } catch (const std::exception &e) {
        emit error(QString("Exception: %1").arg(e.what()));
    } catch (...) {
        emit error("Unknown error occurred");
    }
}

bool FolderWorker::compressAndEncryptFolder(const QString &folderPath,
                                           const QString &outputFile,
                                           const QByteArray &key) {
    emit status("Collecting files...");

    // Get all files recursively
    QStringList allFiles;
    QDirIterator it(folderPath, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        if (info.isFile()) {
            QString relativePath = QDir(folderPath).relativeFilePath(info.filePath());
            allFiles.append(relativePath);
        }

        // Check if thread should stop
        if (isInterruptionRequested()) {
            return false;
        }
    }

    emit status(QString("Found %1 files").arg(allFiles.size()));

    // Create output file
    QFile outFile(outputFile);
    if (!outFile.open(QIODevice::WriteOnly)) {
        emit error("Cannot open output file for writing");
        return false;
    }

    QDataStream stream(&outFile);
//    stream.setVersion(QDataStream::Qt_5_15);

    // Write header
    stream << QString("ENCRYPTED_FOLDER_V2");
    stream << QFileInfo(folderPath).fileName();
    stream << allFiles.size();

    // Generate IV for file list
    unsigned char fileListIv[16];
    RAND_bytes(fileListIv, 16);

    // First, write all file metadata (paths and sizes)
    emit status("Writing file metadata...");
    QByteArray metadata;
    QDataStream metaStream(&metadata, QIODevice::WriteOnly);
//    metaStream.setVersion(QDataStream::Qt_5_15);

    for (const QString &relativePath : allFiles) {
        metaStream << relativePath;

        // Check if thread should stop
        if (isInterruptionRequested()) {
            outFile.close();
            outFile.remove();
            return false;
        }
    }

    // Encrypt metadata
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                      (unsigned char*)key.constData(), fileListIv);

    int len, cipherLen = metadata.size() + 16;
    QByteArray encryptedMetadata(cipherLen, 0);

    EVP_EncryptUpdate(ctx, (unsigned char*)encryptedMetadata.data(), &len,
                     (unsigned char*)metadata.constData(), metadata.size());
    cipherLen = len;

    EVP_EncryptFinal_ex(ctx, (unsigned char*)encryptedMetadata.data() + len, &len);
    cipherLen += len;
    encryptedMetadata.resize(cipherLen);

    EVP_CIPHER_CTX_free(ctx);

    // Write metadata to output
    outFile.write((char*)fileListIv, 16);
    stream << encryptedMetadata;

    // Now process each file
    qint64 processedBytes = 0;
    qint64 fileIndex = 0;

    for (const QString &relativePath : allFiles) {
        emit status(QString("Processing file %1/%2: %3")
                   .arg(++fileIndex).arg(allFiles.size()).arg(relativePath));

        QString absolutePath = QDir(folderPath).absoluteFilePath(relativePath);
        QFile file(absolutePath);

        if (!file.open(QIODevice::ReadOnly)) {
            emit error(QString("Cannot open file: %1").arg(absolutePath));
            outFile.close();
            outFile.remove();
            return false;
        }

        // Read file in chunks to avoid memory issues
        const qint64 CHUNK_SIZE = 1024 * 1024; // 1MB chunks
        qint64 fileSize = file.size();

        // Generate IV for this file
        unsigned char fileIv[16];
        RAND_bytes(fileIv, 16);

        // Write file header
        stream << relativePath;
        stream << fileSize;
        outFile.write((char*)fileIv, 16);

        // Encrypt file in chunks
        EVP_CIPHER_CTX *fileCtx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(fileCtx, EVP_aes_256_cbc(), NULL,
                          (unsigned char*)key.constData(), fileIv);

        QByteArray buffer(CHUNK_SIZE, 0);
        qint64 bytesRead = 0;

        while (bytesRead < fileSize) {
            if (isInterruptionRequested()) {
                EVP_CIPHER_CTX_free(fileCtx);
                file.close();
                outFile.close();
                outFile.remove();
                return false;
            }

            qint64 toRead = qMin(CHUNK_SIZE, fileSize - bytesRead);
            qint64 read = file.read(buffer.data(), toRead);

            if (read <= 0) break;

            int encryptedLen = read + 16;
            QByteArray encrypted(encryptedLen, 0);

            EVP_EncryptUpdate(fileCtx, (unsigned char*)encrypted.data(), &len,
                             (unsigned char*)buffer.constData(), read);
            encryptedLen = len;

            EVP_EncryptFinal_ex(fileCtx, (unsigned char*)encrypted.data() + len, &len);
            encryptedLen += len;
            encrypted.resize(encryptedLen);

            // Write encrypted chunk
            stream << encrypted;

            bytesRead += read;
            processedBytes += read;

            // Update progress
            int percent = qMin(99, static_cast<int>((processedBytes * 100) /
                          (allFiles.size() * CHUNK_SIZE + fileSize)));
            emit progress(percent);
        }

        EVP_CIPHER_CTX_free(fileCtx);
        file.close();
    }

    outFile.close();
    emit progress(100);
    return true;
}

bool FolderWorker::decryptAndExtractFolder(const QString &encryptedFile,
                                          const QString &outputFolder,
                                          const QByteArray &key) {
    emit status("Opening encrypted file...");

    QFile inFile(encryptedFile);
    if (!inFile.open(QIODevice::ReadOnly)) {
        emit error("Cannot open encrypted file");
        return false;
    }

    QDataStream stream(&inFile);
//    stream.setVersion(QDataStream::Qt_5_15);

    // Read and verify header
    QString header;
    QString rootFolderName;
    int fileCount;

    stream >> header;
    if (header != "ENCRYPTED_FOLDER_V2") {
        emit error("Invalid file format");
        inFile.close();
        return false;
    }

    stream >> rootFolderName;
    stream >> fileCount;

    emit status(QString("Found %1 files").arg(fileCount));

    // Read metadata IV
    char fileListIv[16];
    inFile.read(fileListIv, 16);

    // Read encrypted metadata
    QByteArray encryptedMetadata;
    stream >> encryptedMetadata;

    // Decrypt metadata
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                      (unsigned char*)key.constData(), (unsigned char*)fileListIv);

    int len, plainLen = encryptedMetadata.size();
    QByteArray metadata(plainLen + 16, 0);

    EVP_DecryptUpdate(ctx, (unsigned char*)metadata.data(), &len,
                     (unsigned char*)encryptedMetadata.constData(), encryptedMetadata.size());
    plainLen = len;

    EVP_DecryptFinal_ex(ctx, (unsigned char*)metadata.data() + len, &len);
    plainLen += len;
    metadata.resize(plainLen);

    EVP_CIPHER_CTX_free(ctx);

    // Parse metadata
    QDataStream metaStream(metadata);
    QStringList filePaths;

    for (int i = 0; i < fileCount; i++) {
        QString relativePath;
        metaStream >> relativePath;
        filePaths.append(relativePath);

        if (isInterruptionRequested()) {
            inFile.close();
            return false;
        }
    }

    // Create output directory
    QString fullOutputPath = outputFolder + "/" + rootFolderName;
    QDir().mkpath(fullOutputPath);

    emit status("Extracting files...");

    // Process each file
    for (int i = 0; i < fileCount; i++) {
        emit status(QString("Extracting file %1/%2: %3")
                   .arg(i + 1).arg(fileCount).arg(filePaths[i]));

        QString relativePath;
        qint64 fileSize;
        char fileIv[16];

        stream >> relativePath;
        stream >> fileSize;
        inFile.read(fileIv, 16);

        // Create directory if needed
        QString filePath = fullOutputPath + "/" + relativePath;
        QFileInfo fileInfo(filePath);
        QDir().mkpath(fileInfo.absolutePath());

        // Create output file
        QFile outFile(filePath);
        if (!outFile.open(QIODevice::WriteOnly)) {
            emit error(QString("Cannot create file: %1").arg(filePath));
            inFile.close();
            return false;
        }

        // Decrypt file in chunks
        EVP_CIPHER_CTX *fileCtx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(fileCtx, EVP_aes_256_cbc(), NULL,
                          (unsigned char*)key.constData(), (unsigned char*)fileIv);

        qint64 bytesWritten = 0;

        while (bytesWritten < fileSize) {
            if (isInterruptionRequested()) {
                EVP_CIPHER_CTX_free(fileCtx);
                outFile.close();
                inFile.close();
                return false;
            }

            QByteArray encryptedChunk;
            stream >> encryptedChunk;

            int chunkLen = encryptedChunk.size();
            QByteArray decrypted(chunkLen + 16, 0);

            EVP_DecryptUpdate(fileCtx, (unsigned char*)decrypted.data(), &len,
                             (unsigned char*)encryptedChunk.constData(), chunkLen);
            chunkLen = len;

            int finalLen;
            EVP_DecryptFinal_ex(fileCtx, (unsigned char*)decrypted.data() + len, &finalLen);
            chunkLen += finalLen;
            decrypted.resize(chunkLen);

            // Write decrypted chunk
            qint64 toWrite = qMin(static_cast<qint64>(chunkLen), fileSize - bytesWritten);
            outFile.write(decrypted.constData(), toWrite);
            bytesWritten += toWrite;

            // Update progress
            int percent = qMin(99, static_cast<int>((bytesWritten * 100) / fileSize));
            emit progress(percent);
        }

        EVP_CIPHER_CTX_free(fileCtx);
        outFile.close();
    }

    inFile.close();
    emit progress(100);
    return true;
}

// MainWindow implementation
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    worker(nullptr)
{
    ui->setupUi(this);

    // Initialize UI
//    ui->progressBar->setValue(0);
//    ui->statusLabel->setText("Ready");
}

MainWindow::~MainWindow()
{
    if (worker && worker->isRunning()) {
        worker->requestInterruption();
        worker->wait(5000);
    }
    delete worker;
    delete ui;
}

void MainWindow::onEncryptionProgress(int percent)
{
//    ui->progressBar->setValue(percent);
}

void MainWindow::onEncryptionStatus(const QString &message)
{
//    ui->statusLabel->setText(message);
    qDebug() << "Status:" << message;
}

void MainWindow::onEncryptionFinished(bool success, const QString &message)
{
//    ui->progressBar->setValue(100);
//    ui->statusLabel->setText(message);

    if (success) {
        QMessageBox::information(this, "Success", message);
    } else {
        QMessageBox::warning(this, "Error", message);
    }

    // Re-enable buttons
    ui->raw_encrypt->setEnabled(true);
    ui->raw_decrypt->setEnabled(true);

    worker->deleteLater();
    worker = nullptr;
}

void MainWindow::onEncryptionError(const QString &error)
{
//    ui->statusLabel->setText("Error: " + error);
    QMessageBox::critical(this, "Error", error);

    // Re-enable buttons
    ui->raw_encrypt->setEnabled(true);
    ui->raw_decrypt->setEnabled(true);

    if (worker) {
        worker->deleteLater();
        worker = nullptr;
    }
}

void MainWindow::on_raw_encrypt_clicked()
{
    QString key = "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff";
    QString dicomFolder = "D:/dummymotion/kv1";
//    QString dicomFolder = "D:/Encrypt path/Dicom";
    QString encryptedFile = "D:/Encrypt path/dicom_encrypted.bin";

    // Validate paths
    if (!QDir(dicomFolder).exists()) {
        QMessageBox::warning(this, "Error", "Source folder does not exist!");
        return;
    }

    // Disable buttons during operation
    ui->raw_encrypt->setEnabled(false);
    ui->raw_decrypt->setEnabled(false);
//    ui->progressBar->setValue(0);
//    ui->statusLabel->setText("Starting encryption...");

    // Start worker thread
    worker = new FolderWorker(FolderWorker::Encrypt, dicomFolder, encryptedFile, key, this);
    connect(worker, &FolderWorker::progress, this, &MainWindow::onEncryptionProgress);
    connect(worker, &FolderWorker::status, this, &MainWindow::onEncryptionStatus);
    connect(worker, &FolderWorker::finished, this, &MainWindow::onEncryptionFinished);
    connect(worker, &FolderWorker::error, this, &MainWindow::onEncryptionError);

    worker->start();
}

void MainWindow::on_raw_decrypt_clicked()
{
    QString key = "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff";
    QString encryptedFile = "D:/Encrypt path/dicom_encrypted.bin";
    QString decryptedFolder = "D:/Encrypt path/Decrypted";

    // Validate paths
    if (!QFile::exists(encryptedFile)) {
        QMessageBox::warning(this, "Error", "Encrypted file does not exist!");
        return;
    }

    // Disable buttons during operation
    ui->raw_encrypt->setEnabled(false);
    ui->raw_decrypt->setEnabled(false);
//    ui->progressBar->setValue(0);
//    ui->statusLabel->setText("Starting decryption...");

    // Start worker thread
    worker = new FolderWorker(FolderWorker::Decrypt, encryptedFile, decryptedFolder, key, this);
    connect(worker, &FolderWorker::progress, this, &MainWindow::onEncryptionProgress);
    connect(worker, &FolderWorker::status, this, &MainWindow::onEncryptionStatus);
    connect(worker, &FolderWorker::finished, this, &MainWindow::onEncryptionFinished);
    connect(worker, &FolderWorker::error, this, &MainWindow::onEncryptionError);

    worker->start();
}

// Helper method to calculate total size (for information)
qint64 MainWindow::calculateTotalSize(const QString &folderPath)
{
    qint64 totalSize = 0;
    QDirIterator it(folderPath, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        if (info.isFile()) {
            totalSize += info.size();
        }
    }

    return totalSize;
}
