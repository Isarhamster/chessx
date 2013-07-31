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
#include "movelist.h"

#include <QApplication>
#include <QSizePolicy>

using namespace Qt;

const int CoordinateSize = 16;

BoardView::BoardView(QWidget* parent, int flags) : QWidget(parent),
    m_flipped(false), m_showFrame(false), m_showCurrentMove(true),
    m_guessMove(false), m_selectedSquare(InvalidSquare),
    m_hoverSquare(InvalidSquare),
    m_hiFrom(InvalidSquare), m_hiTo(InvalidSquare),
    m_currentFrom(InvalidSquare), m_currentTo(InvalidSquare),
    m_atLineEnd(true),
    m_flags(flags),
    m_coordinates(false), m_dragged(Empty), m_clickUsed(false),m_wheelCurrentDelta(0),
    m_minDeltaWheel(0),m_moveListCurrent(0),m_showMoveIndicator(true),m_DbIndex(0)
{
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	policy.setHeightForWidth(true);
	setSizePolicy(policy);
	setMouseTracking(true);
	installEventFilter(this);
}

BoardView::~BoardView()
{}

bool BoardView::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Leave || ev->type() == QEvent::WindowDeactivate)
    {
		removeGuess();
    }
	return QWidget::eventFilter(obj, ev);
}

void BoardView::setFlags(int flags)
{
	m_flags = flags;
}

