#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include <QVector>
#include <QSet>
#include <QVectorIterator>
#include <QPointF>
#include "triangularunit.h"
#include "convexhull.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(QVector<QPointF>&);
    DelaunayTriangulation(QVector<TriangularUnit>&);
    QSet<Edge> getEdges() const;
    QSet<Edge> findEdgesIncidentToPoint(QPointF p);

    void build(bool);
    void clear();
    void setPoints(const QVector<QPointF> &value);

    QVector<QPointF> inscribedCircleCenters;
    TriangularUnit definePointPositionToTriangulation(QPointF &point);

    bool crossPointIncidentEdge(Edge edge, QPointF p);
    QVector<int> getTriangularUnitNeighbors(TriangularUnit &triangular);
    static bool arePointsSeparatedByEdge(QPointF &p1, QPointF &p2, Edge &edge);

    void setConvexHull(const ConvexHull &value);

    ConvexHull getConvexHull() const;

    void convertToDelaunay();

private:
    QVector<QPointF> points;
    QVector<TriangularUnit> triangles;
    QSet<Edge> edges;
    ConvexHull convexHull;

    TriangularUnit getClosestTriangleToPoint(QPointF&);
    Edge& getClosestConvexEdgeToPoint(QPointF&);

    static int definePointPositionToTriangle(QPointF& pointToAdd, TriangularUnit& nearestTriangle);

    bool checkDelaunayConditionLocaly(QVector<TriangularUnit>& threeNewTriangles);

    void createFirstTriangle(QPointF point1, QPointF point2, QPointF point3);
    QVector<TriangularUnit> replaceTriangleWithThreeSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> replaceTriangleWithTwoSplittedParts(TriangularUnit& triangle, QPointF& point);
    QVector<TriangularUnit> createNewBoundaryTriangles(QPointF &outerPoint);

    void appendTriangles(QVector<TriangularUnit>& newTriangles);
    void appendTriangles(TriangularUnit& triangle);
    void appendEdges(QSet<Edge> &newEdges);
    void removeEdges(QSet<Edge> &newEdges);

    QVector<TriangularUnit> addPointToTriangulation(QPointF);
};

#endif // DELAUNAYTRIANGULATION_H
