#ifndef RAWIMAGELOAD_H
#define RAWIMAGELOAD_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QDebug>
#include <opencv2/opencv.hpp>

class RawImageLoad : public QObject, public QQuickImageProvider {
    Q_OBJECT
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)

public:
    explicit RawImageLoad(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void loadImage();
    Q_INVOKABLE void startWindowLevel(int x, int y);
    Q_INVOKABLE void adjustWindowLevel(int x, int y);
    Q_INVOKABLE void endWindowLevel();
    bool imageLoaded() const;

signals:
    void imageLoadedChanged();
    void imageUpdated();

private:
    QImage loadRawImage();
    void applyWindowLevel();

    QImage myImage;
    cv::Mat cvImage;
    bool m_imageLoaded;
    int windowLevel;
    int windowWidth;
    bool isAdjustingWindowLevel;
    int startX, startY;
};

#endif // RAWIMAGELOAD_H
