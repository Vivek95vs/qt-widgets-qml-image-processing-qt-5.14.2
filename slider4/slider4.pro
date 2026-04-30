QT += quick
QT += sql
QT += widgets
QT += gui
QT += quick quickcontrols2 qml 3dcore 3drender 3dinput 3dextras
LIBS += -ladvapi32
CONFIG += c++14
QT += quick qml

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
        dicomimageprovider.cpp \
        main.cpp \
        vtkvolumerenderer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DicomHandler.h \
    dicomimageprovider.h \
    vtkvolumerenderer.h

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
INCLUDEPATH +="D:\\build 2017 cuda 10.2\\VTK_new\\include"
LIBS +=-L"D:\\build 2017 cuda 10.2\\VTK_new\\VTK_R\\lib"\
    -lvtkRenderingQt-8.2\
    -lvtkRenderingSceneGraph-8.2\
    -lvtkRenderingVolume-8.2\
    -lvtkRenderingVolumeAMR-8.2\
    -lvtkRenderingVolumeOpenGL2-8.2\
    -lvtksqlite-8.2\
    -lvtksys-8.2\
    -lvtkTestingGenericBridge-8.2\
    -lvtkTestingIOSQL-8.2\
    -lvtkTestingRendering-8.2\
    -lvtktheora-8.2\
    -lvtktiff-8.2\
    -lvtkverdict-8.2\
    -lvtkViewsContext2D-8.2\
    -lvtkViewsCore-8.2\
    -lvtkViewsGeovis-8.2\
    -lvtkViewsInfovis-8.2\
    -lvtkViewsQt-8.2\
    -lvtkVPIC-8.2\
    -lvtkWebCore-8.2\
    -lvtkWebGLExporter-8.2\
    -lvtkWrappingPython35Core-8.2\
    -lvtkWrappingTools-8.2\
    -lvtkzfp-8.2\
    -lvtkzlib-8.2\
    -lQVTKWidgetPlugin\
    -lvtkChartsCore-8.2\
    -lvtkCommonColor-8.2\
    -lvtkCommonComputationalGeometry-8.2\
    -lvtkCommonCore-8.2\
    -lvtkCommonDataModel-8.2\
    -lvtkCommonExecutionModel-8.2\
    -lvtkCommonMath-8.2\
    -lvtkCommonMisc-8.2\
    -lvtkCommonSystem-8.2\
    -lvtkCommonTransforms-8.2\
    -lvtkDICOMParser-8.2\
    -lvtkDomainsChemistry-8.2\
    -lvtkDomainsChemistryOpenGL2-8.2\
    -lvtkdoubleconversion-8.2\
    -lvtkexodusII-8.2\
    -lvtkexpat-8.2\
    -lvtkFiltersAMR-8.2\
    -lvtkFiltersCore-8.2\
    -lvtkFiltersExtraction-8.2\
    -lvtkFiltersFlowPaths-8.2\
    -lvtkFiltersGeneral-8.2\
    -lvtkFiltersGeneric-8.2\
    -lvtkFiltersGeometry-8.2\
    -lvtkFiltersHybrid-8.2\
    -lvtkFiltersHyperTree-8.2\
    -lvtkFiltersImaging-8.2\
    -lvtkFiltersModeling-8.2\
    -lvtkFiltersParallel-8.2\
    -lvtkFiltersParallelImaging-8.2\
    -lvtkFiltersPoints-8.2\
    -lvtkFiltersProgrammable-8.2\
    -lvtkFiltersSelection-8.2\
    -lvtkFiltersSMP-8.2\
    -lvtkFiltersSources-8.2\
    -lvtkFiltersStatistics-8.2\
    -lvtkFiltersTexture-8.2\
    -lvtkFiltersTopology-8.2\
    -lvtkFiltersVerdict-8.2\
    -lvtkfreetype-8.2\
    -lvtkGeovisCore-8.2\
    -lvtkgl2ps-8.2\
    -lvtkglew-8.2\
    -lvtkGUISupportMFC-8.2\
    -lvtkGUISupportQt-8.2\
    -lvtkGUISupportQtOpenGL-8.2\
    -lvtkGUISupportQtSQL-8.2\
    -lvtkhdf5_hl-8.2\
    -lvtkhdf5-8.2\
    -lvtkImagingColor-8.2\
    -lvtkImagingCore-8.2\
    -lvtkImagingFourier-8.2\
    -lvtkImagingGeneral-8.2\
    -lvtkImagingHybrid-8.2\
    -lvtkImagingMath-8.2\
    -lvtkImagingMorphological-8.2\
    -lvtkImagingOpenGL2-8.2\
    -lvtkImagingSources-8.2\
    -lvtkImagingStatistics-8.2\
    -lvtkImagingStencil-8.2\
    -lvtkInfovisCore-8.2\
    -lvtkInfovisLayout-8.2\
    -lvtkInteractionImage-8.2\
    -lvtkInteractionStyle-8.2\
    -lvtkInteractionWidgets-8.2\
    -lvtkIOAMR-8.2\
    -lvtkIOAsynchronous-8.2\
    -lvtkIOCityGML-8.2\
    -lvtkIOCore-8.2\
    -lvtkIOEnSight-8.2\
    -lvtkIOExodus-8.2\
    -lvtkIOExport-8.2\
    -lvtkIOExportOpenGL2-8.2\
    -lvtkIOExportPDF-8.2\
    -lvtkIOGeometry-8.2\
    -lvtkIOImage-8.2\
    -lvtkIOImport-8.2\
    -lvtkIOInfovis-8.2\
    -lvtkIOLegacy-8.2\
    -lvtkIOLSDyna-8.2\
    -lvtkIOMINC-8.2\
    -lvtkIOMovie-8.2\
    -lvtkIONetCDF-8.2\
    -lvtkIOParallel-8.2\
    -lvtkIOParallelXML-8.2\
    -lvtkIOPLY-8.2\
    -lvtkIOSegY-8.2\
    -lvtkIOSQL-8.2\
    -lvtkIOTecplotTable-8.2\
    -lvtkIOTRUCHAS-8.2\
    -lvtkIOVeraOut-8.2\
    -lvtkIOVideo-8.2\
    -lvtkIOVPIC-8.2\
    -lvtkIOXML-8.2\
    -lvtkIOXMLParser-8.2\
    -lvtkjpeg-8.2\
    -lvtkjsoncpp-8.2\
    -lvtkkissfft-8.2\
    -lvtklibharu-8.2\
    -lvtklibxml2-8.2\
    -lvtklz4-8.2\
    -lvtklzma-8.2\
    -lvtkmetaio-8.2\
    -lvtkNetCDF-8.2\
    -lvtkogg-8.2\
    -lvtkParallelCore-8.2\
    -lvtkpng-8.2\
    -lvtkproj-8.2\
    -lvtkpugixml-8.2\
    -lvtkPythonInterpreter-8.2\
    -lvtkRenderingAnnotation-8.2\
    -lvtkRenderingContext2D-8.2\
    -lvtkRenderingContextOpenGL2-8.2\
    -lvtkRenderingCore-8.2\
    -lvtkRenderingExternal-8.2\
    -lvtkRenderingFreeType-8.2\
    -lvtkRenderingGL2PSOpenGL2-8.2\
    -lvtkRenderingImage-8.2\
    -lvtkRenderingLabel-8.2\
    -lvtkRenderingLICOpenGL2-8.2\
    -lvtkRenderingLOD-8.2\
    -lvtkRenderingMatplotlib-8.2\
    -lvtkRenderingOpenGL2-8.2
