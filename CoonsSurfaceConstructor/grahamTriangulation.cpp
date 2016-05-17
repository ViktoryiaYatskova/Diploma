#include "grahamTriangulation.h"
#include "exMath.h"
#include <iostream>

GrahamTriangulation::GrahamTriangulation():isBuilt(false), sorted(false){}

QList<Point> GrahamTriangulation::getPoints() const{
    return points;
}

void GrahamTriangulation::setPoints(const QList<Point>& value) {
    points = QList<Point>(value);
}

void GrahamTriangulation::addPoints(Point point) {
    points.append(point);
}

int GrahamTriangulation::size() {
    return points.length();
}

QList<Edge> GrahamTriangulation::getEdgesPreOrder(){
    QList<Edge> edges;
    if(points.length() < 1) return edges;

    Point prevPoint = points.at(0);
    Edge newEdge;

    for(int i = 1; i < points.length(); i++){
        Point point = points.at(i);

        newEdge = Edge(Point(floor(prevPoint.x()+0.5), floor(prevPoint.y()+0.5), floor(prevPoint.z()+0.5)),
                       Point(floor(point.x()+0.5), floor(point.y()+0.5), floor(point.z()+0.5)));

        edges.append(newEdge);
    }

    for(int i = 1; i < points.length(); i++) {
        Point point = points.at(i);
        Point prevPoint = points.at(i-1);

        newEdge = Edge(
                          Point(floor(prevPoint.x() + 0.5),floor(prevPoint.y()+0.5), floor(prevPoint.z()+0.5)),
                          Point(floor(point.x() + 0.5), floor(point.y()+0.5), floor(point.z()+0.5))
                      );

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
    Point centerPoint = getMinXPoint();

    points.removeOne(centerPoint);
    points.prepend(centerPoint);

    sortPointsByAngle();
    sorted = true;
}

void GrahamTriangulation::build(){
    QRoundList stackPoints;
    QRoundList roundPointsList(points);
    Point centerPoint = points[0];

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
    Point prevVertex = roundPointsList.get(i++);
    Point curVertex = roundPointsList.get(i);

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

void GrahamTriangulation::appendEdge(Point p1, Point p2){
    Edge newEdge = Edge(Point(floor(p1.x()+0.5), floor(p1.y()+0.5), floor(p1.z()+0.5)),
                        Point(floor(p2.x()+0.5), floor(p2.y()+0.5), floor(p2.z()+0.5)));
    edges.append(newEdge);
}

Point GrahamTriangulation::getMinXPoint() {
    double minX = 100000;
    Point resPoint;
    QList<Point>::iterator i;
    for (i = points.begin(); i != points.end(); ++i) {
        minX = std::min((double)i->x(), minX);
        if(i->x() == minX) {
            if(!(resPoint.x() < i->x() && resPoint.x() == minX))
                resPoint = *i;
        }
    }

    return resPoint;
}

void GrahamTriangulation::sortPointsByAngle(){
    Point centerPoint = points[0];
    for (int i = points.length() - 1; i >= 1; i--) {
        for (int j = 1; j < i; j++) {
            double angle1 = getAngleTgBetweenPoints(points[j], centerPoint),
                   angle2 = getAngleTgBetweenPoints(points[j+1], centerPoint);

            if (angle1 < angle2) {
                Point tmp = points[j];
                points[j] = points[j+1];
                points[j+1] = tmp;
            }
        }
    }
    sorted = true;
}

double GrahamTriangulation::getAngleTgBetweenPoints(Point point, Point centerPoint){
    return (point.y() - centerPoint.y())*1.0/(point.x() - centerPoint.x());
}

bool GrahamTriangulation::isLeftTurn(Point c, Point a, Point b){
    Point u(b.x() - a.x(), b.y() - a.y(), 0),
          v(c.x() - a.x(), c.y() - a.y(), 0);
    return u.x()*v.y() - u.y()*v.x() >= 0;
}

QList<Point> GrahamTriangulation::getHull() const{
    return hull;
}

void GrahamTriangulation::setHull(const QList<Point> &value){
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

