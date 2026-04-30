#include "myimagecontroller.h"
#include "myimagecontroller.h"
extern QByteArray  TCPData;
ImageController::ImageController(MyImageProvider *provider, QObject *parent)
    : QObject(parent), imageProvider(provider), m_pollThread(nullptr) {


    qDebug()<<"constructor enter";
}

void ImageController::loadRawImage() {
    imageProvider->addRawImage("img1", "D:/DR.raw", 3072, 3072);
    emit imageUpdated();
}

//QString ImageController::hello() const{
//    QString hi="heeeeeee";
//    return hi;
//}
void ImageController::flipImage_vertically() {
    img1 = imageProvider->getImage("img1");
    if (img1.isNull()) return;

    QImage flipped = img1.mirrored(false, true);  // Flip vertically
    imageProvider->addImage("img1", flipped);  // Store flipped image
    emit imageUpdated();  // Notify QML

    QByteArray data("1,");
    // Instead of using Server directly, use the thread
      // You'll need to add m_pollThread as a member
    m_pollThread.sendToClient(1, data);

    TCPData = data;
    qDebug()<<"Send Acknowledge...................................."<<data;
}

void ImageController::flipImage_horizontally() {
    img1 = imageProvider->getImage("img1");
    if (img1.isNull()) return;

    QImage flipped = img1.mirrored(true, false);  // Flip horizontally
    imageProvider->addImage("img1", flipped);  // Store flipped image
    emit imageUpdated();  // Notify QML
}

//cv::Mat ImageController::QImageToCvMat(const QImage &inImage) {
//    switch (inImage.format()) {
//        case QImage::Format_RGB888:
//            return cv::Mat(inImage.height(), inImage.width(), CV_8UC3,
//                           const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine()).clone();
//        case QImage::Format_Grayscale8:
//            return cv::Mat(inImage.height(), inImage.width(), CV_8UC1,
//                           const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine()).clone();
//        default:
//            return cv::Mat();  // Handle unsupported formats
//    }
//}

cv::Mat ImageController::QImageToCvMat(const QImage &inImage) {
    QImage swapped = inImage.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(swapped.height(), swapped.width(), CV_8UC3,
                   const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
}

//cv::Mat ImageController::QImageToCvMat(const QImage &inImage) {
//    QImage converted = inImage.convertToFormat(QImage::Format_RGB888);
//    return cv::Mat(converted.height(), converted.width(), CV_8UC3,
//                   const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
//}

void ImageController::medianFilter()
{
    img1 = imageProvider->getImage("img1");
    if (img1.isNull()) return;

    // Convert QImage to OpenCV Mat
     cv::Mat src = QImageToCvMat(img1);
     if (src.empty()) return;

     // Apply median filter (kernel size must be odd and > 1)
     cv::Mat dst;
     cv::medianBlur(src, dst, 5);  // You can try 3, 5, 7 based on smoothing strength

     // Convert result back to QImage
     QImage result = QImage(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888)
                         .rgbSwapped().copy();  // Ensure proper color channel order

     // Update in imageProvider
     imageProvider->addImage("img1", result);

    emit imageUpdated();
}

void ImageController::scatterCorrection()
{
    img1 = imageProvider->getImage("img1");
    if (img1.isNull()) return;

    QImage gray = img1.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat input(gray.height(), gray.width(), CV_8UC1,
                 const_cast<uchar*>(gray.bits()), gray.bytesPerLine());

    // Create CLAHE object (Contrast Limited Adaptive Histogram Equalization)
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4); // Adjust this parameter as needed
    clahe->setTilesGridSize(cv::Size(8, 8)); // Adjust grid size

    cv::Mat corrected;
    clahe->apply(input, corrected);

    // Convert back to QImage
    QImage result(corrected.cols, corrected.rows, QImage::Format_Grayscale8);
    for (int y = 0; y < corrected.rows; ++y) {
        uchar* srcRow = corrected.ptr<uchar>(y);
        uchar* dstRow = result.scanLine(y);
        memcpy(dstRow, srcRow, corrected.cols);
    }

    imageProvider->addImage("img1", result);
    emit imageUpdated();
}

void ImageController::enhanceEdges(){

    img1 = imageProvider->getImage("img1");
    if (img1.isNull()) return;



    // Convert QImage to cv::Mat (RGB or grayscale)
        cv::Mat matOriginal = QImageToCvMat(img1);
        if (matOriginal.empty()) return;

        cv::Mat matGray;
        if (matOriginal.channels() == 3)
            cv::cvtColor(matOriginal, matGray, cv::COLOR_BGR2GRAY);
        else
            matGray = matOriginal.clone();

        // Apply Sobel Edge Detection
        cv::Mat grad_x, grad_y;
        cv::Sobel(matGray, grad_x, CV_16S, 1, 0, 3);
        cv::Sobel(matGray, grad_y, CV_16S, 0, 1, 3);

        cv::Mat abs_grad_x, abs_grad_y, grad;
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        // Optional: Blend with original image to enhance edges
        cv::Mat enhanced;
        cv::addWeighted(matGray, 0.8, grad, 0.8, 0, enhanced);  // Alpha blend

        // Convert to QImage
        QImage result = QImage(enhanced.data, enhanced.cols, enhanced.rows, enhanced.step, QImage::Format_Grayscale8).copy();


    // Update in imageProvider
        imageProvider->addImage("img1", result);
        emit imageUpdated();
}
