#ifndef COONSTRIANGULARSURFACE_H
#define COONSTRIANGULARSURFACE_H

#include <gl/gl.h>
#include "gl/GLU.h"
#include <QVector>
#include <QHash>
#include <QDebug>
#include "edge.h"
#include "definitions.h"

class CoonsTriangularSurface {

private:
    QVector<Point> vertexes;
    QVector<Vector> normals;

    const double stepLength;

    QHash<BarycenterPoint, Point> points;
    QHash<QPair<Point, Point>, Vector> tangents;

public:
    CoonsTriangularSurface();
    CoonsTriangularSurface(QVector<Point>&, Vector&, Vector&, Vector&, const double);

    BarycenterPoint cartesianToBarycentric(Point&);
    Point barycentricToCartesian(const BarycenterPoint&);
    QVector<BarycenterPoint> oppositePoints(BarycenterPoint&);
    void buildBernesteinApproximateSurface();
    Vector pointNormalVector(const BarycenterPoint&);
    Vector tangentInPoint(const BarycenterPoint &V, const BarycenterPoint &opV);
    Point &getSurfacePoint(BarycenterPoint key);

    static BarycenterPoint negPoint(BarycenterPoint&);
    static BarycenterPoint oppositeToVertexPoint(BarycenterPoint&, BarycenterPoint&);
    static Point &setZCoordinate(Point& p);
    QHash<BarycenterPoint, Point> getPoints() const;
    void setPoints(const QHash<BarycenterPoint, Point> &value);
    QPair<double, double> lambda(Point &s0, Point &s1, Vector &T01, Vector &T10);

    CoonsTriangularSurface &operator=(const CoonsTriangularSurface&);
    Point getBoundaryBernesteinApproximatePoint(BarycenterPoint &b);
    void buildHermiteApproximateSurface();
    Point getBoundaryHermiteApproximatePoint(BarycenterPoint &b);
    void draw() const;
    void drawNormals() const;
    void drawTangents() const;
    void drawTriangularNet() const;
};

inline uint qHash(const QPair<Point, Point>& pVV) {
    uint seed = 0;
    seed ^= qHash(pVV.first) + 0x9e3779b9;
    seed ^= qHash(pVV.second) + 0x9e3779b9;
    return seed;
}

#endif // COONSTRIANGULARSURFACE_H
