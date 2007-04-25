//////////////////////////////////////////////////////////////////////
//
//  FILE:       sqlist.h
//              SquareList class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_SQLIST_H
#define SCID_SQLIST_H

#include "common.h"

namespace Guess
{

const uint MAX_SQUARELIST = 65;  // 64 squares plus null square

class SquareList {
private:

    uint ListSize;
    squareT Squares [MAX_SQUARELIST];

public:

    SquareList() { ListSize = 0; }
    ~SquareList() {}

    inline void Init() { ListSize = 0; }
    inline void Clear() { ListSize = 0; }
    inline void Add(squareT sq) { Squares[ListSize] = sq; ListSize++; }
    inline uint Size() { return ListSize; }
    inline squareT Get (uint index);
    inline bool Contains (squareT sq);
    inline void Remove (uint index);
};

inline squareT
SquareList::Get (uint index)
{
    ASSERT (index < ListSize);
    return Squares[index];
}

inline bool
SquareList::Contains (squareT sq)
{
    for (uint i=0; i < ListSize; i++) {
        if (Squares[i] == sq) { return true; }
    }
    return false;
}

inline void
SquareList::Remove (uint index)
{
    ASSERT (index < ListSize);
    ListSize--;
    if (index != ListSize) {
        Squares[index] = Squares[ListSize];
    }
}

} // End namespace Guess

#endif  // SCID_SQLIST_H

//////////////////////////////////////////////////////////////////////
//  EOF: sqlist.h
//////////////////////////////////////////////////////////////////////
