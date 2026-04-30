QT += quick virtualkeyboard
QT += core gui quick qml
QT += widgets
QT += quick qml

LIBS += -ladvapi32

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DicomHandler.cpp \
        DicomImageProvider.cpp \
        VolumeRenderer.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# openCV
INCLUDEPATH += "D:\\build 2017 cuda 10.2\\opencv4.6without cuda\\include"
LIBS    += -L"D:\\build 2017 cuda 10.2\\opencv4.6without cuda\\x64\\vc15\\lib" \
#            -lopencv_world300\
#            -lopencv_ts300
             -lopencv_world460


##ITK
INCLUDEPATH +="D:\\build 2017 cuda 10.2\\ITK_latest\\include"
LIBS +=-L"D:\\build 2017 cuda 10.2\\ITK_latest\\itk_R\\lib"\
-litkAdaptiveDenoising-5.2\
-lITKBiasCorrection-5.2\
-lITKColormap-5.2\
-lITKCommon-5.2\
-lITKConvolution-5.2\
-lITKDeformableMesh-5.2\
-lITKDenoising-5.2\
-lITKDICOMParser-5.2\
-lITKDiffusionTensorImage-5.2\
-litkdouble-conversion-5.2\
-lITKEXPAT-5.2\
-lITKFastMarching-5.2\
-lITKFFT-5.2\
-lITKgiftiio-5.2\
-lITKGPUAnisotropicSmoothing-5.2\
-lITKGPUCommon-5.2\
-lITKGPUFiniteDifference-5.2\
-lITKGPUImageFilterBase-5.2\
-lITKGPUPDEDeformableRegistration-5.2\
-lITKGPUSmoothing-5.2\
-lITKGPUThresholding-5.2\
-litkgtest_main-5.2\
-litkgtest-5.2\
-litkhdf5_cpp-shared-5.2\
-litkhdf5-shared-5.2\
-lITKImageFeature-5.2\
-lITKImageIntensity-5.2\
-lITKIOBioRad-5.2\
-lITKIOBMP-5.2\
-lITKIOBruker-5.2\
-lITKIOCSV-5.2\
-lITKIOGDCM-5.2\
-lITKIOGE-5.2\
-lITKIOGIPL-5.2\
-lITKIOHDF5-5.2\
-lITKIOImageBase-5.2\
-lITKIOIPL-5.2\
-lITKIOJPEG-5.2\
-lITKIOJPEG2000-5.2\
-lITKIOLSM-5.2\
-lITKIOMeshBase-5.2\
-lITKIOMeshBYU-5.2\
-lITKIOMeshFreeSurfer-5.2\
-lITKIOMeshGifti-5.2\
-lITKIOMeshOBJ-5.2\
-lITKIOMeshOFF-5.2\
-lITKIOMeshVTK-5.2\
-lITKIOMeta-5.2\
-lITKIOMINC-5.2\
-lITKIOMRC-5.2\
-lITKIONIFTI-5.2\
-lITKIONRRD-5.2\
-lITKIOPNG-5.2\
-lITKIOSiemens-5.2\
-lITKIOSpatialObjects-5.2\
-lITKIOStimulate-5.2\
-lITKIOTIFF-5.2\
-lITKIOTransformBase-5.2\
-lITKIOTransformHDF5-5.2\
-lITKIOTransformInsightLegacy-5.2\
-lITKIOTransformMatlab-5.2\
-lITKIOVTK-5.2\
-lITKIOXML-5.2\
-litkjpeg-5.2\
-lITKKLMRegionGrowing-5.2\
-lITKLabelMap-5.2\
-litklbfgs-5.2\
-lITKMarkovRandomFieldsClassifiers-5.2\
-lITKMathematicalMorphology-5.2\
-lITKMesh-5.2\
-lITKMetaIO-5.2\
-litkminc2-5.2\
-litkNetlibSlatec-5.2\
-lITKniftiio-5.2\
-lITKNrrdIO-5.2\
-litkopenjpeg-5.2\
-lITKOptimizers-5.2\
-lITKOptimizersv4-5.2\
-lITKPath-5.2\
-lITKPDEDeformableRegistration-5.2\
-litkpng-5.2\
-lITKPolynomials-5.2\
-lITKQuadEdgeMesh-5.2\
-lITKQuadEdgeMeshFiltering-5.2\
-lITKRegionGrowing-5.2\
-lITKRegistrationMethodsv4-5.2\
-litkSimpleITKFilters-5.2\
-lITKSmoothing-5.2\
-litkSmoothingRecursiveYvvGaussianFilter-5.2\
-lITKSpatialObjects-5.2\
-lITKStatistics-5.2\
-litksys-5.2\
-lITKTestKernel-5.2\
-litktestlib-5.2\
-litktiff-5.2\
-lITKTransform-5.2\
-lITKTransformFactory-5.2\
-litkv3p_netlib-5.2\
-litkvcl-5.2\
-lITKVideoCore-5.2\
-lITKVideoIO-5.2\
-litkvnl_algo-5.2\
-litkvnl-5.2\
-lITKVNLInstantiation-5.2\
-lITKVTK-5.2\
-lITKWatersheds-5.2\
-litkzlib-5.2\
-lITKznz-5.2\
-lgdcmcharls\
-lgdcmCommon\
-lgdcmDICT\
-lgdcmDSED\
-lgdcmexpat\
-lgdcmgetopt\
-lgdcmIOD\
-lgdcmjpeg8\
-lgdcmjpeg12\
-lgdcmjpeg16\
-lgdcmMEXD\
-lgdcmMSFF\
-lgdcmopenjp2\
-lgdcmzlib\
-lsocketxx

