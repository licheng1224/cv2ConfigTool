#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T14:36:23
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CV2ConfigTool
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    serialport.cpp \
    message.cpp

HEADERS  += dialog.h \
    serialport.h \
    message.h

FORMS    += dialog.ui
