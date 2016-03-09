#include "delaunaytriangulation.h"
#include "point_position_to_triangle.h"
#include <iostream>

DelaunayTriangulation::
    DelaunayTriangulation(){}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<QPointF> &trs): points(trs) {}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<TriangularUnit> &trs): triangles(trs){}


void DelaunayTriangulation::build() {
    /**
    1. Вначале производится локализация точки, т.е. находится треугольник (построен-
    ный ранее), в который попадает очередная точка; либо, если точка не попадает внутрь
    триангуляции, то находится треугольник на границе триангуляции, ближайший к очеред-
    ной точке.
    2. Если точка попала внутрь какого-нибудь треугольника, он разбивается на три но-
    вых; иначе, при попадании точки вне триангуляции, строится один или более треугольни-
    ков. Затем проводятся локальные проверки вновь полученных треугольников на соответ-
    ствие условию Делоне.*/

    QVectorIterator<QPointF> it(points);

    while(it.hasNext()) {
        QPointF pointToAdd = it.next();

        // Zero step
        if (pointToAdd == *points.begin()) {
            TriangularUnit firstTriangle(pointToAdd, it.next(), it.next());
            appendTriangles(firstTriangle);
            appendBoundaryEdges(firstTriangle.getEdges());
            continue;
        }

        QVector<TriangularUnit> newTriangles;

        int relation = definePointPositionToTriangulation(pointToAdd);
        if (relation == INSIDE) {

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::INSIDE");
            TriangularUnit nearestTriangle = getClosestTriangleToPoint(pointToAdd);
            newTriangles = replaceTriangleWithThreeSplittedParts(nearestTriangle, pointToAdd);

        } else if (relation == ON_EDGE) {

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::ON_EDGE");
            TriangularUnit nearestTriangle = getClosestTriangleToPoint(pointToAdd);
            newTriangles = replaceTriangleWithTwoSplittedParts(nearestTriangle, pointToAdd);

        } else { // relation == ExMath::POINT_POSITION_TO_TRIANGLE::OUTSIDE

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::OUTSIDE");
            TriangularUnit nearestTriangle = getClosestBoundaryTriangleToPoint(pointToAdd);
            newTriangles = createNewBoundaryTriangles(nearestTriangle, pointToAdd);
        }

        checkDelaunayConditionLocaly(newTriangles);
    }
}
QVector<Edge> DelaunayTriangulation::getEdges() const {
    return edges;
}

bool DelaunayTriangulation::
    isBoundary(Edge& edge) {

    return boundaryEdges.contains(edge);
}

bool DelaunayTriangulation::
    isBoundary(TriangularUnit& tr) {

    for (int i = 0; i < tr.getEdges().length(); i++) {
        Edge edge = tr.getEdges().at(i);
        if (boundaryEdges.contains(edge)) {
            return true;
        }
    }
    return false;
}

TriangularUnit DelaunayTriangulation::
    getClosestBoundaryTriangleToPoint(QPointF &point) {

    double minDistantToTriangle = ExMath::DOUBLE_MAX;
    TriangularUnit closestTriangular, current;

    QVectorIterator<TriangularUnit> it(triangles);

    while(it.hasNext()) {
        current = it.next();
        if (!isBoundary(current)) continue;

        QPointF inscribedCircleCenter = current.getInscribedCircleCenter();
        inscribedCircleCenters.append(inscribedCircleCenter);

        double distantToPoint = ExMath::distantBeetweenPoints( inscribedCircleCenter, point );
        minDistantToTriangle = std::min( minDistantToTriangle, distantToPoint );

        if ( distantToPoint == minDistantToTriangle ) {
            closestTriangular = current;
        }
    }

    return closestTriangular;
}

TriangularUnit DelaunayTriangulation::
    getClosestTriangleToPoint(QPointF &point) {

    double minDistantToTriangle = ExMath::DOUBLE_MAX;
    TriangularUnit closestTriangular, currentTriangle;

    QVectorIterator<TriangularUnit> it(triangles);

    while(it.hasNext()) {
        currentTriangle = it.next();
        if (currentTriangle.isPointInsideTriangle(point)) {
            return currentTriangle;
        }

        QPointF inscribedCircleCenter = currentTriangle.getInscribedCircleCenter();
        inscribedCircleCenters.append(inscribedCircleCenter);

        double distantToPoint = ExMath::distantBeetweenPoints( inscribedCircleCenter, point );
        minDistantToTriangle = std::min( minDistantToTriangle, distantToPoint );

        if ( distantToPoint == minDistantToTriangle ) {
            closestTriangular = currentTriangle;
        }
    }
    return closestTriangular;
}

int DelaunayTriangulation::
    definePointPositionToTriangle(QPointF &point, TriangularUnit &triangle) {


    QVector<QPointF> vertexes = triangle.getVertexes();
    return ExMath::pointPositionToTriangle(vertexes.at(0), vertexes.at(1), vertexes.at(2), point);
}

int DelaunayTriangulation::
    definePointPositionToTriangulation(QPointF &point) {

    QVectorIterator<TriangularUnit> it(triangles);
    while(it.hasNext()) {
        TriangularUnit triangle = it.next();
        QVector<QPointF> vertexes = triangle.getVertexes();
        int positionToTriangle = ExMath::pointPositionToTriangle(vertexes.at(0), vertexes.at(1), vertexes.at(2), point);
        if (positionToTriangle != OUTSIDE) {
            return positionToTriangle;
        }
    }

    return OUTSIDE;
}

