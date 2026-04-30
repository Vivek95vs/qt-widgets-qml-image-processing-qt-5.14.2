#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QDir"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vtkWidget = new MyQvtkWidget(this);
    ui->verticalLayout->addWidget(vtkWidget);


}

MainWindow::~MainWindow()
{
    delete ui;
}

vtkSmartPointer<vtkDICOMReader> MainWindow::dicomRead(QString dicomFolder)
{

    vtkSmartPointer <vtkDICOMDirectory> dicomdir =
            vtkSmartPointer <vtkDICOMDirectory>:: New ();
    dicomdir -> SetDirectoryName (dicomFolder.toStdString().c_str());


    try
    {
        dicomdir ->Update ();
    }
    catch(...)
    {
        qDebug()<<"Exception  reading dicom files";
        return 0;

    }

    //    dicomdir ->Update ();


    vtkSmartPointer<vtkDICOMReader> reader1 = vtkSmartPointer<vtkDICOMReader>::New();

    /*int */ numSeries=dicomdir->GetNumberOfSeries();

    vtkStringArray *sortedFiles;

    qDebug()<<"Number of Series found :"<<numSeries;

    if(numSeries>1 || numSeries == 0)
        return 0;//QMessage Box to be added
    //    while(numSeries>0){
    //if(numSeries==1){

    sortedFiles = dicomdir->GetFileNamesForSeries (numSeries-1);

    for(int i=0;i<sortedFiles->GetNumberOfValues();i++){

        if(reader1->CanReadFile(sortedFiles->GetValue(i))){

            reader1->SetFileName(sortedFiles->GetValue(i));
            reader1->SetGlobalWarningDisplay(false);
            reader1->SetMemoryRowOrderToFileNative();


            try
            {
                reader1->Update();
            }
            catch(...)
            {
                qDebug()<<"Exception  reading dicom files";
                return 0;
            }

            if(QString::fromStdString(reader1->GetMetaData()->GetAttributeValue(
                                          vtkDICOMTag(0x0008,0x0060)).AsString()).compare("RTIMAGE")==0)
            {
                return 0;
            }

            /* Search for RT Structure file and get filename*/
            if(QString::fromStdString(reader1->GetMetaData()->GetAttributeValue(
                                          vtkDICOMTag(0x0008,0x0060)).AsString()).compare("RTSTRUCT")==0){
                rtStructFile = sortedFiles->GetValue(i);

            }

        }
    }


    reader1=NULL;
    reader =NULL;

    reader =vtkSmartPointer<vtkDICOMReader>::New();
    reader->SetFileNames(sortedFiles);
    //    reader->SetDataScalarTypeToShort(); //Commented on 01-01-2022
    reader->SetMemoryRowOrderToFileNative();

    //    re

    try
    {
        reader->Update();

    }
    catch(...)
    {
        qDebug()<<"Exception  reading dicom files";
        return 0;

    }

    //    reader->GetOutput()->GetB


    return reader;


}

int* MainWindow::getDim()
{
    /*Returns the dimensions of the image.*/
    int *d= reader->GetOutput()->GetDimensions();
    dim= new int[3];
    for(int i=0;i<3;i++){
        dim[i]=d[i];
    }
    return dim;
}

void MainWindow::setVtkVolumeToC(float *cVol)
{
    /*
     * Assumes data exists.
     * Exports the vtk image to the c volume and converts the data to float.
     */

    vtkSmartPointer<vtkImageExport> exporter =vtkSmartPointer<vtkImageExport>::New();
    exporter->SetInputData(reader->GetOutput());

    exporter->ImageLowerLeftOn();//default
    try{
        exporter->Update();
        qDebug()<<"cbctreconexporter->Update()";
        v=(short*)exporter->GetPointerToData();//since data is huge get only pointer to memory
    }catch(...){
        return;
    }
    for(int i=0;i< dim[0]*dim[1]*dim[2];i++){
        cVol[i]=static_cast<float>(v[i]);
    }
}

