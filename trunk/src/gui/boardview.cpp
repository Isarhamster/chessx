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

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

using namespace Qt;

const int CoordinateSize = 16;
const int MoveIndicatorSize = 12;

BoardView::BoardView(QWidget* parent, int flags) : QWidget(parent),
    m_flipped(false), m_showFrame(false), m_showCurrentMove(2),
    m_guessMove(false), m_showThreat(false), m_showTargets(false), m_brushMode(false), m_selectedSquare(InvalidSquare),
    m_hoverSquare(InvalidSquare),
    m_hiFrom(InvalidSquare), m_hiTo(InvalidSquare),
    m_threatFrom(InvalidSquare), m_threatTo(InvalidSquare),
    m_currentFrom(InvalidSquare), m_currentTo(InvalidSquare),
    m_storedFrom(InvalidSquare), m_storedTo(InvalidSquare),
    m_dragStartSquare(InvalidSquare),
    m_atLineEnd(true),
    m_flags(flags),
    m_coordinates(false), m_dragged(Empty), m_clickUsed(false), m_wheelCurrentDelta(0),
    m_minDeltaWheel(0), m_moveListCurrent(0), m_showMoveIndicator(true), 
    m_DbIndex(0),
    m_showAttacks(NoColor),
    m_showUnderProtection(NoColor),
    lastMoveEvent(0)
{
    QSizePolicy policy = sizePolicy();
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
    setMouseTracking(true);
    installEventFilter(this);
    setFocusPolicy( Qt::StrongFocus );

    connect(&m_threatGuess, SIGNAL(guessFoundForBoard(Guess::Result, Board)),
            this, SLOT(showThreat(Guess::Result,Board)), Qt::QueuedConnection);
}

BoardView::~BoardView()
{
    removeEventFilter(this);
    m_threatGuess.cancel();
}

int BoardView::heightForWidth(int width) const
{
    int coord = m_coordinates ? CoordinateSize : 0;
    int moveIndicator = (m_showMoveIndicatorMode==Always) ? MoveIndicatorSize:0;
    int xsize = (width - 1 - coord - moveIndicator) / 8;

    int square = moveIndicatorWidth(width, xsize);
    return (width - square);
}

bool BoardView::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::Leave || ev->type() == QEvent::WindowDeactivate)
    {
        removeGuess();
    }
    if(ev->type() == QEvent::KeyPress)
    {
        checkCursor(QApplication::queryKeyboardModifiers());
    }
    return QWidget::eventFilter(obj, ev);
}

void BoardView::setFlags(int flags)
{
    m_flags = flags;
}

int BoardView::flags() const
{
    return m_flags;
}

void BoardView::setBoard(const Board& value, Square from, Square to, bool atLineEnd)
{
    m_clickUsed = true;
    m_board = value;
    m_currentFrom = from;
    m_currentTo = to;
    m_atLineEnd = atLineEnd;
    m_hiFrom = m_hiTo = InvalidSquare;
    m_alertSquare = value.kingInCheck();
    m_targets.clear();
    if(underMouse())
    {
        updateGuess(m_hoverSquare);
    }
    updateThreat();
    update();
}

void BoardView::setStoredMove(Square from, Square to)
{
    m_storedFrom = from;
    m_storedTo = to;
    update();
}

