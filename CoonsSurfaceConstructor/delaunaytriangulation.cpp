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

        TriangularUnit nearestTriangle = getClosestTriangleToPoint(pointToAdd);
        int positionToTriangle = definePointPositionToTriangle(pointToAdd, nearestTriangle);

        if (positionToTriangle == INSIDE) {

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::INSIDE");
            newTriangles = replaceTriangleWithThreeSplittedParts(nearestTriangle, pointToAdd);

        } else if (positionToTriangle == ON_EDGE) {

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::ON_EDGE");
            newTriangles = replaceTriangleWithTwoSplittedParts(nearestTriangle, pointToAdd);

        } else { // relation == ExMath::POINT_POSITION_TO_TRIANGLE::OUTSIDE

            ExMath::consoleLog("POINT_POSITION_TO_TRIANGLE::OUTSIDE");
            newTriangles = createNewBoundaryTriangles(pointToAdd);
        }

        //checkDelaunayConditionLocaly(newTriangles);
    }
}
QSet<Edge> DelaunayTriangulation::
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

Edge &DelaunayTriangulation::
    getClosestEdgeToPoint(QPointF &p) {

    double minDistantToPoint = ExMath::DOUBLE_MAX;
    Edge closestEdge, currentEdge;

    QSetIterator<Edge> it(edges);

    while(it.hasNext()) {
        currentEdge = it.next();

        double distantToPoint = currentEdge.distantToPoint(p);
        minDistantToPoint = std::min( minDistantToPoint, distantToPoint );

        if ( distantToPoint == minDistantToPoint ) {
            closestEdge = currentEdge;
        }
    }
    return closestEdge;
}

int DelaunayTriangulation::
    definePointPositionToTriangle(QPointF &point, TriangularUnit &triangle) {


    QVector<QPointF> vertexes = triangle.getVertexes();
    return ExMath::pointPositionToTriangle(vertexes.at(0), vertexes.at(1), vertexes.at(2), point);
}

TriangularUnit DelaunayTriangulation::
    definePointPositionToTriangulation(QPointF &point) {

    TriangularUnit emptyTriangle;
    TriangularUnit closestTriangle = getClosestTriangleToPoint(point);
    //while(true) {
        //int positionToTriangle = definePointPositionToTriangle(closestTriangle, point);
        //if (positionToTriangle != OUTSIDE) {
            return closestTriangle;
        //}
    //}

    //return emptyTriangle;
}

QVector<TriangularUnit> DelaunayTriangulation::
    replaceTriangleWithThreeSplittedParts(TriangularUnit &triangle, QPointF &splittingPoint) {

    QVector<TriangularUnit> newTriangles;
    QSetIterator<Edge> it(triangle.getEdges());

    while (it.hasNext()) {
        newTriangles.append(TriangularUnit( it.next(), splittingPoint ));
    }
    triangles.removeOne(triangle);
    appendTriangles(newTriangles);

    return newTriangles;
}

