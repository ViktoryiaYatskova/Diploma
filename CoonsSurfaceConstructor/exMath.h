#pragma once

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>       // std::numeric_limits
#include "definitions.h"
#include <iostream>
#include "point_position_to_triangle.h"

class ExMath {

private:
public:
    static double DOUBLE_MAX;
    static const double PRECISION;

    ExMath();
    static bool areLinesCrossed(Point p1, Point p2, Point p3, Point p4);
    static bool isPointOnLine(Point point, Point lineA,  Point lineB);
    static double** initMatrix(int dim);
    static double determinant(double **matr, int n);
    static void getMatrix(double **matr, int n, double **temp_matr, int indRow, int indCol);
    static void freeMatrixMemory(double **matr, int n);
    static double* solveMatrixEquantion(double **a, double *b, int dim);
    static double distantBeetweenPoints(const Point p1, const Point p2);
    static int pointPositionToLine(Point point, Point linePoint1, Point linePoint2);
    static int pointPositionToTriangle(Point a, Point b, Point c, Point p);
    static void consoleLog(const char *error);
    static void consoleLog(int error);
    static double **staticArray3ToDinamicCast(double arr[][3], int n);
    static double determinant3(double matr[3][3]);
    static double cosAngleBetweenVectors(Point endPoint1, Point mutualPoint, Point endPoint2);
    static bool isLeftTurn(Point c, Point a, Point b);
    static bool isRightTurn(Point c, Point a, Point b);
    static double distantToLine(Point p, Point lineA, Point lineB);
    static bool isPointProjectionOnSegment(Point point, Point lineA, Point lineB);
    static float dotProduct2D(Point p1, Point p2);
};

#endif // MATH_H
