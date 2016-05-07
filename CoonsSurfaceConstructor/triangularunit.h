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
    QVector<TriangularUnit> neighborTriangulars;

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

    bool operator ==(const TriangularUnit&);
    bool operator !=(const TriangularUnit&);
    bool getIsEmpty();
    QSet<Edge>& getEdges();
    QVector<Point> getVertexes() const;
    void setVertexes(const QVector<Point> &value);
    Edge getTriangleEdgeThatContainsPoint(Point &point);
    Point getFrontPointToEdge(Edge &edge);
    Edge &getClosestEdgeToPoint(const Point &point);
    Point &getClosestVertexToPoint(const Point &point);
    bool isPointInsideTriangle(Point point);
    Point getNotAdjacentPoint(TriangularUnit& adjacentTriangular);
    bool hasMutualEdge(TriangularUnit &other);
    Edge getMutualEdge(TriangularUnit &other);
    bool getIsRestructered() const;
    void setIsRestructered(bool value);
};

#endif // TRIANGULARUNIT_H
