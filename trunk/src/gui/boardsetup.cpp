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

#include <QActionGroup>
#include <QCursor>
#include <QPushButton>
#include <QMenu>

using namespace Qt;

BoardSetupDialog::BoardSetupDialog(QWidget* parent) : QDialog(parent), m_piece(WhiteKing)
{
  ui.setupUi(this);

  m_actions = new QActionGroup(this);
  m_actions->setExclusive(true);

  ui.boardView->setContextMenuPolicy(Qt::ActionsContextMenu);

  addAction(pieceAction(tr("White king"), WhiteKing, Key_K));
  addAction(pieceAction(tr("White queen"), WhiteQueen, Key_Q));
  addAction(pieceAction(tr("White rook"), WhiteRook, Key_R));
  addAction(pieceAction(tr("White bishop"), WhiteBishop, Key_B));
  addAction(pieceAction(tr("White knight"), WhiteKnight, Key_N));
  addAction(pieceAction(tr("White pawn"), WhitePawn, Key_P));

  QAction* separator =  new QAction(m_actions);
  separator->setSeparator(true);
  ui.boardView->addAction(separator);

  addAction(pieceAction(tr("Black king"), BlackKing, SHIFT + Key_K));
  addAction(pieceAction(tr("Black queen"), WhiteQueen, SHIFT + Key_Q));
  addAction(pieceAction(tr("Black rook"), WhiteRook, SHIFT + Key_R));
  addAction(pieceAction(tr("Black bishop"), BlackBishop, SHIFT + Key_B));
  addAction(pieceAction(tr("Black knight"), BlackKnight, SHIFT + Key_N));
  addAction(pieceAction(tr("Black pawn"), BlackPawn, SHIFT + Key_P));

  connect(m_actions, SIGNAL(triggered(QAction*)), SLOT(slotChoosePiece(QAction*)));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
  connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
  connect(ui.boardView, SIGNAL(clicked(Square, int)), SLOT(slotSelected(Square, int)));
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
  ui.boardView->setBoard(Board());
}

void BoardSetupDialog::slotChoosePiece(QAction* action)
{
  action->setChecked(true);
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
  Piece piece = (button & Qt::MidButton) ? Empty : Piece(m_actions->checkedAction()->data().toInt());
  if (button & Qt::ShiftModifier)
  {
    if (piece >= BlackKing)
      piece = (Piece)(piece - (BlackKing - WhiteKing));
    else
      piece = (Piece)(piece + (BlackKing - WhiteKing));
  }
  Board board = ui.boardView->board();
  if (board.at(square) == piece)
    piece = Empty;
  board.setAt(square, piece);
  ui.boardView->setBoard(board);
}

QAction* BoardSetupDialog::pieceAction(const QString& name, int piece, QKeySequence shortcut)
{
  QAction* action = new QAction(name, m_actions);
  ui.boardView->addAction(action);
  action->setData(piece);
  action->setShortcut(shortcut);
  action->setCheckable(true);
  if (piece == WhiteKing)
    action->setChecked(true);
  return action;
}

