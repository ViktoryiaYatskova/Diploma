#include "point2d.h"

Point2D::Point2D() {}

Point2D::Point2D(const QPoint& p) {
    x = p.x();
    y = p.y();
}

Point2D& Point2D::getClosestPointTo(Point2D* otherPoints, int length) {
    if (length < 1) return;

    Point2D closestPoint, currentPoint;
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

Point2D* Point2D::getTwoClosestPointTo(Point2D* otherPoints, int length) {
    if (length < 1) return;

    Point2D *closestPoints = new Point2D[2];
    closestPoints[0] = this->getClosestPointTo(otherPoints, length);

    Point2D currentPoint;
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
 */
int Point2D::isPointInsideEscribedCircle(Vector < Point2D > & circlePoints) {
    if (circlePoints.length() < 3) throw new ;


    double x = this->x(), y = this->y(),
           x1 = circlePoints[0].x(), y1 = circlePoints[0].y(),
           x2 = circlePoints[1].x(), y2 = circlePoints[1].y(),
           x3 = circlePoints[2].x(), y3 = circlePoints[2].y();
   /* double **matrix = ExMath.initMatrix(4);
    matrix[0][0] = pow(x, 2) + pow(y, 2);   matrix[0][1] = x;  matrix[0][2] = y;  matrix[0][3] = 1;
    matrix[1][0] = pow(x1, 2) + pow(y1, 2); matrix[1][1] = x1; matrix[1][2] = y1; matrix[1][3] = 1;
    matrix[2][0] = pow(x2, 2) + pow(y2, 2); matrix[2][1] = x2; matrix[2][2] = y2; matrix[2][3] = 1;
    matrix[3][0] = pow(x3, 2) + pow(y3, 2); matrix[3][1] = x3; matrix[3][2] = y3; matrix[3][3] = 1;

    determinant(matrix, 4);*/

    double y0 = (((x1*x1 + y1*y1) - (x3*x3 + y3*y3)) / (2* (x1 - x3)) - ((x1*x1 + y1*y2) - (x2*x2 + y2*y2)) / (2* (x1 - x2))) /
                ((y2 - y1) / (x2 - x1) - (y3 - y1)/(x3 - x1));
    double x0 = ((x1*x1 + y1*y1) - (y2*y2 + x2*x2)) / (2 * (x1 - x2)) + y0 (y2 - y1) / (x2 - x1);

    double r = sqrt( pow((x1 - x0), 2) + pow((y1 - y0), 2) );

    return pow(x - x0, 2) + pow( y - y0, 2) - r*r;
}

double Point2D::distantTo(Point2D& point) {
    return sqrt(pow(point.x() - this->x(), 2) + pow(point.y() - this->y(), 2));
}

/**
 * @brief determinant
 * @param matrix
 * @param dim
 * @return
 */
double Point2D::determinant(double** matrix, int dim) {
    int p = 0, t;
    for (int i = 0; i < dim-1; i++) {
        t = 1;
        while (matrix[i][i] == 0) {
            for (int j = 0; j < dim; j++) {
                matrix[i][j] = kst;
                matrix[i][j] = matrix[i+t][j];
                matrix[i+t][j] = kst;
            }
            p++;
            t++;
        }

        for (int k = i+1; k < dim; k++) {
            kst = matrix[k][i]/matrix[i][i];
            for(int j = 0; j < n; j++)
                matrix[k][j] -= matrix[i][j]*kst;
        }
    }

    kst = pow(-1,p);
    for(int i = 0; i < dim; i++)
        kst *= matrix[i][i];

    return kst;
}

