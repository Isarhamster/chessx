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

#include <qstring.h>
#include <qpixmap.h>

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
  BoardTheme();
  ~BoardTheme();
  /** Loads theme description from Eboard-compatible PNG file. */
  bool load(const QString& themeFile);
  /** Turns on/off using single-colored squares. */
  void setPlainSquares(bool value);
  /** @return whether squares are plain or pixmaps. */
	bool plainSquares() const;
  /** Sets the color of light squares. */
  void setLightColor(const QColor& value);
  /** @return color of light squares. */
  QColor lightColor() const;
  /** Sets the color of dark squares. */
  void setDarkColor(const QColor& value);
  /** @return color of dark squares. */
  QColor darkColor() const;
  /** @return pixmap for given piece scaled to current size(). */
  QPixmap pixmap(Piece p) const;
  /** @return unscaled pixmap for given piece. */
  QPixmap originalPixmap(Piece p) const;
  /** @return pixmap for square. */
  QPixmap square(bool dark) const;
  /** @return unscaled pixmap for square. */
  QPixmap originalSquare(bool dark) const;
  /** Checkes whether themes is valid (all pixmaps loaded). */
  bool isValid() const;
  /** Returns filename for pixmap. */
  QString filename() const;
  /** Returns name of theme. It is just file without a path and extension. */
  QString themeName() const;
  /** Sets current size for pixmaps. Resizes all pixmaps. */
  void setSize(int value);
  /** Returns current size of pixmaps. */
  int size() const;

private:
  static const int LightSquare = ConstPieceTypes;
  static const int DarkSquare = ConstPieceTypes+1;
  QPixmap m_originalPixmaps[ConstPieceTypes+2];
  QPixmap m_pixmaps[ConstPieceTypes+2];
  int m_size;
  bool m_plainSquares;
  QColor m_lightColor;
  QColor m_darkColor;
  QString m_filename;
};

#endif

