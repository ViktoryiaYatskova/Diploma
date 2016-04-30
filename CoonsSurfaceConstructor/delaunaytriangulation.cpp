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
    QVector<QPointF> vertexes = firstTriangle.getVertexes();
    point1 = vertexes.at(0);
    point2 = vertexes.at(1);
    point3 = vertexes.at(2);

    appendTriangles(firstTriangle);
    convexHull.append(point1);
    convexHull.append(point2);
    convexHull.append(point3);
}

QVector<TriangularUnit> DelaunayTriangulation::
    addPointToTriangulation(QPointF pointToAdd) {

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

    return newTriangles;
}

void DelaunayTriangulation::
    convertToDelaunay() {

    checkDelaunayConditionLocaly(triangles);
}

void DelaunayTriangulation::
    build(bool isDelaunay) {
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

        QVector<TriangularUnit> newTriangles = addPointToTriangulation(pointToAdd);

        if (isDelaunay) {
            checkDelaunayConditionLocaly(newTriangles);
        }
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
    getClosestConvexEdgeToPoint(QPointF &p) {

    double minDistantToPoint = ExMath::DOUBLE_MAX;
    Edge closestEdge;

    QVectorIterator<QPointF> it(convexHull);
    QPointF prevPoint = it.next();

    while(it.hasNext() || prevPoint == convexHull.last()) {
        QPointF currentEdgeA = prevPoint;
        QPointF currentEdgeB = it.hasNext()? it.next(): convexHull.first();
        Edge currentEdge(currentEdgeA, currentEdgeB);

        double distantToPoint = currentEdge.distantToPoint(p);
        bool isRightTurn = ExMath::isRightTurn(currentEdgeB, p, currentEdgeA);

        if ( isRightTurn ) {
            if ( distantToPoint < minDistantToPoint ) {
                minDistantToPoint = distantToPoint;
                closestEdge = currentEdge;
            }
        }
        prevPoint = currentEdgeB;
    }
    if (closestEdge.getEndPoint().isNull() && closestEdge.getStartPoint().isNull()) {
        ExMath::consoleLog("Not found closest convex edge in right direction!");
        throw std::exception();
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

    int startPointIndex = convexHull.indexOf(splittedEdge.getStartPoint());
    int endPointIndex = convexHull.indexOf(splittedEdge.getEndPoint());

    if ( startPointIndex >= 0 && endPointIndex >= 0 ) {
        convexHull.insert(std::max(startPointIndex, endPointIndex), splittingPoint);
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

    Edge closestEdge = getClosestConvexEdgeToPoint(outerPoint);
    QPointF leftTangentEnd, rightTangentEnd;
    int leftTangentEndIndex, rightTangentEndIndex;

    leftTangentEnd = closestEdge.getStartPoint();
    rightTangentEnd = closestEdge.getEndPoint();
    leftTangentEndIndex = convexHull.indexOf(leftTangentEnd);
    rightTangentEndIndex = convexHull.indexOf(rightTangentEnd);

    int nextLeftTangentEndIndex = leftTangentEndIndex;
    int nextRightTangentEndIndex = rightTangentEndIndex;
    int newPointIndex;

    // Find points that are the ends of the tangents to the triangulation
    double maxAngleBetweenTangents = acos(ExMath::cosAngleBetweenVectors(leftTangentEnd, outerPoint, rightTangentEnd));

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
                nextLeftTangentEndIndex == nextRightTangentEndIndex||
                ExMath::isLeftTurn(rightTangentEnd, outerPoint, leftTangentEnd) /*||
                crossPointIncidentEdge(Edge(outerPoint, convexHull.at(newPointIndex)), closestPoint)*/) {

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

    QPointF hullPointToRemove;

    while (hullPointToRemove != rightTangentEnd) {
        int hullPointToRemoveIndex = (leftTangentEndIndex + 1) % convexHull.length();
        hullPointToRemove = convexHull.at(hullPointToRemoveIndex);
        TriangularUnit triangle(leftTangentEnd, outerPoint, hullPointToRemove);
        newTriangles.append(triangle);
        if (hullPointToRemove != rightTangentEnd) {
            convexHull.remove(hullPointToRemoveIndex);
            if (hullPointToRemoveIndex < leftTangentEndIndex) {
                leftTangentEndIndex--;
            }
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

    QSetIterator<Edge> it(edges);
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

QVector<int> DelaunayTriangulation::
    getTriangularUnitNeighbors(TriangularUnit& triangular) {

    QVector<int> neighborsIndexes;
    neighborsIndexes.reserve(3);

    for (int i = 0; i < triangles.length(); i++) {
        TriangularUnit curTriangle = triangles.at(i);
        if (curTriangle != triangular && curTriangle.hasMutualEdge(triangular)) {
            neighborsIndexes.append(i);
        }
    }

    return neighborsIndexes;
}

/**
 * @brief DelaunayTriangulation::checkDelaunayConditionLocaly
 * @param triangles
 * @return {bool} is triangulation restructured
 */
bool DelaunayTriangulation::
    checkDelaunayConditionLocaly(QVector<TriangularUnit>& trianglesToCheck) {

    bool isRestructured = false;
    QList< TriangularUnit > trianglesToAdd;
    QList< TriangularUnit > trianglesToRemove;

    for (int i = 0; i < trianglesToCheck.length(); i++) {
        int triangleIndex = i;
        TriangularUnit triangle = trianglesToCheck.at(triangleIndex);
        QVector< int > neighborsIndexes = getTriangularUnitNeighbors(triangle);
        QVectorIterator< int > itNeighbors( neighborsIndexes );

        QVector<QPointF> triangleVertexes = triangle.getVertexes();

        if (neighborsIndexes.length() > 3) {
            ExMath::consoleLog(neighborsIndexes.length());
        }

        while (itNeighbors.hasNext()) {
            int neighborIndex = itNeighbors.next();
            TriangularUnit neighbor = triangles.at(neighborIndex);
            QPointF p0 = triangle.getNotAdjacentPoint(neighbor);
            Edge mutualEdge = neighbor.getMutualEdge(triangle);
            double x0 = p0.x(), y0 = p0.y();

            int indexMutualVertex1 = triangleVertexes.indexOf(mutualEdge.getStartPoint());
            int indexMutualVertex2 = triangleVertexes.indexOf(mutualEdge.getEndPoint());
            int i1 = (abs(indexMutualVertex1 - indexMutualVertex2) == 1)?
                         std::min(indexMutualVertex1, indexMutualVertex2):
                         std::max(indexMutualVertex1, indexMutualVertex2);
            int i3 = (i1 == indexMutualVertex1)? indexMutualVertex2: indexMutualVertex1;
            int i2 = 3 - i1 - i3;
            double x1 = triangleVertexes.at(i1).x(), y1 = triangleVertexes.at(i1).y();
            double x2 = triangleVertexes.at(i2).x(), y2 = triangleVertexes.at(i2).y();
            double x3 = triangleVertexes.at(i3).x(), y3 = triangleVertexes.at(i3).y();

            double targetValue = abs((x0 - x1)*(y0 - y3) - (x0 - x3)*(y0 - y1))*
                                 ((x2 - x1)*(x2 - x3) + (y2 - y1)*(y2 - y3)) +
                                 ((x0 - x1)*(x0 - x3) + (y0 - y1)*(y0 - y3))*
                                 abs((x2 - x1)*(y2 - y3) - (x2 - x3)*(y2 - y1));

            //targetValue = (x1*x1 + y1*y1)*(y2*x3 - x2*y3) + (x2*x2 + y2*y2)*(x1*y3 - y1*x3) + (x3*x3 + y3*y3)*(y1*x2 - x1*y2) <= 0;
            if (targetValue < 0/* && !triangle.getIsRestructered() && !neighbor.getIsRestructered()*/) {
                QPointF p4 = neighbor.getNotAdjacentPoint(triangle);
                Edge newMutualEdge(p0, p4);
                TriangularUnit newTriangle1(newMutualEdge, mutualEdge.getEndPoint());
                TriangularUnit newTriangle2(newMutualEdge, mutualEdge.getStartPoint());
                //newTriangle1.setIsRestructered(true);
                //newTriangle2.setIsRestructered(true);

                edges.remove(mutualEdge);
                edges.insert(newMutualEdge);
                triangles[neighborIndex] = newTriangle1;//.setIsRestructered(true);
                triangles[triangles.indexOf(triangle)] = newTriangle2;//.setIsRestructered(true);
                break;
                //trianglesToRemove.append(neighbor);
                //trianglesToRemove.append(triangle);

                //trianglesToAdd.append(newTriangle1);
                //trianglesToAdd.append(newTriangle2);

                ExMath::consoleLog("swapped");
                isRestructured = true;
            }
        }
    }

    /*QListIterator< TriangularUnit > itAdd(trianglesToAdd);
    while (itAdd.hasNext()) {
       triangles.append(itAdd.next());
    }

    QListIterator< TriangularUnit > itRemove(trianglesToRemove);

    while (itRemove.hasNext()) {
        TriangularUnit removeTriangle = itRemove.next();
        triangles.removeOne(removeTriangle);
        //removeEdges(removeTriangle.getEdges());
    }*/

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
