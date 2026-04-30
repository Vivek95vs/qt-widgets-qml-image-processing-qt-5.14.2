#pragma once
#include <QObject>
#include <QImage>
#include <QStringList>
#include <QVector3D>
#include "dicomimageprovider.h"

#include <itkImageSeriesReader.h>
#include <itkImage.h>
#include <itkGDCMImageIOFactory.h>
#include <itkExtractImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

class DicomHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D volumeDimensions READ volumeDimensions NOTIFY volumeLoaded)
    Q_PROPERTY(QVector3D volumeSpacing READ volumeSpacing NOTIFY volumeLoaded)
    Q_PROPERTY(int sliceCount READ sliceCount NOTIFY volumeLoaded)
    Q_PROPERTY(int currentAxialSlice READ currentAxialSlice WRITE setCurrentAxialSlice NOTIFY currentAxialSliceChanged)
    Q_PROPERTY(int currentCoronalSlice READ currentCoronalSlice WRITE setCurrentCoronalSlice NOTIFY currentCoronalSliceChanged)
    Q_PROPERTY(int currentSagittalSlice READ currentSagittalSlice WRITE setCurrentSagittalSlice NOTIFY currentSagittalSliceChanged)
    Q_PROPERTY(bool volumeLoaded READ isVolumeLoaded NOTIFY volumeLoaded)

public:
    using Image3DType = itk::Image<unsigned short, 3>;
    using Image2DType = itk::Image<unsigned short, 2>;

    DicomHandler(QObject* parent = nullptr);
    ~DicomHandler();

    QVector3D volumeDimensions() const { return m_dimensions; }
    QVector3D volumeSpacing() const { return m_spacing; }
    int sliceCount() const { return m_sliceCount; }
    bool isVolumeLoaded() const { return m_volumeLoaded; }

    int currentAxialSlice() const { return m_currentAxialSlice; }
    int currentCoronalSlice() const { return m_currentCoronalSlice; }
    int currentSagittalSlice() const { return m_currentSagittalSlice; }

    void setCurrentAxialSlice(int slice);
    void setCurrentCoronalSlice(int slice);
    void setCurrentSagittalSlice(int slice);

    Q_INVOKABLE void loadDicomSeries();
    Q_INVOKABLE void updateAllSlices();

    DicomImageProvider* axialImageProvider() { return m_axialProvider; }
    DicomImageProvider* coronalImageProvider() { return m_coronalProvider; }
    DicomImageProvider* sagittalImageProvider() { return m_sagittalProvider; }

signals:
    void volumeLoaded();
    void currentAxialSliceChanged();
    void currentCoronalSliceChanged();
    void currentSagittalSliceChanged();
    void axialImageChanged();
    void coronalImageChanged();
    void sagittalImageChanged();

private:
    QImage extractSlice(int plane, int sliceIndex);
    QImage convertITKToQImage(Image2DType::Pointer image);

    typename Image3DType::Pointer m_volume;
    QVector3D m_dimensions;
    QVector3D m_spacing;
    int m_sliceCount;
    bool m_volumeLoaded;

    int m_currentAxialSlice;
    int m_currentCoronalSlice;
    int m_currentSagittalSlice;

    DicomImageProvider* m_axialProvider;
    DicomImageProvider* m_coronalProvider;
    DicomImageProvider* m_sagittalProvider;
};
