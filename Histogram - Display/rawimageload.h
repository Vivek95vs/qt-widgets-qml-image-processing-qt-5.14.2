#ifndef RAWIMAGELOAD_H
#define RAWIMAGELOAD_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>
#include <QVector>

class RawImageLoad : public QObject, public QQuickImageProvider {
    Q_OBJECT
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)
    Q_PROPERTY(QVector<int> histogramData READ histogramData NOTIFY histogramUpdated)

public:
    explicit RawImageLoad(QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    Q_INVOKABLE void loadImage();

    bool imageLoaded() const;
    QVector<int> histogramData() const; // Getter for histogram data

signals:
    void imageLoadedChanged();
    void histogramUpdated();

private:
    QImage loadRawImage();
    void calculateHistogram();

    unsigned char* arr;
    QImage myImage;
    bool m_imageLoaded;
    QVector<int> m_histogram;
};

#endif // RAWIMAGELOAD_H
