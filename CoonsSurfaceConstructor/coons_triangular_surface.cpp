#include "coons_triangular_surface.h"

CoonsTriangularSurface::CoonsTriangularSurface(TriangularUnit& triangle) {
    V1 = triangle.getVertexes().at(0);
    V2 = triangle.getVertexes().at(1);
    V3 = triangle.getVertexes().at(2);
}

// 1) decart -> baricenter
// 2)


QVector3D CoonsTriangularSurface::cartesianToBarycentric(QPointF& p) {
    QVector3D barPoint;
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

QPointF CoonsTriangularSurface::barycentricToCartesian(QVector3D& p) {
    double b1 =  p.x(), b2 =  p.y(), b3 =  p.z();
    x = V1.x() * b1 + V2.x() * b2 + V3.x() * b3;
    y = V1.y() * b1 + V2.y() * b2 + V3.y() * b3;
    return QPointF(x, y);
}

QVector3D CoonsTriangularSurface::negPoint(QVector3D& p) {
    return QVector3D(1 - p.x(), 1 - p.y(), 1 - p.z());
}

QVector<QVector3D> CoonsTriangularSurface::oppositePoints(QVector3D &p1) {
    QVector3D opV1(0, p1.y() / (p1.y() + p1.z()),  p1.z() / (p1.y() + p1.z()));
    QVector3D opV2(p1.x() / (p1.x() + p1.z()), 0, p1.z() / (p1.x() + p1.z()));
    QVector3D opV3(p1.x() / (p1.y() + p1.x()), p1.y() / (p1.y() + p1.x()), 0);

    QVector<QVector3D> v;
    v.push_back(opV1);
    v.push_back(opV2);
    v.push_back(opV3);
    return v;
}
