#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <gdcmUIDGenerator.h>
#include <itkCommand.h>
#include "itkImageFileWriter.h"
#include <itkGDCMImageIO.h>
#include "itkMetaDataObject.h"
#include <itkMetaDataDictionary.h>
#include "itkImportImageFilter.h"
#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Tagwrite_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open Test Image","D:/Workflow/Test images/*.raw",QDir::currentPath());
    if (filename.isEmpty())
       {
           qDebug()<<"Could not open file";
       }
    FILE * Img;
    Img= fopen(filename.toStdString().c_str(),"rb");

    unsigned short* image=new unsigned short[w*h];
    fread(image,1,w*h*sizeof(unsigned short),Img);
    unsigned short* buffer=new unsigned short[w*h];
    buffer=image;
    qDebug()<<"Buff"<<buffer[1]<<filename;

    //Resample Code

//       using PixelType = unsigned short; // Use signed short (int16_t)
//       unsigned int Dimension = 2;
//       using ImageType12 = itk::Image<PixelType, 2>;

//       // Create a DICOM image reader
//       using ReaderType = itk::ImageFileReader<ImageType12>;
//       ReaderType::Pointer reader = ReaderType::New();
//       reader->SetFileName(filename.toStdString().c_str()); // Input DICOM file

//       try
//       {
//           reader->Update();
//       }
//       catch (itk::ExceptionObject & ex)
//       {
//           std::cerr << "Error reading the DICOM image: " << ex << std::endl;
//          // return EXIT_FAILURE;
//       }
//      // std::string outputFilePath = "D:\\PRITAM\\reconstruction time reduction\\output_image_pixelspacing.tiff";
//       // Access the DICOM image
//       double spacing[ 2 ];
//       spacing[0] = 0.3945;    // along X direction
//       spacing[1] = 0.3945;

//       ImageType12::Pointer dicomImage = reader->GetOutput();

//       typedef itk::NearestNeighborInterpolateImageFunction<ImageType12, double > InterpolatorType;
//       InterpolatorType::Pointer interpolator = InterpolatorType::New();

//             double newSpacingX = 0.3945; // New pixel spacing in the X dimension (in millimeters)
//             double newSpacingY = 0.3945; // New pixel spacing in the Y dimension (in millimeters)

//             // Calculate the scaling factors for resizing the image
//             double scalingFactorX = dicomImage->GetSpacing()[0] / newSpacingX;
//             double scalingFactorY = dicomImage->GetSpacing()[1] / newSpacingY;

//             // Create a resampling filter
//             typedef itk::ResampleImageFilter<ImageType12, ImageType12> ResampleFilterType;
//             ResampleFilterType::Pointer resampler = ResampleFilterType::New();

//             // Set the input (original) image
//             resampler->SetInput(dicomImage);

//             // Calculate the new size based on the scaling factors
//             ImageType12::SizeType newSize;
//             newSize[0] = static_cast<unsigned int>(dicomImage->GetLargestPossibleRegion().GetSize()[0] * scalingFactorX*1.74356);
//             newSize[1] = static_cast<unsigned int>(dicomImage->GetLargestPossibleRegion().GetSize()[1] * scalingFactorY*1.74356);

//             // Set the size and spacing of the output image
//             resampler->SetSize(newSize);
//             resampler->SetOutputSpacing(spacing);

//             // Interpolation options (e.g., Linear, NearestNeighbor, etc.)
//             resampler->SetInterpolator(interpolator);

//             // Update the resampler
//             resampler->Update();

     //Running

    //Create New DICOM Image
    gdcm::ImageWriter writer;
    gdcm::Image &image1=writer.GetImage();
    image1.SetNumberOfDimensions(2);

    //Create Sequence
    gdcm::File &file =writer.GetFile();
    gdcm::DataSet &ds=file.GetDataSet();

    //Create UID Sequence
    gdcm::UIDGenerator suid;

    gdcm::Attribute<0x0010,0x0010,VR::PN,VM::VM1>PatientName={"kanta vashumal Rohra"};
    ds.Insert(PatientName.GetAsDataElement());

    gdcm::Attribute<0x0010,0x0020,VR::LO,VM::VM1>PatientID={"4/9493-M"};
    ds.Insert(PatientID.GetAsDataElement());

//    QString date_string_on_db=patDetails.at(3);
//    QStringList datelist2=date_string_on_db.split(" ");
//    QStringList datelist=datelist2[0].split("/");

