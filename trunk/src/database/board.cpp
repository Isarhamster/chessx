/***************************************************************************
                          board.cpp - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005, 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                           (C) 2005 Marius Roets <roets.marius@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <QFile>
#include "board.h"

QMap<quint64, QList<Square> > ecoGuessPositions;

Board::Board()
	: m_hashValue(0), m_hashValue2(0)
{
}

void Board::clear()
{
	BitBoard::clear();
	createHash();
}

void Board::setStandardPosition()
{
	BitBoard::setStandardPosition();
	createHash();
}

bool Board::fromFen(const QString& fen, int* moveNumber)
{
	if (BitBoard::fromFen(fen, moveNumber)) {
		createHash();
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
	if (toMove() != c)
		hashToMove();
	BitBoard::setToMove(c);
}

bool Board::doIt(const Move& m, bool undo)
{
	Q_ASSERT(m.isLegal());

	if (!m.isLegal())
		return false;

	CastlingRights oldCastlingRights = castlingRights();

	hashEpSquare();
	hashPiece(m.from(), m.pieceMoved());
	if (m.isPromotion()) {
		hashPiece(m.to(), m.promotedPiece());
	} else	hashPiece(m.to(), m.pieceMoved());
	if (m.isCastling()) {
		Piece p = (m == White ? WhiteRook : BlackRook);
		hashPiece(m.castlingRookFrom(), p);
		hashPiece(m.castlingRookTo(), p);
	} else if (m.isEnPassant()) {
		if (m == White)
			hashPiece(m.to()-8, m.capturedPiece());
		else	hashPiece(m.to()+8, m.capturedPiece());
	} else if (m.capturedPiece()) {
		hashPiece(m.to(), m.capturedPiece());
	}

	if (undo)
		BitBoard::undoMove(m);
	else	BitBoard::doMove(m);

	hashToMove();
	hashEpSquare();
	hashCastlingRights(oldCastlingRights);
	return true;
}

void Board::hashPiece(Square s, Piece p)
{
	if ((p > Empty) && (p < InvalidPiece)) {
		m_hashValue ^= RAND_VALUES[p-1][s];
		m_hashValue2 ^= RAND_VALUES2[p-1][s];
	}
}

void Board::hashToMove()
{
	m_hashValue = m_hashValue ^ RAND_TO_MOVE;
	m_hashValue2 = m_hashValue2 ^ RAND_TO_MOVE2;
}

void Board::hashCastlingRights(CastlingRights oldCastlingRights)
{
	oldCastlingRights ^= castlingRights();
	if (oldCastlingRights & WhiteKingside) {
		m_hashValue ^= RAND_WHITE_CASTLING_KS;
		m_hashValue2 ^= RAND_WHITE_CASTLING_KS2;
	}
	if (oldCastlingRights & WhiteQueenside) {
		m_hashValue ^= RAND_WHITE_CASTLING_QS;
		m_hashValue2 ^= RAND_WHITE_CASTLING_QS2;
	}
	if (oldCastlingRights & BlackKingside) {
		m_hashValue ^= RAND_BLACK_CASTLING_KS;
		m_hashValue2 ^= RAND_BLACK_CASTLING_KS2;
	}
	if (oldCastlingRights & BlackQueenside) {
		m_hashValue ^= RAND_BLACK_CASTLING_QS;
		m_hashValue2 ^= RAND_BLACK_CASTLING_QS2;
	}
}

void Board::hashEpSquare()
{
	int epSquareIndex;
	Square sq = enPassantSquare();
	if (sq >= 16 && sq <= 23)
		epSquareIndex = sq - 16;
	else if (sq >= 40 && sq <= 47)
		epSquareIndex = sq - 32;
	else	return;
	m_hashValue ^= RAND_EN_PASSANT[epSquareIndex];
	m_hashValue2 ^= RAND_EN_PASSANT2[epSquareIndex];
}

void Board::createHash()
{
	m_hashValue = 0;
	m_hashValue2 = 0;
	for (int i=0; i < MAX_SQUARES; ++i)
		hashPiece(i, pieceAt(i));
	if (toMove() == Black)
		hashToMove();
	hashCastlingRights(CastlingRights(0));
	hashEpSquare();
}

bool Board::ecoMove(const Square square, int* from, int* to) const
{
	quint64 key = getHashPlusSquare(square);
	if (ecoGuessPositions.contains(key)) {
		QList<Square> data = ecoGuessPositions[key];
		*from = data[0];
		*to = data[1];
		return true;
	}
	return false;
}

bool Board::loadEcoFile(const QString& ecoFile)
{
	QFile file(ecoFile);
	file.open(QIODevice::ReadOnly);
	QDataStream sin(&file);
	quint32 id;
	sin >> id;
	sin >> ecoGuessPositions;
	file.close();
	if (id != COMPILED_GUESS_FILE_ID)
		return false;
	return true;
}


quint64 Board::getHashPlusSquare(const Square square) const
{
	return m_hashValue ^ RAND_EN_PASSANT2[square];
}

quint64 Board::getHashValue() const
{
	return m_hashValue;
}

quint64 Board::getHashValue2() const
{
	return m_hashValue2;
}

