#include "grahamTriangulation.h"
#include "exMath.h"
#include <iostream>

GrahamTriangulation::GrahamTriangulation():isBuilt(false), sorted(false){}

QList<QPointF> GrahamTriangulation::getPoints() const{
    return points;
}

void GrahamTriangulation::setPoints(const QList<QPointF>& value) {
    points = QList<QPointF>(value);
}

void GrahamTriangulation::addPoints(QPointF point) {
    points.append(point);
}

int GrahamTriangulation::size() {
    return points.length();
}

QList<Edge> GrahamTriangulation::getEdgesPreOrder(){
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

bool GrahamTriangulation::isBuilted() {
    return isBuilt;
}

bool GrahamTriangulation::isSorted() {
    return sorted;
}

void GrahamTriangulation::prebuild() {
    QPointF centerPoint = getMinXPoint();

    points.removeOne(centerPoint);
    points.prepend(centerPoint);

    sortPointsByAngle();
    sorted = true;
}

void GrahamTriangulation::build(){
    QRoundList stackPoints;
    QRoundList roundPointsList(points);
    QPointF centerPoint = points[0];

    for(int j = 1; j < points.length(); j++) {
       // appendEdge(centerPoint, points[i]);
    }

    //roundPointsList.push_back(centerPoint);

    stackPoints.append(centerPoint);
    stackPoints.append(roundPointsList.get(1));
    stackPoints.append(roundPointsList.get(2));

    appendEdge(stackPoints.at(0), stackPoints.at(1));
    appendEdge(stackPoints.at(1), stackPoints.at(2));

    int i = 2;
    QPointF prevVertex = roundPointsList.get(i++);
    QPointF curVertex = roundPointsList.get(i);

    while( curVertex != centerPoint ){

        if (!isLeftTurn(curVertex, prevVertex, stackPoints.last())) {
            stackPoints.append(curVertex);

            prevVertex = roundPointsList.next(i++);
            curVertex = roundPointsList.next(i);

        } else {
            stackPoints.pop_back();
            prevVertex = stackPoints.last();
        }      
    }

    stackPoints.append(prevVertex);
    stackPoints.append(curVertex);

    for(int j = 0; j < stackPoints.length(); j++) {
       appendEdge(stackPoints.at(j), stackPoints.at( (j+1) % stackPoints.length()));
    }
    //appendEdge(prevVertex, curVertex);
    //appendEdge(curVertex, centerPoint);

    hull = stackPoints;
    isBuilt = true;
}

void GrahamTriangulation::appendEdge(QPointF p1, QPointF p2){
    Edge newEdge = Edge(QPointF(floor(p1.x()+0.5), floor(p1.y()+0.5)),
                   QPointF(floor(p2.x()+0.5), floor(p2.y()+0.5)));
    edges.append(newEdge);
}

QPointF GrahamTriangulation::getMinXPoint() {
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

void GrahamTriangulation::sortPointsByAngle(){
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

double GrahamTriangulation::getAngleTgBetweenPoints(QPointF point, QPointF centerPoint){
    return (point.y() - centerPoint.y())*1.0/(point.x() - centerPoint.x());
}

bool GrahamTriangulation::isLeftTurn(QPointF c, QPointF a, QPointF b){
    QPointF u(b.x() - a.x(), b.y() - a.y()),
           v(c.x() - a.x(), c.y() - a.y());
    return u.x()*v.y() - u.y()*v.x() >= 0;
}

QList<QPointF> GrahamTriangulation::getHull() const{
    return hull;
}

void GrahamTriangulation::setHull(const QList<QPointF> &value){
    hull = value;
}

void GrahamTriangulation::clear(){
    points.clear();
    hull.clear();
    edges.clear();
    sorted = false;
    isBuilt = false;
}

QList<Edge> GrahamTriangulation::getEdges() const {
    return edges;
}

void GrahamTriangulation::setEdges(const QList<Edge> &value) {
    edges = value;
}

