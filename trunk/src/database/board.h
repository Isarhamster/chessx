/***************************************************************************
 *   (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>                     *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#define COMPILED_GUESS_FILE_ID ((quint32)0xCD5CBD03U)


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
	bool fromFen(const QString& fen);

	/** Clear board of all pieces and other state */
	void clear();

	/** Sets side to move - mostly useful when setting position */
	void setToMove(Color c);

	/** Change side to move to opposite of current value */
	void swapToMove() {setToMove(oppositeColor(toMove()));};

	/** Sets a piece on the given square **/
	void setAt(Square s, Piece p);

	/** Remove a piece from a given square **/
	void removeFrom(Square s);

	/** Make standard move. */
	inline bool doMove(const Move& m) { return doIt(m, false); }
	/** Undo standard move (no captures). */
	inline void undoMove(const Move& m) { doIt(m, true); }

	/** Return a suggested move associated with given square based on ECO data */
	bool ecoMove(const Square square, int* from, int* to) const;

	/** Load the guess-the-move ECO hash data which is used by ecoMove() method */
	static bool loadEcoFile(const QString& ecoFile);

	/** Return a hash based on current and a given square number */
	quint64 getHashPlusSquare(const Square square) const;

	/** Return hash value associated with current board position */
	quint64 getHashValue() const;

	/** Return second hash value associated with current board position */
	quint64 getHashValue2() const;

	/** Compare one board to another for equality, based on hash values */
	bool operator == (const Board& b) const
	{
		return m_hashValue == b.getHashValue();
	}

	/** Compare one board to another for differences, based on hash values */
	bool operator != (const Board& b) const
	{
		return m_hashValue != b.getHashValue();
	}

private:
	quint64 m_hashValue;

	/** Play (or undo) move on board and calculate hash value for new position */
	bool doIt(const Move& m, bool undo);
	/** recalculate hash completely */
	void createHash();
	/** Adjust hash value based on piece sitting on given square */
	void hashPiece(Square s, Piece p);
	/** Adjust hash value based on which side is to move */
	void hashToMove();
	/** Adjust hash value based on the current en passant square */
	void hashEpSquare();
	/** Adjust hash value based on the castling rights each side has */
	void hashCastlingRights(CastlingRights oldCastlingRights);
};

extern const Board standardStartBoard;
#endif

