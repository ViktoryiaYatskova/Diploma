#ifndef TRIANGULARUNIT_H
#define TRIANGULARUNIT_H

#include "edge.h"
#include "exMath.h"
#include <QPointF>
#include <QVector>

class TriangularUnit {

private:
    QVector<QPointF> vertexes;
    QVector<Edge> edges;
    QVector<TriangularUnit> neighborTriangulars;

    bool isEmpty;
    QPointF escribedCircleCenter;
    QPointF inscribedCircleCenter;
    double escribedCircleRadius;

public:
    TriangularUnit();

    TriangularUnit(const TriangularUnit&);

    TriangularUnit(QPointF*);

    TriangularUnit(const QPointF, const QPointF, const QPointF);

    TriangularUnit(const Edge&, const Edge&, const Edge&);

    TriangularUnit(const Edge&, const QPointF);

    TriangularUnit(QPointF*, TriangularUnit*);

    QPointF getEscribedCircleCenter();
    QPointF getInscribedCircleCenter();

    bool operator ==(const TriangularUnit&);
    bool getIsEmpty();
    QVector<Edge>& getEdges();
    QVector<QPointF> getVertexes() const;
    void setVertexes(const QVector<QPointF> &value);
    Edge getTriangleEdgeThatContainsPoint(QPointF &point);
    QPointF getFrontPointToEdge(Edge &edge);
    Edge &getClosestEdgeToPoint(const QPointF &point);
    Edge &getClosestBoundaryEdgeToPoint(const QPointF &point);
    bool isPointInsideTriangle(QPointF point);
    bool isBoundary();
    void appendVertex(QPointF vertex);
    TriangularUnit(const Edge &edge1, const Edge &edge2);
};

#endif // TRIANGULARUNIT_H
