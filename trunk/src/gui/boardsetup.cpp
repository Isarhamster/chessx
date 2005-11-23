/***************************************************************************
                          boardsetup.cpp  -  Board setup dialog
                             -------------------
    begin                : 
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

#include "boardsetup.h"
#include "boardview.h"

#include <qcursor.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>

BoardSetupDialog::BoardSetupDialog(QWidget* parent) : BoardSetupBaseDialog(parent), m_piece(WhiteKing)
{
  m_popup = new QPopupMenu(boardView);
  m_popup->insertItem("White king", WhiteKing);
  m_popup->insertItem("White queen", WhiteQueen);
  m_popup->insertItem("White rook", WhiteRook);
  m_popup->insertItem("White bishop", WhiteBishop);
  m_popup->insertItem("White knight", WhiteKnight);
  m_popup->insertItem("White pawn", WhitePawn);
  m_popup->insertSeparator();
  m_popup->insertItem("Black king", BlackKing);
  m_popup->insertItem("Black queen", BlackQueen);
  m_popup->insertItem("Black rook", BlackRook);
  m_popup->insertItem("Black bishop", BlackBishop);
  m_popup->insertItem("Black knight", BlackKnight);
  m_popup->insertItem("Black pawn", BlackPawn);
  m_popup->setAccel(CTRL + Key_K, WhiteKing);
  m_popup->setAccel(CTRL + Key_Q, WhiteQueen);
  m_popup->setAccel(CTRL + Key_R, WhiteRook);
  m_popup->setAccel(CTRL + Key_B, WhiteBishop);
  m_popup->setAccel(CTRL + Key_N, WhiteKnight);
  m_popup->setAccel(CTRL + Key_P, WhitePawn);
  m_popup->setAccel(CTRL + SHIFT + Key_K, BlackKing);
  m_popup->setAccel(CTRL + SHIFT + Key_Q, BlackQueen);
  m_popup->setAccel(CTRL + SHIFT + Key_R, BlackRook);
  m_popup->setAccel(CTRL + SHIFT + Key_B, BlackBishop);
  m_popup->setAccel(CTRL + SHIFT + Key_N, BlackKnight);
  m_popup->setAccel(CTRL + SHIFT + Key_P, BlackPawn);
  connect(m_popup, SIGNAL(activated(int)), SLOT(slotChoosePiece(int)));

  connect(okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(clearButton, SIGNAL(clicked()), SLOT(slotClear()));
  connect(resetButton, SIGNAL(clicked()), SLOT(slotReset()));
  connect(boardView, SIGNAL(mousePressed(const QPoint&, int)), 
          SLOT(slotSelected(const QPoint&, int)));
}

BoardSetupDialog::~BoardSetupDialog()
{
}

Board BoardSetupDialog::board() const
{
  return boardView->board();
}

void BoardSetupDialog::setBoard(const Board& b)
{
  boardView->setBoard(b);
}

int BoardSetupDialog::exec()
{
  boardView->configure();
  return BoardSetupBaseDialog::exec();
}

void BoardSetupDialog::slotReset()
{
  Board b;
  b.setStandardPosition();
  boardView->setBoard(b);
}

void BoardSetupDialog::slotClear()
{
  Board b;
  boardView->setBoard(b);
}

void BoardSetupDialog::slotChoosePiece(int piece)
{
  m_popup->setItemChecked(m_piece, false);
  m_piece = Piece(piece);
  m_popup->setItemChecked(m_piece, true);
}

void BoardSetupDialog::slotSelected(const QPoint& p, int b)
{
  if (b & RightButton)
  {
    m_popup->exec(QCursor::pos());
  }
  else 
  {
    Piece piece = (b & MidButton) ? Empty : m_piece;
    if (b & ShiftButton)
    {
      if (piece >= BlackKing)
        piece = (Piece)(piece - (BlackKing - WhiteKing));
      else
        piece = (Piece)(piece + (BlackKing - WhiteKing));
    }
    Board board = boardView->board();
    board.setAt(boardView->squareAt(p), piece);
    boardView->setBoard(board);
  }
}

