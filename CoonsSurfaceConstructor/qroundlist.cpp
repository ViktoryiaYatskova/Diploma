#include "qroundlist.h"
#include "qlist.h"
#include "QPoint.h"

QRoundList::QRoundList(){
    startIndex = 0;
}

QRoundList::QRoundList(QList<Point> &list){
    startIndex = 0;
    for(int i = 0; i < list.length(); i++)
    {
        push_back(list[i]);
    }
}

Point QRoundList::preLast()
{
    int lastIndex = length() - startIndex - 1;
    return get(lastIndex - 1);
}

Point QRoundList::last()
{
    int lastIndex = length() - startIndex - 1;
    return get(lastIndex);
}

Point QRoundList::next(int i) {
    return get(this->nextIndex(i));
}

Point QRoundList::get(int index) {
    return at((index + startIndex+length()) % length());
}

int QRoundList::getStartIndex() const {
    return startIndex;
}

Point QRoundList::popBack(){
    Point last = this->last();
    pop_back();
    return last;
}

int QRoundList::nextIndex(int index) const
{
    return (index+1)%(length());
}

int QRoundList::prevIndex(int index) const
{
    return (index-1+length())%(length());
}

int QRoundList::getLastIndex() const
{
    return length()-startIndex-1;
}

void QRoundList::setStartIndex(int value)
{
    startIndex = value;
}

void QRoundList::incStartIndex()
{
    startIndex++;
}

void QRoundList::decStartIndex()
{
    startIndex = (startIndex+length())%length();
}
