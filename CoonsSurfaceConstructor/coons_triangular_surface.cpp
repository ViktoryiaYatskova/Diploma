#include "exMath.h"
#include "coons_triangular_surface.h"
#include <cstring>

CoonsTriangularSurface::
    CoonsTriangularSurface():stepNumber(100){}

CoonsTriangularSurface::
    CoonsTriangularSurface(QVector<Point>& Vs, Vector& N1, Vector& N2, Vector& N3, const int st):

    vertexes(Vs),
    stepNumber(st) {


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
    b1 = 1 - b2 - b3;

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

    double z = p.x() * p.x() + p.y() * p.y();
    p.setZ(z);
    return p;
}

void CoonsTriangularSurface::
    buildBernesteinApproximateSurface() {

    double delta = 1.0 / stepNumber;

    // define boundary patch points
    BarycenterPoint opV(1, 0, 0), V;
    for(int l = 2; l < 5; l++) {
        V = opV;
        int i = l%3 == 1? 1: 0, j = l%3 == 2? 1: 0, k = l%3 == 0? 1: 0;
        opV = BarycenterPoint(i, j, k);

        Point s0 = getSurfacePoint(V),
              s1 = getSurfacePoint(opV);

        Vector T01 = tangentInPoint(V, opV);
        Vector T10 = tangentInPoint(opV, V);

        QPair<double, double> lmd = lambda(s0, s1, T01, T10);

        for (double x = delta; x < 1; x += delta) {
            double* B3 = ExMath::bernstein(3, x);
            BarycenterPoint boundaryPoint = V*x + (1. - x)*opV;
            Point point = s0 * B3[0] + (s0 + lmd.first * T01) * B3[1] +
                          (s1 + lmd.second * T10) * B3[2] + s1 * B3[3];
            //points[boundaryPoint] = point;

            delete [] B3;
        }
    }

    // define inner points
    for(double i = delta; i < 1; i += delta) {
        for(double j = delta; j < 1; j += delta) {
            BarycenterPoint p = BarycenterPoint(i, j, 1 - j - i);
            double divCoef = p.x()*p.x() + p.y()*p.y() + p.z()*p.z();

            if (!points[p].isNull()) return;

            points[p] = Point();
            for(int l = 1; l < 4; l++) {
                V = BarycenterPoint((int)l%3 == 1, (int)l%3 == 2, (int)l%3 == 0);
                opV = oppositeToVertexPoint(p, V);
                Point s0 = getSurfacePoint(V),
                      s1 = getSurfacePoint(opV);

                s1 = s1.isNull() ? getBoundaryBernesteinApproximatePoint(opV) : s1;
                ExMath::consoleLogSurfacePoint(opV, s1);

                Vector T01 = tangentInPoint(V, opV);
                Vector T10 = tangentInPoint(opV, V);

                QPair<double, double> lmd = lambda(s0, s1, T01, T10);
                double coef = V.x() * p.x()* p.x() + V.y() * p.y()*p.y() + V.z()* p.z()*p.z();
                double* B3 = ExMath::bernstein(3, 1. - (p*V).length());

                points[p] += coef * (s0 * B3[0] + (s0 + lmd.first * T01) * B3[1] +
                    (s1 + lmd.second * T10) * B3[2] + s1 * B3[3]);

                delete [] B3;
            }

            points[p] /= divCoef;
        }
    }
}

void CoonsTriangularSurface::
    buildHermiteApproximateSurface() {

    double delta = 1.0 / stepNumber;

    // define inner points
    for(double i = delta; i < 1; i += delta) {
        for(double j = delta; j < 1; j += delta) {
            BarycenterPoint p(i, j, 1 - j - i);
            double divCoef = p.x()*p.x() + p.y()*p.y() + p.z()*p.z();

            if (!points[p].isNull()) return;

            points[p] = Point();
            for(int l = 1; l < 4; l++) {
                V = BarycenterPoint((int)l%3 == 1, (int)l%3 == 2, (int)l%3 == 0);
                opV = oppositeToVertexPoint(p, V);
                Point s0 = getSurfacePoint(V),
                      s1 = getSurfacePoint(opV);

                s1 = s1.isNull() ? getBoundaryHermiteApproximatePoint(opV) : s1;
                ExMath::consoleLogSurfacePoint(opV, s1);

                Vector T01 = tangentInPoint(V, opV);
                Vector T10 = tangentInPoint(opV, V);

                double coef = V.x() * p.y()* p.y() * p.z()* p.z()+
                              V.y() * p.x()* p.x() * p.z()* p.z()+
                              V.z() * p.x()* p.x() * p.y()* p.y();
                double* H = ExMath::getHermiteCoefficients((p*V).length());
                double* nH = ExMath::getHermiteCoefficients(1. - (p*V).length());

                points[p] += coef * (s0 * H[0] + T01 * H[1] + T10 * nH[2] + s1 * nH[3]);

                delete [] H;
                delete [] nH;
            }

            points[p] /= divCoef;
        }
    }
}

