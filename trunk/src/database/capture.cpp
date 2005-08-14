/***************************************************************************
                          capture.cpp - infor,mation about capture/promotion
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

#include "capture.h"

Capture::Capture(int move, unsigned char index, Piece p) : m_move(move), m_index(index), m_piece(p)
{}

Capture::Capture(int move, unsigned char index) : m_move(move), m_index(index), m_piece(Empty)
{}

int Capture::move() const
{
  return m_move;
}

void Capture::setMove(int move)
{
  m_move = move;
}

Piece Capture::piece() const
{
  return m_piece;
}

void Capture::setPiece(Piece p)
{
  m_piece = p;
}

unsigned char Capture::index() const
{
  return m_index;
}

void Capture::setIndex(unsigned char index)
{
  m_index = index;
}

bool Capture::isPromotion() const
{
  return m_piece == Empty;
}

bool Capture::isCapture() const
{
  return m_piece != Empty;
}


bool CaptureList::hasMove(int move) const
{
  return top().move() == move;
}



