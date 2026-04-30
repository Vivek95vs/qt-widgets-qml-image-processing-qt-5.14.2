#ifndef MAIN_H
#define MAIN_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QImage>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QObject> // Include this for QObject
#include <opencv2/opencv.hpp>

//// Image Provider Class
//class MyImageProvider : public QQuickImageProvider {
//public:
//    MyImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

//    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
//        if (imageMap.contains(id)) {
//            QImage image = imageMap[id];

//            if (size) *size = image.size();
//            if (requestedSize.isValid())
//                return image.scaled(requestedSize, Qt::KeepAspectRatio);

//            return image;
//        }
//        return QImage();
//    }

//    void addRawImage(const QString &imageId, const QString &filePath, int width, int height) {
//        QImage img = loadRawImage(filePath, width, height);
//        if (!img.isNull()) {
//            imageMap[imageId] = img;
//        }
//    }
//    bool hasImage(const QString &id) const {
//        return imageMap.contains(id);
//    }
//    QImage getImage(const QString &id) const {
//        return imageMap.value(id, QImage());
//    }
//    void addImage(const QString &id, const QImage &image) {
//        imageMap[id] = image;
//    }





//private:
//    QMap<QString, QImage> imageMap;

//    QImage loadRawImage(const QString &filePath, int width, int height) {
//        int pixelSize = 2; // 16-bit grayscale

//        QFile file(filePath);
//        if (!file.open(QIODevice::ReadOnly)) {
//            qWarning() << "Failed to open file:" << filePath;
//            return QImage();
//        }

//        QByteArray rawData = file.readAll();
//        file.close();

//        if (rawData.size() != width * height * pixelSize) {
//            qWarning() << "File size does not match expected dimensions.";
//            return QImage();
//        }

//        QImage image(width, height, QImage::Format_Grayscale8);
//        const quint16 *rawPixels = reinterpret_cast<const quint16 *>(rawData.constData());
//        uchar *imageData = image.bits();

//        for (int i = 0; i < width * height; ++i) {
//            imageData[i] = rawPixels[i] >> 8; // Convert 16-bit to 8-bit
//        }

//        return image;
//    }
//};

//// Image Controller Class
//class ImageController : public QObject {
//    Q_OBJECT
//public:
//    ImageController(MyImageProvider *provider) : imageProvider(provider) {

//    }
//     QImage img;
//public slots:
//    void loadRawImage() {
//        imageProvider->addRawImage("img1", "D:/DR.raw", 3072, 3072);
////        imageProvider->addRawImage("img2", "D:/proj_1.raw", 3072, 3072);
//        emit imageUpdated();
//    }

//    void flipImage_vertically() {
//        img1=imageProvider->getImage("img1");
//            if (img1.isNull()) return;

//            QImage flipped = img1.mirrored(false, true);  // Flip vertically

//            imageProvider->addImage("img1",flipped);  // Store flipped image
//            emit imageUpdated();  // Notify QML
//        }

//    void flipImage_horizontally() {
//        img1=imageProvider->getImage("img1");
//            if (img1.isNull()) return;

//            QImage flipped = img1.mirrored(true, false);  // Flip vertically

//            imageProvider->addImage("img1",flipped);  // Store flipped image
//            emit imageUpdated();  // Notify QML
//        }


//signals:
//    void imageUpdated();

//private:
//    MyImageProvider *imageProvider;
//    QImage img1;

//};

#endif // MAIN_H
