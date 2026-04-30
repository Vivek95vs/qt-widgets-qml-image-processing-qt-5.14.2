#ifndef DICOMIMAGEPROVIDER_H
#define DICOMIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>

class DicomImageProvider : public QQuickImageProvider
{
public:
    DicomImageProvider();

    void setImage(const QImage& image);
    void clearImage();

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    QImage currentImage() const;

private:
    QImage m_image;
    mutable QMutex m_mutex;
};

#endif // DICOMIMAGEPROVIDER_H