void MainWindow::display3D(float* volume, const int *dim, double *pixPitch)
{
    volumeImport = vtkImageImport::New();
    // volume Import
    volumeImport->CopyImportVoidPointer(volume, sizeof(float)*dim[0]*dim[1]*dim[2]);
    volumeImport->SetWholeExtent(0,dim[0]-1, 0,dim[1]-1, 0,dim[2]-1);

    qDebug()<<"check dimension"<<dim[0]<<dim[1]<<dim[2];
    qDebug()<<"pixel dimension"<<pixPitch[0]<<pixPitch[1]<<pixPitch[2];
    //volumeImport->SetDataScalarTypeToUnsignedChar();
    volumeImport->SetDataScalarTypeToFloat();
    volumeImport->SetNumberOfScalarComponents(1);

    volumeImport->SetDataExtentToWholeExtent();
    //volumeImport->SetWholeExtent(0,param->nOutputPixels-1,0,param->nOutputPixels-1,0,param->nOutputPixels-1);
    volumeImport->SetDataSpacing(pixPitch[0], pixPitch[1], pixPitch[2]);

//    volumeImport->SetDataSpacing(double(pixPitch), double(pixPitch), double(pixPitch));

    /******************************************************** MAPPER ***********************************************************/
    // 3D GAUSSIAN FILTER

    vtkImageGaussianSmooth *gaussFilter = vtkImageGaussianSmooth::New();
    gaussFilter->SetInputConnection(volumeImport->GetOutputPort());
    gaussFilter->SetRadiusFactors(1.5, 1.5, 1.5);
    gaussFilter->SetStandardDeviations(1.0, 1.0, 1.0);
    gaussFilter->Update();
    // Map data on GPU
    vtkGPUVolumeRayCastMapper *volumeGPUmapper = vtkGPUVolumeRayCastMapper::New();
       //vtkVolumeRayCastMapper *volumeGPUmapper=vtkVolumeRayCastMapper::New();
    volumeGPUmapper->SetInputConnection(gaussFilter->GetOutputPort());


    /******************************************************** ACTOR ***********************************************************/

    vtkVolume *VTKvolume = vtkVolume::New();
    VTKvolume->SetMapper(volumeGPUmapper);
    // RGB and alpha funcions

    double skinOnBlueMap[28][5] =
    {
        {-923, 0.987853825092316, 1.0,               1.0,               0.0},
        {-733, 0.987853825092316, 1.0,               1.0,               0.00750770070067319},
        {-531, 0.987853825092316, 1.0,               1.0,               0.0733569149832789},
        {-372, 0.987853825092316, 1.0,               1.0,               0.0},
        {142,  0.0,               0.0,               0.0,               0.0},
        {332,  1.0,               0.0,               0.0,               0.132020258336102},
        {534,  1.0,               0.999206542968750, 0.0,               0.317781232734010},
        {679,  1.0,               1.0,               1.0,               0.940655276334784}
    };

    // Apply colormap
    alphaChannelFunc = vtkPiecewiseFunction::New();
    colorFunc = vtkColorTransferFunction::New();
    for(int i = 0; i < sizeof(skinOnBlueMap)/(5*sizeof(double)); i++)
    {
        colorFunc->AddRGBPoint(skinOnBlueMap[i][0], skinOnBlueMap[i][1], skinOnBlueMap[i][2], skinOnBlueMap[i][3]);
        alphaChannelFunc->AddPoint(skinOnBlueMap[i][0], skinOnBlueMap[i][4]);
    }
    // PropertyEditor
    vtkVolumeProperty   *volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(colorFunc);
    volumeProperty->SetScalarOpacity(alphaChannelFunc);
    VTKvolume->SetProperty(volumeProperty);
    if(VTKvolume != NULL)
    {
        qDebug()<<"Every thing fine @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    }

    vtkSmartPointer<vtkCamera> camera =
       vtkSmartPointer<vtkCamera>::New();

    camera->Roll(180);
    camera->Yaw(180);

    ren = vtkRenderer::New();
    ren->AddVolume(VTKvolume);
    ren->SetBackground(0, 0, 0);

    vtkWidget->GetRenderWindow()->AddRenderer(ren);
    ren->SetActiveCamera(camera);
    ren->ResetCamera();

    vtkWidget->GetRenderWindow()->Render();
    // Cleaning.
    gaussFilter->Delete();
    volumeGPUmapper->Delete();
    VTKvolume->Delete();
    volumeProperty->Delete();
    volumeImport->Delete();
    colorFunc->Delete();
    alphaChannelFunc->Delete();
    ren->Delete();

}

void MainWindow::on_startrender_clicked()
{

    QString fPath = "D:\\DICOM_SERIES";
    if(fPath == 0||fPath == "")
    {
        //                   QMessageBox::warning(this,"Error","Path not found..");
        QMessageBox * msgBox = new QMessageBox(this);
        msgBox->setWindowIcon(QIcon(":/theme/Themes/Logo PNG.PNG"));
        msgBox->setWindowTitle("ERROR");
        msgBox->setText("Path not found");
        msgBox->exec();
        delete msgBox;
    }

    QDir dir(fPath);
    QStringList filters;
    QStringList dcmList;

    if(!dir.exists())
    {
        //                   QMessageBox::warning(this,"Error","Directory does not exists..");
        QMessageBox * msgBox = new QMessageBox(this);
        msgBox->setWindowIcon(QIcon(":/theme/Themes/Logo PNG.PNG"));
        msgBox->setWindowTitle("ERROR");
        msgBox->setText("Directory does not exists");
        msgBox->exec();
        delete msgBox;
    }

    filters << "*.dcm" ;
    dir.setNameFilters(filters);
    dcmList = dir.entryList(filters);

    qDebug()<<"No of CT dicom files : "<<dcmList.length();

    if(dcmList.length()==0){
        //                    QMessageBox::warning(this,"Error","No Images Found..");
        QMessageBox * msgBox = new QMessageBox(this);
        msgBox->setWindowIcon(QIcon(":/theme/Themes/Logo PNG.PNG"));
        msgBox->setWindowTitle("ERROR");
        msgBox->setText("No Images Found");
        msgBox->exec();
        delete msgBox;
        return;
    }

    CBCTReader= dicomRead(fPath);

    const int *dim1 = getDim();

    qDebug()<<"Dimension CT"<<dim1[0]<<dim1[1]<<dim1[2];

    if(dim[0]!=dim[1]||dim[1]!=dim[2]){
        //                    QMessageBox::critical(this,"Error","Some Files are missing.");
        QMessageBox * msgBox = new QMessageBox(this);
        msgBox->setWindowIcon(QIcon(":/theme/Themes/Logo PNG.PNG"));
        msgBox->setWindowTitle("ERROR");
        msgBox->setText("Some Files are missing");
        msgBox->exec();
        delete msgBox;
        return;
    }

    cbctvolume = new float[ dim1[0]*dim1[1]*dim1[2]];

    setVtkVolumeToC(cbctvolume);

    pixSpacingCBCT<<256.0/(float)512;
    pixSpacingCBCT<<256.0/(float)512;
    pixSpacingCBCT<<256.0/(float)512;

    double *pixPitch = new double[3];
    pixPitch[0]=pixSpacingCBCT[0];
    pixPitch[1]=pixSpacingCBCT[1];
    pixPitch[2]=pixSpacingCBCT[2];

    display3D(cbctvolume,dim,pixPitch);

}
