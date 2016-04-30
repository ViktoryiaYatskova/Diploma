#include "triangularunit.h"



TriangularUnit::
    TriangularUnit():isEmpty(true), isRestructered(false) {

    vertexes.reserve(3);
    edges.reserve(3);
}

TriangularUnit::
    TriangularUnit(const TriangularUnit &triangle) {

    vertexes = QVector<QPointF>(triangle.getVertexes());
    edges = QSet<Edge>(triangle.edges);
    isEmpty = triangle.isEmpty;
    isRestructered = triangle.isRestructered;
}

TriangularUnit::
    TriangularUnit(QPointF* trVertexes): isRestructered(false) {

    addPoints(trVertexes[0], trVertexes[1], trVertexes[2]);

    for (int i = 0; i < 3; i++) {
        edges.insert(Edge(trVertexes[i], trVertexes[(i + 1) % 3]));
    }

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const QPointF p1, const QPointF p2, const QPointF p3): isRestructered(false) {

    addPoints(p1, p2, p3);

    for (int i = 0; i < 3; i++) {
        edges.insert(Edge(vertexes[i], vertexes[(i + 1) % 3]));
    }

    isEmpty = false;
}

QVector<QPointF> TriangularUnit::getVertexes() const {
    return vertexes;
}

void TriangularUnit::setVertexes(const QVector<QPointF> &value) {
    vertexes = value;
}

bool TriangularUnit::getIsRestructered() const
{
    return isRestructered;
}

void TriangularUnit::setIsRestructered(bool value)
{
    isRestructered = value;
}
void TriangularUnit::addPoints(const QPointF p1, const QPointF p2, const QPointF p3) {
    int p = ((p1.x() - p2.x()) * (p3.x() - p2.x())) - ((p3.y() - p2.y()) * (p1.y() - p2.y()));
    if (p > 0) {
        vertexes.append(p1);
        vertexes.append(p2);
        vertexes.append(p3);
    } else {
        vertexes.append(p3);
        vertexes.append(p2);
        vertexes.append(p1);
    }
}

TriangularUnit::TriangularUnit(const Edge &edge1, const Edge &edge2, const Edge &edge3) {
    edges.insert(edge1);
    edges.insert(edge2);
    edges.insert(edge3);

    QSetIterator<Edge> it(edges);
    QVector<QPointF> tmpVertexes;

    while (it.hasNext()) {
        Edge edge = it.next();

        QPointF p = edge.getStartPoint();
        if (!tmpVertexes.contains(p)) {
            tmpVertexes.append(p);
        }

        p = edge.getEndPoint();
        if (!tmpVertexes.contains(p)) {
            tmpVertexes.append(p);
        }
    }
    addPoints(tmpVertexes.at(0), tmpVertexes.at(1), tmpVertexes.at(2));
    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const Edge &edge, const QPointF point) {

    edges.insert(edge);
    edges.insert(Edge(point, edge.getStartPoint()));
    edges.insert(Edge(point, edge.getEndPoint()));

    addPoints(point, edge.getEndPoint(), edge.getStartPoint());

    isEmpty = false;
}

TriangularUnit::
    TriangularUnit(const Edge &edge1, const Edge &edge2) {

    edges.insert(edge1);
    edges.insert(edge2);

    QPointF p1 = edge1.getMutualPoint(edge2);
    QPointF p2 = edge1.getStartPoint() == p1 ? edge1.getEndPoint(): edge1.getStartPoint();
    QPointF p3 = edge2.getStartPoint() == p1 ? edge2.getEndPoint(): edge2.getStartPoint();
    addPoints(p1, p2, p3);
    edges.insert(Edge(p2, p3));

    isEmpty = false;
}

QPointF TriangularUnit::getNotAdjacentPoint(TriangularUnit &adjacentTriangular) {

    QVector<QPointF> adjacentTriangularPoints = adjacentTriangular.getVertexes();

    for (int i = 0; i < adjacentTriangularPoints.length(); i++) {
        QPointF p = adjacentTriangularPoints.at(i);
        if (!vertexes.contains(p)) {
            return p;
        }
    }
    return QPointF();
}

bool TriangularUnit::
    hasMutualEdge(TriangularUnit& other) {

    QSetIterator< Edge > it( other.getEdges() );
    while (it.hasNext()) {
        Edge edge = it.next();
        if (edges.contains(edge)) {
            return true;
        }
    }

    return false;
}

Edge TriangularUnit::
    getMutualEdge(TriangularUnit& other) {

    QSetIterator< Edge > it( other.getEdges() );
    while (it.hasNext()) {
        Edge edge = it.next();
        if (edges.contains(edge)) {
            return edge;
        }
    }

    return Edge();
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

    return ExMath::pointPositionToTriangle(vertexes[0], vertexes[1], vertexes[2], point) == INSIDE;
    //QPointF center = getEscribedCircleCenter();
    //return pow(point.x() - center.x(), 2) + pow(point.y() - center.y(), 2) >= pow(escribedCircleRadius, 2);
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

bool TriangularUnit::operator !=(const TriangularUnit &other){
    return !(this->operator ==(other));
}

bool TriangularUnit::
    getIsEmpty() {

    return isEmpty;
}

Edge TriangularUnit::
    getTriangleEdgeThatContainsPoint(QPointF &point) {

    QSetIterator< Edge > it( edges );
    while (it.hasNext()) {
        Edge current = it.next();
        if ( ExMath::isPointOnLine(point, current.getStartPoint(), current.getEndPoint()) ) {
            return current;
        }
    }

    throw std::exception(); //"No edge that contains point"
}

QSet<Edge>& TriangularUnit::
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

    ExMath::consoleLog("The triangle is confluent!");
    throw std::exception();
}

QPointF& TriangularUnit::
    getClosestVertexToPoint(const QPointF& point) {

    double closestDistantToPoint = ExMath::DOUBLE_MAX;
    QPointF closestVertex;

    QVectorIterator< QPointF > it( vertexes );
    while (it.hasNext()) {
        QPointF vertex = it.next();
        // as a sum of the distances to the bound vertexes of the edge
        double distantToPoint = ExMath::distantBeetweenPoints(vertex, point);

        closestDistantToPoint = std::min(closestDistantToPoint, distantToPoint);

        if (closestDistantToPoint == distantToPoint) {
            closestVertex = vertex;
        }
    }

    return closestVertex;
}

Edge& TriangularUnit::
    getClosestEdgeToPoint(const QPointF& point) {

    double closestDistantToEdge = ExMath::DOUBLE_MAX;
    Edge closestEdge;

    QSetIterator< Edge > it( edges );
    while (it.hasNext()) {
        Edge edge = it.next();
        // as a sum of the distances to the bound vertexes of the edge
        double distantToEdge = ExMath::distantBeetweenPoints(edge.getStartPoint(), point) +
                        ExMath::distantBeetweenPoints(edge.getEndPoint(), point);

        closestDistantToEdge = std::min(closestDistantToEdge, distantToEdge);

        if (closestDistantToEdge == distantToEdge) {
            closestEdge = edge;
        }
    }

    return closestEdge;
}
