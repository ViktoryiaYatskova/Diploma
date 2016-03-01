#include "createscene.h"

CreateScene::CreateScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setAutoFillBackground(false);

    currentMode = MODE::ADD_POINTS;
    triangulation = NULL;
}

void CreateScene::draw() {
    switch (currentMode) {

    case MODE::ADD_POINTS:
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
                0 + height);     /*win_offset_y+win_height*/
}

void CreateScene::paintGL() {
    drawTriangularPoints();
}

void CreateScene::drawPoints(Vector < Point2D > & points) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glPointSize(3.0);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POINTS);

    QVectorIterator < Point2D > i(points);
    while (i.hasNext()) {
        Point2D p = i.next();
        glVertex2f( p.x(), height() - p.y());
    }

    glEnd();
}

void CreateScene::drawTriangularPoints() {
    drawPoints(points);
}

void CreateScene::mouseReleaseEvent(QMouseEvent *event) {
    QPoint lastPos = event->pos();

    if (currentMode == MODE::ADD_POINTS) {
        points.append(Point2D(lastPos));
        repaint();
    }
}

void CreateScene::buildTriangular() {
    triangulation = new DelaunayTriangulation(points);
}

CreateScene::~CreateScene(){}
