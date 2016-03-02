#include "createscene.h"

#include <iostream>

void consoleLog(char* error) {
    std::cout<<error<<std::endl;
}

void consoleLog(int error) {
    std::cout<<error<<std::endl;
}

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

    qglClearColor(Qt::red);

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
    drawTriangularPoints();
}

void CreateScene::drawPoints(QList<QPointF> &points) {
    glBegin(GL_POINTS);

    QListIterator<QPointF> i(points);
    while (i.hasNext()) {
        QPointF p = i.next();
        glVertex2f( p.x(), height() - p.y());
    }

    glEnd();
}

void CreateScene::drawEdges(QList<Edge> &edges) {
    glBegin(GL_LINES);

    QListIterator<Edge> i(edges);

    while (i.hasNext()) {
        Edge e = i.next();
                consoleLog(e.getStartPoint().x());
                consoleLog(e.getStartPoint().y());
        glVertex2f( e.getStartPoint().x(), height() - e.getStartPoint().y());
        glVertex2f( e.getEndPoint().x(), height() - e.getEndPoint().y());
    }

    glEnd();
}

void CreateScene::drawTriangularPoints() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glPointSize(3.0);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.5);

    drawPoints(points.toList());
    QList<Edge> edges;

    if(grahamScan.isSorted()){
        drawPoints(grahamScan.getHull());
        edges = grahamScan.getEdgesPreOrder();
    }

    if(grahamScan.isBuilted()){
        edges = grahamScan.getHullEdges();
    }

    drawEdges(edges);
}

void CreateScene::mouseReleaseEvent(QMouseEvent *event) {
    QPoint lastPos = event->pos();

    if (currentMode == MODE::ADD_POINTS) {
        points.append(QPointF(lastPos));
        repaint();
    }
}

void CreateScene::buildTriangular() {
    triangulation = new DelaunayTriangulation(points);
    grahamScan.setPoints(points.toList());
    grahamScan.prebuild();
    repaint();
    grahamScan.build();
    repaint();
}

void CreateScene::clear() {
    grahamScan.clear();
    repaint();
}

CreateScene::~CreateScene(){}
