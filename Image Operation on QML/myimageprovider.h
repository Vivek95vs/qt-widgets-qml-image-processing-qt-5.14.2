#ifndef MYIMAGEPROVIDER_H
#define MYIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMap>
#include <QFile>
#include <QDebug>

class MyImageProvider : public QQuickImageProvider {
public:
    MyImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void addRawImage(const QString &imageId, const QString &filePath, int width, int height);
    bool hasImage(const QString &id) const;
    QImage getImage(const QString &id) const;
    void addImage(const QString &id, const QImage &image);

private:
    QMap<QString, QImage> imageMap;
    QImage loadRawImage(const QString &filePath, int width, int height);
};

#endif // MYIMAGEPROVIDER_H

