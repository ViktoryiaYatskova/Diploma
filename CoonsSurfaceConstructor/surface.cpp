#include "surface.h"
#include "config.h"

Surface::Surface(DelaunayTriangulation& delTriangulation):
    triangulation(delTriangulation), STEP_LENGTH(CoonsPatches::STEP) {

    QVectorIterator<TriangularUnit> it(triangulation.getTriangles());
    while(it.hasNext()) {
        TriangularUnit triangle = it.next();
        QVector<Point> triangleVertexes = triangle.getVertexes();
        Vector normal1 = getVertexNormal(triangleVertexes.at(0)),
               normal2 = getVertexNormal(triangleVertexes.at(1)),
               normal3 = getVertexNormal(triangleVertexes.at(2));
        CoonsTriangularSurface patch(triangleVertexes, normal1, normal2, normal3, STEP_LENGTH);
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
        CoonsTriangularSurface trPatch(vertexes, vertexNormal1, vertexNormal2, vertexNormal3, STEP_LENGTH);

        if (CoonsPatches::PATCH_TYPE == CoonsPatches::BERNSTEIN) {
            trPatch.buildBernesteinApproximateSurface();
        } else if (CoonsPatches::PATCH_TYPE == CoonsPatches::HERMITE) {
            trPatch.buildHermiteApproximateSurface();
        }

        append(trPatch);
    }
}

void Surface::clear(){
    QVector<CoonsTriangularSurface>::clear();
    triangulation.clear();
    vertexNormals.clear();
    vertexTriangleNeighborhood.clear();
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
    return normal.normalized();
}

void Surface::drawTrianglesNormals() {
    QVectorIterator <TriangularUnit> it(triangulation.getTriangles());

    glBegin(GL_LINES);
        while (it.hasNext()) {
            TriangularUnit tr = it.next();
            Point P1 = tr.getInscribedCircleCenter();
            Point P2 = P1 + tr.normalVector();

            glVertex3f(P1.x(), P1.y(), P1.z());
            glVertex3f(P2.x(), P2.y(), P2.z());
        }
    glEnd();
}



