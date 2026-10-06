#include "dicomloader.h"



#include <QDebug>
#include <QFileInfo>
#include <cmath>
#include <QVector>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include<debugtimer.h>

DicomLoader::DicomLoader(QObject *parent)
    : QObject(parent),
      m_imageRevision(0),
      m_loaded(false),
      m_imageProcessingApplied(false),
      m_histogramCount(0),
           m_histogramMean(0.0),
           m_histogramStdDev(0.0),
           m_histogramMin(0),
           m_histogramMax(0),
           m_histogramMode(0),
           m_histogramModeCount(0),
           m_histogramBins(256),
           m_histogramBinWidth(0.0)
{
}


// ============================================================
// GETTERS
// ============================================================

QString DicomLoader::filePath() const
{
    return m_filePath;
}


QString DicomLoader::patientName() const
{
    return m_patientName;
}


QString DicomLoader::patientId() const
{
    return m_patientId;
}


QString DicomLoader::studyDescription() const
{
    return m_studyDescription;
}


QString DicomLoader::seriesDescription() const
{
    return m_seriesDescription;
}


int DicomLoader::imageWidth() const
{
    return m_image.width();
}


int DicomLoader::imageHeight() const
{
    return m_image.height();
}


int DicomLoader::imageRevision() const
{
    return m_imageRevision;
}


bool DicomLoader::isLoaded() const
{
    return m_loaded;
}


QImage DicomLoader::image() const
{
    return m_image;
}


// ============================================================
// LOAD DICOM
// ============================================================

