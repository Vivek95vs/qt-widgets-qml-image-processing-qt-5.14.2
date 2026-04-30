#include "rawimageprovider.h"
#include <QFile>
#include <QVector>
#include <QDebug>

RawImageProvider::RawImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage RawImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    const int w = 3072;
    const int h = 2072;

    QFile file("D:/DR.raw");
    if (!file.open(QIODevice::ReadOnly)) return QImage();
    QByteArray rawData = file.readAll();
    file.close();

    QImage img(w, h, QImage::Format_Indexed8);
    QVector<QRgb> table;
    for(int i=0; i<256; i++) table.push_back(qRgb(i,i,i));
    img.setColorTable(table);

    const unsigned short* src = (const unsigned short*)rawData.data();
    int totalPixels = w * h;

    // 1. Find Min and Max values to normalize the X-ray contrast
    unsigned short minVal = 65535;
    unsigned short maxVal = 0;

    for (int i = 0; i < totalPixels; ++i) {
        unsigned short pixel = src[i];
        if (pixel < minVal) minVal = pixel;
        if (pixel > maxVal) maxVal = pixel;
    }

    // 2. Map the X-ray data to 0-255 range
    float range = static_cast<float>(maxVal - minVal);
    if (range <= 0) range = 1.0f;

    for (int y = 0; y < h; y++) {
        uchar* line = img.scanLine(y);
        for (int x = 0; x < w; x++) {
            unsigned short rawValue = src[y * w + x];
            // Normalize: (Value - Min) / (Max - Min) * 255
            line[x] = static_cast<uchar>(((rawValue - minVal) / range) * 255.0f);
        }
    }

    if (size) *size = QSize(w, h);
    return img;
}
