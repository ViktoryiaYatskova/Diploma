#include "grahamScan.h"
#include "exMath.h"
#include <iostream>

GrahamScan::GrahamScan():isBuilt(false), sorted(false){}

QList<QPointF> GrahamScan::getPoints() const{
    return points;
}

void GrahamScan::setPoints(const QList<QPointF>& value) {
    points = QList<QPointF>(value);
}

void GrahamScan::addPoints(QPointF point) {
    points.append(point);
}

int GrahamScan::size() {
    return points.length();
}

QList<Edge> GrahamScan::getHullEdges(){
    QList<Edge> edges;
    if(!isBuilt) return edges;

    Edge newEdge;

    for(int i = 1; i < hull.length(); i++) {
        QPointF point = hull.at(i),
               prevPoint = hull.at(i-1);

        newEdge = Edge(QPointF(floor(prevPoint.x()+0.5), floor(prevPoint.y()+0.5)),
                       QPointF(floor(point.x()+0.5), floor(point.y()+0.5)));

        edges.append(newEdge);
    }

    return edges;
}

QList<Edge> GrahamScan::getEdgesPreOrder(){
    QList<Edge> edges;
    if(points.length() < 1) return edges;

    QPointF prevPoint = points.at(0);
    Edge newEdge;

    for(int i = 1; i < points.length(); i++){
        QPointF point = points.at(i);

        newEdge = Edge(QPointF(floor(prevPoint.x()+0.5), floor(prevPoint.y()+0.5)),
                       QPointF(floor(point.x()+0.5), floor(point.y()+0.5)));

        edges.append(newEdge);
    }

    for(int i = 1; i < points.length(); i++) {
        QPointF point = points.at(i);
        QPointF prevPoint = points.at(i-1);

        newEdge = Edge(QPointF(floor(prevPoint.x() + 0.5),floor(prevPoint.y()+0.5)),
                       QPointF(floor(point.x() + 0.5), floor(point.y()+0.5)));

        edges.append(newEdge);
    }

    return edges;
}

bool GrahamScan::isBuilted() {
    return isBuilt;
}

bool GrahamScan::isSorted() {
    return sorted;
}

void GrahamScan::prebuild() {
    QPointF centerPoint = getMinXPoint();

    points.removeOne(centerPoint);
    points.prepend(centerPoint);

    sortPointsByAngle();
    sorted = true;
}

void GrahamScan::build(){
    isBuilt = true;

    QRoundList stackPoints;
    QRoundList roundPointsList(points);
    QPointF centerPoint = points[0];
    roundPointsList.push_back(centerPoint);

    stackPoints.append(points[0]);
    stackPoints.append(points[1]);

    QPointF curVertex = roundPointsList.get(1);
    QPointF nextVertex = roundPointsList.get(2);

    for(int i = 1; nextVertex != centerPoint;){
        if (GrahamScan::isRightTurn(curVertex, nextVertex, stackPoints.last())){
            stackPoints.append(curVertex);
            curVertex = roundPointsList.next(i++);
            nextVertex = roundPointsList.next(i);
        } else {
            curVertex = stackPoints.popBack();
        }
    }
    stackPoints.append(curVertex);
    stackPoints.append(nextVertex);

    hull = stackPoints;
}

QPointF GrahamScan::getMinXPoint(){
    double minX = 100000;
    QPointF resPoint;
    QList<QPointF>::iterator i;
    for (i = points.begin(); i != points.end(); ++i) {
        minX = std::min(i->x(), minX);
        if(i->x() == minX) {
            if(!(resPoint.x() < i->x() && resPoint.x() == minX))
                resPoint = *i;
        }
    }

    return resPoint;
}

void GrahamScan::sortPointsByAngle(){
    QPointF centerPoint = points[0];
    for (int i = points.length() - 1; i >= 1; i--) {
        for (int j = 1; j < i; j++) {
            double angle1 = getAngleTgBetweenPoints(points[j], centerPoint),
                   angle2 = getAngleTgBetweenPoints(points[j+1], centerPoint);

            if (angle1 < angle2) {
                QPointF tmp = points[j];
                points[j] = points[j+1];
                points[j+1] = tmp;
            }
        }
    }
    sorted = true;
}

double GrahamScan::getAngleTgBetweenPoints(QPointF point, QPointF centerPoint){
    return (point.y() - centerPoint.y())*1.0/(point.x() - centerPoint.x());
}

bool GrahamScan::isRightTurn(QPointF a, QPointF c, QPointF b){
    QPointF u(b.x() - a.x(), b.y() - a.y()),
           v(c.x() - a.x(), c.y() - a.y());
    return u.x()*v.y() - u.y()*v.x() >= 0;
}

QList<QPointF> GrahamScan::getHull() const{
    return hull;
}

void GrahamScan::setHull(const QList<QPointF> &value){
    hull = value;
}

void GrahamScan::clear(){
    points.clear();
    hull.clear();
    sorted = false;
    isBuilt = false;
}
