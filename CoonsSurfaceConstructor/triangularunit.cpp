#include "triangularunit.h"


QVector<QPointF> TriangularUnit::getVertexes() const {
    return vertexes;
}

void TriangularUnit::setVertexes(const QVector<QPointF> &value) {
    vertexes = value;
}

TriangularUnit::
    TriangularUnit() {

    //create empty triangular
    isEmpty = true;
}

TriangularUnit::
    TriangularUnit(const TriangularUnit &triangle) {

    vertexes = QVector<QPointF>(triangle.vertexes);
    edges = QVector<Edge>(triangle.edges);
    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(QPointF* trVertexes) {

    for (int i = 0; i < 3; i++) {
        vertexes.append(trVertexes[i]);
    }

    for (int i = 0; i < 3; i++) {
        edges.append(Edge(trVertexes[i], trVertexes[(i + 1) % 3]));
    }

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const QPointF p1, const QPointF p2, const QPointF p3) {
    vertexes.append(p1);
    vertexes.append(p2);
    vertexes.append(p3);

    for (int i = 0; i < 3; i++) {
        edges.append(Edge(vertexes[i], vertexes[(i + 1) % 3]));
    }

    isEmpty = false;
}

TriangularUnit::TriangularUnit(const Edge &edge1, const Edge &edge2, const Edge &edge3) {
    edges.append(edge1);
    edges.append(edge2);
    edges.append(edge3);

    for (int i = 0; i < 3; i++) {
        QPointF p = edges.at(i).getStartPoint();
        if (!vertexes.contains(p)) {
            vertexes.append(p);
        }

        p = edges.at(i).getEndPoint();
        if (!vertexes.contains(p)) {
            vertexes.append(p);
        }
    }

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const Edge &edge, const QPointF point) {

    edges.append(edge);
    edges.append(Edge(point, edge.getStartPoint()));
    edges.append(Edge(point, edge.getEndPoint()));

    vertexes.append(point);
    vertexes.append(edge.getEndPoint());
    vertexes.append(edge.getStartPoint());

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const Edge &edge1, const Edge &edge2) {

    edges.append(edge1);
    edges.append(edge2);

    appendVertex(edge1.getStartPoint());
    appendVertex(edge1.getEndPoint());
    appendVertex(edge2.getStartPoint());
    appendVertex(edge2.getEndPoint());

    for (int i = 0; i < 3; i++) {
        Edge edge( vertexes.at(i), vertexes.at(i%3) );
        if (edges.contains(edge)) {
            edges.append(edge);
        }
    }

    isEmpty = false;
}

void TriangularUnit::appendVertex(QPointF vertex) {
    if (vertexes.contains(vertex)) {
        vertexes.append(vertex);
    }
}

TriangularUnit::
    TriangularUnit(QPointF* trVertexes, TriangularUnit* triangulars) {

    for (int i = 0; i < 3; i++) {
        neighborTriangulars.append(triangulars[i]);
    }

    for (int i = 0; i < 3; i++) {
        vertexes.append(trVertexes[i]);
    }

    isEmpty = false;
}

bool TriangularUnit::
    isBoundary() {

    for (int i = 0; i < 3; i++) {
        if (edges.at(i).getIsBoundary()) {
            return true;
        }
    }

    return false;
}

QPointF TriangularUnit::
    getEscribedCircleCenter() {

    if (escribedCircleCenter.isNull()) {
        float x1 = vertexes.at(0).x();
        float x2 = vertexes.at(1).x();
        float x3 = vertexes.at(2).x();

        float y1 = vertexes.at(0).y();
        float y2 = vertexes.at(1).y();
        float y3 = vertexes.at(2).y();

        //float s = 0.5*abs(x1*(y2-y3)-y1*(x2-x3)+x2*y3-y2*x3);
        //float x0 = ((x1*x1+y1*y1)*(y2-y3)-y1*(x2*x2+y2*y2-x3*x3-y3*y3)+y3*(x2*x2+y2*y2)-y2*(x3*x3+y3*y3))/(-4.0*s);
        //float y0 = ((x1*x1+y1*y1)*(x2-x3)-x1*(x2*x2+y2*y2-x3*x3-y3*y3)+x3*(x2*x2+y2*y2)-x2*(x3*x3-y3*y3))/(4.0*s);

        double mD[3][3] = { //a
            {x1, y1, 1},
            {x2, y2, 1},
            {x3, y3, 1}
        };

        double mXo[3][3] = { //b
            {x1*x1 + y1*y1, y1, 1},
            {x2*x2 + y2*y2, y2, 1},
            {x3*x3 + y3*y3, y3, 1}
        };

        double mYo[3][3] = { //c
            {x1*x1 + y1*y1, x1, 1},
            {x2*x2 + y2*y2, x2, 1},
            {x3*x3 + y3*y3, x3, 1}
        };

        double md[3][3] = { //d
           {x1*x1 + y1*y1, x1, y1},
           {x2*x2 + y2*y2, x2, y2},
           {x3*x3 + y3*y3, x3, y3}
        };

        double a = ExMath::determinant3(mD);
        double b = ExMath::determinant3(mXo);
        double c = ExMath::determinant3(mYo);
        double d = ExMath::determinant3(md);

        double D = 2 * a;
        double Xo = b / D;
        double Yo = -c / D;

        escribedCircleCenter.setX(Xo);
        escribedCircleCenter.setY(Yo);
        //r = b + c − 4 ad / 4 a
        escribedCircleRadius = sqrt(b*b + c*c - 4*a*d) / (2 * a);
    }
    return escribedCircleCenter;
}

bool TriangularUnit::
    isPointInsideTriangle(QPointF point) {

    QPointF center = getEscribedCircleCenter();
    return pow(point.x() - center.x(), 2) + pow(point.y() - center.y(), 2) >= pow(escribedCircleRadius, 2);
}

QPointF TriangularUnit::
    getInscribedCircleCenter() {

    if (inscribedCircleCenter.isNull()) {
        float x1 = vertexes.at(0).x();
        float x2 = vertexes.at(1).x();
        float x3 = vertexes.at(2).x();

        float y1 = vertexes.at(0).y();
        float y2 = vertexes.at(1).y();
        float y3 = vertexes.at(2).y();

        double a = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
        double b = sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));
        double c = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
        double p = (a + b + c)/2;
        double S = 0.5 * abs(((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1)));
        //double r = S/p;

        double x0 = ((x1 + x2 - 2*x3)*((y3 - y1)*(x2 + x3 - 2*x1) + x1*(y2 + y3 - 2*y1)) - x3*(x2 + x3 - 2*x1)*(y1 + y2 - 2*y3))/((y2 + y3 - 2*y1)*(x1 + x2 - 2*x3) - (x2 + x3 - 2*x1)*(y1 + y2 - 2*y3));
        double y0 = ((x0 - x1)*(y2 + y3 - 2*y1)/(x2 + x3 - 2*x1)) + y1;
        inscribedCircleCenter.setX(x0);
        inscribedCircleCenter.setY(y0);
    }
    return inscribedCircleCenter;
}

