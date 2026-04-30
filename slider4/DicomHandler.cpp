#include "DicomHandler.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <itkStatisticsImageFilter.h>

#include <itkImageSeriesReader.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkExtractImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkFlipImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkImageRegionConstIterator.h>
#include <itkStatisticsImageFilter.h>
#include <itkImageToVTKImageFilter.h>

DicomHandler::DicomHandler(QObject* parent)
    : QObject(parent),
      m_sliceCount(0),
      m_volumeLoaded(false),
      m_currentAxialSlice(0),
      m_currentCoronalSlice(0),
      m_currentSagittalSlice(0)
{
    // Create image providers
    m_axialProvider = new DicomImageProvider(DicomImageProvider::Axial);
    m_coronalProvider = new DicomImageProvider(DicomImageProvider::Coronal);
    m_sagittalProvider = new DicomImageProvider(DicomImageProvider::Sagittal);
}

DicomHandler::~DicomHandler() {
    delete m_axialProvider;
    delete m_coronalProvider;
    delete m_sagittalProvider;
}

void DicomHandler::loadDicomSeries() {
    // Use file dialog to select folder
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select DICOM Folder");
    if (folderPath.isEmpty()) return;

    qDebug() << "Loading DICOM from:" << folderPath;

    try {
        // Register DICOM factory
        itk::GDCMImageIOFactory::RegisterOneFactory();

        using ReaderType = itk::ImageSeriesReader<Image3DType>;
        auto reader = ReaderType::New();

        auto nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->SetDirectory(folderPath.toStdString());

        const auto& seriesUIDs = nameGenerator->GetSeriesUIDs();
        if (seriesUIDs.empty()) {
            qWarning() << "No DICOM series found in folder";
            emit volumeLoaded();
            return;
        }

        qDebug() << "Found" << seriesUIDs.size() << "series";

        // Get filenames for the first series
        const auto& filenames = nameGenerator->GetFileNames(seriesUIDs[0]);
        qDebug() << "Number of files:" << filenames.size();

        reader->SetFileNames(filenames);

        // Update to load volume
        reader->Update();
        m_volume = reader->GetOutput();

        // Get volume information
        auto region = m_volume->GetLargestPossibleRegion();
        auto size = region.GetSize();
        auto spacing = m_volume->GetSpacing();

        m_dimensions = QVector3D(size[0], size[1], size[2]);
        m_spacing = QVector3D(spacing[0], spacing[1], spacing[2]);
        m_sliceCount = size[2];
        m_volumeLoaded = true;

        // Initialize slice positions to middle
        m_currentAxialSlice = size[2] / 2;
        m_currentCoronalSlice = size[1] / 2;
        m_currentSagittalSlice = size[0] / 2;

        qDebug() << "Volume loaded successfully";
        qDebug() << "Dimensions:" << m_dimensions;
        qDebug() << "Spacing:" << m_spacing;
        qDebug() << "Total slices:" << m_sliceCount;

        // Generate initial slices
        updateAllSlices();

        // Emit property change signals
        emit currentAxialSliceChanged();
        emit currentCoronalSliceChanged();
        emit currentSagittalSliceChanged();
        emit volumeLoaded();

    } catch (itk::ExceptionObject& err) {
        qWarning() << "ITK Exception:" << err.GetDescription();
        emit volumeLoaded();
    } catch (...) {
        qWarning() << "Unknown error loading DICOM";
        emit volumeLoaded();
    }
}

void DicomHandler::setCurrentAxialSlice(int slice) {
    if (!m_volume) return;

    int maxSlice = m_dimensions.z() - 1;
    if (slice < 0 || slice > maxSlice) return;

    if (m_currentAxialSlice != slice) {
        m_currentAxialSlice = slice;
        QImage axialImage = extractSlice(2, slice);
        m_axialProvider->setImage(axialImage);
        emit axialImageChanged();
        emit currentAxialSliceChanged();
    }
}

void DicomHandler::setCurrentCoronalSlice(int slice) {
    if (!m_volume) return;

    int maxSlice = m_dimensions.y() - 1;
    if (slice < 0 || slice > maxSlice) return;

    if (m_currentCoronalSlice != slice) {
        m_currentCoronalSlice = slice;
        QImage coronalImage = extractSlice(1, slice);
        m_coronalProvider->setImage(coronalImage);
        emit coronalImageChanged();
        emit currentCoronalSliceChanged();
    }
}

void DicomHandler::setCurrentSagittalSlice(int slice) {
    if (!m_volume) return;

    int maxSlice = m_dimensions.x() - 1;
    if (slice < 0 || slice > maxSlice) return;

    if (m_currentSagittalSlice != slice) {
        m_currentSagittalSlice = slice;
        QImage sagittalImage = extractSlice(0, slice);
        m_sagittalProvider->setImage(sagittalImage);
        emit sagittalImageChanged();
        emit currentSagittalSliceChanged();
    }
}

