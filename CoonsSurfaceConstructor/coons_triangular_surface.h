#ifndef COONSTRIANGULARSURFACE_H
#define COONSTRIANGULARSURFACE_H

#include <triangularunit.h>
#include <QVector>
#include "definitions.h"

class CoonsTriangularSurface {

private:
    Point& V1;
    Point& V2;
    Point& V3;

    Vector& n1;
    Vector& n2;
    Vector& n3;

public:
    CoonsTriangularSurface(TriangularUnit&, Vector&, Vector&, Vector&);

    BarycenterPoint cartesianToBarycentric(Point& p);
    Point barycentricToCartesian(BarycenterPoint& p);
    static BarycenterPoint negPoint(BarycenterPoint& p);
    QVector<BarycenterPoint> oppositePoints(BarycenterPoint&);
};

#endif // COONSTRIANGULARSURFACE_H
