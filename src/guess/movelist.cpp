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

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

using namespace Guess;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::MoveToFront
//   Promotes a move to the front of the list, pushing
//   all moves that were ahead of it down one place.
void
MoveList::MoveToFront(unsigned int index)
{
    move(index,0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SwapWithFirst
//   Swaps the specified move with the move at the start of the list.
//   All other moves remain in their original location.
void
MoveList::SwapWithFirst(unsigned int index)
{
    swap(index,0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::Find
//   Searches for a move in the list.
//   Returns the index of the move with the same from-square,
//   to-square and promotion-piece values as the specified
//   move, if it is in the list. If the move is not found,
//   the value -1 is returned.
int
MoveList::Find(const simpleMoveT * sm) const
{
    for(int i = 0; i < size(); ++i)
    {
        const simpleMoveT& lsm = at(i);
        if(sm->from == lsm.from  &&  sm->to == lsm.to
                &&  sm->promote == lsm.promote)
        {
            return i;
        }
    }
    return -1;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SelectBySquares
//   Restricts the list to only contain only the two
//   moves suggested by (from1,to1) and (from2,to2)
//   Returns the new size of the list.
unsigned int
MoveList::SelectBySquares(squareT f1, squareT t1, squareT f2, squareT t2)
{
    for (int i = 0; i < size(); )
    {
        const simpleMoveT& sm = at(i);
        if((sm.from == f1 && sm.to == t1) || (sm.from == f2 && sm.to == t2))
        {
            ++i;
        }
        else
        {
            removeAt(i);
        }
    }
    return size();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::SelectBySquare
//   Restricts the list to only contain moves which
//   move a piece to or from the specified square.
//   Returns the new size of the list.
unsigned int
MoveList::SelectBySquare(squareT sq)
{
    for(int i = 0; i < size(); )
    {
        const simpleMoveT& sm = at(i);
        if(sm.from != sq  &&  sm.to != sq)
        {
            removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    return size();
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
MoveList::FindBest(unsigned int index)
{
    unsigned int bestIndex = index;
    int bestScore = at(index).score;
    // Search the rest of the list for a better-scoring move:
    for(int i = index + 1; i < size(); ++i)
    {
        if(at(i).score > bestScore)
        {
            bestIndex = i;
            bestScore = at(i).score;
        }
    }
    // Swap if necessary:
    if(index != bestIndex)
    {
        swap(index,bestIndex);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MoveList::Sort
//   Sort the move list by the score field in descending order,
//   with highest scores first.
void
MoveList::Sort(void)
{
    qSort(begin(),end(), qGreater<simpleMoveT>());
}

//////////////////////////////////////////////////////////////////////
//  EOF: movelist.cpp
//////////////////////////////////////////////////////////////////////
