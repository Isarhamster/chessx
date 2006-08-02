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
#include "settings.h"

BoardTheme::BoardTheme() : m_size(0), m_squareType(Scaled)
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
  if (m_squareType == Plain)
    setSquareType(m_squareType);
}

QColor BoardTheme::darkColor() const
{
  return m_darkColor;
}

void BoardTheme::setDarkColor(const QColor& value)
{
  m_darkColor = value;
  if (m_squareType == Plain)
    setSquareType(m_squareType);
}

const QPixmap& BoardTheme::pixmap(Piece p) const
{
  return m_pixmaps[p];
}

const QPixmap& BoardTheme::square(bool dark) const
{
  return m_square[dark];
}

const QPixmap& BoardTheme::originalPixmap(Piece p) const
{
  return m_originalPixmaps[p];
}

const QPixmap& BoardTheme::originalSquare(bool dark) const
{
  return m_originalSquare[dark];
}

QString BoardTheme::pieceThemeName() const
{
  int start = m_pieceFilename.findRev('/') + 1;
  return m_pieceFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

QString BoardTheme::boardThemeName() const
{
  int start = m_boardFilename.findRev('/') + 1;
  return m_boardFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

bool BoardTheme::isValid() const
{
  return !m_pieceFilename.isNull() && !m_boardFilename.isNull();
}

bool BoardTheme::load(const QString& themeFile, LoadTheme load)
{
  QString themePath = QString("%1/themes/%2.png").arg(AppSettings->dataPath()).arg(themeFile);
  QPixmap big;
  if (!big.load(themePath) || big.width() < 160)
    return false;
  int realsize = big.height() / 2;
  if (realsize != big.width() / 7)
    return false;

  /* Cut big theme bitmap into separate pieces */
  if (load & LoadPieces)
  {
    for (int i = 0; i<ConstPieceTypes; i++)
      m_originalPixmaps[i].resize(realsize, realsize);
    copyBlt(&m_originalPixmaps[WhiteRook], 0, 0, &big, 0, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[WhiteKnight], 0, 0, &big, 1 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[WhiteBishop], 0, 0, &big, 2 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[WhiteQueen], 0, 0, &big, 3 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[WhiteKing], 0, 0, &big, 4 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[WhitePawn], 0, 0, &big, 5 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackRook], 0, 0, &big, 0, realsize, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackKnight], 0, 0, &big, 1 * realsize, realsize, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackBishop], 0, 0, &big, 2 * realsize, realsize, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackQueen], 0, 0, &big, 3 * realsize, realsize, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackKing], 0, 0, &big, 4 * realsize, realsize, realsize, realsize);
    copyBlt(&m_originalPixmaps[BlackPawn], 0, 0, &big, 5 * realsize, realsize, realsize, realsize);
    m_pieceFilename = themePath;
  }

  /* Background */
  if (load & LoadBoard)
  {
    m_originalSquare[0].resize(realsize, realsize);
    m_originalSquare[1].resize(realsize, realsize);
    copyBlt(&m_originalSquare[0], 0, 0, &big, 6 * realsize, 0, realsize, realsize);
    copyBlt(&m_originalSquare[1], 0, 0, &big, 6 * realsize, realsize, realsize, realsize);
    m_boardFilename = themePath;
  }
  // Restore previous size
  setSize(size() ? size() : realsize);
  setSquareType(m_squareType);
  return true;
}

bool BoardTheme::load(const QString& pieceFile, const QString& boardFile)
{
  if (pieceFile == boardFile)
    return load(pieceFile, LoadAll);
  else
    return load(pieceFile, LoadPieces) && load(boardFile, LoadBoard);
}

int BoardTheme::size() const
{
  return m_size;
}


void BoardTheme::setSize(int value)
{
  if (!isValid())
    return;
  m_size = value;
  for (int i = 1; i<ConstPieceTypes; i++)
    m_pixmaps[i].convertFromImage(m_originalPixmaps[i].convertToImage().smoothScale(value, value));
  m_square[0].resize(value, value);
  m_square[1].resize(value, value);
  setSquareType(m_squareType);
}

BoardTheme::BoardSquare BoardTheme::squareType() const
{
  return m_squareType;
}

void BoardTheme::setSquareType(BoardSquare type)
{
  if(type == Unscaled && size() > m_originalPixmaps[WhiteRook].width()) {
    type = Scaled;
  }

  m_squareType = type;
  if (!isValid())
    return;
  switch(type)
  {
    case Plain:
      m_square[0].fill(lightColor());
      m_square[1].fill(darkColor());
      break;
    case Scaled:
      m_square[0].convertFromImage(m_originalSquare[0].convertToImage().smoothScale(size(),
        size()));
      m_square[1].convertFromImage(m_originalSquare[1].convertToImage().smoothScale(size(),
        size()));
      break;
    case Unscaled:
      copyBlt(&m_square[0], 0, 0, &m_originalSquare[0], 0, 0, size(), size());
      copyBlt(&m_square[1], 0, 0, &m_originalSquare[1], 0, 0, size(), size());
      break;
  }
}

