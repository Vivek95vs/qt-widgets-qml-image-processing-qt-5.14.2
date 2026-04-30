QT += quick
QT += core gui quick qml
QT -= widgets
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
        DicomController.cpp \
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

HEADERS += \
    DicomController.h
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
