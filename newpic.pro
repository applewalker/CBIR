#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T20:05:49
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = newpic
TEMPLATE = app
QMAKE_CXXFLAGS += -march=i686

SOURCES += main.cpp\
        mainwindow.cpp \
    secondwindow.cpp \
    pictures.cpp \
    cGLCM.cpp \
    cHuMoments.cpp \
    cHSV.cpp \
    valuetodb.cpp \
    picstodb.cpp \
    InitWindows.cpp

HEADERS  += mainwindow.h \
    secondwindow.h \
    picturefunc.h \
    StdHeader.h \
    InitWindows.h
INCLUDEPATH+=D:\CMake\OpenCVforQt\include\opencv\
                    D:\CMake\OpenCVforQt\include\opencv2\
                    D:\CMake\OpenCVforQt\include
LIBS+=D:\CMake\OpenCVforQt\lib\libopencv_*.a
FORMS    += mainwindow.ui \
    secondwindow.ui

RESOURCES += \
    stylesheets.qrc


