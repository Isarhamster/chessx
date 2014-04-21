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

#include "common.h"
#include "board.h"
#include "boardtheme.h"
#include "guess.h"

#include <QWidget>

class BoardTheme;

/** @ingroup GUI
The BoardView class represents a widget for displaying current
position on the screen. */

class BoardView : public QWidget
{
    Q_OBJECT
public:
    enum {WheelUp = Qt::LeftButton, WheelDown = Qt::RightButton};
    enum {Automatic = 0, Always = 1, Never = 2};
    enum {IgnoreSideToMove = 1, SuppressGuessMove = 2, AllowCopyPiece = 4};
    /** Create board widget. */
    BoardView(QWidget* parent = 0, int flags = 0);
    /** Destroy widget. */
    ~BoardView();
    /** Set flags for board. Flags include:
    * @p IgnoreSideToMove - allow dragging all pieces (useful for setting up a position)
    */
    void setFlags(int flags);
    /** Update and shows current position. */
    void setBoard(const Board& value, int from = InvalidSquare, int to = InvalidSquare, bool atLineEnd = true);
    /** @return displayed position. */
    Board board() const;
    /** @return current theme */
    const BoardTheme& theme() const;
    /** @return @p true if board is displayed upside down. */
    bool isFlipped() const;
    /** Make it almost square. */
    virtual int heightForWidth(int width) const;
    /** Switch to next guess */
    void nextGuess(Square s);
    /** Set Move Indicator property */
    void showMoveIndicator(bool visible);
    /** Set a reference to the database to which the current view is associated */
    void setDbIndex(int);
    /** Get a reference to the database to which the current view is associated */
    int dbIndex() const;

public slots:

    /** Flips/unflips board. */
    void setFlipped(bool flipped);
    /** Flips/unflips board. */
    void flip();
    /** Reconfigure current theme. */
    void configure();
    /** Enable / Disable Board for move entry. */
    void setEnabled(bool enabled);
    /** Disable / Enable Board for move entry. */
    void setDisabled(bool disabled);

signals:
    /** User clicked source and destination squares */
    void moveMade(Square from, Square to, int button);
    /** User dragged and dropped a piece holding Control */
    void copyPiece(Square from, Square to);
    /** User dragged and dropped a piece holding Control */
    void invalidMove(Square from);
    /** User clicked square */
    void clicked(Square square, int button, QPoint pos, Square from);
    /** User moved mouse wheel. */
    void wheelScrolled(int dir);
    /** Indicate that a piece was dropped to the board */
    void pieceDropped(Square to, Piece p);

    void moveStarted();
    void moveFinished();

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

protected: //Drag'n'Drop Support
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    /** Resizes pieces for new board size. */
    void resizeBoard(QSize size);
    /** Calculate size and position of square */
    QRect squareRect(Square s);
    /** Calculate size and position of a coordinate indicator in vertical direction */
    QRect coordinateRectVertical(Square square);
    /** Calculate size and position of a coordinate indicator in horizontal direction */
    QRect coordinateRectHorizontal(Square square);
    /** @return square at given position */
    Square squareAt(const QPoint& p) const;
    /** Selects given square. Previously selected square is unselected automatically. */
    void selectSquare(Square s);
    /** Unselects given square. */
    void unselectSquare();
    /** Check if piece at square @p square can be dragged */
    bool canDrag(Square s) const;
    /** Highlights the from and to squares of a guessed move. */
    bool showGuess(Square s);
    /** Recalculate guess when board is changed */
    void updateGuess(Square s);
    /** Remove the guessed move highlight from the board. */
    void removeGuess();
    /** Catch mouse events */
    bool eventFilter(QObject *obj, QEvent *ev);

    void drawSquares(QPaintEvent* event);
    void drawPieces(QPaintEvent* event);
    void drawMoveIndicator(QPaintEvent* event);
    void drawDraggedPieces(QPaintEvent* event);
    void drawCoordinates(QPaintEvent* event);

    void drawSquareAnnotations(QPaintEvent* event);
    void drawSquareAnnotation(QPaintEvent* event, QString annotation);
    void drawArrowAnnotations(QPaintEvent* event);
    void drawArrowAnnotation(QPaintEvent* event, QString annotation);

    Board m_board;
    BoardTheme m_theme;
    bool m_flipped;
    bool m_showFrame;
    bool m_showCurrentMove;
    bool m_guessMove;
    int m_selectedSquare;
    int m_hoverSquare;
    int m_hiFrom;
    int m_hiTo;
    int m_currentFrom;
    int m_currentTo;
    bool m_atLineEnd;
    int m_flags;
    bool m_coordinates;
    Piece m_dragged;
    QPoint m_dragStart;
    QPoint m_dragPoint;
    int m_button;
    bool m_clickUsed;
    int m_wheelCurrentDelta;
    int m_minDeltaWheel;
    Guess::MoveList m_moveList;
    unsigned int m_moveListCurrent;
    bool m_showMoveIndicator;
    int m_showMoveIndicatorMode;
    int m_DbIndex;
};

class BoardViewMimeData : public QMimeData
{
    Q_OBJECT

public:
    Piece m_piece;
};

#endif

