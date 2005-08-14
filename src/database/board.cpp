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


Board::Board()
{
  clear();
}

void Board::clear()
{
  m_toMove = White;
  memset(m_board, InvalidPiece, 64);
  memset(m_pieceType, Empty, 32);
  memset(m_piecePosition, InvalidSquare, 32);
  memset(m_pieceCount, 0, ConstPieceTypes);
}

void Board::fromFEN(const QString&)
{
  clear();
#warning fromFEN not implemented
}

Piece Board::at(Square s) const
{
  return (m_board[s] != InvalidPiece) ? m_pieceType[m_board[s]] : Empty;
}

Piece Board::at(Coord x, Coord y) const
{
  return at(8 * y + x);
}

Piece Board::atIndex(int i) const
{
  return m_pieceType[i];
}

Square Board::piecePosition(int index) const
{
  return m_piecePosition[index];
}

int Board::pieceCount(Piece p) const
{
  return m_pieceCount[p];
}

Square Board::kingPosition(Color c) const
{
  return (c == White) ? m_piecePosition[0] : m_piecePosition[16];
}

Color Board::toMove() const
{
  return m_toMove;
}

void Board::setToMove(Color c)
{
  m_toMove = c;
}

void Board::swapToMove()
{
  m_toMove = m_toMove == White ? Black : White;
}



void Board::doMove(const Move& m)
{
  movePiece(m.from(), m.to());
  if (m.isSpecial())
  {
    if (m.isCastling())
      movePiece(m.castlingRookFrom(), m.castlingRookTo());
    else if (m.isEnPassant())
      m_board[m.enPassantSquare()] = Empty;
    else  /* promotion */
      promotePiece(m.from(), m.to(), m.promotionPiece());
  }
  swapToMove();
}

void Board::undoMove(const Move& m)
{
  movePiece(m.to(), m.from());
  if (m.isCastling())
    movePiece(m.castlingRookTo(), m.castlingRookFrom());
  swapToMove();
}

void Board::undoCapture(const Move& m, int captIndex, Piece captured)
{
  movePiece(m.to(), m.from());
  if (m.isEnPassant())
    restorePiece(m.enPassantSquare(), captured, captIndex);
  else
    restorePiece(m.to(), captured, captIndex);
  swapToMove();
}

void Board::undoPromotion(const Move& m, int promIndex)
{
  movePiece(m.to(), m.from());
# warning undoing promotion not implemented
  // m_board[m.from()] = (m_toMove == White) ? WhitePawn : BlackPawn;
  swapToMove();
}

void Board::undoPromotionCapture(const Move& m, int promIndex, int captIndex, Piece captured)
{
  movePiece(m.to(), m.from());
  restorePiece(m.to(), captured, captIndex);
  // restorePiece(m.to(), m.captured, captIndex);
  swapToMove();
}







void Board::movePiece(Square from, Square to)
{
  if (m_board[to] != InvalidPiece) // capture
  {
    m_pieceCount[m_pieceType[m_board[from]]]--;
    m_pieceType[m_board[from]] = Empty;
  }
  m_board[to] = m_board[from];
  m_board[from] = InvalidPiece;
  m_piecePosition[m_board[from]] = to;
}

void Board::restorePiece(Square from, Piece piece, int index)
{
  m_pieceCount[piece]++;
  m_pieceType[index] = piece;
  m_piecePosition[index] = from;
  m_board[from] = index;
}

void Board::promotePiece(Square from, Square to, Piece promoted)
{
#warning promotionPiece not implemented
}

