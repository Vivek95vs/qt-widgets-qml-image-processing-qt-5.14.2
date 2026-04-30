#-------------------------------------------------
#
# Project created by QtCreator 2025-12-09T12:07:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = encrypthpath
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH +="C:\\Program Files\\OpenSSL-Win64\\include"
LIBS +=-L"C:\\Program Files\\OpenSSL-Win64\\lib\\VC\\x64\MD"\
-llegacy\
-llibapps\
-llibcommon\
-llibcrypto\
-llibcrypto_static\
-llibdefault\
-lliblegacy\
-llibssl\
-llibssl_static\
-llibtemplate\
-llibtestutil\
-lloader_attic\
-lopenssl\
-lossltest\
-lp_minimal\
-lp_test\
-lpadlock\
-lcapi\
-ldasync\
-lec_internal_test\
-levp_extra_test2
LIBS +=-L"C:\\Program Files\\OpenSSL-Win64\\lib\\VC\\x64\MT"\
-llibcrypto\
-llibcrypto_static\
-llibssl\
-llibssl_static
