#include "exMath.h"
#include "coons_triangular_surface.h"
#include <cstring>

CoonsTriangularSurface::
    CoonsTriangularSurface():stepLength(0.01){}

CoonsTriangularSurface::
    CoonsTriangularSurface(QVector<Point>& Vs, Vector& N1, Vector& N2, Vector& N3, const double st):

    vertexes(Vs),
    stepLength(st) {

    points[Point(1., 0., 0.)] = vertexes[0];
    points[Point(0., 1., 0.)] = vertexes[1];
    points[Point(0., 0., 1.)] = vertexes[2];

    normals.append(N1);
    normals.append(N2);
    normals.append(N3);
}

BarycenterPoint CoonsTriangularSurface::
    cartesianToBarycentric(Point& p) {

    BarycenterPoint barPoint,
            V1 = vertexes[0],
            V2 = vertexes[1],
            V3 = vertexes[2];
    double b1, b2, b3,
           x = p.x(), y = p.y(),
           x1 = V1.x(), y1 = V1.y(),
           x2 = V2.x(), y2 = V2.y(),
           x3 = V3.x(), y3 = V3.y();
    b3 = ((y - y1)*(x2 - x1) + (y2 - y1)*(x1 - x)) / ((y2 - y1)*(x1 - x3) + (x2 - x1)*(y3 - y1));
    if (std::abs(x1 - x2) > CoonsPatches::PRECISION ) {
        b2 = ((x - x1) + (x1 - x3) * b3) / (x2 - x1);
    } else {
        b2 = ((y - y1) + (y1 - y3) * b3) / (y2 - y1);
    }
    b1 = 1. - b2 - b3;

    barPoint.setX(b1);
    barPoint.setY(b2);
    barPoint.setZ(b3);
    return barPoint;
}

Point CoonsTriangularSurface::
    barycentricToCartesian(const BarycenterPoint& p) {

    BarycenterPoint
            V1 = vertexes[0],
            V2 = vertexes[1],
            V3 = vertexes[2];
    double b1 =  p.x(), b2 =  p.y(), b3 =  p.z();
    double x = V1.x() * b1 + V2.x() * b2 + V3.x() * b3;
    double y = V1.y() * b1 + V2.y() * b2 + V3.y() * b3;
    Point newPoint(x, y, 0.);
    return setZCoordinate(newPoint);
}

BarycenterPoint CoonsTriangularSurface::
    negPoint(BarycenterPoint& p) {

    return BarycenterPoint(1. - p.x(), 1. - p.y(), 1. - p.z());
}

QVector<BarycenterPoint> CoonsTriangularSurface::
    oppositePoints(BarycenterPoint &p) {

    BarycenterPoint opV1(0, p.y() / (p.y() + p.z()),  p.z() / (p.y() + p.z()));
    BarycenterPoint opV2(p.x() / (p.x() + p.z()), 0, p.z() / (p.x() + p.z()));
    BarycenterPoint opV3(p.x() / (p.y() + p.x()), p.y() / (p.y() + p.x()), 0);

    QVector<BarycenterPoint> v;
    v.push_back(opV1);
    v.push_back(opV2);
    v.push_back(opV3);
    return v;
}

BarycenterPoint CoonsTriangularSurface::
    oppositeToVertexPoint(BarycenterPoint &p, BarycenterPoint &V) {

    BarycenterPoint nV = negPoint(V);
    double pnV = Vector::dotProduct(p,nV);
    return BarycenterPoint( p.x() * nV.x(), p.y() * nV.y(), p.z() * nV.z() ) / pnV;
}

