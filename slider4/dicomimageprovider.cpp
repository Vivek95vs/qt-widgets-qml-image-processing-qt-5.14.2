#include "dicomimageprovider.h"
#include <QDebug>

DicomImageProvider::DicomImageProvider(ImageType type)
    : QQuickImageProvider(QQuickImageProvider::Image),
      m_type(type)
{
}

QImage DicomImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(requestedSize);

    QMutexLocker locker(&m_mutex);

    if (m_image.isNull()) {
        // Return a black placeholder
        QImage placeholder(512, 512, QImage::Format_Grayscale16);
        placeholder.fill(0);

        if (size) {
            *size = placeholder.size();
        }

        return placeholder;
    }

    if (size) {
        *size = m_image.size();
    }

    return m_image;
}

void DicomImageProvider::setImage(const QImage& image)
{
    QMutexLocker locker(&m_mutex);
    m_image = image;
}

QImage DicomImageProvider::getImage() const
{
    QMutexLocker locker(&m_mutex);
    return m_image;
}