void BoardView::setBoard(const Board& value,int from, int to, bool atLineEnd)
{
    m_clickUsed = true;
	Board oldboard = m_board;
	m_board = value;
    m_currentFrom = from;
    m_currentTo = to;
    m_atLineEnd = atLineEnd;
	if (underMouse())
    {
		updateGuess(m_hoverSquare);
    }
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

void BoardView::showMoveIndicator(bool visible)
{
    m_showMoveIndicator = visible;
}

void BoardView::drawSquares(QPaintEvent* event)
{
    QPainter p(this);
    for (Square square = 0; square < 64; square++) {
        QRect rect = squareRect(square);
        if (!event->region().intersects(rect))
            continue;
        int coord =  m_coordinates ? CoordinateSize : 0;
        int x = isFlipped() ? 7 - square % 8 : square % 8;
        int y = isFlipped() ? square / 8 : 7 - square / 8;
        QPoint pos(coord + x * m_theme.size().width(), y * m_theme.size().height());
        p.drawPixmap(pos, m_theme.square((x + y) % 2));
    }
}

void BoardView::drawCoordinates(QPaintEvent* event)
{
    if (m_coordinates)
    {
        QPainter p(this);
        p.save();
        p.setPen(m_theme.color(BoardTheme::Frame));
        for (Square square = 0; square < 8; square++) {
            QRect rect = coordinateRectVertical(square);
            if (!event->region().intersects(rect))
                continue;
            p.drawText(rect, Qt::AlignCenter, QString("%1").arg(square+1));
        }
        for (Square square = 0; square < 8; square++) {
            QRect rect = coordinateRectHorizontal(square);
            if (!event->region().intersects(rect))
                continue;
            p.drawText(rect, Qt::AlignCenter, QString("%1").arg(QChar('a'+square)));
        }
        p.restore();
    }
}

void BoardView::drawDraggedPieces(QPaintEvent* /*event*/)
{
    // Draw dragged piece
    if (m_dragged != Empty)
    {
        QPainter p(this);
        p.drawPixmap(m_dragPoint, m_theme.piece(m_dragged));
    }
}

void BoardView::drawMoveIndicator(QPaintEvent* event)
{
    if( m_showMoveIndicator )
    {
        QPainter p(this);
        // Draw side to move indicator
        int coord =  m_coordinates ? CoordinateSize : 0;
        bool white = m_board.toMove() == White;
        int square = width() - coord - 8 * m_theme.size().width() - 4;
        int maxsquare = m_theme.size().width() / 2;
        if (square > maxsquare)
        {
            square = maxsquare;
        }
        if (square > 8)
        {
            int posy = (white == m_flipped) ? 1 : 8 * m_theme.size().width() - square;
            QRect rect(8 * m_theme.size().width() + 2 + coord, posy, square, square);
            if (!event->region().intersects(rect))
                return;
            QColor color = white ? Qt::white : Qt::black;
            QColor border = white ? Qt::black : Qt::white;
            p.setPen(border);
            p.setBrush(QColor(color));
            p.drawRect(rect);
        }
    }
}

void BoardView::drawPieces(QPaintEvent* event)
{
    QPainter p(this);

    for (Square square = 0; square < 64; square++) {
        QRect rect = squareRect(square);
        if (!event->region().intersects(rect))
             continue;
        int coord =  m_coordinates ? CoordinateSize : 0;
        int x = isFlipped() ? 7 - square % 8 : square % 8;
        int y = isFlipped() ? square / 8 : 7 - square / 8;
        QPoint pos(coord + x * m_theme.size().width(), y * m_theme.size().height());

        p.drawPixmap(pos, m_theme.piece(m_board.pieceAt(square)));

        if (m_showCurrentMove)
        {
            if (square == m_currentFrom || square == m_currentTo) {
                QPen pen;
                pen.setColor(m_theme.color(BoardTheme::CurrentMove));
                pen.setWidth(2);
                pen.setJoinStyle(Qt::MiterJoin);
                p.setPen(pen);
                p.drawRect(pos.x() + 1 + m_showFrame, pos.y() + 1 + m_showFrame,
                        m_theme.size().width() - 2 - m_showFrame, m_theme.size().height() - 2 - m_showFrame);
            }
        }

        if (square == m_selectedSquare || square == m_hiFrom || square == m_hiTo) {
            QPen pen;
            pen.setColor(m_theme.color(BoardTheme::Highlight));
            pen.setWidth(2);
            pen.setJoinStyle(Qt::MiterJoin);
            p.setPen(pen);
            p.drawRect(pos.x() + 1 + m_showFrame, pos.y() + 1 + m_showFrame,
                    m_theme.size().width() - 2 - m_showFrame, m_theme.size().height() - 2 - m_showFrame);
        }

        if (m_showFrame)
        {
            p.setPen(m_theme.color(BoardTheme::Frame));
            p.drawRect(QRect(pos, m_theme.size()));
        }
    }
}

void BoardView::paintEvent(QPaintEvent* event)
{
    drawSquares(event);
    drawCoordinates(event);
    drawSquareAnnotations(event);
    drawPieces(event);
    drawMoveIndicator(event);
    drawArrowAnnotations(event);
    drawDraggedPieces(event);
}

void BoardView::resizeBoard(QSize sz)
{
	// subtract move indicator from width
    int coord = m_coordinates ? CoordinateSize : 0;
    int xsize = (sz.width()-1 - coord ) / 8;
    int ysize = (sz.height()-1 - coord ) / 8;
    int size = std::min (xsize,ysize);
	m_theme.setSize(QSize(size, size));
}

void BoardView::resizeEvent(QResizeEvent* e)
{
    resizeBoard(e->size());
}

Square BoardView::squareAt(const QPoint& p) const
{
	int x = p.x(), y = p.y();
	int width = m_theme.size().width();
	int height = m_theme.size().height();
    x -= m_coordinates ? CoordinateSize : 0;
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

bool BoardView::showGuess(Square s)
{
	// Don't want to constantly recalculate guess, so remember which square
	// the mouse is hovering over, and only show new guess when it changes
    if (m_guessMove && s != m_hoverSquare && !(m_flags & SuppressGuessMove))
    {
		m_hoverSquare = s;
		removeGuess();
        m_moveListCurrent = 0;
        m_moveList.Clear();
#ifdef USE_ECO_GUESS
		if (m_board.ecoMove(s, &m_hifrom, &m_hito)) {
			update(squareRect(m_hifrom));
			update(squareRect(m_hito));
        }
        else
#endif
        {
            Guess::Result sm = Guess::guessMove(qPrintable(m_board.toFen()), (int) s, m_moveList);
            if (!sm.error)
            {
                m_hiFrom = sm.from;
                m_hiTo = sm.to;
                update(squareRect(m_hiFrom));
                update(squareRect(m_hiTo));
			}
		}
        return true;
	}
    return false;
}

void BoardView::updateGuess(Square s)
{
	// Invalidate any currently displayed guess to allow new guess to show
	m_hoverSquare = InvalidSquare;
	showGuess(s);
}

void BoardView::removeGuess()
{
    if (m_hiFrom != InvalidSquare)
    {
        update(squareRect(m_hiFrom));
        update(squareRect(m_hiTo));
        m_hiFrom = m_hiTo = InvalidSquare;
	}
}

void BoardView::nextGuess(Square s)
{
    if (!showGuess(s))
    {
        if (m_moveList.Size() && m_moveListCurrent<m_moveList.Size())
        {
            Guess::simpleMoveT * sold = m_moveList.Get(m_moveListCurrent);
            update(squareRect(sold->from));
            update(squareRect(sold->to));

            if (m_moveListCurrent<m_moveList.Size()-1)
            {
                ++m_moveListCurrent;
            }
            else
            {
                m_moveListCurrent = 0;
            }

            Guess::simpleMoveT * sm = m_moveList.Get(m_moveListCurrent);
            m_hiFrom = sm->from;
            m_hiTo = sm->to;
            update(squareRect(m_hiFrom));
            update(squareRect(m_hiTo));
        }
    }
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
    m_button = event->button() + event->modifiers();
    if (!m_atLineEnd && (event->modifiers() & Qt::ControlModifier))
    {
        if (event->modifiers() & Qt::AltModifier)
        {
            setCursor(QCursor(QPixmap(":/images/insert_move.png")));
        }
        else
        {
            setCursor(QCursor(QPixmap(":/images/replace_move.png")));
        }
    }
    else if (!m_atLineEnd && (event->modifiers() & Qt::AltModifier))
    {
        setCursor(QCursor(QPixmap(":/images/plus.png")));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

    if (!(event->buttons() & Qt::LeftButton))
    {
        if (!(event->modifiers() & Qt::ShiftModifier))
        {
            showGuess(squareAt(event->pos()));
        }
        else
        {
            removeGuess();
        }
		return;
	}

    if (event->modifiers() & Qt::ShiftModifier)
    {
        return;
    }

    if (m_dragged != Empty)
    {
		QRect old = QRect(m_dragPoint, m_theme.size());
		m_dragPoint = event->pos() - m_theme.pieceCenter();
		update(old);
		update(QRect(m_dragPoint, m_theme.size()));
		return;
	}

	if ((event->pos() - m_dragStart).manhattanLength()
            < QApplication::startDragDistance())
    {
        // Click and move - start dragging
		return;
    }

	Square s = squareAt(m_dragStart);
	if (!canDrag(s))
    {
		return;
    }
	removeGuess();
	m_dragged = m_board.pieceAt(s);
	m_dragPoint = event->pos() - m_theme.pieceCenter();
	m_board.removeFrom(s);
	update(squareRect(s));
	update(QRect(m_dragPoint, m_theme.size()));
	unselectSquare();
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    int button = event->button() + event->modifiers();
	Square s = squareAt(event->pos());
	m_clickUsed = false;

    if (!(event->button() & Qt::LeftButton))
    {
        Square from = squareAt(m_dragStart);
        if (s==from) from = InvalidSquare;
		if (s != InvalidSquare)
        {
            emit clicked(s, button, mapToGlobal(event->pos()), from);
        }
        else
        {
            Square from = squareAt(m_dragStart);
            emit invalidMove(from);
        }
        m_dragged = Empty;
		return;
	}
    else
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            if (s != InvalidSquare)
            {
                emit clicked(s, button, mapToGlobal(event->pos()), InvalidSquare);
            }
            return;
        }
    }

    if (m_dragged != Empty)
    {
		Square from = squareAt(m_dragStart);
		m_board.setAt(from, m_dragged);
		QRect oldr = QRect(m_dragPoint, m_theme.size());
		m_dragged = Empty;
		update(squareRect(from));
		update(oldr);
        if (s != InvalidSquare)
        {
            if ((m_flags & AllowCopyPiece) && (event->modifiers() & Qt::AltModifier))
            {
                if (m_board.pieceAt(from) != Empty)
                {
                    emit copyPiece(from, s);
                }
            }
            else
            {
                emit moveMade(from, s, button);
				updateGuess(s);
            }
		}
        else
        {
            emit invalidMove(from);
        }
    }
    else if (m_selectedSquare != InvalidSquare)
    {
		Square from = m_selectedSquare;
		unselectSquare();
		if (s != InvalidSquare)
        {
            emit moveMade(from, s, button);
        }
    }
    else if (m_hiFrom != InvalidSquare)
    {
        if (s == m_hiFrom || s == m_hiTo)
        {
            emit moveMade(m_hiFrom, m_hiTo, button);
        }
		m_hoverSquare = InvalidSquare;
		// Only update guess if "emit moveMade()" did not pop up a window (eg. promotion)
        if (m_hiFrom != InvalidSquare)
        {
			updateGuess(s);
        }
    }
    else
    {
        if (s != InvalidSquare)
        {
            emit clicked(s, button, mapToGlobal(event->pos()), InvalidSquare);
			if (!m_clickUsed && m_board.isMovable(s))
            {
				selectSquare(s);
            }
		}
	}
}

void BoardView::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if (abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        int change = m_wheelCurrentDelta < 0 ? WheelDown : WheelUp;
        emit wheelScrolled(change + e->modifiers());
        m_wheelCurrentDelta = 0;
    }
}

