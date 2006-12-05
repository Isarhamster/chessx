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

/**
   The BoardTheme class contains set of pixmaps and options to
   define current board theme. Themes are read from INI files pointing
   to set of external pixmaps.

   Pixmaps are automatically scaled to given size.
*/


class BoardTheme 
{
public:
  enum BoardSquare {Unscaled, Scaled, Plain};
  enum LoadTheme {LoadBoard = 1, LoadPieces = 2, LoadAll = LoadBoard | LoadPieces};
  BoardTheme();
  ~BoardTheme();
  /** Loads theme description from Eboard-compatible PNG file. */
  bool load(const QString& themeFile, LoadTheme = LoadAll);
  /** Loads theme description from Eboard-compatible PNG file. */
  bool load(const QString& pieceFile, const QString& boardFile);
  /** Sets the color of light squares. */
  void setLightColor(const QColor& value);
  /** @return color of light squares. */
  QColor lightColor() const;
  /** Sets the color of dark squares. */
  void setDarkColor(const QColor& value);
  /** @return color of dark squares. */
  QColor darkColor() const;
  /** @return pixmap for given piece scaled to current size(). */
  const QImage& piece(Piece p) const;
  /** @return unscaled pixmap for given piece. */
  const QImage& originalPiece(Piece p) const;
  /** @return pixmap for square. */
  const QImage& square(bool dark) const;
  /** @return unscaled pixmap for square. */
  const QImage& originalSquare(bool dark) const;
  /** Checkes whether themes is valid (pixmaps loaded). */
  bool isValid() const;
  /** Returns name of board theme. It is just file without a path and extension. */
  QString boardThemeName() const;
  /** Returns name of piece theme. It is just file without a path and extension. */
  QString pieceThemeName() const;
  /** Sets current size for pixmaps. Resizes all pixmaps. */
  void setSize(int value);
  /** Returns current size of pixmaps. */
  int size() const;
  /** Returns current rectangle of pixmaps. */
  QRect rect() const;
  /** Sets background type. */
  void setSquareType(BoardSquare type);
  /** @return whether board pixmaps are scaled or clipped. */
  BoardSquare squareType() const;

private:
  QImage m_originalPiece[ConstPieceTypes];
  QImage m_piece[ConstPieceTypes];
  QImage m_originalSquare[2];
  QImage m_square[2];
  int m_size;
  bool m_plainSquares;
  BoardSquare m_squareType;
  QColor m_lightColor;
  QColor m_darkColor;
  QString m_pieceFilename;
  QString m_boardFilename;
};

#endif

