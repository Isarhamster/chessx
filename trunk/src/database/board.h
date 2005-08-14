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
#include "capture.h"

#include <qstring.h>

/**
   The Board class represents a chess position.
   Pieces are kept directly on the board which is internally represented as simple array[0..63].
   You can easily and quickly make and undo moves, although undoing moves requires keeping track
   of captured pieces.
*/

class Board
{
public:
  /** Empty constructor. Creates empty board with no pieces. */
  Board();
  /** Resets board, remove all pieces */
  void clear();
  /** Resets board, restoring startup position */
  void setStandardPosition();
  /** Sets position from FEN */
  void fromFEN(const QString& fen);

  /* information about pieces position and types */
  /** @return piece on given square */
  Piece at(Square s) const;
  /** @return piece on given square (defined by x,y from 1-8 range) */
  Piece at(Coord x, Coord y) const;
  /** @return piece at given @p index (0..31) */
  Piece atIndex(int i) const;
  /** @return position of piece given by @p index (0..31) */
  Square piecePosition(int index) const;

  /** @return number of all pieces of given type. */
  int pieceCount(Piece p) const;

  /** @return position of king */
  Square kingPosition(Color c) const;

  /** @return side to move */
  Color toMove() const;
   /** Sets side to move - mostly useful when setting position */
  void setToMove(Color c); 
  /** Swaps side to move */
  void swapToMove();

  /** Make standard move. */
  void doMove(const Move& m);
  /** Undo standard move (no captures). */
  void undoMove(const Move& m);
  /** Undo standard move (with capture). */
  void undoCapture(const Move& m, int captIndex, Piece captured);
  /** Undo standard move (with promotion). */
  void undoPromotion(const Move& m, int promIndex);
  /** Undo standard move (with promotion and capture). */
  void undoPromotionCapture(const Move& m, int promIndex, int captIndex, Piece captured);

private:
  /** Move piece from @p from to @p to, leaving source square empty */
  void movePiece(Square from, Square to);
  /** Restore captured piece */
  void restorePiece(Square from, Piece piece, int index);
  /** Move a pawn from @p from to @p to, promoting it to @p promoted */
  void promotePiece(Square from, Square to, Piece promoted);

  unsigned char m_board[64];
  Piece m_pieceType[32];
  Square m_piecePosition[32];
  int m_pieceCount[ConstPieceTypes];
  Color m_toMove;
};




#endif

