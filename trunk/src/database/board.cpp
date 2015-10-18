/***************************************************************************
 *   (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>                     *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QDataStream>
#include <QFile>
#include <QHash>
#include "board.h"
#include "guess.h"
#include "square.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

QHash<quint64, QList<Square> > ecoGuessPositions;

const Board Board::standardStartBoard = getStandardStartBoard();

Board::Board()
    : m_hashValue(0)
{
}

Board::Board(QString fen)
{
    if(!fromFen(fen))
    {
        clear();
    }
}

void Board::clear()
{
    BitBoard::clear();
    m_hashValue = 0;
    m_squareAnnotation = "";
}

void Board::setStandardPosition()
{
    BitBoard::setStandardPosition();

//	Just use precalculated hash values which is _much_ faster
//	createHash();
    m_hashValue = 17059429555746339296ULL;
    m_squareAnnotation = "";
}

bool Board::fromFen(const QString& fen)
{
    if(BitBoard::fromFen(fen))
    {
        createHash();
        m_squareAnnotation = "";
        return true;
    }
    return false;
}

bool Board::from64Char(const QString& qcharboard)
{
    if (BitBoard::from64Char(qcharboard))
    {
        createHash();
        m_squareAnnotation = "";
        return true;
    }
    return false;
}

void Board::setAt(Square s, Piece p)
{
    hashPiece(s, pieceAt(s));
    BitBoard::setAt(s, p);
    hashPiece(s, p);
}

void Board::removeFrom(Square s)
{
    hashPiece(s, pieceAt(s));
    removeAt(s);
}

void Board::setToMove(Color c)
{
    if(toMove() != c)
    {
        BitBoard::setToMove(c);
        hashToMove();
    }
}

void Board::swapToMove()
{
    setToMove(oppositeColor(toMove()));
}

bool Board::doIt(const Move& m, bool undo)
{
    if(m.isNullMove())
    {
        hashEpSquare();
        hashToMove();
        if(undo)
        {
            BitBoard::undoMove(m);
        }
        else
        {
            BitBoard::doMove(m);
        }
        hashEpSquare();
        hashToMove();
        return true;
    }

    if(!m.isLegal())
    {
        return false;
    }

    CastlingRights oldCastlingRights = castlingRights();

    hashEpSquare();
    hashPiece(m.from(), m.pieceMoved());
    if(m.isPromotion())
    {
        hashPiece(m.to(), m.promotedPiece());
    }
    else
    {
        hashPiece(m.to(), m.pieceMoved());
    }
    if(m.isCastling())
    {
        Piece p = (m == White ? WhiteRook : BlackRook);
        hashPiece(m.castlingRookFrom(), p);
        hashPiece(m.castlingRookTo(), p);
    }
    else if(m.isEnPassant())
    {
        if(m == White)
        {
            hashPiece(Square(m.to() - 8), m.capturedPiece());
        }
        else
        {
            hashPiece(Square(m.to() + 8), m.capturedPiece());
        }
    }
    else if(m.capturedPiece())
    {
        hashPiece(m.to(), m.capturedPiece());
    }

    if(undo)
    {
        BitBoard::undoMove(m);
    }
    else
    {
        BitBoard::doMove(m);
    }

    hashToMove();
    hashEpSquare();
    hashCastlingRights(oldCastlingRights);
    return true;
}

void Board::hashPiece(Square s, Piece p)
{
    if((p > Empty) && (p < InvalidPiece))
    {
        m_hashValue ^= RAND_VALUES[p - 1][s];
    }
}

void Board::hashToMove()
{
    m_hashValue = m_hashValue ^ RAND_TO_MOVE;
}

void Board::hashCastlingRights(CastlingRights oldCastlingRights)
{
    oldCastlingRights ^= castlingRights();
    if(oldCastlingRights & WhiteKingside)
    {
        m_hashValue ^= RAND_WHITE_CASTLING_KS;
    }
    if(oldCastlingRights & WhiteQueenside)
    {
        m_hashValue ^= RAND_WHITE_CASTLING_QS;
    }
    if(oldCastlingRights & BlackKingside)
    {
        m_hashValue ^= RAND_BLACK_CASTLING_KS;
    }
    if(oldCastlingRights & BlackQueenside)
    {
        m_hashValue ^= RAND_BLACK_CASTLING_QS;
    }
}

void Board::hashEpSquare()
{
    int epSquareIndex;
    Square sq = enPassantSquare();
    if(sq >= 16 && sq <= 23)
    {
        epSquareIndex = sq - 16;
    }
    else if(sq >= 40 && sq <= 47)
    {
        epSquareIndex = sq - 32;
    }
    else
    {
        return;
    }
    m_hashValue ^= RAND_EN_PASSANT[epSquareIndex];
}

void Board::createHash()
{
    m_hashValue = 0;
    for(Square square = a1; square < NumSquares; ++square)
    {
        hashPiece(square, pieceAt(square));
    }
    if(toMove() == Black)
    {
        hashToMove();
    }
    hashCastlingRights(CastlingRights(0));
    hashEpSquare();
}

bool Board::ecoMove(const Square square, int* from, int* to) const
{
    quint64 key = getHashPlusSquare(square);
    if(ecoGuessPositions.contains(key))
    {
        if(from && to)
        {
            QList<Square> data = ecoGuessPositions[key];
            *from = data[0];
            *to = data[1];
        }
        return true;
    }
    return false;
}

bool Board::loadEcoFile(const QString& ecoFile)
{
    QFile file(ecoFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream sin(&file);
        quint32 id;
        sin >> id;
        if(id == COMPILED_GUESS_FILE_ID)
        {
            sin >> ecoGuessPositions;
        }
        return true;
    }
    return false;
}

quint64 Board::getHashPlusSquare(const Square square) const
{
    return m_hashValue ^ RAND_ECO_SQUARE[square];
}

quint64 Board::getHashValue() const
{
    return m_hashValue;
}

void Board::setSquareAnnotation(QString squareAnnotation)
{
    m_squareAnnotation = squareAnnotation;
}

QString Board::squareAnnotation() const
{
    return m_squareAnnotation;
}

void Board::setArrowAnnotation(QString arrowAnnotation)
{
    m_arrowAnnotation = arrowAnnotation;
}

QString Board::arrowAnnotation() const
{
    return m_arrowAnnotation;
}

int Board::ScoreMaterial() const
{
    QString fen = toFen();
    return Guess::scorePosFromFen(fen.toLatin1());
}

Board Board::getStandardStartBoard()
{
    Board b;
    b.setStandardPosition();
    return b;
}
