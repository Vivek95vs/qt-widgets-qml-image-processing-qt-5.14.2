#include "DicomImageProvider.h"
#include <QPainter>
#include <QDebug>

DicomImageProvider::DicomImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    // Create initial placeholder image
    m_image = QImage(512, 512, QImage::Format_RGB888);
    m_image.fill(QColor(40, 44, 52)); // Dark background

    // Draw welcome message
    QPainter painter(&m_image);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24));
    painter.drawText(m_image.rect(), Qt::AlignCenter, "DICOM Viewer\n\nClick 'Load DICOM'");

    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(0, 400, 512, 100), Qt::AlignCenter, "Ready to load DICOM series");

    qDebug() << "ImageProvider created with initial image size:" << m_image.size();
}

void DicomImageProvider::setImage(const QImage& image)
{
    QMutexLocker locker(&m_mutex);

    if (!image.isNull()) {
        m_image = image.copy();
        qDebug() << "ImageProvider: Image set successfully. Size:" << m_image.size()
                 << "Format:" << m_image.format();
    } else {
        qWarning() << "ImageProvider: Received null image!";
        // Create error image
        m_image = QImage(512, 512, QImage::Format_RGB888);
        m_image.fill(Qt::red);

        QPainter painter(&m_image);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 16));
        painter.drawText(m_image.rect(), Qt::AlignCenter, "ERROR: Null image received");
    }
}

void DicomImageProvider::clearImage()
{
    QMutexLocker locker(&m_mutex);
    m_image = QImage(512, 512, QImage::Format_RGB888);
    m_image.fill(QColor(60, 60, 60));

    QPainter painter(&m_image);
    painter.setPen(Qt::white);
    painter.drawText(m_image.rect(), Qt::AlignCenter, "Image cleared");
}

QImage DicomImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    QMutexLocker locker(&m_mutex);

    qDebug() << "ImageProvider: Request for image id:" << id;

    if (size) {
        *size = m_image.size();
    }

    // Handle different requests
    if (id.startsWith("test")) {
        QImage testImg(200, 200, QImage::Format_RGB888);
        testImg.fill(Qt::green);
        return testImg;
    }

    // Return a copy of the current image
    if (m_image.isNull()) {
        qWarning() << "ImageProvider: m_image is null, returning placeholder";
        QImage placeholder(100, 100, QImage::Format_RGB888);
        placeholder.fill(Qt::blue);
        return placeholder;
    }

    return m_image.copy();
}

QImage DicomImageProvider::currentImage() const
{
    QMutexLocker locker(&m_mutex);
    return m_image;
}
