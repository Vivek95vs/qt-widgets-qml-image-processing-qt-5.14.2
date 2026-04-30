#ifndef MAIN_H
#define MAIN_H

#include <QQuickImageProvider>
#include <QImage>
#include <QFile>
#include <QDebug>

class RawImageProvider : public QQuickImageProvider {
public:
    RawImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        QString filePath = "D:\\RAD\\proj_1.raw"; // Update with your file path

        int width = 3072;   // Define the correct width
        int height = 3072;  // Define the correct height
        int pixelSize = 2; // 16-bit = 2 bytes per pixel

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file:" << filePath;
            return QImage();
        }

        QByteArray rawData = file.readAll();
        file.close();

        if (rawData.size() != width * height * pixelSize) {
            qWarning() << "File size does not match expected dimensions.";
            return QImage();
        }

        // Handle ROI request
        if (id.startsWith("roi?")) {
            QStringList roiParams = id.mid(4).split(",");
            if (roiParams.size() == 4) {
                int x = roiParams[0].toInt();
                int y = roiParams[1].toInt();
                int roiWidth = roiParams[2].toInt();
                int roiHeight = roiParams[3].toInt();

                // Ensure ROI is within bounds
                x = qMax(0, qMin(x, width - roiWidth));
                y = qMax(0, qMin(y, height - roiHeight));

                // Extract the ROI from the image
                QImage image(width, height, QImage::Format_Grayscale8);
                const quint16 *rawPixels = reinterpret_cast<const quint16 *>(rawData.constData());
                uchar *imageData = image.bits();

                // Normalize 16-bit values to 8-bit (divide by 256)
                for (int i = 0; i < width * height; ++i) {
                    imageData[i] = rawPixels[i] >> 8; // Convert 16-bit to 8-bit
                }

                QImage roiImage = image.copy(x, y, roiWidth, roiHeight);
                return roiImage;
            }
        }

        // Default: Return the full image
        QImage image(width, height, QImage::Format_Grayscale8);
        const quint16 *rawPixels = reinterpret_cast<const quint16 *>(rawData.constData());
        uchar *imageData = image.bits();

        // Normalize 16-bit values to 8-bit (divide by 256)
        for (int i = 0; i < width * height; ++i) {
            imageData[i] = rawPixels[i] >> 8; // Convert 16-bit to 8-bit
        }

        return image;
    }
};

 #endif // MAIN_H
