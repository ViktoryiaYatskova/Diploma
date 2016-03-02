#ifndef GRAHAMSCAN_H
#define GRAHAMSCAN_H

#include <QList>
#include <QPoint>
#include <cmath>

#include "edge.h"
#include "qroundlist.h"

class GrahamScan
{
public:
    GrahamScan();
    QList<QPointF> getPoints() const;
    void setPoints(const QList<QPointF>&);
    void addPoints(QPointF);
    int size();
    QList<Edge> getHullEdges();
    void build();
    void prebuild();
    QPointF getMinXPoint();
    void sortPointsByAngle();
    double getAngleTgBetweenPoints(QPointF, QPointF);
    QList<Edge> getEdgesPreOrder();
    bool isBuilted();
    bool isSorted();

    static bool isRightTurn(QPointF, QPointF, QPointF);

    QList<QPointF> getHull() const;
    void setHull(const QList<QPointF> &value);
    void clear();

private:
    QList<QPointF> points;
    QList<QPointF> hull;
    bool isBuilt;
    bool sorted;
};

#endif // GRAHAMSCAN_H