Point &CoonsTriangularSurface::
    setZCoordinate(Point& p) {

    double z;

    switch(CoonsPatches::SURFACE_TYPE) {

    case CoonsPatches::SURFACE_TYPES::HYPERBOLIC_PARABOLOID:
        z = ExMath::getHyperbolicParaboloidZ(p.x(), p.y());
        break;

    case CoonsPatches::SURFACE_TYPES::ELLIPTIC_PARABOLOID:
        z = ExMath::getEllipticParaboloidZ(p.x(), p.y());
        break;

    case CoonsPatches::SURFACE_TYPES::CONE:
        z = ExMath::getConeZ(p.x(), p.y());
        break;

    case CoonsPatches::SURFACE_TYPES::CUBIC:
        z = ExMath::getCubicZ(p.x(), p.y());
        break;
    }

    p.setZ(z);
    return p;
}

void CoonsTriangularSurface::
    buildBernesteinApproximateSurface() {

    double delta = stepLength;
    double one = 1. + CoonsPatches::PRECISION;
    double error = 0;
    int count = 0;
    BarycenterPoint opV, V;

    // define inner points
    for(double i = 0.; i <= one; i += delta) {
        for(double j = 0., k; j <= one - i; j += delta) {
            k = std::max(0., 1. - i - j);
            BarycenterPoint p(i, j, k);//( l == 0? i : l == 1? j: k,
                             //  l == 0? j : l == 1? k: i,
                             //  l == 0? k : l == 1? i: j);
            Point sP;
            double divCoef = p.y()* p.y() * p.z() * p.z()+
                             p.x()* p.x() * p.z() * p.z()+
                             p.x()* p.x() * p.y() * p.y();

            if (!points[p].isNull()) continue;

            for(int l = 0; l < 3; l++) {
                V = BarycenterPoint(l == 0 ? 1: 0, l == 1? 1: 0, l == 2? 1: 0);
                // 0 - 2, 2 - 1, 1 - 0
                opV = oppositeToVertexPoint(p, V);
                Point s0 = getSurfacePoint(V),
                      s1 = getBoundaryBernesteinApproximatePoint(opV);

                //ExMath::consoleLogSurfacePoint(opV, s1);

                Vector T01 = tangentInPoint(V, opV);
                Vector T10 = tangentInPoint(opV, V);

                QPair<double, double> lmd = lambda(s0, s1, T01, T10);
                double coef = V.x() * p.y() * p.y() * p.z() * p.z()+
                              V.y() * p.x() * p.x() * p.z() * p.z()+
                              V.z() * p.x() * p.x() * p.y() * p.y();
                double* B3 = ExMath::bernstein(3, 1. - (p*V).length() );

                sP += (coef/divCoef) * (s0 * B3[0] + (s0 + lmd.first * T01) * B3[1] +
                    (s1 + lmd.second * T10) * B3[2] + s1 * B3[3]);

                delete [] B3;
            }

            points[p] = sP;
            // Find error!!!!
            double trueZ = 0;
            switch(CoonsPatches::SURFACE_TYPE) {

            case CoonsPatches::SURFACE_TYPES::HYPERBOLIC_PARABOLOID:
                trueZ = ExMath::getHyperbolicParaboloidZ(p.x(), p.y());
                break;

            case CoonsPatches::SURFACE_TYPES::ELLIPTIC_PARABOLOID:
                trueZ = ExMath::getEllipticParaboloidZ(p.x(), p.y());
                break;

            case CoonsPatches::SURFACE_TYPES::CONE:
                trueZ = ExMath::getConeZ(p.x(), p.y());
                break;

            case CoonsPatches::SURFACE_TYPES::CUBIC:
                trueZ = ExMath::getCubicZ(p.x(), p.y());
                break;
            }
            error += std::fabs(sP.z() - trueZ);
            count++;
        }
    }
    //qDebug() << error / (double) count;
}

