#ifndef EDGE_H
#define EDGE_H

#include "point2d.h"
#include "exMath.h"
#include "vector.h"

class Edge {

private:
    Point2D point1;
    Point2D point2;

public:
    static int dimension;

    Edge(const Point2D&, const Point2D&);
    Edge();

    bool isCrossedByEdge(Edge& edge);
    bool isCrossedBySomeEdge(Vector <Edge>& edges);
    static Vector <Edge>& createAllPossibleEdgesThatNotCrossOthers(Vector < Point2D > &points, Vector <Edge> &edgesNotToCross);
};

#endif // EDGE_H
