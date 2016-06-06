#pragma once

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>       // std::numeric_limits
#include "definitions.h"
#include <iostream>
#include "point_position_to_triangle.h"
#include "config.h"

class ExMath {

private:
public:
    static double DOUBLE_MAX;

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
    static void consoleLog(const char *error, bool endl=true);
    static void consoleLog(double error, bool endl=true);
    static double **staticArray3ToDinamicCast(double arr[][3], int n);
    static double determinant3(double matr[3][3]);
    static double cosAngleBetweenVectors(Point endPoint1, Point mutualPoint, Point endPoint2);
    static bool isLeftTurn(Point c, Point a, Point b);
    static bool isRightTurn(Point c, Point a, Point b);
    static double distantToLine(Point p, Point lineA, Point lineB);
    static bool isPointProjectionOnSegment(Point point, Point lineA, Point lineB);
    static double dotProduct2D(Point p1, Point p2);
    static double* bernstein(int n, double x);
    static bool precisionEqual(double x, double y);
    static double max(double a, double b, double c);
    static void consoleLogVector3D(const QVector3D &p1);
    static double manhattamDistanse(const QVector3D &p1, const QVector3D &p2);
    static void consoleLogSurfacePoint(BarycenterPoint &b, Point &p);
    static double* getHermiteCoefficients(double t);

    static double getHyperbolicParaboloidZ(double x, double y);
    static double getEllipticParaboloidZ(double x, double y);
    static double getConeZ(double x, double y);

    static Vector getNormalVectorHyperbolicParaboloid(double x, double y);
    static Vector getNormalVectorEllipticParaboloid(double x, double y);
    static Vector getNormalVectorCone(double x, double y);
    static double getCubicZ(double x, double y);
    static Vector getNormalVectorCubic(double x, double y);
};

inline bool operator< (const BarycenterPoint& p1, const BarycenterPoint& p2) {
    double x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y(), z1 = p1.z(), z2 = p2.z();
    //return p1.lengthSquared() < p2.lengthSquared();
    bool xx = ExMath::precisionEqual(x1, x2);
    bool yy = ExMath::precisionEqual(y1, y2);
    bool zz = ExMath::precisionEqual(z1, z2);
    return
            (!xx && x1 < x2) ||
            (xx && !yy && y1 < y2) ||
            (xx && yy && !zz && z1 < z2);
}

#endif // MATH_H