void CoonsTriangularSurface::
    drawNormals() const {

    const double NORMAL_LENGTH = 0.5;
    double defect = 0;
    int num = 0;
    glBegin(GL_LINES);
        for (int i = 0; i < 3; i++) {
            Point P1 = vertexes[i];
            Point P2 = normals[i]*NORMAL_LENGTH + P1;
            Vector realNormal;

            switch(CoonsPatches::SURFACE_TYPE) {

            case CoonsPatches::SURFACE_TYPES::HYPERBOLIC_PARABOLOID:
                realNormal = ExMath::getNormalVectorHyperbolicParaboloid(P1.x(), P1.y());
                break;

            case CoonsPatches::SURFACE_TYPES::ELLIPTIC_PARABOLOID:
                realNormal = ExMath::getNormalVectorEllipticParaboloid(P1.x(), P1.y());
                break;

            case CoonsPatches::SURFACE_TYPES::CONE:
                realNormal = ExMath::getNormalVectorCone(P1.x(), P1.y());
                break;

            case CoonsPatches::SURFACE_TYPES::CUBIC:
                realNormal = ExMath::getNormalVectorCubic(P1.x(), P1.y());
                break;
            }

            defect += (realNormal - normals[i]).length();
            num++;

            glVertex3f(P1.x(), P1.y(), P1.z());
            glVertex3f(P2.x(), P2.y(), P2.z());
        }
    glEnd();

    //qDebug() << defect / (double)num;
}

void CoonsTriangularSurface::
    drawTangents() const {

    const double NORMAL_LENGTH = 0.5;
    QHashIterator<QPair<Point, Point>, Vector> it(tangents);

    glBegin(GL_LINES);
        while (it.hasNext()) {
            it.next();
            Point P1 = it.key().first;
            Point P2 = P1 + it.value()*NORMAL_LENGTH;

            glVertex3f(P1.x(), P1.y(), P1.z());
            glVertex3f(P2.x(), P2.y(), P2.z());
        }
    glEnd();
}

void CoonsTriangularSurface::
    buildHermiteApproximateSurface() {

    double one = 1. + CoonsPatches::PRECISION;
    double delta = stepLength;
    BarycenterPoint opV, V;

    // define inner points
    for(double i = 0.; i <= one; i += delta) {
        for(double j = 0.; j <= one - i; j += delta) {
            BarycenterPoint p(i, j, 1. - j - i);
            Point sp;
            double divCoef =  p.y() * p.y() * p.z() * p.z()+
                              p.x() * p.x() * p.z() * p.z()+
                              p.x() * p.x() * p.y() * p.y();

            if (!points[p].isNull()) continue;

            for(int l = 0; l < 3; l++) {

                V = BarycenterPoint(l == 0 ? 1: 0, l == 1? 1: 0, l == 2? 1: 0);
                opV = oppositeToVertexPoint(p, V);
                Point s0 = getSurfacePoint(V),
                      s1 = getBoundaryHermiteApproximatePoint(opV);
                //ExMath::consoleLogSurfacePoint(opV, s1);

                Vector T01 = tangentInPoint(V, opV);
                Vector T10 = tangentInPoint(opV, V);

                double coef = V.x() * p.y() * p.y() * p.z() * p.z()+
                              V.y() * p.x() * p.x() * p.z() * p.z()+
                              V.z() * p.x() * p.x() * p.y() * p.y();
                double* H = ExMath::getHermiteCoefficients((p*V).length());
                double* nH = ExMath::getHermiteCoefficients((p*opV).length());

                sp += (coef/divCoef) * (s0 * H[0] + T01 * H[1] + T10 * nH[2] + s1 * nH[3]);

                delete [] H;
                delete [] nH;
            }
            points[p] = sp;
            ExMath::consoleLogSurfacePoint(p, sp);
        }
    }
}

