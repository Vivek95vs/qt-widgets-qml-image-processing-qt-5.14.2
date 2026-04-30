#ifndef IMAGEMAGNIFIER_H
#define IMAGEMAGNIFIER_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>

class ImageMagnifier : public QObject, public QQuickImageProvider {
    Q_OBJECT
public:
    explicit ImageMagnifier(QObject *parent = nullptr);

    // Override requestImage to handle both main image and ROI requests
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    // Function to update the ROI image
    Q_INVOKABLE void updateROI(int x, int y, int width, int height, float zoomLevel);

signals:
    void roiImageUpdated(); // Signal to notify QML that the ROI image has changed

private:
    QImage loadRawImage();
    QImage _roiImage; // Stores the current magnified ROI image
    unsigned char* arr;
    QImage myImage;
};

#endif // IMAGEMAGNIFIER_H
