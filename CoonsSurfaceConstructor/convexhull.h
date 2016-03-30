#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <QPointF>
#include <QVector>

class ConvexHull: public QVector<QPointF>
{
public:
    ConvexHull();
    QPointF getMinXPoint();
    void sortPointsByAngle();
    double getAngleTgBetweenPoints(QPointF, QPointF);
    bool sorted;
    void build();
    void append(QPointF p);
    QPointF& operator [](int index);
    void clear();
    bool isLeftTurn(QPointF c, QPointF a, QPointF b);
    QPointF &xAt(int index);
};

#endif // CONVEXHULL_H
