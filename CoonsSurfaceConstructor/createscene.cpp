#include "createscene.h"
#include <QtWidgets>
#include <QtOpenGL>

const int CreateScene::POINTS_NUMBER = 25;

CreateScene::CreateScene(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    MIN_X (-2),
    MIN_Y (-2),
    MIN_Z (-20),
    MAX_X (2),
    MAX_Y (2),
    MAX_Z (20),
    MAX_ROTATOR_VALUE(360),
    ROTATOR_STEP(16),
    MAX_WHEEL(8),
    MIN_WHEEL(-8),
    surface(triangulation) {

    setAutoFillBackground(false);

    currentMode = ADD_POINTS;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    scaling = 1;
    showTriangleNet = false;
}

void CreateScene::draw() {
    qglColor(Qt::red);

    switch (currentMode) {

    case ADD_POINTS:
        glPointSize(3.0);
        glColor3f(0.5f, 0.0f, 0.5f);
        drawPoints();
        break;

    case TRIANGULAR:
        drawTriangular();
        break;

    case CONVEX_HULL:
        drawTriangular();
        drawConvexHull();
        drawNormals();
        drawTangents();
        drawTrianglesNormals();
        break;

    case SURFACE:
        //drawTriangular();
        drawSurface();
        if (CoonsPatches::SHOW_ADDITIONAL_VECTORS) {
            drawNormals();
            drawTangents();
            drawTrianglesNormals();
        }
        break;

    default:
        break;
    }
}

void CreateScene::initializeGL(){
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    qglClearColor(Qt::black);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    initLights();
}

void CreateScene::initLights() {
   GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat position[] = {0.5, 0.5, 1.5, 1.0};
   GLfloat matDiffuseFront[] = {0.5, 0.5, 0.5, 1.0};
   GLfloat matDiffuseBack[] = {0.7, 0.7, 0.7, 1.0};
   GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat matShininess[] = {40.0};

   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_POSITION, position);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseFront);
   glMaterialfv(GL_BACK, GL_DIFFUSE, matDiffuseBack);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
}

void CreateScene::resizeGL(int width, int height){
    setupViewport(width, height);
}

void CreateScene::
    paintGL() {

    setupViewport(width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //glTranslatef(0.0, 0.0, -10.0);

    glScalef(scaling, scaling, scaling);
    glRotatef(xRot / ROTATOR_STEP, 1.0, 0.0, 0.0);
    glRotatef(yRot / ROTATOR_STEP, 0.0, 1.0, 0.0);
    glRotatef(zRot / ROTATOR_STEP, 0.0, 0.0, 1.0);

    draw();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void CreateScene::
    drawPoints(QVector<Point>& points) {

    glBegin(GL_POINTS);
        QVectorIterator<Point> i(points);
        while (i.hasNext()) {
            Point p = i.next();
            glVertex3f( p.x(), p.y(), p.z());
        }
    glEnd();
}

void CreateScene::
    drawPoints(QList<Point>& points) {

    glBegin(GL_POINTS);
        QListIterator<Point> i(points);
        while (i.hasNext()) {
            Point p = i.next();
            glVertex3f( p.x(), p.y(), p.z());
        }
    glEnd();
}

void CreateScene::
    drawEdges(QSet<Edge>& edges) {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0);

    QSetIterator<Edge> i(edges);

    while (i.hasNext()) {
        Edge e = i.next();

        glBegin(GL_LINES);
            glVertex3f(e.getStartPoint().x(), e.getStartPoint().y(), e.getStartPoint().z());
            glVertex3f(e.getEndPoint().x(), e.getEndPoint().y(), e.getEndPoint().z());
        glEnd();
    }

    glPopAttrib();
}

void CreateScene::
    setupViewport(int width, int height) {

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES
    glOrthof(MIN_X, MAX_X, MIN_Y, MAX_Y, MIN_Z, MAX_Z);
#else
    glOrtho(MIN_X, MAX_X, MIN_Y, MAX_Y, MIN_Z, MAX_Z);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void CreateScene::
    drawPoints() {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_POINT_SMOOTH);

    drawPoints(points);

    glPopAttrib();
}

void CreateScene::
    drawTriangular() {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_POINT_SMOOTH);

    glPointSize(3.0);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawPoints(points);

    //glColor3f(0.0f, 0.5f, 0.7f);
    //drawPoints(triangulation.inscribedCircleCenters);

    glColor3f(1.0f, 1.0f, 1.0f);
    QSet<Edge> edges = triangulation.getEdges();
    drawEdges(edges);

    glPopAttrib();
}

void CreateScene::
    drawTrianglesNormals() {

    glEnable(GL_LINE_SMOOTH);

    glLineWidth(1.2);
    glColor3f(0.1f, 0.8f, 0.1f);

    surface.drawTrianglesNormals();
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

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    glPointSize(2.5);
    glLineWidth(2.0);
    glColor3f(1.0f, 0.0f, 1.0f);

    glLineStipple(2, 0xAAAA);

    QVectorIterator<Point> i(triangulation.getConvexHull());
    Point first = i.next(), p1 = first, p2;

    glBegin(GL_LINES);
        while (i.hasNext()) {
            p2 = i.next();

            glVertex3f(p1.x(), p1.y(), p1.z());
            glVertex3f(p2.x(), p2.y(), p2.z());

            p1 = p2;
        }
        p2 = first;
        glVertex3f(p1.x(), p1.y(), p1.z());
        glVertex3f(p2.x(), p2.y(), p2.z());
    glEnd();
    glPopAttrib();
}

