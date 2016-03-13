#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QQueue>

template <class T>
class CircularBuffer : public QQueue<T>
{
public:
    explicit inline CircularBuffer(unsigned int size = 10) : maxSize(size) {}
    inline ~CircularBuffer() {}
    // Interface
    inline void resize(unsigned int size) { maxSize = size; deleteExcess(); }
    // Technical
    inline void swap(CircularBuffer<T> &other) { QQueue<T>::swap(other); } // prevent CircularBuffer<->QQueue swaps
    // Overrride
    inline void enqueue(const T &t) { QQueue<T>::enqueue(t); deleteExcess(); }
    inline void append(const T &t)         { QQueue<T>::append(t); deleteExcess(); }
    inline void append(const QList<T> &t)  { QQueue<T>::append(t); deleteExcess(); }
    inline void prepend(const T &t)        { QQueue<T>::prepend(t); deleteExcess(); }
    inline void insert(int i, const T &t)  { QQueue<T>::insert(i,t); deleteExcess(); }
    inline void push_back(const T &t)      { append(t); }
    inline void push_front(const T &t)     { prepend(t); }
protected:
    inline void deleteExcess() { while (QQueue<T>::count() > (int) maxSize) QQueue<T>::pop_front();
                                 if (QQueue<T>::count())
                                 {
                                    T t = QQueue<T>::last();
                                    int i = QQueue<T>::indexOf(t);
                                    int j = QQueue<T>::lastIndexOf(t);
                                    if (i!=j) QQueue<T>::takeAt(i);
                                 }
                               }
private:
    unsigned int maxSize;
};

#endif // CIRCULARBUFFER_H
