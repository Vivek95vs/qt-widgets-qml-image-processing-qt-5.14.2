#include "myimageprovider.h"

MyImageProvider::MyImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage MyImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    if (imageMap.contains(id)) {
        QImage image = imageMap[id];

        if (size) *size = image.size();
        if (requestedSize.isValid())
            return image.scaled(requestedSize, Qt::KeepAspectRatio);

        return image;
    }
    return QImage();
}

void MyImageProvider::addRawImage(const QString &imageId, const QString &filePath, int width, int height) {
    QImage img = loadRawImage(filePath, width, height);
    if (!img.isNull()) {
        imageMap[imageId] = img;
    }
}

bool MyImageProvider::hasImage(const QString &id) const {
    return imageMap.contains(id);
}

QImage MyImageProvider::getImage(const QString &id) const {
    return imageMap.value(id, QImage());
}

void MyImageProvider::addImage(const QString &id, const QImage &image) {
    imageMap[id] = image;
}

QImage MyImageProvider::loadRawImage(const QString &filePath, int width, int height) {
//    int pixelSize = 2; // 16-bit grayscale

//    QFile file(filePath);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning() << "Failed to open file:" << filePath;
//        return QImage();
//    }

//    QByteArray rawData = file.readAll();
//    file.close();

//    if (rawData.size() != width * height * pixelSize) {
//        qWarning() << "File size does not match expected dimensions.";
//        return QImage();
//    }

//    QImage image(width, height, QImage::Format_Grayscale8);
//    const quint16 *rawPixels = reinterpret_cast<const quint16 *>(rawData.constData());
//    uchar *imageData = image.bits();

//    for (int i = 0; i < width * height; ++i) {
//        imageData[i] = rawPixels[i] >> 8; // Convert 16-bit to 8-bit
//    }

//    return image;


    const int pixelSize = 2; // 16-bit grayscale (2 bytes per pixel)
    quint16 maxPixelValue;

    // Open the file using QFile
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return QImage();
    }

    // Read the entire file into a QByteArray
    QByteArray rawData = file.readAll();
    file.close();

    // Check if the file size matches the expected dimensions
    if (rawData.size() != width * height * pixelSize) {
        qWarning() << "File size does not match expected dimensions.";
        return QImage();
    }

    // Create a QImage with 8-bit grayscale format
    QImage image(width, height, QImage::Format_Grayscale8);

    // Get pointers to the raw data and image data
    const quint16 *rawPixels = reinterpret_cast<const quint16 *>(rawData.constData());
    uchar *imageData = image.bits();

    // Initialize maxPixelValue to the lowest possible value
    maxPixelValue = 0;

    // Convert 16-bit grayscale to 8-bit grayscale and find the maximum pixel value
    for (int i = 0; i < width * height; ++i) {
        // Update maxPixelValue if a higher value is found
        if (rawPixels[i] > maxPixelValue) {
            maxPixelValue = rawPixels[i];
        }

        // Scale the 16-bit value to 8-bit range
        imageData[i] = static_cast<uchar>((rawPixels[i] * 255) / maxPixelValue);
    }

    return image;

}
