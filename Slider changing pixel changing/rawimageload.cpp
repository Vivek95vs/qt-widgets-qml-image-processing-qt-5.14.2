#include "rawimageload.h"
#include <QDebug>

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image),
      m_imageLoaded(false), m_windowWidth(255), m_windowLevel(128), originalImageData(nullptr), arr(nullptr) {}

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
        calculateHistogram();
        computeWindowFromHistogram();
        applyWindow();  // Apply WW/WL initially
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

int RawImageLoad::windowWidth() const {
    return m_windowWidth;
}

int RawImageLoad::windowLevel() const {
    return m_windowLevel;
}

void RawImageLoad::setWindowWidth(int width) {
    if (m_windowWidth != width) {
        m_windowWidth = width;
        applyWindow();
        emit windowWidthChanged();
        emit histogramUpdated();
        emit imageUpdated();
    }
}

void RawImageLoad::setWindowLevel(int level) {
    if (m_windowLevel != level) {
        m_windowLevel = level;
        applyWindow();
        emit windowLevelChanged();
        emit histogramUpdated();
        emit imageUpdated();
    }
}

QImage RawImageLoad::loadRawImage() {
    QString filePath = "D:\\RAD\\DR\\Proj.raw";
    int w = 3072, h = 3072;
    FILE *Img = fopen(filePath.toStdString().c_str(), "rb");

    if (!Img) {
        qDebug() << "Failed to open file!";
        return QImage();
    }

    // Allocate memory for original image and 8-bit image
    originalImageData = new unsigned short[w * h];
    arr = new unsigned char[w * h];

    fread(originalImageData, sizeof(unsigned short), w * h, Img);

    // Scale the 16-bit raw data to 8-bit
    for (int i = 0; i < w * h; i++) {
        arr[i] = static_cast<unsigned char>((originalImageData[i] * 255) / 65535);
    }

    fclose(Img);

    return QImage(arr, w, h, QImage::Format_Grayscale8);
}

void RawImageLoad::calculateHistogram() {
    m_histogram.fill(0, 256);

    for (int i = 0; i < 3072 * 3072; i++) {
        m_histogram[arr[i]]++;
    }
}

void RawImageLoad::computeWindowFromHistogram() {
    QVector<int> cdf(256, 0);
    cdf[0] = m_histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + m_histogram[i];
    }

    int totalPixels = 3072 * 3072;
    int lowerThreshold = totalPixels * 0.025;
    int upperThreshold = totalPixels * 0.975;

    int minIntensity = 0, maxIntensity = 255;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] >= lowerThreshold) {
            minIntensity = i;
            break;
        }
    }

    for (int i = 255; i >= 0; i--) {
        if (cdf[i] <= upperThreshold) {
            maxIntensity = i;
            break;
        }
    }

    m_windowLevel = (minIntensity + maxIntensity) / 2;
    m_windowWidth = maxIntensity - minIntensity;
}

void RawImageLoad::applyWindow() {
    int minVal = m_windowLevel - m_windowWidth / 2;
    int maxVal = m_windowLevel + m_windowWidth / 2;
    minVal = qMax(0, minVal);
    maxVal = qMin(255, maxVal);

    for (int i = 0; i < 3072 * 3072; i++) {
        int value = (originalImageData[i] * 255) / 65535;

        if (value < minVal) {
            arr[i] = 0;
        } else if (value > maxVal) {
            arr[i] = 255;
        } else {
            arr[i] = static_cast<unsigned char>(((value - minVal) * 255) / (maxVal - minVal));
        }
    }

    myImage = QImage(arr, 3072, 3072, QImage::Format_Grayscale8);
    calculateHistogram();
}
