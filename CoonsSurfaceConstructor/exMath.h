#pragma once

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>       // std::numeric_limits
#include <QPointF>
#include <iostream>
#include "point_position_to_triangle.h"

class ExMath {
private:
    static const double PRECISION;
public:
    static double DOUBLE_MAX;

    ExMath();
    static bool areLinesCrossed(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    static bool isPointOnLine(QPointF point, QPointF lineA,  QPointF lineB);
    static double** initMatrix(int dim);
    static double determinant(double **matr, int n);
    static void getMatrix(double **matr, int n, double **temp_matr, int indRow, int indCol);
    static void freeMatrixMemory(double **matr, int n);
    static double* solveMatrixEquantion(double **a, double *b, int dim);
    static double distantBeetweenPoints(const QPointF p1, const QPointF p2);
    static int pointPositionToLine(QPointF point, QPointF linePoint1, QPointF linePoint2);
    static int pointPositionToTriangle(QPointF a, QPointF b, QPointF c, QPointF p);
    static void consoleLog(const char *error);
    static void consoleLog(int error);
    static double **staticArray3ToDinamicCast(double arr[][3], int n);
    static double determinant3(double matr[3][3]);
    static double cosAngleBetweenVectors(QPointF endPoint1, QPointF mutualPoint, QPointF endPoint2);
    static bool isLeftTurn(QPointF c, QPointF a, QPointF b);
};

#endif // MATH_H
