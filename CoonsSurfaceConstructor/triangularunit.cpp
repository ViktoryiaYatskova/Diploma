#include "triangularunit.h"

TriangularUnit::
    ~TriangularUnit(){

    delete[] neiborTriangulars;
    delete[] vertexes;
    delete[] edges;
}

TriangularUnit::
    TriangularUnit() {

    //create empty triangular
    vertexes = new Point2D[3];
    edges = new Edges[3];
    isEmpty = true;
}

TriangularUnit::
    TriangularUnit(Point2D* trVertexes) {

    neiborTriangulars = new TriangularUnit[3];
    vertexes = trVertexes;

    edges = new Edges[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = new Edge(trVertexes[i], trVertexes[(i + 1) % 3]);
    }

    /*for (int i = 0; i < 3; i++) {
        vertexes[i] = trVertexes[i];
    }*/

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(Point2D& p1, Point2D& p2, Point2D& p3) {

    neiborTriangulars = new TriangularUnit[3];
    vertexes[0] = p1;
    vertexes[1] = p2;
    vertexes[2] = p3;

    edges = new Edges[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = new Edge(vertexes[i], vertexes[(i + 1) % 3]);
    }

    /*for (int i = 0; i < 3; i++) {
        vertexes[i] = trVertexes[i];
    }*/

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(Point2D* trVertexes, TriangularUnit* triangulars) {

    neighborTriangulars = triangulars;
    vertexes = trVertexes;

    edges = new Edges[3];
    for (int i = 0; i < 3; i++) {
        edges[i] = new Edge(trVertexes[i], trVertexes[(i + 1) % 3]);
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

