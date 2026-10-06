#ifndef DICOMIMAGEPROVIDER_H
#define DICOMIMAGEPROVIDER_H


#include <QQuickImageProvider>
#include <QImage>
#include <QSize>


class DicomLoader;

class DicomImageProvider : public QQuickImageProvider
{
public:
    explicit DicomImageProvider(
            DicomLoader *dicomLoader);

    QImage requestImage(
            const QString &id,
            QSize *size,
            const QSize &requestedSize) override;

private:
    DicomLoader *m_dicomLoader;
};

#endif // DICOMIMAGEPROVIDER_H



