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
#include <QPointF>

enum MODE {
    ADD_POINTS,
    TRIANGULAR,
    CONVEX_HULL
};

class CreateScene: public QGLWidget {

public:
    CreateScene(QWidget *parent = 0);

    void buildSimpleTriangular();
    void clear();

    void showConvexHull();
    void drawConvexHull();
    void convertToDelaunayTriangular();
protected:
    void mouseReleaseEvent(QMouseEvent*);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    void draw();
    void drawTriangular();
    void drawEdges(QList<Edge> edges);

    void drawPoints(QList<QPointF>);
    void drawPoints();

    ~CreateScene();

    MODE currentMode;
    QPoint lastPos;

    const QColor usualPointColor;
    const QColor hullPointColor;

    QVector<QPointF> points;
    DelaunayTriangulation triangulation;
    GrahamTriangulation grahamScan;
};

#endif // CREATESCENE_H