void DicomHandler::updateAllSlices() {
    if (!m_volume) return;

    QImage axialImage = extractSlice(2, m_currentAxialSlice);
    QImage coronalImage = extractSlice(1, m_currentCoronalSlice);
    QImage sagittalImage = extractSlice(0, m_currentSagittalSlice);

    m_axialProvider->setImage(axialImage);
    m_coronalProvider->setImage(coronalImage);
    m_sagittalProvider->setImage(sagittalImage);

    emit axialImageChanged();
    emit coronalImageChanged();
    emit sagittalImageChanged();
}

QImage DicomHandler::extractSlice(int plane, int sliceIndex) {
    if (!m_volume) return QImage();

    try {
        using ExtractFilterType = itk::ExtractImageFilter<Image3DType, Image2DType>;
        auto extractor = ExtractFilterType::New();

        auto inputRegion = m_volume->GetLargestPossibleRegion();
        auto inputSize = inputRegion.GetSize();
        auto inputIndex = inputRegion.GetIndex();

        // Set up region for extraction based on plane
        itk::ImageRegion<3> desiredRegion;

        if (plane == 2) { // Axial (XY plane)
            desiredRegion.SetSize(0, inputSize[0]);
            desiredRegion.SetSize(1, inputSize[1]);
            desiredRegion.SetSize(2, 0);

            desiredRegion.SetIndex(0, inputIndex[0]);
            desiredRegion.SetIndex(1, inputIndex[1]);
            desiredRegion.SetIndex(2, sliceIndex);
        }
        else if (plane == 1) { // Coronal (XZ plane)
            desiredRegion.SetSize(0, inputSize[0]);
            desiredRegion.SetSize(1, 0);
            desiredRegion.SetSize(2, inputSize[2]);

            desiredRegion.SetIndex(0, inputIndex[0]);
            desiredRegion.SetIndex(1, sliceIndex);
            desiredRegion.SetIndex(2, inputIndex[2]);
        }
        else { // Sagittal (YZ plane)
            desiredRegion.SetSize(0, 0);
            desiredRegion.SetSize(1, inputSize[1]);
            desiredRegion.SetSize(2, inputSize[2]);

            desiredRegion.SetIndex(0, sliceIndex);
            desiredRegion.SetIndex(1, inputIndex[1]);
            desiredRegion.SetIndex(2, inputIndex[2]);
        }

        extractor->SetExtractionRegion(desiredRegion);
        extractor->SetInput(m_volume);
        extractor->SetDirectionCollapseToIdentity();
        extractor->Update();

        auto sliceImage = extractor->GetOutput();
        return convertITKToQImage(sliceImage);

    } catch (itk::ExceptionObject& err) {
        qWarning() << "Error extracting slice:" << err.GetDescription();
        return QImage();
    }
}

QImage DicomHandler::convertITKToQImage(Image2DType::Pointer image) {
    if (!image) return QImage();

    try {
        // Get image statistics for better normalization
        using StatsFilterType = itk::StatisticsImageFilter<Image2DType>;
        auto stats = StatsFilterType::New();
        stats->SetInput(image);
        stats->Update();

        double minValue = stats->GetMinimum();
        double maxValue = stats->GetMaximum();

        // Normalize intensity for display
        using RescaleFilterType = itk::RescaleIntensityImageFilter<Image2DType, Image2DType>;
        auto rescaler = RescaleFilterType::New();
        rescaler->SetInput(image);
        rescaler->SetOutputMinimum(0);
        rescaler->SetOutputMaximum(255);
        rescaler->Update();

        auto normalizedImage = rescaler->GetOutput();
        auto region = normalizedImage->GetLargestPossibleRegion();
        int width = region.GetSize()[0];
        int height = region.GetSize()[1];

        // Create QImage (8-bit grayscale for display)
        QImage qimage(width, height, QImage::Format_Grayscale8);
        qimage.fill(0);

        // Copy pixel data
        itk::ImageRegionConstIterator<Image2DType> it(normalizedImage, region);
        for (int y = 0; y < height; ++y) {
            uchar* line = qimage.scanLine(y);
            for (int x = 0; x < width; ++x) {
                line[x] = static_cast<uchar>(it.Get());
                ++it;
            }
        }

        return qimage;

    } catch (itk::ExceptionObject& err) {
        qWarning() << "Error converting ITK to QImage:" << err.GetDescription();

        // Return a placeholder image
        QImage placeholder(256, 256, QImage::Format_Grayscale8);
        placeholder.fill(128);
        return placeholder;
    }
}
