#include "DicomHandler.h"
#include "DicomImageProvider.h"

// ITK includes
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImage.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkFlipImageFilter.h>

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QPainter>

// Define ITK image types
typedef short PixelType;
typedef itk::Image<PixelType, 3> Image3DType;
typedef itk::Image<PixelType, 2> Image2DType;

DicomHandler::DicomHandler(DicomImageProvider* provider, QObject* parent)
    : QObject(parent)
    , m_provider(provider)
    , m_volume(nullptr)
    , m_currentSlice(0)
    , m_totalSlices(0)
    , m_windowWidth(2000.0f)
    , m_windowLevel(500.0f)
    , m_isVolumeLoaded(false)
    , m_minIntensity(0)
    , m_maxIntensity(0)
{
    qDebug() << "DicomHandler created";
}

DicomHandler::~DicomHandler()
{
    if (m_volume) {
        m_volume->Delete();
        m_volume = nullptr;
    }
}

void DicomHandler::loadDicomSeries(const QString& folderPath)
{
    emit loadingStarted();
    qDebug() << "Loading DICOM series from:" << folderPath;

    QString path = folderPath;
    if (path.isEmpty()) {
        // Use hardcoded path for testing
        path = "D:/KCH site/Head_neck/Without_Norm/DailyImages/DICOM/CBCT";
        qDebug() << "Using default path:" << path;
    }

    loadDicomFromFolder(path);
}

void DicomHandler::loadDicomFromFolder(const QString& folderPath)
{
    try {
        // Register DICOM IO factory
        itk::GDCMImageIOFactory::RegisterOneFactory();

        // Create DICOM series reader
        typedef itk::ImageSeriesReader<Image3DType> ReaderType;
        ReaderType::Pointer reader = ReaderType::New();

        // Generate DICOM series file names
        itk::GDCMSeriesFileNames::Pointer nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->SetDirectory(folderPath.toStdString());

        // Get series UIDs
        const itk::GDCMSeriesFileNames::SeriesUIDContainerType& seriesUIDs = nameGenerator->GetSeriesUIDs();

        if (seriesUIDs.empty()) {
            emit errorOccurred("No DICOM series found in folder: " + folderPath);
            qWarning() << "No DICOM series found";
            emit loadingFinished();
            return;
        }

        qDebug() << "Found" << seriesUIDs.size() << "DICOM series";

        // Use first series
        std::string seriesIdentifier = seriesUIDs.begin()->c_str();
        qDebug() << "Using series:" << QString::fromStdString(seriesIdentifier);

        // Get file names for this series
        const itk::GDCMSeriesFileNames::FileNamesContainerType& fileNames = nameGenerator->GetFileNames(seriesIdentifier);
        qDebug() << "Number of DICOM files:" << fileNames.size();

        reader->SetFileNames(fileNames);

        // Update and read volume
        reader->Update();
        m_volume = reader->GetOutput();
        m_volume->Register(); // Take ownership

        // Get volume information
        Image3DType::RegionType region = m_volume->GetLargestPossibleRegion();
        Image3DType::SizeType size = region.GetSize();
        Image3DType::SpacingType spacing = m_volume->GetSpacing();

        m_dimensions = QVector3D(size[0], size[1], size[2]);
        m_spacing = QVector3D(spacing[0], spacing[1], spacing[2]);
        m_totalSlices = size[2];
        m_currentSlice = m_totalSlices / 2;

        qDebug() << "Volume loaded successfully!";
        qDebug() << "Dimensions:" << m_dimensions;
        qDebug() << "Spacing:" << m_spacing << "mm";
        qDebug() << "Total slices:" << m_totalSlices;

        // Calculate intensity range
        typedef itk::MinimumMaximumImageCalculator<Image3DType> CalculatorType;
        CalculatorType::Pointer calculator = CalculatorType::New();
        calculator->SetImage(m_volume);
        calculator->Compute();

        m_minIntensity = calculator->GetMinimum();
        m_maxIntensity = calculator->GetMaximum();

        qDebug() << "Intensity range:" << m_minIntensity << "to" << m_maxIntensity;

        m_isVolumeLoaded = true;

        emit volumeDataChanged();
        emit volumeLoadedChanged();

        // Show middle slice
        showSlice(m_currentSlice);

        emit loadingFinished();

    } catch (itk::ExceptionObject& err) {
        QString errorMsg = QString("ITK Exception: %1").arg(err.GetDescription());
        emit errorOccurred(errorMsg);
        qCritical() << errorMsg;
        emit loadingFinished();
    } catch (std::exception& err) {
        QString errorMsg = QString("Standard Exception: %1").arg(err.what());
        emit errorOccurred(errorMsg);
        qCritical() << errorMsg;
        emit loadingFinished();
    } catch (...) {
        emit errorOccurred("Unknown error loading DICOM series");
        qCritical() << "Unknown error loading DICOM series";
        emit loadingFinished();
    }
}

