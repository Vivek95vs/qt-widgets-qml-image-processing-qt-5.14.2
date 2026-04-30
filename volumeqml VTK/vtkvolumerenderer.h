#pragma once
#include <QObject>
#include <QVector>
#include <QVector3D>

// Forward declarations to avoid including VTK headers here
class vtkImageData;
class vtkVolume;
class vtkVolumeProperty;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

class VtkVolumeRenderer : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool volumeLoaded READ volumeLoaded NOTIFY volumeChanged)
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY renderParamsChanged)
    Q_PROPERTY(float brightness READ brightness WRITE setBrightness NOTIFY renderParamsChanged)
    Q_PROPERTY(float contrast READ contrast WRITE setContrast NOTIFY renderParamsChanged)

public:
    VtkVolumeRenderer(QObject* parent = nullptr);
    ~VtkVolumeRenderer();

    bool volumeLoaded() const { return m_volumeLoaded; }
    float opacity() const { return m_opacity; }
    float brightness() const { return m_brightness; }
    float contrast() const { return m_contrast; }

    Q_INVOKABLE void loadVolumeData(const QVector<short>& data,
                                   int width, int height, int depth,
                                   double spacingX = 1.0, double spacingY = 1.0, double spacingZ = 1.0);
    Q_INVOKABLE void setOpacity(float opacity);
    Q_INVOKABLE void setBrightness(float brightness);
    Q_INVOKABLE void setContrast(float contrast);
    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE void setRenderMode(int mode);

    // Get raw pointers for integration
    void* getRenderer();
    void* getRenderWindow();

signals:
    void volumeChanged();
    void renderParamsChanged();
    void renderingComplete();

private:
    void setupVolumeRendering();
    void updateTransferFunctions();

    class VtkVolumeRendererPrivate;
    VtkVolumeRendererPrivate* d;

    bool m_volumeLoaded;
    float m_opacity;
    float m_brightness;
    float m_contrast;
};
