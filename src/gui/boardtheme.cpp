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

#include "boardtheme.h"
#include "settings.h"
#include "messagedialog.h"

#include <QPixmap>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
    return m_square[dark ? 1 : 0];
}

const QPixmap& BoardTheme::originalPiece(Piece p) const
{
    return m_originalPiece[p];
}

const QPixmap& BoardTheme::originalSquare(bool dark) const
{
    return m_originalSquare[dark ? 1 : 0];
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

bool BoardTheme::loadPieces(const QString& pieces, int effect)
{
    QString effectPath;
    if(effect == Outline)
    {
        effectPath = "outline";
    }
    else if(effect == Shadow)
    {
        effectPath = "shadow";
    }

    QString themePath = AppSettings->getThemePath(effectPath, pieces);

    QPixmap big;
    if(!big.load(themePath) || big.width() < 160)
    {
        return false;
    }

    big.setDevicePixelRatio(2.0);

    int realsize = big.height() / 2;
    if(realsize == big.width() / 6)
    {
        /* Cut big theme bitmap into separate pieces */
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
    }
    else
    {
        // Test SCID format
        realsize = big.height();
        if(realsize != big.width() / 12)
        {
            return false;
        }
        /* Cut big theme bitmap into separate pieces */
        m_originalPiece[WhitePawn] = big.copy(0 * realsize, 0, realsize, realsize);
        m_originalPiece[WhiteKnight] = big.copy(1 * realsize, 0, realsize, realsize);
        m_originalPiece[WhiteBishop] = big.copy(2 * realsize, 0, realsize, realsize);
        m_originalPiece[WhiteRook] = big.copy(3 * realsize, 0, realsize, realsize);
        m_originalPiece[WhiteQueen] = big.copy(4 * realsize, 0, realsize, realsize);
        m_originalPiece[WhiteKing] = big.copy(5 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackPawn] = big.copy(6 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackKnight] = big.copy(7 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackBishop] = big.copy(8 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackRook] = big.copy(9 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackQueen] = big.copy(10 * realsize, 0, realsize, realsize);
        m_originalPiece[BlackKing] = big.copy(11 * realsize, 0, realsize, realsize);
    }

    m_pieceFilename = themePath;

    if(size().isEmpty())
    {
        setSize(QSize(realsize, realsize));
    }
    else
    {
        setSize(size());
    }
    return true;
}

bool BoardTheme::loadBoard(const QString& board)
{
    if(board.isEmpty())
    {
        m_boardFilename = board;
        updateSquares();
        return true;
    }
    QString themePath = AppSettings->getBoardPath(board);

    QPixmap big;
    if(!big.load(themePath))
    {
        return false;
    }
    int realsize = big.height() / 2;
    if(realsize != big.width())
    {
        return false;
    }

    m_originalSquare[0] = big.copy(0, 0, realsize, realsize);
    m_originalSquare[1] = big.copy(0, realsize, realsize, realsize);
    m_boardFilename = themePath;

    updateSquares();
    return true;
}

void BoardTheme::configure()
{
    AppSettings->beginGroup("/Board/");
    setColor(BoardTheme::LightSquare, AppSettings->getValue("lightColor").value<QColor>());
    setColor(BoardTheme::DarkSquare, AppSettings->getValue("darkColor").value<QColor>());
    setColor(BoardTheme::Highlight, AppSettings->getValue("highlightColor").value<QColor>());
    setColor(BoardTheme::Frame, AppSettings->getValue("frameColor").value<QColor>());
    setColor(BoardTheme::CurrentMove, AppSettings->getValue("currentMoveColor").value<QColor>());
    setColor(BoardTheme::StoredMove, AppSettings->getValue("storedMoveColor").value<QColor>());
    setColor(BoardTheme::Threat, AppSettings->getValue("threatColor").value<QColor>());
    setColor(BoardTheme::Target, AppSettings->getValue("targetColor").value<QColor>());
    setColor(BoardTheme::Check, AppSettings->getValue("checkColor").value<QColor>());
    setColor(BoardTheme::Wall, AppSettings->getValue("wallColor").value<QColor>());
    setColor(BoardTheme::UnderProtected, AppSettings->getValue("underprotectedColor").value<QColor>());

    QString pieceTheme = AppSettings->getValue("pieceTheme").toString();
    int pieceEffect = AppSettings->getValue("pieceEffect").toInt();
    QString boardTheme = AppSettings->getValue("boardTheme").toString();
    AppSettings->endGroup();

    if(!loadPieces(pieceTheme, pieceEffect))
    {
        static bool errorShown = false;
        if (!errorShown)
        {
            errorShown = true;
            MessageDialog::error(tr("Cannot find piece data.\nPlease check your installation."));
        }
    }
    if (!loadBoard(boardTheme))
    {
        static bool errorShown = false;
        if (!errorShown)
        {
            errorShown = true;
            MessageDialog::error(tr("Cannot find board data.\nPlease check your installation."));
        }
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
    if(!isValid())
    {
        return;
    }
    m_size = value;
    for(int i = 1; i < ConstPieceTypes; i++)
    {
        float r = m_originalPiece[i].devicePixelRatio();
        m_piece[i] = m_originalPiece[i].scaled(m_size * r, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        m_piece[i].setDevicePixelRatio(r);
    }
    updateSquares();
}

void BoardTheme::updateSquares()
{
    if(!isValid())
    {
        return;
    }
    bool scale = m_size.width() > m_originalPiece[WhiteRook].width()
                 || m_size.height() > m_originalPiece[WhiteRook].height()
                 || m_size.width() < 30 || m_size.height() < 30;
    if (AppSettings->getValue("/Board/AlwaysScale").toBool())
    {
        scale = true;
    }
    if(isBoardPlain())
    {
        m_square[0] = QPixmap(m_size);
        m_square[0].fill(color(LightSquare).rgb());
        m_square[1] = QPixmap(m_size);
        m_square[1].fill(color(DarkSquare).rgb());
    }
    else if(scale)
    {
        QSize sz = size();
        m_square[0] =  m_originalSquare[0].scaled(sz, Qt::IgnoreAspectRatio,
                       Qt::SmoothTransformation);
        m_square[1] =  m_originalSquare[1].scaled(sz, Qt::IgnoreAspectRatio,
                       Qt::SmoothTransformation);
    }
    else
    {
        m_square[0] =  m_originalSquare[0].copy(rect());
        m_square[1] =  m_originalSquare[1].copy(rect());
    }
}

bool BoardTheme::isBoardPlain() const
{
    return m_boardFilename.isEmpty();
}

void BoardTheme::setColor(ColorRole role, const QColor& value)
{
    m_colors[role] = value;
    if(role == LightSquare || role == DarkSquare)
    {
        updateSquares();
    }
}

QColor BoardTheme::color(ColorRole role) const
{
    return m_colors[role];
}

void BoardTheme::setEnabled(bool enabled)
{
    if(enabled)
    {
        AppSettings->beginGroup("/Board/");
        setColor(BoardTheme::LightSquare, AppSettings->getValue("lightColor").value<QColor>());
        setColor(BoardTheme::DarkSquare, AppSettings->getValue("darkColor").value<QColor>());
        QString boardTheme = AppSettings->getValue("boardTheme").toString();
        AppSettings->endGroup();
        loadBoard(boardTheme);
    }
    else
    {
        setColor(BoardTheme::LightSquare, QColor(Qt::lightGray));
        setColor(BoardTheme::DarkSquare, QColor(Qt::darkGray));
        loadBoard(QString());
    }
}
