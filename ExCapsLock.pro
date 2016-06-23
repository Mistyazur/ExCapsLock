#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T19:47:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExCapsLock
TEMPLATE = app


SOURCES += main.cpp\
    keyboardhook.cpp \
    cmditem.cpp \
    cmdpalette.cpp \
    systemcmd.cpp

HEADERS  += \
    keyboardhook.h \
    cmditem.h \
    cmdpalette.h \
    systemcmd.h

FORMS    += cmd.ui

LIBS += -lUser32

