#include "edge.h"

Edge::Edge() {}

Edge::
    Edge(const Edge& other):
        point1(other.point1),
        point2(other.point2),
        isBoundary(other.isBoundary) {}

Edge::
    Edge(const QPointF &p1, const QPointF &p2):isBoundary(false) {

    point1 = p1.x() < p2.x()? p1 : p2;
    point2 = p1.x() > p2.x()? p1 : p2;
}

QPointF Edge::getStartPoint() const {
    return point1;
}

void Edge::setStartPoint(const QPointF &value) {
    point1 = value;
}

QPointF Edge::getEndPoint() const {
    return point2;
}

void Edge::setEndPoint(const QPointF &value) {
    point1 = value;
}


bool Edge::getIsBoundary() const {
    return isBoundary;
}

void Edge::setIsBoundary(bool value) {
    isBoundary = value;
}

bool Edge::operator==(const Edge &other) {
    return other.getEndPoint() == getEndPoint() && other.getStartPoint() == getStartPoint() ||
           other.getStartPoint() == getEndPoint() && other.getEndPoint() == getStartPoint();
}

bool Edge::operator!=(const Edge &other) {
    return !operator ==(other);
}

bool Edge::areConnected(const Edge& other) {
    return other.getEndPoint() == getEndPoint() || other.getStartPoint() == getStartPoint() ||
           other.getStartPoint() == getEndPoint() || other.getEndPoint() == getStartPoint();
}
