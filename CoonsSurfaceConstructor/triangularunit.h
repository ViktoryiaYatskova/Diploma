#ifndef TRIANGULARUNIT_H
#define TRIANGULARUNIT_H

#include "edge.h"
#include "point2d.h"

class TriangularUnit {

private:
    Point2D* vertexes;
    Edge* edges;
    TriangularUnit* neighborTriangulars;
    bool isEmpty;

public:
    TriangularUnit();

    ~TriangularUnit();

    TriangularUnit(Point2D* trVertexes);

    TriangularUnit(Point2D&, Point2D&, Point2D&);

    TriangularUnit(Point2D* trVertexes, TriangularUnit* triangulars);

    bool getIsEmpty();
    Edge* getEdges();
};

#endif // TRIANGULARUNIT_H
