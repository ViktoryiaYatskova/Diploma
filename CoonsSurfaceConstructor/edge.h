#ifndef EDGE_H
#define EDGE_H

#include <QPointF>

class Edge {

private:
    QPointF point1;
    QPointF point2;

    bool isBoundary;

public:
    static int dimension;

    Edge();
    Edge(const Edge&);
    Edge(const QPointF &p1, const QPointF &p2);

    QPointF getStartPoint() const;
    void setStartPoint(const QPointF &value);

    QPointF getEndPoint() const;
    void setEndPoint(const QPointF &value);

    bool getIsBoundary() const;
    void setIsBoundary(bool value);

    bool operator==(const Edge &other);
    bool operator !=(const Edge &other);
    bool areConnected(const Edge &other);
};

#endif // EDGE_H
