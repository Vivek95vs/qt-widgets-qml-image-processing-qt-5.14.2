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
    Q_PROPERTY(int windowWidth READ windowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowLevel READ windowLevel NOTIFY windowLevelChanged)

public:
    explicit RawImageLoad(QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    Q_INVOKABLE void loadImage();

    bool imageLoaded() const;
    QVector<int> histogramData() const;
    int windowWidth() const;
    int windowLevel() const;

signals:
    void imageLoadedChanged();
    void histogramUpdated();
    void windowWidthChanged();
    void windowLevelChanged();
    void imageUpdated();

private:
    QImage loadRawImage();
    void calculateHistogram();
    void computeWindowFromHistogram();
    void applyWindow();

    unsigned char* arr;
    QImage myImage;
    bool m_imageLoaded;
    QVector<int> m_histogram;
    int m_windowWidth;
    int m_windowLevel;
};

#endif // RAWIMAGELOAD_H
