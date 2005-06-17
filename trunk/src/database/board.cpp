/***************************************************************************
                          board.cpp - board position
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

#include "board.h"
#include "string.h"

Board::Board() : m_toMove(White)
{
  memset(m_board, Empty, 64);
  memset(m_kings, 0, 2);
}

Piece Board::at(Square s) const
{
  return m_board[s];
}

Piece Board::at(Coord x, Coord y) const
{
  return m_board[8 * y + x];
}

Square Board::kingPosition(Color c) const
{
  return m_kings[c];
}

int Board::pieceCount(Piece p) const
{
  int pc = 0;
  for(int i = 0; i < 64; i++)
    pc += m_board[i] == p;
  return pc;
}

Color Board::toMove() const
{
  return m_toMove;
}

void Board::doStandardMove(const Move& m)
{
  movePiece(m.from(), m.to());
  if (m.isSpecial())
  {
    if (m.isCastling())
      movePiece(m.castlingRookFrom(), m.castlingRookTo());
    else if (m.isEnPassant())
      m_board[m.enPassantSquare()] = Empty;
    else  /* promotion */
      m_board[m.to()] = m.promotionPiece();
  }
  if (m_board[m.to()] == WhiteKing)
    m_kings[White] = m.to();
  else if (m_board[m.to()] == BlackKing)
    m_kings[Black] = m.to();
  m_toMove = m_toMove == White ? Black : White;
}

void Board::undoStandardMove(const Move& m, Piece captured)
{
  m_board[m.from()] = m_board[m.to()];
  m_board[m.to()] = captured;
  if (m.isSpecial())
  {
    if (m.isCastling())
      movePiece(m.castlingRookTo(), m.castlingRookFrom());
    else if (m.isEnPassant())
      m_board[m.enPassantSquare()] = (m_toMove == White) ? BlackPawn : WhitePawn;
    else  /* promotion */
      m_board[m.from()] = (m_toMove == White) ? WhitePawn : BlackPawn;
  }
  if (m_board[m.from()] == WhiteKing)
    m_kings[White] = m.from();
  else if (m_board[m.from()] == BlackKing)
    m_kings[Black] = m.from();
  m_toMove = m_toMove == White ? Black : White;
}


void Board::movePiece(Square from, Square to)
{
  m_board[to] = m_board[from];
  m_board[from] = Empty;
}
