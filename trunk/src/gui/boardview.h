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

/**
   The BoardView class represents a widget for displaying current position on the screen.
*/

class BoardView : public QWidget
{
  Q_OBJECT
public:
  /** Create board widget. */
  BoardView(QWidget* parent = 0);
  /** Destroys widget. Usually it is called automatically by Qt. */
  ~BoardView();
  /** Updates and shows current position. */
  void setBoard(const Board& value);
  /** @return displayed position. */
  Board board() const;
  /** Reads new theme from file. */
  bool setTheme(const QString& themeFile);
  /** Flips/unflips board. */
  void flip();
  /** @return @p true if board is displayed upside down. */
  bool isFlipped() const;
  /** Turns frame around each square on/off. */
  void setShowFrame(bool theValue);
  /** @return true if there is a frame around each square. */
  bool showFrame() const;

public slots:
  /** Reconfigure current theme. */
  void configure();
 
protected:
  /** Redraws single square (and piece on it). */
  void repaintSquare(Square square);
  /** Redraws whole board. */
  void repaintBoard();
  /** Resizes pieces for new board size. */
  void resizeBoard();
  /** Redraws whole board if necessary. */
  virtual void paintEvent(QPaintEvent*);
  /** Automatically resizes pieces and redisplays board. */
  virtual void resizeEvent(QResizeEvent*);
signals:
  void slotSourceSelected(Square src);
  void slotDestinationSelected(Square src, Square dest);

private:
  Board m_board;
  BoardTheme* m_theme;
  bool m_flipped;
  bool m_showFrame;
};

#endif

