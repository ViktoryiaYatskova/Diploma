#ifndef SURFACE_H
#define SURFACE_H

#include <QVector>
#include "coons_triangular_surface.h"
#include "delaunaytriangulation.h"
#include <QMap>

class Surface: public QVector<CoonsTriangularSurface> {

private:
    const int STEP_NUMBER;
    DelaunayTriangulation& triangulation;
    QMap<Point, Vector> vertexNormals;
    QMap<Point, QSet<TriangularUnit> > vertexTriangleNeighborhood;

public:
    Surface(DelaunayTriangulation&);

    void build();
    void clear();
    Vector getVertexNormal(const Point &vertex);
};

#endif // SURFACE_H
