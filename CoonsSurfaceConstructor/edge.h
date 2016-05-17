#ifndef EDGE_H
#define EDGE_H

#include <QSet>
#include <QString>
#include "config.h"
#include "definitions.h"

class Edge {

private:
    Point point1;
    Point point2;

public:
    static int dimension;

    Edge();
    Edge(const Edge&);
    Edge(const Point &p1, const Point &p2);

    Point getStartPoint() const;
    void setStartPoint(const Point &value);

    Point getEndPoint() const;
    void setEndPoint(const Point &value);

    bool operator==(const Edge &other) const;
    bool operator!=(const Edge &other) const;
    bool areConnected(const Edge &other);
    bool isAdjacentToPoint(const Point& p) const;
    double distantToPoint(Point& p);
    Point getMutualPoint(const Edge &other) const;
    bool isNull() const;
};


inline qreal roundTillPrecision(qreal n) {
    return ((int)(n*((int)1./CoonsPatches::PRECISION) + (n >= 0 ? 0.5 : -0.5))) * CoonsPatches::PRECISION;
}

inline uint qHash (const QVector3D& key){
    qreal x = roundTillPrecision(key.x());
    qreal y = roundTillPrecision(key.y());
    qreal z = roundTillPrecision(key.z());

    QString temp = QString("%0%1%2").arg(x).arg(y).arg(z);
    return qHash(temp);
}

inline uint qHash(const Edge& edge) {
    uint seed = 0;
    Point p1, p2;

    if (edge.getStartPoint().x() < edge.getEndPoint().x() ||
        (edge.getStartPoint().x() == edge.getEndPoint().x() &&
         edge.getStartPoint().y() < edge.getEndPoint().y())) {

        p1 = edge.getStartPoint();
        p2 = edge.getEndPoint();
    } else {
        p2 = edge.getStartPoint();
        p1 = edge.getEndPoint();
    }

    seed ^= qHash(p1) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= qHash(p2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}

#endif // EDGE_H
