#include "DicomHandler.h"
#include <QDebug>
#include <QPainter>
//#include <QApplication>

// ITK includes
#include <itkImageSeriesReader.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkExtractImageFilter.h>
#include <itkCastImageFilter.h>

DicomHandler::DicomHandler(QObject* parent)
    : QObject(parent)
    , m_volumeLoaded(false)
    , m_crosshairPosition(0, 0, 0)
    , m_window(2000)
    , m_level(0)
    , m_width(0), m_height(0), m_depth(0)
    , m_spacingX(1.0), m_spacingY(1.0), m_spacingZ(1.0)
    , m_lowerThreshold(-1700)
    , m_upperThreshold(1800)
{
}

DicomHandler::~DicomHandler() {
}

void DicomHandler::loadDicomSeries(const QString& folderPath) {
    try {
        qDebug() << "Loading DICOM series from:" << folderPath;

        itk::GDCMImageIOFactory::RegisterOneFactory();

        using ReaderType = itk::ImageSeriesReader<Image3DType>;
        auto reader = ReaderType::New();

        auto nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->SetDirectory(folderPath.toStdString());

        const auto& seriesUIDs = nameGenerator->GetSeriesUIDs();
        if (seriesUIDs.empty()) {
            qWarning() << "No DICOM series found in folder.";
            return;
        }

        qDebug() << "Found" << seriesUIDs.size() << "series";

        const auto& filenames = nameGenerator->GetFileNames(seriesUIDs[0]);
        qDebug() << "Number of DICOM files:" << filenames.size();

        reader->SetFileNames(filenames);

        qDebug() << "Reading DICOM files...";

        try {
            reader->Update();
        } catch (const itk::ExceptionObject& e) {
            qWarning() << "ITK warning during update:" << e.GetDescription();
        }

        m_volume = reader->GetOutput();
        if (!m_volume) {
            qWarning() << "Failed to load volume";
            return;
        }

        m_volumeLoaded = true;

        auto region = m_volume->GetLargestPossibleRegion();
        m_width = region.GetSize()[0];   // X dimension
        m_height = region.GetSize()[1];  // Y dimension
        m_depth = region.GetSize()[2];   // Z dimension

        auto spacing = m_volume->GetSpacing();
        m_spacingX = spacing[0];
        m_spacingY = spacing[1];
        m_spacingZ = spacing[2];

        // Set initial crosshair to center of volume
        m_crosshairPosition.setX(m_width / 2);
        m_crosshairPosition.setY(m_height / 2);
        m_crosshairPosition.setZ(m_depth / 2);

        qDebug() << "Volume dimensions - X:" << m_width << "Y:" << m_height << "Z:" << m_depth;
        qDebug() << "Voxel spacing:" << m_spacingX << m_spacingY << m_spacingZ;
        qDebug() << "Fixed thresholds: lower=" << m_lowerThreshold << "upper=" << m_upperThreshold;

        emit volumeLoadedChanged();
        emit crosshairPositionChanged(m_crosshairPosition);

        // Force update of all slice images
//        for (int i = 0; i < 3; i++) {
//            emit sliceUpdated(i, 0);
//        }

        emit sliceUpdated(0, static_cast<int>(m_crosshairPosition.z())); // Axial
        emit sliceUpdated(1, static_cast<int>(m_crosshairPosition.y())); // Coronal
        emit sliceUpdated(2, static_cast<int>(m_crosshairPosition.x())); // Sagittal

        qDebug() << "DICOM loading complete";

    } catch (const itk::ExceptionObject& e) {
        qCritical() << "ITK Error:" << e.GetDescription();
    } catch (const std::exception& e) {
        qCritical() << "Standard Exception:" << e.what();
    }
}

