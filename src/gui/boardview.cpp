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

#include <qpainter.h>


BoardView::BoardView(QWidget* parent) : QWidget(parent), m_flipped(false)
{
  m_theme = new BoardTheme;
}

BoardView::~BoardView()
{
  delete m_theme;
}

void BoardView::setBoard(const Board& value, RepaintMode)
{
   m_board = value;
}

Board BoardView::board() const
{
  return m_board;
}

void BoardView::repaintSquare(Square square)
{
  QPainter p(this);
  int x = square % 8;
  int y = m_flipped ? square / 8 : 7 - square / 8;
  int posx = x * m_theme->size();
  int posy = y * m_theme->size();
  if (m_theme->plainSquares())
  {
     p.setPen((x + y) % 2 ? m_theme->darkColor() : m_theme->lightColor());
     p.drawRect(posx, posy, m_theme->size(), m_theme->size());
  }
  else
     p.drawPixmap(posx, posy, m_theme->square((x + y) % 2));
  p.drawPixmap(posx, posy, m_theme->pixmap(m_board.at(square)));
}

void BoardView::repaintBoard()
{
  for (Square i = 0; i < 64; i++)
    repaintSquare(i);
}

void BoardView::paintEvent(QPaintEvent*)
{
  repaintBoard();
}

void BoardView::resizeEvent(QResizeEvent*)
{
  int xsize = width() / 8;
  int ysize = height() / 8;
  int size = xsize < ysize ? xsize : ysize;
  m_theme->setSize(size);
}

bool BoardView::setTheme(const QString & themeFile)
{
  return m_theme->load(themeFile);
}

void BoardView::flip()
{
  m_flipped = !m_flipped;
  repaint();
}

bool BoardView::isFlipped() const
{
  return m_flipped;
}
