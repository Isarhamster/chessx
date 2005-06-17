/***************************************************************************
                          board.h - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
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

/**
   The Board class represents a chess position.
   Pieces are kept directly on the board which is internally represented as simple array[0..63].
   You can easily and quickly make and undo moves, although undoing moves requires keeping track
   of captured pieces.
*/

class Board
{
public:
  /** Empty constructor. Clears all the board */
  Board();
  /** Reset board, remove all pieces */
  void clear();

  /** @return piece on given square */
  Piece at(Square s) const;
  /** @return piece on given square (defined by x,y from 1-8 range) */
  Piece at(Coord x, Coord y) const;

  /** @return position of piece given by index */
  Square piecePosition(int index) const;
  /** @return king position of given side */
  Square kingPosition(Color c) const;
  /** @return number of all pieces of given type */
  int pieceCount(Piece p) const;
  /** @return side to move */
  Color toMove() const;
  /** @return set side to move - mostly useful when setting position */
  /** Make standard move (not castling, not e.p. */
  void doStandardMove(const Move& m);
  /** Undo standard move, possibly restoring captured piece */
  void undoStandardMove(const Move& m, Piece captured = Empty);

private:
  /** Move piece from @p from to @p to, leaving source square empty */
  void movePiece(Square from, Square to);
  Piece m_board[64];
  Color m_toMove;
  Square m_kings[2];
};




#endif

