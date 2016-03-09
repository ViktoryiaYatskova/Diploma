#include "createscene.h"

CreateScene::CreateScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setAutoFillBackground(false);

    currentMode = ADD_POINTS;
}

void CreateScene::draw() {

    switch (currentMode) {

    case ADD_POINTS:
        drawPoints();
        break;

    case TRIANGULAR:
        drawTriangularPoints();
        break;

    default:
        break;
    }
}

void CreateScene::initializeGL(){

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
            glVertex2f( p.x(), height() - p.y());
        }
    glEnd();
}

void CreateScene::drawEdges(QList<Edge> edges) {
    glLineWidth(1.0);

    QListIterator<Edge> i(edges);

    while (i.hasNext()) {
        Edge e = i.next();

        glBegin(GL_LINES);
            glVertex2f(e.getStartPoint().x(),height() - e.getStartPoint().y());
            glVertex2f(e.getEndPoint().x(), height() - e.getEndPoint().y());
        glEnd();
    }
}

void CreateScene::
    drawPoints() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPointSize(3.0);

    glColor3f(0.5f, 0.0f, 0.5f);
    drawPoints(points.toList());
}

void CreateScene::drawTriangularPoints() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPointSize(3.0);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawPoints(points.toList());

        glColor3f(0.0f, 0.5f, 0.7f);
        drawPoints(triangulation.inscribedCircleCenters.toList());

    glColor3f(1.0f, 1.0f, 1.0f);
    drawEdges(triangulation.getEdges().toList());
}

void CreateScene::mouseReleaseEvent(QMouseEvent *event) {
    QPoint lastPos = event->pos();

    if (currentMode == ADD_POINTS) {
        points.append(QPointF(lastPos));
        repaint();
    }
}

void CreateScene::buildTriangular() {
    triangulation.setPoints(points);
    triangulation.build();

    currentMode = TRIANGULAR;
    //grahamScan.setPoints(points.toList());
    //grahamScan.prebuild();
    //grahamScan.build();
    repaint();
}

void CreateScene::clear() {
    points.clear();
    triangulation.clear();

    currentMode = ADD_POINTS;
    repaint();
}

CreateScene::~CreateScene(){}
