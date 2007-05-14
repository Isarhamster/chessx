/***************************************************************************
                          history.cpp - information about capture/promotion
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 William Hoggarth
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

#include "history.h"

HistoryItem::HistoryItem()
{
	m_index = 0;
	m_piece = InvalidPiece;
	m_castlingRights = AllRights;
	m_epSquare = NoEPSquare;
	m_halfMoveClock = 0;
}

HistoryItem::HistoryItem(int capturedIndex, Piece capturedPiece,
			 CastlingRights castlingRights, Square epSquare, int halfMoveClock)
{
	m_index = capturedIndex;
	m_piece = capturedPiece;
	m_castlingRights = castlingRights;
	m_epSquare = epSquare;
	m_halfMoveClock = halfMoveClock;
}

Piece HistoryItem::piece() const
{
	return m_piece;
}

void HistoryItem::setPiece(Piece p)
{
	m_piece = p;
}

int HistoryItem::index() const
{
	return m_index;
}

void HistoryItem::setIndex(int index)
{
	m_index = index;
}

CastlingRights HistoryItem::castlingRights() const
{
	return m_castlingRights;
}

void HistoryItem::setCastlingRights(CastlingRights castlingRights)
{
	m_castlingRights = castlingRights;
}

Square HistoryItem::epSquare() const
{
	return m_epSquare;
}


void HistoryItem::setEPSquare(Square epSquare)
{
	m_epSquare = epSquare;
}

int HistoryItem::halfMoveClock() const
{
	return m_halfMoveClock;
}

void HistoryItem::setHalfMoveClock(int halfMoveClock)
{
	m_halfMoveClock = halfMoveClock;
}
