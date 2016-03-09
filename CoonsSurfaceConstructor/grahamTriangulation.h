#ifndef GRAHAMSCAN_H
#define GRAHAMSCAN_H

#include <QList>
#include <QPoint>
#include <cmath>

#include "edge.h"
#include "qroundlist.h"

class GrahamTriangulation
{
public:
    GrahamTriangulation();
    QList<QPointF> getPoints() const;
    void setPoints(const QList<QPointF>&);
    void addPoints(QPointF);
    int size();
    void build();
    void prebuild();
    QPointF getMinXPoint();
    void sortPointsByAngle();
    double getAngleTgBetweenPoints(QPointF, QPointF);
    QList<Edge> getEdgesPreOrder();
    bool isBuilted();
    bool isSorted();

    static bool isLeftTurn(QPointF, QPointF, QPointF);

    QList<QPointF> getHull() const;
    void setHull(const QList<QPointF> &value);

    void clear();

    QList<Edge> getEdges() const;
    void setEdges(const QList<Edge> &value);

private:
    QList<QPointF> points;
    QList<QPointF> hull;
    QList<Edge> edges;

    bool isBuilt;
    bool sorted;

    void appendEdge(QPointF p1, QPointF p2);
};

#endif // GRAHAMSCAN_H
