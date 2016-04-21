#ifndef EDGE_H
#define EDGE_H

#include <QSet>
#include <QPointF>

class Edge {

private:
    QPointF point1;
    QPointF point2;

public:
    static int dimension;

    Edge();
    Edge(const Edge&);
    Edge(const QPointF &p1, const QPointF &p2);

    QPointF getStartPoint() const;
    void setStartPoint(const QPointF &value);

    QPointF getEndPoint() const;
    void setEndPoint(const QPointF &value);

    bool operator==(const Edge &other) const;
    bool operator!=(const Edge &other) const;
    bool areConnected(const Edge &other);
    double distantToPoint(QPointF& p);
    QPointF getMutualPoint(const Edge &other) const;
};

inline uint qHash (const QPointF & key){
    return qHash (key.x () + 0x9e3779b9 + key.y () );
}

inline uint qHash(const Edge& edge) {
    uint seed = 0;

    seed ^= qHash(edge.getStartPoint()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= qHash(edge.getEndPoint()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}

#endif // EDGE_H
