#pragma once

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVolumeProperty.h>

class VtkVolumeRendererPrivate {
public:
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;
    vtkSmartPointer<vtkPiecewiseFunction> opacityFunction;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
};
