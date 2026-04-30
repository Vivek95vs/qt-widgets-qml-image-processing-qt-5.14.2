#include "imagemagnifier.h"
#include <QFile>
#include <QDebug>

ImageMagnifier::ImageMagnifier(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image) {
    // Constructor
}

QImage ImageMagnifier::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);

    qDebug() << "Requesting image with id:" << id;

    if (id == "main") {
        // Return the main raw image
        return loadRawImage();
    } else if (id == "roi") {
        // Return the current magnified ROI image
        return _roiImage;
    }

    return QImage();
}

QImage ImageMagnifier::loadRawImage() {
    QString filePath = "D:\\RAD\\proj_1.raw"; // Update with your file path
    int w=3072;
    int h=3072;
    FILE * Img;
    Img= fopen(filePath.toStdString().c_str(),"rb");

    unsigned short* Image=new unsigned short[w*h];
    fread(Image,1,w*h*sizeof(unsigned short),Img);
    arr=new unsigned char[w*h];
    for(int i=0;i<w*h;i++)
    {
        arr[i]=((Image[i]*255)/65535);
    }

    qDebug()<<arr[20];
    myImage = QImage(arr,w,h,QImage::Format_Grayscale8);

    // Clean up dynamically allocated memory
    fclose(Img);
    delete[] Image;
    return myImage;

}

void ImageMagnifier::updateROI(int x, int y, int width, int height, float zoomLevel) {
    qDebug() << "ROI Coordinates from QML - X:" << x << "Y:" << y << "Width:" << width << "Height:" << height;

    QImage fullImage = loadRawImage();

    if (fullImage.isNull()) {
        qWarning() << "Full image is null!";
        return;
    }

    // Ensure ROI is within bounds
    x = qMax(0, qMin(x, fullImage.width() - width));
    y = qMax(0, qMin(y, fullImage.height() - height));

    qDebug() << "Final ROI Coordinates - X:" << x << "Y:" << y << "Width:" << width << "Height:" << height;

    // Extract the ROI from the image
    QImage roiImage = fullImage.copy(x, y, width, height);

    // Scale the ROI image based on the zoom level
    _roiImage = roiImage.scaled(
        width * zoomLevel,
        height * zoomLevel,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    // Notify QML that the ROI image has changed
    emit roiImageUpdated();
}
