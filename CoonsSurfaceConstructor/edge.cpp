#include "edge.h"
#include "exMath.h"

Edge::Edge() {}

Edge::
    Edge(const Edge& other):
        point1(other.point1),
        point2(other.point2) {}

Edge::
    Edge(const QPointF &p1, const QPointF &p2){

    point1 = p1.x() < p2.x()? p1 : p2;
    point2 = p1.x() > p2.x()? p1 : p2;
}

QPointF Edge::
    getStartPoint() const {

    return point1;
}

void Edge::
    setStartPoint(const QPointF &value) {

    point1 = value;
}

QPointF Edge::getEndPoint() const {
    return point2;
}

void Edge::setEndPoint(const QPointF &value) {
    point1 = value;
}

bool Edge::operator==(const Edge &other) const {
    return other.getEndPoint() == getEndPoint() && other.getStartPoint() == getStartPoint() ||
           other.getStartPoint() == getEndPoint() && other.getEndPoint() == getStartPoint();
}

bool Edge::operator!=(const Edge &other) const {
    return !operator ==(other);
}

bool Edge::areConnected(const Edge& other) {
    return other.getEndPoint() == getEndPoint() || other.getStartPoint() == getStartPoint() ||
            other.getStartPoint() == getEndPoint() || other.getEndPoint() == getStartPoint();
}

QPointF Edge::getMutualPoint(const Edge& other) const {
    if( other.getEndPoint() == getEndPoint() || other.getEndPoint() == getStartPoint() )
        return other.getEndPoint();

    return other.getStartPoint();
}

double Edge::distantToPoint(QPointF &p) {
    double distToPoint1 = ExMath::distantBeetweenPoints(p, point1);
    double distToPoint2 = ExMath::distantBeetweenPoints(p, point2);
    double distToEdgeLine = ((point1.y() - point2.y()) * p.x() + (point2.x() - point1.x()) * p.y() + (point1.x() * point2.y() - point2.x() * point1.y())) /
                            ExMath::distantBeetweenPoints(point1, point2);
    return std::min( std::min(distToPoint1, distToPoint2), distToEdgeLine);
}

inline bool operator==(const Edge& ed1, const Edge& ed2){
     return (ed1.getEndPoint() == ed2.getEndPoint() &&
             ed1.getStartPoint() == ed2.getStartPoint() ||
             ed1.getEndPoint() == ed2.getStartPoint() &&
             ed1.getStartPoint() == ed2.getEndPoint());
}