Point CoonsTriangularSurface::
    getBoundaryBernesteinApproximatePoint(BarycenterPoint& b){

    int i = (b.x() != 0),
        j = (b.y() != 0 && i == 0),
        k = 0;

    BarycenterPoint V(i, j, k);

    i = 0,
    j = (b.y() != 0 && j == 0),
    k = !j;

    BarycenterPoint opV(i, j, k);

    Point s0 = getSurfacePoint(V),
          s1 = getSurfacePoint(opV);

    Vector T01 = tangentInPoint(V, opV);
    Vector T10 = tangentInPoint(opV, V);

    QPair<double, double> lmd = lambda(s0, s1, T01, T10);
    Point surfasePoint;

    double x = b.x() || b.y();
    double* B3 = ExMath::bernstein(3, x);
    surfasePoint = s0 * B3[0] + (s0 + lmd.first * T01) * B3[1] +
            (s1 + lmd.second * T10) * B3[2] + s1 * B3[3];

    delete [] B3;
    points[b] = surfasePoint;
    return surfasePoint;
}

Point CoonsTriangularSurface::
    getBoundaryHermiteApproximatePoint(BarycenterPoint& b){

    int i = (b.x() != 0),
        j = (b.y() != 0 && i == 0),
        k = 0;

    BarycenterPoint V(i, j, k);

    i = 0,
    j = (b.y() != 0 && j == 0),
    k = !j;

    BarycenterPoint opV(i, j, k);

    Point s0 = getSurfacePoint(V),
          s1 = getSurfacePoint(opV);

    Vector T01 = tangentInPoint(V, opV);
    Vector T10 = tangentInPoint(opV, V);

    Point surfasePoint;

    double* H = ExMath::getHermiteCoefficients((p*V).length());
    double* nH = ExMath::getHermiteCoefficients(1. - (p*V).length());
    surfasePoint = s0 * H[0] + T01 * H[1] + T10 * nH[2] + s1 * nH[3];

    delete [] H;
    delete [] nH;
    points[b] = surfasePoint;
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
    operator=(const CoonsTriangularSurface &surface){

    points = QHash<BarycenterPoint, Point>(surface.getPoints());
    vertexes = QVector<Point> (surface.vertexes);
    normals = QVector<Vector> (surface.normals);
}

Point& CoonsTriangularSurface::
    getSurfacePoint(BarycenterPoint key) {

    /*if (!points[key].isNull()) {
       return points[key];
    }*/

    if (key.x() == 1. || key.y() == 1. || key.z() == 1.) {
        points[key] = barycentricToCartesian(key);
        return points[key];
    }

    return points[key];

    // error! not calced yet
    // TODO: move calculation here
    ExMath::consoleLog("Not found!");

    Point closest;
    double closestDistance;

    QHashIterator<BarycenterPoint, Point> it(points);
    while(it.hasNext()) {
        it.next();
        double distance = ExMath::manhattamDistanse(it.key(), key);
        if(distance < closestDistance || closest.isNull()) {
            closestDistance = distance;
            closest = it.value();
        }
    }

    ExMath::consoleLogVector3D(key);
    ExMath::consoleLogVector3D(closest);
    return closest;
}

Vector CoonsTriangularSurface::
    pointNormalVector(const BarycenterPoint& p) {

    return p.x() * normals[0] + p.y() * normals[1] + p.z() * normals[2];
}

Vector CoonsTriangularSurface::
    tangentInPoint(const BarycenterPoint& bV, const BarycenterPoint& bOpV) {

    Vector N = pointNormalVector(bV);
    Point V = barycentricToCartesian(bV);
    Point opV = barycentricToCartesian(bOpV);
    Vector tangent = -N * (opV - V) * -N;
    return tangent.normalized();
}

QHash<BarycenterPoint, Point> CoonsTriangularSurface::
    getPoints() const {
    return points;
}

void CoonsTriangularSurface::
    setPoints(const QHash<BarycenterPoint, Point> &value) {
    points = value;
}
