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
        cmd.cpp \
    keyboardhook.cpp

HEADERS  += cmd.h \
    keyboardhook.h

FORMS    += cmd.ui

LIBS += -lUser32
