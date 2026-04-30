#pragma once
#include <QObject>
#include <QImage>
#include <QString>
#include <QVector3D>
#include <QVector>
#include "DicomImageProvider.h"

#include <itkImageSeriesReader.h>
#include <itkImage.h>
#include <itkGDCMImageIOFactory.h>
#include <itkMinimumMaximumImageCalculator.h>

class DicomHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(int sliceCount READ sliceCount NOTIFY volumeLoaded)
    Q_PROPERTY(QString seriesName READ seriesName NOTIFY volumeLoaded)
    Q_PROPERTY(QVector3D volumeDimensions READ volumeDimensions NOTIFY volumeLoaded)
    Q_PROPERTY(double windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowChanged)
    Q_PROPERTY(double windowCenter READ windowCenter WRITE setWindowCenter NOTIFY windowChanged)

public:
    using Image3DType = itk::Image<short, 3>;
    using Image2DType = itk::Image<short, 2>;

    DicomHandler(DicomImageProvider* provider, QObject* parent = nullptr);

    Q_INVOKABLE void loadDicomSeries(const QString& folderPath);
    Q_INVOKABLE void showSlice(int index);
    Q_INVOKABLE QImage getSliceImage(int index);
    Q_INVOKABLE void saveCurrentSlice(const QString& filePath);
    Q_INVOKABLE void extractVolumeData();

    int sliceCount() const { return m_sliceCount; }
    QString seriesName() const { return m_seriesName; }
    QVector3D volumeDimensions() const { return m_volumeDimensions; }
    double windowWidth() const { return m_windowWidth; }
    double windowCenter() const { return m_windowCenter; }

    void setWindowWidth(double width);
    void setWindowCenter(double center);

    Image3DType::Pointer getVolume() const { return m_volume; }
    QVector<short> getVolumeData() const { return m_volumeData; }

signals:
    void volumeLoaded();
    void sliceChanged(int index);
    void windowChanged();
    void errorOccurred(const QString& message);

private:
    DicomImageProvider* m_provider;
    Image3DType::Pointer m_volume;
    QVector<short> m_volumeData;
    int m_sliceCount;
    QString m_seriesName;
    QVector3D m_volumeDimensions;
    double m_windowWidth;
    double m_windowCenter;
    short m_minIntensity;
    short m_maxIntensity;

    void applyWindowLevel(QImage& image, short* sliceData, int width, int height);
};
