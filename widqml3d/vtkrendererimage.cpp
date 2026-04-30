#include "vtkrendererimage.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QFileDialog>

// VTK initialization macros
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

VTKRendererImage::VTKRendererImage(QObject *parent)
    : QObject(parent)
    , m_tempImagePath(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/vtk_render.png")
{
    // Initialize VTK render window
    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderWindow->AddRenderer(m_renderer);

    // Set background color
    m_renderer->SetBackground(0.1, 0.2, 0.3);
}

VTKRendererImage::~VTKRendererImage()
{
    // Cleanup
    if (QFile::exists(m_tempImagePath)) {
        QFile::remove(m_tempImagePath);
    }
}

void VTKRendererImage::renderVolumeFromDICOM(const QString &dicomFolder)
{
    try {
        // Read DICOM files
        vtkSmartPointer<vtkDICOMReader> reader = vtkSmartPointer<vtkDICOMReader>::New();

        // Get all DICOM files in folder
        QDir dir(dicomFolder);
        QStringList dicomFiles = dir.entryList(QStringList() << "*.dcm" << "*.dicom", QDir::Files);

        if (dicomFiles.isEmpty()) {
            emit renderingError("No DICOM files found in folder");
            return;
        }

        // Create file list for VTK - FIXED API
        vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
        for (const QString &file : dicomFiles) {
            QString fullPath = QDir::cleanPath(dicomFolder + "/" + file);
            fileArray->InsertNextValue(fullPath.toStdString());
        }

        reader->SetFileNames(fileArray);
        reader->Update();

        // Create render pipeline
        createRenderPipeline(reader->GetOutput());

        // Save render to image
        saveRenderToImage();

        emit renderingComplete();

    } catch (std::exception &e) {
        emit renderingError(QString("Error rendering DICOM: %1").arg(e.what()));
    }
}

void VTKRendererImage::renderVolumeFromRawData(const QVector<float> &data,
                                         int width, int height, int depth,
                                         double spacingX, double spacingY, double spacingZ)
{
    try {
        // Create vtkImageData from raw data
        vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
        imageData->SetDimensions(width, height, depth);
        imageData->SetSpacing(spacingX, spacingY, spacingZ);
        imageData->SetOrigin(0, 0, 0);
        imageData->AllocateScalars(VTK_FLOAT, 1);

        // Copy data
        float* ptr = static_cast<float*>(imageData->GetScalarPointer());
        int totalSize = width * height * depth;
        for (int i = 0; i < totalSize; i++) {
            ptr[i] = data[i];
        }

        // Create render pipeline
        createRenderPipeline(imageData);

        // Save render to image
        saveRenderToImage();

        emit renderingComplete();

    } catch (std::exception &e) {
        emit renderingError(QString("Error rendering raw data: %1").arg(e.what()));
    }
}

void VTKRendererImage::createRenderPipeline(vtkImageData* imageData)
{
    // Apply Gaussian smoothing
    vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmooth =
        vtkSmartPointer<vtkImageGaussianSmooth>::New();
    gaussianSmooth->SetInputData(imageData);
    gaussianSmooth->SetRadiusFactors(1.0, 1.0, 1.0);
    gaussianSmooth->SetStandardDeviations(1.0, 1.0, 1.0);
    gaussianSmooth->Update();

    // Create volume mapper
    m_mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    m_mapper->SetInputConnection(gaussianSmooth->GetOutputPort());
    m_mapper->SetSampleDistance(0.5);
    m_mapper->SetAutoAdjustSampleDistances(0);

    // Create transfer functions
    vtkSmartPointer<vtkColorTransferFunction> colorFunc =
        vtkSmartPointer<vtkColorTransferFunction>::New();
    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc =
        vtkSmartPointer<vtkPiecewiseFunction>::New();

    // Define transfer functions (adjust based on your data range)
    double range[2];
    imageData->GetScalarRange(range);

    qDebug() << "Data range:" << range[0] << "to" << range[1];

    // Example transfer functions for medical data
    if (range[1] - range[0] < 1000) {
        // For CT data (Hounsfield units)
        colorFunc->AddRGBPoint(-1000, 0.0, 0.0, 0.0);      // Air
        colorFunc->AddRGBPoint(-500, 0.5, 0.5, 0.5);      // Lung
        colorFunc->AddRGBPoint(0, 1.0, 1.0, 1.0);         // Water
        colorFunc->AddRGBPoint(300, 1.0, 0.5, 0.0);       // Soft tissue
        colorFunc->AddRGBPoint(1000, 1.0, 1.0, 1.0);      // Bone

        opacityFunc->AddPoint(-1000, 0.0);
        opacityFunc->AddPoint(-500, 0.1);
        opacityFunc->AddPoint(0, 0.2);
        opacityFunc->AddPoint(300, 0.4);
        opacityFunc->AddPoint(1000, 0.8);
    } else {
        // For general data
        colorFunc->AddRGBPoint(range[0], 0.0, 0.0, 0.0);
        colorFunc->AddRGBPoint((range[0] + range[1]) / 3.0, 0.0, 0.0, 1.0);
        colorFunc->AddRGBPoint((range[0] + range[1]) * 2.0 / 3.0, 0.0, 1.0, 0.0);
        colorFunc->AddRGBPoint(range[1], 1.0, 0.0, 0.0);

        opacityFunc->AddPoint(range[0], 0.0);
        opacityFunc->AddPoint((range[0] + range[1]) / 2.0, 0.5);
        opacityFunc->AddPoint(range[1], 1.0);
    }

    // Create volume property
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
        vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(colorFunc);
    volumeProperty->SetScalarOpacity(opacityFunc);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    // Create volume
    m_volume = vtkSmartPointer<vtkVolume>::New();
    m_volume->SetMapper(m_mapper);
    m_volume->SetProperty(volumeProperty);

    // Clear previous actors and add new volume
    m_renderer->RemoveAllViewProps();
    m_renderer->AddVolume(m_volume);

    // Set up camera
    m_renderer->ResetCamera();
    m_renderer->GetActiveCamera()->Azimuth(30);
    m_renderer->GetActiveCamera()->Elevation(30);
    m_renderer->GetActiveCamera()->Dolly(1.5);

    // Render
    m_renderWindow->Render();
}

void VTKRendererImage::saveRenderToImage()
{
    // Create window to image filter
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
        vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(m_renderWindow);
    windowToImageFilter->SetScale(1); // Set scale for higher resolution if needed
    windowToImageFilter->ReadFrontBufferOff(); // Read from the back buffer
    windowToImageFilter->Update();

    // Save to PNG file
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(m_tempImagePath.toStdString().c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

    // Update the property
    m_renderImagePath = m_tempImagePath;
    emit renderImagePathChanged(m_renderImagePath);

    qDebug() << "Render saved to:" << m_renderImagePath;
}
