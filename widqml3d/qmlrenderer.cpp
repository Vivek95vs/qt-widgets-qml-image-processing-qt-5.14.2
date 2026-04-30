#include "qmlrenderer.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

QMLRenderer::QMLRenderer(QObject *parent)
    : QObject(parent)
    , m_vtkRenderer(new VTKRendererImage(this))
    , m_rendering(false)
    , m_statusText("Ready")
{
    // Connect signals from VTKRenderer
    connect(m_vtkRenderer, &VTKRendererImage::renderingComplete,
            this, &QMLRenderer::handleRenderingComplete);
    connect(m_vtkRenderer, &VTKRendererImage::renderingError,
            this, &QMLRenderer::handleRenderingError);
    connect(m_vtkRenderer, &VTKRendererImage::renderImagePathChanged,
            this, &QMLRenderer::handleRenderImagePathChanged);
}

QMLRenderer::~QMLRenderer()
{
    // VTKRenderer will be deleted automatically as it's a child
}

void QMLRenderer::loadDICOMFolder(const QUrl &folderUrl)
{
    if (m_rendering) {
        setStatusText("Already rendering...");
        return;
    }

    QString localPath = folderUrl.toLocalFile();
    QFileInfo fileInfo(localPath);

    if (!fileInfo.exists() || !fileInfo.isDir()) {
        setStatusText("Invalid folder path");
        return;
    }

    setRendering(true);
    setStatusText("Rendering DICOM volume...");

    // Start rendering in background (you might want to use QThreadPool for this)
    m_vtkRenderer->renderVolumeFromDICOM(localPath);
}

void QMLRenderer::loadSampleData()
{
    if (m_rendering) {
        setStatusText("Already rendering...");
        return;
    }

    setRendering(true);
    setStatusText("Generating sample volume...");

    // Create sample volume data (sphere)
    int width = 64, height = 64, depth = 64;
    QVector<float> data(width * height * depth, 0.0f);

    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float centerZ = depth / 2.0f;
    float radius = width / 4.0f;

    int index = 0;
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float dx = x - centerX;
                float dy = y - centerY;
                float dz = z - centerZ;
                float distance = sqrt(dx*dx + dy*dy + dz*dz);

                if (distance <= radius) {
                    // Create gradient from center to edge
                    float value = 1.0f - (distance / radius);
                    data[index] = value * 1000.0f; // Scale for better visualization
                }
                index++;
            }
        }
    }

    // Render the sample data
    m_vtkRenderer->renderVolumeFromRawData(data, width, height, depth, 1.0, 1.0, 1.0);
}

void QMLRenderer::handleRenderingComplete()
{
    setRendering(false);
    setStatusText("Rendering complete");
    qDebug() << "Rendering completed successfully";
}

void QMLRenderer::handleRenderingError(const QString &error)
{
    setRendering(false);
    setStatusText("Error: " + error);
    qDebug() << "Rendering error:" << error;
}

void QMLRenderer::handleRenderImagePathChanged(const QString &path)
{
    if (QFile::exists(path)) {
        setImageSource(QUrl::fromLocalFile(path));
        qDebug() << "Image source updated:" << path;
    }
}

void QMLRenderer::setRendering(bool rendering)
{
    if (m_rendering != rendering) {
        m_rendering = rendering;
        emit renderingChanged(m_rendering);
    }
}

void QMLRenderer::setStatusText(const QString &status)
{
    if (m_statusText != status) {
        m_statusText = status;
        emit statusTextChanged(m_statusText);
    }
}

void QMLRenderer::setImageSource(const QUrl &source)
{
    if (m_imageSource != source) {
        m_imageSource = source;
        emit imageSourceChanged(m_imageSource);
    }
}
