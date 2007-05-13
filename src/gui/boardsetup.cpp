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
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QWheelEvent>

using namespace Qt;

BoardSetupDialog::BoardSetupDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	ui.boardView->configure();
	ui.boardView->setFlags(BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);

	m_actions = new QActionGroup(this);
	m_actions->setExclusive(true);

	QGridLayout *layout = new QGridLayout;
	for (int piece = Empty; piece <= BlackPawn; piece++) {
		QAction* action = new QAction(QString(), m_actions);
		action->setData(piece);
		action->setCheckable(true);
		if (piece == WhiteKing)
			action->setChecked(true);
		action->setIcon(ui.boardView->theme().piece(Piece(piece)));
		QToolButton* button = new QToolButton(ui.buttonWidget);
		button->setDefaultAction(action);
		if (piece == Empty)
			layout->addWidget(button, 6, 0);
		else
			layout->addWidget(button, (piece - 1) % 6, piece >= BlackKing);
	}
	ui.buttonWidget->setLayout(layout);

	connect(m_actions, SIGNAL(triggered(QAction*)), SLOT(slotChoosePiece(QAction*)));
	connect(ui.okButton, SIGNAL(clicked()), SLOT(slotAccept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
	connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
	connect(ui.boardView, SIGNAL(clicked(Square, int)), SLOT(slotSelected(Square, int)));
	connect(ui.boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMovePiece(Square, Square)));
	connect(ui.boardView, SIGNAL(wheelScrolled(int)), SLOT(slotChangePiece(int)));
	connect(ui.toMoveButton, SIGNAL(clicked()), SLOT(slotToggleSide()));
}

BoardSetupDialog::~BoardSetupDialog()
{}

Board BoardSetupDialog::board() const
{
	Board b = ui.boardView->board();
	return b;
}

void BoardSetupDialog::setFlipped(bool flipped)
{
	ui.boardView->setFlipped(flipped);
}

void BoardSetupDialog::setBoard(const Board& b)
{
	ui.boardView->setBoard(b);
	m_toMove = b.toMove();
	showSideToMove();
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
	m_toMove = White;
	showSideToMove();
}

void BoardSetupDialog::slotAccept()
{
	int state;
	if (ui.boardView->board().isValid(&state))
		accept();
	else {
		QString reason = tr("uknown");
		switch (state) {
		case NoWhiteKing:
			reason = tr("no white king");
			break;
		case NoBlackKing:
			reason = tr("no black king");
			break;
		case DoubleCheck:
			reason = tr("both kings in check");
			break;
		case OppositeCheck:
			reason = tr("side not to move is in check");
			break;
		default:
			reason = tr("unknown");
		}
		QMessageBox::critical(0, tr("Invalid position"),
				      tr("Current position is not valid.\nReason: %1.").arg(reason));
	}
}

void BoardSetupDialog::slotClear()
{
	ui.boardView->setBoard(Board());
	m_toMove = White;
	showSideToMove();
}

void BoardSetupDialog::slotChoosePiece(QAction* action)
{
	action->setChecked(true);
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
	Piece piece = (button & Qt::MidButton) ? Empty : Piece(m_actions->checkedAction()->data().toInt());
	if (button & Qt::RightButton) {
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

void BoardSetupDialog::setMoveNumber(int i)
{
	ui.moveSpin->setValue(i);
}

int BoardSetupDialog::moveNumber() const
{
	return ui.moveSpin->value();
}

void BoardSetupDialog::showSideToMove()
{
	if (m_toMove == White)
		ui.toMoveButton->setText(tr("White"));
	else ui.toMoveButton->setText(tr("Black"));
	QPalette palette;
	palette.setColor(ui.toMoveButton->backgroundRole(), m_toMove == White ? Qt::white : Qt::black);
	ui.toMoveButton->setPalette(palette);
	palette.setColor(ui.toMoveButton->foregroundRole(), m_toMove == White ? Qt::black : Qt::white);
	ui.toMoveButton->setPalette(palette);
}

void BoardSetupDialog::slotToggleSide()
{
	m_toMove = oppositeColor(m_toMove);
	Board b = ui.boardView->board();
	b.setToMove(m_toMove);
	ui.boardView->setBoard(b);
	showSideToMove();
}

void BoardSetupDialog::slotChangePiece(int dir)
{
	int i = m_actions->actions().indexOf(m_actions->checkedAction());
	i += (dir == BoardView::WheelUp) ? -1 : 1;
	if (i < 0) i = m_actions->actions().count() - 1;
	else if (i == m_actions->actions().count()) i = 0;
	m_actions->actions().at(i)->setChecked(true);
}

void BoardSetupDialog::slotMovePiece(Square from, Square to)
{
	Board b = ui.boardView->board();
	Piece p = b.at(from);
	b.removeFrom(from);
	b.setAt(to, p);
	ui.boardView->setBoard(b);
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
	slotChangePiece(e->delta() < 0 ? BoardView::WheelDown : BoardView::WheelUp);
}



