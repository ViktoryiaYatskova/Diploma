#include "convexhull.h"

ConvexHull::ConvexHull() {}

void ConvexHull::append(Point p) {
    QVector<Point>::append(p);
    build();
}

void ConvexHull::build() {
    Point centerPoint = getMinXPoint();

    this->removeOne(centerPoint);
    this->prepend(centerPoint);

    sortPointsByAngle();
    sorted = true;
}

Point ConvexHull::getMinXPoint() {
    double minX = 100000;
    Point resPoint;
    QVector<Point>::iterator i;
    for (i = begin(); i != end(); ++i) {
        minX = std::min((double)i->x(), minX);
        if(i->x() == minX) {
            if(!(resPoint.x() < i->x() && resPoint.x() == minX))
                resPoint = *i;
        }
    }

    return resPoint;
}

Point& ConvexHull::xAt(int index) {
    return QVector<Point>::operator [](index);
}

void ConvexHull::sortPointsByAngle(){
    Point centerPoint = this->at(0);
    for (int i = length() - 1; i >= 1; i--) {
        for (int j = 1; j < i; j++) {
            double angle1 = getAngleTgBetweenPoints(xAt(j), centerPoint),
                   angle2 = getAngleTgBetweenPoints(xAt(i), centerPoint);

            if (angle1 > angle2) {
                Point tmp = xAt(i);
                xAt(i)= xAt(j);
                xAt(j) = tmp;
            }
        }
    }
     sorted = true;
}

double ConvexHull::getAngleTgBetweenPoints(Point point, Point centerPoint){
    return (centerPoint.y() - point.y())*1.0/(centerPoint.x() - point.x());
}

bool ConvexHull::isLeftTurn(Point c, Point a, Point b){
    Point u(b.x() - a.x(), b.y() - a.y(), 0),
          v(c.x() - a.x(), c.y() - a.y(), 0);
    return u.x()*v.y() - u.y()*v.x() >= 0;
}


void ConvexHull::clear(){
    QVector<Point>::clear();
    sorted = false;
}