Point CoonsTriangularSurface::
    getBoundaryBernesteinApproximatePoint(BarycenterPoint& b) {

    if (!points[b].isNull()) return points[b];

    int i = (b.x() != 0) ? 1: 0,
        j = (b.y() != 0 && i == 0)? 1: 0,
        k = 0;

    BarycenterPoint V(i, j, k);

    i = 0,
    j = (b.y() != 0 && j == 0)? 1: 0,
    k = !j? 1: 0;

    BarycenterPoint opV(i, j, k);

    Point s0 = getSurfacePoint(V),
          s1 = getSurfacePoint(opV);

    Vector T01 = tangentInPoint(V, opV);
    Vector T10 = tangentInPoint(opV, V);

    QPair<double, double> lmd = lambda(s0, s1, T01, T10);
    Point surfasePoint;

    double x = Vector::dotProduct(b, V);
    double* B3 = ExMath::bernstein(3, 1 - x);
    surfasePoint = s0 * B3[0] + (s0 + lmd.first * T01) * B3[1] +
            (s1 + lmd.second * T10) * B3[2] + s1 * B3[3];

    delete [] B3;
    points[b] = surfasePoint;
    return surfasePoint;
}

Point CoonsTriangularSurface::
    getBoundaryHermiteApproximatePoint(BarycenterPoint& p) {

    if (!points[p].isNull()) return points[p];

    int i = (p.x() != 0) ? 1: 0,
        j = (p.y() != 0 && i == 0)? 1: 0,
        k = 0;

    BarycenterPoint V(i, j, k);

    i = 0,
    j = (p.y() != 0 && j == 0)? 1: 0,
    k = !j? 1: 0;

    BarycenterPoint opV(i, j, k);

    Point s0 = getSurfacePoint(V),
          s1 = getSurfacePoint(opV);

    Vector T01 = tangentInPoint(V, opV);
    Vector T10 = tangentInPoint(opV, V);

    Point surfasePoint;

    double* H = ExMath::getHermiteCoefficients((p*V).length());
    double* nH = ExMath::getHermiteCoefficients((p*opV).length());
    surfasePoint = s0 * H[0] + T01 * H[1] + T10 * H[2] + s1 * H[3];

    delete [] H;
    delete [] nH;
    points[p] = surfasePoint;
    ExMath::consoleLogSurfacePoint(p, surfasePoint);
    return surfasePoint;
}

QPair<double, double> CoonsTriangularSurface::
    lambda(Point& s0, Point& s1, Vector& T01, Vector& T10) {

    QPair<double, double> lmd;
    double TT = Vector::dotProduct(T01, T10);
    double coef = 15376. - 3481. * TT*TT;
    lmd.first = Vector::dotProduct(s1 - s0, 7564. * T01 - 3599. * TT * T10) / coef;
    lmd.second = Vector::dotProduct(s0 - s1, 7564. * T10 - 3599. * TT * T01) / coef;
    return lmd;
}

CoonsTriangularSurface &CoonsTriangularSurface::
    operator=(const CoonsTriangularSurface &surface) {

    points = QHash<BarycenterPoint, Point>(surface.getPoints());
    vertexes = QVector<Point> (surface.vertexes);
    normals = QVector<Vector> (surface.normals);
}

Point& CoonsTriangularSurface::
    getSurfacePoint(BarycenterPoint key) {

    if (!points[key].isNull()) {
       return points[key];
    }

    return points[key];
}

Vector CoonsTriangularSurface::
    pointNormalVector(const BarycenterPoint& p) {

    return (p.x() * normals[0] + p.y() * normals[1] + p.z() * normals[2]).normalized();
}

Vector CoonsTriangularSurface::
    tangentInPoint(const BarycenterPoint& bV, const BarycenterPoint& bOpV) {

    Vector N = -pointNormalVector(bV);
    Point V = getSurfacePoint(bV);
    Point opV = getSurfacePoint(bOpV);
    Vector tangent = Vector::crossProduct(
                         N,
                         Vector::crossProduct(opV - V, N).normalized()
                      );
    tangent = tangent.normalized();
    tangents[QPair<Point, Point>(V, opV)] = tangent;
    return tangent;
}

QHash<BarycenterPoint, Point> CoonsTriangularSurface::
    getPoints() const {
    return points;
}

