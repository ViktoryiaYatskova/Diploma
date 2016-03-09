#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include <QVector>
#include <QPointF>
#include "triangularunit.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(QVector<QPointF>&);
    DelaunayTriangulation(QVector<TriangularUnit>&);
    QVector<Edge> getEdges() const;

    void build();
    void appendBoundaryEdges(QVector<Edge> &newEdges);
    void appendBoundaryEdges(Edge newEdge1, Edge newEdge2);
    void clear();
    void setPoints(const QVector<QPointF> &value);

    QVector<QPointF> inscribedCircleCenters;

    int definePointPositionToTriangulation(QPointF &point);

    bool isBoundary(Edge &edge);
    bool isBoundary(TriangularUnit &tr);

private:
    QVector<QPointF> points;
    QVector<TriangularUnit> triangles;
    QVector<Edge> edges;
    QVector<Edge> boundaryEdges;

    TriangularUnit getClosestBoundaryTriangleToPoint(QPointF&);
    TriangularUnit getClosestTriangleToPoint(QPointF&);

    static int definePointPositionToTriangle(QPointF& pointToAdd, TriangularUnit& nearestTriangle);

    void checkDelaunayConditionLocaly(QVector<TriangularUnit>& threeNewTriangles);

    QVector<TriangularUnit> replaceTriangleWithThreeSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> replaceTriangleWithTwoSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> createNewBoundaryTriangles(TriangularUnit &triangle, QPointF &outerPoint);

    void appendTriangles(QVector<TriangularUnit>& newTriangles);
    void appendTriangles(TriangularUnit& triangle);
    void appendEdges(QVector<Edge> &newEdges);
};

#endif // DELAUNAYTRIANGULATION_H
