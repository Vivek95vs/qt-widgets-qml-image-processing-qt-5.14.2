#include "storageinfo.h"
#include <QStorageInfo>
#include <QFileInfo>
#include <QDebug>
#include <QPainter>
#include <cmath>

StorageInfo::StorageInfo(QObject *parent)
    : QObject(parent)
{
}

QString StorageInfo::getDriveInfo(const QString &driveLetter)
{
    QStorageInfo storage(driveLetter + ":/");
    qint64 total = storage.bytesTotal();
    qint64 free = storage.bytesFree();
    qint64 used = total - free;

    QString info = QString("Drive %1:\nTotal: %2 GB\nUsed: %3 GB\nFree: %4 GB")
            .arg(driveLetter)
            .arg(total / (1024 * 1024 * 1024))
            .arg(used / (1024 * 1024 * 1024))
            .arg(free / (1024 * 1024 * 1024));
    return info;
}

QString StorageInfo::loadImage(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        return "Error: File does not exist";
    }

    if (!currentImage.load(filePath)) {
        return "Error: Failed to load image";
    }

    return QString("Image loaded successfully: %1x%2 pixels")
            .arg(currentImage.width())
            .arg(currentImage.height());
}

double StorageInfo::calculateStenosis(double normalDiameter, double narrowedDiameter)
{
    if (normalDiameter <= 0) return 0.0;

    double stenosisPercentage = ((normalDiameter - narrowedDiameter) / normalDiameter) * 100.0;
    return qMax(0.0, qMin(100.0, stenosisPercentage)); // Clamp between 0-100%
}

QString StorageInfo::analyzeVessel(const QVariantList &points)
{
    if (points.size() < 4) {
        return "Error: Need at least 2 points for normal diameter and 2 points for narrowed diameter";
    }

    // Calculate normal diameter (first two points)
    QPointF p1(points[0].toPointF());
    QPointF p2(points[1].toPointF());
    double normalDiameter = std::sqrt(std::pow(p2.x() - p1.x(), 2) + std::pow(p2.y() - p1.y(), 2));

    // Calculate narrowed diameter (next two points)
    QPointF p3(points[2].toPointF());
    QPointF p4(points[3].toPointF());
    double narrowedDiameter = std::sqrt(std::pow(p4.x() - p3.x(), 2) + std::pow(p4.y() - p3.y(), 2));

    double stenosis = calculateStenosis(normalDiameter, narrowedDiameter);

    return QString("Stenosis Analysis:\nNormal Diameter: %1 pixels\nNarrowed Diameter: %2 pixels\nStenosis: %3%")
            .arg(normalDiameter, 0, 'f', 2)
            .arg(narrowedDiameter, 0, 'f', 2)
            .arg(stenosis, 0, 'f', 2);
}
