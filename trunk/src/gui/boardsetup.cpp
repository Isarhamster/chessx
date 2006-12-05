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
#include <q3popupmenu.h>

using namespace Qt;

BoardSetupDialog::BoardSetupDialog(QWidget* parent) : QDialog(parent), m_piece(WhiteKing)
{
  ui.setupUi(this);

  m_popup = new Q3PopupMenu(ui.boardView);
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
  m_popup->setAccel(Key_K, WhiteKing);
  m_popup->setAccel(Key_Q, WhiteQueen);
  m_popup->setAccel(Key_R, WhiteRook);
  m_popup->setAccel(Key_B, WhiteBishop);
  m_popup->setAccel(Key_N, WhiteKnight);
  m_popup->setAccel(Key_P, WhitePawn);
  m_popup->setAccel(SHIFT + Key_K, BlackKing);
  m_popup->setAccel(SHIFT + Key_Q, BlackQueen);
  m_popup->setAccel(SHIFT + Key_R, BlackRook);
  m_popup->setAccel(SHIFT + Key_B, BlackBishop);
  m_popup->setAccel(SHIFT + Key_N, BlackKnight);
  m_popup->setAccel(SHIFT + Key_P, BlackPawn);
  connect(m_popup, SIGNAL(activated(int)), SLOT(slotChoosePiece(int)));

  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
  connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
  connect(ui.boardView, SIGNAL(mousePressed(const QPoint&, int)),
          SLOT(slotSelected(const QPoint&, int)));
}

BoardSetupDialog::~BoardSetupDialog()
{
}

Board BoardSetupDialog::board() const
{
  return ui.boardView->board();
}

void BoardSetupDialog::setBoard(const Board& b)
{
  ui.boardView->setBoard(b);
}

int BoardSetupDialog::exec()
{
  ui.boardView->configure();
  return QDialog::exec();
}

void BoardSetupDialog::slotReset()
{
  Board b;
  b.setStandardPosition();
  ui.boardView->setBoard(b);
}

void BoardSetupDialog::slotClear()
{
  Board b;
  ui.boardView->setBoard(b);
}

void BoardSetupDialog::slotChoosePiece(int piece)
{
  m_popup->setItemChecked(m_piece, false);
  m_piece = Piece(piece);
  m_popup->setItemChecked(m_piece, true);
}

void BoardSetupDialog::slotSelected(const QPoint& p, int b)
{
  if (b & Qt::RightButton)
    m_popup->exec(QCursor::pos());
  else
  {
    Piece piece = (b & Qt::MidButton) ? Empty : m_piece;
    if (b & Qt::ShiftModifier)
    {
      if (piece >= BlackKing)
        piece = (Piece)(piece - (BlackKing - WhiteKing));
      else
        piece = (Piece)(piece + (BlackKing - WhiteKing));
    }
    Board board = ui.boardView->board();
    Square square = ui.boardView->squareAt(p);
    if (board.at(square) == piece)
      piece = Empty;
    board.setAt(square, piece);
    ui.boardView->setBoard(board);
  }
}

