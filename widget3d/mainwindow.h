#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume16Reader.h>
#include <vtkVolume.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkCamera.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkMetaImageWriter.h>
#include "myqvtkwidget.h"

// MACROS NEEDED FOR SOME VTK INITIALIZATION ISSUE
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

// First include the required header files for the VTK classes we are using.
#include <QVTKWidget.h>
#include <vtkImageImport.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkDICOMReader.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
//#include <itkImageToVTKImageFilter.h>
#include <vtkImageCast.h>
#include <vtkImageImport.h>
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkImageReslice.h>

#include <vtkCamera.h>

#include <vtkSmartPointer.h>
#include <vtkDICOMReader.h>
#include <vtkDICOMTag.h>
#include <vtkDICOMWriter.h>
#include <vtkDICOMMRGenerator.h>
#include <vtkDICOMMetaData.h>
#include <vtkImageImport.h>
#include <vtkDICOMCTGenerator.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include<vtkDirectory.h>
#include<vtkMatrix4x4.h>
#include<vtkDICOMSorter.h>
#include<vtkDICOMDirectory.h>
#include<vtkImageExport.h>
#include<vtkIntArray.h>
#include<vtkMedicalImageProperties.h>

#include <vtkDICOMImageReader.h>
#include <vtkImageReslice.h>
#include <QGraphicsColorizeEffect>
#include <vtkDICOMGenerator.h>
#include <vtkDICOMUIDGenerator.h>
#include <vtkStringArray.h>

#include <vtkTransform.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    vtkSmartPointer<vtkDICOMReader> CBCTReader;
    vtkSmartPointer<vtkDICOMReader> reader=NULL;
    vtkImageImport* volumeImport=NULL;
    int numSeries;
    QString rtStructFile;
    int* dim;
    float* cbctvolume=NULL;
    short *v;
    QList<double> pixSpacingCBCT;
    void display3D(float* volume,const int *dim,double *pixPitch);

    MyQvtkWidget *vtkWidget;
    vtkColorTransferFunction *colorFunc;
    vtkPiecewiseFunction *alphaChannelFunc;
    vtkRenderer *ren;

    ~MainWindow();
public slots:
    vtkSmartPointer<vtkDICOMReader> dicomRead(QString dicomFolder);
    int* getDim() ;
    void setVtkVolumeToC(float *vol);

private slots:
    void on_startrender_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
