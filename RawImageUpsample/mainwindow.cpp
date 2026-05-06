#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QVector>
#include <QtEndian>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::upsample16BitRawTo3072(const QString& inputPath, const QString& outputPath) {

    // Step 1: Load 16-bit RAW data (1879x2401)
    QFile file(inputPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open input file:" << inputPath;
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    int originalWidth = 2401;
    int originalHeight = 1879;
    int expectedSize = originalWidth * originalHeight * 2; // 16-bit = 2 bytes per pixel

    if (rawData.size() < expectedSize) {
        qCritical() << "Raw data size mismatch. Expected:" << expectedSize
                    << "bytes, Got:" << rawData.size() << "bytes";
        return false;
    }

    // Step 2: Convert 16-bit data to QImage (as 16-bit grayscale)
    // QImage doesn't natively support 16-bit grayscale, so we'll use QImage::Format_Grayscale16

    QImage inputImage(originalWidth, originalHeight, QImage::Format_Grayscale16);

    // Copy 16-bit data to QImage (assuming little-endian)
    const quint16* srcData = reinterpret_cast<const quint16*>(rawData.constData());
    quint16* dstData = reinterpret_cast<quint16*>(inputImage.bits());

    for (int i = 0; i < originalWidth * originalHeight; ++i) {
        dstData[i] = srcData[i]; // Copy as-is (adjust endianness if needed)
    }

    // Step 3: Upsample to 3072x3072
    // For 16-bit, we need to use a workaround since Qt's scaling preserves format
    QImage upsampledImage = inputImage.scaled(3072, 3072,
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation);

    // Ensure format is preserved (should be Grayscale16)
    if (upsampledImage.format() != QImage::Format_Grayscale16) {
        upsampledImage = upsampledImage.convertToFormat(QImage::Format_Grayscale16);
    }

    // Step 4: Save as 16-bit RAW file
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Failed to create output file:" << outputPath;
        return false;
    }

    // Extract 16-bit pixel data
    const quint16* outputData = reinterpret_cast<const quint16*>(upsampledImage.constBits());
    int pixelCount = 3072 * 3072;
    int byteCount = pixelCount * 2;

    // Write raw 16-bit data
    qint64 bytesWritten = outputFile.write(reinterpret_cast<const char*>(outputData), byteCount);
    outputFile.close();

    if (bytesWritten != byteCount) {
        qCritical() << "Failed to write all data. Written:" << bytesWritten
                    << "Expected:" << byteCount;
        return false;
    }

    qDebug() << "Success! Upsampled from" << originalWidth << "x" << originalHeight
             << "to 3072x3072 (16-bit)";
    qDebug() << "Output RAW file size:" << byteCount << "bytes";
    qDebug() << "Saved to:" << outputPath;

    return true;
}