QVector<TriangularUnit> DelaunayTriangulation::
    replaceTriangleWithThreeSplittedParts(TriangularUnit &triangle, QPointF &splittingPoint) {

    QVector<TriangularUnit> newTriangles;
    QVector<Edge> edges = triangle.getEdges();
    int len = edges.length();
    for (int i = 0; i < len; i++) {
        newTriangles.append(TriangularUnit( edges.at(i), splittingPoint ));
    }
    triangles.removeOne(triangle);
    appendTriangles(newTriangles);

    return newTriangles;
}

QVector<TriangularUnit> DelaunayTriangulation::
    replaceTriangleWithTwoSplittedParts(TriangularUnit &triangle, QPointF &splittingPoint) {

    QVector<TriangularUnit> newTriangles;
    Edge splittedEdge = triangle.getTriangleEdgeThatContainsPoint(splittingPoint);

    int i = 0;
    Edge remaindEdge1 = splittedEdge != triangle.getEdges().at(i)?
                                     triangle.getEdges().at(i):         // at(0) i = 0
                                     triangle.getEdges().at(++i);       // at(1) i = 1

    Edge remaindEdge2 = splittedEdge != triangle.getEdges().at(++i)?    // i = 1 || 2
                                     triangle.getEdges().at(i):         // i = 1 || 2
                                     triangle.getEdges().at(++i);       // i = 2 || 3

    Edge splittedEdgePart1(splittedEdge.getStartPoint(), splittingPoint);
    Edge splittedEdgePart2(splittingPoint, splittedEdge.getEndPoint());

    if (splittedEdge.getIsBoundary()) {
        splittedEdgePart1.setIsBoundary(true);
        splittedEdgePart2.setIsBoundary(true);
        boundaryEdges.removeOne(splittedEdge);
        boundaryEdges.append(splittedEdgePart1);
        boundaryEdges.append(splittedEdgePart2);
    }

    if (splittedEdgePart1.areConnected(remaindEdge1)) {
        newTriangles.append(TriangularUnit(splittedEdgePart1, remaindEdge1));
        newTriangles.append(TriangularUnit(splittedEdgePart2, remaindEdge2));
    } else {
        newTriangles.append(TriangularUnit(splittedEdgePart2, remaindEdge1));
        newTriangles.append(TriangularUnit(splittedEdgePart1, remaindEdge2));
    }

    triangles.removeOne(triangle);
    appendTriangles(newTriangles);

    return newTriangles;
}

QVector<TriangularUnit> DelaunayTriangulation::
    createNewBoundaryTriangles(TriangularUnit &triangle, QPointF &outerPoint) {

    QVector<TriangularUnit> newTriangles;
    Edge newTriangleEdge = triangle.getClosestBoundaryEdgeToPoint(outerPoint);
    Edge newEdge1(newTriangleEdge.getStartPoint(), outerPoint);
    Edge newEdge2(newTriangleEdge.getEndPoint(), outerPoint);

    newTriangleEdge.setIsBoundary(false);
    TriangularUnit newTriangle(newTriangleEdge, newEdge1, newEdge2);

    appendBoundaryEdges(newEdge1, newEdge2);
    appendEdges(newTriangle.getEdges());

    newTriangles.append(newTriangle);
    appendTriangles(newTriangles);

    return newTriangles;
}

void DelaunayTriangulation::appendTriangles(QVector<TriangularUnit> &newTriangles) {
    for(int i = 0; i < newTriangles.length(); i++) {
        appendTriangles(newTriangles[i]);
    }
}

void DelaunayTriangulation::
    appendEdges(QVector<Edge> &newEdges) {

    for(int i = 0; i < newEdges.length(); i++) {
        edges.append(newEdges[i]);
    }
}

void DelaunayTriangulation::
    appendBoundaryEdges(Edge newEdge1, Edge newEdge2) {

    boundaryEdges.append(newEdge1);
    boundaryEdges.append(newEdge2);
}

void DelaunayTriangulation::
    appendBoundaryEdges(QVector<Edge> &newEdges) {

    for (int i = 0; i < newEdges.length(); i++) {
        newEdges[i].setIsBoundary(true);
        boundaryEdges.append(newEdges[i]);
    }
}

void DelaunayTriangulation::
    appendTriangles(TriangularUnit& triangle) {

    triangles.append(triangle);
    appendEdges(triangle.getEdges());
}

void DelaunayTriangulation::
    checkDelaunayConditionLocaly(QVector<TriangularUnit>& triangles) {

    //QVectorIterator<QPointF> it( triangles );
    //while (it.hasNext()) {
    //    TriangularUnit i
    //}
}

void DelaunayTriangulation::
    clear() {

    ExMath::consoleLog("_______________New triangulation_______________");
    triangles.clear();
    edges.clear();
    points.clear();
    inscribedCircleCenters.clear();
}

void DelaunayTriangulation::setPoints(const QVector<QPointF> &value) {
    clear();
    points = value;
}

