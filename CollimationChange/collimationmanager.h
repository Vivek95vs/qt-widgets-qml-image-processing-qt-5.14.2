#ifndef COLLIMATIONMANAGER_H
#define COLLIMATIONMANAGER_H

#include <QObject>
#include <QRectF>
#include <QSizeF>
#include <QPointF>
#include <QImage>

class CollimationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF cropRect READ cropRect WRITE setCropRect NOTIFY cropRectChanged)
    Q_PROPERTY(QSizeF originalSize READ originalSize CONSTANT)
    Q_PROPERTY(double fid READ fid CONSTANT)
    Q_PROPERTY(double collimationX READ collimationX NOTIFY collimationChanged)
    Q_PROPERTY(double collimationY READ collimationY NOTIFY collimationChanged)
    Q_PROPERTY(bool hasImage READ hasImage NOTIFY imageLoadedChanged)
    Q_PROPERTY(QString imageData READ imageData NOTIFY imageLoadedChanged)

public:
    explicit CollimationManager(QObject *parent = nullptr);

    QRectF cropRect() const { return m_cropRect; }
    QSizeF originalSize() const { return m_originalSize; }
    double fid() const { return m_fid; }
    double collimationX() const { return m_collimationX; }
    double collimationY() const { return m_collimationY; }
    bool hasImage() const { return !m_image.isNull(); }
    QString imageData() const { return m_imageData; }

    void setCropRect(const QRectF &rect);

    Q_INVOKABLE void loadRawImage(const QString &filePath, int width, int height);
    Q_INVOKABLE void resetCollimation();

    Q_INVOKABLE void startResize(int handleIndex, qreal startX, qreal startY,
                                  qreal imageWidth, qreal imageHeight);
    Q_INVOKABLE void updateResize(qreal currentX, qreal currentY);
    Q_INVOKABLE void endResize();

    Q_INVOKABLE void startMove(qreal startX, qreal startY, qreal imageWidth, qreal imageHeight);
    Q_INVOKABLE void updateMove(qreal currentX, qreal currentY);
    Q_INVOKABLE void endMove();

signals:
    void cropRectChanged();
    void collimationChanged();
    void imageLoadedChanged();

private:
    void updateCollimation();
    void convertAndStoreImage();
    bool load16BitRaw(const QString &filePath, int width, int height);

    QRectF m_cropRect;
    QSizeF m_originalSize;
    double m_fid;
    double m_collimationX;
    double m_collimationY;
    QImage m_image;
    QString m_imageData;

    int m_activeHandle;
    bool m_isMoving;
    qreal m_startX, m_startY;
    qreal m_imageWidth, m_imageHeight;
    QRectF m_startRect;
};

#endif // COLLIMATIONMANAGER_H
