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
  m_popup->insertItem("King", WhiteKing);
  m_popup->insertItem("Queen", WhiteQueen);
  m_popup->insertItem("Rook", WhiteRook);
  m_popup->insertItem("Bishop", WhiteBishop);
  m_popup->insertItem("Knight", WhiteKnight);
  m_popup->insertItem("Pawn", WhitePawn);
  m_popup->setAccel(CTRL + Key_K, WhiteKing);
  m_popup->setAccel(CTRL + Key_Q, WhiteQueen);
  m_popup->setAccel(CTRL + Key_Q, WhiteRook);
  m_popup->setAccel(CTRL + Key_B, WhiteBishop);
  m_popup->setAccel(CTRL + Key_N, WhiteKnight);
  m_popup->setAccel(CTRL + Key_P, WhitePawn);
  connect(m_popup, SIGNAL(activated(int)), SLOT(slotChoosePiece(int)));

  connect(okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(clearButton, SIGNAL(clicked()), SLOT(slotClear()));
  connect(resetButton, SIGNAL(clicked()), SLOT(slotReset()));
  connect(boardView, SIGNAL(mousePressed(const QPoint&, ButtonState)), 
          SLOT(slotSelected(const QPoint&, ButtonState)));
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
  m_piece = Piece(piece);
}

void BoardSetupDialog::slotSelected(const QPoint& p, ButtonState b)
{
  if (b == LeftButton)
  {
    m_popup->exec(QCursor::pos());
  }
}

