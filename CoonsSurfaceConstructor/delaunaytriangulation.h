#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include <QVector>
#include <QSet>
#include <QVectorIterator>
#include "definitions.h"
#include "triangularunit.h"
#include "convexhull.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(QVector<Point>&);
    DelaunayTriangulation(QVector<TriangularUnit>&);
    QSet<Edge> getEdges() const;
    QSet<Edge> findEdgesIncidentToPoint(Point p);

    void build(bool);
    void clear();
    void setPoints(const QVector<Point> &value);

    QVector<Point> inscribedCircleCenters;
    TriangularUnit definePointPositionToTriangulation(Point &point);
    static bool arePointsSeparatedByEdge(Point &p1, Point &p2, Edge &edge);
    void convertToDelaunay();

    void setConvexHull(const ConvexHull &value);
    ConvexHull getConvexHull() const;

    QVector<TriangularUnit> getTriangles() const;
    void setTriangles(const QVector<TriangularUnit> &value);
    QSet<TriangularUnit> getVertexAdjacentTriangles(const Point&) const;

    QVector<Point> getPoints() const;

private:
    QVector<Point> points;
    QVector<TriangularUnit> triangles;
    QSet<Edge> edges;
    ConvexHull convexHull;

    TriangularUnit getClosestTriangleToPoint(Point&);
    Edge& getClosestConvexEdgeToPoint(Point&);

    static int definePointPositionToTriangle(Point& pointToAdd, TriangularUnit& nearestTriangle);

    bool checkDelaunayConditionLocaly(QVector<TriangularUnit>& threeNewTriangles);

    QVector<TriangularUnit> replaceTriangleWithThreeSplittedParts(TriangularUnit& triangle, Point& point);
    QVector<TriangularUnit> replaceTriangleWithTwoSplittedParts(TriangularUnit& triangle, Point& point);
    QVector<TriangularUnit> createNewBoundaryTriangles(Point &outerPoint);
    QVector<TriangularUnit> addPointToTriangulation(Point);
    QSet<TriangularUnit> getTriangleEdgeNeighbors(int triangularIndex) const;

    void createFirstTriangle(Point point1, Point point2, Point point3);
    void appendTriangles(QVector<TriangularUnit>& newTriangles);
    void appendTriangles(TriangularUnit& triangle);
    void appendEdges(const QSet<Edge> &newEdges);
    void removeEdges(QSet<Edge> &newEdges);
};

#endif // DELAUNAYTRIANGULATION_H
