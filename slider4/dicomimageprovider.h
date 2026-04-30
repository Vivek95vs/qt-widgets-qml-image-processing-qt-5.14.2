#ifndef DICOMIMAGEPROVIDER_H
#define DICOMIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class DicomImageProvider : public QQuickImageProvider
{
public:
    enum ImageType {
        Axial,
        Coronal,
        Sagittal
    };

    DicomImageProvider(ImageType type);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    void setImage(const QImage& image);
    QImage getImage() const;

private:
    mutable QMutex m_mutex;
    QImage m_image;
    ImageType m_type;
};

#endif // DICOMIMAGEPROVIDER_H
