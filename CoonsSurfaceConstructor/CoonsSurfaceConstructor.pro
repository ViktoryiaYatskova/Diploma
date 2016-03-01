#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T16:04:16
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoonsSurfaceConstructor
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
    delaunaytriangulation.cpp \
    createscene.cpp \
    triangularunit.cpp \
    edge.cpp \
    vector.cpp \
    point2d.cpp \
    exMath.cpp

HEADERS  += mainwindow.h \
    delaunaytriangulation.h \
    createscene.h \
    triangularunit.h \
    edge.h \
    vector.h \
    point2d.h \
    exMath.h

FORMS    += mainwindow.ui
