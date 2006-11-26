/***************************************************************************
                          history.h - information about capture/promotion
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005, 2006 William Hoggarth
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

#ifndef __HISTORY_ITEM_H__
#define __HISTORY_ITEM_H__

#include "common.h"
#include <qglobal.h>
#include <qstack.h>

class HistoryItem {
public:

	/** Default Constructor */
	HistoryItem();

	/** Constructs a history item which contains the extra information required
			for undoing a move */
	HistoryItem(int capturedIndex, Piece capturedPiece,
							CastlingRights castlingRights, Square epSquare, int halfMoveClock);
	
  /* @return captured piece. */
  Piece piece() const;
  /* Sets captured piece. */
  void setPiece(Piece p);
  /* @return index of captured piece. */
  int index() const;
  /* Sets captured piece. */
  void setIndex(int index);
  /* @return castling rights */
	CastlingRights castlingRights() const;
	/* Sets castling rights */
	void setCastlingRights(CastlingRights castlingRights);
	/* @return en passant square */
	Square epSquare() const;
	/* Sets en passant square */
	void setEPSquare(Square epSquare);
	/* @return half move clock */
	int halfMoveClock() const;
	/* Sets half move clock */
	void setHalfMoveClock(int halfMoveClock);

private:
	
	int m_index;
	Piece m_piece;
	CastlingRights m_castlingRights;
	Square m_epSquare;
	int m_halfMoveClock;
};

typedef QStack<HistoryItem> History;

#endif