QImage DicomHandler::getSlice(int sliceIndex, int viewType) {
    if (!m_volumeLoaded || !m_volume) {
        return QImage();
    }

    try {
        using ExtractFilterType = itk::ExtractImageFilter<Image3DType, Image2DType>;

        auto region = m_volume->GetLargestPossibleRegion();

        // Create extraction region based on view type
        Image3DType::RegionType extractRegion;
        Image3DType::IndexType start;
        Image3DType::SizeType size;

        int outputWidth = 0;
        int outputHeight = 0;

        switch(viewType) {
            case 0: { // Axial (XY plane at Z position)
                if (sliceIndex < 0 || sliceIndex >= m_depth) return QImage();
                start[0] = 0; start[1] = 0; start[2] = sliceIndex;
                size[0] = m_width; size[1] = m_height; size[2] = 0;
                outputWidth = m_width;
                outputHeight = m_height;
                return getAxialSlice(sliceIndex, outputWidth, outputHeight);
            }
            case 1: { // Coronal/Frontal (XZ plane at Y position)
                if (sliceIndex < 0 || sliceIndex >= m_height) return QImage();
                start[0] = 0; start[1] = sliceIndex; start[2] = 0;
                size[0] = m_width; size[1] = 0; size[2] = m_depth;
                outputWidth = m_width;
                outputHeight = m_depth;
                return getCoronalSlice(sliceIndex, outputWidth, outputHeight);
            }
            case 2: { // Sagittal (YZ plane at X position)
                if (sliceIndex < 0 || sliceIndex >= m_width) return QImage();
                start[0] = sliceIndex; start[1] = 0; start[2] = 0;
                size[0] = 0; size[1] = m_height; size[2] = m_depth;
                outputWidth = m_height;
                outputHeight = m_depth;
                return getSagittalSlice(sliceIndex, outputWidth, outputHeight);
            }
            default:
                return QImage();
        }

    } catch (const itk::ExceptionObject& e) {
        qWarning() << "Error extracting slice:" << e.GetDescription();
        return QImage();
    }
}

QImage DicomHandler::getAxialSlice(int zSlice, int width, int height) {
    // Create 8-bit grayscale image
    QImage qimg(width, height, QImage::Format_Grayscale8);

    double lowerThreshold = m_lowerThreshold;
    double upperThreshold = m_upperThreshold;
    double windowRange = upperThreshold - lowerThreshold;
    if (windowRange <= 0) windowRange = 1.0;

    // Axial view - similar to your code but without patient setup checks
    for (int y = 0; y < height; ++y) {
        uchar* line = qimg.scanLine(y);
        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType idx;
            idx[0] = x;          // X coordinate
            idx[1] = y;          // Y coordinate
            idx[2] = zSlice;     // Z slice

            double pixelValue = m_volume->GetPixel(idx);

            if (pixelValue <= lowerThreshold) {
                line[x] = 0;
            } else if (pixelValue >= upperThreshold) {
                line[x] = 255;
            } else {
                double normalized = ((pixelValue - lowerThreshold) * 255.0) / windowRange;
                line[x] = static_cast<uchar>(qBound(0.0, normalized, 255.0));
            }
        }
    }

    return qimg;
}

QImage DicomHandler::getCoronalSlice(int ySlice, int width, int height) {
    // Create 8-bit grayscale image
    QImage qimg(width, height, QImage::Format_Grayscale8);

    double lowerThreshold = m_lowerThreshold;
    double upperThreshold = m_upperThreshold;
    double windowRange = upperThreshold - lowerThreshold;
    if (windowRange <= 0) windowRange = 1.0;

    // Fill image with FLIPPED data directly
    for (int z = 0; z < height; ++z) {
        // Calculate FLIPPED row position (vertical flip)
        int targetZ = height - 1 - z;     // Flip vertically

        // Get the line at FLIPPED position
//        uchar* line = qimg.scanLine(targetZ);  // Write to flipped row
         uchar* line = qimg.scanLine(targetZ);  // Write to flipped row


         // Read from REVERSE Z position
//         int sourceZ = height - 1 - z;  // This reverses the order

        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType idx;
            idx[0] = x;                // X coordinate
            idx[1] = 512-ySlice;           // Y slice (fixed for coronal)
            idx[2] = z;                // Z coordinate (ORIGINAL, not flipped!)

            double pixelValue = m_volume->GetPixel(idx);

            // Calculate FLIPPED column position (horizontal flip)
            int targetX = width - 1 - x;      // Flip horizontally

            // Apply window/level and write to FLIPPED position
            if (pixelValue <= lowerThreshold) {
                line[targetX] = 0;
            } else if (pixelValue >= upperThreshold) {
                line[targetX] = 255;
            } else {
                double normalized = ((pixelValue - lowerThreshold) * 255.0) / windowRange;
                line[targetX] = static_cast<uchar>(qBound(0.0, normalized, 255.0));
            }
        }
    }

    return qimg;
}