void BoardView::setFlipped(bool flipped)
{
	m_flipped = flipped;
	update();
}

void BoardView::flip()
{
    m_flipped = !m_flipped;
    update();
}

bool BoardView::isFlipped() const
{
	return m_flipped;
}

void BoardView::configure()
{
    AppSettings->beginGroup("/Board/");
    m_showFrame = AppSettings->getValue("showFrame").toBool();
    m_coordinates = AppSettings->getValue("showCoordinates").toBool();
    m_showCurrentMove = AppSettings->getValue("showCurrentMove").toBool();
    m_guessMove = AppSettings->getValue("guessMove").toBool();
    m_minDeltaWheel = AppSettings->getValue("minWheelCount").toInt();
	AppSettings->endGroup();
    m_theme.configure();
    m_theme.setEnabled(isEnabled());
    removeGuess();
	unselectSquare();
    if (size().height()>=minimumSize().height())
        resizeBoard(size());
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
    {
		update(squareRect(prev));
    }
}

QRect BoardView::squareRect(Square square)
{
    int coord =  m_coordinates ? CoordinateSize : 0;
	int x = isFlipped() ? 7 - square % 8 : square % 8;
	int y = isFlipped() ? square / 8 : 7 - square / 8;
    return QRect(QPoint(x * m_theme.size().width() + coord,
                        y * m_theme.size().height()),  m_theme.size());
}

