#include "vector.h"

Vector::Vector() {}

Vector::Vector(T* itemsArray, int length) {
    appendItems(itemsArray, length);
}

void Vector::appendItems(T* itemsArray, int length) {

    for (int i = 0; i < length; i++) {
        append(itemsArray[i]);
    }
}

void Vector::appendItems( Vector <T>& items) {
    appendItems(items.data(), items.length());
}

