#include "vtkvolumerenderer.h"
#include "VtkVolumeRendererPrivate.h"
#include <QDebug>

VtkVolumeRenderer::VtkVolumeRenderer(QObject* parent)
    : QObject(parent), d(new VtkVolumeRendererPrivate()),
      m_volumeLoaded(false), m_opacity(0.5f),
      m_brightness(1.0f), m_contrast(1.0f) {

    // Setup VTK rendering pipeline
    d->renderer = vtkSmartPointer<vtkRenderer>::New();
    d->renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    d->interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    d->renderer->SetBackground(0.1, 0.1, 0.1);
    d->renderWindow->AddRenderer(d->renderer);
    d->interactor->SetRenderWindow(d->renderWindow);

    // Setup interactor style
    auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    d->interactor->SetInteractorStyle(style);

    // Setup volume mapper
    d->volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    d->opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    d->volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();

    d->volume = vtkSmartPointer<vtkVolume>::New();
    d->volume->SetMapper(d->volumeMapper);
    d->volume->SetProperty(d->volumeProperty);

    d->renderer->AddVolume(d->volume);
}

VtkVolumeRenderer::~VtkVolumeRenderer() {
    delete d;
}

void VtkVolumeRenderer::loadVolumeData(const QVector<short>& data,
                                      int width, int height, int depth,
                                      double spacingX, double spacingY, double spacingZ) {
    if (data.isEmpty() || width <= 0 || height <= 0 || depth <= 0) {
        qWarning() << "Invalid volume data";
        return;
    }

    d->imageData = vtkSmartPointer<vtkImageData>::New();
    d->imageData->SetDimensions(width, height, depth);
    d->imageData->SetSpacing(spacingX, spacingY, spacingZ);
    d->imageData->SetOrigin(0, 0, 0);
    d->imageData->AllocateScalars(VTK_SHORT, 1);

    // Copy data
    short* ptr = static_cast<short*>(d->imageData->GetScalarPointer());
    std::copy(data.begin(), data.end(), ptr);

    // Set up volume rendering
    d->volumeMapper->SetInputData(d->imageData);
    setupVolumeRendering();

    m_volumeLoaded = true;
    emit volumeChanged();

    // Start rendering
    d->renderWindow->Render();
    emit renderingComplete();
}

void VtkVolumeRenderer::setupVolumeRendering() {
    // Configure volume mapper
    d->volumeMapper->SetBlendModeToComposite();

    // Setup transfer functions
    updateTransferFunctions();

    d->volume->Update();
}

void VtkVolumeRenderer::updateTransferFunctions() {
    if (!d->imageData) return;

    // Get scalar range
    double range[2];
    d->imageData->GetScalarRange(range);

    // Clear existing functions
    d->colorFunction->RemoveAllPoints();
    d->opacityFunction->RemoveAllPoints();

    // Set color transfer function (grayscale)
    d->colorFunction->AddRGBPoint(range[0], 0.0, 0.0, 0.0);
    d->colorFunction->AddRGBPoint((range[0] + range[1]) * 0.25, 0.3, 0.3, 0.3);
    d->colorFunction->AddRGBPoint((range[0] + range[1]) * 0.5, 0.6, 0.6, 0.6);
    d->colorFunction->AddRGBPoint((range[0] + range[1]) * 0.75, 0.8, 0.8, 0.8);
    d->colorFunction->AddRGBPoint(range[1], 1.0, 1.0, 1.0);

    // Set opacity transfer function
    d->opacityFunction->AddPoint(range[0], 0.0);
    d->opacityFunction->AddPoint(range[0] + (range[1] - range[0]) * 0.1, 0.0);
    d->opacityFunction->AddPoint(range[0] + (range[1] - range[0]) * 0.3, m_opacity * 0.3);
    d->opacityFunction->AddPoint(range[0] + (range[1] - range[0]) * 0.5, m_opacity * 0.6);
    d->opacityFunction->AddPoint(range[0] + (range[1] - range[0]) * 0.7, m_opacity * 0.8);
    d->opacityFunction->AddPoint(range[1], m_opacity);

    // Apply to volume property
    d->volumeProperty->SetColor(d->colorFunction);
    d->volumeProperty->SetScalarOpacity(d->opacityFunction);
    d->volumeProperty->SetInterpolationTypeToLinear();
    d->volumeProperty->ShadeOn();
    d->volumeProperty->SetAmbient(0.3 * m_brightness);
    d->volumeProperty->SetDiffuse(0.7 * m_brightness);
    d->volumeProperty->SetSpecular(0.2 * m_brightness);
    d->volumeProperty->SetSpecularPower(10.0 * m_contrast);

    d->renderWindow->Render();
}

void VtkVolumeRenderer::setOpacity(float opacity) {
    m_opacity = qBound(0.0f, opacity, 1.0f);
    updateTransferFunctions();
    emit renderParamsChanged();
}

void VtkVolumeRenderer::setBrightness(float brightness) {
    m_brightness = qBound(0.1f, brightness, 2.0f);
    updateTransferFunctions();
    emit renderParamsChanged();
}

void VtkVolumeRenderer::setContrast(float contrast) {
    m_contrast = qBound(0.1f, contrast, 3.0f);
    updateTransferFunctions();
    emit renderParamsChanged();
}

void VtkVolumeRenderer::resetCamera() {
    d->renderer->ResetCamera();
    d->renderWindow->Render();
}

void VtkVolumeRenderer::setRenderMode(int mode) {
    if (!d->volumeMapper) return;

    switch(mode) {
        case 0: // Composite
            d->volumeMapper->SetBlendModeToComposite();
            break;
        case 1: // Maximum
            d->volumeMapper->SetBlendModeToMaximumIntensity();
            break;
        case 2: // Minimum
            d->volumeMapper->SetBlendModeToMinimumIntensity();
            break;
    }
    d->renderWindow->Render();
}

void* VtkVolumeRenderer::getRenderer() {
    return static_cast<void*>(d->renderer.GetPointer());
}

void* VtkVolumeRenderer::getRenderWindow() {
    return static_cast<void*>(d->renderWindow.GetPointer());
}
