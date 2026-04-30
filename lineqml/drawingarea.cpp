#include "Drawingarea.h"
#include <QLineF>
#include <QRectF>
//#include <QEllipse>

DrawingArea::DrawingArea(QObject *parent)
    : QObject(parent),
      m_lineColor(Qt::black),
      m_rectColor(Qt::blue),
      m_circleColor(Qt::red)
{
}

QColor DrawingArea::lineColor() const
{
    return m_lineColor;
}

void DrawingArea::setLineColor(const QColor &color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        emit lineColorChanged();
    }
}

QColor DrawingArea::rectColor() const
{
    return m_rectColor;
}

void DrawingArea::setRectColor(const QColor &color)
{
    if (m_rectColor != color) {
        m_rectColor = color;
        emit rectColorChanged();
    }
}

QColor DrawingArea::circleColor() const
{
    return m_circleColor;
}

void DrawingArea::setCircleColor(const QColor &color)
{
    if (m_circleColor != color) {
        m_circleColor = color;
        emit circleColorChanged();
    }
}

void DrawingArea::handleMousePressed(const QPointF &point)
{
    m_lastPoint = point;
}

void DrawingArea::handleMouseMoved(const QPointF &point)
{
    // Handle shape drawing on mouse move.
}

void DrawingArea::handleMouseReleased()
{
    // Add shapes to lists based on the current mode
}

const QList<QLineF> &DrawingArea::lines() const
{
    return m_lines;
}

const QList<QRectF> &DrawingArea::rectangles() const
{
    return m_rectangles;
}

//const QList<QEllipse> &DrawingArea::circles() const
//{
//    return m_circles;
//}
