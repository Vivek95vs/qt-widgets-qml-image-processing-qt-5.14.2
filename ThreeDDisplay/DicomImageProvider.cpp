#include "DicomImageProvider.h"
#include <QDebug>

DicomImageProvider::DicomImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_showCrosshair(true)
    , m_crosshairColor(Qt::red)
    , m_volumeDimensions(1, 1, 1) {
}

void DicomImageProvider::setSliceImage(int viewType, const QImage& image) {
    if (viewType >= 0 && viewType < 3) {
        QMutexLocker locker(&m_mutex);
        m_sliceImages[viewType] = image;
    }
}

void DicomImageProvider::setCrosshairPosition(const QVector3D& position, const QVector3D& volumeDims) {
    QMutexLocker locker(&m_mutex);
    m_crosshairPosition = position;
    m_volumeDimensions = volumeDims;
}

void DicomImageProvider::setShowCrosshair(bool show) {
    QMutexLocker locker(&m_mutex);
    m_showCrosshair = show;
}

void DicomImageProvider::setCrosshairColor(const QColor& color) {
    QMutexLocker locker(&m_mutex);
    m_crosshairColor = color;
}

QImage DicomImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    Q_UNUSED(requestedSize)

    QMutexLocker locker(&m_mutex);

    // Parse view type from id
    int viewType = -1;
    if (id.startsWith("axial")) viewType = 0;
    else if (id.startsWith("coronal")) viewType = 1;
    else if (id.startsWith("sagittal")) viewType = 2;

    if (viewType < 0 || viewType > 2 || m_sliceImages[viewType].isNull()) {
        // Return a blank image
        QImage blank(512, 512, QImage::Format_RGB888);
        blank.fill(Qt::black);
        if (size) *size = blank.size();
        return blank;
    }

    QImage result = m_sliceImages[viewType];

    if (m_showCrosshair && m_volumeDimensions.x() > 0 && m_volumeDimensions.y() > 0 && m_volumeDimensions.z() > 0) {
        result = addCrosshairToImage(result, viewType);
    }

    if (size) *size = result.size();
    return result;
}

QImage DicomImageProvider::addCrosshairToImage(const QImage& baseImage, int viewType) {
    if (baseImage.isNull()) {
        return baseImage;
    }

    QImage imageWithCrosshair;
    if (baseImage.format() == QImage::Format_Grayscale16) {
        // Convert to RGB for drawing colored crosshair
        imageWithCrosshair = baseImage.convertToFormat(QImage::Format_RGB888);
    } else {
        imageWithCrosshair = baseImage.copy();
    }

    QPainter painter(&imageWithCrosshair);

    painter.setPen(QPen(m_crosshairColor, 2));
    painter.setRenderHint(QPainter::Antialiasing);

    int imgWidth = imageWithCrosshair.width();
    int imgHeight = imageWithCrosshair.height();

    // Calculate crosshair position based on view type
    float crossX = 0, crossY = 0;

    switch(viewType) {
        case 0: // Axial
            crossX = (m_crosshairPosition.x() / m_volumeDimensions.x()) * imgWidth;
            crossY = (m_crosshairPosition.y() / m_volumeDimensions.y()) * imgHeight;
            break;
        case 1: // Coronal
            crossX = (m_crosshairPosition.x() / m_volumeDimensions.x()) * imgWidth;
            crossY = (m_crosshairPosition.z() / m_volumeDimensions.z()) * imgHeight;
            break;
        case 2: // Sagittal
            crossX = (m_crosshairPosition.y() / m_volumeDimensions.y()) * imgWidth;
            crossY = (m_crosshairPosition.z() / m_volumeDimensions.z()) * imgHeight;
            break;
    }

    // Draw crosshair lines
    painter.drawLine(0, crossY, imgWidth, crossY); // Horizontal line
    painter.drawLine(crossX, 0, crossX, imgHeight); // Vertical line

    // Draw center circle
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(crossX, crossY), 5, 5);

    return imageWithCrosshair;
}
