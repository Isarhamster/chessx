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

#include <QBitmap>
#include <QImage>

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

const QImage& BoardTheme::piece(Piece p) const
{
  return m_piece[p];
}

const QImage& BoardTheme::square(bool dark) const
{
  return m_square[dark];
}

const QImage& BoardTheme::originalPiece(Piece p) const
{
  return m_originalPiece[p];
}

const QImage& BoardTheme::originalSquare(bool dark) const
{
  return m_originalSquare[dark];
}

QString BoardTheme::pieceThemeName() const
{
  int start = m_pieceFilename.lastIndexOf('/') + 1;
  return m_pieceFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

QString BoardTheme::boardThemeName() const
{
  int start = m_boardFilename.lastIndexOf('/') + 1;
  return m_boardFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

bool BoardTheme::isValid() const
{
  return !m_pieceFilename.isNull() && !m_boardFilename.isNull();
}

bool BoardTheme::load(const QString& themeFile, LoadTheme load)
{
  QString themePath = QString("%1/themes/%2.png").arg(AppSettings->dataPath()).arg(themeFile);
  QImage big;
  if (!big.load(themePath) || big.width() < 160)
    return false;
  int realsize = big.height() / 2;
  if (realsize != big.width() / 7)
    return false;

  /* Cut big theme bitmap into separate pieces */
  if (load & LoadPieces)
  {
    m_originalPiece[WhiteRook] = big.copy(0 * realsize, 0, realsize, realsize);
    m_originalPiece[WhiteKnight] = big.copy(1 * realsize, 0, realsize, realsize);
    m_originalPiece[WhiteBishop] = big.copy(2 * realsize, 0, realsize, realsize);
    m_originalPiece[WhiteQueen] = big.copy(3 * realsize, 0, realsize, realsize);
    m_originalPiece[WhiteKing] = big.copy(4 * realsize, 0, realsize, realsize);
    m_originalPiece[WhitePawn] = big.copy(5 * realsize, 0, realsize, realsize);
    m_originalPiece[BlackRook] = big.copy(0 * realsize, realsize, realsize, realsize);
    m_originalPiece[BlackKnight] = big.copy(1 * realsize, realsize, realsize, realsize);
    m_originalPiece[BlackBishop] = big.copy(2 * realsize, realsize, realsize, realsize);
    m_originalPiece[BlackQueen] = big.copy(3 * realsize, realsize, realsize, realsize);
    m_originalPiece[BlackKing] = big.copy(4 * realsize, realsize, realsize, realsize);
    m_originalPiece[BlackPawn] = big.copy(5 * realsize, realsize, realsize, realsize);
    m_pieceFilename = themePath;
  }

  /* Background */
  if (load & LoadBoard)
  {
    m_originalSquare[0] = big.copy(6 * realsize, 0, realsize, realsize);
    m_originalSquare[1] = big.copy(6 * realsize, realsize, realsize, realsize);
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

QRect BoardTheme::rect() const
{
  return QRect(QPoint(0, 0), QSize(m_size, m_size));
}

void BoardTheme::setSize(int value)
{
  if (!isValid())
    return;
  m_size = value;
  for (int i = 1; i<ConstPieceTypes; i++)
    m_piece[i] = m_originalPiece[i].scaled(value, value, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  setSquareType(m_squareType);
}

BoardTheme::BoardSquare BoardTheme::squareType() const
{
  return m_squareType;
}

void BoardTheme::setSquareType(BoardSquare type)
{
  if(type == Unscaled && size() > m_originalPiece[WhiteRook].width()) {
    type = Scaled;
  }

  m_squareType = type;
  if (!isValid())
    return;
  switch(type)
  {
    case Plain:
      m_square[0] = QImage(size(), size(), QImage::Format_RGB32);
      m_square[0].fill(lightColor().rgb());
      m_square[1] = QImage(size(), size(), QImage::Format_RGB32);
      m_square[1].fill(darkColor().rgb());
      break;
    case Scaled:
      m_square[0] =  m_originalSquare[0].scaled(size(), size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
      m_square[1] =  m_originalSquare[1].scaled(size(), size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
      break;
    case Unscaled:
      m_square[0] =  m_originalSquare[0].copy(rect());
      m_square[1] =  m_originalSquare[1].copy(rect());
      break;
  }
}


