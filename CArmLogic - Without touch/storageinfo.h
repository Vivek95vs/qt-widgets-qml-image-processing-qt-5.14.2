#ifndef STORAGEINFO_H
#define STORAGEINFO_H

#include <QObject>
#include <QImage>
#include <QPointF>

class StorageInfo : public QObject
{
    Q_OBJECT
public:
    explicit StorageInfo(QObject *parent = nullptr);
    Q_INVOKABLE QString getDriveInfo(const QString &driveLetter);
    Q_INVOKABLE QString loadImage(const QString &filePath);
    Q_INVOKABLE double calculateStenosis(double normalDiameter, double narrowedDiameter);
    Q_INVOKABLE QString analyzeVessel(const QVariantList &points);

private:
    QImage currentImage;
};

#endif // STORAGEINFO_H
