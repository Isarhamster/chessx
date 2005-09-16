/***************************************************************************
                          board.h - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
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

#ifndef __BOARD_H__
#define __BOARD_H__

#include "move.h"
#include "history.h"
#include "movelist.h"
#include "common.h"

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
	/** @return FEN string for position */
	QString toFEN() const;
	/** @return ASCII representation for debugging */
	QString toASCII() const;
	
  /* information about pieces position and types */
  /** @return piece on given square */
  Piece at(Square s) const;
  /** @return piece on given square (defined by x,y from 0-7 range) */
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

	/* move information methods */
	/** @return move object represented by the given short algerbraic notation */
	Move singleMove(QString& SAN);
	/** @return move object represented by the given long algerbraic notation */
	Move singleLANMove(QString& LAN);
	/** @return whether is particular sqaure is attacked */
  bool isAttacked(Square sq,Color c) const;
	/** @return whether a given move is legal in the current position */
  bool isLegal(Move& m);
	
	/* move / undo move methods */
  /** Make standard move. */
  HistoryItem doMove(const Move& m);
  /** Undo standard move (no captures). */
  void undoMove(const Move& m, const HistoryItem& historyItem);

private:
  /** Move piece from @p from to @p to, leaving source square empty */
  void movePiece(Square from, Square to);
  /** Restore captured piece */
  void restorePiece(Square from, Piece piece, int index);
  /** Promotes a piece / changes its type*/
  void promotePiece(Square square, Piece promoted);

  unsigned char m_board[64];
  Piece m_pieceType[32];
  Square m_piecePosition[32];
  int m_pieceCount[ConstPieceTypes];
	
  Color m_toMove;
	Square m_epSquare;
	CastlingRights m_castlingRights;
	int m_halfMoveClock;
};

#endif