//    qDebug()<<"udgfi::CheckDate"<<datelist;
//    QString appendDate= datelist[2]+datelist[1]+datelist[0];

//    qDebug()<<"Chech date"<<appendDate;
//    QString DOB=appendDate.toStdString().c_str();
//    DOB.resize(10);

    gdcm::Attribute<0x0008,0x0064,VR::CS,VM::VM1>ConversionType={""};
    ds.Insert(ConversionType.GetAsDataElement());

    gdcm::Attribute<0x0010,0x0030,VR::DA,VM::VM1>PatBirthDate={"194810233"};
    ds.Insert(PatBirthDate.GetAsDataElement());

    gdcm::Attribute<0x0010,0x0040,VR::CS,VM::VM1>PatientSex={"F"};
    ds.Insert(PatientSex.GetAsDataElement());

    gdcm::Attribute<0x0010,0x1010,VR::AS,VM::VM1>PatientAge={"76"};
    ds.Insert(PatientAge.GetAsDataElement());

    gdcm::Attribute<0x0010,0x1020,VR::DS,VM::VM1>PatientSize={};
    ds.Insert(PatientSize.GetAsDataElement());

    gdcm::Attribute<0x0020,0x000D,VR::UI,VM::VM1>StudyUID={"1C"};
    ds.Insert(StudyUID.GetAsDataElement());

    QString date=QString(QDate::currentDate().toString("yyyyMMdd"));
    QString time=QString(QTime::currentTime().toString("hhmmss.zzzzzzz"));

    gdcm::Attribute<0x0008,0x0020,VR::DA,VM::VM1>StudyDate={date.toStdString().c_str()};
    ds.Insert(StudyDate.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0030,VR::TM,VM::VM1>StudyTime={time.toStdString().c_str()};
    ds.Insert(StudyTime.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0090,VR::PN,VM::VM1>PhysicianName={"DR. VENKAT"};
    ds.Insert(PhysicianName.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0022,VR::DA,VM::VM1>AcquisitionDate={date.toStdString().c_str()};
    ds.Insert(AcquisitionDate.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0032,VR::TM,VM::VM1>AcquisitionTime={time.toStdString().c_str()};
    ds.Insert(AcquisitionTime.GetAsDataElement());

    gdcm::Attribute<0x0020,0x0010,VR::SH,VM::VM1>StudyID={"2708"};
    ds.Insert(StudyID.GetAsDataElement());

    gdcm::Attribute<0x0008,0x1030,VR::LO,VM::VM1>StudyDescription={"IMG"};
    ds.Insert(StudyDescription.GetAsDataElement());

    gdcm::Attribute<0x0008,0x103e,VR::LO,VM::VM1>SeriesDescription={"3"};
    ds.Insert(SeriesDescription.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0050,VR::SH,VM::VM1>AccessionNum={"1"};
    ds.Insert(AccessionNum.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0060,VR::CS,VM::VM1>Modality={"RTIMAGE"};
    ds.Insert(Modality.GetAsDataElement());

    gdcm::Attribute<0x0018,0x1000,VR::LO,VM::VM1>DeviceSerialNumber={"985518756"};
    ds.Insert(DeviceSerialNumber.GetAsDataElement());

    gdcm::Attribute<0x0020,0x000E,VR::UI,VM::VM1>SeriesUID={suid.Generate()};
    ds.Insert(SeriesUID.GetAsDataElement());

    gdcm::Attribute<0x0020,0x0011,VR::IS,VM::VM1>SeriesNum={1};
    ds.Insert(SeriesNum.GetAsDataElement());

    gdcm::Attribute<0x0008,0x1070,VR::PN,VM::VM1>OperatorName={"User"};
    ds.Insert(OperatorName.GetAsDataElement());

    gdcm::Attribute<0x0020,0x0052,VR::UI,VM::VM1>FrameOfReferenceUID={suid.Generate()};
    ds.Insert(FrameOfReferenceUID.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0070,VR::LO,VM::VM1>Manufacturer={"PANACEA MEDICALS"};
    ds.Insert(Manufacturer.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0080,VR::LO,VM::VM1>InstituitionName={"ALPHA HOSPITAL"};
    ds.Insert(InstituitionName.GetAsDataElement());

    gdcm::Attribute<0x0008,0x1090,VR::LO,VM::VM1>ManufacturerModelName={"Siddharth-II"};
    ds.Insert(ManufacturerModelName.GetAsDataElement());

    gdcm::Attribute<0x0018,0x1020,VR::LO,VM::VM1>SoftwareVersions={"1.0.1"};
    ds.Insert(SoftwareVersions.GetAsDataElement());

    gdcm::Attribute<0x0020,0x0013,VR::IS,VM::VM1>InstanceNumber={1};
    ds.Insert(InstanceNumber.GetAsDataElement());

    gdcm::Attribute<0x0020,0x0020,VR::CS,VM::VM2>PatOrientation={""};
    ds.Insert(PatOrientation.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0023,VR::DA,VM::VM1>ContentDate={date.toStdString().c_str()};
    ds.Insert(ContentDate.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0033,VR::TM,VM::VM1>ContentTime={time.toStdString().c_str()};
    ds.Insert(ContentTime.GetAsDataElement());

    gdcm::Attribute<0x0008,0x0008,VR::CS,VM::VM4>ImageType={"ORIGINAL","PRIMARY"};
    ds.Insert(ImageType.GetAsDataElement());

    gdcm::Attribute<0x0028,0x0002,VR::US,VM::VM1>SamplesPerPixels={1};
    ds.Insert(SamplesPerPixels.GetAsDataElement());

    gdcm::Attribute<0x0028,0x0010,VR::US,VM::VM1>Rows={w};
    ds.Insert(Rows.GetAsDataElement());

    gdcm::Attribute<0x0028,0x0011,VR::US,VM::VM1>Coloums={h};
    ds.Insert(Coloums.GetAsDataElement());

    gdcm::Attribute<0x0028,0x0030,VR::DS,VM::VM2>ImgerPixelSpacing={0.139,0.139};
    ds.Insert(ImgerPixelSpacing.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0002,VR::SH,VM::VM1>RTImageLabel={"first"};
    ds.Insert(RTImageLabel.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0004,VR::ST,VM::VM1>RTImageDescription={"IMG"};
    ds.Insert(RTImageDescription.GetAsDataElement());

    gdcm::Attribute<0x3002,0x000A,VR::CS,VM::VM1>ReportedValuesOrigin={"OPERATOR"};
    ds.Insert(ReportedValuesOrigin.GetAsDataElement());

    gdcm::Attribute<0x3002,0x000C,VR::CS,VM::VM1>RTImagePlane={"NORMAL"};
    ds.Insert(RTImagePlane.GetAsDataElement());

    gdcm::Attribute<0x3002,0x000D,VR::DS,VM::VM3>XRayImageReceptorTranslation={0.355};
    ds.Insert(XRayImageReceptorTranslation.GetAsDataElement());

    gdcm::Attribute<0x3002,0x000E,VR::DS,VM::VM1>XRayImageReceptorAngle={0};
    ds.Insert(XRayImageReceptorAngle.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0010,VR::DS,VM::VM6>RTImageOrientation={};
    ds.Insert(RTImageOrientation.GetAsDataElement());

//    qDebug()<<"size"<<-w/2;

//    float RTimagePosX,RTimagePosY;

//    RTimagePosX=-w/2;  //RTimagePosX = -isoCentreX;
//    RTimagePosY=h/2;   //RTimagePosY = (radSize*pixspacing) - isoCentreY;

//    gdcm::Attribute<0x3002,0x0011,VR::DS,VM::VM2>ImagePlanePixelSpacing={0.075482897,0.075482897};
//    ds.Insert(ImagePlanePixelSpacing.GetAsDataElement());


    gdcm::Attribute<0x3002,0x0012,VR::DS,VM::VM2>RTImagePosition={-115.94172668457,115.94172668457};
    ds.Insert(RTImagePosition.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0020,VR::SH,VM::VM1>RadiationMachineName={"LINAC"};
    ds.Insert(RadiationMachineName.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0022,VR::DS,VM::VM1>RadiationMachineSAD={921};
    ds.Insert(RadiationMachineSAD.GetAsDataElement());

    gdcm::Attribute<0x3002,0x0026,VR::DS,VM::VM1>RTImageSID={1696};
    ds.Insert(RTImageSID.GetAsDataElement());

    gdcm::MediaStorage msUID=gdcm::MediaStorage::RTImageStorage;
    std::string u=msUID.GetString();
    gdcm::Attribute<0x0008,0x0016,VR::UI,VM::VM1> SOPClassUID={u};
    ds.Insert(SOPClassUID.GetAsDataElement());

    gdcm::UIDGenerator gen;
    gdcm::Attribute<0x0008,0x0018,VR::UI,VM::VM1>SOPInstanceUID={gen.Generate()};
    ds.Insert(SOPInstanceUID.GetAsDataElement());

    //Sequence Start
    gdcm::SmartPointer<gdcm::SequenceOfItems>ReferencedRTPlanSequence=new gdcm::SequenceOfItems();
    ReferencedRTPlanSequence->SetLengthToUndefined();

    gdcm::MediaStorage ms1 = gdcm::MediaStorage::RTImageStorage;
    std::string k=ms1.GetString();
    gdcm::Attribute<0x0008,0x1150,VR::UI,VM::VM1> ReferencedSOPClassUID = {k};
    gdcm::DataElement rfID = ReferencedSOPClassUID.GetAsDataElement();

    gdcm::Attribute<0x0008,0x1155,VR::UI,VM::VM1> ReferencedSOPInstanceUID = {suid.Generate()};
    gdcm::DataElement refSOPID = ReferencedSOPInstanceUID.GetAsDataElement();

    gdcm::Item ReferencedRTPlanItem;
    gdcm::DataSet &dsReferencedRTPlan = ReferencedRTPlanItem.GetNestedDataSet();
    dsReferencedRTPlan.Insert( rfID );
    dsReferencedRTPlan.Insert(refSOPID);

    ReferencedRTPlanSequence->AddItem(ReferencedRTPlanItem);

    gdcm::DataElement desReferencedRTPlan( gdcm::Tag(0x300C,0x0002) );
    desReferencedRTPlan.SetVR(gdcm::VR::SQ);
    desReferencedRTPlan.SetValue(*ReferencedRTPlanSequence);
    desReferencedRTPlan.SetVLToUndefined();
    ds.Insert(desReferencedRTPlan);

    //Sequence End

    gdcm::Attribute<0x0018,0x0060,VR::DS,VM::VM1> KVP = {40};
    ds.Insert( KVP.GetAsDataElement() );

    gdcm::Attribute<0x0018,0x1151,VR::IS,VM::VM1> XRayTubeCurrent = {20};
    ds.Insert( XRayTubeCurrent.GetAsDataElement() );

    gdcm::Attribute<0x0018,0x1150,VR::IS,VM::VM1> ExposureTime = {10};
    ds.Insert( ExposureTime.GetAsDataElement() );

    //Sequence Start

    gdcm::SmartPointer<gdcm::SequenceOfItems> ExposureSequence = new gdcm::SequenceOfItems();
    ExposureSequence->SetLengthToUndefined();

    gdcm::Attribute<0x0008,0x1160,VR::IS,VM::VM1> ReferencedFrameNum = {1};
    gdcm::DataElement ReferencedFrame = ReferencedFrameNum.GetAsDataElement();

    gdcm::Attribute<0x3002,0x0032,VR::DS,VM::VM1> MeterSetExpo = {1};
    gdcm::DataElement ExpoMeterset = MeterSetExpo.GetAsDataElement();

    gdcm::Attribute<0x300A,0x00F0,VR::IS,VM::VM1> NumOfBlocks = {0};
    gdcm::DataElement NBlocks = NumOfBlocks.GetAsDataElement();

    gdcm::Attribute<0x300A,0x011E,VR::DS,VM::VM1> GantryAngle1 = {0};
    gdcm::DataElement GAngle = GantryAngle1.GetAsDataElement();

    gdcm::Attribute<0x300A,0x0120,VR::DS,VM::VM1> BeamLimitingDeviceAngle1 = {};
    gdcm::DataElement BLDAngle = BeamLimitingDeviceAngle1.GetAsDataElement();

    gdcm::Attribute<0x300A,0x0122,VR::DS,VM::VM1> PatientSupportAngle1 = {};
    gdcm::DataElement PSAngle = PatientSupportAngle1.GetAsDataElement();

    gdcm::SmartPointer<gdcm::SequenceOfItems> BeamLimitingDeviceSequence = new gdcm::SequenceOfItems();
    BeamLimitingDeviceSequence->SetLengthToUndefined();

    for(int i=0; i<2;i++){
        if(i==0){

            gdcm::Attribute<0x300A,0x00B8,VR::CS,VM::VM1> RTBeamLimitingDeviceType = {"ASYMX"};
            gdcm::DataElement RtDeviceType = RTBeamLimitingDeviceType.GetAsDataElement();

            gdcm::Attribute<0x300A,0x00BC,VR::IS,VM::VM1> NumberOfLeafJawPairs = {3};
            gdcm::DataElement JawPairs= NumberOfLeafJawPairs.GetAsDataElement();

            gdcm::Attribute<0x300A,0x011C,VR::DS,VM::VM6> LeafJawPositions = {150,150,100,250,300,270};
            gdcm::DataElement JawPosition= LeafJawPositions.GetAsDataElement();

            gdcm::Item beamLimitingItem;
            gdcm::DataSet &dsbeamLimiting = beamLimitingItem.GetNestedDataSet();
            dsbeamLimiting.Insert( RtDeviceType );
            dsbeamLimiting.Insert(JawPairs);
            dsbeamLimiting.Insert(JawPosition);

            BeamLimitingDeviceSequence->AddItem(beamLimitingItem);
        }

        if(i==1){

            gdcm::Attribute<0x300A,0x00B8,VR::CS,VM::VM1> RTBeamLimitingDeviceType = {"ASYMY"};
            gdcm::DataElement RtDeviceType = RTBeamLimitingDeviceType.GetAsDataElement();

            gdcm::Attribute<0x300A,0x00BC,VR::IS,VM::VM1> NumberOfLeafJawPairs = {3};
            gdcm::DataElement JawPairs= NumberOfLeafJawPairs.GetAsDataElement();

            gdcm::Attribute<0x300A,0x011C,VR::DS,VM::VM6> LeafJawPositions = {150,150,100,250,300,270};
            gdcm::DataElement JawPosition= LeafJawPositions.GetAsDataElement();

            gdcm::Item beamLimitingItem;
            gdcm::DataSet &dsbeamLimiting = beamLimitingItem.GetNestedDataSet();
            dsbeamLimiting.Insert( RtDeviceType );
            dsbeamLimiting.Insert(JawPairs);
            dsbeamLimiting.Insert(JawPosition);
            BeamLimitingDeviceSequence->AddItem(beamLimitingItem);
        }

    }

        gdcm::DataElement desbeamLimitingItem( gdcm::Tag(0x300A,0x00B6) );
        desbeamLimitingItem.SetVR(gdcm::VR::SQ);
        desbeamLimitingItem.SetValue(*BeamLimitingDeviceSequence);
        desbeamLimitingItem.SetVLToUndefined();

        gdcm::Item ExposureItem;
        gdcm::DataSet &dsExposure = ExposureItem.GetNestedDataSet();
        dsExposure.Insert( ReferencedFrame );
        dsExposure.Insert(ExpoMeterset);
        dsExposure.Insert(NBlocks);
        dsExposure.Insert(GAngle);
        dsExposure.Insert(BLDAngle);
        dsExposure.Insert(PSAngle);
        dsExposure.Insert(desbeamLimitingItem);

        ExposureSequence->AddItem(ExposureItem);

        gdcm::DataElement desExposureSequenceItem( gdcm::Tag(0x3002,0x0030) );
        desExposureSequenceItem.SetVR(gdcm::VR::SQ);
        desExposureSequenceItem.SetValue(*ExposureSequence);
        desExposureSequenceItem.SetVLToUndefined();
        ds.Insert(desExposureSequenceItem);

        //Sequence End

        gdcm::Attribute<0x300A,0x011E,VR::DS,VM::VM1> GantryAngle = {0};
        ds.Insert(GantryAngle.GetAsDataElement());

        gdcm::Attribute<0x300A,0x0120,VR::DS,VM::VM1> BeamLimitingDeviceAngle = {};
        ds.Insert(BeamLimitingDeviceAngle.GetAsDataElement());

        gdcm::Attribute<0x300A,0x0122,VR::DS,VM::VM1> PatientSupportAngle = {};
        ds.Insert(PatientSupportAngle.GetAsDataElement());

        if(treatmentMachineName == "Bhabhatron"){
            gdcm::Attribute<0x300A,0x00B3,VR::CS,VM::VM1> PrimaryDosimeterUnit = {"MINUTE"};
            ds.Insert( PrimaryDosimeterUnit.GetAsDataElement() );
        }
        else{
            gdcm::Attribute<0x300A,0x00B3,VR::CS,VM::VM1> PrimaryDosimeterUnit = {"MU"};
            ds.Insert( PrimaryDosimeterUnit.GetAsDataElement() );
        }

        gdcm::Attribute<0x300A,0x0128,VR::DS,VM::VM1> TableTopVerticalPosition = {};
        ds.Insert(TableTopVerticalPosition.GetAsDataElement());

        gdcm::Attribute<0x300A,0x0129,VR::DS,VM::VM1> TableTopLongitudinalPosition = {};
        ds.Insert(TableTopLongitudinalPosition.GetAsDataElement());

        gdcm::Attribute<0x300A,0x012A,VR::DS,VM::VM1> TableTopLateralPosition = {};
        ds.Insert(TableTopLateralPosition.GetAsDataElement());

        gdcm::Attribute<0x300A,0x012C,VR::DS,VM::VM3> IsocenterPosition = {};
        ds.Insert( IsocenterPosition.GetAsDataElement() );

        gdcm::Attribute<0x300A,0x00F0,VR::IS,VM::VM1> NumberOfBlocks = {0};
        ds.Insert(NumberOfBlocks.GetAsDataElement());

        gdcm::Attribute<0x0018,0x5100,VR::CS,VM::VM1> PatientPosition = {"POS"};
        ds.Insert(PatientPosition.GetAsDataElement());

        gdcm::Attribute<0x0028,0x1052,VR::DS,VM::VM1> RescaleIntercept = {0.0};
        ds.Insert(RescaleIntercept.GetAsDataElement());

        gdcm::Attribute<0x0028,0x1053,VR::DS,VM::VM1> RescaleSlope = {1.0};
        ds.Insert(RescaleSlope.GetAsDataElement());

        gdcm::Attribute<0x0028,0x1054,VR::LO,VM::VM1> RescaleType = {"US"};
        ds.Insert(RescaleType.GetAsDataElement());

//        gdcm::Attribute<0x0028,0x1050,VR::DS,VM::VM1> WindowCenter = {32000};
//        ds.Insert(WindowCenter.GetAsDataElement());

//        gdcm::Attribute<0x0028,0x1051,VR::DS,VM::VM1> WindowWidth = {65535};
//        ds.Insert(WindowWidth.GetAsDataElement());

        gdcm::Attribute<0x300C,0x0006,VR::IS,VM::VM1> ReferencedBeamNumber = {1};
        ds.Insert(ReferencedBeamNumber.GetAsDataElement());

        gdcm::Attribute<0x300E,0x0002,VR::CS,VM::VM1> ApprovalStatus = {"UNAPPROVED"};
        ds.Insert(ApprovalStatus.GetAsDataElement());

        gdcm::Attribute<0x0008,0x0005,VR::CS,VM::VM1> SpecificCharacterSet = {"ISO_IR_100"};
        ds.Insert(SpecificCharacterSet.GetAsDataElement());

        unsigned int dims[2] = {};
        dims[0] = w;
        dims[1] = h;

        image1.SetDimensions( dims );

        image1.SetOrigin(0,0);
        image1.SetOrigin(1,1);


//        double *spacing1 = new double[2];
//        spacing1[0]=0.319;
//        spacing1[1]=0.319;

//        image1.SetSpacing(spacing1);

        gdcm::PixelFormat pf = gdcm::PixelFormat::UINT16;
        pf.SetSamplesPerPixel( 1 );
        image1.SetPixelFormat( pf );
        qDebug()<<"hi";

        gdcm::PhotometricInterpretation pi =
                gdcm::PhotometricInterpretation::MONOCHROME2;
        image1.SetPhotometricInterpretation( pi );

        image1.SetTransferSyntax( gdcm::TransferSyntax::ExplicitVRLittleEndian );

        gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
        pixeldata.SetByteValue( (char*)buffer, w*h*sizeof(unsigned short) );
        image1.SetDataElement( pixeldata );

        QString path = ("D:\\OnlineRnV_1.3.9");
        qDebug()<<"path"<<path;
        writer.SetFileName((path+"\\Images_"+ QString::number(2)+".dcm").toStdString().c_str());
        writer.SetFile(file);
        if( !writer.Write())
        {
            qDebug("Dicom Image Write Error!");
        }
        qDebug()<<"done";

}
