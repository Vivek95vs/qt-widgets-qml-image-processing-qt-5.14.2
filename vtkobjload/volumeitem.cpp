#include "volumeitem.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>


#include <vtkXMLImageDataReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>

class VolumeRenderer : public QQuickFramebufferObject::Renderer
{
public:
    VolumeRenderer(const QString &path)
    {
        renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderWindow->AddRenderer(renderer);

        if (!path.isEmpty())
            loadVTI(path);
    }

    void loadVTI(const QString &path)
    {
        vtkNew<vtkXMLImageDataReader> reader;
        reader->SetFileName(path.toStdString().c_str());
        reader->Update();

        vtkNew<vtkGPUVolumeRayCastMapper> mapper;
        mapper->SetInputData(reader->GetOutput());

        vtkNew<vtkColorTransferFunction> color;
        color->AddRGBPoint(-1000, 0.0, 0.0, 0.0);
        color->AddRGBPoint(0,     1.0, 0.5, 0.3);
        color->AddRGBPoint(1000,  1.0, 1.0, 1.0);

        vtkNew<vtkPiecewiseFunction> opacity;
        opacity->AddPoint(-1000, 0.0);
        opacity->AddPoint(0,     0.05);
        opacity->AddPoint(1000,  0.8);

        vtkNew<vtkVolumeProperty> property;
        property->SetColor(color);
        property->SetScalarOpacity(opacity);
        property->ShadeOn();
        property->SetInterpolationTypeToLinear();

        volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(mapper);
        volume->SetProperty(property);

        renderer->RemoveAllViewProps();
        renderer->AddVolume(volume);
        renderer->ResetCamera();
    }

    void render() override
    {
        renderWindow->Render();
//        update();   // continuous rendering
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::Depth);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkVolume> volume;
};

QQuickFramebufferObject::Renderer *
VolumeItem::createRenderer() const
{
    return new VolumeRenderer(vtiPath);
}

void VolumeItem::loadVTI(const QString &path)
{
    vtiPath = path;
    update();
}
