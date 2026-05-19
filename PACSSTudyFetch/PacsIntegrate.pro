QT += quick virtualkeyboard


CONFIG += c++14
win32: LIBS += -lkernel32 -luser32 -lole32 -luuid

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -ladvapi32

SOURCES += \
        dicommanager.cpp \
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


INCLUDEPATH +="D:\\build 2017 cuda 10.2\\DCMTK-new\\include"
LIBS +=-L"D:\\build 2017 cuda 10.2\\DCMTK-new\\DCMTK_R\\lib"\
-lofstd\
-lcmr\
-ldcmdata\
-ldcmdsig\
-ldcmect\
-ldcmfg\
-ldcmimage\
-ldcmimgle\
-ldcmiod\
-ldcmjpeg\
-ldcmjpls\
-ldcmnet\
-ldcmpmap\
-ldcmpstat\
-ldcmqrdb\
-ldcmrt\
-ldcmseg\
-ldcmsr\
-ldcmtkcharls\
-ldcmtls\
-ldcmtract\
-ldcmwlm\
-li2d\
-lijg8\
-lijg12\
-lijg16\
-loflog

HEADERS += \
    dicommanager.h

DISTFILES += \
    config.ini

