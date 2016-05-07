#include "qlist.h"
#include "definitions.h"

#ifndef QROUNDLIST_H
#define QROUNDLIST_H

class QRoundList:public QList<Point>
{
public:
    QRoundList();
    QRoundList(QList<Point>& list);
    Point preLast();
    Point last();
    Point next(int i);
    Point get(int i);
    Point popBack();

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
