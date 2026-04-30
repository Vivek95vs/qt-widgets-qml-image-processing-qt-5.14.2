#include "DicomHandler.h"

#include <itkImageSeriesReader.h>
#include <itkImageSliceConstIteratorWithIndex.h>
#include <itkNumericSeriesFileNames.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageRegionConstIterator.h>
#include <QDebug>


DicomHandler::DicomHandler(DicomImageProvider* provider, QObject* parent)
    : QObject(parent), m_provider(provider) {}

void DicomHandler::loadDicomSeries() {
    QString folderPath="D:\\KCH site\\Head_neck\\Without_Norm\\DailyImages\\DICOM\\CBCT";
    itk::GDCMImageIOFactory::RegisterOneFactory();

    using ReaderType = itk::ImageSeriesReader<Image3DType>;
    auto reader = ReaderType::New();

    auto nameGenerator = itk::GDCMSeriesFileNames::New();
    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021"); // Series Date
    nameGenerator->SetDirectory(folderPath.toStdString());

    const auto& seriesUID = nameGenerator->GetSeriesUIDs();
    if (seriesUID.empty()) {
        qWarning("No DICOM series found in folder.");
        return;
    }

    const auto& filenames = nameGenerator->GetFileNames(seriesUID[0]);
    reader->SetFileNames(filenames);

    try {
        reader->Update();
        m_volume = reader->GetOutput();
        showSlice(0);  // show first slice initially
    } catch (itk::ExceptionObject& e) {
        qWarning("ITK error: %s", e.GetDescription());
    }
}

void DicomHandler::showSlice(int index) {
    if (!m_volume) return;

    auto region = m_volume->GetLargestPossibleRegion();
    const int width = region.GetSize()[0];
    const int height = region.GetSize()[1];
    const int depth = region.GetSize()[2];

    if (index < 0 || index >= depth) return;

    QImage qimg(width, height, QImage::Format_Grayscale16);

    for (int y = 0; y < height; ++y) {
        ushort* line = reinterpret_cast<ushort*>(qimg.scanLine(y));
        for (int x = 0; x < width; ++x) {
            Image3DType::IndexType idx = { {x, y, index} };
            line[x] = m_volume->GetPixel(idx);
        }
    }

    m_provider->setImage(qimg);
}
