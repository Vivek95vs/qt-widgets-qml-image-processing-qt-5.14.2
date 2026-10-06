QT += core gui
# Remove QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# OpenMP support
QMAKE_CXXFLAGS += -openmp
LIBS += -fopenmp

SOURCES += main.cpp \
    performancemonitor.cpp

HEADERS += \
    performancemonitor.h