void CreateScene::
    mouseReleaseEvent(QMouseEvent *event) {

    Point lastPoint(event->pos());

    if (currentMode == ADD_POINTS) {
        if(points.contains(lastPoint)) { return; }
        points.append(CoonsTriangularSurface::setZCoordinate(toOpenGLPoint(lastPoint)));

        updateGL();
    }
}

void CreateScene::
    wheelEvent(QWheelEvent *event) {

    double delta = event->delta()/120;
    delta = std::min(MAX_WHEEL, event->delta()/120);
    delta = std::max((double)MIN_WHEEL, delta);

    scaling += delta/(double)MAX_WHEEL;
    scaling = std::max(0.0, scaling);
    scaling = std::min(2.0, scaling);

    updateGL();
}

Point& CreateScene::
    toOpenGLPoint(Point& p) {

    p.setX(p.x() / (double) width()
               * (MAX_X - MIN_X) + MIN_X);
    p.setY((1 - p.y() / (double) height())
               * (MAX_Y - MIN_Y) + MIN_Y);
    return p;
}

void CreateScene::
    buildSimpleTriangular() {

    if (points.length() < 3) return;

    triangulation.setPoints(points);
    triangulation.build(false);
    setCursor(Qt::ArrowCursor);

    currentMode = TRIANGULAR;
    repaint();
}

void CreateScene::
    buildSurface() {

    if (currentMode == TRIANGULAR ||
        currentMode == CONVEX_HULL ) {

        currentMode = SURFACE;        
        surface.build();
    }
    showTriangleNet = false;
    repaint();
}

void CreateScene::
    buildTriangularNet() {

    if (currentMode == TRIANGULAR ||
        currentMode == CONVEX_HULL ) {

        currentMode = SURFACE;
        surface.build();
    }
    showTriangleNet = true;
    repaint();
}

void CreateScene::
    drawSurface() {

    glPushAttrib(GL_ENABLE_BIT);

    glColor3f(0.1f, 0.6f, 0.8f);
    QVectorIterator<CoonsTriangularSurface> it(surface);

    while(it.hasNext()) {
        if (showTriangleNet) {
            it.next().drawTriangularNet();
        } else {
            it.next().draw();
        }
    }

    glPopAttrib();
}

void CreateScene::
    drawNormals() {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_LINE_SMOOTH);

    glLineWidth(0.9);

    glColor3f(0.9f, 0.1f, 0.1f);
    QVectorIterator<CoonsTriangularSurface> it(surface);
    while(it.hasNext()) {
        it.next().drawNormals();
    }

    glPopAttrib();
}

void CreateScene::
    drawTangents() {

    glPushAttrib(GL_ENABLE_BIT);

    glEnable(GL_LINE_SMOOTH);

    glLineWidth(0.9);

    glColor3f(8.f, 0.f, 0.5f);
    QVectorIterator<CoonsTriangularSurface> it(surface);
    while(it.hasNext()) {
        it.next().drawTangents();
    }

    glPopAttrib();
}

void CreateScene::
    convertToDelaunayTriangular() {

    if (points.length() < 3) return;

    if (currentMode != TRIANGULAR &&
        currentMode != CONVEX_HULL ) {

        triangulation.setPoints(points);
        triangulation.build(true);
        currentMode = TRIANGULAR;
        setCursor(Qt::ArrowCursor);
    } else {
        triangulation.convertToDelaunay();
    }

    repaint();
}

void CreateScene::
    generatePoints() {

    if (currentMode != ADD_POINTS) { return; }

    const int OFFSET = width() / 20;
    for (int i = 0; i < POINTS_NUMBER; i++) {
        Point p;
        p.setX((std::rand() * std::rand() + OFFSET) % (width() - OFFSET));
        p.setY((std::rand() * std::rand() + OFFSET) % (height() - OFFSET));
        points.append(CoonsTriangularSurface::setZCoordinate(toOpenGLPoint(p)));
    }
    repaint();
}

void CreateScene::clear() {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    scaling = 1;
    points.clear();
    surface.clear();

    emit xRotationChanged(0);
    emit yRotationChanged(0);
    emit zRotationChanged(0);

    currentMode = ADD_POINTS;
    setCursor(Qt::PointingHandCursor);
    repaint();
}

void CreateScene::
    qNormalizeAngle(int &angle) {

    angle %= MAX_ROTATOR_VALUE;
    angle *= ROTATOR_STEP;
    /*while (angle < 0)
        angle += MAX_ROTATOR_VALUE  * ROTATOR_STEP;
    while (angle > MAX_ROTATOR_VALUE)
        angle -= MAX_ROTATOR_VALUE * ROTATOR_STEP;*/
}

void CreateScene::
    setXRotation(int angle) {

    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        //emit xRotationChanged(angle);
        updateGL();
    }
}

void CreateScene::
    setYRotation(int angle) {

    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        //emit yRotationChanged(angle);
        updateGL();
    }
}

void CreateScene::
    setZRotation(int angle) {

    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        //emit zRotationChanged(angle);
        updateGL();
    }
}

CreateScene::~CreateScene(){}
