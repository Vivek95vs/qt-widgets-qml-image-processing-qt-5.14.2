#include "rawimageload.h"
#include <QDebug>

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image), m_imageLoaded(false), m_windowWidth(255), m_windowLevel(128) {
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
        computeWindowFromHistogram(); // Compute WW and WL from the histogram
        applyWindow(); // Apply the computed WW and WL to the image
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

    // Scale the 16-bit raw data to 8-bit [0, 255]
    for (int i = 0; i < w * h; i++) {
        arr[i] = static_cast<unsigned char>((Image[i] * 255) / 65535); // Scale to 8-bit
    }

    QImage image(arr, w, h, QImage::Format_Grayscale8);

    fclose(Img);
    delete[] Image;

    return image;
}

void RawImageLoad::calculateHistogram() {
    m_histogram.fill(0, 256); // Reset histogram array

    // Calculate histogram based on the current pixel values
    for (int i = 0; i < 3072 * 3072; i++) {
        m_histogram[arr[i]]++;
    }
}

void RawImageLoad::computeWindowFromHistogram() {
    // Compute the cumulative distribution function (CDF) from the histogram
    QVector<int> cdf(256, 0);
    cdf[0] = m_histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + m_histogram[i];
    }

    // Find the intensity range that covers 95% of the pixels
    int totalPixels = 3072 * 3072;
    int lowerThreshold = totalPixels * 0.025; // 2.5% of pixels
    int upperThreshold = totalPixels * 0.975; // 97.5% of pixels

    int minIntensity = 0;
    int maxIntensity = 255;

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

    // Compute Window Level (WL) as the mean intensity
    m_windowLevel = (minIntensity + maxIntensity) / 2;

    // Compute Window Width (WW) as the range of intensities
    m_windowWidth = maxIntensity - minIntensity;

    qDebug() << "Computed Window Level:" << m_windowLevel << "Window Width:" << m_windowWidth;
}

void RawImageLoad::applyWindow() {
    int minVal = m_windowLevel - m_windowWidth / 2;
    int maxVal = m_windowLevel + m_windowWidth / 2;

    // Ensure minVal and maxVal are within the valid range [0, 255]
    minVal = qMax(0, minVal);
    maxVal = qMin(255, maxVal);

    // Apply windowing to the image
    for (int i = 0; i < 3072 * 3072; i++) {
        if (arr[i] < minVal) {
            arr[i] = 0; // Clamp values below minVal to 0
        } else if (arr[i] > maxVal) {
            arr[i] = 255; // Clamp values above maxVal to 255
        } else {
            // Scale values within the window to the range [0, 255]
            arr[i] = ((arr[i] - minVal) * 255) / (maxVal - minVal);
        }
    }

    // Update the image
    myImage = QImage(arr, 3072, 3072, QImage::Format_Grayscale8);

    // Recalculate the histogram based on the modified pixel values
    calculateHistogram();

    // Notify QML to update the image and histogram
    emit imageUpdated();
    emit histogramUpdated();
}
