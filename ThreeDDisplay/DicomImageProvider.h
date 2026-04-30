#pragma once
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QMutex>
#include <QVector3D>

class DicomImageProvider : public QQuickImageProvider {
public:
    DicomImageProvider();

    void setSliceImage(int viewType, const QImage& image);
    void setCrosshairPosition(const QVector3D& position, const QVector3D& volumeDims);
    void setShowCrosshair(bool show);
    void setCrosshairColor(const QColor& color);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    QImage addCrosshairToImage(const QImage& baseImage, int viewType);

    QImage m_sliceImages[3]; // 0=axial, 1=coronal, 2=sagittal
    QVector3D m_crosshairPosition;
    QVector3D m_volumeDimensions;
    bool m_showCrosshair;
    QColor m_crosshairColor;
    QMutex m_mutex;
};
