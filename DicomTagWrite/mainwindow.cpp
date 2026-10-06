#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <gdcmUIDGenerator.h>
#include <itkCommand.h>
#include "itkImageFileWriter.h"
#include <itkGDCMImageIO.h>
#include "itkMetaDataObject.h"
#include <itkMetaDataDictionary.h>
#include "itkImportImageFilter.h"
#include "itkImage.h"

// DCMTK includes
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/ofstd/ofcond.h"
#include "dcmtk/ofstd/ofstring.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_RF_modality_clicked()
{
    qDebug() << "=== START RF MODALITY (DCMTK) ===";

    // Get multiple files
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Open RF Image Sequence",
                                                          "D:/Workflow/Test images/*.raw",
                                                          QDir::currentPath());
    if (filenames.isEmpty())
    {
        qDebug()<<"Could not open files";
        return;
    }

    int numberOfFrames = filenames.size();
    qDebug()<<"Number of frames:"<<numberOfFrames;
    qDebug()<<"w:"<<w<<"h:"<<h;
    qDebug()<<"Frame size in bytes:"<<w * h * sizeof(unsigned short);

    // Read all images into a vector - store as unsigned short directly
    std::vector<unsigned short*> imageFrames;

    for (int frameIdx = 0; frameIdx < numberOfFrames; frameIdx++)
    {
        qDebug() << "Reading frame" << frameIdx << ":" << filenames[frameIdx];

        FILE* Img = fopen(filenames[frameIdx].toStdString().c_str(), "rb");
        if (!Img) {
            qDebug()<<"Failed to open file:"<<filenames[frameIdx];
            continue;
        }

        // Allocate memory for unsigned short (2 bytes per pixel)
        unsigned short* image = new (std::nothrow) unsigned short[w * h];
        if (!image) {
            qDebug() << "Failed to allocate memory for frame" << frameIdx;
            fclose(Img);
            continue;
        }

        size_t bytesRead = fread(image, sizeof(unsigned short), w * h, Img);
        qDebug() << "Read" << bytesRead << "pixels (expected" << w * h << ")";

        fclose(Img);

        if (bytesRead != static_cast<size_t>(w * h)) {
            qDebug()<<"Failed to read complete file:"<<filenames[frameIdx];
            delete[] image;
            continue;
        }

        // Verify first few pixel values
        qDebug() << "Frame" << frameIdx << "first 5 pixels:"
                 << image[0] << image[1] << image[2] << image[3] << image[4];

        imageFrames.push_back(image);
        qDebug() << "Frame" << frameIdx << "loaded successfully";
    }

    if (imageFrames.empty()) {
        qDebug()<<"No valid images loaded";
        return;
    }

    numberOfFrames = imageFrames.size();
    qDebug() << "Successfully loaded" << numberOfFrames << "frames";

    // Combine all frames into a single buffer for DCMTK
    size_t totalPixels = static_cast<size_t>(w) * static_cast<size_t>(h) * static_cast<size_t>(numberOfFrames);
    qDebug() << "Total pixels:" << totalPixels;

    Uint16* pixelData = new Uint16[totalPixels];
    if (!pixelData) {
        qDebug() << "Failed to allocate pixel data buffer";
        for (unsigned short* frame : imageFrames) {
            delete[] frame;
        }
        imageFrames.clear();
        return;
    }

    // Copy all frames into the combined buffer
    for (int frameIdx = 0; frameIdx < numberOfFrames; frameIdx++) {
        size_t offset = static_cast<size_t>(frameIdx) * static_cast<size_t>(w) * static_cast<size_t>(h);
        memcpy(pixelData + offset,
               imageFrames[frameIdx],
               static_cast<size_t>(w) * static_cast<size_t>(h) * sizeof(unsigned short));
        delete[] imageFrames[frameIdx];
    }
    imageFrames.clear();

    qDebug() << "All frames combined into single buffer";

    // ============ DCMTK WRITING ============
    qDebug() << "Creating DICOM with DCMTK...";

    // Create file format object
    DcmFileFormat fileformat;
    DcmDataset *dataset = fileformat.getDataset();

    // Generate UIDs
    char studyUID[100], seriesUID[100], instanceUID[100];
    dcmGenerateUniqueIdentifier(studyUID, SITE_STUDY_UID_ROOT);
    dcmGenerateUniqueIdentifier(seriesUID, SITE_SERIES_UID_ROOT);
    dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

    qDebug() << "Generated UIDs:";
    qDebug() << "Study UID:" << studyUID;
    qDebug() << "Series UID:" << seriesUID;
    qDebug() << "Instance UID:" << instanceUID;

    // ===== PATIENT INFORMATION =====
    qDebug() << "Adding Patient Information...";
    OFCondition status;

    status = dataset->putAndInsertString(DCM_PatientName, "Aravinth");
    if (status.bad()) qDebug() << "Error adding PatientName:" << status.text();

    status = dataset->putAndInsertString(DCM_PatientID, "700");
    if (status.bad()) qDebug() << "Error adding PatientID:" << status.text();

    status = dataset->putAndInsertString(DCM_PatientBirthDate, "19950921");
    if (status.bad()) qDebug() << "Error adding PatientBirthDate:" << status.text();

    status = dataset->putAndInsertString(DCM_PatientSex, "M");
    if (status.bad()) qDebug() << "Error adding PatientSex:" << status.text();

    // ===== STUDY INFORMATION =====
    qDebug() << "Adding Study Information...";
    QString date = QDate::currentDate().toString("yyyyMMdd");
    QString time = QTime::currentTime().toString("hhmmss");

    status = dataset->putAndInsertString(DCM_StudyDate, date.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding StudyDate:" << status.text();

    status = dataset->putAndInsertString(DCM_StudyTime, time.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding StudyTime:" << status.text();

    status = dataset->putAndInsertString(DCM_StudyInstanceUID, studyUID);
    if (status.bad()) qDebug() << "Error adding StudyInstanceUID:" << status.text();

    status = dataset->putAndInsertString(DCM_StudyDescription, "RF Cine Study");
    if (status.bad()) qDebug() << "Error adding StudyDescription:" << status.text();

    status = dataset->putAndInsertString(DCM_StudyID, "1");
    if (status.bad()) qDebug() << "Error adding StudyID:" << status.text();

    // ===== SERIES INFORMATION =====
    qDebug() << "Adding Series Information...";
    status = dataset->putAndInsertString(DCM_Modality, "RF");
    if (status.bad()) qDebug() << "Error adding Modality:" << status.text();

    status = dataset->putAndInsertString(DCM_SeriesInstanceUID, seriesUID);
    if (status.bad()) qDebug() << "Error adding SeriesInstanceUID:" << status.text();

    status = dataset->putAndInsertString(DCM_SeriesDescription, "RF Cine Series");
    if (status.bad()) qDebug() << "Error adding SeriesDescription:" << status.text();

    status = dataset->putAndInsertString(DCM_SeriesNumber, "1");
    if (status.bad()) qDebug() << "Error adding SeriesNumber:" << status.text();

    // ===== FRAME OF REFERENCE =====
    qDebug() << "Adding Frame of Reference...";
    char frefUID[100];
    dcmGenerateUniqueIdentifier(frefUID, SITE_STUDY_UID_ROOT);
    status = dataset->putAndInsertString(DCM_FrameOfReferenceUID, frefUID);
    if (status.bad()) qDebug() << "Error adding FrameOfReferenceUID:" << status.text();

    // ===== EQUIPMENT INFORMATION =====
    qDebug() << "Adding Equipment Information...";
    status = dataset->putAndInsertString(DCM_Manufacturer, "PANACEA MEDICALS");
    if (status.bad()) qDebug() << "Error adding Manufacturer:" << status.text();

    status = dataset->putAndInsertString(DCM_InstitutionName, "ALPHA HOSPITAL");
    if (status.bad()) qDebug() << "Error adding InstitutionName:" << status.text();

    status = dataset->putAndInsertString(DCM_ManufacturerModelName, "RF Imagin");
    if (status.bad()) qDebug() << "Error adding ManufacturerModelName:" << status.text();

    status = dataset->putAndInsertString(DCM_SoftwareVersions, "1.0.1");
    if (status.bad()) qDebug() << "Error adding SoftwareVersions:" << status.text();

    status = dataset->putAndInsertString(DCM_DeviceSerialNumber, "985518756");
    if (status.bad()) qDebug() << "Error adding DeviceSerialNumber:" << status.text();

    // ===== RF SPECIFIC INFORMATION =====
    qDebug() << "Adding RF Specific Information...";
    status = dataset->putAndInsertFloat64(DCM_KVP, 40.0);
    if (status.bad()) qDebug() << "Error adding KVP:" << status.text();

    status = dataset->putAndInsertString(DCM_XRayTubeCurrent, "20");
    if (status.bad()) qDebug() << "Error adding XRayTubeCurrent:" << status.text();

    status = dataset->putAndInsertString(DCM_ExposureTime, "10");
    if (status.bad()) qDebug() << "Error adding ExposureTime:" << status.text();

    // ===== CINE INFORMATION =====
    if (numberOfFrames > 1) {
        qDebug() << "Adding Cine Information...";
        status = dataset->putAndInsertString(DCM_CineRate, "30");
        if (status.bad()) qDebug() << "Error adding CineRate:" << status.text();

        status = dataset->putAndInsertFloat64(DCM_FrameTime, 33.33);
        if (status.bad()) qDebug() << "Error adding FrameTime:" << status.text();
    }

    // ===== NUMBER OF FRAMES =====
    if (numberOfFrames > 1) {
        qDebug() << "Adding Number of Frames...";
        char numFramesStr[10];
        sprintf(numFramesStr, "%d", numberOfFrames);
        status = dataset->putAndInsertString(DCM_NumberOfFrames, numFramesStr);
        if (status.bad()) qDebug() << "Error adding NumberOfFrames:" << status.text();
    }

    status = dataset->putAndInsertString(DCM_DistanceSourceToDetector, "1171");
    if (status.bad()) qDebug() << "Error adding DistanceSourceToDetector:" << status.text();

    status = dataset->putAndInsertString(DCM_DistanceSourceToPatient, "720");
    if (status.bad()) qDebug() << "Error adding DistanceSourceToPatient:" << status.text();

    status = dataset->putAndInsertString(DCM_FieldOfViewShape, "RECTANGLE");
    if (status.bad()) qDebug() << "Error adding FieldOfViewShape:" << status.text();

    status = dataset->putAndInsertString(DCM_FieldOfViewDimensions, "200\\200");
    if (status.bad()) qDebug() << "Error adding FieldOfViewDimensions:" << status.text();

    status = dataset->putAndInsertString(DCM_AveragePulseWidth, "8");
    if (status.bad()) qDebug() << "Error adding AveragePulseWidth:" << status.text();

    status = dataset->putAndInsertString(DCM_RadiationSetting, "GR");
    if (status.bad()) qDebug() << "Error adding RadiationSetting:" << status.text();

    status = dataset->putAndInsertString(DCM_RadiationMode, "Pulsed");
    if (status.bad()) qDebug() << "Error adding RadiationMode:" << status.text();

    status = dataset->putAndInsertString(DCM_CollimatorShape, "RECTANGULAR");
    if (status.bad()) qDebug() << "Error adding CollimatorShape:" << status.text();

    status = dataset->putAndInsertString(DCM_CollimatorLeftVerticalEdge, "6");
    if (status.bad()) qDebug() << "Error adding CollimatorLeftVerticalEdge:" << status.text();

    status = dataset->putAndInsertString(DCM_CollimatorRightVerticalEdge, "1530");
    if (status.bad()) qDebug() << "Error adding CollimatorRightVerticalEdge:" << status.text();

    status = dataset->putAndInsertString(DCM_CollimatorUpperHorizontalEdge, "6");
    if (status.bad()) qDebug() << "Error adding CollimatorUpperHorizontalEdge:" << status.text();

    status = dataset->putAndInsertString(DCM_CollimatorLowerHorizontalEdge, "1530");
    if (status.bad()) qDebug() << "Error adding CollimatorLowerHorizontalEdge:" << status.text();

    // ===== IMAGE INFORMATION =====
    qDebug() << "Adding Image Information...";
    status = dataset->putAndInsertString(DCM_InstanceNumber, "1");
    if (status.bad()) qDebug() << "Error adding InstanceNumber:" << status.text();

    status = dataset->putAndInsertString(DCM_AcquisitionDate, date.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding AcquisitionDate:" << status.text();

    status = dataset->putAndInsertString(DCM_AcquisitionTime, time.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding AcquisitionTime:" << status.text();

    status = dataset->putAndInsertString(DCM_ContentDate, date.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding ContentDate:" << status.text();

    status = dataset->putAndInsertString(DCM_ContentTime, time.toStdString().c_str());
    if (status.bad()) qDebug() << "Error adding ContentTime:" << status.text();

    // ===== IMAGE TYPE =====
    qDebug() << "Adding Image Type...";
    if (numberOfFrames > 1) {
        status = dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\CINE");
        if (status.bad()) qDebug() << "Error adding ImageType:" << status.text();
    } else {
        status = dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
        if (status.bad()) qDebug() << "Error adding ImageType:" << status.text();
    }

    // ===== IMAGE PIXEL INFORMATION =====
    qDebug() << "Adding Image Pixel Information...";
    status = dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
    if (status.bad()) qDebug() << "Error adding SamplesPerPixel:" << status.text();

    status = dataset->putAndInsertUint16(DCM_Rows, static_cast<Uint16>(h));
    if (status.bad()) qDebug() << "Error adding Rows:" << status.text();

    status = dataset->putAndInsertUint16(DCM_Columns, static_cast<Uint16>(w));
    if (status.bad()) qDebug() << "Error adding Columns:" << status.text();

    status = dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
    if (status.bad()) qDebug() << "Error adding BitsAllocated:" << status.text();

    status = dataset->putAndInsertUint16(DCM_BitsStored, 16);
    if (status.bad()) qDebug() << "Error adding BitsStored:" << status.text();

    status = dataset->putAndInsertUint16(DCM_HighBit, 15);
    if (status.bad()) qDebug() << "Error adding HighBit:" << status.text();

    status = dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
    if (status.bad()) qDebug() << "Error adding PixelRepresentation:" << status.text();

    status = dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
    if (status.bad()) qDebug() << "Error adding PhotometricInterpretation:" << status.text();

    status = dataset->putAndInsertString(DCM_PixelSpacing, "0.319\\0.319");
    if (status.bad()) qDebug() << "Error adding PixelSpacing:" << status.text();

    // ===== RESCALE / WINDOW SETTINGS =====
    qDebug() << "Adding Rescale/Window Settings...";
    status = dataset->putAndInsertFloat64(DCM_RescaleIntercept, 0.0);
    if (status.bad()) qDebug() << "Error adding RescaleIntercept:" << status.text();

    status = dataset->putAndInsertFloat64(DCM_RescaleSlope, 1.0);
    if (status.bad()) qDebug() << "Error adding RescaleSlope:" << status.text();

    status = dataset->putAndInsertString(DCM_RescaleType, "US");
    if (status.bad()) qDebug() << "Error adding RescaleType:" << status.text();

    status = dataset->putAndInsertFloat64(DCM_WindowCenter, 32768.0);
    if (status.bad()) qDebug() << "Error adding WindowCenter:" << status.text();

    status = dataset->putAndInsertFloat64(DCM_WindowWidth, 65535.0);
    if (status.bad()) qDebug() << "Error adding WindowWidth:" << status.text();

    // ===== SOP COMMON =====
    qDebug() << "Adding SOP Common...";
    const char* sopClassUID = UID_XRayRadiofluoroscopicImageStorage;
    status = dataset->putAndInsertString(DCM_SOPClassUID, sopClassUID);
    if (status.bad()) qDebug() << "Error adding SOPClassUID:" << status.text();

    status = dataset->putAndInsertString(DCM_SOPInstanceUID, instanceUID);
    if (status.bad()) qDebug() << "Error adding SOPInstanceUID:" << status.text();

    // ===== CHARACTER SET =====
    qDebug() << "Adding Character Set...";
    status = dataset->putAndInsertString(DCM_SpecificCharacterSet, "ISO_IR_100");
    if (status.bad()) qDebug() << "Error adding SpecificCharacterSet:" << status.text();

    // ===== PATIENT ORIENTATION =====
    qDebug() << "Adding Patient Orientation...";
    status = dataset->putAndInsertString(DCM_PatientOrientation, "");
    if (status.bad()) qDebug() << "Error adding PatientOrientation:" << status.text();

    // ===== POSITIONER INFORMATION =====
    qDebug() << "Adding Positioner Information...";
    status = dataset->putAndInsertFloat64(DCM_GantryAngle, 0.0);
    if (status.bad()) qDebug() << "Error adding GantryAngle:" << status.text();

    // ===== INSERT PIXEL DATA =====
    qDebug() << "Inserting Pixel Data...";
    status = dataset->putAndInsertUint16Array(DCM_PixelData, pixelData, totalPixels);
    if (status.bad()) {
        qDebug() << "Error inserting PixelData:" << status.text();
    } else {
        qDebug() << "Pixel Data inserted successfully";
    }

    // ===== WRITE FILE =====
    qDebug() << "Writing DICOM file...";
    static int fileCounter = 0;
    QString path = "D:\\ImageWrite\\RFWrite";

    // Create directory if it doesn't exist
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Creating directory:" << path;
        dir.mkpath(".");
    }

    QString filename = path + "\\RF_Cine_" + QString::number(++fileCounter) + ".dcm";
    qDebug() << "Writing to:" << filename;

    // Save the file
    status = fileformat.saveFile(filename.toStdString().c_str(), EXS_LittleEndianExplicit);

    if (status.bad()) {
        qDebug() << "DICOM Image Write Error:" << status.text();
    } else {
        qDebug() << "DICOM file written successfully!";
    }

    // Clean up
    qDebug() << "Cleaning up memory...";
    delete[] pixelData;

    qDebug() << "=== RF MODALITY COMPLETE ===";
}

void MainWindow::on_SaveRaw_clicked()
{
    qDebug() << "=== START RF DICOM READ & SAVE AS RAW ===";

       // Select DICOM file to read
       QString filename = QFileDialog::getOpenFileName(this,
                                                       "Open RF DICOM File",
                                                       "D:/ImageWrite",
                                                       "DICOM Files (*.dcm)");
       if (filename.isEmpty()) {
           qDebug()<<"No file selected";
           return;
       }

       // ===== READ DICOM FILE =====
       qDebug() << "Reading DICOM file:" << filename;

       DcmFileFormat fileformat;
       OFCondition status = fileformat.loadFile(filename.toStdString().c_str());

       if (status.bad()) {
           qDebug() << "Error reading DICOM file:" << status.text();
           return;
       }

       DcmDataset *dataset = fileformat.getDataset();

       // ===== EXTRACT IMAGE DIMENSIONS =====
       Uint16 rows = 0, columns = 0;
       dataset->findAndGetUint16(DCM_Rows, rows);
       dataset->findAndGetUint16(DCM_Columns, columns);

       if (rows == 0 || columns == 0) {
           qDebug() << "Error: Invalid image dimensions";
           return;
       }

       qDebug() << "Image dimensions:" << rows << "x" << columns;

       // ===== GET NUMBER OF FRAMES =====
       int numberOfFrames = 1;
       OFString numberOfFramesStr;
       if (dataset->findAndGetOFString(DCM_NumberOfFrames, numberOfFramesStr).good()) {
           numberOfFrames = atoi(numberOfFramesStr.c_str());
       }
       qDebug() << "Number of frames:" << numberOfFrames;

       // ===== EXTRACT PIXEL DATA =====
       qDebug() << "Extracting pixel data...";

       DcmElement *pixelElement = NULL;
       status = dataset->findAndGetElement(DCM_PixelData, pixelElement);
       if (status.bad() || pixelElement == NULL) {
           qDebug() << "Error: Could not find pixel data";
           return;
       }

       Uint32 pixelDataLength = pixelElement->getLength();
       if (pixelDataLength == 0) {
           qDebug() << "Error: Pixel data is empty";
           return;
       }

       qDebug() << "Pixel data length:" << pixelDataLength << "bytes";

       // ===== COPY PIXEL DATA TO OUR OWN BUFFER =====
       // We need to copy the data, not just use the pointer
       Uint8* pixelData = new Uint8[pixelDataLength];
       if (!pixelData) {
           qDebug() << "Error: Failed to allocate memory for pixel data";
           return;
       }

       // Get the pixel data and copy it
       Uint8* internalData = NULL;
       status = pixelElement->getUint8Array(internalData);
       if (status.bad() || internalData == NULL) {
           qDebug() << "Error: Failed to get pixel data:" << status.text();
           delete[] pixelData;
           return;
       }

       // Copy the data to our buffer
       memcpy(pixelData, internalData, pixelDataLength);
       qDebug() << "Pixel data copied successfully!";

       // ===== SAVE AS RAW IMAGES =====
       qDebug() << "Saving raw images...";

       // Create output directory
       QString outputPath = "D:/ImageWrite/Raw_Output";
       QDir dir(outputPath);
       if (!dir.exists()) {
           qDebug() << "Creating directory:" << outputPath;
           dir.mkpath(".");
       }

       // Calculate frame size (assuming 16-bit data)
       size_t frameSize = static_cast<size_t>(rows) * static_cast<size_t>(columns) * sizeof(unsigned short);
       qDebug() << "Frame size:" << frameSize << "bytes";

       // Save each frame as separate raw file
       for (int frameIdx = 0; frameIdx < numberOfFrames; frameIdx++) {
           QString rawFilename = outputPath + "/proj_" +
                                QString::number(frameIdx).rightJustified(1, '0') + ".raw";

           FILE* rawFile = fopen(rawFilename.toStdString().c_str(), "wb");
           if (!rawFile) {
               qDebug() << "Error: Could not create raw file:" << rawFilename;
               continue;
           }

           size_t offset = frameIdx * frameSize;
           size_t bytesWritten = fwrite(pixelData + offset, 1, frameSize, rawFile);
           fclose(rawFile);

           if (bytesWritten == frameSize) {
               qDebug() << "Frame" << frameIdx << "saved successfully";
           } else {
               qDebug() << "Warning: Frame" << frameIdx << "only wrote" << bytesWritten << "of" << frameSize << "bytes";
           }
       }

       // Clean up - delete our copy of the data
       delete[] pixelData;
       pixelData = NULL;

       qDebug() << "=== RF DICOM READ & SAVE COMPLETE ===";
       qDebug() << "Raw files saved to:" << outputPath;
}

void MainWindow::on_XA_modality_clicked()
{
    qDebug() << "=== START XA MODALITY (DCMTK) ===";

        // Get multiple files
        QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                              "Open XA Image Sequence",
                                                              "D:/Workflow/Test images/*.raw",
                                                              QDir::currentPath());
        if (filenames.isEmpty())
        {
            qDebug()<<"Could not open files";
            return;
        }

        int numberOfFrames = filenames.size();
        qDebug()<<"Number of frames:"<<numberOfFrames;
        qDebug()<<"w:"<<w<<"h:"<<h;
        qDebug()<<"Frame size in bytes:"<<w * h * sizeof(unsigned short);

        // Read all images into a vector - store as unsigned short directly
        std::vector<unsigned short*> imageFrames;

        for (int frameIdx = 0; frameIdx < numberOfFrames; frameIdx++)
        {
            qDebug() << "Reading frame" << frameIdx << ":" << filenames[frameIdx];

            FILE* Img = fopen(filenames[frameIdx].toStdString().c_str(), "rb");
            if (!Img) {
                qDebug()<<"Failed to open file:"<<filenames[frameIdx];
                continue;
            }

            // Allocate memory for unsigned short (2 bytes per pixel)
            unsigned short* image = new (std::nothrow) unsigned short[w * h];
            if (!image) {
                qDebug() << "Failed to allocate memory for frame" << frameIdx;
                fclose(Img);
                continue;
            }

            size_t bytesRead = fread(image, sizeof(unsigned short), w * h, Img);
            qDebug() << "Read" << bytesRead << "pixels (expected" << w * h << ")";

            fclose(Img);

            if (bytesRead != static_cast<size_t>(w * h)) {
                qDebug()<<"Failed to read complete file:"<<filenames[frameIdx];
                delete[] image;
                continue;
            }

            // Verify first few pixel values
            qDebug() << "Frame" << frameIdx << "first 5 pixels:"
                     << image[0] << image[1] << image[2] << image[3] << image[4];

            imageFrames.push_back(image);
            qDebug() << "Frame" << frameIdx << "loaded successfully";
        }

        if (imageFrames.empty()) {
            qDebug()<<"No valid images loaded";
            return;
        }

        numberOfFrames = imageFrames.size();
        qDebug() << "Successfully loaded" << numberOfFrames << "frames";

        // Combine all frames into a single buffer for DCMTK
        size_t totalPixels = static_cast<size_t>(w) * static_cast<size_t>(h) * static_cast<size_t>(numberOfFrames);
        qDebug() << "Total pixels:" << totalPixels;

        Uint16* pixelData = new Uint16[totalPixels];
        if (!pixelData) {
            qDebug() << "Failed to allocate pixel data buffer";
            for (unsigned short* frame : imageFrames) {
                delete[] frame;
            }
            imageFrames.clear();
            return;
        }

        // Copy all frames into the combined buffer
        for (int frameIdx = 0; frameIdx < numberOfFrames; frameIdx++) {
            size_t offset = static_cast<size_t>(frameIdx) * static_cast<size_t>(w) * static_cast<size_t>(h);
            memcpy(pixelData + offset,
                   imageFrames[frameIdx],
                   static_cast<size_t>(w) * static_cast<size_t>(h) * sizeof(unsigned short));
            delete[] imageFrames[frameIdx];
        }
        imageFrames.clear();

        qDebug() << "All frames combined into single buffer";

        // ============ DCMTK WRITING ============
        qDebug() << "Creating DICOM with DCMTK...";

        // Create file format object
        DcmFileFormat fileformat;
        DcmDataset *dataset = fileformat.getDataset();

        // Generate UIDs
        char studyUID[100], seriesUID[100], instanceUID[100];
        dcmGenerateUniqueIdentifier(studyUID, SITE_STUDY_UID_ROOT);
        dcmGenerateUniqueIdentifier(seriesUID, SITE_SERIES_UID_ROOT);
        dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

        qDebug() << "Generated UIDs:";
        qDebug() << "Study UID:" << studyUID;
        qDebug() << "Series UID:" << seriesUID;
        qDebug() << "Instance UID:" << instanceUID;

        // ===== PATIENT INFORMATION =====
        qDebug() << "Adding Patient Information...";
        OFCondition status;

        status = dataset->putAndInsertString(DCM_PatientName, "Aravinth");
        if (status.bad()) qDebug() << "Error adding PatientName:" << status.text();

        status = dataset->putAndInsertString(DCM_PatientID, "700");
        if (status.bad()) qDebug() << "Error adding PatientID:" << status.text();

        status = dataset->putAndInsertString(DCM_PatientBirthDate, "19950921");
        if (status.bad()) qDebug() << "Error adding PatientBirthDate:" << status.text();

        status = dataset->putAndInsertString(DCM_PatientSex, "M");
        if (status.bad()) qDebug() << "Error adding PatientSex:" << status.text();

        // ===== STUDY INFORMATION =====
        qDebug() << "Adding Study Information...";
        QString date = QDate::currentDate().toString("yyyyMMdd");
        QString time = QTime::currentTime().toString("hhmmss");

        status = dataset->putAndInsertString(DCM_StudyDate, date.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding StudyDate:" << status.text();

        status = dataset->putAndInsertString(DCM_StudyTime, time.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding StudyTime:" << status.text();

        status = dataset->putAndInsertString(DCM_StudyInstanceUID, studyUID);
        if (status.bad()) qDebug() << "Error adding StudyInstanceUID:" << status.text();

        // ===== CHANGE: Study Description for XA =====
        status = dataset->putAndInsertString(DCM_StudyDescription, "XA Cine Study");
        if (status.bad()) qDebug() << "Error adding StudyDescription:" << status.text();

        status = dataset->putAndInsertString(DCM_StudyID, "1");
        if (status.bad()) qDebug() << "Error adding StudyID:" << status.text();

        // ===== SERIES INFORMATION =====
        qDebug() << "Adding Series Information...";
        // ===== CHANGE: Modality to XA =====
        status = dataset->putAndInsertString(DCM_Modality, "XA");
        if (status.bad()) qDebug() << "Error adding Modality:" << status.text();

        status = dataset->putAndInsertString(DCM_SeriesInstanceUID, seriesUID);
        if (status.bad()) qDebug() << "Error adding SeriesInstanceUID:" << status.text();

        // ===== CHANGE: Series Description for XA =====
        status = dataset->putAndInsertString(DCM_SeriesDescription, "XA Cine Series");
        if (status.bad()) qDebug() << "Error adding SeriesDescription:" << status.text();

        status = dataset->putAndInsertString(DCM_SeriesNumber, "1");
        if (status.bad()) qDebug() << "Error adding SeriesNumber:" << status.text();

        // ===== FRAME OF REFERENCE =====
        qDebug() << "Adding Frame of Reference...";
        char frefUID[100];
        dcmGenerateUniqueIdentifier(frefUID, SITE_STUDY_UID_ROOT);
        status = dataset->putAndInsertString(DCM_FrameOfReferenceUID, frefUID);
        if (status.bad()) qDebug() << "Error adding FrameOfReferenceUID:" << status.text();

        // ===== EQUIPMENT INFORMATION =====
        qDebug() << "Adding Equipment Information...";
        status = dataset->putAndInsertString(DCM_Manufacturer, "PANACEA MEDICALS");
        if (status.bad()) qDebug() << "Error adding Manufacturer:" << status.text();

        status = dataset->putAndInsertString(DCM_InstitutionName, "ALPHA HOSPITAL");
        if (status.bad()) qDebug() << "Error adding InstitutionName:" << status.text();

        status = dataset->putAndInsertString(DCM_ManufacturerModelName, "XA Imagin");
        if (status.bad()) qDebug() << "Error adding ManufacturerModelName:" << status.text();

        status = dataset->putAndInsertString(DCM_SoftwareVersions, "1.0.1");
        if (status.bad()) qDebug() << "Error adding SoftwareVersions:" << status.text();

        status = dataset->putAndInsertString(DCM_DeviceSerialNumber, "985518756");
        if (status.bad()) qDebug() << "Error adding DeviceSerialNumber:" << status.text();

        // ===== XA SPECIFIC INFORMATION =====
        qDebug() << "Adding XA Specific Information...";
        status = dataset->putAndInsertFloat64(DCM_KVP, 40.0);
        if (status.bad()) qDebug() << "Error adding KVP:" << status.text();

        status = dataset->putAndInsertString(DCM_XRayTubeCurrent, "20.0");
        if (status.bad()) qDebug() << "Error adding XRayTubeCurrent:" << status.text();

        status = dataset->putAndInsertString(DCM_ExposureTime, "10.0");
        if (status.bad()) qDebug() << "Error adding ExposureTime:" << status.text();

        // ===== CINE INFORMATION =====
        if (numberOfFrames > 1) {
            qDebug() << "Adding Cine Information...";
            status = dataset->putAndInsertString(DCM_CineRate, "30.0");
            if (status.bad()) qDebug() << "Error adding CineRate:" << status.text();

            status = dataset->putAndInsertFloat64(DCM_FrameTime, 33.33);
            if (status.bad()) qDebug() << "Error adding FrameTime:" << status.text();
        }

        status = dataset->putAndInsertString(DCM_DistanceSourceToDetector, "1171");
        if (status.bad()) qDebug() << "Error adding DistanceSourceToDetector:" << status.text();

        status = dataset->putAndInsertString(DCM_DistanceSourceToPatient, "720");
        if (status.bad()) qDebug() << "Error adding DistanceSourceToPatient:" << status.text();

        status = dataset->putAndInsertString(DCM_FieldOfViewShape, "RECTANGLE");
        if (status.bad()) qDebug() << "Error adding FieldOfViewShape:" << status.text();

        status = dataset->putAndInsertString(DCM_FieldOfViewDimensions, "200\\200");
        if (status.bad()) qDebug() << "Error adding FieldOfViewDimensions:" << status.text();

        status = dataset->putAndInsertString(DCM_AveragePulseWidth, "8");
        if (status.bad()) qDebug() << "Error adding AveragePulseWidth:" << status.text();

        status = dataset->putAndInsertString(DCM_RadiationSetting, "GR");
        if (status.bad()) qDebug() << "Error adding RadiationSetting:" << status.text();

        status = dataset->putAndInsertString(DCM_RadiationMode, "Pulsed");
        if (status.bad()) qDebug() << "Error adding RadiationMode:" << status.text();

        status = dataset->putAndInsertString(DCM_CollimatorShape, "RECTANGULAR");
        if (status.bad()) qDebug() << "Error adding CollimatorShape:" << status.text();

        status = dataset->putAndInsertString(DCM_CollimatorLeftVerticalEdge, "6");
        if (status.bad()) qDebug() << "Error adding CollimatorLeftVerticalEdge:" << status.text();

        status = dataset->putAndInsertString(DCM_CollimatorRightVerticalEdge, "1530");
        if (status.bad()) qDebug() << "Error adding CollimatorRightVerticalEdge:" << status.text();

        status = dataset->putAndInsertString(DCM_CollimatorUpperHorizontalEdge, "6");
        if (status.bad()) qDebug() << "Error adding CollimatorUpperHorizontalEdge:" << status.text();

        status = dataset->putAndInsertString(DCM_CollimatorLowerHorizontalEdge, "1530");
        if (status.bad()) qDebug() << "Error adding CollimatorLowerHorizontalEdge:" << status.text();

        // ===== NUMBER OF FRAMES =====
        if (numberOfFrames > 1) {
            qDebug() << "Adding Number of Frames...";
            char numFramesStr[10];
            sprintf(numFramesStr, "%d", numberOfFrames);
            status = dataset->putAndInsertString(DCM_NumberOfFrames, numFramesStr);
            if (status.bad()) qDebug() << "Error adding NumberOfFrames:" << status.text();
        }

        // ===== IMAGE INFORMATION =====
        qDebug() << "Adding Image Information...";
        status = dataset->putAndInsertString(DCM_InstanceNumber, "1");
        if (status.bad()) qDebug() << "Error adding InstanceNumber:" << status.text();

        status = dataset->putAndInsertString(DCM_AcquisitionDate, date.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding AcquisitionDate:" << status.text();

        status = dataset->putAndInsertString(DCM_AcquisitionTime, time.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding AcquisitionTime:" << status.text();

        status = dataset->putAndInsertString(DCM_ContentDate, date.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding ContentDate:" << status.text();

        status = dataset->putAndInsertString(DCM_ContentTime, time.toStdString().c_str());
        if (status.bad()) qDebug() << "Error adding ContentTime:" << status.text();

        // ===== IMAGE TYPE =====
        qDebug() << "Adding Image Type...";
        if (numberOfFrames > 1) {
            // ===== CHANGE: Image Type for XA Cine =====
            status = dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\CINE");
            if (status.bad()) qDebug() << "Error adding ImageType:" << status.text();
        } else {
            status = dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
            if (status.bad()) qDebug() << "Error adding ImageType:" << status.text();
        }

        // ===== IMAGE PIXEL INFORMATION =====
        qDebug() << "Adding Image Pixel Information...";
        status = dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        if (status.bad()) qDebug() << "Error adding SamplesPerPixel:" << status.text();

        status = dataset->putAndInsertUint16(DCM_Rows, static_cast<Uint16>(h));
        if (status.bad()) qDebug() << "Error adding Rows:" << status.text();

        status = dataset->putAndInsertUint16(DCM_Columns, static_cast<Uint16>(w));
        if (status.bad()) qDebug() << "Error adding Columns:" << status.text();

        status = dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
        if (status.bad()) qDebug() << "Error adding BitsAllocated:" << status.text();

        status = dataset->putAndInsertUint16(DCM_BitsStored, 16);
        if (status.bad()) qDebug() << "Error adding BitsStored:" << status.text();

        status = dataset->putAndInsertUint16(DCM_HighBit, 15);
        if (status.bad()) qDebug() << "Error adding HighBit:" << status.text();

        status = dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
        if (status.bad()) qDebug() << "Error adding PixelRepresentation:" << status.text();

        status = dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        if (status.bad()) qDebug() << "Error adding PhotometricInterpretation:" << status.text();

        status = dataset->putAndInsertString(DCM_PixelSpacing, "0.319\\0.319");
        if (status.bad()) qDebug() << "Error adding PixelSpacing:" << status.text();

        // ===== RESCALE / WINDOW SETTINGS =====
        qDebug() << "Adding Rescale/Window Settings...";
        status = dataset->putAndInsertFloat64(DCM_RescaleIntercept, 0.0);
        if (status.bad()) qDebug() << "Error adding RescaleIntercept:" << status.text();

        status = dataset->putAndInsertFloat64(DCM_RescaleSlope, 1.0);
        if (status.bad()) qDebug() << "Error adding RescaleSlope:" << status.text();

        status = dataset->putAndInsertString(DCM_RescaleType, "US");
        if (status.bad()) qDebug() << "Error adding RescaleType:" << status.text();

        status = dataset->putAndInsertFloat64(DCM_WindowCenter, 32768.0);
        if (status.bad()) qDebug() << "Error adding WindowCenter:" << status.text();

        status = dataset->putAndInsertFloat64(DCM_WindowWidth, 65535.0);
        if (status.bad()) qDebug() << "Error adding WindowWidth:" << status.text();

        // ===== SOP COMMON =====
        qDebug() << "Adding SOP Common...";
        // ===== CHANGE: SOP Class UID for XA =====
        const char* sopClassUID = UID_XRayAngiographicImageStorage;
        status = dataset->putAndInsertString(DCM_SOPClassUID, sopClassUID);
        if (status.bad()) qDebug() << "Error adding SOPClassUID:" << status.text();

        status = dataset->putAndInsertString(DCM_SOPInstanceUID, instanceUID);
        if (status.bad()) qDebug() << "Error adding SOPInstanceUID:" << status.text();

        // ===== CHARACTER SET =====
        qDebug() << "Adding Character Set...";
        status = dataset->putAndInsertString(DCM_SpecificCharacterSet, "ISO_IR_100");
        if (status.bad()) qDebug() << "Error adding SpecificCharacterSet:" << status.text();

        // ===== PATIENT ORIENTATION =====
        qDebug() << "Adding Patient Orientation...";
        status = dataset->putAndInsertString(DCM_PatientOrientation, "");
        if (status.bad()) qDebug() << "Error adding PatientOrientation:" << status.text();

        // ===== POSITIONER INFORMATION =====
        qDebug() << "Adding Positioner Information...";
        status = dataset->putAndInsertFloat64(DCM_GantryAngle, 0.0);
        if (status.bad()) qDebug() << "Error adding GantryAngle:" << status.text();

        // ===== INSERT PIXEL DATA =====
        qDebug() << "Inserting Pixel Data...";
        status = dataset->putAndInsertUint16Array(DCM_PixelData, pixelData, totalPixels);
        if (status.bad()) {
            qDebug() << "Error inserting PixelData:" << status.text();
        } else {
            qDebug() << "Pixel Data inserted successfully";
        }

        // ===== WRITE FILE =====
        qDebug() << "Writing DICOM file...";
        static int fileCounter = 0;
        QString path = "D:\\ImageWrite\\XAWrite";

        // Create directory if it doesn't exist
        QDir dir(path);
        if (!dir.exists()) {
            qDebug() << "Creating directory:" << path;
            dir.mkpath(".");
        }

        QString filename = path + "\\XA_Cine_" + QString::number(++fileCounter) + ".dcm";
        qDebug() << "Writing to:" << filename;

        // Save the file
        status = fileformat.saveFile(filename.toStdString().c_str(), EXS_LittleEndianExplicit);

        if (status.bad()) {
            qDebug() << "DICOM Image Write Error:" << status.text();
        } else {
            qDebug() << "DICOM file written successfully!";
        }

        // Clean up
        qDebug() << "Cleaning up memory...";
        delete[] pixelData;

        qDebug() << "=== XA MODALITY COMPLETE ===";
}
