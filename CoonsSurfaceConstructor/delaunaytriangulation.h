#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include <QVector>
#include <QPointF>
#include "triangularunit.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(QVector<QPointF>&);
    DelaunayTriangulation(QVector<TriangularUnit>&);
    DelaunayTriangulation(QPointF*, int);

    void build(DelaunayTriangulation&);
private:
    QVector<QPointF> points;
    QVector<TriangularUnit> triangles;
    QVector<Edge> edges;
};

#endif // DELAUNAYTRIANGULATION_H
