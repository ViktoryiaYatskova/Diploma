#include "delaunaytriangulation.h"

DelaunayTriangulation::
    DelaunayTriangulation(){}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<QPointF> &trs): points(trs) {}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<TriangularUnit> &trs): triangles(trs){}

DelaunayTriangulation::
    DelaunayTriangulation(QPointF* triangularPoints, int length) {

    //points = QVector<QPointF>(triangularPoints, length);
}

void DelaunayTriangulation::
    build (DelaunayTriangulation& trianqular) {
}
