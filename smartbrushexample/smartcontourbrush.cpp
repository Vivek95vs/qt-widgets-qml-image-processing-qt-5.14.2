#include "smartcontourbrush.h"
#include <QDebug>
#include <cmath>
#include <QVariantMap>

SmartContourBrush::SmartContourBrush(QObject *parent)
    : QObject(parent)
    , m_isActive(false)
    , m_isErasing(false)
    , m_currentContourIndex(-1)
    , m_brushSize(25.0)
    , m_brushColor(Qt::green)
{
}

double SmartContourBrush::brushSize() const
{
    return m_brushSize;
}

void SmartContourBrush::setBrushSize(double size)
{
    if (qFuzzyCompare(m_brushSize, size))
        return;

    m_brushSize = size;
    emit brushSizeChanged();
}

QColor SmartContourBrush::brushColor() const
{
    return m_brushColor;
}

void SmartContourBrush::setBrushColor(const QColor &color)
{
    if (m_brushColor == color)
        return;

    m_brushColor = color;
    emit brushColorChanged();
}

void SmartContourBrush::startNewContour(qreal x, qreal y)
{
    m_isActive = true;
    m_isErasing = false;

    // Create a new contour
    Contour newContour;
    newContour.isActive = true;
    newContour.lastPoint = QPointF(x, y);

    // Start with a circle at the first point
    QPainterPath circlePath = createCirclePath(newContour.lastPoint, m_brushSize);
    newContour.path = circlePath;

    m_contours.append(newContour);
    m_currentContourIndex = m_contours.size() - 1;

    updatePoints();
    emit activeChanged();
}

void SmartContourBrush::updateCurrentContour(qreal x, qreal y)
{
    if (!m_isActive || m_isErasing || m_currentContourIndex < 0)
        return;

    QPointF currentPos(x, y);
    Contour &currentContour = m_contours[m_currentContourIndex];

    // Check distance moved
    double dx = currentPos.x() - currentContour.lastPoint.x();
    double dy = currentPos.y() - currentContour.lastPoint.y();
    double distance = sqrt(dx * dx + dy * dy);

    // Only update if moved enough
    if (distance > m_brushSize * 0.3) {
        addPointToCurrentContour(currentPos);
        currentContour.lastPoint = currentPos;

        // Update display less frequently for performance
        static int updateCounter = 0;
        if (++updateCounter % 3 == 0) {
            updatePoints();
            updateCounter = 0;
        }
    }
}

void SmartContourBrush::finishCurrentContour()
{
    if (!m_isActive || m_isErasing)
        return;

    m_isActive = false;
    if (m_currentContourIndex >= 0) {
        m_contours[m_currentContourIndex].isActive = false;
    }
    m_currentContourIndex = -1;

    mergeCloseContours();
    updatePoints();
    emit activeChanged();
}

void SmartContourBrush::startErase(qreal x, qreal y)
{
    m_isActive = true;
    m_isErasing = true;

    removeFromAllContours(QPointF(x, y));
    emit activeChanged();
}

void SmartContourBrush::updateErase(qreal x, qreal y)
{
    if (!m_isActive || !m_isErasing)
        return;

    QPointF currentPos(x, y);

    // Check if moved enough
    static QPointF lastErasePos;
    double dx = currentPos.x() - lastErasePos.x();
    double dy = currentPos.y() - lastErasePos.y();
    double distance = sqrt(dx * dx + dy * dy);

    if (distance > m_brushSize * 0.5) {
        removeFromAllContours(currentPos);
        lastErasePos = currentPos;

        // Update display less frequently
        static int updateCounter = 0;
        if (++updateCounter % 5 == 0) {
            updatePoints();
            updateCounter = 0;
        }
    }
}

void SmartContourBrush::finishErase()
{
    if (!m_isActive || !m_isErasing)
        return;

    m_isActive = false;
    updatePoints();
    emit activeChanged();
}

void SmartContourBrush::clear()
{
    m_contours.clear();
    m_contourPoints.clear();
    m_isActive = false;
    m_currentContourIndex = -1;

    updatePoints();
    emit activeChanged();
}

QPainterPath SmartContourBrush::createCirclePath(const QPointF &center, double radius)
{
    QPainterPath circlePath;
    circlePath.addEllipse(center, radius, radius);
    return circlePath;
}

void SmartContourBrush::addPointToCurrentContour(const QPointF &point)
{
    if (m_currentContourIndex < 0 || m_brushSize <= 0)
        return;

    Contour &contour = m_contours[m_currentContourIndex];
    QPainterPath circlePath = createCirclePath(point, m_brushSize);

    if (contour.path.isEmpty()) {
        contour.path = circlePath;
    } else {
        contour.path = contour.path.united(circlePath);
    }
}

void SmartContourBrush::removeFromAllContours(const QPointF &center)
{
    if (m_brushSize <= 0)
        return;

    QPainterPath eraseCircle = createCirclePath(center, m_brushSize);

    // Remove from all contours
    for (int i = 0; i < m_contours.size(); ++i) {
        if (!m_contours[i].path.isEmpty()) {
            m_contours[i].path = m_contours[i].path.subtracted(eraseCircle);

            // Remove empty contours
            if (m_contours[i].path.isEmpty()) {
                m_contours.removeAt(i);
                i--;

                // Update current contour index if needed
                if (m_currentContourIndex == i) {
                    m_currentContourIndex = -1;
                } else if (m_currentContourIndex > i) {
                    m_currentContourIndex--;
                }
            }
        }
    }
}

void SmartContourBrush::updatePoints()
{
    m_contourPoints.clear();

    // Combine all contours into one list of points
    for (const Contour &contour : m_contours) {
        if (!contour.path.isEmpty()) {
            // Add separator marker between contours
            QVariantMap separator;
            separator["x"] = -1000.0;  // Special marker value
            separator["y"] = -1000.0;
            m_contourPoints.append(separator);

            // Add points from this contour
            for (int i = 0; i < contour.path.elementCount(); ++i) {
                QPainterPath::Element element = contour.path.elementAt(i);
                if (element.type == QPainterPath::MoveToElement ||
                    element.type == QPainterPath::LineToElement) {
                    QVariantMap pointMap;
                    pointMap["x"] = element.x;
                    pointMap["y"] = element.y;
                    m_contourPoints.append(pointMap);
                }
            }
        }
    }

    emit contourChanged();
}

void SmartContourBrush::mergeCloseContours()
{
    // Optional: Merge contours that are very close to each other
    // This prevents having many tiny separate contours
    // Implement if needed
}

double SmartContourBrush::calculateArea() const
{
    double totalArea = 0.0;
    for (const Contour &contour : m_contours) {
        totalArea += contour.path.boundingRect().width() *
                    contour.path.boundingRect().height() * 0.7;
    }
    return totalArea;
}
