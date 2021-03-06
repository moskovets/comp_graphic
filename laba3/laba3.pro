#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T20:04:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = laba1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicview.cpp \
    mycontroller.cpp \
    errors.cpp \
    work.cpp \
    drawLine.cpp \
    algo.cpp \
    analizalgo.cpp

HEADERS  += mainwindow.h \
    mygraphicview.h \
    mycontroller.h \
    my_scene.h \
    errors.h \
    work.h \
    drawLine.h \
    point.h \
    algo.h \
    point.h \
    analizalgo.h

FORMS    += mainwindow.ui \
    mygraphicview.ui \
    mycontroller.ui
