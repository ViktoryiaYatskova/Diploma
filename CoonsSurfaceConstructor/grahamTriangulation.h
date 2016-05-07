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
    QList<Point> getPoints() const;
    void setPoints(const QList<Point>&);
    void addPoints(Point);
    int size();
    void build();
    void prebuild();
    Point getMinXPoint();
    void sortPointsByAngle();
    double getAngleTgBetweenPoints(Point, Point);
    QList<Edge> getEdgesPreOrder();
    bool isBuilted();
    bool isSorted();

    static bool isLeftTurn(Point, Point, Point);

    QList<Point> getHull() const;
    void setHull(const QList<Point> &value);

    void clear();

    QList<Edge> getEdges() const;
    void setEdges(const QList<Edge> &value);

private:
    QList<Point> points;
    QList<Point> hull;
    QList<Edge> edges;

    bool isBuilt;
    bool sorted;

    void appendEdge(Point p1, Point p2);
};

#endif // GRAHAMSCAN_H
