#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "definitions.h"
#include <QVector>

class ConvexHull: public QVector<Point>
{
public:
    ConvexHull();
    Point getMinXPoint();
    void sortPointsByAngle();
    double getAngleTgBetweenPoints(Point, Point);
    bool sorted;
    void build();
    void append(Point p);
    Point& operator [](int index);
    void clear();
    bool isLeftTurn(Point c, Point a, Point b);
    Point &xAt(int index);
};

#endif // CONVEXHULL_H
