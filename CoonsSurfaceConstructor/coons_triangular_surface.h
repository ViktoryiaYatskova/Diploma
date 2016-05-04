#ifndef COONSTRIANGULARSURFACE_H
#define COONSTRIANGULARSURFACE_H

#include <triangularunit.h>
#include <QVector3D>
#include <QVector>
#include <QPointF>

class CoonsTriangularSurface {
private:
    QPointF V1;
    QPointF V2;
    QPointF V3;

public:
    CoonsTriangularSurface(TriangularUnit&);

    QVector3D cartesianToBarycentric(QPointF& p);
    QPointF barycentricToCartesian(QVector3D& p);
    QVector3D negPoint(QVector3D& p);
    QVector<QVector3D> oppositePoints(QVector3D&);
};

#endif // COONSTRIANGULARSURFACE_H