QImage DicomHandler::getSagittalSlice(int xSlice, int width, int height) {
    // Create 8-bit grayscale image
    QImage qimg(width, height, QImage::Format_Grayscale8);

    double lowerThreshold = m_lowerThreshold;
    double upperThreshold = m_upperThreshold;
    double windowRange = upperThreshold - lowerThreshold;
    if (windowRange <= 0) windowRange = 1.0;

    // Fill image with FLIPPED data directly
    for (int z = 0; z < height; ++z) {
        // Calculate FLIPPED row position (vertical flip)
        int targetZ = height - 1 - z;     // Flip vertically

        // Get the line at FLIPPED position
        uchar* line = qimg.scanLine(targetZ);  // Write to flipped row

        for (int y = 0; y < width; ++y) {
            Image3DType::IndexType idx;
            idx[0] = xSlice;           // X slice (fixed for sagittal)
            idx[1] = y;                // Y coordinate
            idx[2] = z;                // Z coordinate (ORIGINAL, not flipped!)

            double pixelValue = m_volume->GetPixel(idx);

            // NO horizontal flip for sagittal
            // Write to CORRECT position (targetZ, y)
            if (pixelValue <= lowerThreshold) {
                line[y] = 0;  // Write to flipped row, original column
            } else if (pixelValue >= upperThreshold) {
                line[y] = 255;
            } else {
                double normalized = ((pixelValue - lowerThreshold) * 255.0) / windowRange;
                line[y] = static_cast<uchar>(qBound(0.0, normalized, 255.0));
            }
        }
    }

    return qimg;
}

// Alternative simpler version without flipping in C++ (flip in QML)
QImage DicomHandler::getSliceSimple(int sliceIndex, int viewType) {
    if (!m_volumeLoaded || !m_volume) {
        return QImage();
    }

    try {
        using ExtractFilterType = itk::ExtractImageFilter<Image3DType, Image2DType>;

        auto region = m_volume->GetLargestPossibleRegion();

        // Create extraction region based on view type
        Image3DType::RegionType extractRegion;
        Image3DType::IndexType start;
        Image3DType::SizeType size;

        int outputWidth = 0;
        int outputHeight = 0;

        switch(viewType) {
            case 0: { // Axial
                if (sliceIndex < 0 || sliceIndex >= m_depth) return QImage();
                start[0] = 0; start[1] = 0; start[2] = sliceIndex;
                size[0] = m_width; size[1] = m_height; size[2] = 0;
                outputWidth = m_width;
                outputHeight = m_height;
                break;
            }
            case 1: { // Coronal
                if (sliceIndex < 0 || sliceIndex >= m_height) return QImage();
                start[0] = 0; start[1] = sliceIndex; start[2] = 0;
                size[0] = m_width; size[1] = 0; size[2] = m_depth;
                outputWidth = m_width;
                outputHeight = m_depth;
                break;
            }
            case 2: { // Sagittal
                if (sliceIndex < 0 || sliceIndex >= m_width) return QImage();
                start[0] = sliceIndex; start[1] = 0; start[2] = 0;
                size[0] = 0; size[1] = m_height; size[2] = m_depth;
                outputWidth = m_height;
                outputHeight = m_depth;
                break;
            }
            default:
                return QImage();
        }

        extractRegion.SetIndex(start);
        extractRegion.SetSize(size);

        auto extractFilter = ExtractFilterType::New();
        extractFilter->SetInput(m_volume);
        extractFilter->SetExtractionRegion(extractRegion);
        extractFilter->SetDirectionCollapseToIdentity();
        extractFilter->Update();

        auto extractedImage = extractFilter->GetOutput();

        // Apply window/level
        return applyWindowLevel(extractedImage, outputWidth, outputHeight);

    } catch (const itk::ExceptionObject& e) {
        qWarning() << "Error extracting slice:" << e.GetDescription();
        return QImage();
    }
}

