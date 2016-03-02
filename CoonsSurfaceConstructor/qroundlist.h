#include "qlist.h"
#include "QPoint.h"

#ifndef QROUNDLIST_H
#define QROUNDLIST_H

class QRoundList:public QList<QPointF>
{
public:
    QRoundList();
    QRoundList(QList<QPointF>& list);
    QPointF preLast();
    QPointF last();
    QPointF next(int i);
    QPointF get(int i);
    QPointF popBack();

    int getStartIndex() const;
    int getLastIndex() const;
    int nextIndex(int index) const;
    int prevIndex(int index) const;
    void setStartIndex(int value);
    void incStartIndex();
    void decStartIndex();

private:
    int startIndex;
};

#endif // QROUNDLIST_H
