#pragma once

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <cmath>

class ExMath {
public:
    ExMath();
    static double** initMatrix(int dim);
    static int determinant(double **matr, int n);
    static void getMatrix(double **matr, int n, double **temp_matr, int indRow, int indCol);
    static void freeMatrixMemory(double **matr, int n);
    static double* solveMatrixEquantion(double **a, double *b, int dim);
};

#endif // MATH_H
