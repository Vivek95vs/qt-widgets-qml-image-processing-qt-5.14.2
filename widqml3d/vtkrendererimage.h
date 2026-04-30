#ifndef VTKRENDERERIMAGE_H
#define VTKRENDERERIMAGE_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QQuickItem>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageImport.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkDICOMReader.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>

class VTKRendererImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString renderImagePath READ renderImagePath NOTIFY renderImagePathChanged)

public:
    explicit VTKRendererImage(QObject *parent = nullptr);
    ~VTKRendererImage();

    QString renderImagePath() const { return m_renderImagePath; }

    Q_INVOKABLE void renderVolumeFromDICOM(const QString &dicomFolder);
    Q_INVOKABLE void renderVolumeFromRawData(const QVector<float> &data,
                                            int width, int height, int depth,
                                            double spacingX, double spacingY, double spacingZ);

signals:
    void renderImagePathChanged(const QString &path);
    void renderingComplete();
    void renderingError(const QString &error);

private:
    void createRenderPipeline(vtkImageData* imageData);
    void saveRenderToImage();

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkVolume> m_volume;
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> m_mapper;

    QString m_renderImagePath;
    QString m_tempImagePath;
};
#endif // VTKRENDERER_H
