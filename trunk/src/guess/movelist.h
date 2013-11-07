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
#include <QList>

namespace Guess
{

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

inline bool isNullMove(simpleMoveT * sm)
{
    return (sm->from == sm->to  &&  sm->from != NULL_SQUARE
            &&  piece_Type(sm->movingPiece) == KING);
}

errorT writeSimpleMove(FILE * fp, simpleMoveT * sm);
errorT readSimpleMove(FILE * fp, simpleMoveT * sm);


class MoveList : public QList<simpleMoveT>
{
public:
    MoveList() : QList<simpleMoveT>() {}
    ~MoveList() { }

    inline simpleMoveT * Get(unsigned int index) { return &this->operator [](index); }

    void MoveToFront(unsigned int index);
    void SwapWithFirst(unsigned int index);
    int  Find(const simpleMoveT * sm) const;
    void FindBest(unsigned int index);
    void Sort(void);
    bool IsSorted() const;
    unsigned int SelectBySquare(squareT sq);
    unsigned int SelectBySquares(squareT f1, squareT t1, squareT f2, squareT t2);
};

} // End namespace Guess

#endif  // SCID_MOVELIST_H

//////////////////////////////////////////////////////////////////////
//  EOF: movelist.h
//////////////////////////////////////////////////////////////////////
