#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QQueue>

template <class T>
class CircularBuffer : private QQueue<T>
{
public:
    explicit CircularBuffer(unsigned int size = 10) : maxSize(size) {}
    ~CircularBuffer() {}
    // Interface
    void resize(unsigned int size) { maxSize = size; deleteExcess(); }
    // Overrride
    void append(const T &t)         { int i = QQueue<T>::indexOf(t);
                                      if (i>=0) { QQueue<T>::takeAt(i); QQueue<T>::append(t); }
                                      else { QQueue<T>::append(t); deleteExcess(); }}

    void enqueue(const T &t)        { append(t); }
    void push_back(const T &t)      { append(t); }

public:
    using QQueue<T>::clear;
    using typename QQueue<T>::const_iterator;
    using QQueue<T>::cbegin;
    using QQueue<T>::cend;

private:
    inline void deleteExcess() { while (QQueue<T>::count() > (int) maxSize) QQueue<T>::pop_front(); }

private:
    unsigned int maxSize;
};

#endif // CIRCULARBUFFER_H
