#include "createscene.h"

const int CreateScene::POINTS_NUMBER = 25;

CreateScene::CreateScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setAutoFillBackground(false);

    currentMode = ADD_POINTS;
}

void CreateScene::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (currentMode) {

    case ADD_POINTS:
        drawPoints();
        break;

    case TRIANGULAR:
        drawTriangular();
        break;

    case CONVEX_HULL:
        drawTriangular();
        drawConvexHull();
        break;

    default:
        break;
    }
}

void CreateScene::initializeGL(){

    glShadeModel (GL_FLAT);
    qglClearColor(Qt::black);

    resizeGL(width(), height());
}

void CreateScene::resizeGL(int width, int height){

    glViewport(0,       //win_offset_x,
               0,       //win_offset_y,
               width,   //win_width,
               height); //win_height

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( 0,             /*win_offset_x*/
                0 + width,     /*win_offset_x+win_width*/
                0,             /*win_offset_y*/
                0 + height);   /*win_offset_y+win_height*/
}

void CreateScene::paintGL() {
    draw();
}

void CreateScene::drawPoints(QList<QPointF> points) {
    glBegin(GL_POINTS);
        QListIterator<QPointF> i(points);
        while (i.hasNext()) {
            QPointF p = i.next();
            glVertex2f( p.x(), p.y());
        }
    glEnd();
}

void CreateScene::drawEdges(QList<Edge> edges) {
    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0);

    QListIterator<Edge> i(edges);

    while (i.hasNext()) {
        Edge e = i.next();

        glBegin(GL_LINES);
            glVertex2f(e.getStartPoint().x(), e.getStartPoint().y());
            glVertex2f(e.getEndPoint().x(), e.getEndPoint().y());
        glEnd();
    }

    glPopAttrib();
}

void CreateScene::
    drawPoints() {

    glPushAttrib(GL_ENABLE_BIT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    glPointSize(3.0);

    glColor3f(0.5f, 0.0f, 0.5f);
    drawPoints(points.toList());

    glPopAttrib();
}

void CreateScene::
    drawTriangular() {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_POINT_SMOOTH);

    glPointSize(3.0);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawPoints(points.toList());

    glColor3f(0.0f, 0.5f, 0.7f);
    drawPoints(triangulation.inscribedCircleCenters.toList());

    glColor3f(1.0f, 1.0f, 1.0f);
    drawEdges(triangulation.getEdges().toList());

    glPopAttrib();
}

void CreateScene::
    showConvexHull() {

    currentMode = CONVEX_HULL;
    repaint();
}

void CreateScene::
    drawConvexHull() {

    if (currentMode != CONVEX_HULL || points.length() < 3) return;

    glPushAttrib(GL_ENABLE_BIT); //glPushAttrib is done to return everything to normal after drawing

    //glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    glPointSize(3.0);
    glLineWidth(1.0);
    glColor3f(1.0f, 0.0f, 1.0f);

    glLineStipple(1, 0xAAAA);

    QVectorIterator<QPointF> i(triangulation.getConvexHull());
    QPointF first = i.next(), p1 = first, p2;

    glBegin(GL_LINES);
        while (i.hasNext()) {
            p2 = i.next();

            glVertex2f(p1.x(), p1.y());
            glVertex2f(p2.x(), p2.y());

            p1 = p2;
        }
        p2 = first;
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());

    glEnd();
    glPopAttrib();
}

void CreateScene::mouseReleaseEvent(QMouseEvent *event) {
    QPoint lastPos = event->pos();

    if (currentMode == ADD_POINTS) {
        if(points.contains(lastPos)) { return; }
        points.append(QPointF(lastPos.x(), height() - lastPos.y()));
        repaint();
    }
}

void CreateScene::
    buildSimpleTriangular() {
    if (points.length() < 3) return;

    triangulation.setPoints(points);
    triangulation.build(false);

    currentMode = TRIANGULAR;
    //grahamScan.setPoints(points.toList());
    //grahamScan.prebuild();
    //grahamScan.build();
    repaint();
}

void CreateScene::convertToDelaunayTriangular() {
    if (points.length() < 3) return;

    if (currentMode != TRIANGULAR &&
        currentMode != CONVEX_HULL ) {

        triangulation.setPoints(points);
        triangulation.build(true);
        currentMode = TRIANGULAR;

    } else {
        triangulation.convertToDelaunay();
    }

    repaint();
}

void CreateScene::generatePoints() {
    if (currentMode != ADD_POINTS) { return; }

    for (int i = 0; i < POINTS_NUMBER; i++) {
        int x = std::rand() * std::rand() % width();
        int y = std::rand() * std::rand() % height();
        points.append(QPointF(x, y));
    }
    repaint();
}

void CreateScene::clear() {
    points.clear();
    triangulation.clear();

    currentMode = ADD_POINTS;
    repaint();
}

CreateScene::~CreateScene(){}
