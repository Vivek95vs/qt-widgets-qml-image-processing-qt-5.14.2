#include "collimationmanager.h"
#include <QDebug>
#include <QFile>
#include <QBuffer>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CollimationManager::CollimationManager(QObject *parent)
    : QObject(parent)
    , m_originalSize(42.7, 42.7)
    , m_fid(1200.0)
    , m_collimationX(0.0)
    , m_collimationY(0.0)
    , m_activeHandle(-1)
    , m_isMoving(false)
{
    m_cropRect = QRectF(0, 0, m_originalSize.width(), m_originalSize.height());
}

bool CollimationManager::load16BitRaw(const QString &filePath, int width, int height)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open:" << filePath;
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    int expectedSize = width * height * 2;
    if (rawData.size() < expectedSize) {
        qDebug() << "File size mismatch. Expected:" << expectedSize << "Got:" << rawData.size();
        return false;
    }

    m_image = QImage(width, height, QImage::Format_Grayscale16);
    memcpy(m_image.bits(), rawData.data(), expectedSize);

    // Auto-level
    quint16 minVal = 65535, maxVal = 0;
    quint16 *pixels = reinterpret_cast<quint16*>(m_image.bits());
    int numPixels = width * height;

    for (int i = 0; i < numPixels; i++) {
        if (pixels[i] < minVal) minVal = pixels[i];
        if (pixels[i] > maxVal) maxVal = pixels[i];
    }

    if (maxVal > minVal) {
        for (int i = 0; i < numPixels; i++) {
            pixels[i] = static_cast<quint16>((pixels[i] - minVal) * 65535.0 / (maxVal - minVal));
        }
    }

    return true;
}

void CollimationManager::loadRawImage(const QString &filePath, int width, int height)
{
    if (load16BitRaw(filePath, width, height)) {
        convertAndStoreImage();
        resetCollimation();
        emit imageLoadedChanged();
        qDebug() << "Image loaded successfully";
    } else {
        m_image = QImage(width, height, QImage::Format_Grayscale16);
        m_image.fill(QColor(32768,32768,32768).rgb());
        convertAndStoreImage();
        emit imageLoadedChanged();
        qDebug() << "Created test pattern";
    }
}

void CollimationManager::convertAndStoreImage()
{
    if (m_image.isNull()) return;

    QImage displayImage = m_image.convertToFormat(QImage::Format_Grayscale8);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    displayImage.save(&buffer, "PNG");
    m_imageData = QString("data:image/png;base64,") + ba.toBase64();
}

void CollimationManager::setCropRect(const QRectF &rect)
{
    QRectF newRect = rect;

    // Clamp to image bounds
    if (newRect.left() < 0) newRect.setLeft(0);
    if (newRect.top() < 0) newRect.setTop(0);
    if (newRect.right() > m_originalSize.width()) newRect.setRight(m_originalSize.width());
    if (newRect.bottom() > m_originalSize.height()) newRect.setBottom(m_originalSize.height());

    // Ensure minimum size
    if (newRect.width() < 0.5) newRect.setWidth(0.5);
    if (newRect.height() < 0.5) newRect.setHeight(0.5);

    if (m_cropRect != newRect) {
        m_cropRect = newRect;
        updateCollimation();
        emit cropRectChanged();
        qDebug() << "Crop rect:" << m_cropRect.x() << m_cropRect.y() << m_cropRect.width() << m_cropRect.height();
    }
}

void CollimationManager::startResize(int handleIndex, qreal startX, qreal startY,
                                      qreal imageWidth, qreal imageHeight)
{
    m_activeHandle = handleIndex;
    m_startX = startX;
    m_startY = startY;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_startRect = m_cropRect;

    qDebug() << "Start resize - handle:" << handleIndex << "startRect:" << m_startRect;
}

