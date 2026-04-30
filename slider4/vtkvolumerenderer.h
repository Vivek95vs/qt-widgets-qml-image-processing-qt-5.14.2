#ifndef VTKVOLUMERENDERER_H
#define VTKVOLUMERENDERER_H

#include <QObject>
#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkInteractorStyleTrackballCamera.h>

class VTKVolumeRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit VTKVolumeRenderer(QWidget* parent = nullptr);
    ~VTKVolumeRenderer();

    void setVolumeData(vtkSmartPointer<vtkImageData> volumeData);
    void renderVolume();
    void resetView();

    // Window/Level controls
    void setWindowWidth(double width);
    void setWindowLevel(double level);

    // Transfer function controls
    void setOpacity(double opacity);
    void setColor(double r, double g, double b);

    vtkRenderWindow* getRenderWindow() { return m_renderWindow; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    vtkSmartPointer<vtkImageData> m_volumeData;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> m_volumeMapper;
    vtkSmartPointer<vtkVolume> m_volume;
    vtkSmartPointer<vtkVolumeProperty> m_volumeProperty;
    vtkSmartPointer<vtkColorTransferFunction> m_colorFunction;
    vtkSmartPointer<vtkPiecewiseFunction> m_opacityFunction;

    void setupRenderer();
    void setupVolumeMapper();
    void setupTransferFunctions();
    void updateTransferFunctions();

    double m_windowWidth;
    double m_windowLevel;
    double m_opacity;
    QColor m_color;
};

#endif // VTKVOLUMERENDERER_H
