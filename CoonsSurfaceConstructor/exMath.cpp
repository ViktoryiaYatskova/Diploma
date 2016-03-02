#include "exMath.h"

ExMath::ExMath() {}

//==============================================================================================================
//                              вычисление определителя
//==============================================================================================================
//функция вычисления определителя матрицы
int ExMath::determinant(double **matr, int n) {
    double temp = 0;   //временная переменная для хранения определителя
    int k = 1;      //степень
    if(n < 1) {
        return 0;
    } else if (n == 1) {
        temp = matr[0][0];
    } else if (n == 2) {
        temp = matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1];
    } else {
        for(int i = 0; i < n; i++){
            int m = n - 1;

            double **temp_matr = new double * [m];
            for(int j = 0; j < m; j++) {
                temp_matr[j] = new double [m];
            }

            getMatrix(matr, n, temp_matr, 0, i);
            temp = temp + k * matr[0][i] * determinant(temp_matr, m);
            k = -k;
            freeMatrixMemory(temp_matr, m);
        }
    }
    return temp;
}

void ExMath::freeMatrixMemory(double **matr, int n) {
    for(int i = 0; i < n; i++)
        delete [] matr[i];
    delete [] matr;
}

//функция вычеркивания строки и столбца
void ExMath::getMatrix(double **matr, int n, double **temp_matr, int indRow, int indCol) {
    int ki = 0;
    for (int i = 0; i < n; i++ ){
        if(i != indRow) {
            for (int j = 0, kj = 0; j < n; j++) {
                if (j != indCol){
                    temp_matr[ki][kj] = matr[i][j];
                    kj++;
                }
            }
            ki++;
        }
    }
}

double** initMatrix(int dim) {
    double **matrix = new double*[dim];

    for(int q = 0; q < dim; q++) {
        matrix[q] = new double[dim];

        for(int m = 0; m < dim; m++) {
            matrix[q][m] = 0;
        }
    }

    return matrix;
}

double* ExMath::solveMatrixEquantion(double** a, double* b, int dim) {
    int n = dim,i,k,j,p;
    double **l,**u,sum,*z,*x;

    l = new double*[dim];
    u = new double*[dim];

    for(int q = 0; q < dim; q++) {
        l[q] = new double[dim];
        u[q] = new double[dim];

        for(int m = 0; m < dim; m++) {
            l[q][m] = 0;
            u[q][m] = 0;
        }
    }

    z = new double[dim];
    x = new double[dim];
    for(int m = 0; m < dim; m++) {
        z[m] = 0;
        x[m] = 0;
    }

    //********** LU decomposition *****//
        for(k=1;k<=n;k++)
        {
            u[k][k]=1;
            for(i=k;i<=n;i++)
            {
                sum=0;
                for(p=1;p<=k-1;p++)
                    sum+=l[i][p]*u[p][k];
                l[i][k]=a[i][k]-sum;
            }

            for(j=k+1;j<=n;j++)
            {
                sum=0;
                for(p=1;p<=k-1;p++)
                    sum+=l[k][p]*u[p][j];
                u[k][j]=(a[k][j]-sum)/l[k][k];
            }
        }
        //******** Displaying LU matrix**********//
        /*cout<<endl<<endl<<"LU matrix is "<<endl;
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
                cout<<l[i][j]<<"  ";
            cout<<endl;
        }
        cout<<endl;
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
                cout<<u[i][j]<<"  ";
            cout<<endl;
        }*/

        //***** FINDING Z; LZ=b*********//

        for(i=1;i<=n;i++) {//forward subtitution method
            sum=0;
            for(p=1;p<i;p++)
            sum+=l[i][p]*z[p];
            z[i]=(b[i]-sum)/l[i][i];
        }
        //********** FINDING X; UX=Z***********//
        for(i=n;i>0;i--) {
            sum=0;
            for(p=n;p>i;p--)
                sum+=u[i][p]*x[p];
            x[i]=(z[i]-sum)/u[i][i];
        }
        //*********** DISPLAYING SOLUTION**************//
        /*cout<<endl<<"Set of solution is"<<endl;
        for(i=1;i<=n;i++)
            cout<<endl<<x[i];*/
        return z;
}

