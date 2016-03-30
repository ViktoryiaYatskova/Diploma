#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include <QVector>
#include <QPointF>
#include "triangularunit.h"
#include "convexhull.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(QVector<QPointF>&);
    DelaunayTriangulation(QVector<TriangularUnit>&);
    QVector<Edge> getEdges() const;

    void build();
    void clear();
    void setPoints(const QVector<QPointF> &value);

    QVector<QPointF> inscribedCircleCenters;

    int definePointPositionToTriangulation(QPointF &point);

    QVector<Edge> findEdgesIncidentToPoint(QPointF p);
    bool crossPointIncidentEdge(Edge edge, QPointF p);
private:
    QVector<QPointF> points;
    QVector<TriangularUnit> triangles;
    QVector<Edge> edges;
    ConvexHull convexHull;

    TriangularUnit getClosestTriangleToPoint(QPointF&);

    static int definePointPositionToTriangle(QPointF& pointToAdd, TriangularUnit& nearestTriangle);

    void checkDelaunayConditionLocaly(QVector<TriangularUnit>& threeNewTriangles);

    void createFirstTriangle(QPointF point1, QPointF point2, QPointF point3);
    QVector<TriangularUnit> replaceTriangleWithThreeSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> replaceTriangleWithTwoSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> createNewBoundaryTriangles(QPointF &outerPoint);

    void appendTriangles(QVector<TriangularUnit>& newTriangles);
    void appendTriangles(TriangularUnit& triangle);
    void appendEdges(QVector<Edge> &newEdges);
};

#endif // DELAUNAYTRIANGULATION_H
