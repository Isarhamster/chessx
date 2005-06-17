/***************************************************************************
                          move.cpp - single abstract move
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

#include "move.h"

Move::Move() : m_from(0), m_to(0)
{
}

Move::Move(Square f, Square t) : m_from(f), m_to(t)
{
}

Square Move::from() const
{
  return m_from;
}

void Move::setFrom(Square f)
{
  m_from = f;
}

Square Move::to() const
{
  return m_to;
}

void Move::setTo(Square t)
{
  m_to = t;
}

void Move::setMove(Square f, Square t, MoveType tp)
{
  m_from = f;
  m_to = t;
  m_type = tp;
}

void Move::setType(MoveType tp)
{
  m_type = tp;
}

MoveType Move::type() const
{
  return m_type;
}

bool Move::isSpecial() const
{
  return m_type != StandardMove;
}

bool Move::isPromotion() const
{
  return m_type >= PromotionWhiteQueen && m_type <= PromotionBlackKnight;
}

Piece Move::promotionPiece() const
{
  return (Piece)m_type;
}

bool Move::isCastling() const
{
  return m_type == Castling;
}

Square Move::castlingRookFrom() const
{
  return (m_to % 8 == 2) ? m_to - 2 : m_to + 1;
}

Square Move::castlingRookTo() const
{
  return (m_from + m_to) / 2;
}

bool Move::isEnPassant() const
{
  return m_type == EnPassant;
}

Square Move::enPassantSquare() const
{
  return m_from > 31 ? m_to - 8 : m_to + 8;
}

bool Move::isValid() const
{
  return m_from != m_to && m_from < 64 && m_to < 64;
}

bool operator==(const Move& m1, const Move& m2)
{
  return m1.from() == m2.from() && m1.to() == m2.to() && m1.type() == m2.type();
}