QRect BoardView::coordinateRectVertical(Square square)
{
    Q_ASSERT(m_coordinates);
    int x = isFlipped() ? square % 8 : 7 - square % 8;
    return QRect(0, x * m_theme.size().height() + (m_theme.size().height()-CoordinateSize) / 2,
                        CoordinateSize, CoordinateSize);
}

QRect BoardView::coordinateRectHorizontal(Square square)
{
    Q_ASSERT(m_coordinates);
    int y = isFlipped() ? 7 - square % 8 : square % 8;
    return QRect(CoordinateSize + (y * (m_theme.size().width())) + (m_theme.size().width() - CoordinateSize) / 2,
                 8 * m_theme.size().height(),
                 CoordinateSize, CoordinateSize);
}

bool BoardView::canDrag(Square s) const
{
	if (m_dragged != Empty) // already dragging
		return false;
	if (s == InvalidSquare)
		return false;
	else if (m_flags & IgnoreSideToMove)
		return m_board.pieceAt(s) != Empty;
	else return m_board.isMovable(s);
}

int BoardView::heightForWidth(int width) const
{
    return width;
}

void BoardView::dragEnterEvent(QDragEnterEvent *event)
{
    const BoardViewMimeData *mimeData = qobject_cast<const BoardViewMimeData *>(event->mimeData());
    if (mimeData)
    {
        event->acceptProposedAction();
    }
}