bool DicomLoader::loadDicom(const QString &filePath)
{
    qDebug() << "========================================";
    qDebug() << "Loading DICOM:";
    qDebug() << filePath;
    qDebug() << "========================================";

    setStatus(
            "Loading...",
            "loading");

    clearHistogram();
    // --------------------------------------------------------
    // VALIDATE FILE
    // --------------------------------------------------------

    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        const QString errorMessage =
            QString("File does not exist: %1")
                .arg(filePath);

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    // Clear previous data
    clearData(false);


    // --------------------------------------------------------
    // LOAD DICOM DATASET
    // --------------------------------------------------------

    DcmFileFormat fileFormat;

    const QByteArray localPath =
        QFile::encodeName(filePath);

    OFCondition status =
        fileFormat.loadFile(
            localPath.constData());


    if (status.bad())
    {
        const QString errorMessage =
            QString("Unable to load DICOM file: %1")
                .arg(
                    QString::fromLatin1(
                        status.text()));

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    DcmDataset *dataset =
        fileFormat.getDataset();


    if (!dataset)
    {
        const QString errorMessage =
            "Unable to access DICOM dataset.";

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    // --------------------------------------------------------
    // READ PATIENT INFORMATION
    // --------------------------------------------------------

    OFString value;


    if (dataset->findAndGetOFString(
            DCM_PatientName,
            value).good())
    {
        m_patientName =
            QString::fromLocal8Bit(
                value.c_str());
    }


    if (dataset->findAndGetOFString(
            DCM_PatientID,
            value).good())
    {
        m_patientId =
            QString::fromLocal8Bit(
                value.c_str());
    }

    // Patient Birth Date
    if (dataset->findAndGetOFString(
            DCM_PatientBirthDate,
            value).good())
    {
        m_patientBirthDate =
            QString::fromLocal8Bit(value.c_str());
    }

    // Patient Sex
    if (dataset->findAndGetOFString(
            DCM_PatientSex,
            value).good())
    {
        m_patientSex =
            QString::fromLocal8Bit(value.c_str());
    }

    // Modality
    if (dataset->findAndGetOFString(
            DCM_Modality,
            value).good())
    {
        m_modality =
            QString::fromLocal8Bit(value.c_str());
    }

    // Study Date
    if (dataset->findAndGetOFString(
            DCM_StudyDate,
            value).good())
    {
        m_studyDate =
            QString::fromLocal8Bit(value.c_str());
    }

    // --------------------------------------------------------
    // READ STUDY INFORMATION
    // --------------------------------------------------------

    if (dataset->findAndGetOFString(
            DCM_StudyDescription,
            value).good())
    {
        m_studyDescription =
            QString::fromLocal8Bit(
                value.c_str());
    }


    // --------------------------------------------------------
    // READ SERIES INFORMATION
    // --------------------------------------------------------

    if (dataset->findAndGetOFString(
            DCM_SeriesDescription,
            value).good())
    {
        m_seriesDescription =
            QString::fromLocal8Bit(
                value.c_str());
    }


    // --------------------------------------------------------
    // LOAD DICOM IMAGE
    // --------------------------------------------------------

    DicomImage dicomImage(
        localPath.constData());


    if (dicomImage.getStatus() != EIS_Normal)
    {
        const QString errorMessage =
            "Unable to decode DICOM image.";

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    const int width =
        static_cast<int>(
            dicomImage.getWidth());


    const int height =
        static_cast<int>(
            dicomImage.getHeight());


    if (width <= 0 || height <= 0)
    {
        const QString errorMessage =
            "Invalid DICOM image dimensions.";

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    // --------------------------------------------------------
    // CONVERT TO 8-BIT GRAYSCALE
    // --------------------------------------------------------

//    const void *pixelData =
//        dicomImage.getOutputData(
//            8);
    // --------------------------------------------------------
    // CONVERT TO 16-BIT GRAYSCALE
    // --------------------------------------------------------

    const void *pixelData =
        dicomImage.getOutputData(
            16);

    if (!pixelData)
    {
        const QString errorMessage =
            "Unable to retrieve DICOM pixel data.";

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


//    QImage temporaryImage(
//        static_cast<const uchar *>(
//            pixelData),
//        width,
//        height,
//        QImage::Format_Grayscale8);

//    QImage temporaryImage(
//        static_cast<const uchar *>(pixelData),
//        width,
//        height,
//        QImage::Format_Grayscale16);

    QImage temporaryImage(
        static_cast<const uchar *>(pixelData),
        width,
        height,
        width * sizeof(quint16),
        QImage::Format_Grayscale16);

    // IMPORTANT:
    // DicomImage owns pixelData.
    // Therefore copy the data.

    m_image =
        temporaryImage.copy();

    double dataMin = 65535.0;
    double dataMax = 0.0;

    for (int y = 0; y < m_image.height(); ++y)
    {
        const quint16 *line =
            reinterpret_cast<const quint16 *>(
                m_image.constScanLine(y));

        for (int x = 0; x < m_image.width(); ++x)
        {
            const double pixel = line[x];

            if (pixel < dataMin)
                dataMin = pixel;

            if (pixel > dataMax)
                dataMax = pixel;
        }
    }

    m_imageProcessor.setDataRange(dataMin, dataMax);

    qDebug() << "ImageProcessor Data Range:"
             << "Min =" << dataMin
             << "Max =" << dataMax;

    qDebug() << "================================";
    qDebug() << "DICOM IMAGE LOADED";
    qDebug() << "Original Image Size:"
             << m_image.width()
             << "x"
             << m_image.height();
    qDebug() << "Image format:" << m_image.format();

    qDebug() << "Bytes per line:" << m_image.bytesPerLine();
    qDebug() << "================================";

    m_imageProcessor.reset();
// view image 512*512
//    QImage preview =
//            m_image.scaled(
//                512,
//                512,
//                Qt::KeepAspectRatio,
//                Qt::SmoothTransformation);

//    preview = preview.convertToFormat(QImage::Format_Grayscale16);
    QImage preview = m_image;

    qDebug() << "Preview format:"
             << preview.format();

    m_imageProcessor.setSourceImage(
                preview);

    m_previewImage =
            m_imageProcessor.processPreview();


    if (m_image.isNull())
    {
        const QString errorMessage =
            "Failed to create QImage from DICOM pixel data.";

        qDebug() << errorMessage;

        emit errorOccurred(errorMessage);

        return false;
    }


    // --------------------------------------------------------
    // UPDATE STATE
    // --------------------------------------------------------

    m_filePath = filePath;

    m_loaded = true;

    ++m_imageRevision;
    m_modified = false;

    emit modifiedChanged();

    setStatus(
        "Ready",
        "ready");

    // --------------------------------------------------------
    // NOTIFY QML
    // --------------------------------------------------------

    emit filePathChanged();

    emit metadataChanged();

    emit imageChanged();

    emit loadedChanged();




    qDebug() << "========================================";
    qDebug() << "DICOM loaded successfully";

    qDebug() << "Patient Name:"
             << m_patientName;

    qDebug() << "Patient ID:"
             << m_patientId;

    qDebug() << "Study:"
             << m_studyDescription;

    qDebug() << "Series:"
             << m_seriesDescription;

    qDebug() << "Image Size:"
             << width << "x" << height;

    qDebug() << "Image Revision:"
             << m_imageRevision;

    qDebug() << "========================================";


    return true;
}


// ============================================================
// CLEAR
// ============================================================

void DicomLoader::clear()
{
    clearData(true);
}


// ============================================================
// CLEAR INTERNAL DATA
// ============================================================

void DicomLoader::clearData(bool emitSignals)
{
    const bool wasLoaded =
        m_loaded;


    m_filePath.clear();

    m_patientName.clear();

    m_patientId.clear();

    m_studyDescription.clear();

    m_seriesDescription.clear();


    m_image =
        QImage();


    m_loaded =
        false;


    if (emitSignals)
    {
        emit filePathChanged();

        emit metadataChanged();

        emit imageChanged();

        if (wasLoaded)
        {
            emit loadedChanged();
        }
    }
}

QString DicomLoader::patientBirthDate() const
{
    return m_patientBirthDate;
}

QString DicomLoader::patientSex() const
{
    return m_patientSex;
}

QString DicomLoader::modality() const
{
    return m_modality;
}

QString DicomLoader::studyDate() const
{
    return m_studyDate;
}

// for windows level, weidth
int DicomLoader::minimum() const
{

    return m_imageProcessor.minimum();
}
int DicomLoader::maximum() const
{
    return m_imageProcessor.maximum();
}
int DicomLoader::brightness() const
{
    return m_imageProcessor.brightness();
}
double DicomLoader::contrast() const
{
    return m_imageProcessor.contrast();
}

void DicomLoader::updatePreview()
{
    if (m_image.isNull())
        return;
    qDebug() << "====================================";
       qDebug() << "DICOM IMAGE PROCESSING";

       // Original image
       qDebug() << "Original Image Size:"
                << m_image.width()
                << "x"
                << m_image.height();

    // Create a maximum 512×512 working preview
//    QImage scaledPreview =
//            m_image.scaled(
//                512,
//                512,
//                Qt::KeepAspectRatio,
//                Qt::SmoothTransformation);

       QImage scaledPreview = m_image;

    // Working preview
        qDebug() << "Working Preview Size:"
                 << scaledPreview.width()
                 << "x"
                 << scaledPreview.height();


    // Give preview to processor
    m_imageProcessor.setSourceImage(
                scaledPreview);

    // Process preview
    m_previewImage =
            m_imageProcessor.processPreview();


    // Processed preview
    qDebug() << "Processed Preview Size:"
             << m_previewImage.width()
             << "x"
             << m_previewImage.height();

    qDebug() << "====================================";


    // Tell QML/provider image changed
    emit imageChanged();

    emit imageProcessingChanged();
}

void DicomLoader::setMinimum(int value)
{
    DebugTimer timer(__FUNCTION__);
    if (!m_loaded)
        return;

    m_imageProcessor.setMinimum(value);

    m_modified = true;

    emit modifiedChanged();
    emit imageProcessingChanged();

    updatePreview();

    setStatus(
            "Editing...",
            "editing");
}

void DicomLoader::setMaximum(int value)
{
    DebugTimer timer(__FUNCTION__);
    if (!m_loaded)
        return;

    m_imageProcessor.setMaximum(value);

    m_modified = true;

    emit modifiedChanged();
    emit imageProcessingChanged();

    updatePreview();

    setStatus(
            "Editing...",
            "editing");
}

void DicomLoader::setBrightness(int value)
{
    DebugTimer timer(__FUNCTION__);
    if (!m_loaded)
        return;

    m_imageProcessor.setBrightness(value);

    m_modified = true;

    emit modifiedChanged();
    emit imageProcessingChanged();

    updatePreview();

    setStatus(
            "Editing...",
            "editing");
}

void DicomLoader::setContrast(double value)
{
    DebugTimer timer(__FUNCTION__);
    if (!m_loaded)
        return;

    m_imageProcessor.setContrast(value);

    m_modified = true;

    emit modifiedChanged();
    emit imageProcessingChanged();

    updatePreview();

    setStatus(
            "Editing...",
            "editing");
}
void DicomLoader::resetImageProcessing()
{
    qDebug() << "================================";
        qDebug() << "RESET IMAGE PROCESSING";
        qDebug() << "================================";

    if (!m_loaded)
        return;

    m_imageProcessor.reset();

    updatePreview();

    qDebug() << "Reset Minimum:" << minimum();
       qDebug() << "Reset Maximum:" << maximum();
       qDebug() << "Reset Brightness:" << brightness();
       qDebug() << "Reset Contrast:" << contrast();

       emit imageProcessingChanged();
       setStatus(
               "Ready",
               "ready");
}
QImage DicomLoader::previewImage() const
{
    return m_previewImage;
}
void DicomLoader::autoImageProcessing()
{
    qDebug() << "================================";
    qDebug() << "AUTO IMAGE PROCESSING CALLED";
    qDebug() << "================================";

    if (m_image.isNull())
    {
        qDebug() << "AUTO: No image loaded";
        return;
    }

    // =====================================================
    // CREATE WORKING PREVIEW
    // =====================================================

//    QImage preview =
//            m_image.scaled(
//                512,
//                512,
//                Qt::KeepAspectRatio,
//                Qt::SmoothTransformation);

//    preview =
//            preview.convertToFormat(
//                QImage::Format_Grayscale8);

    QImage preview = m_image;

    if (preview.isNull())
    {
        qDebug() << "AUTO: Preview creation failed";
        return;
    }

    // =====================================================
    // BUILD HISTOGRAM
    // =====================================================

    QVector<quint64> histogram(65536, 0);

    const int totalPixels =
            preview.width()
            * preview.height();

    for (int y = 0;
         y < preview.height();
         ++y)
    {


        const quint16 *line =
                reinterpret_cast<const quint16 *>(
                    preview.constScanLine(y));

        for (int x = 0;
             x < preview.width();
             ++x)
        {
            const int value =
                    static_cast<int>(line[x]);

            histogram[value]++;
        }
    }

    // =====================================================
    // IGNORE EXTREME PIXELS
    //
    // Ignore lowest 1%
    // Ignore highest 1%
    // =====================================================

    const quint64 lowerThreshold =
            qMax(
                1,
                totalPixels / 100);

    const quint64 upperThreshold =
            totalPixels -
            lowerThreshold;

    // =====================================================
    // FIND AUTO MINIMUM
    // =====================================================

    int accumulated = 0;

    int minValue = 0;

    for (int i = 0;
         i < 65536;
         ++i)
    {
        accumulated +=
                histogram[i];

        if (accumulated >=
                lowerThreshold)
        {
            minValue = i;
            break;
        }
    }

    // =====================================================
    // FIND AUTO MAXIMUM
    // =====================================================

    accumulated = 0;

    int maxValue = 65535;

    for (int i = 0;
         i < 65536;
         ++i)
    {
        accumulated +=
                histogram[i];

        if (accumulated >=
                upperThreshold)
        {
            maxValue = i;
            break;
        }
    }

    // =====================================================
    // VALIDATE RANGE
    // =====================================================

//    if (minValue >= maxValue)
//    {
//        minValue = 0;
//        maxValue = 255;
//    }

    if (minValue >= maxValue)
    {
        minValue = 0;
        maxValue = 65535;
    }

    // =====================================================
    // APPLY AUTO VALUES
    //
    // Maximum is the master value.
    //
    // Maximum decreases
    // → Brightness increases
    // → Contrast increases
    //
    // setMaximum() also keeps Minimum = 0
    // =====================================================

    m_imageProcessor.setMaximum(maxValue);
    // =====================================================
    // DEBUG
    // =====================================================

    qDebug() << "================================";
    qDebug() << "AUTO RESULT";
    qDebug() << "Preview Size:"
             << preview.width()
             << "x"
             << preview.height();

    qDebug() << "Total Pixels:"
             << totalPixels;

    qDebug() << "Minimum:"
             << m_imageProcessor.minimum();

    qDebug() << "Maximum:"
             << m_imageProcessor.maximum();

    qDebug() << "Brightness:"
             << m_imageProcessor.brightness();

    qDebug() << "Contrast:"
             << m_imageProcessor.contrast();

    qDebug() << "================================";

    // =====================================================
    // UPDATE IMAGE
    // =====================================================

    updatePreview();

    // Update QML sliders
    emit imageProcessingChanged();

    setStatus(
                "Editing...",
                "editing");
}
//apply
void DicomLoader::applyImageProcessing()
{
    DebugTimer timer(__FUNCTION__);
    qDebug() << "================================";
    qDebug() << "APPLY IMAGE PROCESSING";
    qDebug() << "================================";

    if (m_image.isNull())
    {
        qDebug() << "APPLY: No image loaded";

        m_imageProcessingApplied = false;

        return;
    }

    qDebug() << "Original Image Size:"
             << m_image.width()
             << "x"
             << m_image.height();

    qDebug() << "Original Image Format:"
             << m_image.format();

    qDebug() << "Processing Values:";

    qDebug() << "Minimum:" << minimum();
    qDebug() << "Maximum:" << maximum();
    qDebug() << "Brightness:" << brightness();
    qDebug() << "Contrast:" << contrast();

    // =====================================================
    // PROCESS FULL RESOLUTION IMAGE
    // =====================================================

    m_imageProcessor.setSourceImage(m_image);

    m_processedImage =
            m_imageProcessor.processPreview();

    // =====================================================
    // VALIDATE RESULT
    // =====================================================

    if (m_processedImage.isNull())
    {
        qDebug() << "APPLY: Processing failed";

        m_imageProcessingApplied = false;

        return;
    }

    m_imageProcessingApplied = true;

    qDebug() << "Processed Image Size:"
             << m_processedImage.width()
             << "x"
             << m_processedImage.height();

    qDebug() << "Processed Image Format:"
             << m_processedImage.format();

    qDebug() << "================================";
    qDebug() << "FULL RESOLUTION APPLY COMPLETE";
    qDebug() << "================================";
    setStatus(
            "Not Saved !",
            "notSaved");
}
bool DicomLoader::imageProcessingApplied() const
{
    return m_imageProcessingApplied;
}

// Save
bool DicomLoader::saveProcessedDicom(
        const QString &outputPath)
{
    qDebug() << "================================";
    qDebug() << "SAVE PROCESSED DICOM";
    qDebug() << "================================";

    if (!m_loaded)
    {
        qDebug() << "SAVE: No DICOM loaded";
        return false;
    }

    if (m_filePath.isEmpty())
    {
        qDebug() << "SAVE: Original file path is empty";
        return false;
    }

    // =====================================================
    // SELECT IMAGE TO SAVE
    // =====================================================

    QImage imageToSave;

    if (m_imageProcessingApplied &&
        !m_processedImage.isNull())
    {
        qDebug() << "SAVE: Using processed image";

        imageToSave = m_processedImage;
    }
    else
    {
        qDebug() << "SAVE: Using original image";

        imageToSave = m_image;
    }

    // =====================================================
    // ENSURE 8-BIT GRAYSCALE
    // =====================================================

    imageToSave =
            imageToSave.convertToFormat(
                QImage::Format_Grayscale8);

    if (imageToSave.isNull())
    {
        qDebug() << "SAVE: Image is empty";
        return false;
    }

    // =====================================================
    // LOAD ORIGINAL DICOM
    // This preserves metadata
    // =====================================================

    DcmFileFormat fileFormat;

    const QByteArray originalPath =
            QFile::encodeName(m_filePath);

    OFCondition status =
            fileFormat.loadFile(
                originalPath.constData());

    if (status.bad())
    {
        qDebug()
                << "SAVE: Failed to load original DICOM:"
                << status.text();

        return false;
    }

    DcmDataset *dataset =
            fileFormat.getDataset();

    if (!dataset)
    {
        qDebug() << "SAVE: Dataset is null";

        return false;
    }
    // =====================================================
    // READ ORIGINAL DICOM BIT DEPTH
    // =====================================================

    Uint16 originalBitsAllocated = 8;
    Uint16 originalBitsStored = 8;
    Uint16 originalHighBit = 7;
    Uint16 originalPixelRepresentation = 0;

    dataset->findAndGetUint16(
                DCM_BitsAllocated,
                originalBitsAllocated);

    dataset->findAndGetUint16(
                DCM_BitsStored,
                originalBitsStored);

    dataset->findAndGetUint16(
                DCM_HighBit,
                originalHighBit);

    dataset->findAndGetUint16(
                DCM_PixelRepresentation,
                originalPixelRepresentation);

    qDebug() << "================================";
    qDebug() << "ORIGINAL DICOM BIT DEPTH";
    qDebug() << "Bits Allocated:" << originalBitsAllocated;
    qDebug() << "Bits Stored:" << originalBitsStored;
    qDebug() << "High Bit:" << originalHighBit;
    qDebug() << "Pixel Representation:"
             << originalPixelRepresentation;
    qDebug() << "================================";

    // =====================================================
    // IMAGE INFORMATION
    // =====================================================

    const Uint16 rows =
            static_cast<Uint16>(
                imageToSave.height());

    const Uint16 columns =
            static_cast<Uint16>(
                imageToSave.width());

    qDebug() << "Saving image size:"
             << columns
             << "x"
             << rows;

    // =====================================================
    // UPDATE DICOM IMAGE TAGS
    // =====================================================

    dataset->putAndInsertUint16(
                DCM_Rows,
                rows);

    dataset->putAndInsertUint16(
                DCM_Columns,
                columns);

    dataset->putAndInsertUint16(
                DCM_SamplesPerPixel,
                1);

    dataset->putAndInsertString(
                DCM_PhotometricInterpretation,
                "MONOCHROME2");
    // =====================================================
    // PRESERVE ORIGINAL BIT DEPTH
    // =====================================================

    const bool is16Bit =
            originalBitsAllocated > 8;

    if (is16Bit)
    {
        qDebug() << "SAVE: Using 16-bit grayscale";

        imageToSave =
                imageToSave.convertToFormat(
                    QImage::Format_Grayscale16);

        dataset->putAndInsertUint16(
                    DCM_BitsAllocated,
                    16);

        dataset->putAndInsertUint16(
                    DCM_BitsStored,
                    originalBitsStored);

        dataset->putAndInsertUint16(
                    DCM_HighBit,
                    originalHighBit);

        dataset->putAndInsertUint16(
                    DCM_PixelRepresentation,
                    originalPixelRepresentation);
    }
    else
    {
        qDebug() << "SAVE: Using 8-bit grayscale";

        imageToSave =
                imageToSave.convertToFormat(
                    QImage::Format_Grayscale8);

        dataset->putAndInsertUint16(
                    DCM_BitsAllocated,
                    8);

        dataset->putAndInsertUint16(
                    DCM_BitsStored,
                    8);

        dataset->putAndInsertUint16(
                    DCM_HighBit,
                    7);

        dataset->putAndInsertUint16(
                    DCM_PixelRepresentation,
                    originalPixelRepresentation);
    }

    // =====================================================
    // PIXEL DATA
    // =====================================================


    const int width =
            imageToSave.width();

    const int height =
            imageToSave.height();

    const int pixelCount =
            width * height;

    if (is16Bit)
    {
        qDebug() << "SAVE: Writing 16-bit pixel data";

        QByteArray pixelBuffer;

        pixelBuffer.resize(
                    pixelCount * 2);

        for (int y = 0;
             y < height;
             ++y)
        {
            const uchar *sourceLine =
                    imageToSave.constScanLine(y);

            char *destinationLine =
                    pixelBuffer.data()
                    + (y * width * 2);

            memcpy(
                        destinationLine,
                        sourceLine,
                        width * 2);
        }

        status =
                dataset->putAndInsertUint16Array(
                    DCM_PixelData,
                    reinterpret_cast<const Uint16 *>(
                        pixelBuffer.constData()),
                    pixelCount);
    }
    else
    {
        qDebug() << "SAVE: Writing 8-bit pixel data";

        QByteArray pixelBuffer;

        pixelBuffer.resize(
                    pixelCount);

        for (int y = 0;
             y < height;
             ++y)
        {
            const uchar *sourceLine =
                    imageToSave.constScanLine(y);

            char *destinationLine =
                    pixelBuffer.data()
                    + (y * width);

            memcpy(
                        destinationLine,
                        sourceLine,
                        width);
        }

        status =
                dataset->putAndInsertUint8Array(
                    DCM_PixelData,
                    reinterpret_cast<const Uint8 *>(
                        pixelBuffer.constData()),
                    pixelCount);
    }
    // =====================================================
    // SAVE FILE
    // =====================================================

    const QByteArray savePath =
            QFile::encodeName(outputPath);

    status =
            fileFormat.saveFile(
                savePath.constData(),
                EXS_LittleEndianExplicit);

    if (status.bad())
    {
        qDebug()
                << "SAVE: Failed:"
                << status.text();

        return false;
    }

    qDebug() << "================================";
    qDebug() << "DICOM SAVED SUCCESSFULLY";
    qDebug() << "Path:" << outputPath;
    qDebug() << "Image:"
             << imageToSave.width()
             << "x"
             << imageToSave.height();
    qDebug() << "================================";
    setStatus(
            "Saved",
            "saved");

    return true;
}

// Histgram
QVariantList DicomLoader::histogramData() const
{
    return m_histogramData;
}


qulonglong DicomLoader::histogramCount() const
{
    return m_histogramCount;
}


double DicomLoader::histogramMean() const
{
    return m_histogramMean;
}


double DicomLoader::histogramStdDev() const
{
    return m_histogramStdDev;
}


int DicomLoader::histogramMin() const
{
    return m_histogramMin;
}


int DicomLoader::histogramMax() const
{
    return m_histogramMax;
}


int DicomLoader::histogramMode() const
{
    return m_histogramMode;
}


qulonglong DicomLoader::histogramModeCount() const
{
    return m_histogramModeCount;
}


int DicomLoader::histogramBins() const
{
    return m_histogramBins;
}


double DicomLoader::histogramBinWidth() const
{
    return m_histogramBinWidth;
}

void DicomLoader::calculateHistogram()
{
    qDebug() << "================================";
    qDebug() << "CALCULATE HISTOGRAM";
    qDebug() << "================================";
DebugTimer timer(__FUNCTION__);
    // =====================================================
    // CHECK IMAGE
    // =====================================================

    if (m_image.isNull())
    {
        qDebug() << "HISTOGRAM: No image loaded";
        return;
    }

    // =====================================================
    // SELECT IMAGE
    // =====================================================

    QImage imageToAnalyze;

    if (m_imageProcessingApplied &&
        !m_processedImage.isNull())
    {
        qDebug() << "HISTOGRAM: Using processed image";

        imageToAnalyze = m_processedImage;
    }
    else
    {
        qDebug() << "HISTOGRAM: Using original image";

        imageToAnalyze = m_image;
    }

    // =====================================================
    // DETECT BIT DEPTH
    //
    // Use actual QImage format.
    // Do not depend on m_bitsAllocated here.
    // =====================================================

    const bool is16Bit =
        imageToAnalyze.format() ==
        QImage::Format_Grayscale16;

    // =====================================================
    // CONVERT IMAGE FORMAT
    // =====================================================

    if (is16Bit)
    {
        qDebug() << "HISTOGRAM: Using 16-bit grayscale";

        imageToAnalyze =
            imageToAnalyze.convertToFormat(
                QImage::Format_Grayscale16);
    }
    else
    {
        qDebug() << "HISTOGRAM: Using 8-bit grayscale";

        imageToAnalyze =
            imageToAnalyze.convertToFormat(
                QImage::Format_Grayscale8);
    }

    // =====================================================
    // CHECK CONVERSION
    // =====================================================

    if (imageToAnalyze.isNull())
    {
        qDebug() << "HISTOGRAM: Image conversion failed";
        return;
    }

    // =====================================================
    // HISTOGRAM CONFIGURATION
    // =====================================================

    const int displayBins = 256;

    const int rawBins =
        is16Bit
        ? 65536
        : 256;

    // =====================================================
    // DISPLAY HISTOGRAM
    //
    // Always 256 bins for QML display.
    // =====================================================

    QVector<qulonglong> histogram(
        displayBins,
        0);

    // =====================================================
    // RAW HISTOGRAM
    //
    // 16-bit -> 65536 bins
    // 8-bit  -> 256 bins
    // =====================================================

    QVector<qulonglong> rawHistogram(
        rawBins,
        0);

    // =====================================================
    // IMAGE SIZE
    // =====================================================

    const int width =
        imageToAnalyze.width();

    const int height =
        imageToAnalyze.height();

    // =====================================================
    // TOTAL PIXEL COUNT
    // =====================================================

    m_histogramCount =
        static_cast<qulonglong>(width)
        *
        static_cast<qulonglong>(height);

    // =====================================================
    // INITIALIZE STATISTICS
    // =====================================================

    quint64 sum = 0;

    long double sumSquares = 0.0;

    m_histogramMin =
        is16Bit
        ? 65535
        : 255;

    m_histogramMax = 0;

    // =====================================================
    // READ 16-BIT PIXELS
    // =====================================================

    if (is16Bit)
    {
        qDebug()
            << "HISTOGRAM: Reading 16-bit pixels";

        for (int y = 0;
             y < height;
             ++y)
        {
            const ushort *line =
                reinterpret_cast<const ushort *>(
                    imageToAnalyze.constScanLine(y));

            for (int x = 0;
                 x < width;
                 ++x)
            {
                const int value =
                    static_cast<int>(line[x]);

                // -----------------------------------------
                // Raw 16-bit histogram
                // -----------------------------------------

                rawHistogram[value]++;

                // -----------------------------------------
                // Map 16-bit value to 256 display bins
                //
                // 0     - 255     -> bin 0
                // 256   - 511     -> bin 1
                // ...
                // 65280 - 65535   -> bin 255
                // -----------------------------------------

                int bin =
                    (value * displayBins)
                    / 65536;

                if (bin >= displayBins)
                {
                    bin =
                        displayBins - 1;
                }

                histogram[bin]++;

                // -----------------------------------------
                // Statistics
                // -----------------------------------------

                sum +=
                    static_cast<quint64>(value);

                sumSquares +=
                    static_cast<long double>(value)
                    *
                    static_cast<long double>(value);

                // -----------------------------------------
                // Minimum
                // -----------------------------------------

                if (value < m_histogramMin)
                {
                    m_histogramMin = value;
                }

                // -----------------------------------------
                // Maximum
                // -----------------------------------------

                if (value > m_histogramMax)
                {
                    m_histogramMax = value;
                }
            }
        }
    }

    // =====================================================
    // READ 8-BIT PIXELS
    // =====================================================

    else
    {
        qDebug()
            << "HISTOGRAM: Reading 8-bit pixels";

        for (int y = 0;
             y < height;
             ++y)
        {
            const uchar *line =
                imageToAnalyze.constScanLine(y);

            for (int x = 0;
                 x < width;
                 ++x)
            {
                const int value =
                    static_cast<int>(line[x]);

                // -----------------------------------------
                // Raw histogram
                // -----------------------------------------

                rawHistogram[value]++;

                // -----------------------------------------
                // Display histogram
                // -----------------------------------------

                histogram[value]++;

                // -----------------------------------------
                // Statistics
                // -----------------------------------------

                sum +=
                    static_cast<quint64>(value);

                sumSquares +=
                    static_cast<long double>(value)
                    *
                    static_cast<long double>(value);

                // -----------------------------------------
                // Minimum
                // -----------------------------------------

                if (value < m_histogramMin)
                {
                    m_histogramMin = value;
                }

                // -----------------------------------------
                // Maximum
                // -----------------------------------------

                if (value > m_histogramMax)
                {
                    m_histogramMax = value;
                }
            }
        }
    }

    // =====================================================
    // MEAN
    // =====================================================

    if (m_histogramCount > 0)
    {
        m_histogramMean =
            static_cast<double>(sum)
            /
            static_cast<double>(
                m_histogramCount);
    }
    else
    {
        m_histogramMean = 0.0;
    }

    // =====================================================
    // STANDARD DEVIATION
    // =====================================================

    if (m_histogramCount > 0)
    {
        const long double mean =
            static_cast<long double>(
                m_histogramMean);

        long double variance =
            (sumSquares
             /
             static_cast<long double>(
                 m_histogramCount))
            -
            (mean * mean);

        if (variance < 0.0)
        {
            variance = 0.0;
        }

        m_histogramStdDev =
            std::sqrt(
                static_cast<double>(
                    variance));
    }
    else
    {
        m_histogramStdDev = 0.0;
    }

    // =====================================================
    // MODE
    //
    // First find the highest 256-bin display histogram.
    // Then, for 16-bit, find the actual pixel value
    // inside that selected bin.
    // =====================================================

    m_histogramMode = 0;
    m_histogramModeCount = 0;

    int modeBin = 0;

    // -----------------------------------------------------
    // Find highest display histogram bin
    // -----------------------------------------------------

    for (int i = 0;
         i < histogram.size();
         ++i)
    {
        if (histogram[i] >
            m_histogramModeCount)
        {
            m_histogramModeCount =
                histogram[i];

            modeBin = i;
        }
    }

    // -----------------------------------------------------
    // Convert mode bin to actual image value
    // -----------------------------------------------------

    if (is16Bit)
    {
        // Each display bin represents 256
        // 16-bit intensity values.

        const int startValue =
            modeBin * 256;

        const int endValue =
            qMin(
                startValue + 255,
                65535);

        int actualMode =
            startValue;

        qulonglong actualModeCount = 0;

        // -------------------------------------------------
        // Find most frequent actual 16-bit value
        // inside selected mode bin.
        // -------------------------------------------------

        for (int value = startValue;
             value <= endValue;
             ++value)
        {
            if (rawHistogram[value] >
                actualModeCount)
            {
                actualModeCount =
                    rawHistogram[value];

                actualMode =
                    value;
            }
        }

        // Actual 16-bit mode value
        m_histogramMode =
            actualMode;

        // Mode count from display histogram
        m_histogramModeCount =
            histogram[modeBin];
    }
    else
    {
        // For 8-bit:
        // one bin directly represents one value.

        m_histogramMode =
            modeBin;

        m_histogramModeCount =
            histogram[modeBin];
    }

    // =====================================================
    // SEND DISPLAY HISTOGRAM TO QML
    // =====================================================

    m_histogramData.clear();

    for (int i = 0;
         i < displayBins;
         ++i)
    {
        m_histogramData.append(
            QVariant::fromValue(
                static_cast<qulonglong>(
                    histogram[i])));
    }

    // =====================================================
    // HISTOGRAM INFORMATION
    // =====================================================

    m_histogramBins =
        displayBins;

    // -----------------------------------------------------
    // Bin width
    // -----------------------------------------------------

    if (is16Bit)
    {
        m_histogramBinWidth =
            65536.0
            /
            static_cast<double>(
                displayBins);
    }
    else
    {
        m_histogramBinWidth =
            256.0
            /
            static_cast<double>(
                displayBins);
    }

    // =====================================================
    // DEBUG
    // =====================================================

    qDebug()
        << "Histogram Bit Depth:"
        << (is16Bit
            ? "16-bit"
            : "8-bit");

    qDebug()
        << "Histogram Count:"
        << m_histogramCount;

    qDebug()
        << "Min:"
        << m_histogramMin;

    qDebug()
        << "Max:"
        << m_histogramMax;

    qDebug()
        << "Mean:"
        << m_histogramMean;

    qDebug()
        << "StdDev:"
        << m_histogramStdDev;

    qDebug()
        << "Mode:"
        << m_histogramMode;

    qDebug()
        << "Mode Count:"
        << m_histogramModeCount;

    qDebug()
        << "Bins:"
        << m_histogramBins;

    qDebug()
        << "Bin Width:"
        << m_histogramBinWidth;

    qDebug() << "================================";

    emit histogramChanged();
}
void DicomLoader::clearHistogram()
{
    m_histogramData.clear();

    m_histogramCount = 0;

    m_histogramMean = 0.0;
    m_histogramStdDev = 0.0;

    m_histogramMin = 0;
    m_histogramMax = 0;

    m_histogramMode = 0;
    m_histogramModeCount = 0;

    m_histogramBins = 0;
    m_histogramBinWidth = 0.0;


        qDebug() << "HISTOGRAM DATA CLEARED";


    emit histogramChanged();
}


// Status

QString DicomLoader::statusText() const
{
    return m_statusText;
}
QString DicomLoader::statusType() const
{
    return m_statusType;
}
bool DicomLoader::isModified() const
{
    return m_modified;
}

void DicomLoader::setStatus(
        const QString &text,
        const QString &type)
{
    bool changed = false;

    if (m_statusText != text)
    {
        m_statusText = text;
        changed = true;
    }

    if (m_statusType != type)
    {
        m_statusType = type;
        changed = true;
    }

    if (changed)
    {
        qDebug() << "STATUS:"
                 << m_statusText
                 << "|"
                 << m_statusType;

        emit statusChanged();
    }
}
