#ifndef RAWIMAGELOAD_H
#define RAWIMAGELOAD_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>

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

signals:
    void imageLoadedChanged();

private:
    QImage loadRawImage();
    unsigned char* arr;
    QImage myImage;
    bool m_imageLoaded; // Private member variable
};

#endif // RAWIMAGELOAD_H
