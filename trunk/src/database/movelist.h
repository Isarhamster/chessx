/***************************************************************************
                          movelist.h - board position
                             -------------------
    begin                : 27 August 2005
    copyright            : (C) 2005 William Hoggarth
														<whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/** @ingroup Core
   Low level list of moves, used by bitboard
*/

#ifndef __MOVELIST_H__
#define __MOVELIST_H__
#include "move.h"

class MoveList
{
public:
	MoveList() : m_cnt(0) {}
	// There really really must be a better way.
	Move& add() { Q_ASSERT(m_cnt < 90); return m_moves[m_cnt++]; };
	int size() { return m_cnt; };
	bool empty() { return !m_cnt; };
	void append(const Move& move) { Q_ASSERT(m_cnt < 90); m_moves[m_cnt++] = move; };
	Move& operator[](int index) {return m_moves[index];};
private:
	Move m_moves[500];		// Later This should be made dynamic.
	int m_cnt;
};

#endif // __MOVELIST_H__