void BoardView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void BoardView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void BoardView::dropEvent(QDropEvent *event)
{
    const BoardViewMimeData *mimeData = qobject_cast<const BoardViewMimeData *>(event->mimeData());
    if (mimeData)
    {
        Square s = squareAt(event->pos());
        emit pieceDropped(s, mimeData->m_piece);
        event->acceptProposedAction();
    }
}

void BoardView::drawSquareAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.squareAnnotation();

    if( !annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for (QStringList::Iterator it = list.begin(); it != list.end(); it++) {
            if (*it != "") {
                drawSquareAnnotation(event,*it);
            }
        }
    }
}

void BoardView::drawArrowAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.arrowAnnotation();

    if( !annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for (QStringList::Iterator it = list.begin(); it != list.end(); it++) {
            if (*it != "") {
                drawArrowAnnotation(event,*it);
            }
        }
    }
}

void BoardView::drawSquareAnnotation(QPaintEvent* event, QString annotation)
{
    QPainter p(this);
    QString trimmed = annotation.simplified();
    QChar colorChar = trimmed[0];
    QChar fileChar = trimmed[1];
    QChar rankChar = trimmed[2];
    QString files = "abcdefgh";
    QString ranks = "12345678";
    int file = files.indexOf(fileChar);
    int rank = ranks.indexOf(rankChar);
    int square = rank * 8 + file;

    QRect rect = squareRect(square);
    if (!event->region().intersects(rect))
        return;
    int x = isFlipped() ? 7 - square % 8 : square % 8;
    int y = isFlipped() ? square / 8 : 7 - square / 8;
    QPoint pos(x * m_theme.size().width(), y * m_theme.size().height());
    //p.drawPixmap(pos, m_theme.square((x + y) % 2));

    QColor color = Qt::red;
    if(colorChar == 'Y') {
        color = Qt::yellow;
    } else if(colorChar == 'G') {
        color = Qt::green;
    } else if(colorChar == 'B') {
        color = Qt::blue;
    }

    p.save();
    QPen pen(color);
    QBrush brush(color);
    p.setPen(pen);
    p.setBrush(brush);

    p.setOpacity(.20);

    int coord =  m_coordinates ? CoordinateSize : 0;
    p.drawRect(coord + pos.x(),pos.y(),m_theme.size().width(),m_theme.size().height());

    p.restore();

}

