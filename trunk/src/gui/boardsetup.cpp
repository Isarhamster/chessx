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
#include "boardsetuptoolbutton.h"

#include <algorithm>
#include <QtGui>
#include <QPixmap>
#include <QPushButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

BoardSetupDialog::BoardSetupDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f),
    m_wheelCurrentDelta(0), m_selectedPiece(Empty), inDrag(false)
{
    setObjectName("BoardSetupDialog");
    ui.setupUi(this);

    QPushButton *pasteButton = ui.buttonBox->addButton(tr("Paste FEN"), QDialogButtonBox::ActionRole);
    copyButton = ui.buttonBox->addButton(tr("Copy FEN"), QDialogButtonBox::ApplyRole);
    btCopyText = ui.buttonBox->addButton(tr("Copy Text"), QDialogButtonBox::ApplyRole);

    restoreLayout();

    ui.boardView->configure();
    ui.boardView->setFlags(BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove | BoardView::AllowCopyPiece);
    ui.boardView->showMoveIndicator(false);
    ui.boardView->showCoordinates(true);

    m_minDeltaWheel = AppSettings->getValue("/Board/minWheelCount").toInt();

    for(int piece = Empty; piece <= BlackPawn; piece++)
    {
        BoardSetupToolButton* button = new BoardSetupToolButton(this);
        button->setPiece(static_cast<Piece>(piece));
        if(piece == Empty)
        {
            button->setBasePixmap(QPixmap(0, 0));
            ui.buttonLayout->addWidget(button, 6, 0);
        }
        else
        {
            button->setBasePixmap(ui.boardView->theme().piece(Piece(piece)));
            ui.buttonLayout->addWidget(button, (piece - 1) % 6, piece >= BlackKing);
        }
        connect(button, SIGNAL(signalDragStarted(QWidget*, QMouseEvent*)), this, SLOT(startDrag(QWidget*, QMouseEvent*)));
        connect(button, SIGNAL(signalClicked(Piece)), this, SLOT(labelClicked(Piece)));
        connect(this, SIGNAL(signalClearBackground(Piece)), button, SLOT(slotClearBackground(Piece)));
    }

    ui.buttonLayout->setColumnStretch(0,1);
    ui.buttonLayout->setColumnStretch(1,1);
    emit signalClearBackground(Empty);

    ui.buttonBoxTools->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Clear"));
    connect(ui.buttonBoxTools->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()), SLOT(slotClear()));
    connect(ui.buttonBoxTools->button(QDialogButtonBox::Reset), SIGNAL(clicked()), SLOT(slotReset()));

    connect(ui.boardView, SIGNAL(clicked(Square, int, QPoint, Square)), SLOT(slotSelected(Square, int)));
    connect(ui.boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotMovePiece(Square, Square)));
    connect(ui.boardView, SIGNAL(copyPiece(Square, Square)), SLOT(slotCopyPiece(Square, Square)));
    connect(ui.boardView, SIGNAL(invalidMove(Square)), SLOT(slotInvalidMove(Square)));
    connect(ui.boardView, SIGNAL(wheelScrolled(int)), SLOT(slotChangePiece(int)));
    connect(ui.boardView, SIGNAL(pieceDropped(Square, Piece)), SLOT(slotDroppedPiece(Square, Piece)));
    connect(ui.toMoveButton, SIGNAL(clicked()), SLOT(slotToggleSide()));
    connect(ui.wkCastleCheck, SIGNAL(clicked(bool)), SLOT(slotCastlingRights()));
    connect(ui.wqCastleCheck, SIGNAL(clicked(bool)), SLOT(slotCastlingRights()));
    connect(ui.bkCastleCheck, SIGNAL(clicked(bool)), SLOT(slotCastlingRights()));
    connect(ui.bqCastleCheck, SIGNAL(clicked(bool)), SLOT(slotCastlingRights()));
    connect(ui.castleFile00, SIGNAL(activated(int)), SLOT(slotCastlingRights()));
    connect(ui.castleFile000, SIGNAL(activated(int)), SLOT(slotCastlingRights()));
    connect(ui.epCombo, SIGNAL(currentIndexChanged(int)), SLOT(slotEnPassantSquare()));
    connect(ui.btCheck960, SIGNAL(clicked(bool)), SLOT(slotChess960()));
    connect(ui.halfmoveSpin, SIGNAL(valueChanged(int)), SLOT(slotHalfmoveClock()));
    connect(ui.moveSpin, SIGNAL(valueChanged(int)), SLOT(slotMoveNumber()));
    connect(ui.btFlipBoard, SIGNAL(clicked()), ui.boardView, SLOT(flip()));
    ui.btFlipBoard->setCheckable(true);

    connect(copyButton, SIGNAL(clicked()), SLOT(slotCopyFen()));
    connect(pasteButton, SIGNAL(clicked()), SLOT(slotPasteFen()));
    connect(btCopyText, SIGNAL(clicked()), SLOT(slotCopyText()));

    connect(ui.btFlipVertical, SIGNAL(clicked()), SLOT(mirrorVertical()));
    connect(ui.btFlipHorizontal, SIGNAL(clicked()), SLOT(mirrorHorizontal()));
    connect(ui.btSwapColor, SIGNAL(clicked()), SLOT(swapColors()));

    connect(ui.chess960pos, SIGNAL(valueChanged(int)), SLOT(chess960posChanged(int)));
    connect(ui.randomChess960, SIGNAL(clicked()), SLOT(chess960randomPos()));

    ui.tabWidget_2->setCurrentIndex(0);
}

