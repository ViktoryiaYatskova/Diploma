#include "convexhull.h"

ConvexHull::ConvexHull() {}

void ConvexHull::append(QPointF p) {
    QVector<QPointF>::append(p);
    build();
}

void ConvexHull::build() {
    QPointF centerPoint = getMinXPoint();

    this->removeOne(centerPoint);
    this->prepend(centerPoint);

    sortPointsByAngle();
    sorted = true;
}

QPointF ConvexHull::getMinXPoint() {
    double minX = 100000;
    QPointF resPoint;
    QVector<QPointF>::iterator i;
    for (i = begin(); i != end(); ++i) {
        minX = std::min(i->x(), minX);
        if(i->x() == minX) {
            if(!(resPoint.x() < i->x() && resPoint.x() == minX))
                resPoint = *i;
        }
    }

    return resPoint;
}

QPointF& ConvexHull::xAt(int index) {
    return QVector<QPointF>::operator [](index);
}

void ConvexHull::sortPointsByAngle(){
    QPointF centerPoint = this->at(0);
    for (int i = length() - 1; i >= 1; i--) {
        for (int j = 1; j < i; j++) {
            double angle1 = getAngleTgBetweenPoints(xAt(j), centerPoint),
                   angle2 = getAngleTgBetweenPoints(xAt(i), centerPoint);

            if (angle1 > angle2) {
                QPointF tmp = xAt(i);
                xAt(i)= xAt(j);
                xAt(j) = tmp;
            }
        }
    }
     sorted = true;
}

double ConvexHull::getAngleTgBetweenPoints(QPointF point, QPointF centerPoint){
    return (centerPoint.y() - point.y())*1.0/(centerPoint.x() - point.x());
}

bool ConvexHull::isLeftTurn(QPointF c, QPointF a, QPointF b){
    QPointF u(b.x() - a.x(), b.y() - a.y()),
           v(c.x() - a.x(), c.y() - a.y());
    return u.x()*v.y() - u.y()*v.x() >= 0;
}


void ConvexHull::clear(){
    QVector<QPointF>::clear();
    sorted = false;
}
