#include "vtk_render_to_image.h"

#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkFixedPointVolumeRayCastMapper.h>   // CPU SAFE
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkDICOMImageReader.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

VtkRenderToImage::VtkRenderToImage(QObject *parent)
    : QObject(parent)
{
}

QString VtkRenderToImage::imagePath() const
{
    return m_imagePath;
}

void VtkRenderToImage::renderAndSave()
{
    // -------- READ DICOM --------
    auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName("D:/DICOM_SERIES");
    reader->Update();

    // -------- SAFE CPU MAPPER (NO GPU) --------
    auto mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    // -------- TRANSFER FUNCTIONS --------
    auto color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(-1000, 0.0, 0.0, 0.0);
    color->AddRGBPoint(500, 1.0, 1.0, 1.0);

    auto opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(-1000, 0.0);
    opacity->AddPoint(500, 1.0);

    auto property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetColor(color);
    property->SetScalarOpacity(opacity);

    // -------- VOLUME --------
    auto volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(mapper);
    volume->SetProperty(property);

    // -------- RENDERER --------
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddVolume(volume);
    renderer->SetBackground(0, 0, 0);

    // -------- GENERIC OPENGL WINDOW --------
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->SetSize(800, 800);
    renderWindow->AddRenderer(renderer);
    renderWindow->Render();

    // -------- SAVE IMAGE --------
    auto w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2i->SetInput(renderWindow);
    w2i->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    m_imagePath = "D:/vtk_render.png";
    writer->SetFileName(m_imagePath.toStdString().c_str());
    writer->SetInputConnection(w2i->GetOutputPort());
    writer->Write();

    emit imagePathChanged();
}
