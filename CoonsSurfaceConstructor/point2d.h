#ifndef POINT2D_H
#define POINT2D_H

#include "exMath.h"
#include <QPointF>
#include "vector.h"

using namespace std;

class Point2D: public QPointF {

public:
    static const int DIMENSION = 2;

    Point2D();
    Point2D(const QPoint&);

    double distantTo(Point2D& point);
    Point2D& getClosestPointTo(Point2D *otherPoints, int length);
    Point2D* getTwoClosestPointTo(Point2D *otherPoints, int length);
    int isPointInsideEscribedCircle(Vector < Point2D >& circlePoints);
    double determinant(double **matrix, int dim);
};

#endif // POINT2D_H
