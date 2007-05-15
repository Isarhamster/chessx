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
#include <QPixmap>

#include "boardtheme.h"
#include "settings.h"

BoardTheme::BoardTheme()
{}

BoardTheme::~BoardTheme()
{}

const QPixmap& BoardTheme::piece(Piece p) const
{
	return m_piece[p];
}

const QPixmap& BoardTheme::square(bool dark) const
{
	return m_square[dark];
}

const QPixmap& BoardTheme::originalPiece(Piece p) const
{
	return m_originalPiece[p];
}

const QPixmap& BoardTheme::originalSquare(bool dark) const
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
	return !m_pieceFilename.isNull();
}

bool BoardTheme::load(const QString& themeFile, LoadTheme load)
{
	QString themePath = QString("%1/%2.png").arg(themeDirectory()).arg(themeFile);
	QPixmap big;
	if (!big.load(themePath) || big.width() < 160)
		return false;
	int realsize = big.height() / 2;
	if (realsize != big.width() / 7)
		return false;

	/* Cut big theme bitmap into separate pieces */
	if (load & LoadPieces) {
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
	if (load & LoadBoard) {
		m_originalSquare[0] = big.copy(6 * realsize, 0, realsize, realsize);
		m_originalSquare[1] = big.copy(6 * realsize, realsize, realsize, realsize);
		m_boardFilename = themePath;
	}
	// Restore previous size
	if (size().isEmpty())
		setSize(QSize(realsize, realsize));
	else
		setSize(size());
	updateSquares();
	return true;
}

bool BoardTheme::load(const QString& pieceFile, const QString& boardFile)
{
	if (pieceFile == boardFile)
		return load(pieceFile, LoadAll);
	else if (!boardFile.isEmpty())
		return load(pieceFile, LoadPieces) && load(boardFile, LoadBoard);
	else {
		m_boardFilename = QString();
		return load(pieceFile, LoadPieces);
	}
}

QSize BoardTheme::size() const
{
	return m_size;
}

QPoint BoardTheme::pieceCenter() const
{
	return QPoint(m_size.width() / 2, m_size.height() / 2);
}

QRect BoardTheme::rect() const
{
	return QRect(QPoint(0, 0), m_size);
}

void BoardTheme::setSize(const QSize& value)
{
	if (!isValid())
		return;
	m_size = value;
	for (int i = 1; i < ConstPieceTypes; i++)
		m_piece[i] = m_originalPiece[i].scaled(m_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	updateSquares();
}

void BoardTheme::updateSquares()
{
	if (!isValid())
		return;
	bool scale = m_size.width() > m_originalPiece[WhiteRook].width()
		     || m_size.height() > m_originalPiece[WhiteRook].height()
		     || m_size.width() < 30 || m_size.height() < 30;
	if (isBoardPlain()) {
		m_square[0] = QPixmap(m_size);
		m_square[0].fill(color(LightSquare).rgb());
		m_square[1] = QPixmap(m_size);
		m_square[1].fill(color(DarkSquare).rgb());
	} else if (scale) {
		m_square[0] =  m_originalSquare[0].scaled(size(), Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);
		m_square[1] =  m_originalSquare[1].scaled(size(), Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);
	} else {
		m_square[0] =  m_originalSquare[0].copy(rect());
		m_square[1] =  m_originalSquare[1].copy(rect());
	}
}

QString BoardTheme::themeDirectory() const
{
	return AppSettings->dataPath() + "/themes";
}

bool BoardTheme::isBoardPlain() const
{
	return m_boardFilename.isEmpty();
}

void BoardTheme::setColor(ColorRole role, const QColor& value)
{
	m_colors[role] = value;
	if (role == LightSquare || role == DarkSquare)
		updateSquares();
}

QColor BoardTheme::color(ColorRole role) const
{
	return m_colors[role];
}

