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
#include "settings.h"

#include <qpainter.h>
#include <qmessagebox.h>

BoardView::BoardView(QWidget* parent) : QWidget(parent, "board"), m_flipped(false), m_showFrame(false)
{
  m_theme = new BoardTheme;
  connect(parent, SIGNAL(reconfigure()), SLOT(configure()));
}

BoardView::~BoardView()
{
  delete m_theme;
}

void BoardView::setBoard(const Board& value)
{
   m_board = value;
   update();
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
  p.drawPixmap(posx, posy, m_theme->square((x + y) % 2));
  p.drawPixmap(posx, posy, m_theme->pixmap(m_board.at(square)));
  if (m_showFrame)
  {
    p.setPen(QColor(Qt::black));
    p.drawRect(posx, posy, m_theme->size() + m_showFrame, m_theme->size() + m_showFrame);
  }
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

bool BoardView::setTheme(const QString& themeFile)
{
  bool result = m_theme->load(themeFile);
  if (!result)
  {
    QMessageBox::warning(0, tr("Error"), tr("<qt>Cannot open theme <b>%1</b></qt>")
        .arg(themeFile));
    // If there is no theme, try to load default
    if (m_theme->filename().isNull())
    {
      result = m_theme->load("default");
      if (result)
      {
        resizeBoard();
      }
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
  QString theme = AppSettings->readEntry("theme", "default");
  if (!theme.isNull())
    setTheme(theme);
  AppSettings->endGroup();
  update();
}

