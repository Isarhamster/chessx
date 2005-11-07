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

#ifndef __BOARDVIEW_H__
#define __BOARDVIEW_H__

#include <qwidget.h>

#include "common.h"
#include "board.h"

class BoardTheme;

class BoardView : public QWidget
{
public:
  enum RepaintMode {NoRepaint, RepaintChanged, ForceRepaint};
  BoardView(QWidget* parent = 0);
  ~BoardView();
  void setBoard(const Board& value, RepaintMode = RepaintChanged);
  Board board() const;
  bool setTheme(const QString& themeFile);
  void flip();
  bool isFlipped() const;
protected:
  void repaintSquare(Square square);
  void repaintBoard();
  virtual void paintEvent(QPaintEvent*);
  virtual void resizeEvent(QResizeEvent*);
signals:
  void slotSourceSelected(Square src);
  void slotDestinationSelected(Square src, Square dest);

private:
  Board m_board;
  BoardTheme* m_theme;
  bool m_flipped;
};

#endif

