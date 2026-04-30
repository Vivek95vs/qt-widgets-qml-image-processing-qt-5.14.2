#ifndef RAWIMAGELOAD_H
#define RAWIMAGELOAD_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

class RawImageLoad : public QObject, public QQuickImageProvider {
    Q_OBJECT
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged) // Declare as Q_PROPERTY

public:
    explicit RawImageLoad(QObject *parent = nullptr);

    // Override requestImage to handle both main image and ROI requests
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    // Slot to load the image
    Q_INVOKABLE void loadImage();

    // Getter for imageLoaded
    bool imageLoaded() const;

    // Mouse event handlers
    Q_INVOKABLE void startWindowLevel(int x, int y);
    Q_INVOKABLE void adjustWindowLevel(int x, int y);
    Q_INVOKABLE void endWindowLevel();

signals:
    void imageLoadedChanged();
    void imageUpdated(); // Signal to notify QML that the image has been updated

private:
    QImage loadRawImage();
    void applyWindowLevel();
    unsigned char* arr;
    QImage myImage;
    bool m_imageLoaded; // Private member variable

    // Window level and width
    int windowLevel;
    int windowWidth;
    cv::Mat cvImage;
    bool isAdjustingWindowLevel;
    int startX, startY;
};

#endif // RAWIMAGELOAD_H
