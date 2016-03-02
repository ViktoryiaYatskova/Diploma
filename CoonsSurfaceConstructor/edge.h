#ifndef EDGE_H
#define EDGE_H

#include <QPointF>

class Edge {

private:
    QPointF point1;
    QPointF point2;

public:
    static int dimension;

    Edge();
    Edge(const QPointF &p1, const QPointF &p2);

    QPointF getStartPoint() const;
    void setStartPoint(const QPointF &value);

    QPointF getEndPoint() const;
    void setEndPoint(const QPointF &value);
};

#endif // EDGE_H
