#ifndef DICOMHANDLER_H
#define DICOMHANDLER_H

#include <QObject>
#include <QImage>
#include <QVector3D>
#include <QByteArray>

class DicomImageProvider;

// Forward declare ITK types to avoid including ITK headers here
namespace itk {
    template<unsigned int VDimension> class Image;
    typedef Image<3> Image3DType;
}

class DicomHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray volumeData READ volumeData NOTIFY volumeDataChanged)
    Q_PROPERTY(QVector3D volumeDimensions READ volumeDimensions NOTIFY volumeDataChanged)
    Q_PROPERTY(QVector3D volumeSpacing READ volumeSpacing NOTIFY volumeDataChanged)
    Q_PROPERTY(bool volumeLoaded READ volumeLoaded NOTIFY volumeLoadedChanged)

public:
    explicit DicomHandler(DicomImageProvider* provider, QObject* parent = nullptr);
    ~DicomHandler();

    Q_INVOKABLE void loadDicomSeries(const QString& folderPath = "");
    Q_INVOKABLE void showSlice(int sliceIndex);
    Q_INVOKABLE void nextSlice();
    Q_INVOKABLE void prevSlice();
    Q_INVOKABLE void setWindowLevel(float window, float level);

    QByteArray volumeData() const;
    QVector3D volumeDimensions() const;
    QVector3D volumeSpacing() const;
    bool volumeLoaded() const;

    Q_INVOKABLE int getCurrentSlice() const { return m_currentSlice; }
    Q_INVOKABLE int getTotalSlices() const { return m_totalSlices; }

signals:
    void volumeDataChanged();
    void volumeLoadedChanged();
    void sliceChanged(int sliceIndex);
    void errorOccurred(const QString& message);
    void loadingStarted();
    void loadingFinished();

private:
    void loadDicomFromFolder(const QString& folderPath);
    void updateCurrentSlice();
    QImage convertSliceToImage(int sliceIndex);

    DicomImageProvider* m_provider;
    itk::Image3DType* m_volume;
    QByteArray m_volumeData;
    QVector3D m_dimensions;
    QVector3D m_spacing;

    int m_currentSlice;
    int m_totalSlices;
    float m_windowWidth;
    float m_windowLevel;
    bool m_isVolumeLoaded;

    short m_minIntensity;
    short m_maxIntensity;
};

#endif // DICOMHANDLER_H
