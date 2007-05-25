/***************************************************************************
                          move.h - single abstract move
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __MOVE_H__
#define __MOVE_H__

#include "common.h"

class Board;

/** @ingroup Database
   The Move class represents abstract moves.
   Moves are independent of current position (no piece/check/capture flags are set). Both source and destination squares
   are represented as numbers 0..63.
*/

class Move
{
public:
	/** Default constructor, creates empty move */
	Move();
	/** Typical constructor. */
	Move(Square f, Square t);
	/** Constructs move with given move type */
	Move(Square from, Square to, MoveType moveType);
	/** Constructor with promotion piece */
	Move(Square f, Square t, Piece p);
	/** Constucts move with correct move type */
	Move(const Board& board, Square from, Square to);
	/** Get source square. */
	Square from() const;
	/** Set source square. */
	void setFrom(Square f);
	/** Get destrination square. */
	Square to() const;
	/** Set destination square. */
	void setTo(Square f);
	/** Get move type */
	MoveType type() const;
	/** Set move type */
	void setType(MoveType t);
	/** Set whole move */
	void setMove(Square f, Square t, MoveType tp = StandardMove);
	/** Set promiotion piece */
	void setPromotionPiece(PieceType p);
	/** If move is promotion, get promotion piece. Result is undefined if there is no promotion */
	Piece promotionPiece() const;
	/** Check whether move is special (promotion, castling, en passant */
	bool isSpecial() const;
	/** Check whether move is a promotion */
	bool isPromotion() const;
	/** Check whether move is a castling */
	bool isCastling() const;
	/** @return Square when rook was placed before castling */
	Square castlingRookFrom() const;
	/** @return Square when rook is placed after castling */
	Square castlingRookTo() const;
	/** Check whether the move is a pawn double advance */
	bool isDoubleAdvance() const;
	/** Check whether move is an en passant */
	bool isEnPassant() const;
	/** @return Square when en-passant is captured. Undefined if there is no en-passant. */
	Square enPassantSquare() const;

	/** Check whether move is valid. The only checks that are done are: whether squares are within range (0..63) and
	    whether @p from differs from @p to. */
	bool isValid() const;

	/** Check whether two moves are the same. Moves are considered the same, if both from and to are equal */
	friend bool operator==(const Move& m1, const Move& m2);
private:
	Square m_from, m_to;
	MoveType m_type;
};

#endif // __MOVE_H__
