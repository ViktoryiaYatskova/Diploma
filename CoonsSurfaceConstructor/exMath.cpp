#include "exMath.h"

void ExMath::consoleLog(const char* error) {
    std::cout<<error<<std::endl;
}

void ExMath::consoleLog(int error) {
    std::cout<<error<<std::endl;
}

ExMath::ExMath() {}

const double ExMath::PRECISION = 0.0001;

double ExMath::DOUBLE_MAX = std::numeric_limits<double>::max();

double ExMath::
    cosAngleBetweenVectors(Point endPoint1, Point  mutualPoint, Point endPoint2) {

    double vector1X = endPoint1.x() - mutualPoint.x(),
           vector1Y = endPoint1.y() - mutualPoint.y();

    double vector2X = endPoint2.x() - mutualPoint.x(),
           vector2Y = endPoint2.y() - mutualPoint.y();

    double modVector1 = sqrt(vector1X * vector1X + vector1Y * vector1Y);
    double modVector2 = sqrt(vector2X * vector2X + vector2Y * vector2Y);
    double scProduct =  vector1X*vector2X + vector1Y*vector2Y;

    return scProduct / (modVector1 * modVector2);
}

bool ExMath::
    isPointOnLine(Point point, Point lineA, Point lineB) {

    double product = dotProduct2D(lineB - point, lineA - point);
    product /= distantBeetweenPoints(lineB, point) * distantBeetweenPoints(lineA, point);

    return fabs(product + 1) < PRECISION;
}

float ExMath::dotProduct2D(Point p1, Point p2) {
    return p1.x() * p2.x() + p1.y() * p2.y();
}

bool ExMath::isPointProjectionOnSegment(Point point, Point lineA, Point lineB) {
    Point p1 = lineB, p3 = lineA, p2 = point;
    double cos = dotProduct2D(p2 - p3, p1 - p3);
    return cos < 0;
}

double ExMath::distantBeetweenPoints(const Point p1, const Point p2) {
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}

double ExMath::distantToLine(Point p, Point lineA, Point lineB) {
    return fabs(((lineA.y() - lineB.y()) * p.x() + (lineB.x() - lineA.x()) * p.y() + (lineA.x() * lineB.y() - lineB.x() * lineA.y())) /
            ExMath::distantBeetweenPoints(lineA, lineB));
}

int ExMath::
    pointPositionToLine(Point point, Point linePoint1, Point linePoint2) {

    double ax = linePoint1.x(), ay = linePoint1.y(), bx = linePoint2.x(), by = linePoint2.y(),
           px = point.x(), py = point.y();

    double s = (bx-ax)*(py-ay)-(by-ay)*(px-ax);

    return fabs(s) > PRECISION ? ( s / fabs(s) ): 0;
}

bool ExMath::isLeftTurn(Point c, Point a, Point b){
    Point u(b.x() - a.x(), b.y() - a.y(), 0),
           v(c.x() - a.x(), c.y() - a.y(), 0);
    return u.x()*v.y() - u.y()*v.x() >= 0;
}

bool ExMath::isRightTurn(Point c, Point a, Point b){
    return !isLeftTurn(c, a, b);
}

// функция определяет относительное положение точки: внутри или нет
int ExMath::
    pointPositionToTriangle(Point a, Point b, Point c, Point p) {

    int s1,s2,s3;
    s1 = pointPositionToLine(p, a, b);
    s2 = pointPositionToLine(p, b, c);
    s3 = pointPositionToLine(p, c, a);

    if ( s2*s1 > 0 && s3*s2 > 0 ) {
        return INSIDE;
    } else if ( s1*s2*s3 == 0 ) {
        return ON_EDGE;
    } else {
        return OUTSIDE;
    }
}

double** ExMath::staticArray3ToDinamicCast(double arr[][3], int n) {
    double** dinArr = new double*[n];

    for (int i = 0; i < n; i++) {
        dinArr[i] = new double[3];

        for(int j = 0; j < 3; j++) {
            dinArr[i][j] = arr[i][j];
        }
    }

    return dinArr;
}

double ExMath::determinant3(double matr[3][3]) {
    double **dinMatr = staticArray3ToDinamicCast(matr, 3);
    double res = determinant(dinMatr, 3);

    for (int i = 0; i < 3; i++) {
        delete [] dinMatr[i];
    }

    return res;
}

double area (Point a, Point b, Point c) {
    return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

bool intersect_1 (float a, float b, float c, float d) {
    int temp;
    if (a > b) {
        temp = a;
        a = b;
        b = temp;
    }
    if (c > d) {
        temp = c;
        c = d;
        d = temp;
    }
    return std::max(a,c) < std::min(b,d);
}

bool ExMath::areLinesCrossed(Point a, Point b, Point c, Point d) {
    return intersect_1 (a.x(), b.x(), c.x(), d.x())
        && intersect_1 (a.y(), b.y(), c.y(), d.y())
        && area(a,b,c) * area(a,b,d) < 0
        && area(c,d,a) * area(c,d,b) < 0;
}


//==============================================================================================================
//                              вычисление определителя
//==============================================================================================================
//функция вычисления определителя матрицы
double ExMath::determinant(double **matr, int n) {
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
Point& Point::
    getClosestPointTo(Point* otherPoints, int length) {

    if (length < 1) return;

    Point closestPoint, currentPoint;
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

Point* Point::
    getTwoClosestPointTo(Point* otherPoints, int length) {

    if (length < 1) return;

    Point *closestPoints = new Point[2];
    closestPoints[0] = this->getClosestPointTo(otherPoints, length);

    Point currentPoint;
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
//int Point::
//    isPointInsideEscribedCircle(QVector <Point> & circlePoints) {

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

//double Point::distantTo(Point &point) {
//    return sqrt(pow(point.x() - this->x(), 2) + pow(point.y() - this->y(), 2));
//}

///**
// * @brief determinant
// * @param matrix
// * @param dim
// * @return
// */
//double Point::determinant(double** matrix, int dim) {
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
