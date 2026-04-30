#pragma once

#include <QObject>
#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkFixedPointVolumeRayCastMapper.h>  // For VTK 8.2
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>

class DicomVolumeRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    DicomVolumeRenderer();
    ~DicomVolumeRenderer();

    // QQuickFramebufferObject::Renderer interface
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    void synchronize(QQuickFramebufferObject* item) override;

public slots:
    void setImageData(vtkSmartPointer<vtkImageData> data);
    void setWindowLevel(double window, double level);
    void setOpacity(double opacity);
    void setRenderMode(int mode);
    void resetCamera();

private:
    void initializeGL();
    void setupVolumeRendering();
    void updateTransferFunctions();

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper;  // For VTK 8.2
    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction;
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
    vtkSmartPointer<vtkImageData> imageData;

    bool initialized;
    double windowWidth;
    double windowLevel;
    double volumeOpacity;
};

class DicomVolumeItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString dicomDirectory READ dicomDirectory WRITE setDicomDirectory NOTIFY dicomDirectoryChanged)
    Q_PROPERTY(double window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(double level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int renderMode READ renderMode WRITE setRenderMode NOTIFY renderModeChanged)

public:
    DicomVolumeItem(QQuickItem* parent = nullptr);
    ~DicomVolumeItem();

    // QQuickFramebufferObject interface
    QQuickFramebufferObject::Renderer* createRenderer() const override;

    // Property getters
    QString dicomDirectory() const;
    double window() const;
    double level() const;
    double opacity() const;
    int renderMode() const;

    // Property setters
    void setDicomDirectory(const QString& directory);
    void setWindow(double window);
    void setLevel(double level);
    void setOpacity(double opacity);
    void setRenderMode(int mode);

    // QML invokable functions
    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE void loadDicomSeries();

signals:
    void dicomDirectoryChanged();
    void windowChanged();
    void levelChanged();
    void opacityChanged();
    void renderModeChanged();
    void imageDataChanged(vtkSmartPointer<vtkImageData> data);

private:
    QString m_dicomDirectory;
    double m_window;
    double m_level;
    double m_opacity;
    int m_renderMode;

    mutable DicomVolumeRenderer* m_renderer;
    vtkSmartPointer<vtkImageData> m_imageData;
};
