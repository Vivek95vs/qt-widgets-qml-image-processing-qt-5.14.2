#ifndef SMARTCONTOURBRUSH_H
#define SMARTCONTOURBRUSH_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QColor>
#include <QVariant>
#include <QPainterPath>

class SmartContourBrush : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double brushSize READ brushSize WRITE setBrushSize NOTIFY brushSizeChanged)
    Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor NOTIFY brushColorChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY activeChanged)
    Q_PROPERTY(QVariantList contourPoints READ getContourPoints NOTIFY contourChanged)

public:
    explicit SmartContourBrush(QObject *parent = nullptr);

    double brushSize() const;
    void setBrushSize(double size);

    QColor brushColor() const;
    void setBrushColor(const QColor &color);

    bool isActive() const { return m_isActive; }

    QVariantList getContourPoints() const { return m_contourPoints; }

    Q_INVOKABLE void startNewContour(qreal x, qreal y);
    Q_INVOKABLE void updateCurrentContour(qreal x, qreal y);
    Q_INVOKABLE void finishCurrentContour();

    Q_INVOKABLE void startErase(qreal x, qreal y);
    Q_INVOKABLE void updateErase(qreal x, qreal y);
    Q_INVOKABLE void finishErase();

    Q_INVOKABLE void clear();
    Q_INVOKABLE double calculateArea() const;

signals:
    void brushSizeChanged();
    void brushColorChanged();
    void contourChanged();
    void activeChanged();

private:
    struct Contour {
        QPainterPath path;
        bool isActive;
        QPointF lastPoint;
    };

    QVector<Contour> m_contours;
    QVariantList m_contourPoints;
    bool m_isActive;
    bool m_isErasing;
    int m_currentContourIndex;

    double m_brushSize;
    QColor m_brushColor;

    void updatePoints();
    void addPointToCurrentContour(const QPointF &point);
    void removeFromAllContours(const QPointF &center);
    QPainterPath createCirclePath(const QPointF &center, double radius);
    void mergeCloseContours();
};

#endif // SMARTCONTOURBRUSH_H
