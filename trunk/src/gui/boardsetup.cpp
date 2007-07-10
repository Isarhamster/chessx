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

#include <QApplication>
#include <QActionGroup>
#include <QClipboard>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QWheelEvent>

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

	QAction* copyFen = new QAction(tr("Copy position to clipboard"), this);
	copyFen->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
	connect(copyFen, SIGNAL(triggered(bool)), SLOT(slotCopyFen()));
	addAction(copyFen);
	QAction* pasteFen = new QAction(tr("Paste position from clipboard"), this);
	pasteFen->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
	connect(pasteFen, SIGNAL(triggered(bool)), SLOT(slotPasteFen()));
	addAction(pasteFen);
	setContextMenuPolicy(Qt::ActionsContextMenu);

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
	setStatusMessage();
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

	QString reason = boardStatusMessage();
	if (reason.isEmpty())
		accept();
	else QMessageBox::critical(0, tr("Invalid position"),
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
	setStatusMessage();
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
	setBoard(b);
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
	setBoard(b);
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
	slotChangePiece(e->delta() < 0 ? BoardView::WheelDown : BoardView::WheelUp);
}

QString BoardSetupDialog::boardStatusMessage() const
{
	switch (ui.boardView->board().validate()) {
	case Valid:
		return QString();
	case NoWhiteKing:
		return tr("No white king");
	case NoBlackKing:
		return tr("No black king");
	case DoubleCheck:
		return tr("Both kings are in check");
	case OppositeCheck:
		return tr("Side to move has opponent in check already");
	case TooManyBlackPawns:
		return tr("Black has too many pawns");
	case TooManyWhitePawns:
		return tr("White has too many pawns");
	case PawnsOn18:
		return tr("There are pawns on the first or eighth rank");
	case TooManyKings:
		return tr("Too many kings");
	case TooManyBlack:
		return tr("Too many black pieces");
	case TooManyWhite:
		return tr("Too many white pieces");
	case BadCastlingRights:
		return tr("Bad castling rights");
	case InvalidEnPassant:
		return tr("En passant square is not correct");
	default:
		return tr("Unknown reason");
	}
}

void BoardSetupDialog::setStatusMessage()
{
	QString reason = boardStatusMessage();
	ui.okButton->setEnabled(reason.isEmpty());
	if (reason.isEmpty())
		ui.fenLabel->setText(tr("FEN: %1").arg(ui.boardView->board().toFen()));
	else
		ui.fenLabel->setText(tr("Illegal position: %1").arg(reason));
}

void BoardSetupDialog::slotCopyFen()
{
	QApplication::clipboard()->setText(ui.boardView->board().toFen());
}

void BoardSetupDialog::slotPasteFen()
{
	QString fen = QApplication::clipboard()->text().trimmed();
	if (!ui.boardView->board().isValidFen(fen)) {
		QString msg = fen.length() ?
			      tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
				tr("There is no text in clipboard.");
		QMessageBox::warning(0, "Paste FEN", msg);
	}
	else {
		Board b;
		b.fromFen(fen);
		setBoard(b);
	}
}




