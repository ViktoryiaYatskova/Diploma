#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T16:04:16
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += c++11

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
            qroundlist.cpp \
    grahamTriangulation.cpp \
    convexhull.cpp

HEADERS  += mainwindow.h \
            delaunaytriangulation.h \
            createscene.h \
            triangularunit.h \
            edge.h \
            exMath.h \
            qroundlist.h \
    grahamTriangulation.h \
    point_position_to_triangle.h \
    convexhull.h

FORMS    += mainwindow.ui
