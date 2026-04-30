#include "DicomVolumeRendereDicom.h"
#include "DicomLoader.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QTimer>
#include "QDebug"

// Helper function to check OpenGL errors
void checkGLError(const char* context) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qWarning("OpenGL error in %s: %d", context, error);
    }
}

DicomVolumeRenderer::DicomVolumeRenderer()
    : initialized(false)
    , windowWidth(2000.0)
    , windowLevel(500.0)
    , volumeOpacity(1.0)
{
    // Initialize VTK objects
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    volume = vtkSmartPointer<vtkVolume>::New();

    // Use FixedPointVolumeRayCastMapper for VTK 8.2 (more stable)
    volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();

    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();

    renderWindow->AddRenderer(renderer);
    renderer->SetBackground(0.1, 0.2, 0.4);
}

DicomVolumeRenderer::~DicomVolumeRenderer()
{
    if (renderWindow) {
        renderWindow->Finalize();
    }
}

void DicomVolumeRenderer::initializeGL()
{
    if (!initialized) {
        initializeOpenGLFunctions();

        // Important: Set the OpenGL context for VTK
        QOpenGLContext* context = QOpenGLContext::currentContext();
        if (context) {
            renderWindow->SetSharedRenderWindow(renderWindow);
//            renderWindow->SetDisplayId(context->nativeHandle().value<WId>());
            renderWindow->Initialize();
        }

        initialized = true;
    }
}

QOpenGLFramebufferObject* DicomVolumeRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);  // MSAA for better quality

    return new QOpenGLFramebufferObject(size, format);
}

void DicomVolumeRenderer::synchronize(QQuickFramebufferObject* item)
{
    if (!item) return;

    DicomVolumeItem* volumeItem = qobject_cast<DicomVolumeItem*>(item);
    if (!volumeItem) return;

    // Update properties from QML
    setWindowLevel(volumeItem->window(), volumeItem->level());
    setOpacity(volumeItem->opacity());

    // Handle reset camera request
    static bool resetRequested = false;
    if (resetRequested) {
        resetCamera();
        resetRequested = false;
    }
}

void DicomVolumeRenderer::render()
{
    initializeGL();
    checkGLError("Before render");

    if (!renderWindow || !renderer) {
        return;
    }

    // Make sure we have a valid OpenGL context
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context) {
        qWarning("No OpenGL context available");
        return;
    }

    // Update render window size
    renderWindow->SetSize(/*width()*/1024, /*height()*/1024);

    // Render the scene
    renderWindow->Render();

    checkGLError("After render");

    // Update the texture
    update();
}

void DicomVolumeRenderer::setImageData(vtkSmartPointer<vtkImageData> data)
{
    if (!data || data == imageData) {
        return;
    }

    imageData = data;
    setupVolumeRendering();

    // Reset camera to fit the new volume
    resetCamera();
}

void DicomVolumeRenderer::setupVolumeRendering()
{
    if (!imageData) return;

    // Configure volume mapper
    volumeMapper->SetInputData(imageData);
    volumeMapper->SetSampleDistance(1.0);
    volumeMapper->SetMaximumImageSampleDistance(10.0);
    volumeMapper->SetAutoAdjustSampleDistances(1);

    // Configure volume property
    updateTransferFunctions();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.1);
    volumeProperty->SetDiffuse(0.9);
    volumeProperty->SetSpecular(0.2);
    volumeProperty->SetSpecularPower(10.0);

    // Set up the volume
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    // Add volume to renderer (remove existing first)
    renderer->RemoveAllViewProps();
    renderer->AddVolume(volume);
}

