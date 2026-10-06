
#include "dicomimageprovider.h"
#include "dicomloader.h"


#include <QDebug>


DicomImageProvider::DicomImageProvider(
        DicomLoader *dicomLoader)
    : QQuickImageProvider(
          QQuickImageProvider::Image),
      m_dicomLoader(dicomLoader)
{
}

QImage DicomImageProvider::requestImage(
        const QString &id,
        QSize *size,
        const QSize &requestedSize)
{
    Q_UNUSED(id)
    Q_UNUSED(requestedSize)

    // ---------------------------------------------------------
    // CHECK LOADER
    // ---------------------------------------------------------

    if (!m_dicomLoader)
    {
        qDebug()
                << "DicomImageProvider:"
                << "DicomLoader is null";

        return QImage();
    }

    // ---------------------------------------------------------
    // GET PROCESSED PREVIEW IMAGE
    // ---------------------------------------------------------

    const QImage image =
            m_dicomLoader->previewImage();

    // ---------------------------------------------------------
    // CHECK IMAGE
    // ---------------------------------------------------------

    if (image.isNull())
    {
        qDebug()
                << "DicomImageProvider:"
                << "Preview image is empty";

        return QImage();
    }

    // ---------------------------------------------------------
    // DEBUG FORMAT
    // ---------------------------------------------------------

    qDebug()
            << "DicomImageProvider format:"
            << image.format();

    // ---------------------------------------------------------
    // RETURN IMAGE SIZE
    // ---------------------------------------------------------

    if (size)
    {
        *size = image.size();
    }

    // ---------------------------------------------------------
    // RETURN PROCESSED 16-BIT PREVIEW
    // ---------------------------------------------------------

    return image;
}
