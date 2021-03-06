#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T19:55:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paint
TEMPLATE = app


SOURCES += main.cpp\
        paint.cpp \
    paintscene.cpp \
    mainwindow.cpp \
    mycontroller.cpp \
    algo.cpp \
    base_algo.cpp \
    point_transf.cpp

HEADERS  += paint.h \
    paintscene.h \
    mainwindow.h \
    mycontroller.h \
    algo.h \
    base_algo.h \
    point_transf.h \
    f.h

FORMS    += paint.ui \
    mainwindow.ui \
    mycontroller.ui