void DicomHandler::showSlice(int sliceIndex)
{
    if (!m_isVolumeLoaded || !m_volume) {
        qWarning() << "No volume loaded to show slice";
        return;
    }

    if (sliceIndex < 0 || sliceIndex >= m_totalSlices) {
        qWarning() << "Slice index out of range:" << sliceIndex;
        return;
    }

    m_currentSlice = sliceIndex;

    QImage sliceImage = convertSliceToImage(sliceIndex);

    if (m_provider && !sliceImage.isNull()) {
        m_provider->setImage(sliceImage);
        emit sliceChanged(sliceIndex);
        qDebug() << "Displaying slice:" << sliceIndex << "size:" << sliceImage.size();
    } else {
        qWarning() << "Failed to create or set slice image";
    }
}

QImage DicomHandler::convertSliceToImage(int sliceIndex)
{
    if (!m_volume || sliceIndex < 0 || sliceIndex >= m_totalSlices) {
        return QImage();
    }

    Image3DType::RegionType region = m_volume->GetLargestPossibleRegion();
    int width = region.GetSize()[0];
    int height = region.GetSize()[1];

    // Create QImage
    QImage image(width, height, QImage::Format_RGB888);

    // Calculate normalization factors
    float windowMin = m_windowLevel - m_windowWidth / 2.0f;
    float windowMax = m_windowLevel + m_windowWidth / 2.0f;

    // Fill image with pixel data
    for (int y = 0; y < height; ++y) {
        QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType index;
            index[0] = x;
            index[1] = y;
            index[2] = sliceIndex;

            PixelType pixelValue = m_volume->GetPixel(index);

            // Apply window/level
            float normalized = (pixelValue - windowMin) / (windowMax - windowMin);
            normalized = qBound(0.0f, normalized, 1.0f);

            int intensity = static_cast<int>(normalized * 255);
            intensity = qBound(0, intensity, 255);

            scanLine[x] = qRgb(intensity, intensity, intensity);
        }
    }

    return image;
}

void DicomHandler::nextSlice()
{
    if (m_isVolumeLoaded && m_currentSlice < m_totalSlices - 1) {
        showSlice(m_currentSlice + 1);
    }
}

void DicomHandler::prevSlice()
{
    if (m_isVolumeLoaded && m_currentSlice > 0) {
        showSlice(m_currentSlice - 1);
    }
}

void DicomHandler::setWindowLevel(float window, float level)
{
    m_windowWidth = window;
    m_windowLevel = level;

    if (m_isVolumeLoaded) {
        showSlice(m_currentSlice);
    }
}

QByteArray DicomHandler::volumeData() const
{
    return m_volumeData;
}

QVector3D DicomHandler::volumeDimensions() const
{
    return m_dimensions;
}

QVector3D DicomHandler::volumeSpacing() const
{
    return m_spacing;
}

bool DicomHandler::volumeLoaded() const
{
    return m_isVolumeLoaded;
}
