#include "rawimageload.h"
#include "QDebug"

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image), m_imageLoaded(false) {
}

QImage RawImageLoad::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);

    if (id == "main") {
        return myImage;
    }
    return QImage();
}

void RawImageLoad::loadImage() {
    myImage = loadRawImage();
    if (!myImage.isNull()) {
        m_imageLoaded = true;
        calculateHistogram(); // Calculate histogram after loading
        emit imageLoadedChanged();
        emit histogramUpdated();
    }
}

bool RawImageLoad::imageLoaded() const {
    return m_imageLoaded;
}

QVector<int> RawImageLoad::histogramData() const {
    return m_histogram;
}

QImage RawImageLoad::loadRawImage() {
    QString filePath = "D:\\RAD\\DR\\Proj.raw";
    int w = 3072, h = 3072;
    FILE *Img = fopen(filePath.toStdString().c_str(), "rb");

    if (!Img) {
        qDebug() << "Failed to open file!";
        return QImage();
    }

    unsigned short* Image = new unsigned short[w * h];
    fread(Image, sizeof(unsigned short), w * h, Img);
    arr = new unsigned char[w * h];

    for (int i = 0; i < w * h; i++) {
        arr[i] = (Image[i] * 255) / 25000;
    }

    QImage image(arr, w, h, QImage::Format_Grayscale8);

    fclose(Img);
    delete[] Image;

    return image;
}

void RawImageLoad::calculateHistogram() {
    m_histogram.fill(0, 256); // Reset histogram array

    for (int i = 0; i < 3072 * 3072; i++) {
        m_histogram[arr[i]]++;
    }
}
