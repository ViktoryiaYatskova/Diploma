#include "vector.h"

Vector::
    Vector():QVector3D(){
}

Vector::Vector(const Vector &p):QVector3D(p){
}

Vector::Vector(const QVector3D &p):QVector3D(p){
}

Vector::Vector(const QPoint &p):QVector3D(p){
}

Vector::Vector(double p1, double p2, double p3):QVector3D(p1, p2, p3) {
}