void BoardView::getStoredMove(Square& from, Square& to)
{
    from = m_storedFrom;
    to = m_storedTo;
    m_storedFrom = InvalidSquare;
    m_storedTo = InvalidSquare;
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

QSize BoardView::themeSize() const
{
    return m_theme.size();
}

void BoardView::showMoveIndicator(bool visible)
{
    m_showMoveIndicator = visible;
}

bool BoardView::showMoveIndicator() const
{
    return m_showMoveIndicator;
}

void BoardView::showCoordinates(bool visible)
{
    m_coordinates = visible;
}

bool BoardView::showCoordinates() const
{
    return m_coordinates;
}

void BoardView::drawSquares(QPaintEvent* event)
{
    QPainter p(this);
    for (Square square=a1; square<NumSquares; ++square)
    {
        QRect rect = squareRect(square);
        if(!event->region().intersects(rect))
        {
            continue;
        }
        int coord =  m_coordinates ? CoordinateSize : 0;
        int x = isFlipped() ? 7 - square % 8 : square % 8;
        int y = isFlipped() ? square / 8 : 7 - square / 8;
        QPoint pos(coord + x * m_theme.size().width(), y * m_theme.size().height());
        p.drawPixmap(pos, m_theme.square((x + y) % 2));
    }
}

QRect BoardView::totalRect() const
{
    QRect rect1 = squareRect(isFlipped() ? h1 : a8);
    QRect rect2;
    if (m_coordinates)
    {
        rect2 = coordinateRectHorizontal(0);
    }
    else
    {
        rect2 = squareRect(isFlipped() ? a8 : h1);
    }
    QRect totalRect = QRect(0,rect1.top(), width(), rect2.bottom()+2);
    return totalRect;
}

void BoardView::drawCoordinates(QPaintEvent* event)
{
    if(m_coordinates)
    {
        QPainter p(this);
        p.save();
        p.setPen(m_theme.color(BoardTheme::Frame));
        for(int i = 0; i<8; ++i)
        {
            QRect rect = coordinateRectVertical(i);
            if(!event->region().intersects(rect))
            {
                continue;
            }
            p.drawText(rect, Qt::AlignCenter, QString("%1").arg(i + 1));
        }
        for(int i = 0; i<8; ++i)
        {
            QRect rect = coordinateRectHorizontal(i);
            if(!event->region().intersects(rect))
            {
                continue;
            }
            p.drawText(rect, Qt::AlignCenter, QString("%1").arg(QChar('a' + i)));
        }
        p.restore();
    }
}

void BoardView::drawDraggedPieces(QPaintEvent* /*event*/)
{
    // Draw dragged piece
    if(m_dragged != Empty)
    {
        QPainter p(this);
        p.drawPixmap(m_dragPoint, m_theme.piece(m_dragged));
    }
}

void BoardView::drawMoveIndicator(QPaintEvent* event)
{
    if(m_showMoveIndicator && m_showMoveIndicatorMode!=Never)
    {
        QPainter p(this);
        // Draw side to move indicator
        int coord =  m_coordinates ? CoordinateSize : 0;
        bool white = m_board.toMove() == White;
        int square = moveIndicatorWidth(width(), m_theme.size().width());
        if(square)
        {
            int posy = (white == m_flipped) ? 1 : 8 * m_theme.size().width() - square;
            QRect rect(QPoint(8 * m_theme.size().width() + 2 + coord, posy),
                       QSize(square, square));
            if(!event->region().intersects(rect))
            {
                return;
            }
            QColor color = white ? Qt::white : Qt::black;
            QColor border = white ? Qt::black : Qt::gray;
            p.setPen(border);
            p.setBrush(QColor(color));
            p.drawRect(rect);
        }
    }
}

void BoardView::drawHiliteSquare(QPoint pos, BoardTheme::ColorRole role)
{
    QPainter p(this);

    QPen pen;
    pen.setColor(m_theme.color(role));
    pen.setWidth(2);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.drawRect(pos.x() + 1 + m_showFrame, pos.y() + 1 + m_showFrame,
               m_theme.size().width() - 2 - m_showFrame, m_theme.size().height() - 2 - m_showFrame);
}

void BoardView::drawHiliting(QPaintEvent* event)
{
    for (Square square=a1; square<NumSquares; ++square)
    {
        QRect rect = squareRect(square);
        if(!event->region().intersects(rect))
        {
            continue;
        }

        QPoint pos = posFromSquare(square);

        if(square == m_storedFrom || square == m_storedTo)
        {
            drawHiliteSquare(pos, BoardTheme::StoredMove);
        }

        if(m_showCurrentMove==1)
        {
            if(square == m_currentFrom || square == m_currentTo)
            {
                drawHiliteSquare(pos, BoardTheme::CurrentMove);
            }
        }

        if(square == m_selectedSquare || square == m_hiFrom || square == m_hiTo)
        {
            drawHiliteSquare(pos, BoardTheme::Highlight);
        }
    }

    if ((m_showCurrentMove==2) && m_currentFrom != InvalidSquare && m_currentTo != InvalidSquare)
    {
        QRect rect1 = squareRect(m_currentFrom);
        QRect rect2 = squareRect(m_currentTo);
        QRect u = rect1.united(rect2);
        if(event->region().intersects(u))
        {
            drawArrow(m_currentFrom, m_currentTo, m_theme.color(BoardTheme::CurrentMove));
        }
    }

    if (m_threatFrom != InvalidSquare && m_threatTo != InvalidSquare)
    {
        QRect rect1 = squareRect(m_threatFrom);
        QRect rect2 = squareRect(m_threatTo);
        QRect u = rect1.united(rect2);
        if(event->region().intersects(u))
        {
            drawArrow(m_threatFrom, m_threatTo, m_theme.color(BoardTheme::Threat));
        }
    }
}

void BoardView::drawTargets(QPaintEvent* event)
{
    for (Square square=a1; square<NumSquares; ++square)
    {
        if (m_targets.contains(square))
        {
            drawColorRect(event, square, m_theme.color(BoardTheme::Target));
        }
    }
}

void BoardView::drawCheck(QPaintEvent* event)
{
    if (isEnabled())
    {
        QPainter p(this);
        drawColorRect(event, m_alertSquare, m_theme.color(BoardTheme::Check));
    }
}

void BoardView::drawAttacks(QPaintEvent* event)
{
    if (isEnabled() && Guess::guessAllowed())
    {
        if (m_showAttacks != NoColor)
        {
            QPainter p(this);
            for (Square square=a1; square<NumSquares; ++square)
            {
                if (m_board.isAttackedBy(m_showAttacks, square))
                {
                    drawColorRect(event, square, m_theme.color(BoardTheme::Wall), true);
                }
            }
        }
    }
}

void BoardView::drawUnderProtection(QPaintEvent* event)
{
    if (isEnabled() && Guess::guessAllowed())
    {
        if (m_showUnderProtection != NoColor)
        {
            QPainter p(this);
            for (Square square=a1; square<NumSquares; ++square)
            {
                if (m_board.colorAt(square) == m_showUnderProtection)
                {
                    if (pieceType(m_board.pieceAt(square)) != King)
                    {
                        int numDefenders = m_board.DefendersOfSquare(square);
                        if ((m_showUnderProtection == White && numDefenders < 0) ||
                            (m_showUnderProtection == Black && numDefenders > 0))
                        {
                            drawColorRect(event, square, m_theme.color(BoardTheme::UnderProtected), true);
                        }
                    }
                }
            }
        }
    }
}



void BoardView::drawPieces(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    for (Square square=a1; square<NumSquares; ++square)
    {
        QRect rect = squareRect(square);
        if(!event->region().intersects(rect))
        {
            continue;
        }

        QPoint pos = posFromSquare(square);

        if(m_showFrame)
        {
            p.setPen(m_theme.color(BoardTheme::Frame));
            p.drawRect(QRect(pos, m_theme.size()));
        }

        if(m_dragged != Empty)
        {
            // Do not paint piece that is dragged
            if (m_dragStartSquare == square)
            {
                continue;
            }
        }

        p.drawPixmap(pos, m_theme.piece(m_board.pieceAt(square)));
    }

    // Always draw outer rectangle
    QRect rect1 = squareRect(a8);
    QRect rect2 = squareRect(h1);
    p.setPen(m_theme.color(BoardTheme::Frame));
    p.drawRect(!isFlipped() ? QRect(rect1.topLeft(),rect2.bottomRight()) :
                              QRect(rect2.topLeft(),rect1.bottomRight()));
}

void BoardView::paintEvent(QPaintEvent* event)
{
    drawSquares(event);
    drawCoordinates(event);
    drawAttacks(event);
    drawSquareAnnotations(event);
    drawTargets(event);
    drawCheck(event);
    drawUnderProtection(event);
    drawPieces(event);
    drawHiliting(event);
    drawMoveIndicator(event);
    drawArrowAnnotations(event);
    drawDraggedPieces(event);
}

void BoardView::resizeBoard(QSize sz)
{
    // subtract move indicator from width
    int coord = m_coordinates ? CoordinateSize : 0;
    int moveIndicator = (m_showMoveIndicatorMode==Always) ? MoveIndicatorSize:0;
    int xsize = (sz.width() - 1 - coord - moveIndicator) / 8;
    int ysize = (sz.height() - 1 - coord) / 8;
    int size = std::min(xsize, ysize);
    m_theme.setSize(QSize(size, size));

    // calculate the translation vector to center the widget inside its parent
    int square = moveIndicatorWidth(width(), m_theme.size().width());

    QSize widgetSize(size * 8 + 1 + coord + square, size * 8 + 1 + coord);
    QPoint widgetCenter(widgetSize.width() / 2, widgetSize.height() / 2);
}

void BoardView::resizeEvent(QResizeEvent* e)
{
    resizeBoard(e->size());
    QWidget::resizeEvent(e);
}

QSize BoardView::sizeHint() const
{
    int w = geometry().width();
    int h = heightForWidth(w);
    return QSize(w,h);
}

int BoardView::moveIndicatorWidth(int width, int themeWidth) const
{
    if(m_showMoveIndicator && m_showMoveIndicatorMode!=Never)
    {
        int coord = m_coordinates ? CoordinateSize : 0;
        int square = width - coord - 8 * themeWidth - 4;
        int maxsquare = themeWidth / 2;
        if (square > maxsquare)
        {
            square = maxsquare;
        }
        if (square <= 8)
        {
            square = 0;
        }
        return square;
    }
    return 0;
}


Square BoardView::squareAt(const QPoint& p) const
{
    int x = p.x();
    int y = p.y();
    int width = m_theme.size().width();
    int height = m_theme.size().height();
    x -= m_coordinates ? CoordinateSize : 0;
    if(x <= 0 || y <= 0 || x >= width * 8 || y >= height * 8)
    {
        return InvalidSquare;
    }
    x /= width;
    y /= height;
    return Square(isFlipped() ? (8 * y + 7 - x) : (8 * (7 - y) + x));
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    m_dragStart = event->pos();
    setStoredMove(InvalidSquare,InvalidSquare);
}

bool BoardView::showGuess(Square s)
{
    // Don't want to constantly recalculate guess, so remember which square
    // the mouse is hovering over, and only show new guess when it changes
    if((m_guessMove || m_showTargets) && s != InvalidSquare && s != m_hoverSquare && !(m_flags & SuppressGuessMove))
    {
        removeGuess();
        m_hoverSquare = s;

        if (s != InvalidSquare)
        {
            Guess::Result sm = Guess::guessMove(qPrintable(m_board.toFen()), m_board.chess960(), m_board.castlingRooks(),
                                                static_cast<Guess::squareT>(s), m_moveList);
            if(!sm.error)
            {
                if (m_guessMove)
                {
                    m_hiFrom = Square(sm.from);
                    m_hiTo = Square(sm.to);
                    update(squareRect(m_hiFrom));
                    update(squareRect(m_hiTo));
                }

                if (m_showTargets)
                {
                    foreach(Guess::simpleMoveT sm, m_moveList)
                    {
                        if (s == Square(sm.from))
                        {
                            Square target = Square(sm.visualTo());
                            m_targets.append(target);
                            update(squareRect(target));
                        }
                    }
                }
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

void BoardView::updateThreat()
{
    removeThreat();
    if(m_showThreat && !(m_flags & SuppressGuessMove) && Guess::guessAllowed())
    {
        if (board() != Board::standardStartBoard)
        {
            m_threatGuess.guessMove(board());
        }
    }
}

void BoardView::showThreat(Guess::Result sm, Board b)
{
    if (board() == b)
    {
        if(!sm.error)
        {
            m_threatFrom = Square(sm.from);
            m_threatTo = Square(sm.to);
            update();
        }
    }
    else
    {
        removeThreat();
        m_threatGuess.guessMove(board());
    }
}

void BoardView::removeThreat()
{
    m_threatFrom = InvalidSquare;
    m_threatTo = InvalidSquare;
}

void BoardView::removeGuess()
{
    if(m_hiFrom != InvalidSquare)
    {
        update(squareRect(m_hiFrom));
        update(squareRect(m_hiTo));
        m_hiFrom = m_hiTo = InvalidSquare;
    }
    m_hoverSquare = InvalidSquare;
    m_moveListCurrent = 0;
    m_moveList.clear();
    foreach(Square s, m_targets)
    {
        update(squareRect(s));
    }
    m_targets.clear();
}

void BoardView::nextGuess(Square s)
{
    if(!showGuess(s))
    {
        if(m_moveList.size() && (int) m_moveListCurrent < m_moveList.size())
        {
            Guess::simpleMoveT * sold = m_moveList.Get(m_moveListCurrent);
            update(squareRect(Square(sold->from)));
            update(squareRect(Square(sold->visualTo())));

            if((int)m_moveListCurrent < m_moveList.size() - 1)
            {
                ++m_moveListCurrent;
            }
            else
            {
                m_moveListCurrent = 0;
            }

            Guess::simpleMoveT * sm = m_moveList.Get(m_moveListCurrent);
            m_hiFrom = Square(sm->from);
            m_hiTo = Square(sm->visualTo());
            update(squareRect(m_hiFrom));
            update(squareRect(m_hiTo));
        }
    }
}

BoardView::BoardViewAction BoardView::moveActionFromModifier(Qt::KeyboardModifiers modifiers) const
{
    switch (modifiers & 0x7e000000)
    {
    case ShiftModifier:
        return ActionPen;
    case ControlModifier:
        return m_atLineEnd ? ActionStandard : ActionReplace;
    case AltModifier:
        return m_atLineEnd ? ActionStandard : ActionAdd;
    case (unsigned int)ControlModifier | (unsigned int)AltModifier:
        return m_atLineEnd ? ActionStandard : ActionInsert;
    case MetaModifier:
        return ActionQuery;
    case (unsigned int)ControlModifier | (unsigned int)ShiftModifier:
        return ActionAskEngine;
    case (unsigned int)ShiftModifier | (unsigned int)AltModifier:
        return ActionEvalMove;
    default:
        return ActionStandard;
    }
}

void BoardView::checkCursor(Qt::KeyboardModifiers modifiers)
{
    const char* file = 0;
    QString text;

    switch (moveActionFromModifier(modifiers))
    {
    case ActionStandard:
        break;
    case ActionQuery:
        file = ":/images/query_move.png";
        text = tr("Query for piece in case of promotion");
        break;
    case ActionReplace:
        file = ":/images/replace_move.png";
        text = tr("Replace remainder of game with new move");
        break;
    case ActionInsert:
        file = ":/images/insert_move.png";
        text = tr("Insert new move and keep as much as possible of remaining moves");
        break;
    case ActionAdd:
        file = ":/images/plus.png";
        text = tr("Force adding a variation");
        break;
    case ActionPen:
        file = ":/images/pen.png";
        text = tr("Draw a square or arrow annotation");
        break;
    case ActionAskEngine:
        file = ":/images/engine.png";
        text = tr("Query the engine as if piece was located at target");
        break;
    case ActionEvalMove:
        file = ":/images/engine.png";
        text = tr("Query the engine for the best reply");
        break;
    }

    if(file)
    {
        setCursor(QCursor(QPixmap(file)));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

    emit actionHint(text);

    if ((m_dragged != Empty) && (lastMoveEvent))
    {
        lastMoveEvent->setModifiers(modifiers);
        handleMouseMoveEvent(lastMoveEvent);
    }
}

void BoardView::keyPressEvent(QKeyEvent *event)
{
    checkCursor(QApplication::queryKeyboardModifiers());
    QWidget::keyPressEvent(event);
}

void BoardView::keyReleaseEvent(QKeyEvent *event)
{
    checkCursor(QApplication::queryKeyboardModifiers());
    QWidget::keyReleaseEvent(event);
}

void BoardView::enterEvent(QEvent *event)
{
    setFocus();
    raise();
    QWidget::enterEvent(event);
}

void BoardView::focusInEvent(QFocusEvent *event)
{
    checkCursor(QApplication::queryKeyboardModifiers());
    QWidget::focusInEvent(event);
}

void BoardView::handleMouseMoveEvent(QMouseEvent *event)
{
    setFocus();

    Qt::KeyboardModifiers mdf = event->modifiers();

    if(!(event->buttons() & Qt::LeftButton))
    {
        if(!(mdf & Qt::ShiftModifier))
        {
            showGuess(squareAt(event->pos()));
        }
        else
        {
            removeGuess();
        }
        return;
    }

    if(m_dragged != Empty)
    {
        QRect old = QRect(m_dragPoint, m_theme.size());
        m_dragPoint = event->pos() - m_theme.pieceCenter();
        update(old);
        update(QRect(m_dragPoint, m_theme.size()));
        if (moveActionFromModifier(mdf) == ActionAskEngine)
        {
            emit evalRequest(m_dragStartSquare, squareAt(event->pos()));
        }
        else if (moveActionFromModifier(mdf) == ActionEvalMove)
        {
            emit evalMove(m_dragStartSquare, squareAt(event->pos()));
        }
        return;
    }

    if(mdf & Qt::ShiftModifier && (moveActionFromModifier(mdf) != ActionAskEngine) && (moveActionFromModifier(mdf) != ActionEvalMove))
    {
        return;
    }

    if((event->pos() - m_dragStart).manhattanLength()
            < QApplication::startDragDistance())
    {
        // Click and move - start dragging
        return;
    }

    Square s = squareAt(m_dragStart);
    if(!canDrag(s, mdf))
    {
        return;
    }

    startToDrag(event, s);
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
    if (!lastMoveEvent)
    {
        lastMoveEvent = new QMouseEvent(*event);
    }
    else
    {
        *lastMoveEvent = *event;
    }

    handleMouseMoveEvent(event);
    QWidget::mouseMoveEvent(event);
}

void BoardView::startToDrag(QMouseEvent *event, Square s)
{
    removeGuess();
    if (!m_brushMode) m_dragged = m_board.pieceAt(s);
    m_dragPoint = event->pos() - m_theme.pieceCenter();
    m_dragStartSquare = s;
    update(squareRect(s));
    update(QRect(m_dragPoint, m_theme.size()));
    unselectSquare();
}

void BoardView::setShowUnderProtection(const Color &showUnderProtection)
{
    m_showUnderProtection = showUnderProtection;
    update();
}

void BoardView::setShowAttacks(const Color &showAttacks)
{
    m_showAttacks = showAttacks;
    update();
}

bool BoardView::getBrushMode() const
{
    return m_brushMode;
}

void BoardView::setBrushMode(bool brushMode)
{
    m_brushMode = brushMode;
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
    delete lastMoveEvent;
    lastMoveEvent = 0;
    int button = event->button() + event->modifiers();
    Square s = squareAt(event->pos());
    m_clickUsed = false;
    Square from = squareAt(m_dragStart);

    if (!canDrop(from))
    {
        if(m_dragged != Empty)
        {
            Square from = squareAt(m_dragStart);
            m_dragStartSquare = InvalidSquare;
            QRect oldr = QRect(m_dragPoint, m_theme.size());
            m_dragged = Empty;
            update(squareRect(from));
            update(oldr);
            emit evalModeDone();
            return;
        }
    }

    if(!(event->button() & Qt::LeftButton))
    {

        if(s == from)
        {
            from = InvalidSquare;
        }
        if(s != InvalidSquare)
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
        if ((moveActionFromModifier(event->modifiers()) == ActionAskEngine) || (moveActionFromModifier(event->modifiers()) == ActionEvalMove))
        {
            m_dragStartSquare = InvalidSquare;
            QRect oldr = QRect(m_dragPoint, m_theme.size());
            m_dragged = Empty;
            update(squareRect(from));
            update(oldr);
            emit evalModeDone();
            return;
        }
        if(event->modifiers() & Qt::ShiftModifier)
        {
            if(s != InvalidSquare)
            {
                Square from = squareAt(m_dragStart);
                emit clicked(s, button, mapToGlobal(event->pos()), from);
            }
            m_dragged = Empty;
            return;
        }
    }

    if(m_brushMode)
    {
        if(s != InvalidSquare)
        {
            Square from = squareAt(m_dragStart);
            emit clicked(s, button, mapToGlobal(event->pos()), from);
            m_dragged = Empty;
        }
    }
    else if(m_dragged != Empty)
    {
        Square from = squareAt(m_dragStart);
        m_dragStartSquare = InvalidSquare;
        QRect oldr = QRect(m_dragPoint, m_theme.size());
        m_dragged = Empty;
        update(squareRect(from));
        update(oldr);
        if(s != InvalidSquare)
        {
            if((m_flags & AllowCopyPiece) && (event->modifiers() & Qt::AltModifier))
            {
                if(m_board.pieceAt(from) != Empty)
                {
                    emit copyPiece(from, s);
                }
            }
            else
            {
                updateGuess(s);
                emit moveMade(from, s, button);
            }
        }
        else
        {
            emit invalidMove(from);
        }
    }
    else if(m_selectedSquare != InvalidSquare)
    {
        Square from = m_selectedSquare;
        unselectSquare();
        if(s != InvalidSquare)
        {
            emit moveMade(from, s, button);
        }
    }
    else if(m_hiFrom != InvalidSquare)
    {
        if(s == m_hiFrom || s == m_hiTo)
        {
            emit moveMade(m_hiFrom, m_hiTo, button);
        }
        m_hoverSquare = InvalidSquare;
        // Only update guess if "emit moveMade()" did not pop up a window (eg. promotion)
        if(m_hiFrom != InvalidSquare)
        {
            updateGuess(s);
        }
    }
    else
    {
        if(s != InvalidSquare)
        {
            emit clicked(s, button, mapToGlobal(event->pos()), InvalidSquare);
            if(!m_clickUsed && m_board.isMovable(s))
            {
                selectSquare(s);
            }
        }
    }
    m_dragged = Empty;
    QWidget::mouseReleaseEvent(event);
}

void BoardView::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if(abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        int change = m_wheelCurrentDelta < 0 ? WheelDown : WheelUp;
        emit wheelScrolled(change + e->modifiers());
        m_wheelCurrentDelta = 0;
    }
    QWidget::wheelEvent(e);
}

void BoardView::setFlipped(bool flipped)
{
    bool wasFlipped = m_flipped;
    m_flipped = flipped;
    update();
    emit signalFlipped(wasFlipped, m_flipped);
}

void BoardView::flip()
{
    setFlipped(!m_flipped);
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
    m_showCurrentMove = AppSettings->getValue("showCurrentMove").toInt();
    m_guessMove = AppSettings->getValue("guessMove").toBool();
    m_showTargets = AppSettings->getValue("showTargets").toBool();
    m_showThreat = AppSettings->getValue("showThreat").toBool();
    m_minDeltaWheel = AppSettings->getValue("minWheelCount").toInt();
    m_showMoveIndicatorMode = AppSettings->getValue("showMoveIndicator").toInt();
    AppSettings->endGroup();
    m_theme.configure();
    m_theme.setEnabled(isEnabled());
    removeGuess();
    unselectSquare();
    if(size().height() >= minimumSize().height())
    {
        resizeBoard(size());
    }
    update();
}

void BoardView::selectSquare(Square s)
{
    // You can't select a square when guess move is enabled
    if(m_guessMove && !(m_flags & SuppressGuessMove))
    {
        return;
    }
    if(m_selectedSquare == s)
    {
        return;
    }
    unselectSquare();
    m_selectedSquare = s;
    update(squareRect(s));
}

void BoardView::unselectSquare()
{
    Square prev = m_selectedSquare;
    m_selectedSquare = InvalidSquare;
    if(prev != InvalidSquare)
    {
        update(squareRect(prev));
    }
}

QPoint BoardView::posFromSquare(int square) const
{
    int coord =  m_coordinates ? CoordinateSize : 0;
    int x = isFlipped() ? 7 - square % 8 : square % 8;
    int y = isFlipped() ? square / 8 : 7 - square / 8;
    QPoint pos(coord + x * m_theme.size().width(), y * m_theme.size().height());
    return pos;
}

QRect BoardView::squareRect(Square square) const
{
    return QRect(posFromSquare(square), m_theme.size());
}

QRect BoardView::coordinateRectVertical(int n) const
{
    Q_ASSERT(m_coordinates);
    int x = isFlipped() ? n % 8 : 7 - n % 8;
    return QRect(QPoint(0, x * m_theme.size().height() + (m_theme.size().height() - CoordinateSize) / 2),
                 QSize(CoordinateSize, CoordinateSize));
}

QRect BoardView::coordinateRectHorizontal(int n) const
{
    Q_ASSERT(m_coordinates);
    int y = isFlipped() ? 7 - n % 8 : n % 8;
    return QRect(QPoint(CoordinateSize + (y * (m_theme.size().width())) + (m_theme.size().width() - CoordinateSize) / 2,
                        8 * m_theme.size().height()),
                 QSize(CoordinateSize, CoordinateSize));
}

bool BoardView::canDrop(Square s) const
{
    if(m_dragged == Empty)  // not dragging
    {
        return false;
    }
    if(s == InvalidSquare)
    {
        return false;
    }
    if(!(m_flags & IgnoreSideToMove))
    {
        return m_board.isMovable(s);
    }
    return true;
}

bool BoardView::canDrag(Square s, Qt::KeyboardModifiers mdf) const
{
    if(m_dragged != Empty)  // already dragging
    {
        return false;
    }
    if(s == InvalidSquare)
    {
        return false;
    }
    else if( (m_flags & IgnoreSideToMove) ||
             (moveActionFromModifier(mdf) == ActionAskEngine) ||
             (moveActionFromModifier(mdf) == ActionEvalMove))
    {
        return m_board.pieceAt(s) != Empty;
    }
    else
    {
        return m_board.isMovable(s);
    }
}

void BoardView::dragEnterEvent(QDragEnterEvent *event)
{
    const BoardViewMimeData *mimeData = qobject_cast<const BoardViewMimeData *>(event->mimeData());
    if(mimeData)
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
    if(mimeData)
    {
        Square s = squareAt(event->pos());
        if (s != InvalidSquare)
        {
            emit pieceDropped(s, mimeData->m_piece);
        }
        event->acceptProposedAction();
    }
}

void BoardView::drawSquareAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.squareAnnotation();

    if(!annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for(QStringList::ConstIterator it = list.constBegin(); it != list.constEnd(); it++)
        {
            if(*it != "")
            {
                drawSquareAnnotation(event, *it);
            }
        }
    }
}

void BoardView::drawArrowAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.arrowAnnotation();

    if(!annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for(QStringList::ConstIterator it = list.constBegin(); it != list.constEnd(); it++)
        {
            if(*it != "")
            {
                drawArrowAnnotation(event, *it);
            }
        }
    }
}

void BoardView::drawColorRect(QPaintEvent* event, Square square, QColor color, bool plain)
{
    QPainter p(this);

    QRect rect = squareRect(square);
    if(!event->region().intersects(rect))
    {
        return;
    }

    int x = isFlipped() ? 7 - square % 8 : square % 8;
    int y = isFlipped() ? square / 8 : 7 - square / 8;
    QPoint pos(x * m_theme.size().width(), y * m_theme.size().height());

    p.save();
    QPen pen(color);
    QBrush brush(color);
    p.setPen(pen);
    p.setBrush(brush);
    p.setOpacity(plain ? 0.7 : 0.35);

    int coord =  m_coordinates ? CoordinateSize : 0;

    if (plain)
    {
        p.fillRect(coord + pos.x(), pos.y(), m_theme.size().width(), m_theme.size().height(), color);
    }
    else
    {
        QRadialGradient radialGrad(QPointF(coord + pos.x()+m_theme.size().width()/2, pos.y()+m_theme.size().height()/2), m_theme.size().width());
        radialGrad.setColorAt(0, color.lighter());
        radialGrad.setColorAt(0.5, color);
        radialGrad.setColorAt(1, color);

        p.fillRect(coord + pos.x(), pos.y(), m_theme.size().width(), m_theme.size().height(), radialGrad);
    }

    p.restore();
}

void BoardView::drawSquareAnnotation(QPaintEvent* event, QString annotation)
{
    QString trimmed = annotation.simplified();
    QChar colorChar = trimmed[0];
    QChar fileChar = trimmed[1];
    QChar rankChar = trimmed[2];
    QString files = "abcdefgh";
    QString ranks = "12345678";
    unsigned char file = files.indexOf(fileChar);
    int rank = ranks.indexOf(rankChar);

    QColor color = Qt::red;
    if(colorChar == 'Y')
    {
        color = Qt::yellow;
    }
    else if(colorChar == 'G')
    {
        color = Qt::green;
    }
    else if(colorChar == 'B')
    {
        color = Qt::blue;
    }

    Square square = SquareFromRankAndFile(rank, file);
    drawColorRect(event, square, color);
}

void BoardView::drawArrow(int square1, int square2, QColor color)
{
    QPainter p(this);

    int x1 = isFlipped() ? 7 - square1 % 8 : square1 % 8;
    int y1 = isFlipped() ? square1 / 8 : 7 - square1 / 8;
    int x2 = isFlipped() ? 7 - square2 % 8 : square2 % 8;
    int y2 = isFlipped() ? square2 / 8 : 7 - square2 / 8;
    int w = m_theme.size().width();
    int h = m_theme.size().height();
    int coord =  m_coordinates ? CoordinateSize : 0;
    QPoint pos1(coord + (x1 * w) + (w / 2), (y1 * h) + (h / 2));
    QPoint pos2(coord + (x2 * w) + (w / 2), (y2 * h) + (h / 2));

    // Now to Draw Arrow Head
    qreal headWidth = m_theme.size().width() / 4;
    qreal headLength = headWidth;
    qreal headIndent = headWidth / 4;
    qreal netIndent = headLength - headIndent;

    qreal halfHead = headWidth / 2;
    int px1 = pos1.x();
    int px2 = pos2.x();
    int py1 = pos1.y();
    int py2 = pos2.y();
    int dX = px2 - px1;
    int dY = py2 - py1;

    qreal  arrowLength = qSqrt(dX * dX + dY * dY);

    QPointF arrowPts[7];

    // we will shorten the line somewhat to avoid arrows all colliding in the center of the square
    int adjust = (w + h) / 8;

    px1 = px1 + ((adjust * dX) / arrowLength);
    px2 = px2 - ((adjust * dX) / arrowLength);
    py1 = py1 + ((adjust * dY) / arrowLength);
    py2 = py2 - ((adjust * dY) / arrowLength);


    // calculate the points that form the arrow
    arrowPts[0].setX(px2 - ((netIndent * dX) / arrowLength));
    arrowPts[0].setY(py2 - ((netIndent * dY) / arrowLength));
    arrowPts[4].setX(px2 - ((headLength * dX) / arrowLength));
    arrowPts[4].setY(py2 - ((headLength * dY) / arrowLength));
    arrowPts[1].setX(arrowPts[4].x() - ((halfHead * (dY)) / arrowLength));
    arrowPts[1].setY(arrowPts[4].y() - ((halfHead * (-dX)) / arrowLength));
    arrowPts[3].setX(arrowPts[4].x() + ((halfHead * (dY)) / arrowLength));
    arrowPts[3].setY(arrowPts[4].y() + ((halfHead * (-dX)) / arrowLength));
    arrowPts[2].setX(px2);
    arrowPts[2].setY(py2);

    QPoint pos3(px1, py1);
    QPoint pos4(px2- ((adjust * dX) / arrowLength), py2- ((adjust * dY) / arrowLength));

    p.save();
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    color.setAlpha(176);
    QPen pen(color);
    int penWidth = std::max(2, (int)(headWidth / 3));
    pen.setWidth(penWidth);
    p.setPen(pen);
    p.drawLine(pos3, pos4);

    QBrush brush(color);
    p.setBrush(brush);
    pen.setWidth(2);
    p.setPen(pen);
    p.drawPolygon(arrowPts, 4);

    p.restore();
}

void BoardView::drawArrowAnnotation(QPaintEvent* event, QString annotation)
{
    static const QString letters = "abcdefgh";
    static const QString numbers = "12345678";

    QString trimmed = annotation.simplified();

    QChar colorChar = trimmed[0];
    QChar fileChar1 = trimmed[1];
    QChar rankChar1 = trimmed[2];
    QChar fileChar2 = trimmed[3];
    QChar rankChar2 = trimmed[4];
    unsigned char file1 = letters.indexOf(fileChar1);
    int rank1 = numbers.indexOf(rankChar1);
    unsigned char file2 = letters.indexOf(fileChar2);
    int rank2 = numbers.indexOf(rankChar2);

    if(file1 < 0 || file2 < 0 || rank1 < 0 || rank2 < 0)
    {
        return;
    }
    Square square1 = SquareFromRankAndFile(rank1, file1);
    Square square2 = SquareFromRankAndFile(rank2, file2);

    QRect rect1 = squareRect(square1);
    QRect rect2 = squareRect(square2);
    QRect u = rect1.united(rect2);
    if(!event->region().intersects(u))
    {
        return;
    }

    QColor color = Qt::red;
    if(colorChar == 'Y')
    {
        color = Qt::yellow;
    }
    else if(colorChar == 'G')
    {
        color = Qt::green;
    }
    else if(colorChar == 'B')
    {
        color = Qt::blue;
    }

    drawArrow(square1, square2, color);
}

Piece BoardView::dragged() const
{
    return m_dragged;
}

void BoardView::setDragged(const Piece &dragged)
{
    m_dragged = dragged;
}

void BoardView::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
}

void BoardView::setDisabled(bool disabled)
{
    QWidget::setDisabled(disabled);
}

void BoardView::setDbIndex(QObject* dbIndex)
{
    m_DbIndex = dbIndex;
}

QObject* BoardView::dbIndex() const
{
    return m_DbIndex;
}

void BoardView::renderImage(QImage &image, double scaling) const
{
    BoardView boardView(0, BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    QSize s;
    if (scaling < 0)
    {
        if (AppSettings->getValue("/Board/fixedImageSize").toBool())
        {
            int n = AppSettings->getValue("/Board/copyImageSize").toInt();
            s = QSize(n, n*height()/width());
        }
        else
        {
            scaling = -scaling;
            s = size()*scaling;
        }
    }
    else
    {
        s = size()*scaling;
    }

    boardView.setMinimumSize(s);
    boardView.setEnabled(isEnabled() && AppSettings->getValue("/Board/colorCopy").toBool());
    boardView.setFlipped(isFlipped());
    boardView.showMoveIndicator(showMoveIndicator());
    boardView.showCoordinates(showCoordinates());
    boardView.resize(s);
    boardView.configure();
    boardView.setBoard(board());

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::transparent);
    boardView.setAutoFillBackground(true);
    boardView.setPalette(Pal);
    QRect sourceRect = boardView.totalRect();
    QPixmap pixmap(sourceRect.size());
    pixmap.fill();

    boardView.render(&pixmap, QPoint(), sourceRect, QWidget::DrawChildren);
    image = pixmap.toImage();
}

void BoardView::renderImageForBoard(const Board &b, QImage &image, QSize size)
{
    BoardView boardView(0, BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    boardView.setBoard(b);
    boardView.setMinimumSize(size);
    boardView.resize(size);
    boardView.setEnabled(false);
    boardView.renderImage(image, 1.0);
}
