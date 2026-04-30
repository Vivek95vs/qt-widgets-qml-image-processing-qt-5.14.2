#include "rawimageload.h"
#include "QDebug"

RawImageLoad::RawImageLoad(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image), m_imageLoaded(false) {
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
    m_imageLoaded = true;
    emit imageLoadedChanged(); // Notify QML that the image has been loaded
}

bool RawImageLoad::imageLoaded() const {
    return m_imageLoaded; // Getter for imageLoaded
}

QImage RawImageLoad::loadRawImage() {
    QString filePath = "D:\\RAD\\proj_1.raw"; // Update with your file path
    int w = 3072;
    int h = 3072;
    FILE *Img;
    Img = fopen(filePath.toStdString().c_str(), "rb");

    unsigned short* Image = new unsigned short[w * h];
    fread(Image, 1, w * h * sizeof(unsigned short), Img);
    arr = new unsigned char[w * h];
    for (int i = 0; i < w * h; i++) {
        arr[i] = ((Image[i] * 255) / 65535);
    }

    qDebug() << arr[20];
    QImage image = QImage(arr, w, h, QImage::Format_Grayscale8);

    // Clean up dynamically allocated memory
    fclose(Img);
    delete[] Image;
    return image;
}
