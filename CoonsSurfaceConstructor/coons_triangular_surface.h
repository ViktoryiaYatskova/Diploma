#ifndef COONSTRIANGULARSURFACE_H
#define COONSTRIANGULARSURFACE_H

#include <triangularunit.h>
#include <QVector>
#include "definitions.h"

typedef QVector3D BarycenterPoint;

class CoonsTriangularSurface {

private:
    Point V1;
    Point V2;
    Point V3;

public:
    CoonsTriangularSurface(TriangularUnit&);

    BarycenterPoint cartesianToBarycentric(Point& p);
    Point barycentricToCartesian(BarycenterPoint& p);
    BarycenterPoint negPoint(BarycenterPoint& p);
    QVector<BarycenterPoint> oppositePoints(BarycenterPoint&);
};

#endif // COONSTRIANGULARSURFACE_H
