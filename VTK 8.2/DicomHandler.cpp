#include "DicomHandler.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QPainter>

#include <itkImageSeriesReader.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageRegionConstIterator.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkIntensityWindowingImageFilter.h>

DicomHandler::DicomHandler(DicomImageProvider* provider, QObject* parent)
    : QObject(parent), m_provider(provider), m_sliceCount(0),
      m_windowWidth(400), m_windowCenter(40),
      m_minIntensity(0), m_maxIntensity(4095) {}

void DicomHandler::loadDicomSeries(const QString& folderPath) {
    try {
        itk::GDCMImageIOFactory::RegisterOneFactory();

        using ReaderType = itk::ImageSeriesReader<Image3DType>;
        auto reader = ReaderType::New();

        auto nameGenerator = itk::GDCMSeriesFileNames::New();
        nameGenerator->SetUseSeriesDetails(true);
        nameGenerator->AddSeriesRestriction("0008|0021");
        nameGenerator->SetDirectory(folderPath.toStdString());

        const auto& seriesUID = nameGenerator->GetSeriesUIDs();
        if (seriesUID.empty()) {
            emit errorOccurred("No DICOM series found in folder.");
            return;
        }

        // Get series description for display
        const auto& filenames = nameGenerator->GetFileNames(seriesUID[0]);
        m_seriesName = QString::fromStdString(seriesUID[0]);

        reader->SetFileNames(filenames);
        reader->Update();
        m_volume = reader->GetOutput();

        // Get volume properties
        auto region = m_volume->GetLargestPossibleRegion();
        m_volumeDimensions.setX(region.GetSize()[0]);
        m_volumeDimensions.setY(region.GetSize()[1]);
        m_volumeDimensions.setZ(region.GetSize()[2]);
        m_sliceCount = region.GetSize()[2];

        // Calculate intensity range
        auto calculator = itk::MinimumMaximumImageCalculator<Image3DType>::New();
        calculator->SetImage(m_volume);
        calculator->Compute();
        m_minIntensity = calculator->GetMinimum();
        m_maxIntensity = calculator->GetMaximum();

        // Extract volume data for 3D rendering
        extractVolumeData();

        emit volumeLoaded();
        showSlice(0);

        qDebug() << "DICOM loaded:" << m_sliceCount << "slices, dimensions:"
                 << m_volumeDimensions << "intensity range:"
                 << m_minIntensity << "-" << m_maxIntensity;

    } catch (itk::ExceptionObject& e) {
        emit errorOccurred(QString("ITK Error: %1").arg(e.GetDescription()));
    }
}

void DicomHandler::extractVolumeData() {
    if (!m_volume) return;

    auto region = m_volume->GetLargestPossibleRegion();
    int totalPixels = region.GetSize()[0] * region.GetSize()[1] * region.GetSize()[2];

    m_volumeData.resize(totalPixels);

    itk::ImageRegionConstIterator<Image3DType> it(m_volume, region);
    int index = 0;

    while (!it.IsAtEnd()) {
        m_volumeData[index++] = it.Get();
        ++it;
    }
}

void DicomHandler::showSlice(int index) {
    if (!m_volume || index < 0 || index >= m_sliceCount) return;

    auto region = m_volume->GetLargestPossibleRegion();
    const int width = region.GetSize()[0];
    const int height = region.GetSize()[1];

    // Extract slice data
    QVector<short> sliceData(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType idx = {{x, y, index}};
            sliceData[y * width + x] = m_volume->GetPixel(idx);
        }
    }

    // Create image with window/level
    QImage qimg(width, height, QImage::Format_ARGB32);
    applyWindowLevel(qimg, sliceData.data(), width, height);

    m_provider->setImage(qimg);
    emit sliceChanged(index);
}

void DicomHandler::applyWindowLevel(QImage& image, short* sliceData, int width, int height) {
    double windowMin = m_windowCenter - m_windowWidth / 2.0;
    double windowMax = m_windowCenter + m_windowWidth / 2.0;
    double scale = 255.0 / (windowMax - windowMin);

    for (int y = 0; y < height; ++y) {
        QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < width; ++x) {
            short pixel = sliceData[y * width + x];
            double value = qBound(windowMin, static_cast<double>(pixel), windowMax);
            int gray = static_cast<int>((value - windowMin) * scale);
            gray = qBound(0, gray, 255);
            scanLine[x] = qRgb(gray, gray, gray);
        }
    }
}

void DicomHandler::setWindowWidth(double width) {
    if (qFuzzyCompare(m_windowWidth, width)) return;
    m_windowWidth = qMax(1.0, width);
    emit windowChanged();
}

void DicomHandler::setWindowCenter(double center) {
    if (qFuzzyCompare(m_windowCenter, center)) return;
    m_windowCenter = center;
    emit windowChanged();
}

QImage DicomHandler::getSliceImage(int index) {
    if (!m_volume || index < 0 || index >= m_sliceCount)
        return QImage();

    auto region = m_volume->GetLargestPossibleRegion();
    const int width = region.GetSize()[0];
    const int height = region.GetSize()[1];

    QVector<short> sliceData(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType idx = {{x, y, index}};
            sliceData[y * width + x] = m_volume->GetPixel(idx);
        }
    }

    QImage qimg(width, height, QImage::Format_ARGB32);
    applyWindowLevel(qimg, sliceData.data(), width, height);
    return qimg;
}
