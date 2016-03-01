#ifndef CREATESCENE_H
#define CREATESCENE_H

#include <windows.h>
#include <gl/gl.h>
#include "gl/GLU.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QDebug>

#include "delaunaytriangulation.h"
#include "vector.h"
#include "point2d.h"

class CreateScene: public QGLWidget {

public:
    CreateScene(QWidget *parent = 0);

    void buildTriangular();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    void draw();
    void drawTriangularPoints();

    void drawPoints(Vector < Point2D > & points);
    ~CreateScene();

    enum MODE {
        ADD_POINTS,
        TRIANGULAR
    };

    MODE currentMode;
    QPoint lastPos;

    Vector < Point2D > points;
    DelaunayTriangulation* triangulation;
};

#endif // CREATESCENE_H