void BoardSetupDialog::restoreLayout()
{
    AppSettings->layout(this);
}

BoardSetupDialog::~BoardSetupDialog()
{}

Board BoardSetupDialog::board() const
{
    Board b = ui.boardView->board();
    return b;
}

void BoardSetupDialog::chess960posChanged(int value)
{
    if (value >= 0)
    {
        Board b;
        b.fromChess960pos(value);
        setBoard(b);
    }
}

void BoardSetupDialog::chess960randomPos()
{
    int randomPos = rand() % 960;
    ui.chess960pos->setValue(randomPos);
}

void BoardSetupDialog::mirrorVertical()
{
    QString fen = board().toFen();
    QString fenRows = fen.left(fen.indexOf(" "));
    QStringList rows = fenRows.split("/");
    std::reverse(rows.begin(), rows.end());
    QString newFen = rows.join("/");
    Board b(newFen);
    b.setMoveNumber(board().moveNumber());
    b.setHalfMoveClock(board().halfMoveClock());
    setBoard(b);
    repaint(); // Workaround Bug in Qt at least up to version 5.12
}

void BoardSetupDialog::mirrorHorizontal()
{
    QString fen = board().toFen();
    QString fenRows = fen.left(fen.indexOf(" "));
    QStringList rows = fenRows.split("/");
    for(QStringList::Iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        std::reverse((*iter).begin(), (*iter).end());
    }
    QString newFen = rows.join("/");
    Board b(newFen);
    b.setMoveNumber(board().moveNumber());
    b.setHalfMoveClock(board().halfMoveClock());
    setBoard(b);
    repaint(); // Workaround Bug in Qt at least up to version 5.12
}

