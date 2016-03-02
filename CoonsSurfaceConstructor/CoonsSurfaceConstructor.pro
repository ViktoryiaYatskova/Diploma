#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T16:04:16
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoonsSurfaceConstructor
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            delaunaytriangulation.cpp \
            createscene.cpp \
            triangularunit.cpp \
            edge.cpp \
            exMath.cpp \
            grahamScan.cpp \
            qroundlist.cpp

HEADERS  += mainwindow.h \
            delaunaytriangulation.h \
            createscene.h \
            triangularunit.h \
            edge.h \
            exMath.h \
            grahamScan.h \
            qroundlist.h

FORMS    += mainwindow.ui
