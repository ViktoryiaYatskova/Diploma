#ifndef TRIANGULARUNIT_H
#define TRIANGULARUNIT_H

#include "edge.h"
#include <QPointF>

class TriangularUnit {

private:
    QPointF* vertexes;
    Edge* edges;
    TriangularUnit* neighborTriangulars;
    bool isEmpty;

public:
    TriangularUnit();

    //~TriangularUnit();

    TriangularUnit(QPointF*);

    TriangularUnit(QPointF&, QPointF&, QPointF&);

    TriangularUnit(QPointF*, TriangularUnit*);

    bool getIsEmpty();
    Edge* getEdges();
};

#endif // TRIANGULARUNIT_H