##VTK
INCLUDEPATH += "D:/build 2017 cuda 10.2/VTK-9.2.60/include"
LIBS += -L"D:/build 2017 cuda 10.2/VTK-9.2.60/VTK_R/lib" \
        -lport\
        -lqmlvtkplugin\
        -lvtkcgns-9.2\
        -lvtkChartsCore-9.2\
        -lvtkCommon-9.2\
        -lvtkDICOMParser-9.2\
        -lvtkDomainsChemistry-9.2\
        -lvtkDomainsChemistryOpenGL2-9.2\
        -lvtkdoubleconversion-9.2\
        -lvtkexodusII-9.2\
        -lvtkexpat-9.2\
        -lvtkFilters-9.2\
        -lvtkFiltersFlowPaths-9.2\
        -lvtkFiltersHybrid-9.2\
        -lvtkfmt-9.2\
        -lvtkfreetype-9.2\
        -lvtkGeovisCore-9.2\
        -lvtkgl2ps-9.2\
        -lvtkglew-9.2\
        -lvtkGUISupportMFC-9.2\
        -lvtkGUISupportQt-9.2\
        -lvtkGUISupportQtQuick-9.2\
        -lvtkGUISupportQtSQL-9.2\
        -lvtkhdf5_hl-9.2\
        -lvtkhdf5-9.2\
        -lvtkImaging-9.2\
        -lvtkImagingHybrid-9.2\
        -lvtkImagingOpenGL2-9.2\
        -lvtkInfovisCore-9.2\
        -lvtkInfovisLayout-9.2\
        -lvtkInteraction-9.2\
        -lvtkIO-9.2\
        -lvtkIOAsynchronous-9.2\
        -lvtkIOCesium3DTiles-9.2\
        -lvtkIOChemistry-9.2\
        -lvtkIOCONVERGECFD-9.2\
        -lvtkIOExport-9.2\
        -lvtkIOExportGL2PS-9.2\
        -lvtkIOExportPDF-9.2\
        -lvtkIOImport-9.2\
        -lvtkIOInfovis-9.2\
        -lvtkIOIOSS-9.2\
        -lvtkIOMINC-9.2\
        -lvtkIOMotionFX-9.2\
        -lvtkioss-9.2\
        -lvtkIOTecplotTable-9.2\
        -lvtkIOTRUCHAS-9.2\
        -lvtkjpeg-9.2\
        -lvtkjsoncpp-9.2\
        -lvtkkissfft-9.2\
        -lvtklibharu-9.2\
        -lvtklibproj-9.2\
        -lvtklibxml2-9.2\
        -lvtkloguru-9.2\
        -lvtklz4-9.2\
        -lvtklzma-9.2\
        -lvtkmetaio-9.2\
        -lvtknetcdf-9.2\
        -lvtkogg-9.2\
        -lvtkOpenGL-9.2\
        -lvtkParallel-9.2\
        -lvtkpng-9.2\
        -lvtkpugixml-9.2\
        -lvtkRendering-9.2\
        -lvtkRenderingExternal-9.2\
        -lvtkRenderingGL2PSOpenGL2-9.2\
        -lvtkRenderingHyperTreeGrid-9.2\
        -lvtkRenderingLICOpenGL2-9.2\
        -lvtkRenderingQt-9.2\
        -lvtkRenderingRayTracing-9.2\
        -lvtkRenderingUI-9.2\
        -lvtkRenderingVR-9.2\
        -lvtkRenderingVtkJS-9.2\
        -lvtksqlite-9.2\
        -lvtksys-9.2\
        -lvtkTestingRendering-9.2\
        -lvtktheora-9.2\
        -lvtktiff-9.2\
        -lvtkverdict-9.2\
        -lvtkViews-9.2\
        -lvtkViewsInfovis-9.2\
        -lvtkViewsQt-9.2\
        -lvtkvpic-9.2\
        -lvtkWebGLExporter-9.2\
        -lvtkWrappingTools-9.2\
        -lvtkzfp-9.2\
        -lvtkzlib-9.2

##VTK Dicom library
INCLUDEPATH +="D:\\build 2017 cuda 10.2\\DicomLibrary_2\\include"
LIBS +=-L"D:\\build 2017 cuda 10.2\\DicomLibrary_2\\DICOM_R\\lib"\
        -ldicomcli\
        -lvtkDICOM-9.2

HEADERS += \
    DicomHandler.h \
    DicomImageProvider.h \
    VolumeRenderer.h