QVector<TriangularUnit> DelaunayTriangulation::
    replaceTriangleWithTwoSplittedParts(TriangularUnit &triangle, QPointF &splittingPoint) {

    QVector<TriangularUnit> newTriangles;
    Edge splittedEdge = triangle.getTriangleEdgeThatContainsPoint(splittingPoint);
    triangle.getEdges().remove(splittedEdge);
    QSetIterator<Edge> it(triangle.getEdges());

    Edge remaindEdge1 = it.next(), remaindEdge2 = it.next();
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

bool DelaunayTriangulation::
    arePointsSeparatedByEdge(QPointF& p1, QPointF& p2, Edge& edge) {

    return ExMath::areLinesCrossed(p1, p2, edge.getEndPoint(), edge.getStartPoint());
}
ConvexHull DelaunayTriangulation::
    getConvexHull() const {

    return convexHull;
}


QVector<TriangularUnit> DelaunayTriangulation::
    createNewBoundaryTriangles(QPointF &outerPoint) {

    QVector<TriangularUnit> newTriangles;

    if (!convexHull.length()) return newTriangles;

    int closestPointIndex = 0;
    double minDistant = ExMath::DOUBLE_MAX;
    Edge closestEdge = getClosestEdgeToPoint(outerPoint);

    for ( int i = 0; i < convexHull.length(); i++) {
        QPointF curPoint = convexHull.at(i);
        double distantToPoint = ExMath::distantBeetweenPoints(curPoint, outerPoint);

        if (distantToPoint < minDistant && !arePointsSeparatedByEdge(curPoint, outerPoint, closestEdge)) {
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
        if (hullPointToRemove != rightTangentEnd) {
            convexHull.remove(nextHullPointIndex);
        }

        leftTangentEnd = hullPointToRemove;
    }
    convexHull.insert((leftTangentEndIndex + 1) % convexHull.length(), outerPoint);
    appendTriangles(newTriangles);
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

    QSetIterator<Edge> it(findEdgesIncidentToPoint(p));
    while (it.hasNext()) {
        Edge curEdge = it.next();
        if (ExMath::areLinesCrossed(curEdge.getStartPoint(), curEdge.getEndPoint(), edge.getStartPoint(), edge.getEndPoint())) {
            return true;
        }
    }

    return false;
}

QSet<Edge> DelaunayTriangulation::
    findEdgesIncidentToPoint(QPointF p) {

    QSet<Edge> incidentEdges;
    incidentEdges.reserve(3);
    QSetIterator<Edge> it(edges);

    while (it.hasNext()) {
        Edge curEdge = it.next();
        if (curEdge.getEndPoint() == p || curEdge.getStartPoint() == p) {
            incidentEdges.insert(curEdge);
        }
    }

    return incidentEdges;
}

void DelaunayTriangulation::
    appendEdges(QSet<Edge> &newEdges) {

    QSetIterator<Edge> it(newEdges);
    while (it.hasNext()) {
        Edge nextEdge = it.next();
        edges.insert(nextEdge);
    }
}

void DelaunayTriangulation::
    removeEdges(QSet<Edge> &newEdges) {

    QSetIterator<Edge> it(newEdges);
    while (it.hasNext()) {
        Edge nextEdge = it.next();
        edges.remove(nextEdge);
    }
}

void DelaunayTriangulation::
    appendTriangles(TriangularUnit& triangle) {

    triangles.append(triangle);
    appendEdges(triangle.getEdges());
}

QVector<TriangularUnit> DelaunayTriangulation::
    getTriangularUnitNeighbors(TriangularUnit& triangular) {

    QVector<TriangularUnit> neighbors;
    neighbors.reserve(3);

    for (int i = 0; i < triangles.length(); i++) {
        TriangularUnit curTriangle = triangles.at(i);
        if (curTriangle != triangular && curTriangle.hasMutualEdge(triangular)) {
            neighbors.append(curTriangle);
        }
    }

    return neighbors;
}

/**
 * @brief DelaunayTriangulation::checkDelaunayConditionLocaly
 * @param triangles
 * @return {bool} is triangulation restructured
 */
bool DelaunayTriangulation::
    checkDelaunayConditionLocaly(QVector<TriangularUnit>& trianglesToCheck) {

    bool isRestructured = false;
    QVectorIterator<TriangularUnit> it( trianglesToCheck );
    while (it.hasNext()) {
        TriangularUnit triangle = it.next();
        QVector<TriangularUnit> neighbors = getTriangularUnitNeighbors(triangle);
        QVectorIterator<TriangularUnit> itNeighbors( neighbors );

        QVector<QPointF> triangleVertexes = triangle.getVertexes();

        while (itNeighbors.hasNext()) {
            TriangularUnit neighbor = itNeighbors.next();
            QPointF p0 = triangle.getNotAdjacentPoint(neighbor);
            Edge mutualEdge = neighbor.getMutualEdge(triangle);
            double x0 = p0.x(), y0 = p0.y();

            int indexMutualVertex1 = triangle.getVertexes().indexOf(mutualEdge.getStartPoint());
            int indexMutualVertex2 = triangle.getVertexes().indexOf(mutualEdge.getEndPoint());
            int i1 = abs(indexMutualVertex1 - indexMutualVertex2) == 1?
                         std::min(indexMutualVertex1, indexMutualVertex2):
                         std::max(indexMutualVertex1, indexMutualVertex2);
            int i3 = i1 == indexMutualVertex1? indexMutualVertex2: indexMutualVertex1;
            int i2 = 3 - i1 - i3;
            double x1 = triangleVertexes.at(i1).x(), y1 = triangleVertexes.at(i1).y();
            double x2 = triangleVertexes.at(i2).x(), y2 = triangleVertexes.at(i2).y();
            double x3 = triangleVertexes.at(i3).x(), y3 = triangleVertexes.at(i3).y();

            double targetValue = abs((x0 - x1)*(y0 - y3) - (x0 - x3)*(y0 - y1))*
                                 ((x2 - x1)*(x2 - x3) + (y2 - y1)*(y2 - y3)) +
                                 ((x0 - x1)*(x0 - x3) + (y0 - y1)*(y0 - y3))*
                                 abs((x2 - x1)*(y2 - y3) - (x2 - x3)*(y2 - y1));

            //targetValue = (x1*x1 + y1*y1)*(y2*x3 - x2*y3) + (x2*x2 + y2*y2)*(x1*y3 - y1*x3) + (x3*x3 + y3*y3)*(y1*x2 - x1*y2) <= 0;
            if (targetValue < 0) {
                QPointF p4 = neighbor.getNotAdjacentPoint(triangle);
                Edge newMutualEdge(p0, p4);
                TriangularUnit newTriangle1(newMutualEdge, mutualEdge.getEndPoint());
                TriangularUnit newTriangle2(newMutualEdge, mutualEdge.getStartPoint());

                edges.remove(mutualEdge);
                edges.insert(newMutualEdge);
                triangles.removeOne(neighbor);
                triangles.removeOne(triangle);

                triangles.append(newTriangle1);
                triangles.append(newTriangle2);

                ExMath::consoleLog("swapped");
                isRestructured = true;
            }
        }
    }

    return isRestructured;
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