void CoonsTriangularSurface::
    setPoints(const QHash<BarycenterPoint, Point> &value) {
    points = value;
}

void CoonsTriangularSurface::
    drawTriangularNet() const {

    glEnable(GL_LINE_SMOOTH);

    glLineWidth(1.1);

    double delta = stepLength;
    double one = 1. + CoonsPatches::PRECISION;

    glBegin(GL_LINES);

    for(double i = 0. ; i <= one; i += delta) {
        for(double j = 0., k; j <= one - i; j += delta) {
            k = 1. - j - i;

            BarycenterPoint B(i, j, k), B2;
            Point P1 = points[B], P2;
            double i2, j2, k2;

                if (i < 1 && k > 0) {
                    i2 = i + delta;
                    j2 = j;
                    k2 = std::max(0., 1. - i2 - j2);
                    B2 = BarycenterPoint(i2, j2, k2);
                    P2 = points[B2];

                    if ( !P2.isNull() ) {
                            glVertex3f(P1.x(), P1.y(), P1.z());
                            glVertex3f(P2.x(), P2.y(), P2.z());
                    } else {
                        ExMath::consoleLogVector3D(B2);
                    }
                }

                if (i > 0 && j < 1) {
                    i2 = i - delta;
                    j2 = j + delta;
                    k2 = std::max(0., 1. - i2 - j2);
                    B2 = BarycenterPoint(i2, j2, k2);
                    P2 = points[B2];

                    if (!P2.isNull()) {
                        glVertex3f(P1.x(), P1.y(), P1.z());
                        glVertex3f(P2.x(), P2.y(), P2.z());
                    } else {
                        ExMath::consoleLogVector3D(B2);
                    }
                }

                if (j < 1 && k > 0) {
                    i2 = i;
                    j2 = j + delta;
                    k2 = std::max(0., 1. - i2 - j2);
                    B2 = BarycenterPoint(i2, j2, k2);
                    P2 = points[B2];

                    if (!P2.isNull()) {
                            glVertex3f(P1.x(), P1.y(), P1.z());
                            glVertex3f(P2.x(), P2.y(), P2.z());
                    } else {
                        ExMath::consoleLogVector3D(B2);
                    }
                }
        }
    }
    glEnd();
}

void CoonsTriangularSurface::
    draw() const {

    double delta = stepLength;
    double one = 1. + CoonsPatches::PRECISION;
    double i2, j2, k2;
    double i3, j3, k3;
    double i4, j4, k4;

    glBegin(GL_TRIANGLES);

    int l = 0;
    for(double i = 0. ; i + delta <= one; i += delta, l++) {
        for(double j = 0., k; j + delta <= one - i; j += delta, l++) {
            k = 1. - j - i;

            BarycenterPoint B1(i, j, k);
            Point P1 = points[B1];

            i2 = i;
            j2 = j + delta;
            k2 = std::max(0., 1. - i2 - j2);
            BarycenterPoint B2(i2, j2, k2);
            Point P2 = points[B2];

            i3 = i + delta;
            j3 = j;
            k3 = std::max(0., 1. - i3 - j3);
            BarycenterPoint B3(i3, j3, k3);
            Point P3 = points[B3];

            glVertex3f(P3.x(), P3.y(), P3.z());
            glVertex3f(P2.x(), P2.y(), P2.z());
            glVertex3f(P1.x(), P1.y(), P1.z());

            if (j2 <= one - i3) {
                i4 = i3;
                j4 = j2;
                k4 = std::max(0., 1. - i4 - j4);
                BarycenterPoint B4(i4, j4, k4);
                Point P4 = points[B4];

                if (!P4.isNull()) {
                    glVertex3f(P2.x(), P2.y(), P2.z());
                    glVertex3f(P3.x(), P3.y(), P3.z());
                    glVertex3f(P4.x(), P4.y(), P4.z());
                } else {
                    qDebug() << B4;
                }
            }
        }
    }
    glEnd();
}
