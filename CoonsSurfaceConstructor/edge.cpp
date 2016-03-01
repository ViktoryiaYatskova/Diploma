#include "edge.h"

int Edge::dimension = Point2D::DIMENSION;

Edge::Edge(const Point2D& p1, const Point2D& p2):point1(p1), point2(p2) {}

Edge::Edge() {}

Vector <Edge>& Edge::createAllPossibleEdgesThatNotCrossOthers(Vector < Point2D >& points, QVector <Edge>& edgesNotToCross) {
    Vector <Edge> edges;

    for(int i = 0; i < points.length(); i++) {
        Point2D currentPoint = points.at(i);

        for(int j = 0; j < points.length(); j++) {
            if (i == j) continue;

            Point2D anotherPoint = points.at(j);
            Edge newEdge(currentPoint, anotherPoint);

            if (!newEdge.isCrossedBySomeEdge(edgesNotToCross)) {
                edges.append(newEdge);
            }
        }
    }
    return edges;
}

bool Edge::isCrossedBySomeEdge(Vector <Edge>& edges) {
    for (int i = 0; i < edges.length(); i++) {
        if (this->isCrossedByEdge(edges[i])) {
            return true;
        }
    }
    return false;
}

bool Edge::isCrossedByEdge(Edge& edge) {

    float x1 = edge.point1.x(), x2 = edge.point2.x(),
            x3 = this->point1.x(), x4 = this->point2.x(),

          y1 = edge.point1.y(), y2 = edge.point2.y(),
            y3 = this->point1.y(), y4 = this->point2.y();

    double** A = new double*[dimension];
    A[0] = new double[dimension];
    A[1] = new double[dimension];

    A[0][0] = x1 - x2; A[0][1] = x3 - x4;
    A[1][0] = y1 - y2; A[1][1] = y3 - y4;

    double* b = new double[dimension];
    b[0] = x3 - x2;
    b[1] = y3 - y2;

    double* x = ExMath.solveMatrixEquantion(A, b, dimension);
    bool areCrossed = 0 <= x[0] && x[0] <= 1 &&
                      0 <= x[1] && x[1] <= 1;

    for (int i = 0; i < 2; i++) {
        delete[] A[i];
    }

    delete[] A;
    delete[] b;
    delete[] x;

    return areCrossed;
}
