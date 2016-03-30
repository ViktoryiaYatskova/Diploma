#include "delaunaytriangulation.h"
#include "point_position_to_triangle.h"
#include <iostream>

DelaunayTriangulation::
    DelaunayTriangulation(){}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<QPointF> &trs): points(trs) {}

DelaunayTriangulation::
    DelaunayTriangulation(QVector<TriangularUnit> &trs): triangles(trs){}


void DelaunayTriangulation::
    createFirstTriangle(QPointF point1, QPointF point2, QPointF point3) {

    TriangularUnit firstTriangle(point1, point2, point3);
    appendTriangles(firstTriangle);
    convexHull.append(point1);
    convexHull.append(point2);
    convexHull.append(point3);
}

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
            createFirstTriangle(it.next(), it.next(), pointToAdd);
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
            newTriangles = createNewBoundaryTriangles(pointToAdd);
        }

        checkDelaunayConditionLocaly(newTriangles);
    }
}
QVector<Edge> DelaunayTriangulation::
    getEdges() const {

    return edges;
}

TriangularUnit DelaunayTriangulation::
    getClosestTriangleToPoint(QPointF &point) {

    double minDistantToTriangle = ExMath::DOUBLE_MAX;
    TriangularUnit closestTriangular, currentTriangle;

    QVectorIterator<TriangularUnit> it(triangles);

    while(it.hasNext()) {
        currentTriangle = it.next();
        if (currentTriangle.isPointInsideTriangle(point)) {
            ExMath::consoleLog("inside the triangle");
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
        if (vertexes.length() < 3) {
            ExMath::consoleLog("Error triangle");
        }
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

    for (int i = 0, len = edges.length(); i < len; i++) {
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

    if (convexHull.contains(splittedEdge.getStartPoint()) &&
        convexHull.contains(splittedEdge.getEndPoint()) ) {

        convexHull.insert(convexHull.indexOf(splittedEdge.getStartPoint()), splittingPoint);
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
    createNewBoundaryTriangles(QPointF &outerPoint) {

    QVector<TriangularUnit> newTriangles;

    if (!convexHull.length()) return newTriangles;

    int closestPointIndex = 0;
    double minDistant = ExMath::DOUBLE_MAX;

    for ( int i = 0; i < convexHull.length(); i++) {
        double distantToPoint = ExMath::distantBeetweenPoints(outerPoint, convexHull.at(i));

        if (distantToPoint < minDistant) {
            closestPointIndex = i;
            minDistant = distantToPoint;
        }
    }

    int leftTangentEndIndex = closestPointIndex;
    int rightTangentEndIndex = leftTangentEndIndex;

    int nextLeftTangentEndIndex = leftTangentEndIndex;
    int nextRightTangentEndIndex = rightTangentEndIndex;
    int newPointIndex;

    QPointF closestPoint = convexHull.at(closestPointIndex),
            leftTangentEnd, rightTangentEnd;

    // Find points that are the ends of the tangents to the triangulation
    double maxAngleBetweenTangents = 0;

    for (int j = 0; j < 2; j++) {
        for (int i = 0, len = convexHull.length(); i < len; i++) {

            if (j == 0) {
                leftTangentEndIndex = nextLeftTangentEndIndex;
                newPointIndex = nextLeftTangentEndIndex = (leftTangentEndIndex + len - 1) % len;
            } else {
                rightTangentEndIndex = nextRightTangentEndIndex;
                newPointIndex = nextRightTangentEndIndex = (rightTangentEndIndex + 1) % len;
            }

            leftTangentEnd = convexHull.at(nextLeftTangentEndIndex);
            rightTangentEnd = convexHull.at(nextRightTangentEndIndex);
            double angleBetweenTangents = acos(ExMath::cosAngleBetweenVectors(leftTangentEnd, outerPoint, rightTangentEnd));

            if (angleBetweenTangents < maxAngleBetweenTangents ||
                ExMath::isLeftTurn(leftTangentEnd, outerPoint, rightTangentEnd) ||
                crossPointIncidentEdge(Edge(outerPoint, convexHull.at(newPointIndex)), closestPoint)) {

                nextLeftTangentEndIndex = leftTangentEndIndex;
                nextRightTangentEndIndex = rightTangentEndIndex;
                break;
            } else {
                maxAngleBetweenTangents = angleBetweenTangents;
            }
        }
    }

    leftTangentEnd = convexHull.at(leftTangentEndIndex);
    rightTangentEnd = convexHull.at(rightTangentEndIndex);

    while (leftTangentEnd != rightTangentEnd) {
        int nextHullPointIndex = (leftTangentEndIndex + 1) % convexHull.length();
        QPointF hullPointToRemove = convexHull.at(nextHullPointIndex);
        TriangularUnit triangle(leftTangentEnd, outerPoint, hullPointToRemove);
        newTriangles.append(triangle);
        appendTriangles(triangle);
        if (nextHullPointIndex != rightTangentEndIndex) {
            convexHull.remove(nextHullPointIndex);
        }

        leftTangentEnd = hullPointToRemove;
    }
    convexHull.insert((leftTangentEndIndex + 1 + convexHull.length()) % convexHull.length(), outerPoint);
    return newTriangles;
}

void DelaunayTriangulation::
    appendTriangles(QVector<TriangularUnit> &newTriangles) {

    for(int i = 0; i < newTriangles.length(); i++) {
        appendTriangles(newTriangles[i]);
    }
}

bool DelaunayTriangulation::
    crossPointIncidentEdge(Edge edge, QPointF p) {

    QVector<Edge> edges = findEdgesIncidentToPoint(p);
    for (int i = 0; i < edges.length(); i++) {
        Edge curEdge = edges[i];
        if (ExMath::areLinesCrossed(curEdge.getStartPoint(), curEdge.getEndPoint(), edge.getStartPoint(), edge.getEndPoint())) {
            return true;
        }
    }

    return false;
}

QVector<Edge> DelaunayTriangulation::
    findEdgesIncidentToPoint(QPointF p) {

    QVector<Edge> incidentEdges;
    for (int i = 0; i < edges.length(); i++) {
        Edge curEdge = edges[i];
        if (curEdge.getEndPoint() == p || curEdge.getStartPoint() == p) {
            incidentEdges.append(curEdge);
        }
    }

    return incidentEdges;
}

void DelaunayTriangulation::
    appendEdges(QVector<Edge> &newEdges) {

    for(int i = 0; i < newEdges.length(); i++) {
        edges.append(newEdges[i]);
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
    convexHull.clear();
    inscribedCircleCenters.clear();
}

void DelaunayTriangulation::setPoints(const QVector<QPointF> &value) {
    clear();
    points = value;
}
