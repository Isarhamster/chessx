/***************************************************************************
                      BoardView - view of the current board
                             -------------------
    begin                : Sun 21 Aug 2005
    copyright            : (C) 2005-2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "boardview.h"
#include "settings.h"
#include "guess.h"

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QEvent>

using namespace Qt;

BoardView::BoardView(QWidget* parent, int flags) : QWidget(parent),
		m_flipped(false), m_showFrame(false), m_guessMove(false), m_selectedSquare(InvalidSquare),
		m_hoverSquare(InvalidSquare), m_hifrom(InvalidSquare), m_hito(InvalidSquare), m_flags(flags),
		m_dragged(Empty), m_clickUsed(false)
{
	setMouseTracking(true);
	installEventFilter(this);
}

BoardView::~BoardView()
{}

bool BoardView::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Leave || ev->type() == QEvent::WindowDeactivate)
		removeGuess();
	return QWidget::eventFilter(obj, ev);
}

void BoardView::setFlags(int flags)
{
	m_flags = flags;
}

void BoardView::setBoard(const Board& value)
{
	m_clickUsed = true;
	Board oldboard = m_board;
	m_board = value;
	updateGuess(m_hoverSquare);
	update();
}

Board BoardView::board() const
{
	return m_board;
}

const BoardTheme& BoardView::theme() const
{
	return m_theme;
}

void BoardView::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	// Draw squares
	for (Square square = 0; square < 64; square++) {
		QRect rect = squareRect(square);
		if (!event->region().intersects(rect))
			continue;
		int x = isFlipped() ? 7 - square % 8 : square % 8;
		int y = isFlipped() ? square / 8 : 7 - square / 8;
		QPoint pos(x * m_theme.size().width(), y * m_theme.size().height());
		p.drawPixmap(pos, m_theme.square((x + y) % 2));
		p.drawPixmap(pos, m_theme.piece(m_board.at(square)));
		if (square == m_selectedSquare || square == m_hifrom || square == m_hito) {
			QPen pen;
			pen.setColor(m_theme.color(BoardTheme::Highlight));
			pen.setWidth(2);
			pen.setJoinStyle(Qt::MiterJoin);
			p.setPen(pen);
			p.drawRect(pos.x() + 1 + m_showFrame, pos.y() + 1 + m_showFrame,
				   m_theme.size().width() - 2 - m_showFrame, m_theme.size().height() - 2 - m_showFrame);
		}
		if (m_showFrame) {
			p.setPen(m_theme.color(BoardTheme::Frame));
			p.drawRect(QRect(pos, m_theme.size()));
		}
	}
	// Draw side to move indicator
	bool white = m_board.toMove() == White;
	int square = m_theme.size().width() / 3;
	QColor color = white ? Qt::white : Qt::black;
	QColor border = white ? Qt::black : Qt::white;
	int posy = (white == m_flipped) ? 1 : 8 * m_theme.size().width() - square;
	p.setPen(border);
	p.setBrush(QColor(color));
	p.drawRect(8 * m_theme.size().width() + 8, posy, square, square);

	// Fix border up
	if (m_showFrame) {
		QPen pen;
		pen.setColor(m_theme.color(BoardTheme::Frame));
		pen.setWidth(1);
		p.setPen(pen);
		int flx = m_theme.size().width() * 8;
		int fly = m_theme.size().height() * 8;
		p.drawLine(flx, 0, flx, fly);
		p.drawLine(0, fly, flx, fly);
	}

	// Draw dragged piece
	if (m_dragged != Empty)
		p.drawPixmap(m_dragPoint, m_theme.piece(m_dragged));
}

void BoardView::resizeBoard()
{
	// subtract move indicator from width
	int xsize = (width() - (8 + width() / 24) - 1) / 8;
	int ysize = (height() - 1) / 8;
	int size = xsize < ysize ? xsize : ysize;
	m_theme.setSize(QSize(size, size));
}

void BoardView::resizeEvent(QResizeEvent*)
{
	resizeBoard();
}

Square BoardView::squareAt(const QPoint& p) const
{
	int x = p.x(), y = p.y();
	int width = m_theme.size().width();
	int height = m_theme.size().height();
	if (x <= 0 || y <= 0 || x >= width*8 || y >= height*8)
		return InvalidSquare;
	x /= width;
	y /= height;
	return isFlipped() ? (8 * y + 7 - x) : (8 *(7 - y) + x);
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
	m_dragStart = event->pos();
}

void BoardView::showGuess(Square s)
{
	// Don't want to constantly recalculate guess, so remember which square
	// the mouse is hovering over, and only show new guess when it changes
	if (m_guessMove && s != m_hoverSquare && !(m_flags & SuppressGuessMove)) {
		m_hoverSquare = s;
		removeGuess();
		Guess::Result sm = Guess::guessMove(qPrintable(m_board.toFEN()), (int) s);
		if (!sm.error) {
			m_hifrom = sm.from;
			m_hito = sm.to;
			update(squareRect(m_hifrom));
			update(squareRect(m_hito));
		}
	}
}

void BoardView::updateGuess(Square s)
{
	// Invalidate any currently displayed guess to allow new guess to show
	m_hoverSquare = InvalidSquare;
	showGuess(s);
}

void BoardView::removeGuess()
{
	if (m_hifrom != InvalidSquare) {
		update(squareRect(m_hifrom));
		update(squareRect(m_hito));
		m_hifrom = m_hito = InvalidSquare;
	}
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton)) {
		showGuess(squareAt(event->pos()));
		return;
	}
	if (m_dragged != Empty) {
		QRect old = QRect(m_dragPoint, m_theme.size());
		m_dragPoint = event->pos() - m_theme.pieceCenter();
		update(old);
		update(QRect(m_dragPoint, m_theme.size()));
		return;
	}
	if ((event->pos() - m_dragStart).manhattanLength()
			< QApplication::startDragDistance())  // Click and move - start dragging
		return;
	Square s = squareAt(m_dragStart);
	if (!canDrag(s))
		return;
	removeGuess();
	m_dragged = m_board.at(s);
	m_dragPoint = event->pos() - m_theme.pieceCenter();
	m_board.removeFrom(s);
	update(squareRect(s));
	update(QRect(m_dragPoint, m_theme.size()));
	unselectSquare();
	QPixmap icon = m_theme.piece(m_dragged);
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
	Square s = squareAt(event->pos());
	m_clickUsed = false;
	if (event->button() != Qt::LeftButton) {
		if (s != InvalidSquare)
			emit clicked(s, event->button() + event->modifiers());
		return;
	}

	if (m_dragged != Empty) {
		Square from = squareAt(m_dragStart);
		m_board.setAt(from, m_dragged);
		QRect oldr = QRect(m_dragPoint, m_theme.size());
		m_dragged = Empty;
		update(squareRect(from));
		update(oldr);
		if (s != InvalidSquare)
			emit moveMade(from, s);
		updateGuess(s);
	} else if (m_selectedSquare != InvalidSquare) {
		Square from = m_selectedSquare;
		unselectSquare();
		if (s != InvalidSquare)
			emit moveMade(from, s);
	} else if (m_hifrom != InvalidSquare) {
		if (s == m_hifrom || s == m_hito)
			emit moveMade(m_hifrom, m_hito);
		m_hoverSquare = InvalidSquare;
		// Only update guess if "emit moveMade()" did not pop up a window (eg. promotion)
		if (m_hifrom != InvalidSquare)
			updateGuess(s);
	} else {
		if (s != InvalidSquare)
			emit clicked(s, event->button() + event->modifiers());
		if (!m_clickUsed && isPieceColor(m_board.at(s), m_board.toMove()))
			selectSquare(s);
	}
}

void BoardView::wheelEvent(QWheelEvent* e)
{
	int change = e->delta() < 0 ? WheelDown : WheelUp;
	emit wheelScrolled(change + e->modifiers());
}

bool BoardView::setTheme(const QString& pieceFile, const QString& boardFile)
{
	bool result = m_theme.load(pieceFile, boardFile);
	if (!result) {
		QMessageBox::warning(0, tr("Error"), tr("<qt>Cannot open theme <b>%1</b> from directory:<br>%2</qt>")
				     .arg(pieceFile).arg(m_theme.themeDirectory()));
		// If there is no theme, try to load default
		if (!m_theme.isValid()) {
			result = m_theme.load("merida");
			if (result)
				resizeBoard();
		}
	}
	if (result)
		update();
	return result;
}

void BoardView::setFlipped(bool flipped)
{
	m_flipped = flipped;
	update();
}

bool BoardView::isFlipped() const
{
	return m_flipped;
}

void BoardView::configure()
{
	AppSettings->beginGroup("/Board/");
	m_showFrame = AppSettings->value("showFrame", true).toBool();
	m_guessMove = AppSettings->value("guessMove", true).toBool();
	m_theme.setColor(BoardTheme::LightSquare, AppSettings->value("lightColor", "#d0d0d0").value<QColor>());
	m_theme.setColor(BoardTheme::DarkSquare, AppSettings->value("darkColor", "#a0a0a0").value<QColor>());
	m_theme.setColor(BoardTheme::Highlight, AppSettings->value("highlightColor",
			 "#ffff00").value<QColor>());
	m_theme.setColor(BoardTheme::Frame, AppSettings->value("frameColor", "#000000").value<QColor>());
	QString pieceTheme = AppSettings->value("pieceTheme", "merida").toString();
	QString boardTheme = AppSettings->value("boardTheme", "merida").toString();
	setTheme(pieceTheme, boardTheme);
	AppSettings->endGroup();
	removeGuess();
	unselectSquare();
	update();
}

void BoardView::selectSquare(Square s)
{
	// You can't select a square when guess move is enabled
	if (m_guessMove && !(m_flags & SuppressGuessMove))
		return;
	if (m_selectedSquare == s)
		return;
	unselectSquare();
	m_selectedSquare = s;
	update(squareRect(s));
}

void BoardView::unselectSquare()
{
	Square prev = m_selectedSquare;
	m_selectedSquare = InvalidSquare;
	if (prev != InvalidSquare)
		update(squareRect(prev));
}

QRect BoardView::squareRect(Square square)
{
	int x = isFlipped() ? 7 - square % 8 : square % 8;
	int y = isFlipped() ? square / 8 : 7 - square / 8;
	return QRect(QPoint(x * m_theme.size().width(), y * m_theme.size().height()),
		     m_theme.size());
}

bool BoardView::canDrag(Square s) const
{
	if (m_dragged != Empty) // already dragging
		return false;
	if (s == InvalidSquare)
		return false;
	else if (m_flags & IgnoreSideToMove)
		return m_board.at(s) != Empty;
	else return isPieceColor(m_board.at(s), m_board.toMove());
}

