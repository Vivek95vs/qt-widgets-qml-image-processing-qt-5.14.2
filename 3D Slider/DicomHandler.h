#pragma once
#include <QObject>
#include <QImage>
#include <QStringList>
#include "DicomImageProvider.h"

#include <itkImageSeriesReader.h>
#include <itkImage.h>
#include <itkGDCMImageIOFactory.h>

class DicomHandler : public QObject {
    Q_OBJECT
public:
    using Image3DType = itk::Image<unsigned short, 3>;
    using Image2DType = itk::Image<unsigned short, 2>;

    DicomHandler(DicomImageProvider* provider, QObject* parent = nullptr);

    Q_INVOKABLE void loadDicomSeries(); // From QML
    Q_INVOKABLE void showSlice(int index);                        // From QML slider

private:
    DicomImageProvider* m_provider;
    typename Image3DType::Pointer m_volume;
};
