#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"
#include <QVector3D>
#include <QPoint>

class Vector: public QVector3D {

public:
    Vector();
    Vector(const Vector&);
    Vector(const QVector3D&);
    Vector(const QPoint &p);
    Vector(double p1, double p2, double p3);
};

inline bool operator==(const Vector &v1, const Vector &v2) {
    return  std::fabs(v1.x() - v2.x()) < CoonsPatches::PRECISION &&
            std::fabs(v1.z() - v2.z()) < CoonsPatches::PRECISION &&
            std::fabs(v1.y() - v2.y()) < CoonsPatches::PRECISION;
}

#endif // VECTOR_H
