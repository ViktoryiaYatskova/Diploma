#ifndef CREATESCENE_H
#define CREATESCENE_H

#include <windows.h>
#include <gl/gl.h>
#include "gl/GLU.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QDebug>

#include "delaunaytriangulation.h"
#include "grahamTriangulation.h"
#include <QVector>
#include <QList>
#include "definitions.h"

enum MODE {
    ADD_POINTS,
    TRIANGULAR,
    CONVEX_HULL
};

class CreateScene : public QGLWidget {
    Q_OBJECT
public:
    explicit CreateScene(QWidget *parent = 0);

    void buildSimpleTriangular();
    void clear();

    void showConvexHull();
    void drawConvexHull();
    void convertToDelaunayTriangular();
    void generatePoints();

    ~CreateScene();

protected:
    void mouseReleaseEvent(QMouseEvent*);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    void setupViewport(int width, int height);
    void draw();
    void drawTriangular();
    void drawEdges(QSet<Edge>& edges);

    void drawPoints(QVector<Point>&);
    void drawPoints();

    MODE currentMode;
    Point lastPos;

    const QColor usualPointColor;
    const QColor hullPointColor;

    QVector<Point> points;
    DelaunayTriangulation triangulation;
    GrahamTriangulation grahamScan;

    static const int POINTS_NUMBER;
    const float MIN_X;
    const float MIN_Y;
    const float MIN_Z;

    const float MAX_X;
    const float MAX_Y;
    const float MAX_Z;

    const int MAX_ROTATOR_VALUE;
    const int ROTATOR_STEP;

    int xRot;
    int yRot;
    int zRot;

    Point& toOpenGLPoint(Point &p);
    Point& addZCoordinate(Point &p);
    void qNormalizeAngle(int &angle);
public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
};

#endif // CREATESCENE_H
