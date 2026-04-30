#include "DicomController.h"
#include <vtkDICOMDirectory.h>
#include <vtkStringArray.h>
#include <vtkNew.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <QDebug>
#include <QQuickVTKRenderItem.h>
DicomController::DicomController(QObject *parent) : QObject(parent) {
    m_reader = vtkSmartPointer<vtkDICOMReader>::New();
}

void DicomController::setRenderer(QObject* renderer) {
    if (renderer) {
        // Cast the QML object to the actual VTK Render Item class
        QQuickVTKRenderItem* vtkItem = qobject_cast<QQuickVTKRenderItem*>(renderer);
        if (vtkItem) {
            m_renderer = vtkItem->renderer();
            qDebug() << "Renderer successfully linked!";
        } else {
            qDebug() << "Failed to cast QObject to QQuickVTKRenderItem";
        }
    }
}

void DicomController::startRender(const QString& folderPath) {
    if (!m_renderer) return;

    vtkNew<vtkDICOMDirectory> dicomdir;
    dicomdir->SetDirectoryName(folderPath.toStdString().c_str());
    dicomdir->Update();

    m_seriesCount = dicomdir->GetNumberOfSeries();
    emit seriesCountChanged();
    qDebug() << "Number of Series found :" << m_seriesCount;

    if (m_seriesCount > 0) {
        vtkStringArray* sortedFiles = dicomdir->GetFileNamesForSeries(m_seriesCount - 1);
        m_fileCount = sortedFiles->GetNumberOfValues();
        emit fileCountChanged();
        qDebug() << "No of CT dicom files :" << m_fileCount;

        m_reader->SetFileNames(sortedFiles);
        m_reader->Update();
        double range[2];
        m_reader->GetOutput()->GetScalarRange(range);
        qDebug() << "DICOM Scalar Range: " << range[0] << " to " << range[1];
        runVolumePipeline(m_reader->GetOutput());
    }
}

void DicomController::runVolumePipeline(vtkImageData* data) {
    if (!m_renderer) {
        qDebug() << "Error: Renderer not linked. Cannot run pipeline.";
        return;
    }

    if (!data) {
        qDebug() << "Error: Image data is null.";
        return;
    }

    // 1. Check Data Range (Crucial for visibility)
    double range[2];
    data->GetScalarRange(range);
    qDebug() << "DICOM Scalar Range: " << range[0] << " to " << range[1];

    // 2. Pre-processing: Smoothing
    vtkNew<vtkImageGaussianSmooth> gaussianSmooth;
    gaussianSmooth->SetInputData(data);
    gaussianSmooth->SetStandardDeviations(1.0, 1.0, 1.0);
    gaussianSmooth->SetRadiusFactors(1.5, 1.5, 1.5);
    gaussianSmooth->Update();

    // 3. Volume Mapper
    // Note: If you see a black screen, try vtkFixedPointVolumeRayCastMapper
    vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
    volumeMapper->SetInputConnection(gaussianSmooth->GetOutputPort());
    volumeMapper->SetAutoAdjustSampleDistances(1);

    // 4. Color Transfer Function (Skin and Tissue)
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    // Map: Scalar Value, R, G, B
    colorTransferFunction->AddRGBPoint(-923, 0.987, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(-733, 0.987, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(-531, 0.987, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(-372, 0.987, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(142,  0.0,   0.0, 0.0);
    colorTransferFunction->AddRGBPoint(332,  1.0,   0.0, 0.0);
    colorTransferFunction->AddRGBPoint(534,  1.0,   0.99, 0.0);
    colorTransferFunction->AddRGBPoint(679,  1.0,   1.0, 1.0);

    // 5. Opacity Transfer Function (Transparency)
    vtkNew<vtkPiecewiseFunction> scalarOpacity;
    // Map: Scalar Value, Opacity (0.0 to 1.0)
    scalarOpacity->AddPoint(-923, 0.0);
    scalarOpacity->AddPoint(-733, 0.007);
    scalarOpacity->AddPoint(-531, 0.073);
    scalarOpacity->AddPoint(-372, 0.0);
    scalarOpacity->AddPoint(142,  0.0);
    scalarOpacity->AddPoint(332,  0.132);
    scalarOpacity->AddPoint(534,  0.317);
    scalarOpacity->AddPoint(679,  0.94);

    // 6. Volume Property
    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(scalarOpacity);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn(); // Enables lighting for 3D depth
    volumeProperty->SetAmbient(0.2);
    volumeProperty->SetDiffuse(0.8);
    volumeProperty->SetSpecular(0.1);

    // 7. Volume Actor
    vtkNew<vtkVolume> volume;
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    // 8. Add to Renderer
    m_renderer->RemoveAllViewProps(); // Clear previous actors
    m_renderer->AddVolume(volume);

    // Set background to a dark grey so we know the renderer is working
    m_renderer->SetBackground(0.1, 0.1, 0.1);

    // 9. Camera Setup
    m_renderer->ResetCamera();
    vtkCamera* camera = m_renderer->GetActiveCamera();
    camera->Azimuth(30);
    camera->Elevation(30);

    // 10. Update Render Window (Crucial for QML)
    vtkRenderWindow* renderWindow = m_renderer->GetRenderWindow();
    if (renderWindow) {
        renderWindow->Modified(); // Flag changes
        renderWindow->Render();   // Request draw
    }

    qDebug() << "Volume Pipeline Execution: Everything fine @@@@@@@@@@@@@@@@@@@@@@";
}
