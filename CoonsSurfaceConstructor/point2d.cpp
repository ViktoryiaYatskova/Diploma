#include <QPointF>

int QPointF::DIMENSION = 2;

QPointF::QPointF() {}

QPointF::QPointF(const QPoint& p) {
    x = p.x();
    y = p.y();
}


double QPointF::getX() const
{
    return x;
}

void QPointF::setX(double value)
{
    x = value;
}
double QPointF::getY() const
{
    return y;
}

void QPointF::setY(double value)
{
    y = value;
}



