#include "rawimageload.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image), m_imageLoaded(false), windowLevel(128), windowWidth(256), isAdjustingWindowLevel(false) {
    // Constructor
}

QImage RawImageLoad::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);

    qDebug() << "Requesting image with id:" << id;

    if (id == "main") {
        // Return the main raw image
        return myImage;
    }
    return QImage();
}

void RawImageLoad::loadImage() {
    myImage = loadRawImage();
    cvImage = cv::Mat(myImage.height(), myImage.width(), CV_8UC1, myImage.bits());
    m_imageLoaded = true;
    emit imageLoadedChanged(); // Notify QML that the image has been loaded
}

bool RawImageLoad::imageLoaded() const {
    return m_imageLoaded; // Getter for imageLoaded
}

QImage RawImageLoad::loadRawImage() {
    QString filePath = "D:\\RAD\\DR\\Proj.raw"; // Update with your file path
    int w = 3072;
    int h = 3072;
    FILE *Img;
    Img = fopen(filePath.toStdString().c_str(), "rb");

    unsigned short* Image = new unsigned short[w * h];
    fread(Image, 1, w * h * sizeof(unsigned short), Img);
    arr = new unsigned char[w * h];
    for (int i = 0; i < w * h; i++) {
        arr[i] = ((Image[i] * 255) / 25000);
    }

    qDebug() << arr[20];
    QImage image = QImage(arr, w, h, QImage::Format_Grayscale8);

    // Clean up dynamically allocated memory
    fclose(Img);
    delete[] Image;
    return image;

}

void RawImageLoad::startWindowLevel(int x, int y) {
    isAdjustingWindowLevel = true;
    startX = x;
    startY = y;
}

void RawImageLoad::adjustWindowLevel(int x, int y) {
    if (!isAdjustingWindowLevel) return;

    int deltaX = x - startX;
    int deltaY = y - startY;

    // Adjust window level and width based on mouse movement
    windowLevel += deltaY;
    windowWidth += deltaX;

    // Clamp values to avoid overflow
    windowLevel = std::max(0, std::min(255, windowLevel));
    windowWidth = std::max(1, std::min(512, windowWidth));

    applyWindowLevel();

    startX = x;
    startY = y;
}

void RawImageLoad::endWindowLevel() {
    isAdjustingWindowLevel = false;
}

void RawImageLoad::applyWindowLevel() {
    if (cvImage.empty()) return;

    cv::Mat adjustedImage;
    cvImage.convertTo(adjustedImage, CV_8UC1, 1.0, windowLevel - 128);

    // Apply window width
    double alpha = 255.0 / windowWidth;
    adjustedImage = adjustedImage * alpha;

    // Clamp values to 0-255
    cv::threshold(adjustedImage, adjustedImage, 255, 255, cv::THRESH_TRUNC);
    cv::threshold(adjustedImage, adjustedImage, 0, 0, cv::THRESH_TOZERO);

    // Convert back to QImage
    myImage = QImage(adjustedImage.data, adjustedImage.cols, adjustedImage.rows, adjustedImage.step, QImage::Format_Grayscale8).copy();

    qDebug() << "Window Level:" << windowLevel << "Window Width:" << windowWidth;
    qDebug() << "Image updated successfully!";

    // Notify QML that the image has been updated
    emit imageLoadedChanged();
    emit imageUpdated();
}
