#ifndef TRIANGULARUNIT_H
#define TRIANGULARUNIT_H

#include "edge.h"
#include "exMath.h"
#include <QPointF>
#include <QVector>

class TriangularUnit {

private:
    QVector<QPointF> vertexes;
    QSet<Edge> edges;
    QVector<TriangularUnit> neighborTriangulars;

    bool isEmpty;
    bool isRestructered;
    QPointF escribedCircleCenter;
    QPointF inscribedCircleCenter;
    double escribedCircleRadius;

    void addPoints(const QPointF p1, const QPointF p2, const QPointF p3);
public:
    TriangularUnit();

    TriangularUnit(const TriangularUnit&);

    TriangularUnit(QPointF*);

    TriangularUnit(const QPointF, const QPointF, const QPointF);

    TriangularUnit(const Edge&, const Edge&, const Edge&);

    TriangularUnit(const Edge&, const QPointF);

    TriangularUnit(QPointF*, TriangularUnit*);
    TriangularUnit(const Edge &edge1, const Edge &edge2);

    QPointF getEscribedCircleCenter();
    QPointF getInscribedCircleCenter();

    bool operator ==(const TriangularUnit&);
    bool operator !=(const TriangularUnit&);
    bool getIsEmpty();
    QSet<Edge>& getEdges();
    QVector<QPointF> getVertexes() const;
    void setVertexes(const QVector<QPointF> &value);
    Edge getTriangleEdgeThatContainsPoint(QPointF &point);
    QPointF getFrontPointToEdge(Edge &edge);
    Edge &getClosestEdgeToPoint(const QPointF &point);
    QPointF &getClosestVertexToPoint(const QPointF &point);
    bool isPointInsideTriangle(QPointF point);
    QPointF getNotAdjacentPoint(TriangularUnit& adjacentTriangular);
    bool hasMutualEdge(TriangularUnit &other);
    Edge getMutualEdge(TriangularUnit &other);
    bool getIsRestructered() const;
    void setIsRestructered(bool value);
};

#endif // TRIANGULARUNIT_H
