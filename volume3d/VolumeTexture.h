#pragma once
#include <QQuickItem>
#include <QSGSimpleTextureNode>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class VolumeTexture : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QByteArray volumeData READ volumeData WRITE setVolumeData NOTIFY volumeDataChanged)
    Q_PROPERTY(QVector3D dimensions READ dimensions WRITE setDimensions NOTIFY dimensionsChanged)
    Q_PROPERTY(float stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(float brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(float contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

public:
    VolumeTexture(QQuickItem* parent = nullptr);
    ~VolumeTexture();

    QByteArray volumeData() const { return m_volumeData; }
    void setVolumeData(const QByteArray& data);

    QVector3D dimensions() const { return m_dimensions; }
    void setDimensions(const QVector3D& dim);

    float stepSize() const { return m_stepSize; }
    void setStepSize(float size);

    float brightness() const { return m_brightness; }
    void setBrightness(float brightness);

    float contrast() const { return m_contrast; }
    void setContrast(float contrast);

    float opacity() const { return m_opacity; }
    void setOpacity(float opacity);

signals:
    void volumeDataChanged();
    void dimensionsChanged();
    void stepSizeChanged();
    void brightnessChanged();
    void contrastChanged();
    void opacityChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

private:
    void initializeGL();
    void updateTexture();

    QByteArray m_volumeData;
    QVector3D m_dimensions;
    float m_stepSize = 0.001f;
    float m_brightness = 1.0f;
    float m_contrast = 1.0f;
    float m_opacity = 0.8f;

    QOpenGLTexture* m_texture3D = nullptr;
    QOpenGLShaderProgram* m_shaderProgram = nullptr;
    QOpenGLBuffer m_vertexBuffer;
    bool m_initialized = false;
    bool m_textureDirty = true;
};
