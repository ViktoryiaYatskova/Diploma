#include "surface.h"

Surface::Surface(DelaunayTriangulation& delTriangulation):
    triangulation(delTriangulation), STEP_NUMBER(100) {

    QVectorIterator<TriangularUnit> it(triangulation.getTriangles());
    while(it.hasNext()) {
        TriangularUnit triangle = it.next();
        QVector<Point> triangleVertexes = triangle.getVertexes();
        Vector normal1 = getVertexNormal(triangleVertexes.at(0)),
               normal2 = getVertexNormal(triangleVertexes.at(1)),
               normal3 = getVertexNormal(triangleVertexes.at(2));
        CoonsTriangularSurface patch(triangleVertexes, normal1, normal2, normal3, STEP_NUMBER);
        append(patch);
    }
}

void Surface::build(){
    const QVector< TriangularUnit > &triangles(triangulation.getTriangles());
    for (int i = 0; i < triangles.length(); i++) {
        QVector<Point> vertexes = triangles.at(i).getVertexes();
        Vector vertexNormal1 = getVertexNormal(vertexes[0]);
        Vector vertexNormal2 = getVertexNormal(vertexes[1]);
        Vector vertexNormal3 = getVertexNormal(vertexes[2]);
        CoonsTriangularSurface trPatch(vertexes, vertexNormal1, vertexNormal2, vertexNormal3, STEP_NUMBER);
        trPatch.buildHermiteApproximateSurface();
        append(trPatch);
    }
}

Vector Surface::
    getVertexNormal(const Point& vertex) {

    Vector normal;
    QSet<TriangularUnit> adjacentTriangulars = vertexTriangleNeighborhood[vertex];
    if (adjacentTriangulars.isEmpty()) {
        adjacentTriangulars = triangulation.getVertexAdjacentTriangles(vertex);
        vertexTriangleNeighborhood[vertex] = adjacentTriangulars;
    }

    QSetIterator <TriangularUnit> it(adjacentTriangulars);
    while (it.hasNext()) {
        TriangularUnit tr = it.next();
        double a = std::asin(tr.getSinAngleBetweenEdges(vertex));
        normal += a * tr.normalVector();
    }
    return normal;
}



