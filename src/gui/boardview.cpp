/***************************************************************************
                      BoardView - view of the current board
                             -------------------
    begin                : Sun 21 Aug 2005
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

#include "boardview.h"
#include "boardtheme.h"
#include <settings.h>

#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QWheelEvent>

using namespace Qt;

BoardView::BoardView(QWidget* parent, const char* name) : QWidget(parent, name),
   m_flipped(false), m_showFrame(false), m_selectedSquare(InvalidSquare)
{
  m_theme = new BoardTheme;
}

BoardView::~BoardView()
{
  delete m_theme;
}

void BoardView::setBoard(const Board& value)
{
  Board oldboard = m_board; 
  m_board = value;
  update();
  emit changed();
}

Board BoardView::board() const
{
  return m_board;
}

void BoardView::repaintSquare(Square square)
{
  QPainter p(this);
  int x = isFlipped() ? 7 - square % 8 : square % 8;
  int y = isFlipped() ? square / 8 : 7 - square / 8;
  int posx = x * m_theme->size();
  int posy = y * m_theme->size();
  p.drawImage(QPoint(posx, posy), m_theme->square((x + y) % 2));
  p.drawImage(QPoint(posx, posy), m_theme->piece(m_board.at(square)));
  if (square == m_selectedSquare)
  {
    QPen pen;
    pen.setColor(QColor(Qt::yellow));
    pen.setWidth(2);
    p.setPen(pen);
    p.drawRect(posx + 1 + m_showFrame, posy + 1 + m_showFrame, m_theme->size() - 2 - m_showFrame,
               m_theme->size() - 2 - m_showFrame);
  }
  if (m_showFrame)
  {
    p.setPen(QColor(Qt::black));
    p.drawRect(posx, posy, m_theme->size(), m_theme->size());
  }
}

void BoardView::paintEvent(QPaintEvent*)
{
  for (Square i = 0; i < 64; i++)
    repaintSquare(i);
}

void BoardView::resizeBoard()
{
  int xsize = (width() - 1) / 8;
  int ysize = (height() - 1) / 8;
  int size = xsize < ysize ? xsize : ysize;
  m_theme->setSize(size);
}

void BoardView::resizeEvent(QResizeEvent*)
{
  resizeBoard();
}

Square BoardView::squareAt(QPoint p) const
{
  int x = isFlipped() ? 7 - p.x() / m_theme->size() : p.x() / m_theme->size();
  int y = isFlipped() ? p.y() / m_theme->size() : 7 - p.y() / m_theme->size();
  if (x >= 0 && x < 8 && y >= 0 && y <= 8)
    return 8 * y + x;
  else return InvalidSquare;
}

void BoardView::mouseReleaseEvent(QMouseEvent* e)
{
  Square s = squareAt(e->pos());
  if (s == InvalidSquare || e->button() & Qt::RightButton)
  {
    e->ignore();
    return;
  }
  emit clicked(s, e->button() + e->modifiers());
  e->accept();
  if (e->button() != Qt::LeftButton)
    return;
  if (selectedSquare() == InvalidSquare)
    selectSquare(s);
  else 
  {
    Square from = selectedSquare();
    unselectSquare();
    emit moveMade(from, s);
  }
}

void BoardView::wheelEvent(QWheelEvent* e)
{
  int change = e->delta() < 0 ? WheelDown : WheelUp;
  emit wheelScrolled(change + e->modifiers());
}

bool BoardView::setTheme(const QString& pieceFile, const QString& boardFile)
{
  bool result = m_theme->load(pieceFile, boardFile);
  if (!result)
  {
    QMessageBox::warning(0, tr("Error"), tr("<qt>Cannot open theme <b>%1</b></qt>")
        .arg(pieceFile));
    // If there is no theme, try to load default
    if (!m_theme->isValid())
    {
      result = m_theme->load("default");
      if (result)
        resizeBoard();
    }
  }
  if (result)
    update();
  return result;
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

bool BoardView::showFrame() const
{
   return m_showFrame;
}

void BoardView::setShowFrame(bool value) 
{
   if (value == m_showFrame)
     return;
   m_showFrame = value;
   resizeBoard();
}

void BoardView::configure()
{
  AppSettings->beginGroup("/Board/");
  m_showFrame = AppSettings->readBoolEntry("showFrame", true);
  m_theme->setSquareType(BoardTheme::BoardSquare(AppSettings->readNumEntry("squareType", 0)));
  m_theme->setLightColor(QColor(AppSettings->readEntry("lightColor", "#d0d0d0")));
  m_theme->setDarkColor(QColor(AppSettings->readEntry("darkColor", "#a0a0a0")));
  QString pieceTheme = AppSettings->readEntry("pieceTheme", "default");
  QString boardTheme = AppSettings->readEntry("boardTheme", "default");
  setTheme(pieceTheme, boardTheme);
  AppSettings->endGroup();
  update();
}

Square BoardView::selectedSquare() const
{
  return m_selectedSquare;
}

void BoardView::selectSquare(Square s)
{
  if (m_selectedSquare == InvalidSquare && !isPieceColor(m_board.at(s), m_board.toMove()))
    return;
  Square prev = m_selectedSquare;
  m_selectedSquare = s;
  if (prev != m_selectedSquare)
    update();
}

void BoardView::unselectSquare()
{
  Square prev = m_selectedSquare;
  m_selectedSquare = InvalidSquare;
  if (prev != m_selectedSquare)
    update();
}

