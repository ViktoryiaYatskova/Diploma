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
#include "surface.h"

enum MODE {
    ADD_POINTS,
    TRIANGULAR,
    CONVEX_HULL,
    SURFACE
};

class CreateScene : public QGLWidget {
    Q_OBJECT
public:
    explicit CreateScene(QWidget *parent = 0);

    void buildSimpleTriangular();
    void buildSurface();
    void clear();

    void showConvexHull();
    void drawConvexHull();
    void convertToDelaunayTriangular();
    void generatePoints();

    ~CreateScene();

    void drawSurface();
protected:
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent *);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    void setupViewport(int width, int height);
    void draw();
    void drawTriangular();
    void drawEdges(QSet<Edge>& edges);

    void drawPoints(QVector<Point>&);
    void drawPoints(QList<Point>&);
    void drawPoints();

    MODE currentMode;
    Point lastPos;

    const QColor usualPointColor;
    const QColor hullPointColor;

    QVector<Point> points;
    DelaunayTriangulation triangulation;
    GrahamTriangulation grahamScan;
    Surface surface;

    static const int POINTS_NUMBER;
    const double MIN_X;
    const double MIN_Y;
    const double MIN_Z;

    const double MAX_X;
    const double MAX_Y;
    const double MAX_Z;

    const int MAX_ROTATOR_VALUE;
    const int ROTATOR_STEP;

    const int MAX_WHEEL;
    const int MIN_WHEEL;

    int xRot;
    int yRot;
    int zRot;

    double scaling;

    Point& toOpenGLPoint(Point &p);
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
