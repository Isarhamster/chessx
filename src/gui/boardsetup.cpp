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
	ui.moveSpin->setValue(b.moveNumber());
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
	setBoard(b);
}

void BoardSetupDialog::slotAccept()
{
	// Need to make sure the board is updated with move number set by user
	Board b(ui.boardView->board());
	b.setMoveNumber(ui.moveSpin->value());
	ui.boardView->setBoard(b);

	QString reason;
	switch (ui.boardView->board().validate()) {
	case Valid:
		accept();
		return;
	case NoWhiteKing:
		reason = tr("No white king");
		break;
	case NoBlackKing:
		reason = tr("No black king");
		break;
	case DoubleCheck:
		reason = tr("Both kings are in check");
		break;
	case OppositeCheck:
		reason = tr("Side to move has opponent in check already");
		break;
	case TooManyBlackPawns:
		reason = tr("Black has too many pawns");
		break;
	case TooManyWhitePawns:
		reason = tr("White has too many pawns");
		break;
	case PawnsOn18:
		reason = tr("There are pawns on the first or eighth rank");
		break;
	case TooManyKings:
		reason = tr("Too many kings");
		break;
	case TooManyBlack:
		reason = tr("Too many black pieces");
		break;
	case TooManyWhite:
		reason = tr("Too many white pieces");
		break;
	case BadCastlingRights:
		reason = tr("Bad castling rights");
		break;
	case InvalidEnPassant:
		reason = tr("En passant square is not correct");
		break;
	default:
		reason = tr("Unknown reason");
		break;
	}
	QMessageBox::critical(0, tr("Invalid position"),
		      tr("Current position is not valid.\n\n%1.").arg(reason));
}

void BoardSetupDialog::slotClear()
{
	setBoard(Board());
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
	if (board.pieceAt(square) == piece)
		piece = Empty;
	board.setAt(square, piece);
	ui.boardView->setBoard(board);
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
	Piece p = b.pieceAt(from);
	b.removeFrom(from);
	b.setAt(to, p);
	ui.boardView->setBoard(b);
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
	slotChangePiece(e->delta() < 0 ? BoardView::WheelDown : BoardView::WheelUp);
}



