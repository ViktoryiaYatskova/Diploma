#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <QVector>

using namespace std;

template <typename T>
class Vector: public QVector <T> {

public:
    Vector();
    Vector(T*, int);
    void appendItems( T *itemsArray, int length );
    void appendItems( Vector <T>& );
};

#endif // VECTOR_H