// Alternative: Manual bilinear interpolation for better control
bool MainWindow::upsample16BitRawManual(const QString& inputPath, const QString& outputPath) {

    // Load 16-bit RAW data
    QFile file(inputPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open input file";
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    int srcWidth = 2401;
    int srcHeight = 1879;
    int dstWidth = 3072;
    int dstHeight = 3072;

    // Convert raw data to vector of 16-bit values
    QVector<quint16> srcPixels(srcWidth * srcHeight);
    memcpy(srcPixels.data(), rawData.constData(), srcWidth * srcHeight * 2);

    // Create output buffer
    QVector<quint16> dstPixels(dstWidth * dstHeight);

    // Calculate scaling factors
    double scaleX = (double)srcWidth / dstWidth;
    double scaleY = (double)srcHeight / dstHeight;

    // Bilinear interpolation
    for (int y = 0; y < dstHeight; ++y) {
        double srcY = y * scaleY;
        int y1 = qBound(0, (int)srcY, srcHeight - 1);
        int y2 = qBound(0, y1 + 1, srcHeight - 1);
        double fy = srcY - y1;

        for (int x = 0; x < dstWidth; ++x) {
            double srcX = x * scaleX;
            int x1 = qBound(0, (int)srcX, srcWidth - 1);
            int x2 = qBound(0, x1 + 1, srcWidth - 1);
            double fx = srcX - x1;

            // Get four surrounding pixels
            quint16 p11 = srcPixels[y1 * srcWidth + x1];
            quint16 p12 = srcPixels[y2 * srcWidth + x1];
            quint16 p21 = srcPixels[y1 * srcWidth + x2];
            quint16 p22 = srcPixels[y2 * srcWidth + x2];

            // Bilinear interpolation
            double interpolated = (p11 * (1 - fx) * (1 - fy)) +
                                  (p21 * fx * (1 - fy)) +
                                  (p12 * (1 - fx) * fy) +
                                  (p22 * fx * fy);

            dstPixels[y * dstWidth + x] = qBound(0, (int)interpolated, 65535);
        }
    }

    // Save as 16-bit RAW
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Failed to create output file";
        return false;
    }

    qint64 bytesWritten = outputFile.write(reinterpret_cast<const char*>(dstPixels.constData()),
                                           dstPixels.size() * 2);
    outputFile.close();

    if (bytesWritten != dstPixels.size() * 2) {
        qCritical() << "Failed to write all data";
        return false;
    }

    qDebug() << "Manual upsampling completed!";
    return true;
}

// Handle different endianness (for big-endian RAW files)
bool MainWindow::upsample16BitRawWithEndianness(const QString& inputPath,
                                     const QString& outputPath,
                                     bool isBigEndian = false) {

    QFile file(inputPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open input file";
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    int srcWidth = 2401;
    int srcHeight = 1879;

    // Convert to proper endianness if needed
    QVector<quint16> srcPixels(srcWidth * srcHeight);

    if (isBigEndian) {
        // Convert from big-endian to little-endian
        const quint16* data16 = reinterpret_cast<const quint16*>(rawData.constData());
        for (int i = 0; i < srcWidth * srcHeight; ++i) {
            srcPixels[i] = qFromBigEndian(data16[i]);
        }
    } else {
        memcpy(srcPixels.data(), rawData.constData(), srcWidth * srcHeight * 2);
    }

    // Create QImage from 16-bit data
    QImage inputImage(srcWidth, srcHeight, QImage::Format_Grayscale16);
    memcpy(inputImage.bits(), srcPixels.constData(), srcWidth * srcHeight * 2);

    // Upsample
    QImage upsampledImage = inputImage.scaled(3072, 3072,
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation);

    if (upsampledImage.format() != QImage::Format_Grayscale16) {
        upsampledImage = upsampledImage.convertToFormat(QImage::Format_Grayscale16);
    }

    // Save as 16-bit RAW (little-endian)
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Failed to create output file";
        return false;
    }

    outputFile.write(reinterpret_cast<const char*>(upsampledImage.constBits()),
                     upsampledImage.sizeInBytes());
    outputFile.close();

    qDebug() << "16-bit RAW with endianness handling completed!";
    return true;
}


void MainWindow::on_pushButton_clicked()
{
    QString inputFile = "D:\\DR\\DR\\proj_1.raw";    // Your 1879x2401 16-bit RAW file
        QString outputFile = "D:\\DR\\DR\\output_1.raw";   // Upsampled 3072x3072 16-bit RAW file

        // Method 1: Using QImage (recommended)
//        if (upsample16BitRawTo3072(inputFile, outputFile)) {
//            qDebug() << "Successfully upsampled 16-bit RAW image!";
//        } else {
//            qCritical() << "Failed to process image!";
////            return 1;
//        }

        if (upsample16BitRawManual(inputFile, outputFile)) {
            qDebug() << "Successfully upsampled 16-bit RAW image!";
        } else {
            qCritical() << "Failed to process image!";
//            return 1;
        }


}
