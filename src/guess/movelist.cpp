//////////////////////////////////////////////////////////////////////
//
//  FILE:       movelist.cpp
//              MoveList class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "movelist.h"
#include "misc.h"

namespace Guess
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::MoveToFront
//   Promotes a move to the front of the list, pushing
//   all moves that were ahead of it down one place.
void
MoveList::MoveToFront (uint index)
{
    ASSERT (index < ListSize);
    if (index == 0) { return; }
    simpleMoveT smTemp = Moves[index];
    for (int i = index; i > 0; i--) {
        Moves[i] = Moves[i-1];
    }
    Moves[0] = smTemp;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SwapWithFirst
//   Swaps the specified move with the move at the start of the list.
//   All other moves remain in their original location.
void
MoveList::SwapWithFirst (uint index)
{
    ASSERT (index < ListSize);
    if (index == 0) { return; }
    simpleMoveT smTemp = Moves[index];
    Moves[index] = Moves[0];
    Moves[0] = smTemp;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::Find
//   Searches for a move in the list.
//   Returns the index of the move with the same from-square,
//   to-square and promotion-piece values as the specified
//   move, if it is in the list. If the move is not found,
//   the value -1 is returned.
int
MoveList::Find (simpleMoveT * sm)
{
    for (uint i=0; i < ListSize; ++i) {
        simpleMoveT * lsm = &(Moves[i]);
        if (sm->from == lsm->from  &&  sm->to == lsm->to
                &&  sm->promote == lsm->promote) {
            return (int) i;
        }
    }
    return -1;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SelectBySquares
//   Restricts the list to only contain only the two
//   moves suggested by (from1,to1) and (from2,to2)
//   Returns the new size of the list.
uint
MoveList::SelectBySquares (squareT f1, squareT t1, squareT f2, squareT t2)
{
    uint index = 0;
    for (uint i=0; i < ListSize; ++i) {
        simpleMoveT * sm = &(Moves[i]);
	if ((sm->from == f1 && sm->to == t1) || (sm->from == f2 && sm->to == t2)) {
		if (i != index)
		    Moves[index] = *sm;
		index++;
	}
    }
    ListSize = index;
    return ListSize;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SelectBySquare
//   Restricts the list to only contain moves which
//   move a piece to or from the specified square.
//   Returns the new size of the list.
uint
MoveList::SelectBySquare (squareT sq)
{
    uint index = 0;
    for (uint i=0; i < ListSize; i++) {
        simpleMoveT * sm = &(Moves[i]);
        if (sm->from != sq  &&  sm->to != sq) {
            // Skip this move, it does not match.
            continue;
        }
        // Copy this move to an earlier index if necesary:
        if (i != index) {
            Moves[index] = *sm;
        }
        ++index;
    }
    ListSize = index;
    return ListSize;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::FindBest
//   Searches the move list from the specified index to the end of
//   the list to find the move in that range of the list with the
//   highest score, and swaps the best move found with the move at
//   the specified index if it is not already there.
//   The moves earlier in the list (at indices 0 to index-1) are
//   ignored and are not moved.
void
MoveList::FindBest (uint index)
{
    ASSERT (index < ListSize);
    uint bestIndex = index;
    int bestScore = Moves[index].score;
    // Search the rest of the list for a better-scoring move:
    for (uint i = index + 1; i < ListSize; ++i) {
        if (Moves[i].score > bestScore) {
            bestIndex = i;
            bestScore = Moves[i].score;
        }
    }
    // Swap if necessary:
    if (index != bestIndex) {
        simpleMoveT smTemp = Moves[index];
        Moves[index] = Moves[bestIndex];
        Moves[bestIndex] = smTemp;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::Sort
//   Sort the move list by the score field in descending order,
//   with highest scores first.
void
MoveList::Sort (void)
{
    // Do a simple selection sort, which works fine for small list sizes.
    for (uint i=0; i < ListSize; ++i) {
        FindBest (i);
    }
}

bool
MoveList::IsSorted (void)
{
    if (ListSize < 2) { return true; }
    for (uint i = 0; i < ListSize - 1; ++i) {
        if (Moves[i].score < Moves[i+1].score) { return false; }
    }
    return true;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeSimpleMove, readSimpleMove:
//    I/O for simpleMoveT structs.
//
errorT
writeSimpleMove (FILE * fp, simpleMoveT * sm)
{
    errorT err;
    writeOneByte (fp, sm->pieceNum);
    writeOneByte (fp, sm->movingPiece);

    writeOneByte (fp, sm->from);
    writeOneByte (fp, sm->to);

    writeOneByte (fp, sm->capturedNum);
    writeOneByte (fp, sm->capturedPiece);

    writeOneByte (fp, sm->promote);
    writeOneByte (fp, 0);     // Obsolete unused value

    writeOneByte (fp, sm->capturedSquare);
    writeOneByte (fp, sm->castleFlags);

    writeOneByte (fp, sm->epSquare);
    err = writeTwoBytes (fp, sm->oldHalfMoveClock);
    return err;
}

errorT
readSimpleMove (FILE * fp, simpleMoveT * sm)
{
    sm->pieceNum = readOneByte (fp);
    sm->movingPiece = readOneByte (fp);

    sm->from = readOneByte (fp);
    sm->to = readOneByte (fp);

    sm->capturedNum = readOneByte (fp);
    sm->capturedPiece = readOneByte (fp);

    sm->promote = readOneByte (fp);
    readOneByte (fp);   // Obsolete unused value

    sm->capturedSquare = readOneByte (fp);
    sm->castleFlags = readOneByte (fp);

    sm->epSquare = readOneByte (fp);
    sm->oldHalfMoveClock = readTwoBytes (fp);
    return OK;
}

} // End namespace Guess

//////////////////////////////////////////////////////////////////////
//  EOF: movelist.cpp
//////////////////////////////////////////////////////////////////////