void BoardView::drawArrowAnnotation(QPaintEvent* event, QString annotation)
{
    QPainter p(this);

    static const QString letters = "abcdefgh";
    static const QString numbers = "12345678";

    QString trimmed = annotation.simplified();

    QChar colorChar = trimmed[0];
    QChar fileChar1 = trimmed[1];
    QChar rankChar1 = trimmed[2];
    QChar fileChar2 = trimmed[3];
    QChar rankChar2 = trimmed[4];
    int file1 = letters.indexOf(fileChar1);
    int rank1 = numbers.indexOf(rankChar1);
    int file2 = letters.indexOf(fileChar2);
    int rank2 = numbers.indexOf(rankChar2);

    if( file1 < 0 || file2 < 0 || rank1 < 0 || rank2 < 0 )
    {
        return;
    }
    int square1 = rank1 * 8 + file1;
    int square2 = rank2 * 8 + file2;

    QRect rect1 = squareRect(square1);
    QRect rect2 = squareRect(square2);
    QRect u = rect1.united(rect2);
    if (!event->region().intersects(u))
        return;
    int x1 = isFlipped() ? 7 - square1 % 8 : square1 % 8;
    int y1 = isFlipped() ? square1 / 8 : 7 - square1 / 8;
    int x2 = isFlipped() ? 7 - square2 % 8 : square2 % 8;
    int y2 = isFlipped() ? square2 / 8 : 7 - square2 / 8;
    int w = m_theme.size().width();
    int h = m_theme.size().height();
    int coord =  m_coordinates ? CoordinateSize : 0;
    QPoint pos1(coord + (x1 * w) + (w/2), (y1 * h) + (h/2));
    QPoint pos2(coord + (x2 * w) + (w/2), (y2 * h) + (h/2));

    QColor color = Qt::red;
    if(colorChar == 'Y') {
        color = Qt::yellow;
    } else if(colorChar == 'G') {
        color = Qt::green;
    } else if(colorChar == 'B') {
        color = Qt::blue;
    }

    // Now to Draw Arrow Head
    qreal headWidth = m_theme.size().width() / 4;
    qreal headLength = headWidth;
    qreal headIndent = headWidth / 4;
    qreal netIndent = headLength- headIndent;

    qreal halfHead = headWidth / 2;
    int px1 = pos1.x();
    int px2 = pos2.x();
    int py1 = pos1.y();
    int py2 = pos2.y();
    int dX = px2 - px1;
    int dY = py2 - py1;

    qreal  arrowLength = qSqrt( dX * dX + dY * dY );

    QPointF arrowPts[7];

    // we will shorten the line somewhat to avoid arrows all colliding in the center of the square
    int adjust = ( w + h ) / 8;

    px1 = px1 + (( adjust * dX ) / arrowLength);
    px2 = px2 - (( adjust * dX ) / arrowLength);
    py1 = py1 + (( adjust * dY ) / arrowLength);
    py2 = py2 - (( adjust * dY ) / arrowLength);


    // calculate the points that form the arrow
    arrowPts[0].setX( px2 - ((netIndent * dX) / arrowLength));
    arrowPts[0].setY( py2 - ((netIndent * dY) / arrowLength));
    arrowPts[4].setX( px2 - ((headLength * dX) / arrowLength));
    arrowPts[4].setY( py2 - ((headLength * dY) / arrowLength));
    arrowPts[1].setX( arrowPts[4].x() - ((halfHead * (dY)) / arrowLength));
    arrowPts[1].setY( arrowPts[4].y() - ((halfHead * (-dX)) / arrowLength));
    arrowPts[3].setX( arrowPts[4].x() + ((halfHead * (dY)) / arrowLength));
    arrowPts[3].setY( arrowPts[4].y() + ((halfHead * (-dX)) / arrowLength));
    arrowPts[2].setX( px2);
    arrowPts[2].setY( py2);

    QPoint pos3(px1,py1);
    QPoint pos4(px2,py2);

    p.save();
    QPen pen(color);
    int penWidth = std::max(1,(int)(headWidth/8));
    pen.setWidth(penWidth);
    p.setPen(pen);

    p.drawLine(pos3,pos4);

    // For now only draw part of the arrowhead
    //p.drawLine(arrowPts[2],arrowPts[1]);
    //p.drawLine(arrowPts[2],arrowPts[3]);

    QBrush brush(color);
    p.setBrush(brush);

    p.drawPolygon(arrowPts,4);
    p.restore();
}

void BoardView::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
}

void BoardView::setDisabled(bool disabled)
{
    QWidget::setDisabled(disabled);
}

void BoardView::setDbIndex(int dbIndex)
{
    m_DbIndex = dbIndex;
}

int BoardView::dbIndex() const
{
    return m_DbIndex;
}
