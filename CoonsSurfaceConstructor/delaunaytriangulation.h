#pragma once
#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <exception>
#include "exMath.h"
#include "vector.h"
#include "point2d.h"
#include "triangularunit.h"
#include "edge.h"

class DelaunayTriangulation {

public:
    DelaunayTriangulation();
    DelaunayTriangulation(Vector < Point2D >& triangularPoints);
    DelaunayTriangulation(Vector < TriangularUnit >& triangulars);
    DelaunayTriangulation(Point2D *triangularPoints, int length);

    static DelaunayTriangulation& build(DelaunayTriangulation&);
    static DelaunayTriangulation& joinTriangulars(DelaunayTriangulation&, DelaunayTriangulation&);

private:
    Vector<Point2D> points;
    Vector<TriangularUnit> triangles;
    Vector<Edge> edges;

    static bool isPolygonConvex(Point2D*);
    static Vector <TriangularUnit>* triangularsFromConvexPolygon(Vector <Point2D>& polygon);

};

#endif // DELAUNAYTRIANGULATION_H
