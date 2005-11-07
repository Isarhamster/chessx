/***************************************************************************
                          BoardTheme - graphics theme for BoardView
                             -------------------
    begin                : nie sie 21 2005
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

#include <qbitmap.h>
#include <qimage.h>

#include "boardtheme.h"

BoardTheme::BoardTheme() : m_size(0), m_plainSquares(false)
{
}

BoardTheme::~BoardTheme()
{
}

QColor BoardTheme::lightColor() const
{
  return m_lightColor;
}

void BoardTheme::setLightColor(const QColor& value)
{
  m_lightColor = value;
}

QColor BoardTheme::darkColor() const
{
  return m_darkColor;
}

void BoardTheme::setDarkColor(const QColor& value)
{
  m_darkColor = value;
}

QPixmap BoardTheme::pixmap(Piece p) const
{
  return m_pixmaps[p];
}

QPixmap BoardTheme::square(bool dark) const
{
  return m_pixmaps[ConstPieceTypes + dark];
}

QPixmap BoardTheme::originalPixmap(Piece p) const
{
  return m_originalPixmaps[p];
}

QPixmap BoardTheme::originalSquare(bool dark) const
{
  return m_originalPixmaps[ConstPieceTypes + dark];
}

QString BoardTheme::filename() const
{
  return m_filename;
}

bool BoardTheme::isValid() const
{
  return !filename().isNull();
}

bool BoardTheme::load(const QString& themeFile)
{
  QPixmap big;
  big.load(themeFile);
  int realsize = big.height() / 2;
  if (realsize != big.width() / 7)
    return false;

  /* Make theme bitmap transparent */
  QImage src = big.convertToImage();
  int w = src.width();
  int h = src.height();
  QImage maskImage(w, h, src.depth());
  QRgb opaque = Qt::color1.rgb();
  QRgb maskColor = src.pixel(0, 0);

  maskImage.fill(Qt::color0.rgb());
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      if(src.pixel(i, j) != maskColor)
        maskImage.setPixel(i, j, opaque);
  QBitmap mask;
  mask.convertFromImage(maskImage);
  big.setMask(mask);

  /* Cut big theme bitmap into separate pieces */
  for (int i = 0; i<ConstPieceTypes + 2; i++)
     m_originalPixmaps[i].resize(realsize, realsize);
  copyBlt(&m_originalPixmaps[WhiteRook], 0, 0, &big, 0, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[WhiteKnight], 0, 0, &big, 1 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[WhiteBishop], 0, 0, &big, 2 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[WhiteQueen], 0, 0, &big, 3 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[WhiteKing], 0, 0, &big, 4 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[WhitePawn], 0, 0, &big, 5 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[ConstPieceTypes], 0, 0, &big, 6 * realsize, 0, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackRook], 0, 0, &big, 0, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackKnight], 0, 0, &big, 1 * realsize, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackBishop], 0, 0, &big, 2 * realsize, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackQueen], 0, 0, &big, 3 * realsize, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackKing], 0, 0, &big, 4 * realsize, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[BlackPawn], 0, 0, &big, 5 * realsize, realsize, realsize, realsize);
  copyBlt(&m_originalPixmaps[ConstPieceTypes+1], 0, 0, &big, 6 * realsize, realsize, realsize, realsize);
  m_lightColor = QColor("#F0F0F0");
  m_darkColor = QColor("#D0D0D0");
  m_filename = themeFile;
  setSize(realsize);
  return true;
}

int BoardTheme::size() const
{
  return m_size;
}


void BoardTheme::setSize(int value)
{
  if (m_size == value)
    return;
  m_size = value;
  for (int i = 1; i<ConstPieceTypes+2; i++)
    m_pixmaps[i].convertFromImage(m_originalPixmaps[i].convertToImage().smoothScale(value, value));
}

bool BoardTheme::plainSquares() const
{
   return m_plainSquares;
}

void BoardTheme::setPlainSquares(bool value)
{
   m_plainSquares = value;
}