void BoardSetupDialog::swapColors()
{
    QString fen = board().toFen();
    QString fenRows = fen.left(fen.indexOf(" "));
    QStringList rows = fenRows.split("/");
    for(QStringList::Iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        QString& fenString = (*iter);
        for(QString::Iterator pChar = fenString.begin(); pChar != fenString.end(); ++pChar)
        {
            if(*pChar >= 'a' && *pChar <= 'z')
            {
                (*pChar) = (*pChar).toUpper();
            }
            else if(*pChar >= 'A' && *pChar <= 'Z')
            {
                (*pChar) = (*pChar).toLower();
            }
        }
    }
    QString newFen = rows.join("/");
    Board b(newFen);
    b.setMoveNumber(board().moveNumber());
    b.setHalfMoveClock(board().halfMoveClock());
    setBoard(b);
    repaint(); // Workaround Bug in Qt at least up to version 5.12
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
    if(b.enPassantSquare() == NoEPSquare)
    {
        ui.epCombo->setCurrentIndex(0);
    }
    else if(b.toMove() == White && b.pieceAt(b.enPassantSquare() - 8) == BlackPawn &&
            b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() + 8) == Empty)
    {
        ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
    }
    else if(b.toMove() == Black && b.pieceAt(b.enPassantSquare() + 8) == WhitePawn &&
            b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() - 8) == Empty)
    {
        ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
    }
    else
    {
        ui.epCombo->setCurrentIndex(0);
    }
    ui.wkCastleCheck->setChecked(b.canCastleShort(White));
    ui.wqCastleCheck->setChecked(b.canCastleLong(White));
    ui.bkCastleCheck->setChecked(b.canCastleShort(Black));
    ui.bqCastleCheck->setChecked(b.canCastleLong(Black));
    m_toMove = b.toMove();

    ui.btCheck960->setChecked(b.chess960());
    ui.chess960pos->setEnabled(b.chess960());
    ui.randomChess960->setEnabled(b.chess960());

    if (b.chess960())
    {
        int ccPos = b.chess960Pos();
        ui.chess960pos->setValue(ccPos);
    }

    ui.castleFile00->setEnabled(b.hasAmbiguousCastlingRooks() &&
                                (b.canCastleShort(White) || b.canCastleShort(Black)));
    ui.castleFile000->setEnabled(b.hasAmbiguousCastlingRooks() &&
                                 (b.canCastleLong(White) || b.canCastleLong(Black)));

    char index00 = 0;
    if(b.canCastleShort(White))
    {
        index00 = 'a'+File(b.CastlingRook(1));
    }
    else if(b.canCastleShort(Black))
    {
        index00 = 'a'+File(b.CastlingRook(3));
    }
    char index000 = 0;
    if(b.canCastleLong(White))
    {
        index000 = 'a'+File(b.CastlingRook(0));
    }
    else if(b.canCastleLong(Black))
    {
        index000 = 'a'+File(b.CastlingRook(2));
    }

    ui.castleFile00->clear();
    ui.castleFile000->clear();
    if (b.chess960())
    {
        for (char i='a';i<='f';++i)
        {
            if (b.HasRookOnFileForCastling(i-'a', true))
            {
                ui.castleFile000->addItem(QString(i));
            }
        }
        if (!ui.castleFile000->count()) ui.castleFile000->setEnabled(false);
        for (char i='c';i<='h';++i)
        {
            if (b.HasRookOnFileForCastling(i-'a', false))
            {
                ui.castleFile00->addItem(QString(i));
            }
        }
        if (!ui.castleFile00->count()) ui.castleFile00->setEnabled(false);
        if (index00 && ui.castleFile00->findText(QString(index00)))
        {
            ui.castleFile00->setCurrentText(QString(index00));
        }
        if (index000 && ui.castleFile000->findText(QString(index000)))
        {
            ui.castleFile000->setCurrentText(QString(index000));
        }
    }
    else
    {
        ui.castleFile00->addItem("h");
        ui.castleFile000->addItem("a");
    }

    showSideToMove();
    setStatusMessage();
}

void BoardSetupDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    adjustSize();
}

void BoardSetupDialog::slotReset()
{
    Board b;
    b.setStandardPosition();
    setBoard(b);
    slotCastlingRights();
}

