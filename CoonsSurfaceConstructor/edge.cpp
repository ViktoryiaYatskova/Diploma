#include "edge.h"
#include "exMath.h"

Edge::Edge() {}

Edge::
    Edge(const Edge& other):
        point1(other.point1),
        point2(other.point2) {}

Edge::
    Edge(const Point &p1, const Point &p2){

    point1 = p1;
    point2 = p2;
}

Point Edge::
    getStartPoint() const {

    return point1;
}

void Edge::
    setStartPoint(const Point &value) {

    point1 = value;
}

Point Edge::getEndPoint() const {
    return point2;
}

void Edge::setEndPoint(const Point &value) {
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

bool Edge::isAdjacentToPoint(const Point &p) const {
    return ( point1 == p || point2 == p );
}

Point Edge::getMutualPoint(const Edge& other) const {
    if( other.getEndPoint() == getEndPoint() || other.getEndPoint() == getStartPoint() )
        return other.getEndPoint();

    return other.getStartPoint();
}

bool Edge::isNull() const {
   return point1.isNull() && point2.isNull();
}

double Edge::distantToPoint(Point &p) {
    double distToPoint1 = ExMath::distantBeetweenPoints(p, point1);
    double distToPoint2 = ExMath::distantBeetweenPoints(p, point2);
    double distToEdgeLine = ExMath::distantToLine(p, point1, point2);
    bool isClosestPointEdgeVertex = !ExMath::isPointProjectionOnSegment(p, point1, point2);

    if (isClosestPointEdgeVertex) {
        return std::min(distToPoint1, distToPoint2);
    }

    return distToEdgeLine;
}

inline bool operator==(const Edge& ed1, const Edge& ed2){
     return (ed1.getEndPoint() == ed2.getEndPoint() &&
             ed1.getStartPoint() == ed2.getStartPoint() ||
             ed1.getEndPoint() == ed2.getStartPoint() &&
             ed1.getStartPoint() == ed2.getEndPoint());
}