void CollimationManager::updateResize(qreal currentX, qreal currentY)
{
    if (m_activeHandle < 0) return;

    qreal deltaX = currentX - m_startX;
    qreal deltaY = currentY - m_startY;

    qreal scaleX = m_originalSize.width() / m_imageWidth;
    qreal scaleY = m_originalSize.height() / m_imageHeight;

    qreal dx = deltaX * scaleX;
    qreal dy = deltaY * scaleY;

    if (dx == 0 && dy == 0) return;

    qDebug() << "Resize - handle:" << m_activeHandle << "delta:" << deltaX << deltaY << "dx dy:" << dx << dy;

    QRectF newRect = m_startRect;

    switch(m_activeHandle) {
        case 0: // Top-left
            newRect.setLeft(qMax(0.0, newRect.left() + dx));
            newRect.setTop(qMax(0.0, newRect.top() + dy));
            break;
        case 1: // Top
            newRect.setTop(qMax(0.0, newRect.top() + dy));
            break;
        case 2: // Top-right
            newRect.setRight(qMin(m_originalSize.width(), newRect.right() + dx));
            newRect.setTop(qMax(0.0, newRect.top() + dy));
            break;
        case 3: // Right
            newRect.setRight(qMin(m_originalSize.width(), newRect.right() + dx));
            break;
        case 4: // Bottom-right
            newRect.setRight(qMin(m_originalSize.width(), newRect.right() + dx));
            newRect.setBottom(qMin(m_originalSize.height(), newRect.bottom() + dy));
            break;
        case 5: // Bottom
            newRect.setBottom(qMin(m_originalSize.height(), newRect.bottom() + dy));
            break;
        case 6: // Bottom-left
            newRect.setLeft(qMax(0.0, newRect.left() + dx));
            newRect.setBottom(qMin(m_originalSize.height(), newRect.bottom() + dy));
            break;
        case 7: // Left
            newRect.setLeft(qMax(0.0, newRect.left() + dx));
            break;
    }

    // Ensure valid rectangle (width and height > 0)
    if (newRect.width() < 0.5) {
        if (m_activeHandle == 0 || m_activeHandle == 6 || m_activeHandle == 7)
            newRect.setLeft(newRect.right() - 0.5);
        else
            newRect.setRight(newRect.left() + 0.5);
    }
    if (newRect.height() < 0.5) {
        if (m_activeHandle == 0 || m_activeHandle == 1 || m_activeHandle == 2)
            newRect.setTop(newRect.bottom() - 0.5);
        else
            newRect.setBottom(newRect.top() + 0.5);
    }

    setCropRect(newRect);
}
void CollimationManager::endResize()
{
    m_activeHandle = -1;
}

void CollimationManager::startMove(qreal startX, qreal startY, qreal imageWidth, qreal imageHeight)
{
    m_isMoving = true;
    m_startX = startX;
    m_startY = startY;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_startRect = m_cropRect;
}

void CollimationManager::updateMove(qreal currentX, qreal currentY)
{
    if (!m_isMoving) return;

    qreal deltaX = currentX - m_startX;
    qreal deltaY = currentY - m_startY;

    qreal scaleX = m_originalSize.width() / m_imageWidth;
    qreal scaleY = m_originalSize.height() / m_imageHeight;

    qreal moveX = deltaX * scaleX;
    qreal moveY = deltaY * scaleY;

    QRectF newRect = m_startRect;
    newRect.translate(moveX, moveY);

    // Clamp to image bounds
    if (newRect.left() < 0) newRect.setLeft(0);
    if (newRect.top() < 0) newRect.setTop(0);
    if (newRect.right() > m_originalSize.width()) newRect.setRight(m_originalSize.width());
    if (newRect.bottom() > m_originalSize.height()) newRect.setBottom(m_originalSize.height());

    setCropRect(newRect);
}

void CollimationManager::endMove()
{
    m_isMoving = false;
}

void CollimationManager::updateCollimation()
{
    m_collimationX = (m_originalSize.width() - m_cropRect.width()) / 2.0;
    m_collimationY = (m_originalSize.height() - m_cropRect.height()) / 2.0;
    emit collimationChanged();
}

void CollimationManager::resetCollimation()
{
    setCropRect(QRectF(0, 0, m_originalSize.width(), m_originalSize.height()));
}