void DicomVolumeRenderer::updateTransferFunctions()
{
    if (!imageData) return;

    double range[2];
    imageData->GetScalarRange(range);

    double minVal = range[0];
    double maxVal = range[1];

    // Clear existing transfer functions
    colorTransferFunction->RemoveAllPoints();
    opacityTransferFunction->RemoveAllPoints();

    // Calculate window/level bounds
    double lower = windowLevel - windowWidth / 2.0;
    double upper = windowLevel + windowWidth / 2.0;

    // Clamp to data range
    lower = qMax(lower, minVal);
    upper = qMin(upper, maxVal);

    // Normalize to [0,1] for transfer functions
    double normalizedLower = (lower - minVal) / (maxVal - minVal);
    double normalizedUpper = (upper - minVal) / (maxVal - minVal);

    // Color transfer function (grayscale)
    colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(normalizedLower, 0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(normalizedUpper, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(1.0, 1.0, 1.0, 1.0);

    // Opacity transfer function
    opacityTransferFunction->AddPoint(0.0, 0.0);
    opacityTransferFunction->AddPoint(normalizedLower, 0.0);
    opacityTransferFunction->AddPoint(normalizedUpper, volumeOpacity);
    opacityTransferFunction->AddPoint(1.0, volumeOpacity);

    // Update the volume property
    if (volumeProperty) {
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);
    }
}

void DicomVolumeRenderer::setWindowLevel(double window, double level)
{
    if (window != windowWidth || level != windowLevel) {
        windowWidth = window;
        windowLevel = level;
        updateTransferFunctions();
    }
}

void DicomVolumeRenderer::setOpacity(double opacity)
{
    if (opacity != volumeOpacity) {
        volumeOpacity = opacity;
        updateTransferFunctions();
    }
}

void DicomVolumeRenderer::setRenderMode(int mode)
{
    // Simplified render modes for VTK 8.2
    if (!volumeMapper) return;

    switch (mode) {
    case 0: // Composite
        volumeMapper->SetBlendModeToComposite();
        break;
    case 1: // Maximum Intensity Projection
        volumeMapper->SetBlendModeToMaximumIntensity();
        break;
    case 2: // Minimum Intensity Projection
        volumeMapper->SetBlendModeToMinimumIntensity();
        break;
    default:
        volumeMapper->SetBlendModeToComposite();
        break;
    }
}

void DicomVolumeRenderer::resetCamera()
{
    if (renderer) {
        renderer->ResetCamera();
        renderer->GetActiveCamera()->SetParallelProjection(0);

        // Adjust clipping planes based on volume bounds
//        double bounds[6];
//        if (volume && volume->GetMapper() && volume->GetMapper()->GetInput()) {
//            volume->GetMapper()->GetInput()->GetBounds(bounds);
//            double diagonal = sqrt(
//                pow(bounds[1] - bounds[0], 2) +
//                pow(bounds[3] - bounds[2], 2) +
//                pow(bounds[5] - bounds[4], 2)
//            );

//            renderer->GetActiveCamera()->SetClippingRange(
//                diagonal / 100.0,
//                diagonal * 10.0
//            );
//        }

        if (renderWindow) {
            renderWindow->Render();
        }
    }
}

// DicomVolumeItem implementation
DicomVolumeItem::DicomVolumeItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , m_window(2000.0)
    , m_level(500.0)
    , m_opacity(1.0)
    , m_renderMode(0)
    , m_renderer(nullptr)
{
    setMirrorVertically(true);
    setTextureFollowsItemSize(true);
}

DicomVolumeItem::~DicomVolumeItem()
{
}

QQuickFramebufferObject::Renderer* DicomVolumeItem::createRenderer() const
{
    m_renderer = new DicomVolumeRenderer();

    // Connect signals if we already have image data
    if (m_imageData) {
        m_renderer->setImageData(m_imageData);
    }

    return m_renderer;
}

QString DicomVolumeItem::dicomDirectory() const
{
    return m_dicomDirectory;
}

void DicomVolumeItem::setDicomDirectory(const QString& directory)
{
    if (m_dicomDirectory != directory) {
        m_dicomDirectory = directory;
        emit dicomDirectoryChanged();
        loadDicomSeries();
    }
}

double DicomVolumeItem::window() const
{
    return m_window;
}

void DicomVolumeItem::setWindow(double window)
{
    if (qFuzzyCompare(m_window, window)) return;

    m_window = window;
    emit windowChanged();

    if (m_renderer) {
        m_renderer->setWindowLevel(m_window, m_level);
        update();
    }
}

double DicomVolumeItem::level() const
{
    return m_level;
}

void DicomVolumeItem::setLevel(double level)
{
    if (qFuzzyCompare(m_level, level)) return;

    m_level = level;
    emit levelChanged();

    if (m_renderer) {
        m_renderer->setWindowLevel(m_window, m_level);
        update();
    }
}

double DicomVolumeItem::opacity() const
{
    return m_opacity;
}

void DicomVolumeItem::setOpacity(double opacity)
{
    if (qFuzzyCompare(m_opacity, opacity)) return;

    m_opacity = opacity;
    emit opacityChanged();

    if (m_renderer) {
        m_renderer->setOpacity(m_opacity);
        update();
    }
}

int DicomVolumeItem::renderMode() const
{
    return m_renderMode;
}

void DicomVolumeItem::setRenderMode(int mode)
{
    if (m_renderMode == mode) return;

    m_renderMode = mode;
    emit renderModeChanged();

    if (m_renderer) {
        m_renderer->setRenderMode(m_renderMode);
        update();
    }
}

void DicomVolumeItem::resetCamera()
{
    if (m_renderer) {
        m_renderer->resetCamera();
        update();
    }
}

void DicomVolumeItem::loadDicomSeries()
{
    if (m_dicomDirectory.isEmpty()) {
        qWarning("No DICOM directory specified");
        return;
    }

    DicomLoader loader;
    if (loader.loadDirectory(m_dicomDirectory.toStdString())) {
        m_imageData = loader.getImageData();

        if (m_imageData) {
            qDebug() << "DICOM loaded successfully. Dimensions:"
                     << m_imageData->GetDimensions()[0] << "x"
                     << m_imageData->GetDimensions()[1] << "x"
                     << m_imageData->GetDimensions()[2];

            // Update renderer if it exists
            if (m_renderer) {
                m_renderer->setImageData(m_imageData);
                update();
            }

            emit imageDataChanged(m_imageData);
        }
    } else {
//        qWarning("Failed to load DICOM series from:" << m_dicomDirectory);
        qDebug()<<"Check the folder"<<m_dicomDirectory;
    }
}
