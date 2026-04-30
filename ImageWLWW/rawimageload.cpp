#include "rawimageload.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image), m_imageLoaded(false), windowLevel(32768), windowWidth(65536), isAdjustingWindowLevel(false) {
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
    cvImage = cv::Mat(myImage.height(), myImage.width(), CV_16UC1, myImage.bits());
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

    // Open the raw file
    FILE *Img = fopen(filePath.toStdString().c_str(), "rb");
    if (!Img) {
        qWarning() << "Failed to open raw image file";
        return QImage();
    }

    // Read 16-bit raw data
    unsigned short* Image = new unsigned short[w * h];
    fread(Image, sizeof(unsigned short), w * h, Img);

    // Create a cv::Mat from the raw data
    cv::Mat cvImage(h, w, CV_16UC1, Image);

    // Debug: Print the type of cvImage
    qDebug() << "cvImage type in loadRawImage:" << cvImage.type();

    // Normalize the 16-bit image to 8-bit for display
    cv::Mat normalizedImage;
    cv::normalize(cvImage, normalizedImage, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    // Convert the normalized OpenCV image to QImage
    QImage qImage(normalizedImage.data, normalizedImage.cols, normalizedImage.rows, normalizedImage.step, QImage::Format_Grayscale8);

    fclose(Img);

    // Clean up dynamically allocated memory
    delete[] Image;

    return qImage.copy(); // Return a copy to avoid memory issues
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
    windowLevel += deltaY * 256; // Scale deltaY for 16-bit range
    windowWidth += deltaX * 512; // Scale deltaX for 16-bit range

    // Clamp values to avoid overflow
    windowLevel = std::max(0, std::min(65535, windowLevel));
    windowWidth = std::max(1, std::min(65535, windowWidth)); // Ensure windowWidth is at least 1

    qDebug() << "Adjusted Window Level:" << windowLevel << "Window Width:" << windowWidth;

    applyWindowLevel();

    startX = x;
    startY = y;
}

void RawImageLoad::endWindowLevel() {
    isAdjustingWindowLevel = false;
}

void RawImageLoad::applyWindowLevel() {
    if (cvImage.empty()) {
        qWarning() << "cvImage is empty!";
        return;
    }

    // Debug: Print the type of cvImage
    qDebug() << "cvImage type:" << cvImage.type();
    qDebug() << "CV_16UC1:" << CV_16UC1;

    // Ensure cvImage is in CV_16UC1 format
    if (cvImage.type() != CV_16UC1) {
        qWarning() << "cvImage is not in CV_16UC1 format. Converting...";
        cvImage.convertTo(cvImage, CV_16UC1);
    }

    // Ensure windowWidth is valid
    if (windowWidth <= 0) {
        qWarning() << "Invalid windowWidth:" << windowWidth;
        windowWidth = 1; // Set to a valid minimum value
    }

    // Ensure windowLevel is within valid range
    windowLevel = std::max(0, std::min(65535, windowLevel));

    // Debug: Print image dimensions
    qDebug() << "cvImage dimensions:" << cvImage.cols << "x" << cvImage.rows;

    // Apply window level adjustment
    cv::Mat adjustedImage;
    try {
        qDebug()<<"Enter try adjust";
        cvImage.convertTo(adjustedImage, CV_16UC1, 1.0, windowLevel - 32768);
    } catch (const cv::Exception &e) {
        qWarning() << "Error in convertTo:" << e.what();
        return;
    }

    // Debug: Print adjustedImage dimensions
    qDebug() << "adjustedImage dimensions:" << adjustedImage.cols << "x" << adjustedImage.rows;

    // Apply window width adjustment
    double alpha = 65535.0 / windowWidth;
    try {
        adjustedImage = adjustedImage * alpha;
    } catch (const cv::Exception &e) {
        qWarning() << "Error in alpha multiplication:" << e.what();
        return;
    }

    // Clamp values to 0-65535
    try {
        cv::threshold(adjustedImage, adjustedImage, 65535, 65535, cv::THRESH_TRUNC);
        cv::threshold(adjustedImage, adjustedImage, 0, 0, cv::THRESH_TOZERO);
    } catch (const cv::Exception &e) {
        qWarning() << "Error in thresholding:" << e.what();
        return;
    }

    // Debug: Print min and max pixel values in adjustedImage
    double minVal, maxVal;
    cv::minMaxLoc(adjustedImage, &minVal, &maxVal);
    qDebug() << "adjustedImage minVal:" << minVal << "maxVal:" << maxVal;

    // Scale 16-bit data to 8-bit for display
    unsigned char* scaledImage = new unsigned char[adjustedImage.rows * adjustedImage.cols];
    for (int y = 0; y < adjustedImage.rows; ++y) {
        for (int x = 0; x < adjustedImage.cols; ++x) {
            // Ensure pixel access is within bounds
            if (y >= 0 && y < adjustedImage.rows && x >= 0 && x < adjustedImage.cols) {
                unsigned short pixelValue = adjustedImage.at<unsigned short>(y, x);
                scaledImage[y * adjustedImage.cols + x] = static_cast<unsigned char>(pixelValue >> 8); // Scale to 8-bit
            } else {
                qWarning() << "Pixel access out of bounds! y:" << y << "x:" << x;
            }
        }
    }

    // Convert back to QImage (copy the data to avoid memory issues)
    myImage = QImage(scaledImage, adjustedImage.cols, adjustedImage.rows, QImage::Format_Grayscale8).copy();

    // Clean up
    delete[] scaledImage;

    qDebug() << "Window Level:" << windowLevel << "Window Width:" << windowWidth;
    qDebug() << "Image updated successfully!";

    // Notify QML that the image has been updated
    emit imageLoadedChanged();
    emit imageUpdated();
}