QImage DicomHandler::applyWindowLevel(Image2DType::Pointer image, int width, int height) {
    if (!image || width <= 0 || height <= 0) {
        return QImage();
    }

    QImage qimg(width, height, QImage::Format_Grayscale8);

    double lowerThreshold = m_lowerThreshold;
    double upperThreshold = m_upperThreshold;
    double windowRange = upperThreshold - lowerThreshold;
    if (windowRange <= 0) windowRange = 1.0;

    for (int y = 0; y < height; ++y) {
        uchar* line = qimg.scanLine(y);
        for (int x = 0; x < width; ++x) {
            Image2DType::IndexType idx;
            idx[0] = x;
            idx[1] = y;

            double pixelValue = image->GetPixel(idx);

            if (pixelValue <= lowerThreshold) {
                line[x] = 0;
            } else if (pixelValue >= upperThreshold) {
                line[x] = 255;
            } else {
                double normalized = ((pixelValue - lowerThreshold) * 255.0) / windowRange;
                line[x] = static_cast<uchar>(qBound(0.0, normalized, 255.0));
            }
        }
    }

    return qimg;
}

void DicomHandler::setCrosshairPosition(const QVector3D& position) {
    QVector3D newPos;
    newPos.setX(qBound(0.0f, position.x(), static_cast<float>(m_width - 1)));
    newPos.setY(qBound(0.0f, position.y(), static_cast<float>(m_height - 1)));
    newPos.setZ(qBound(0.0f, position.z(), static_cast<float>(m_depth - 1)));

    if (m_crosshairPosition != newPos) {
        m_crosshairPosition = newPos;
        emit crosshairPositionChanged(m_crosshairPosition);

        // Update slice indices
        emit sliceUpdated(0, static_cast<int>(m_crosshairPosition.z())); // Axial
        emit sliceUpdated(1, static_cast<int>(m_crosshairPosition.y())); // Coronal
        emit sliceUpdated(2, static_cast<int>(m_crosshairPosition.x())); // Sagittal
    }
}

QVector3D DicomHandler::getVolumeDimensions() {
    return QVector3D(m_width, m_height, m_depth);
}

QVector3D DicomHandler::getVoxelSpacing() {
    return QVector3D(m_spacingX, m_spacingY, m_spacingZ);
}

int DicomHandler::getMaxSliceCount(int viewType) {
    if (!m_volumeLoaded) return 0;

    switch(viewType) {
        case 0: return m_depth;  // Axial
        case 1: return m_height; // Coronal
        case 2: return m_width;  // Sagittal
        default: return 0;
    }
}

void DicomHandler::setWindowLevel(double window, double level) {
    m_window = window;
    m_level = level;

    // Force update of all slices
    if (m_volumeLoaded) {
        emit sliceUpdated(0, static_cast<int>(m_crosshairPosition.z()));
        emit sliceUpdated(1, static_cast<int>(m_crosshairPosition.y()));
        emit sliceUpdated(2, static_cast<int>(m_crosshairPosition.x()));
    }
}

double DicomHandler::getLowerThreshold() const {
    return m_lowerThreshold;
}

double DicomHandler::getUpperThreshold() const {
    return m_upperThreshold;
}

double DicomHandler::getWindow() const {
    return m_window;
}

double DicomHandler::getLevel() const {
    return m_level;
}
