#ifndef COONSTRIANGULARSURFACE_H
#define COONSTRIANGULARSURFACE_H

#include <QVector>
#include <QHash>
#include "edge.h"
#include "definitions.h"

class CoonsTriangularSurface {

private:    
    QVector<Point> vertexes;
    QVector<Vector> normals;

    const int stepNumber;

    QHash<BarycenterPoint, Point> points;

public:
    CoonsTriangularSurface();
    CoonsTriangularSurface(QVector<Point>&, Vector&, Vector&, Vector&, const int);

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
};

#endif // COONSTRIANGULARSURFACE_H
