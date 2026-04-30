#include "VolumeTexture.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGTextureMaterial>
#include <QOpenGLFunctions_3_3_Core>

VolumeTexture::VolumeTexture(QQuickItem* parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

VolumeTexture::~VolumeTexture() {
    if (m_texture3D) {
        delete m_texture3D;
        m_texture3D = nullptr;
    }
    if (m_shaderProgram) {
        delete m_shaderProgram;
        m_shaderProgram = nullptr;
    }
}

void VolumeTexture::setVolumeData(const QByteArray& data) {
    if (m_volumeData == data) return;

    m_volumeData = data;
    m_textureDirty = true;
    update();
    emit volumeDataChanged();
}

void VolumeTexture::setDimensions(const QVector3D& dim) {
    if (m_dimensions == dim) return;

    m_dimensions = dim;
    m_textureDirty = true;
    update();
    emit dimensionsChanged();
}

void VolumeTexture::setStepSize(float size) {
    if (qFuzzyCompare(m_stepSize, size)) return;

    m_stepSize = size;
    update();
    emit stepSizeChanged();
}

void VolumeTexture::setBrightness(float brightness) {
    if (qFuzzyCompare(m_brightness, brightness)) return;

    m_brightness = brightness;
    update();
    emit brightnessChanged();
}

void VolumeTexture::setContrast(float contrast) {
    if (qFuzzyCompare(m_contrast, contrast)) return;

    m_contrast = contrast;
    update();
    emit contrastChanged();
}

void VolumeTexture::setOpacity(float opacity) {
    if (qFuzzyCompare(m_opacity, opacity)) return;

    m_opacity = opacity;
    update();
    emit opacityChanged();
}

QSGNode* VolumeTexture::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    if (!m_initialized) {
        initializeGL();
        m_initialized = true;
    }

    if (m_textureDirty && !m_volumeData.isEmpty()) {
        updateTexture();
    }

    if (m_volumeData.isEmpty() || m_dimensions.isNull()) {
        return nullptr;
    }

    // Create a simple texture node
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(oldNode);
    if (!node) {
        node = new QSGGeometryNode();

        QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        // Create material
        QSGTextureMaterial* material = new QSGTextureMaterial();
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    // Update geometry to fill the item
    QRectF rect = boundingRect();
    QSGGeometry::TexturedPoint2D* vertices = node->geometry()->vertexDataAsTexturedPoint2D();

    vertices[0].set(rect.left(), rect.top(), 0.0f, 0.0f);
    vertices[1].set(rect.right(), rect.top(), 1.0f, 0.0f);
    vertices[2].set(rect.left(), rect.bottom(), 0.0f, 1.0f);
    vertices[3].set(rect.right(), rect.bottom(), 1.0f, 1.0f);

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

void VolumeTexture::initializeGL() {
    // Initialize OpenGL context
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context) {
        qWarning("No OpenGL context available");
        return;
    }

    QOpenGLFunctions* gl = context->functions();

    // Create shader program
    m_shaderProgram = new QOpenGLShaderProgram();

    // Simple vertex shader
    const char* vsrc =
        "#version 330 core\n"
        "layout(location = 0) in vec2 vertex;\n"
        "layout(location = 1) in vec2 texCoord;\n"
        "out vec2 vTexCoord;\n"
        "void main() {\n"
        "    vTexCoord = texCoord;\n"
        "    gl_Position = vec4(vertex * 2.0 - 1.0, 0.0, 1.0);\n"
        "}\n";

    // Simple fragment shader for volume rendering
    const char* fsrc =
        "#version 330 core\n"
        "in vec2 vTexCoord;\n"
        "out vec4 fragColor;\n"
        "uniform sampler3D volumeTexture;\n"
        "uniform vec3 dimensions;\n"
        "uniform float stepSize;\n"
        "uniform float brightness;\n"
        "uniform float contrast;\n"
        "uniform float opacity;\n"
        "void main() {\n"
        "    vec3 texCoord = vec3(vTexCoord, 0.5);\n"
        "    float intensity = texture(volumeTexture, texCoord).r;\n"
        "    intensity = (intensity - 0.5) * contrast + 0.5;\n"
        "    intensity *= brightness;\n"
        "    intensity = clamp(intensity, 0.0, 1.0);\n"
        "    fragColor = vec4(intensity, intensity, intensity, opacity);\n"
        "}\n";

    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc)) {
        qWarning("Failed to compile vertex shader");
        return;
    }

    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc)) {
        qWarning("Failed to compile fragment shader");
        return;
    }

    if (!m_shaderProgram->link()) {
        qWarning("Failed to link shader program");
        return;
    }
}

void VolumeTexture::updateTexture() {
    if (m_volumeData.isEmpty() || m_dimensions.isNull()) return;

    // Create or update 3D texture
    if (!m_texture3D) {
        m_texture3D = new QOpenGLTexture(QOpenGLTexture::Target3D);
        m_texture3D->setFormat(QOpenGLTexture::R8_UNorm);
        m_texture3D->setMinificationFilter(QOpenGLTexture::Linear);
        m_texture3D->setMagnificationFilter(QOpenGLTexture::Linear);
        m_texture3D->setWrapMode(QOpenGLTexture::ClampToEdge);
    }

    int width = m_dimensions.x();
    int height = m_dimensions.y();
    int depth = m_dimensions.z();

    // Check if texture needs to be reallocated
    if (m_texture3D->width() != width ||
        m_texture3D->height() != height ||
        m_texture3D->depth() != depth) {

        m_texture3D->destroy();
        m_texture3D->setSize(width, height, depth);
        m_texture3D->allocateStorage();
    }

    // Upload texture data
    m_texture3D->setData(0, 0, 0,
                         width, height, depth,
                         QOpenGLTexture::Red,
                         QOpenGLTexture::UInt8,
                         m_volumeData.constData());

    m_textureDirty = false;
}