bool TriangularUnit::
    operator ==(const TriangularUnit & other){

    for (int i = 0; i < 3; i++) {
        if (!vertexes.contains(other.vertexes.at(i))) {
            return false;
        }
    }
    return true;
}

bool TriangularUnit::
    getIsEmpty() {

    return isEmpty;
}

Edge TriangularUnit::
    getTriangleEdgeThatContainsPoint(QPointF &point) {

    for (int i = 0; i < edges.length(); i++) {
        Edge current = edges.at(i);
        if ( ExMath::isPointOnLine(point, current.getStartPoint(), current.getEndPoint()) ) {
            return edges.at(i);
        }
    }

    throw std::exception(); //"No edge that contains point"
}

QVector<Edge>& TriangularUnit::
        getEdges() {

    return edges;
}

QPointF TriangularUnit::
    getFrontPointToEdge(Edge& edge) {

    for (int i = 0; i < vertexes.length(); i++) {
        if (!ExMath::isPointOnLine(vertexes.at(i), edge.getEndPoint(), edge.getStartPoint())) {
            return vertexes.at(i);
        }
    }

    throw std::exception();//"The triangle is confluent!");
}

Edge& TriangularUnit::
    getClosestEdgeToPoint(const QPointF& point) {

    double closestDistantToEdge = ExMath::DOUBLE_MAX;
    int closestEdgeIndex;

    for (int i = 0; i < edges.length(); i++) {
        // as a sum of the distances to the bound vertexes of the edge
        double distantToEdge = ExMath::distantBeetweenPoints(edges.at(i).getStartPoint(), point) +
                        ExMath::distantBeetweenPoints(edges.at(i).getEndPoint(), point);

        closestDistantToEdge = std::min(closestDistantToEdge, distantToEdge);

        if (closestDistantToEdge == distantToEdge) {
            closestEdgeIndex = i;
        }
    }

    return edges[closestEdgeIndex];
}

Edge& TriangularUnit::
    getClosestBoundaryEdgeToPoint(const QPointF& point) {

    double closestDistantToEdge = ExMath::DOUBLE_MAX;
    int closestEdgeIndex;

    for (int i = 0; i < edges.length(); i++) {
        if (!edges[i].getIsBoundary()) continue;

        // as a sum of the distances to the bound vertexes of the edge
        double distantToEdge = ExMath::distantBeetweenPoints(edges.at(i).getStartPoint(), point) +
                        ExMath::distantBeetweenPoints(edges.at(i).getEndPoint(), point);

        closestDistantToEdge = std::min(closestDistantToEdge, distantToEdge);

        if (closestDistantToEdge == distantToEdge) {
            closestEdgeIndex = i;
        }
    }

    return edges[closestEdgeIndex];
}
