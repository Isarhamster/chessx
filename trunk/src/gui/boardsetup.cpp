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
#include "messagedialog.h"
#include "settings.h"
#include <QtGui>
#include "boardsetuptoolbutton.h"


BoardSetupDialog::BoardSetupDialog(QWidget* parent) : QDialog(parent), m_wheelCurrentDelta(0), m_selectedPiece(Empty)
{
	ui.setupUi(this);
	ui.boardView->configure();
    ui.boardView->setFlags(BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove | BoardView::AllowCopyPiece);
    ui.boardView->showMoveIndicator(false);

    m_minDeltaWheel = AppSettings->getValue("/Board/minWheelCount").toInt();

	for (int piece = Empty; piece <= BlackPawn; piece++) {
        BoardSetupToolButton* button = new BoardSetupToolButton(this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //button->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        button->setMinimumSize(QSize(10,10));
        button->m_piece = (Piece)piece;
        if (piece == Empty)
        {
            button->m_pixmap = 0;
            ui.buttonLayout->addWidget(button, 6, 0);
        }
		else
        {
            button->m_pixmap = ui.boardView->theme().piece(Piece(piece));
            ui.buttonLayout->addWidget(button, (piece - 1) % 6, piece >= BlackKing);
        }
        connect(button, SIGNAL(signalDragStarted(QWidget*,QMouseEvent*)), this, SLOT(startDrag(QWidget*,QMouseEvent*)));
        connect(button, SIGNAL(signalClicked(Piece)), this, SLOT(labelClicked(Piece)));
        connect(this, SIGNAL(signalClearBackground(Piece)),button, SLOT(slotClearBackground(Piece)));
	}

    emit signalClearBackground(Empty);

	connect(ui.okButton, SIGNAL(clicked()), SLOT(slotAccept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
	connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
	connect(ui.boardView, SIGNAL(clicked(Square, int)), SLOT(slotSelected(Square, int)));
    connect(ui.boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotMovePiece(Square, Square)));
    connect(ui.boardView, SIGNAL(copyPiece(Square, Square)), SLOT(slotCopyPiece(Square, Square)));
    connect(ui.boardView, SIGNAL(invalidMove(Square)), SLOT(slotInvalidMove(Square)));
	connect(ui.boardView, SIGNAL(wheelScrolled(int)), SLOT(slotChangePiece(int)));
    connect(ui.boardView, SIGNAL(pieceDropped(Square,Piece)), SLOT(slotDroppedPiece(Square, Piece)));
	connect(ui.toMoveButton, SIGNAL(clicked()), SLOT(slotToggleSide()));
	connect(ui.wkCastleCheck, SIGNAL(stateChanged(int)), SLOT(slotCastlingRights()));
	connect(ui.wqCastleCheck, SIGNAL(stateChanged(int)), SLOT(slotCastlingRights()));
	connect(ui.bkCastleCheck, SIGNAL(stateChanged(int)), SLOT(slotCastlingRights()));
	connect(ui.bqCastleCheck, SIGNAL(stateChanged(int)), SLOT(slotCastlingRights()));
	connect(ui.epCombo, SIGNAL(currentIndexChanged(int)), SLOT(slotEnPassantSquare()));
	connect(ui.halfmoveSpin, SIGNAL(valueChanged(int)), SLOT(slotHalfmoveClock()));
	connect(ui.moveSpin, SIGNAL(valueChanged(int)), SLOT(slotMoveNumber()));
	connect(ui.copyButton, SIGNAL(clicked()), SLOT(slotCopyFen()));
	connect(ui.pasteButton, SIGNAL(clicked()), SLOT(slotPasteFen()));
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
	ui.halfmoveSpin->setValue(b.halfMoveClock());
	if (b.enPassantSquare() == NoEPSquare)
		ui.epCombo->setCurrentIndex(0);
	else if (b.toMove() == White && b.pieceAt(b.enPassantSquare() - 8) == BlackPawn &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() + 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else if (b.toMove() == Black && b.pieceAt(b.enPassantSquare() + 8) == WhitePawn &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() - 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else ui.epCombo->setCurrentIndex(0);
	ui.wkCastleCheck->setChecked(b.castlingRights() & WhiteKingside);
	ui.wqCastleCheck->setChecked(b.castlingRights() & WhiteQueenside);
	ui.bkCastleCheck->setChecked(b.castlingRights() & BlackKingside);
	ui.bqCastleCheck->setChecked(b.castlingRights() & BlackQueenside);
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
	else MessageDialog::error(tr("Current position is not valid.\n\n%1.").arg(reason));
}

void BoardSetupDialog::slotClear()
{
	Board b;
	b.setAt(e1, WhiteKing);
	b.setAt(e8, BlackKing);
	setBoard(b);
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
    Piece piece = (button & Qt::MidButton) ? Empty : m_selectedPiece;
    if (button & Qt::RightButton) {
        if (piece >= BlackKing)
            piece = (Piece)(piece - (BlackKing - WhiteKing));
        else if (piece != Empty)
            piece = (Piece)(piece + (BlackKing - WhiteKing));
    }
    Board board = ui.boardView->board();
    if (board.pieceAt(square) == piece)
        piece = Empty;
    board.setAt(square, piece);
    setBoard(board);
}

void BoardSetupDialog::showSideToMove()
{
	QSize size = ui.toMoveButton->iconSize();
	QPixmap pixmap(size);
	QPainter painter(&pixmap);
	painter.fillRect(0, 0, size.width(), size.height(), m_toMove == White ? Qt::black : Qt::white);
	painter.fillRect(1, 1, size.width() - 2, size.height() - 2, m_toMove == White ? Qt::white : Qt::black);
	ui.toMoveButton->setIcon(QIcon(pixmap));
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
    int i = m_selectedPiece;
    i += (dir == BoardView::WheelUp) ? -1 : 1;
    if (i < 0) i = (int) BlackPawn;
    else if (i>BlackPawn) i = (int) Empty;
    m_selectedPiece = (Piece) i;
    emit signalClearBackground(m_selectedPiece);
}

void BoardSetupDialog::slotDroppedPiece(Square s, Piece p)
{
    Board b = ui.boardView->board();
    b.setAt(s, p);
    setBoard(b);
}

void BoardSetupDialog::slotMovePiece(Square from, Square to)
{
	Board b = ui.boardView->board();
	Piece p = b.pieceAt(from);
	b.removeFrom(from);
	b.setAt(to, p);
	setBoard(b);
}

void BoardSetupDialog::slotCopyPiece(Square from, Square to)
{
	Board b = ui.boardView->board();
	Piece p = b.pieceAt(from);
	b.setAt(to, p);
	setBoard(b);
}

void BoardSetupDialog::slotInvalidMove(Square from)
{
    Board b = ui.boardView->board();
    Piece p = b.pieceAt(from);
    if (pieceType(p) != King)
    {
        b.removeFrom(from);
        setBoard(b);
    }
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if (abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        slotChangePiece(m_wheelCurrentDelta < 0 ? BoardView::WheelDown : BoardView::WheelUp);
        m_wheelCurrentDelta = 0;
    }

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
    if (fen.contains("\""))
    {
        int n1 = fen.indexOf('"');
        int n2 = fen.lastIndexOf('"');
        if (n2>n1+1)
        {
            fen = fen.mid(n1+1,n2-n1-1);
        }
    }

    Board b;
    if (!b.fromFen(fen))
    {
		QString msg = fen.length() ?
					tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
					tr("There is no text in clipboard.");
		MessageDialog::warning(msg);
    }
    else
    {
		setBoard(b);
	}
}

void BoardSetupDialog::slotCastlingRights()
{
	Board b(board());
	CastlingRights cr = 0;
	if (ui.wkCastleCheck->isChecked()) // && b.pieceAt(4) == WhiteKing && b.pieceAt(7) == WhiteRook)
		cr += WhiteKingside;
	if (ui.wqCastleCheck->isChecked()) // && b.pieceAt(4) == WhiteKing && b.pieceAt(0) == WhiteRook)
		cr += WhiteQueenside;
	if (ui.bkCastleCheck->isChecked()) // && b.pieceAt(60) == BlackKing && b.pieceAt(63) == BlackRook)
		cr += BlackKingside;
	if (ui.bqCastleCheck->isChecked()) // && b.pieceAt(60) == BlackKing && b.pieceAt(56) == BlackRook)
		cr += BlackQueenside;
	b.setCastlingRights(cr);
	setBoard(b);
}

void BoardSetupDialog::slotEnPassantSquare()
{
	Board b(board());
	if (ui.epCombo->currentIndex() == 0)
		b.clearEnPassantSquare();
	else {
		int shift = b.toMove() == White ? 39 : 15;
		b.setEnPassantSquare(shift + ui.epCombo->currentIndex());
	}
	setBoard(b);
}

void BoardSetupDialog::slotHalfmoveClock()
{
	Board b(board());
	b.setHalfMoveClock(ui.halfmoveSpin->value());
	setBoard(b);
}

void BoardSetupDialog::slotMoveNumber()
{
	Board b(board());
	b.setMoveNumber(ui.moveSpin->value());
	setBoard(b);
}

void BoardSetupDialog::startDrag(QWidget* w, QMouseEvent* event)
{
    BoardSetupToolButton *child = qobject_cast<BoardSetupToolButton*>(w);
    if (!child)
        return;
    Piece p = child->m_piece;

    QPoint hotSpot = event->pos();

    BoardViewMimeData *mimeData = new BoardViewMimeData;
    mimeData->m_piece = p;

    QPixmap pixmap = *child->pixmap();

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    pDrag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction)
        child->close();
}

void BoardSetupDialog::labelClicked(Piece p)
{
    m_selectedPiece = p;
    emit signalClearBackground(m_selectedPiece);
}
