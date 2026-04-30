#pragma once
#include <QObject>
#include <QImage>
#include <QVector3D>
#include "DicomImageProvider.h"

#include <itkImageSeriesReader.h>
#include <itkImage.h>
#include <itkGDCMImageIOFactory.h>
#include <itkMinimumMaximumImageCalculator.h>

class DicomHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D crosshairPosition READ crosshairPosition WRITE setCrosshairPosition NOTIFY crosshairPositionChanged)
    Q_PROPERTY(bool volumeLoaded READ volumeLoaded NOTIFY volumeLoadedChanged)
    Q_PROPERTY(double window READ getWindow NOTIFY windowLevelChanged)
    Q_PROPERTY(double level READ getLevel NOTIFY windowLevelChanged)
    Q_PROPERTY(double lowerThreshold READ getLowerThreshold CONSTANT)
    Q_PROPERTY(double upperThreshold READ getUpperThreshold CONSTANT)

public:
    using Image3DType = itk::Image<short, 3>;
    using Image2DType = itk::Image<unsigned short, 2>;

    DicomHandler(QObject* parent = nullptr);
    ~DicomHandler();

    Q_INVOKABLE void loadDicomSeries(const QString& folderPath);
    Q_INVOKABLE QImage getSlice(int sliceIndex, int viewType); // 0=axial, 1=coronal, 2=sagittal

    QVector3D crosshairPosition() const { return m_crosshairPosition; }
    void setCrosshairPosition(const QVector3D& position);

    bool volumeLoaded() const { return m_volumeLoaded; }

    Q_INVOKABLE QVector3D getVolumeDimensions();
    Q_INVOKABLE QVector3D getVoxelSpacing();
    Q_INVOKABLE int getMaxSliceCount(int viewType);
    Q_INVOKABLE void setWindowLevel(double window, double level);

    // Threshold methods
    double getLowerThreshold() const;
    double getUpperThreshold() const;
    double getWindow() const;
    double getLevel() const;

signals:
    void crosshairPositionChanged(const QVector3D& position);
    void volumeLoadedChanged();
    void sliceUpdated(int viewType, int sliceIndex);
    void windowLevelChanged();

private:
    // Individual slice extraction methods with correct orientation
    QImage getAxialSlice(int zSlice, int width, int height);
    QImage getCoronalSlice(int ySlice, int width, int height);
    QImage getSagittalSlice(int xSlice, int width, int height);

    // Alternative simple extraction (without orientation correction)
    QImage getSliceSimple(int sliceIndex, int viewType);

    // Window/level application
    QImage applyWindowLevel(Image2DType::Pointer image, int width, int height);

    // Volume data
    typename Image3DType::Pointer m_volume;
    bool m_volumeLoaded;
    QVector3D m_crosshairPosition;

    // Window/Level parameters
    double m_window;
    double m_level;

    // Fixed thresholds (as per your QtWidgets code)
    double m_lowerThreshold;  // Fixed: -1000
    double m_upperThreshold;  // Fixed: 1000

    // Volume dimensions
    int m_width;    // X dimension
    int m_height;   // Y dimension
    int m_depth;    // Z dimension

    // Voxel spacing
    double m_spacingX;
    double m_spacingY;
    double m_spacingZ;
};
