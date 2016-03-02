#include "triangularunit.h"

TriangularUnit::
    TriangularUnit() {

    //create empty triangular
    vertexes = new QPointF[3];
    edges = new Edge[3];
    isEmpty = true;
}

TriangularUnit::
    TriangularUnit(QPointF* trVertexes) {

    neighborTriangulars = new TriangularUnit[3];
    vertexes = trVertexes;

    edges = new Edge[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = Edge(trVertexes[i], trVertexes[(i + 1) % 3]);
    }

    /*for (int i = 0; i < 3; i++) {
        vertexes[i] = trVertexes[i];
    }*/

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(QPointF& p1, QPointF& p2, QPointF& p3) {

    neighborTriangulars = new TriangularUnit[3];
    vertexes[0] = p1;
    vertexes[1] = p2;
    vertexes[2] = p3;

    edges = new Edge[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = Edge(vertexes[i], vertexes[(i + 1) % 3]);
    }

    /*for (int i = 0; i < 3; i++) {
        vertexes[i] = trVertexes[i];
    }*/

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(QPointF* trVertexes, TriangularUnit* triangulars) {

    neighborTriangulars = triangulars;
    vertexes = trVertexes;

    edges = new Edge[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = Edge(trVertexes[i], trVertexes[(i + 1) % 3]);
    }

    /*for (int i = 0; i < 3; i++) {
        vertexes[i] = trVertexes[i];
    }*/

    isEmpty = false;
}

bool TriangularUnit::
    getIsEmpty() {

    return isEmpty;
}

Edge* TriangularUnit::
    getEdges() {

    return edges;
}

/*TriangularUnit::
    ~TriangularUnit() {

    delete[] neiborTriangulars;
    delete[] vertexes;
    delete[] edges;
}*/

