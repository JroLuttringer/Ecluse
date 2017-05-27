#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T13:59:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    equipment.cpp \
    feu.cpp \
    vanne.cpp \
    porte.cpp \
    alarm.cpp \
    bassin.cpp \
    ecluse.cpp

HEADERS  += mainwindow.h \
    equipment.h \
    feu.h \
    vanne.h \
    porte.h \
    alarm.h \
    bassin.h \
    ecluse.h

FORMS    += mainwindow.ui

RESOURCES += \
    MyResources.qrc
