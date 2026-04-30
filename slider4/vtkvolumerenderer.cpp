#include "vtkvolumerenderer.h"
#include <QResizeEvent>
#include <QDebug>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>

VTKVolumeRenderer::VTKVolumeRenderer(QWidget* parent)
    : QWidget(parent),
      m_windowWidth(400.0),
      m_windowLevel(40.0),
      m_opacity(0.8),
      m_color(Qt::white)
{
    // Set up VTK render window
    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    // Set up renderer
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderWindow->AddRenderer(m_renderer);

    // Set up interactor
    m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactor->SetRenderWindow(m_renderWindow);

    // Set up interactor style
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    m_interactor->SetInteractorStyle(style);

    // Set background color
    m_renderer->SetBackground(0.1, 0.1, 0.2);

    // Initialize volume components
    m_volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    m_volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    m_colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    m_opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();

    setupVolumeMapper();
    setupTransferFunctions();

    // Create volume
    m_volume = vtkSmartPointer<vtkVolume>::New();
    m_volume->SetMapper(m_volumeMapper);
    m_volume->SetProperty(m_volumeProperty);

    // Add volume to renderer
    m_renderer->AddVolume(m_volume);

    // Set up lighting
    m_renderer->SetUseDepthPeeling(1);
    m_renderer->SetMaximumNumberOfPeels(100);
    m_renderer->SetOcclusionRatio(0.1);

    // Start interactor
    m_interactor->Initialize();
}

VTKVolumeRenderer::~VTKVolumeRenderer()
{
}

void VTKVolumeRenderer::setVolumeData(vtkSmartPointer<vtkImageData> volumeData)
{
    if (!volumeData) {
        qWarning() << "VTKVolumeRenderer: Null volume data";
        return;
    }

    m_volumeData = volumeData;
    qDebug() << "VTK Volume data dimensions:"
             << m_volumeData->GetDimensions()[0] << "x"
             << m_volumeData->GetDimensions()[1] << "x"
             << m_volumeData->GetDimensions()[2];

    // Set volume data to mapper
    m_volumeMapper->SetInputData(m_volumeData);

    // Calculate initial window/level from data range
    double range[2];
    m_volumeData->GetScalarRange(range);
    m_windowLevel = (range[0] + range[1]) / 2.0;
    m_windowWidth = (range[1] - range[0]) * 0.5;

    // Update transfer functions
    updateTransferFunctions();

    // Reset camera to fit volume
    m_renderer->ResetCamera();

    // Render
    m_renderWindow->Render();
}

void VTKVolumeRenderer::renderVolume()
{
    if (!m_volumeData) {
        qWarning() << "VTKVolumeRenderer: No volume data to render";
        return;
    }

    updateTransferFunctions();
    m_renderWindow->Render();
}

void VTKVolumeRenderer::resetView()
{
    if (m_renderer) {
        m_renderer->ResetCamera();
        m_renderWindow->Render();
    }
}

void VTKVolumeRenderer::setWindowWidth(double width)
{
    m_windowWidth = width;
    updateTransferFunctions();
    m_renderWindow->Render();
}

void VTKVolumeRenderer::setWindowLevel(double level)
{
    m_windowLevel = level;
    updateTransferFunctions();
    m_renderWindow->Render();
}

void VTKVolumeRenderer::setOpacity(double opacity)
{
    m_opacity = qBound(0.0, opacity, 1.0);
    updateTransferFunctions();
    m_renderWindow->Render();
}

void VTKVolumeRenderer::setColor(double r, double g, double b)
{
    m_color = QColor::fromRgbF(r, g, b);
    updateTransferFunctions();
    m_renderWindow->Render();
}

void VTKVolumeRenderer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (m_renderWindow) {
        m_renderWindow->SetSize(event->size().width(), event->size().height());
        m_renderWindow->Render();
    }
}

void VTKVolumeRenderer::setupVolumeMapper()
{
    // Configure volume mapper
    m_volumeMapper->SetBlendModeToComposite();
    m_volumeMapper->SetSampleDistance(0.5);
    m_volumeMapper->SetAutoAdjustSampleDistances(0);
    m_volumeMapper->SetMaximumImageSampleDistance(10.0);
}

void VTKVolumeRenderer::setupTransferFunctions()
{
    // Configure volume property
    m_volumeProperty->SetColor(m_colorFunction);
    m_volumeProperty->SetScalarOpacity(m_opacityFunction);
    m_volumeProperty->SetInterpolationTypeToLinear();
    m_volumeProperty->ShadeOn();
    m_volumeProperty->SetAmbient(0.4);
    m_volumeProperty->SetDiffuse(0.6);
    m_volumeProperty->SetSpecular(0.2);
    m_volumeProperty->SetSpecularPower(10.0);
}

void VTKVolumeRenderer::updateTransferFunctions()
{
    if (!m_volumeData) return;

    double range[2];
    m_volumeData->GetScalarRange(range);

    // Calculate window/level boundaries
    double minVal = m_windowLevel - m_windowWidth / 2.0;
    double maxVal = m_windowLevel + m_windowWidth / 2.0;

    // Clamp to data range
    minVal = qMax(minVal, range[0]);
    maxVal = qMin(maxVal, range[1]);

    // Clear and rebuild transfer functions
    m_colorFunction->RemoveAllPoints();
    m_opacityFunction->RemoveAllPoints();

    // Color transfer function (grayscale by default)
    m_colorFunction->AddRGBPoint(minVal, 0.0, 0.0, 0.0);
    m_colorFunction->AddRGBPoint((minVal + maxVal) / 2.0,
                                 m_color.redF() * 0.7,
                                 m_color.greenF() * 0.7,
                                 m_color.blueF() * 0.7);
    m_colorFunction->AddRGBPoint(maxVal, m_color.redF(), m_color.greenF(), m_color.blueF());

    // Opacity transfer function
    m_opacityFunction->AddPoint(minVal, 0.0);
    m_opacityFunction->AddPoint(minVal + (maxVal - minVal) * 0.1, 0.0);
    m_opacityFunction->AddPoint(minVal + (maxVal - minVal) * 0.3, m_opacity * 0.3);
    m_opacityFunction->AddPoint(minVal + (maxVal - minVal) * 0.5, m_opacity * 0.6);
    m_opacityFunction->AddPoint(minVal + (maxVal - minVal) * 0.7, m_opacity * 0.8);
    m_opacityFunction->AddPoint(maxVal, m_opacity);

    // Update volume property
    m_volumeProperty->SetColor(m_colorFunction);
    m_volumeProperty->SetScalarOpacity(m_opacityFunction);
}
