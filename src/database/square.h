/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef SQUARE_H
#define SQUARE_H

#include <QtCore>
class QDataStream;

enum SquareNames
#ifdef USE_C11
: unsigned char
#endif
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NumSquares,
    InvalidSquare = 255,
    NoEPSquare = 255
};

#ifdef USE_C11
typedef enum SquareNames Square;
#else
typedef unsigned char Square;
#endif

inline Square SquareFromRankAndFile(unsigned char rank, unsigned char file)
{
    return Square(rank*8+file);
}

#ifdef USE_C11

inline Square operator+(Square square, int offset)
{
    return Square((unsigned char)square+offset);
}

inline Square operator-(Square square, int offset)
{
    return Square((unsigned char)square-offset);
}

inline Square& operator+=(Square& square,int offset)
{
    square = Square((unsigned char)square + offset);
    return square;
}

// Pre-Increment
inline Square& operator++(Square& w)
{
    if(w < h8)
    {
        return w = Square(w + 1);
    }
    else
    {
        return w = InvalidSquare;
    }
}

// Pre-Decrement
inline Square& operator--(Square& w)
{
    if(w > a1)
    {
        return w = Square(w - 1);
    }
    else
    {
        return w = InvalidSquare;
    }
}

// Post-Increment
inline Square operator++(Square& w, int)
{
    Square square = w;
    ++w;
    return square;
}

// Post-Decrement
inline Square operator--(Square& w, int)
{
    Square square = w;
    --w;
    return square;
}
inline QDataStream& operator<<(QDataStream& s, Square square)
{
    s << quint32(square);
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Square& square)
{
    quint32 tmp;
    s >> tmp;
    square = Square(tmp);
    return s;
}

#endif // USE_C11

#endif // SQUARE_H

