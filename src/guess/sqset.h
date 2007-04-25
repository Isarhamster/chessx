//////////////////////////////////////////////////////////////////////
//
//  FILE:       sqset.h
//              SquareSet class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_SQSET_H
#define SCID_SQSET_H

#include "common.h"

namespace Guess
{

class SquareSet {
private:

    uint Bits_a1h4;
    uint Bits_a5h8;

public:

    SquareSet() { Bits_a1h4 = Bits_a5h8 = 0; }
    SquareSet(squareT * squares) {
        Bits_a1h4 = Bits_a5h8 = 0;
        AddAll (squares);
    }
    ~SquareSet() {}

    inline void Clear (void) { Bits_a1h4 = Bits_a5h8 = 0; }
    inline void Add (squareT sq);
    inline void AddAll (void) { Bits_a1h4 = Bits_a5h8 = 0xFFFFFFFFu; }
    inline void AddAll (squareT * squares);
    inline bool Contains (squareT sq);
    inline void Remove (squareT sq);
};

inline void
SquareSet::Add (squareT sq)
{
    ASSERT (sq <= H8);
    if (sq <= H4) {
        Bits_a1h4 |= (1 << sq);
    } else {
        Bits_a5h8 |= (1 << (sq & 31));
    }
}

inline void
SquareSet::AddAll (squareT * squares)
{
    while (true) {
        squareT sq = *squares;
        if (sq == NULL_SQUARE) { break; }
        ASSERT (sq <= H8);
        squares++;
        if (sq <= H4) {
            Bits_a1h4 |= (1 << sq);
        } else {
            Bits_a5h8 |= (1 << (sq & 31));
        }
    }
}

inline bool
SquareSet::Contains (squareT sq)
{
    ASSERT (sq <= H8);
    if (sq <= H4) {
        return (Bits_a1h4 & (1 << sq)) != 0;
    } else {
        return (Bits_a5h8 & (1 << (sq & 31))) != 0;
    }
}

inline void
SquareSet::Remove (squareT sq)
{
    ASSERT (sq <= H8);
    if (sq <= H4) {
        Bits_a1h4 &= ~(1 << sq);
    } else {
        Bits_a5h8 &= ~(1 << (sq & 31));
    }
}

} // End namespace Guess

#endif  // SCID_SQSET_H

//////////////////////////////////////////////////////////////////////
//  EOF: sqset.h
//////////////////////////////////////////////////////////////////////