void BoardSetupDialog::accept()
{
    // Need to make sure the board is updated with move number set by user
    Board b(ui.boardView->board());
    b.setMoveNumber(ui.moveSpin->value());
    ui.boardView->setBoard(b);

    AppSettings->setLayout(this);

    QString reason = boardStatusMessage();
    if(reason.isEmpty())
    {
        QDialog::accept();
    }
    else
    {
        MessageDialog::error(tr("Current position is not valid.\n\n%1.").arg(reason));
    }
}

void BoardSetupDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void BoardSetupDialog::slotClear()
{
    Board b;
    b.setAt(e1, WhiteKing);
    b.setAt(e8, BlackKing);
    setBoard(b);
    slotCastlingRights();
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
    Piece piece = (button & Qt::MidButton) ? Empty : m_selectedPiece;
    if(button & Qt::RightButton)
    {
        if(piece >= BlackKing)
        {
            piece = static_cast<Piece>(piece - (BlackKing - WhiteKing));
        }
        else if(piece != Empty)
        {
            piece = static_cast<Piece>(piece + (BlackKing - WhiteKing));
        }
    }
    Board board = ui.boardView->board();
    if(board.pieceAt(square) == piece)
    {
        piece = Empty;
    }
    board.setAt(square, piece);
    setBoard(board);
    slotCastlingRights();
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
    repaint(); // Workaround Bug in Qt at least up to version 5.12
}

void BoardSetupDialog::slotChangePiece(int dir)
{
    int i = m_selectedPiece;
    i += (dir == BoardView::WheelUp) ? -1 : 1;
    if(i < 0)
    {
        i = static_cast<int>(BlackPawn);
    }
    else if(i > BlackPawn)
    {
        i = static_cast<int>(Empty);
    }
    m_selectedPiece = static_cast<Piece>(i);
    emit signalClearBackground(m_selectedPiece);
}

void BoardSetupDialog::slotDroppedPiece(Square s, Piece p)
{
    Board b = ui.boardView->board();
    b.setAt(s, p);
    setBoard(b);
    slotCastlingRights();
}

void BoardSetupDialog::slotMovePiece(Square from, Square to)
{
    Board b = ui.boardView->board();
    Piece p = b.pieceAt(from);
    b.removeFrom(from);
    b.setAt(to, p);
    setBoard(b);
    slotCastlingRights();
}

void BoardSetupDialog::slotCopyPiece(Square from, Square to)
{
    Board b = ui.boardView->board();
    Piece p = b.pieceAt(from);
    b.setAt(to, p);
    setBoard(b);
    slotCastlingRights();
}

void BoardSetupDialog::slotInvalidMove(Square from)
{
    Board b = ui.boardView->board();
    Piece p = b.pieceAt(from);
    if(pieceType(p) != King)
    {
        b.removeFrom(from);
        setBoard(b);
    }
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if(abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        slotChangePiece(m_wheelCurrentDelta < 0 ? BoardView::WheelDown : BoardView::WheelUp);
        m_wheelCurrentDelta = 0;
    }
}

