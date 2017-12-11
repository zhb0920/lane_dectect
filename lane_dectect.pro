#-------------------------------------------------
#
# Project created by QtCreator 2017-12-05T19:16:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = lane_dectect
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    process.cpp
INCLUDEPATH += /usr/local/include \
INCLUDEPATH += /usr/local/include/opencv \
INCLUDEPATH += /usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so

HEADERS += \
    process.h