/*
QPointF& QPointF::
    getClosestPointTo(QPointF* otherPoints, int length) {

    if (length < 1) return;

    QPointF closestPoint, currentPoint;
    double closestDistant = -1;

    for (int i = 0; i < length; i++) {
        currentPoint = otherPoints[i];
        double distant = this->distantTo(currentPoint);
        if (closestDistant < 0 || distant < closestDistant) {
            closestDistant = distant;
            closestPoint = currentPoint;
        }
    }
    return closestPoint;
}

QPointF* QPointF::
    getTwoClosestPointTo(QPointF* otherPoints, int length) {

    if (length < 1) return;

    QPointF *closestPoints = new QPointF[2];
    closestPoints[0] = this->getClosestPointTo(otherPoints, length);

    QPointF currentPoint;
    double closestDistant = -1;

    for (int i = 0; i < length; i++) {
        currentPoint = otherPoints[i];
        double distant = this -> distantTo(currentPoint);

        if (currentPoint != closestPoints[0] &&
                (closestDistant < 0 || distant < closestDistant)) {

            closestDistant = distant;
            closestPoints[1] = currentPoint;
        }
    }

    return closestPoints;
}

/**
 * < 0 - inside the circle
 * = 0 - on the
 * > 0 - outside the circle
 *
 * @brief Point2D::isPointInsideEscribedCircle
 * @param circlePoints
 * @return
// */
//int QPointF::
//    isPointInsideEscribedCircle(QVector <QPointF> & circlePoints) {

//    if (circlePoints.length() < 3) throw new ;


//    double x = this->x(), y = this->y(),
//           x1 = circlePoints[0].x(), y1 = circlePoints[0].y(),
//           x2 = circlePoints[1].x(), y2 = circlePoints[1].y(),
//           x3 = circlePoints[2].x(), y3 = circlePoints[2].y();
//   /* double **matrix = ExMath.initMatrix(4);
//    matrix[0][0] = pow(x, 2) + pow(y, 2);   matrix[0][1] = x;  matrix[0][2] = y;  matrix[0][3] = 1;
//    matrix[1][0] = pow(x1, 2) + pow(y1, 2); matrix[1][1] = x1; matrix[1][2] = y1; matrix[1][3] = 1;
//    matrix[2][0] = pow(x2, 2) + pow(y2, 2); matrix[2][1] = x2; matrix[2][2] = y2; matrix[2][3] = 1;
//    matrix[3][0] = pow(x3, 2) + pow(y3, 2); matrix[3][1] = x3; matrix[3][2] = y3; matrix[3][3] = 1;

//    determinant(matrix, 4);*/

//    double y0 = (((x1*x1 + y1*y1) - (x3*x3 + y3*y3)) / (2* (x1 - x3)) - ((x1*x1 + y1*y2) - (x2*x2 + y2*y2)) / (2* (x1 - x2))) /
//                ((y2 - y1) / (x2 - x1) - (y3 - y1)/(x3 - x1));
//    double x0 = ((x1*x1 + y1*y1) - (y2*y2 + x2*x2)) / (2 * (x1 - x2)) + y0 (y2 - y1) / (x2 - x1);

//    double r = sqrt( pow((x1 - x0), 2) + pow((y1 - y0), 2) );

//    return pow(x - x0, 2) + pow( y - y0, 2) - r*r;
//}

//double QPointF::distantTo(QPointF &point) {
//    return sqrt(pow(point.x() - this->x(), 2) + pow(point.y() - this->y(), 2));
//}

///**
// * @brief determinant
// * @param matrix
// * @param dim
// * @return
// */
//double QPointF::determinant(double** matrix, int dim) {
//    int p = 0, t;
//    for (int i = 0; i < dim-1; i++) {
//        t = 1;
//        while (matrix[i][i] == 0) {
//            for (int j = 0; j < dim; j++) {
//                matrix[i][j] = kst;
//                matrix[i][j] = matrix[i+t][j];
//                matrix[i+t][j] = kst;
//            }
//            p++;
//            t++;
//        }

//        for (int k = i+1; k < dim; k++) {
//            kst = matrix[k][i]/matrix[i][i];
//            for(int j = 0; j < n; j++)
//                matrix[k][j] -= matrix[i][j]*kst;
//        }
//    }

//    kst = pow(-1,p);
//    for(int i = 0; i < dim; i++)
//        kst *= matrix[i][i];

//    return kst;
//}
