/***************************************************************************
                          BoardTheme - graphics theme for BoardView
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

#ifndef __BOARDTHEME_H__
#define __BOARDTHEME_H__

#include <QString>
#include <QPixmap>

#include "common.h"

/** @ingroup GUI
   The BoardTheme class contains set of pixmaps and options to
   define current board theme. Themes are read from INI files pointing
   to set of external pixmaps.

   Pixmaps are automatically scaled to given size.
*/


class BoardTheme
{
public:
	enum ColorRole {LightSquare, DarkSquare, Highlight, Frame};
	enum LoadTheme {LoadBoard = 1, LoadPieces = 2, LoadAll = LoadBoard | LoadPieces};
	enum Effects {Plain, Outline = 1, Shadow = 2};
	BoardTheme();
	~BoardTheme();
	/** Reset piece and board selections based on config values */
	void configure();
	/** Load piece graphics named by string + effect flag */
	bool loadPieces(const QString& pieces, int effect);
	/** Load board graphics named by string */
	bool loadBoard(const QString& board);
	/** Sets one of the board colors. */
	void setColor(ColorRole role, const QColor& value);
	/** @return one of the board colors. */
	QColor color(ColorRole role) const;
	/** @return pixmap for given piece scaled to current size(). */
	const QPixmap& piece(Piece p) const;
	/** @return unscaled pixmap for given piece. */
	const QPixmap& originalPiece(Piece p) const;
	/** @return pixmap for square. */
	const QPixmap& square(bool dark) const;
	/** @return unscaled pixmap for square. */
	const QPixmap& originalSquare(bool dark) const;
	/** Checkes whether themes is valid (pixmaps loaded). */
	bool isValid() const;
	/** Returns name of board theme. It is just file without a path and extension. */
	QString boardThemeName() const;
	/** Returns name of piece theme. It is just file without a path and extension. */
	QString pieceThemeName() const;
	/** Returns theme directory */
	QString themeDirectory() const;
	/** Sets current size for pixmaps. Resizes all pixmaps. */
	void setSize(const QSize& size);
	/** Returns current size of pixmaps. */
	QSize size() const;
	/** Returns center point of piece pixmap. Useful for centering piece when dragging. */
	QPoint pieceCenter() const;
	/** Returns current rectangle of pixmaps. */
	QRect rect() const;
	/** Updates board square. */
	void updateSquares();

private:
	bool isBoardPlain() const;
	QPixmap m_originalPiece[ConstPieceTypes];
	QPixmap m_piece[ConstPieceTypes];
	QPixmap m_originalSquare[2];
	QPixmap m_square[2];
	QSize m_size;
	QColor m_colors[4];
	QString m_pieceFilename;
	QString m_boardFilename;
};

#endif

