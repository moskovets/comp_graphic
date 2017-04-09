#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T20:04:18
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = laba1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicview.cpp \
    mycontroller.cpp \
    errors.cpp \
    CImage.cpp \
    mytimeanaliz.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    mygraphicview.h \
    mycontroller.h \
    my_scene.h \
    errors.h \
    point.h \
    point.h \
    CImage.h \
    mytimeanaliz.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    mygraphicview.ui \
    mycontroller.ui
