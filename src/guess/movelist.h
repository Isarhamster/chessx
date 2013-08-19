//////////////////////////////////////////////////////////////////////
//
//  FILE:       movelist.h
//              MoveList class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 1999-2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_MOVELIST_H
#define SCID_MOVELIST_H

#include "common.h"

namespace Guess
{

//////////////////////////////////////////////////////////////////////
//  MoveList:  Constants

const unsigned int  MAX_LEGAL_MOVES = 256;  // max. length of the moves list


///////////////////////////////////////////////////////////////////////////
//  MoveList:  Data Structures

// *** SimpleMove: less expensive to store than a full move as defined
//      in game.h, but still fully undoable.
//
struct simpleMoveT
{
    unsigned char     pieceNum;
    pieceT   movingPiece;
    squareT  from;
    squareT  to;
    unsigned char     capturedNum;
    pieceT   capturedPiece;
    pieceT   promote;
    squareT  capturedSquare; // ONLY different to "to" field if this capture
                            //    is an en passant capture.
    unsigned char     castleFlags;    // pre-move information
    squareT  epSquare;       // pre-move information
    unsigned short   oldHalfMoveClock;
    int      score;          // used for alpha/beta ordering.
};

inline bool isNullMove (simpleMoveT * sm)
{
    return (sm->from == sm->to  &&  sm->from != NULL_SQUARE
              &&  piece_Type(sm->movingPiece) == KING);
}

errorT writeSimpleMove (FILE * fp, simpleMoveT * sm);
errorT readSimpleMove (FILE * fp, simpleMoveT * sm);


class MoveList
{
private:
    unsigned int ListSize;
    simpleMoveT Moves [MAX_LEGAL_MOVES];

public:
    MoveList() { ListSize = 0; }
    ~MoveList() {}

    inline unsigned int Size() { return ListSize; }
    inline void Clear() { ListSize = 0; }
    inline void Add (simpleMoveT * sm);
    inline simpleMoveT * Add ();
    inline simpleMoveT * Get (unsigned int index);
    inline void Remove (unsigned int index);

    void MoveToFront (unsigned int index);
    void SwapWithFirst (unsigned int index);
    int  Find (simpleMoveT * sm);
    void FindBest (unsigned int index);
    void Sort (void);
    bool IsSorted (void);
    unsigned int SelectBySquare (squareT sq);
    unsigned int SelectBySquares(squareT f1, squareT t1, squareT f2, squareT t2);
};

inline void
MoveList::Add (simpleMoveT * sm)
{
    ASSERT (sm != NULL);
    ASSERT (ListSize < MAX_LEGAL_MOVES);
    Moves[ListSize++] = *sm;
}

inline simpleMoveT *
MoveList::Add (void)
{
    ASSERT (ListSize < MAX_LEGAL_MOVES);
    simpleMoveT * sm = &(Moves[ListSize]);
    ListSize++;
    return sm;
}

inline simpleMoveT *
MoveList::Get (unsigned int index)
{
    ASSERT (index < ListSize);
    return &(Moves[index]);
}

inline void
MoveList::Remove (unsigned int index)
{
    ASSERT (index < ListSize);
    ListSize--;
    if (index != ListSize) {
        Moves[index] = Moves[ListSize];
    }
}

} // End namespace Guess

#endif  // SCID_MOVELIST_H

//////////////////////////////////////////////////////////////////////
//  EOF: movelist.h
//////////////////////////////////////////////////////////////////////
