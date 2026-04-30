#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QColor>

class DrawingArea : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QColor rectColor READ rectColor WRITE setRectColor NOTIFY rectColorChanged)
    Q_PROPERTY(QColor circleColor READ circleColor WRITE setCircleColor NOTIFY circleColorChanged)

public:
    explicit DrawingArea(QObject *parent = nullptr);

    QColor lineColor() const;
    void setLineColor(const QColor &color);

    QColor rectColor() const;
    void setRectColor(const QColor &color);

    QColor circleColor() const;
    void setCircleColor(const QColor &color);

signals:
    void lineColorChanged();
    void rectColorChanged();
    void circleColorChanged();

public slots:
    void handleMousePressed(const QPointF &point);
    void handleMouseMoved(const QPointF &point);
    void handleMouseReleased();

    const QList<QLineF> &lines() const;
    const QList<QRectF> &rectangles() const;
    //const QList<QEllipse> &circles() const;

private:
    QList<QLineF> m_lines;
    QList<QRectF> m_rectangles;
    //QList<QEllipse> m_circles;

    QPointF m_lastPoint;
    QColor m_lineColor;
    QColor m_rectColor;
    QColor m_circleColor;
};

#endif // DRAWINGAREA_H
