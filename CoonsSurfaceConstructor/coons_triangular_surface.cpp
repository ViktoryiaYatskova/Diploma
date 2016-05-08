#include "coons_triangular_surface.h"

CoonsTriangularSurface::
    CoonsTriangularSurface(TriangularUnit& triangle, Vector& N1, Vector& N2, Vector& N3):

    V1(triangle.getVertexes()[0]),
    V2(triangle.getVertexes()[1]),
    V3(triangle.getVertexes()[2]),
    n1(N1),
    n2(N2),
    n3(N3)
{}

BarycenterPoint CoonsTriangularSurface::
    cartesianToBarycentric(Point& p) {

    BarycenterPoint barPoint;
    double b1, b2, b3,
           x = p.x(), y = p.y(),
           x1 = V1.x(), y1 = V1.y(),
           x2 = V2.x(), y2 = V2.y(),
           x3 = V3.x(), y3 = V3.y();
    b3 = ((y - y1)*(x2 - x1) + (y2 - y1)*(x1 - x)) / ((y2 - y1)*(x1 - x3) + (x2 - x1)*(y3 - y1));
    if (std::abs(x1 - x2) > ExMath::PRECISION ) {
        b2 = ((x - x1) + (x1 - x3) * b3) / (x2 - x1);
    } else {
        b2 = ((y - y1) + (y1 - y3) * b3) / (y2 - y1);
    }
    b1 = 1 - b2 - b3;

    barPoint.setX(b1);
    barPoint.setY(b2);
    barPoint.setZ(b3);
    return barPoint;
}

Point CoonsTriangularSurface::
    barycentricToCartesian(BarycenterPoint& p) {

    double b1 =  p.x(), b2 =  p.y(), b3 =  p.z();
    double x = V1.x() * b1 + V2.x() * b2 + V3.x() * b3;
    double y = V1.y() * b1 + V2.y() * b2 + V3.y() * b3;
    double z = V1.z() * b1 + V2.z() * b2 + V3.z() * b3;
    return Point(x, y, z);
}

BarycenterPoint CoonsTriangularSurface::
    negPoint(BarycenterPoint& p) {

    return BarycenterPoint(1 - p.x(), 1 - p.y(), 1 - p.z());
}

QVector<BarycenterPoint> CoonsTriangularSurface::
    oppositePoints(BarycenterPoint &p1) {

    BarycenterPoint opV1(0, p1.y() / (p1.y() + p1.z()),  p1.z() / (p1.y() + p1.z()));
    BarycenterPoint opV2(p1.x() / (p1.x() + p1.z()), 0, p1.z() / (p1.x() + p1.z()));
    BarycenterPoint opV3(p1.x() / (p1.y() + p1.x()), p1.y() / (p1.y() + p1.x()), 0);

    QVector<BarycenterPoint> v;
    v.push_back(opV1);
    v.push_back(opV2);
    v.push_back(opV3);
    return v;
}
