#ifndef TRIANGULARUNIT_H
#define TRIANGULARUNIT_H

#include "edge.h"
#include "exMath.h"
#include "definitions.h"
#include <QVector>

class TriangularUnit {

private:
    QVector<Point> vertexes;
    QSet<Edge> edges;
    QSet<TriangularUnit> neighborTriangulars;

    bool isEmpty;
    bool isRestructered;
    Point escribedCircleCenter;
    Point inscribedCircleCenter;
    double escribedCircleRadius;

    void addPoints(const Point p1, const Point p2, const Point p3);
public:
    TriangularUnit();

    TriangularUnit(const TriangularUnit&);

    TriangularUnit(Point*);

    TriangularUnit(const Point, const Point, const Point);

    TriangularUnit(const Edge&, const Edge&, const Edge&);

    TriangularUnit(const Edge&, const Point);

    TriangularUnit(Point*, TriangularUnit*);
    TriangularUnit(const Edge &edge1, const Edge &edge2);

    Point getEscribedCircleCenter();
    Point getInscribedCircleCenter();

    bool operator ==(const TriangularUnit&) const;
    bool operator !=(const TriangularUnit&) const;
    bool getIsEmpty();
    QSet<Edge> getEdges() const;
    QVector<Point> getVertexes() const;
    void setVertexes(const QVector<Point> &value);
    Edge getTriangleEdgeThatContainsPoint(const Point &point);
    Point getFrontPointToEdge(const Edge&);
    Edge &getClosestEdgeToPoint(const Point &point);
    Point &getClosestVertexToPoint(const Point &point);
    bool isPointInsideTriangle(const Point& point);
    Point getNotAdjacentPoint(const TriangularUnit& adjacentTriangular);
    bool hasMutualEdge(const TriangularUnit &other) const;
    Edge getMutualEdge(const TriangularUnit &other) const;
    bool getIsRestructered() const;
    void setIsRestructered(bool value);
    void removeNeighbor(TriangularUnit &neighbor);
    void appendNeighbor(TriangularUnit &neighbor);
    QSet<TriangularUnit> getNeighborTriangulars() const;
    void setNeighborTriangulars(const QSet<TriangularUnit> &value);
    Vector normalVector();
    double getSinAngleBetweenEdges(const Point& mutualVertex);
};

inline uint qHash(const TriangularUnit& triangle) {
    uint seed = 0;

    seed ^= qHash(triangle.getVertexes().at(0)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= qHash(triangle.getVertexes().at(1)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= qHash(triangle.getVertexes().at(2)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}

#endif // TRIANGULARUNIT_H