QString BoardSetupDialog::boardStatusMessage() const
{
    switch(ui.boardView->board().validate())
    {
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
    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(reason.isEmpty());
    copyButton->setEnabled(reason.isEmpty());
    btCopyText->setEnabled(reason.isEmpty());
    if(reason.isEmpty())
    {
        ui.fenLabel->setText(ui.boardView->board().toFen());
        ui.humanFEN->setText(ui.boardView->board().toHumanFen());
    }
    else
    {
        ui.fenLabel->setText(tr("Illegal position: %1").arg(reason));
        ui.humanFEN->setText("");
    }
}

void BoardSetupDialog::slotCopyFen()
{
    QApplication::clipboard()->setText(ui.boardView->board().toFen());
}

void BoardSetupDialog::slotCopyText()
{
    QApplication::clipboard()->setText(ui.boardView->board().toHumanFen());
}

void BoardSetupDialog::slotPasteFen()
{
    // Prepare Fen - clean up code like this:
    // [FEN "***"] to ***
    QString fen = QApplication::clipboard()->text().simplified();
    if(fen.contains("\""))
    {
        int n1 = fen.indexOf('"');
        int n2 = fen.lastIndexOf('"');
        if(n2 > n1 + 1)
        {
            fen = fen.mid(n1 + 1, n2 - n1 - 1);
        }
    }

    // Another go at Fens copied from Wikis: [FEN]***[/FEN] is reduced to ***
    fen.remove(QRegExp("\\[[^\\]]*\\]"));

    // Now parse the hopefully naked Fen
    Board b;
    if(!b.fromFen(fen))
    {
        QString msg = fen.length() ?
                      tr("Text in clipboard does not represent valid FEN:<br><i>%1</i>").arg(fen) :
                      tr("There is no text in clipboard.");
        MessageDialog::warning(msg);
    }
    else
    {
        if (b.canCastleShort(White))
        {
            ui.castleFile00->setCurrentText(QString('a'+File(b.CastlingRook(1))));
        }
        else if (b.canCastleShort(Black))
        {
            ui.castleFile00->setCurrentText(QString('a'+File(b.CastlingRook(3))));
        }
        if (b.canCastleLong(White))
        {
            ui.castleFile000->setCurrentText(QString('a'+File(b.CastlingRook(0))));
        }
        else if (b.canCastleLong(Black))
        {
            ui.castleFile000->setCurrentText(QString('a'+File(b.CastlingRook(2))));
        }
        setBoard(b);
    }
}

char BoardSetupDialog::castlingFile00() const
{
    return ui.castleFile00->isEnabled() ? ui.castleFile00->currentText().at(0).toLatin1() : 0;
}

char BoardSetupDialog::castlingFile000() const
{
    return ui.castleFile000->isEnabled() ? ui.castleFile000->currentText().at(0).toLatin1() : 0;
}

void BoardSetupDialog::slotCastlingRights()
{
    Board b(board());
    CastlingRights cr = 0;
    // Do not test rook positions to allow for Chess960 setups

    if(ui.wkCastleCheck->isChecked())
    {
        cr += WhiteKingside;
    }
    if(ui.wqCastleCheck->isChecked())
    {
        cr += WhiteQueenside;
    }
    if(ui.bkCastleCheck->isChecked())
    {
        cr += BlackKingside;
    }
    if(ui.bqCastleCheck->isChecked())
    {
        cr += BlackQueenside;
    }
    b.setCastlingRights(cr);
    b.setCastlingRooks(castlingFile000(),castlingFile00());
    setBoard(b);
}

void BoardSetupDialog::slotEnPassantSquare()
{
    Board b(board());
    if(ui.epCombo->currentIndex() == 0)
    {
        b.clearEnPassantSquare();
    }
    else
    {
        Square shift = (b.toMove() == White) ? h5 : h2;
        b.setEnPassantSquare(shift + ui.epCombo->currentIndex());
    }
    setBoard(b);
}

void BoardSetupDialog::slotChess960()
{
    Board b(board());
    b.setChess960(ui.btCheck960->isChecked());
    setBoard(b);
    slotCastlingRights();
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
    if (inDrag)
    {
        return;
    }
    BoardSetupToolButton *child = qobject_cast<BoardSetupToolButton*>(w);
    if(!child)
    {
        return;
    }
    Piece p = child->piece();

    QPoint hotSpot = event->pos();

    BoardViewMimeData *mimeData = new BoardViewMimeData;
    mimeData->m_piece = p;

    QPixmap pixmap = child->BasePixmap();
    QSize pieceSize = ui.boardView->themeSize();
    qreal r = pixmap.devicePixelRatio();

    pixmap = pixmap.scaled(pieceSize*r, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    pDrag->setHotSpot(hotSpot);

    inDrag = true;
    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    inDrag = false;
}

void BoardSetupDialog::labelClicked(Piece p)
{
    m_selectedPiece = p;
    emit signalClearBackground(m_selectedPiece);
}
