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

#include <QWidget>

#include "common.h"
#include "board.h"
#include "boardtheme.h"

class BoardTheme;

/** @ingroup GUI
The BoardView class represents a widget for displaying current
position on the screen. */

class BoardView : public QWidget
{
	Q_OBJECT
public:
	enum {WheelUp = Qt::LeftButton, WheelDown = Qt::RightButton};
	enum {IgnoreSideToMove = 1, SuppressGuessMove = 2};
	/** Create board widget. */
	BoardView(QWidget* parent = 0, int flags = 0);
	/** Destroy widget. */
	~BoardView();
	/** Set flags for board. Flags include:
	* @p IgnoreSideToMove - allow dragging all pieces (useful for setting up a position)
	*/
	void setFlags(int flags);
	/** Update and shows current position. */
	void setBoard(const Board& value);
	/** @return displayed position. */
	Board board() const;
	/** @return current theme */
	const BoardTheme& theme() const;
	/** Reads new theme from file. If two files are given, pieces are read from the first one,
	board from the second. */
	bool setTheme(const QString& themeFile, const QString& boardFile = QString::null);
	/** Flips/unflips board. */
	void flip();
	/** @return @p true if board is displayed upside down. */
	bool isFlipped() const;
public slots:
	/** Reconfigure current theme. */
	void configure();

signals:
	/** User clicked source and destination squares */
	void moveMade(Square from, Square to);
	/** User clicked square */
	void clicked(Square square, int button);
	/** User moved mouse wheel. */
	void wheelScrolled(int dir);
protected:
	/** Redraws whole board if necessary. */
	virtual void paintEvent(QPaintEvent*);
	/** Automatically resizes pieces and redisplays board. */
	virtual void resizeEvent(QResizeEvent*);
	/** Handle mouse events */
	virtual void mousePressEvent(QMouseEvent* e);
	/** Handle mouse events */
	virtual void mouseMoveEvent(QMouseEvent* e);
	/** Handle mouse events */
	virtual void mouseReleaseEvent(QMouseEvent* e);
	/** Handle mouse wheel events */
	virtual void wheelEvent(QWheelEvent* e);
private:
	/** Resizes pieces for new board size. */
	void resizeBoard();
	/** Calculate size and position of square */
	QRect squareRect(Square s);
	/** @return square at given position */
	Square squareAt(const QPoint& p) const;
	/** Selects given square. Previously selected square is unselected automatically. */
	void selectSquare(Square s);
	/** Unselects given square. */
	void unselectSquare();
	/** Check if piece at square @p square can be dragged */
	bool canDrag(Square s) const;
	/** Highlights the from and to squares of a guessed move. */
	void showGuess(Square s);
	/** Recalculate guess when board is changed */
	void updateGuess(Square s);
	/** Remove the guessed move highlight from the board. */
	void removeGuess();

	Board m_board;
	BoardTheme m_theme;
	bool m_flipped;
	bool m_showFrame;
	bool m_guessMove;
	int m_selectedSquare;
	int m_hoverSquare;
	int m_hifrom;
	int m_hito;
	int m_flags;
	Piece m_dragged;
	QPoint m_dragStart;
	QPoint m_dragPoint;
	bool m_clickUsed;
};

#endif

