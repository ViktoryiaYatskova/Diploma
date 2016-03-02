#include "edge.h"

Edge::Edge(const QPointF &p1, const QPointF &p2):point1(p1), point2(p2) {}

QPointF Edge::getStartPoint() const {
    return point1;
}

void Edge::setStartPoint(const QPointF &value) {
    point1 = value;
}

QPointF Edge::getEndPoint() const {
    return point1;
}

void Edge::setEndPoint(const QPointF &value) {
    point1 = value;
}

Edge::Edge() {}
