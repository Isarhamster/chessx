/***************************************************************************
                          board.h - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005, 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                           (C) 2005 Marius Roets <roets.marius@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __BOARD_H__
#define __BOARD_H__

#include "move.h"
#include "movelist.h"
#include "common.h"
#include "rand64.h"
#include "bitboard.h"

#include <QString>
#include <QStack>


/** @ingroup Core
   Track a chess game, and calculate a hash for each position.  This hash
   can be used by other parts of ChessX to do position searches etc.

   This class is actually only responsible for calculating a hash for each
   position; it derives from BitBoard to provide all other functionality.
   Only operations that affect the hash values are implemented here, for
   everything else the derived methods are used.
*/

class Board : public BitBoard
{
public:
	/** Empty constructor. Creates empty board with no pieces. */
	Board();

	/** Resets board, restoring startup position */
	void setStandardPosition();

	/** Sets position from FEN, returns true if FEN was loaded */
	bool fromFen(const QString& fen, int* moveNumber = NULL);

	/** Clear board of all pieces and other state */
	void clear();

	/** Sets side to move - mostly useful when setting position */
	void setToMove(Color c);
	void swapToMove() {setToMove(oppositeColor(toMove()));};

	/** Sets a piece on the given square **/
	void setAt(Square s, Piece p);

	/** Remove a piece from a given square **/
	void removeFrom(Square s);

	/** Make standard move. */
	inline bool doMove(const Move& m) { return doIt(m, false); }
	/** Undo standard move (no captures). */
	inline void undoMove(const Move& m) { doIt(m, true); }

	quint64 getHashValue() const;
	quint64 getHashValue2() const;

	bool operator == (const Board& b) const
	{
		return ((m_hashValue == b.getHashValue()) &&
			(m_hashValue2 == b.getHashValue2()));
	}
	bool operator != (const Board& b) const
	{
		return ((m_hashValue != b.getHashValue()) ||
			(m_hashValue2 != b.getHashValue2()));
	}

private:
	quint64 m_hashValue;
	quint64 m_hashValue2;

	bool doIt(const Move& m, bool undo);
	void createHash();
	void hashPiece(Square s, Piece p);
	void hashToMove();
	void hashEpSquare();
	void hashCastlingRights(CastlingRights oldCastlingRights);
};
#endif

