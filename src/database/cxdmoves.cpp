/***************************************************************************
                          cxdmoves.cpp  - storing and retrieving moves
                                        for chessxdatabase
                             -------------------
   begin                : 15 July 2007
   copyright            : (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 3 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License version 3 for more details.                 *
 *                                                                         *
 ***************************************************************************/

#include <QDir>
#include <QStringList>
#include <QtDebug>
#include <QXmlDefaultHandler>
#include <QMessageBox>
#include <QIODevice>
#include <QDataStream>

#include "board.h"
#include "nag.h"

#include "cxdmoves.h"


namespace   //unnamed namespace for helper classes and functions
{

class error{}; // default error throwed in exception handling

// ------------------------------------------------
// ------------------------------------------------
// |  Encoding and decoding of games              |
// ------------------------------------------------
// ------------------------------------------------

const unsigned char nopiece = 255;
const Square standardwpiecesquares[16] = {e1, a1, b1, c1, d1, f1, g1, h1, a2, b2, c2, d2, e2, f2, g2, h2};
const Square standardbpiecesquares[16] = {e8, a8, b8, c8, d8, f8, g8, h8, a7, b7, c7, d7, e7, f7, g7, h7};
const PieceType standardPieceTypes[16] = {King, Rook, Knight, Bishop, Queen, Bishop, Knight, Rook,
		Pawn, Pawn, Pawn, Pawn, Pawn, Pawn, Pawn, Pawn
					 };

// Parsing bytes
const unsigned char nag = 11;
const unsigned char startComment = 12;
const unsigned char beginVariation = 13;
const unsigned char endVariation = 14;
const unsigned char nextByteIsSpecial = 15;

const unsigned char endComment = '\0';


// class for encoding of games
class Encoding
{
public:
	Encoding() {}

	// We only try to guarantee that
	// the piece number of a square is correct in case that
	// there is a piece on the corresponding square.
	unsigned char piecenumbers[64];

	void encodeComment(const QString& qs, QIODevice& qiod) const;
	void setToStandardPieceNumbers();
	bool setPieceNumbers(const Board& b);
	void writeMoveByte(const unsigned char& piecenumber,
			   const unsigned char& movecode,
			   QIODevice& qiod) const;

	void encodeKing(const Move& m, QIODevice& qiod);
	void encodeKnight(const Move& m, QIODevice& qiod);
	void encodeRook(const Move& m, QIODevice& qiod);
	void encodeBishop(const Move& m, QIODevice& qiod);
	void encodeQueen(const Move& m, QIODevice& qiod);
	void encodePawn(const Move& m, QIODevice& qiod);
	void encodeMove(const Move& m, QIODevice& qiod);
	void encodeNags(const NagSet& ns, QIODevice& qiod);
	void encodeVariation(Game& game, const int& variation, QIODevice& qiod);
	bool encodeNonHeader(Game& game, QIODevice& qiod);

};

// class for decoding of games previously encoded with the Encoding class
class Decoding
{
public:
	Decoding() {}

	// The arrays link the piece numbers
	// of the white resp black pieces to their corresponding
	// squares. If a piece number is not used, the corresponding
	// square is InvalidSquare (=255).
	Square wpiecesquares[16];
	Square bpiecesquares[16];

	// The arrays link the piece numbers
	// to the piece types. During decoding these arrays have
	// to be correct only for piecenumbers corresponding to
	// existing pieces.
	PieceType wpiecetypes[16];
	PieceType bpiecetypes[16];

	int getWpiecenumber(const Square& s) const;
	int getBpiecenumber(const Square& s) const;

	void decodeComment(QString& qs, QIODevice& qiod) const;
	void setToStandardPieceSquares();
	void setToStandardPieceTypes();

        void setPieceTypesAndSquares(const Board& game);

	void squareDiffDecode(const unsigned char& piecenumber,
			      const unsigned char& movecode,
			      const Color& color,
			      Square& from,
			      Square& to,
			      const unsigned char* squareDiff);

	void decodeKing(const unsigned char& piecenumber,
			const unsigned char& movecode,
			const Color& color,
			Square& from,
			Square& to);
	void decodeKnight(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to);
	void decodeRook(const unsigned char& piecenumber,
			const unsigned char& movecode,
			const Color& color,
			Square& from,
			Square& to);
	void decodeBishop(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to);
	void decodeQueen(const unsigned char& piecenumber,
			 const unsigned char& movecode,
			 const Color& color,
			 Square& from,
			 Square& to,
			 QIODevice& qiod);
	void decodePawn(const unsigned char& piecenumber,
			const unsigned char& movecode,
			const Color& color,
			Square& from,
			Square& to,
			PieceType& pt);
	void decodeMove(const unsigned char& piecenumber,
			const unsigned char& movecode,
			const Color& color,
			Square& from,
			Square& to,
			QIODevice& qiod,
			PieceType& pt);
	void decodeNags(NagSet& ns, QIODevice& qiod) const;
	void decodeVariation(Game& game, QIODevice& qiod);
	void decodeNonHeader(Game& game, QIODevice& qiod);

};

// ------------------------------------------------
// ------------------------------------------------
// | Definitions for class Encoding               |
// ------------------------------------------------
// ------------------------------------------------

void Encoding::encodeComment(const QString& qs, QIODevice& qiod) const
{
	qiod.putChar(startComment);
	std::string s(qs.toStdString());
	qiod.write(s.c_str(), s.size());
	qiod.putChar(endComment);
}

void Encoding::setToStandardPieceNumbers()
{
	// It would not be necessary to put everywhere "nopiece" where there
	// is no piece because piecenumbers only has to be "correct" for
	// squares with pieces on them. However it is neither a bottleneck
	// operation and simplifies some error tracking.
	for (int i = 0; i < 4 ; ++i) piecenumbers[i] = i + 1;
	piecenumbers[4] = 0;
	for (int i = 5; i < 16; ++i) piecenumbers[i] = i;
	for (int i = 16; i < 48; ++i) piecenumbers[i] = nopiece;
	for (int i = 48; i < 56; ++i) piecenumbers[i] = i - 40;
	for (int i = 56; i < 60; ++i) piecenumbers[i] = i - 55;
	piecenumbers[60] = 0;
	for (int i = 61; i < 64; ++i) piecenumbers[i] = i - 56;

// piecenumbers is finally of the following form:
//    piecenumbers=
//      {  1,  2,  3,  4,  0,  5,  6,  7,
//         8,  9, 10, 11, 12, 13, 14, 15,
//         nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,
//         nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,
//         nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,
//         nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,nopiece,
//         8,  9, 10, 11, 12, 13, 14, 15,
//         1,  2,  3,  4,  0,  5,  6,  7
//      };
}

// Sets piecenumbers according to the position given by b. If the board
// b is possible to encode then true is returned, otherwise false. A
// position is possible to encode if every site has no more than 16
// pieces containing exactly one King.
bool Encoding::setPieceNumbers(const Board& b)
{
	unsigned char wPieceIndex(1), bPieceIndex(1);
	Piece currentPiece;
	for (int i = 0; i < 64; ++i) {
		currentPiece = b.pieceAt(i);
		if (currentPiece == Empty)
			{piecenumbers[i] = nopiece;}
		else if (currentPiece == WhiteKing || currentPiece == BlackKing)
			{piecenumbers[i] = 0;}
		else if (isWhite(currentPiece)) {
			if (wPieceIndex > 15) return 0;
			else {
				piecenumbers[i] = wPieceIndex;
				++wPieceIndex;
			}
		} else if (isBlack(currentPiece)) {
			if (bPieceIndex > 15) return 0;
			else {
				piecenumbers[i] = bPieceIndex;
				++bPieceIndex;
			}
		}
	}
	return 1;
}

void Encoding::writeMoveByte(const unsigned char& piecenumber,
			     const unsigned char& movecode,
			     QIODevice& qiod) const
{qiod.putChar((piecenumber << 4) | movecode);}

void Encoding::encodeKing(const Move& m, QIODevice& qiod)
{
	// The encoding of the King move is a function of the difference
	// of the end-sqare to the start-square. The following differences
	// are possible:
	// -9, -8, -7, -1, 1, 7, 8, 9, and -2 and 2 for castling.
	// These differences will be converted into a value in the
	// range [1-10] by adding 9 and looking up in the array val.
	//
	// The move value 0 corresponds to an "emtpy move", the values
	// 1-8 are one-square moves, 9 and 10 are castling.

	int diff(m.to() - m.from());
	static const unsigned char val[19] = {
		// -9 -8 -7 -6 -5 -4 -3 -2 -1  0  1   2  3  4  5  6  7  8  9
		1, 2, 3, 0, 0, 0, 0, 9, 4, 0, 5, 10, 0, 0, 0, 0, 6, 7, 8
	};

	writeMoveByte(piecenumbers[m.from()], val[diff+9], qiod);
	piecenumbers[m.to()] = piecenumbers[m.from()];

	// Adjust piecenumbers of rooks in case of castling.
	if (diff == -2 || diff == 2) {
		switch (m.to()) {
		case g1:
			piecenumbers[f1] = piecenumbers[h1];
			break;
		case g8:
			piecenumbers[f8] = piecenumbers[h8];
			break;
		case c1:
			piecenumbers[d1] = piecenumbers[a1];
			break;
		case c8:
			piecenumbers[d8] = piecenumbers[a8];
			break;
		}
	}
}

void Encoding::encodeKnight(const Move& m, QIODevice& qiod)
{
	// Possible to-from differences:
	// -17, -15, -10, -6, 6, 10, 15, 17.
	// These differences will be converted into a value in the range [1-8]
	// by adding 17 and looking up in the array var.

	int diff(m.to() - m.from());
	static const unsigned char val[35] = {
		// -17 -16 -15 -14 -13 -12 -11 -10 -9 -8 -7 -6 -5 -4 -3 -2 -1  0
		1,  0,  2,  0,  0,  0,  0,  3,  0, 0, 0, 4, 0, 0, 0, 0, 0, 0,
		//  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
		0, 0, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 8
	};

	writeMoveByte(piecenumbers[m.from()], val[diff+17], qiod);
	piecenumbers[m.to()] = piecenumbers[m.from()];
}

void Encoding::encodeRook(const Move& m, QIODevice& qiod)
{
	// The numbers 0,1,...,7 are used for moves on the same
	// rank and indicate the file on which the rook move ends
	// (one number is not used as it corresponds to the starting
	// file of the rook). The number 8,9,...,15 are used in the
	// same way to indicate rook moves on the same file.
	unsigned char movecode(0);
	int diff(m.to() - m.from());
	if (diff % 8) // rook move on the same rank
		{movecode = m.to() % 8;}
	else // rook move on the same file
		{movecode = m.to() / 8 + 8;}
	writeMoveByte(piecenumbers[m.from()], movecode, qiod);
	piecenumbers[m.to()] = piecenumbers[m.from()];
}

void Encoding::encodeBishop(const Move& m, QIODevice& qiod)
{
	// The first bit in the decoding indicates if the
	// direction was down-left/up-right (this corresponds to
	// the value 0) or down-right/up-left.
	// The following three bits indicate the file on which
	// the Bishop moved.
	unsigned char movecode(0);
	int filediff((m.to() % 8) - (m.from() % 8));
	int rankdiff(m.to() / 8 - m.from() / 8);
	if ((rankdiff >= 0 && filediff <= 0) || (rankdiff <= 0 && filediff >= 0))
		{movecode += 8;}
	movecode += m.to() % 8;
	writeMoveByte(piecenumbers[m.from()], movecode, qiod);
	piecenumbers[m.to()] = piecenumbers[m.from()];
}

void Encoding::encodeQueen(const Move& m, QIODevice& qiod)
{
	// Horizontal and vertical Queen moves are encoded in one
	// byte, diagonal Queen moves will be encoded in a second
	// byte by indicating the end square. To indicate a diagonal
	// Queen move, the first byte
	// contains a "horizontal move" to its own square.
	if (m.from() / 8 == m.to() / 8) // horizontal move
		{writeMoveByte(piecenumbers[m.from()], m.to() % 8, qiod);}
	else if (m.from() % 8 == m.to() % 8) // vertical move
		{writeMoveByte(piecenumbers[m.from()], 8 + m.to() / 8, qiod);}
	else { // diagonal move
		// first byte contains horizontal "null" move
		writeMoveByte(piecenumbers[m.from()], m.from() % 8, qiod);
		// now comes the end square
		qiod.putChar(m.to());
	}
	piecenumbers[m.to()] = piecenumbers[m.from()];
}

void Encoding::encodePawn(const Move& m, QIODevice& qiod)
{
	// Pawn moves are encoded in the following way:
	// 0 : capture to a file with lower index (looking as
	//     the white player the capture goes to the left).
	//     this capture may be an en passant move
	// 1 : one square forward
	// 2 : capture to a file with higher index (looking as
	//     the white player the capture goes to the right)
	//    3/4/5 : 0/1/2 with promotion to Queen
	//    6/7/8 : 0/1/2 with promotion to Rook
	//  9/10/11 : 0/1/2 with promotion to Bishop
	// 12/13/14 : 0/1/2 with promotion to Knight
	// 15 : two squares forward

	int diff(m.to() - m.from());
	static const unsigned char val[33] = {
		// -16 -15 -14 -13 -12 -11 -10
		15, -1, -1, -1, -1, -1, -1,
		// -9 -8 -7 -6 -5 -4 -3 -2 -1  0
		0, 1, 2, -1, -1, -1, -1, -1, -1, -1,
		//  1  2  3  4  5  6  7  8  9
		-1, -1, -1, -1, -1, -1, 0, 1, 2,
		// 10 11 12 13 14 15 16
		-1, -1, -1, -1, -1, -1, 15

	};
	unsigned char movecode = val[diff+16];
	if (m.isPromotion()) {
		switch (pieceType(m.promotedPiece())) {
		case Queen:
			movecode += 3;
			break;
		case Rook:
			movecode += 6;
			break;
		case Bishop:
			movecode += 9;
			break;
		case Knight:
			movecode += 12;
			break;
		default:
			throw error();
		}
	}
	writeMoveByte(piecenumbers[m.from()], movecode, qiod);
	piecenumbers[m.to()] = piecenumbers[m.from()];
}

void Encoding::encodeMove(const Move& m, QIODevice& qiod)
{
	switch (m.pieceMoved()) {
	case WhiteKing:
	case BlackKing:
		encodeKing(m, qiod);
		break;
	case WhiteQueen:
	case BlackQueen:
		encodeQueen(m, qiod);
		break;
	case WhiteRook:
	case BlackRook:
		encodeRook(m, qiod);
		break;
	case WhiteBishop:
	case BlackBishop:
		encodeBishop(m, qiod);
		break;
	case WhiteKnight:
	case BlackKnight:
		encodeKnight(m, qiod);
		break;
	case WhitePawn:
	case BlackPawn:
		encodePawn(m, qiod);
		break;
	default:
		throw error();
	}
}

void Encoding::encodeNags(const NagSet& ns, QIODevice& qiod)
{
	for (int i = 0; i < ns.count(); ++i) {
		qiod.putChar(nag);
		qiod.putChar(char(ns.nags()[i]));
	}
}

// Encodes a variation and subvariations occuring in this variation
// by recursive calls. If variation=0 then all sibling variations at
// the first move will also be encoded. Assumes that piecenumbers
// is correctly set. The variation to encode must not be empty.
void Encoding::encodeVariation(Game& game, const int& variation, QIODevice& qiod)
{
	Move m;
	int var(variation);

	// The following variable holds the piecenumber of the piece
	// where the current move ends. It will be used for recursively
	// calling encodeVariation to set the Encoding object to the
	// right state.
	int to_pn;

	qiod.putChar(beginVariation);
	while (true) {
		m = game.move(var);
		encodeNags(game.nags(var), qiod);
		if (game.annotation(var) != "") encodeComment(game.annotation(var), qiod);
		to_pn = piecenumbers[m.to()];
		encodeMove(m, qiod);
		if (var == 0 && game.variationCount() > 1) {
			for (int i = 1; i < game.variationCount(); ++i) {
				Encoding cxEnc_sub(*this);
				// setting cxEnc_sub to its state one move earlier
				cxEnc_sub.piecenumbers[m.from()] = piecenumbers[m.to()];
				cxEnc_sub.piecenumbers[m.to()] = to_pn;
				if (m.isCastling()) {
					cxEnc_sub.piecenumbers[m.castlingRookFrom()] =
						piecenumbers[m.castlingRookTo()];
				}
				Game g(game);
				cxEnc_sub.encodeVariation(g, i, qiod);
			}
		}
		game.enterVariation(var);
		if (game.atEnd()) break;
		var = 0;
	}
	qiod.putChar(endVariation);
}

// Encodes the part of a Game belonging to the game-file
// at the current position of qiod. Returns false if the
// game could not have been encoded because setPieceNumbers
// failed.
bool Encoding::encodeNonHeader(Game& game, QIODevice& qiod)
{
	Board startBoard(game.startBoard());
	if (startBoard == standardStartBoard) {
		setToStandardPieceNumbers();
	} else {
		if (!setPieceNumbers(startBoard)) return 0;
	}

	// Encode start annotation.
	if (game.startAnnotation() != "")
		{encodeComment(game.startAnnotation(), qiod);}

	// Encode variations. The variation part of a game without
	// moves is encoded by beginVariation EndVariation.
	game.moveToStart();
	if (game.variationCount()) // game has variations
		{encodeVariation(game, 0, qiod);}
	else { // game has no variations to encode
		qiod.putChar(beginVariation);
		qiod.putChar(endVariation);
	}

	return 1;
}




// ------------------------------------------------
// ------------------------------------------------
// | Definitions for class Decoding               |
// ------------------------------------------------
// ------------------------------------------------

int Decoding::getWpiecenumber(const Square& s) const
{
	for (int i = 0; i < 16; ++i)
		{if(wpiecesquares[i] == s) return i;}
	return -1;
}

int Decoding::getBpiecenumber(const Square& s) const
{
	for (int i = 0; i < 16; ++i)
		{if(bpiecesquares[i] == s) return i;}
	return -1;
}

void Decoding::decodeComment(QString& qs, QIODevice& qiod) const
{
	qs = "";
	char c;
	qiod.getChar(&c);
	while (c != 0) {
		qs += c;
		qiod.getChar(&c);
	}
}

void Decoding::setToStandardPieceSquares()
{
	for (int i = 0; i < 16; ++i) {
		wpiecesquares[i] = standardwpiecesquares[i];
		bpiecesquares[i] = standardbpiecesquares[i];
	}
}

void Decoding::setToStandardPieceTypes()
{
	for (int i = 0; i < 16; ++i)
		{wpiecetypes[i] = bpiecetypes[i] = standardPieceTypes[i];}
}

void Decoding::setPieceTypesAndSquares(const Board& b)
{
	unsigned char wPieceIndex(1), bPieceIndex(1);
	Piece currentPiece;
	PieceType currentPieceType;
	for(int i=0; i<64; ++i) {
		currentPiece = b.pieceAt(i);
		currentPieceType = pieceType(currentPiece);
		if(isWhite(currentPiece))
		{
                  if(currentPieceType==King)
                  {
			wpiecetypes[0]=King;
			wpiecesquares[0]=i;
                  }
		  else
		  {
			wpiecetypes[wPieceIndex]=currentPieceType;
			wpiecesquares[wPieceIndex]=i;
			++wPieceIndex;
		  }
                }
		else if(isBlack(currentPiece))
		{
		  if(currentPieceType==King)
		  {
			bpiecetypes[0]=King;
			bpiecesquares[0]=i;
		  }
		  else
		  {
			bpiecetypes[bPieceIndex]=currentPieceType;
			bpiecesquares[bPieceIndex]=i;
			++bPieceIndex;
		  }
		}
	}
	for(int i=wPieceIndex; i<16; ++i) wpiecesquares[i]=InvalidSquare;
	for(int i=bPieceIndex; i<16; ++i) bpiecesquares[i]=InvalidSquare;
}

// Decoding of a move using a squareDiff array which indicates
// the difference of the end square and the start square in
// function of the movecode.
void Decoding::squareDiffDecode(const unsigned char& piecenumber,
				const unsigned char& movecode,
				const Color& color,
				Square& from,
				Square& to,
				const unsigned char* squareDiff)
{
	if (color == White) {
		from = wpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		wpiecesquares[piecenumber] = to;
	} else {
		from = bpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		bpiecesquares[piecenumber] = to;
	}
}

void Decoding::decodeKing(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to)
{
	static const int squareDiff[11] = {
		0, -9, -8, -7, -1, 1, 7, 8, 9, -2, 2
	};
	if (color == White) {
		from = wpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		wpiecesquares[piecenumber] = to;
		if (movecode == 9) { // long castling
			// Determine the piece on a1 (0). This passage can be speeded up
			// by different ways. For example giving to the rooks always the
			// same piecenumber if they exist. But this operation shouldn't be
			// bottleneck and therefore it is probably not worth to complicate
			// matters. This comment is also valid for code below handling the
			// other castling cases.
			int pn(getWpiecenumber(0));
			Q_ASSERT(pn >= 1 && pn <= 15);
			wpiecesquares[pn] = 3;
		} else if (movecode == 10) { // short castling
			int pn(getWpiecenumber(7));
			Q_ASSERT(pn >= 1 && pn <= 15);
			wpiecesquares[pn] = 5;
		}
	} else {
		from = bpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		bpiecesquares[piecenumber] = to;
		if (movecode == 9) { // long castling
			int pn(getBpiecenumber(56));
			Q_ASSERT(pn >= 1 && pn <= 15);
			bpiecesquares[pn] = 59;
		} else if (movecode == 10) { // short castling
			int pn(getBpiecenumber(63));
			Q_ASSERT(pn >= 1 && pn <= 15);
			bpiecesquares[pn] = 61;
		}
	}
}

void Decoding::decodeKnight(const unsigned char& piecenumber,
			    const unsigned char& movecode,
			    const Color& color,
			    Square& from,
			    Square& to)
{
	static const int squareDiff[] = {
		0, -17, -15, -10, -6, 6, 10, 15, 17
	};

	if (color == White) {
		from = wpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		wpiecesquares[piecenumber] = to;
	} else {
		from = bpiecesquares[piecenumber];
		to = from + squareDiff[movecode];
		bpiecesquares[piecenumber] = to;
	}
}

void Decoding::decodeRook(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to)
{
	Square* cpiecesquares;
	if (color == White) cpiecesquares = wpiecesquares;
	else cpiecesquares = bpiecesquares;

	from = cpiecesquares[piecenumber];

	int diff;
	if (movecode < 8) // move on the same rank
		{diff = movecode - (from % 8);}
	else // move on the same file
		{diff = 8 * ((movecode - 8) - from / 8);}
	to = from + diff;

	cpiecesquares[piecenumber] = to;
}

void Decoding::decodeBishop(const unsigned char& piecenumber,
			    const unsigned char& movecode,
			    const Color& color,
			    Square& from,
			    Square& to)
{
	Square* cpiecesquares;
	if (color == White) cpiecesquares = wpiecesquares;
	else cpiecesquares = bpiecesquares;

	from = cpiecesquares[piecenumber];

	int diffunit;
	if (movecode < 8) diffunit = 9;
	else diffunit = -7;
	int filediff((movecode % 8) - (from % 8));
	to = from + diffunit * filediff;

	cpiecesquares[piecenumber] = to;
}

void Decoding::decodeQueen(const unsigned char& piecenumber,
			   const unsigned char& movecode,
			   const Color& color,
			   Square& from,
			   Square& to,
			   QIODevice& qiod)
{
	Square* cpiecesquares;
	if (color == White) cpiecesquares = wpiecesquares;
	else cpiecesquares = bpiecesquares;

	from = cpiecesquares[piecenumber];

	if (movecode >= 8) // vertical move
		{to = from % 8 + 8 * (movecode - 8);}
	else if (movecode % 8 != from % 8) // horizontal move
		{to = 8 * (from / 8) + movecode;}
	else { // diagonal move
		char c;
		qiod.getChar(&c);
		to = (unsigned char)c;
	}

	cpiecesquares[piecenumber] = to;
}

// If the pawn move is a promotion, pt contains
// the corresponding piece type, otherwise pt is Pawn.
void Decoding::decodePawn(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to,
			  PieceType& pt)
{
	static const PieceType ptval[] = {Pawn, Pawn, Pawn,
					  Queen, Queen, Queen,
					  Rook, Rook, Rook,
					  Bishop, Bishop, Bishop,
					  Knight, Knight, Knight,
					  Pawn
					 };
	pt = ptval[movecode];
	if (color == White) {
		from = wpiecesquares[piecenumber];
		static const int wsquareDiff[] = {7, 8, 9, 7, 8, 9, 7, 8, 9, 7, 8, 9, 7, 8, 9, 16};
		to = from + wsquareDiff[movecode];
		wpiecesquares[piecenumber] = to;
		wpiecetypes[piecenumber] = pt;
	} else {
		from = bpiecesquares[piecenumber];
		static const int bsquareDiff[] = { -9, -8, -7, -9, -8, -7, -9, -8, -7, -9, -8, -7,
						   -9, -8, -7, -16
						 };
		to = from + bsquareDiff[movecode];
		bpiecesquares[piecenumber] = to;
		bpiecetypes[piecenumber] = pt;
	}
}

// Decode a move. pt is only used for pawn moves (to decode promotions)
// and qiod is only used for diagonal Queen moves. decodeMove
// supposes that the tuple (piecenumber,movecode,color) is valid
// (i.e. no impossible encoding or special tag).
void Decoding::decodeMove(const unsigned char& piecenumber,
			  const unsigned char& movecode,
			  const Color& color,
			  Square& from,
			  Square& to,
			  QIODevice& qiod,
			  PieceType& pt)
{
	PieceType mpt;
	if (color == White) mpt = wpiecetypes[piecenumber];
	else mpt = bpiecetypes[piecenumber];
	switch (mpt) {
	case King:
		decodeKing(piecenumber, movecode, color, from, to);
		break;
	case Queen:
		decodeQueen(piecenumber, movecode, color, from, to, qiod);
		break;
	case Rook:
		decodeRook(piecenumber, movecode, color, from, to);
		break;
	case Bishop:
		decodeBishop(piecenumber, movecode, color, from, to);
		break;
	case Knight:
		decodeKnight(piecenumber, movecode, color, from, to);
		break;
	case Pawn:
		decodePawn(piecenumber, movecode, color, from, to, pt);
		break;
	default:
		throw error();
	}
}

// The position of qiod must be just after a nag tag. The position
// of qiod after calling decodeNags will be just after the last
// byte corresponding to a nag encoding.
void Decoding::decodeNags(NagSet& ns, QIODevice& qiod) const
{
	ns.clear();
	char c;
	qiod.getChar(&c);

	ns.addNag(Nag((unsigned char)c));
	qiod.peek(&c, 1);
	while ((unsigned char)c == nag) {
		qiod.getChar(&c);
		qiod.getChar(&c);
		ns.addNag(Nag((unsigned char)c));
		qiod.peek(&c, 1);
	}
}

// Decodes a variation and subvariations beginnning at the current
// position of qiod and writing the variations in game. game is
// supposed to be in a state consitent with the variations to be
// encoded. qiod is supposed to be just after a beginVariation
// tag. The position of qiod after execution of decodeVariation
// will be just after the endVariation tag of the decoded
// Variation.
void Decoding::decodeVariation(Game& game, QIODevice& qiod)
{
	int variation;
	char c;
	unsigned char u;
	qiod.getChar(&c);
	u = c;
	NagSet ns;
	QString comment;
	unsigned char piecenumber, movecode;
	Square from, to;
	PieceType pt(None);
	int mid; // used for moveId for recursive calls
	Move m;
	while (true) {
		switch (u) {
		case nag:
			decodeNags(ns, qiod);
			break;
		case startComment:
			decodeComment(comment, qiod);
			break;
		case beginVariation:
			mid = game.currentMoveId();
			game.backward();

			{ // begin scope
				Decoding cxDec_sub(*this);
				// setting cxDec_sub to its state one move earlier
				if (game.board().toMove() == White) {
					cxDec_sub.wpiecesquares[piecenumber] = from;
					cxDec_sub.wpiecetypes[piecenumber] = pieceType(m.pieceMoved());
					if (m.isCastling()) {
						int rookpn = getWpiecenumber(m.castlingRookTo());
						cxDec_sub.wpiecesquares[rookpn] = m.castlingRookFrom();
					}
				} else {
					cxDec_sub.bpiecesquares[piecenumber] = from;
					cxDec_sub.bpiecetypes[piecenumber] = pieceType(m.pieceMoved());
					if (m.isCastling()) {
						int rookpn = getBpiecenumber(m.castlingRookTo());
						cxDec_sub.bpiecesquares[rookpn] = m.castlingRookFrom();
					}
				}

				cxDec_sub.decodeVariation(game, qiod);
			} // end scope

			// setting game to state before begin of subvariation
			game.moveToId(mid);
			break;
		case endVariation:
			return;
		default: // normal move
			piecenumber = u >> 4;
			movecode = u & 15;
			decodeMove(piecenumber, movecode,
				   game.board().toMove(), from, to, qiod, pt);
			m = game.board().prepareMove(from, to);

			// Set right promotion piece if move is pawn promotion.
			if (m.isPromotion()) m.setPromotionPiece(pt);

			variation = game.addMove(m, comment, ns);
			game.enterVariation(variation);
			ns.clear();
			comment = "";
			break;
		}
		qiod.getChar(&c);
		u = c;
	}
}

// game is supposed not to contain any moves (but may contain
// tags).
void Decoding::decodeNonHeader(Game& game, QIODevice& qiod)
{
	char c;
	unsigned char u;
	// Setting piece squares and types.
	if (game.tag(TagNames[TagFEN]) == QString()) { // standard start position

		// todo: exchange the following line to something more
		// efficient (beside other optimization it would perhaps
		// be interesting to control if game is already in the
		// standard start position).
		game.setStartBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP"
				   "/RNBQKBNR w KQkq - 0 1");

		setToStandardPieceTypes();
		setToStandardPieceSquares();
	} else { // non standard start position
		// FEN of header-file is read
		game.setStartBoard(game.tag(TagNames[TagFEN]));
                setPieceTypesAndSquares(game.board());
	}

	// Decode start annotation.
	qiod.peek(&c, 1);
	u = c;
	if (u == startComment) {
		QString qs;
		decodeComment(qs, qiod);
		game.setStartAnnotation(qs);
	}
	// Decode variations.
	qiod.getChar(&c);
	u = c;
	if (u != beginVariation) throw error();
	decodeVariation(game, qiod);
}

// --------------------------------------------------
// --------------------------------------------------
// |  Futher stuff in unnamed namespace for         |
// --------------------------------------------------
// --------------------------------------------------


Encoding cxEnc; //object that will be used for all encodings
Decoding cxDec; //object that will be used for all decodings

} //end of unnamed namespace for helper classes and functions



// --------------------------------------------------
// --------------------------------------------------
// |  Begin of definitions for class CxdMoves       |
// --------------------------------------------------
// --------------------------------------------------

const int CxdMoves::gameARecordSize = 8;

CxdMoves::CxdMoves()
:m_gameACFile(sizeof(qint64)), m_gameACBlock(m_gameACFile,0)
{
	m_isOpen = false;
}

CxdMoves::~CxdMoves()
{if(m_isOpen) close();}

bool CxdMoves::open(const SaxHandler& saxhandler)
{
	if (m_isOpen) return 0;
	m_gameFile.setFileName(saxhandler.m_gameFilename);
	if (!m_gameFile.exists()) return 0;
	m_gameFile.open(QIODevice::ReadWrite);
	if (!m_gameACFile.open(saxhandler.m_gameAccessFilename)) return 0;

	m_isOpen = 1;
	return 1;
}

void CxdMoves::close()
{
	if (!m_isOpen) return;
	m_gameFile.close();
	m_gameACFile.close();
	m_isOpen = false;
}

bool CxdMoves::create(const SaxHandler& saxhandler)
{
	if (m_isOpen) return 0;

	m_gameFile.setFileName(saxhandler.m_gameFilename);
	// Here we could warn if the files already exist
	m_gameFile.resize(0);
	m_gameFile.open(QIODevice::ReadWrite);
	if(!m_gameACFile.create(saxhandler.m_gameAccessFilename)) return 0;
	m_isOpen = 1;
	return 1;
}

void CxdMoves::compact(const QList<bool>& ql, const CxdAssign& cxda)
{
  int gameAi=0;
  qint64 nextPos=0;
  int firstid=-1;
  for(int i=0; i<=ql.size(); ++i)
  {
    if(firstid!=-1 && (i==ql.size() || !ql[i]))
    {
      copyInterval(firstid, i-1, nextPos, gameAi);
      gameAi+=i-firstid;
      nextPos=endPos(gameAi-1);
      firstid=-1;
    }
    else if(firstid==-1) firstid=i;
  }
  // resize files
  m_gameFile.resize(nextPos);
  m_gameACFile.qf()->resize(gameAi*m_gameACFile.recordsize());
}

bool CxdMoves::loadMoves(const int& index, Game& game)
{
	qint64 pos(startPos(index));
	m_gameFile.seek(pos);
	cxDec.decodeNonHeader(game, m_gameFile);
	return 1;
}


bool CxdMoves::appendGame(Game& game)
{
	qint64 pos = appendToGameFile(game);
	if (pos == -1) {
		// todo: depending on what went wrong, some cleanup operations
		// have probably to be implemented.
		return 0;
	}
	if (!appendToGameAFile(pos)) {
		// todo: the last appended game can now be deleted. But with
		// the current design this should not be necessary
		// for the functioning of the database but it is anyway
		// probably a good idea to implement.
		return 0;
	}
	return 1;
}

QString CxdMoves::gameFilename() const
{return m_gameFile.fileName();}

QString CxdMoves::gameAFilename() const
{return m_gameACFile.fileName();}

int CxdMoves::nb_games()
{return m_gameACFile.nb_entries();}

qint64 CxdMoves::startPos(const int& index)
{
	return m_gameACBlock.read(index);	
}

qint64 CxdMoves::endPos(const int& index)
{
	if(index==nb_games()-1) return m_gameFile.size();
	return startPos(index+1);	
}

qint64 CxdMoves::appendToGameFile(Game& game)
{
	qint64 pos(m_gameFile.size());
	m_gameFile.seek(pos);
	if (!cxEnc.encodeNonHeader(game, m_gameFile))
		{return -1;}
	m_gameFile.flush();
	return pos;
}

bool CxdMoves::appendToGameAFile(const qint64& pos)
{
        // Normally the appendEntries function of m_gameACFile should
        // first be called. But in this special case as only one CBlock
        // manages the file we can avoid the overhead.
        m_gameACBlock.write(nb_games(),pos);
	return 1;
}

void CxdMoves::writeToGameAFile(const int& index, const qint64& pos)
{
	m_gameACBlock.write(index,pos);
} 

void CxdMoves::copyMemInGameFile(const qint64& sourceStart, const qint64& sourceEnd,
				 const qint64& destStart)
{
	m_gameFile.seek(sourceStart);
	QByteArray qba(m_gameFile.read(sourceEnd-sourceStart));
	m_gameFile.seek(destStart);
	m_gameFile.write(qba);
}

void CxdMoves::copyInterval(const int& sourceFirst, const int& sourceLast,
			    const qint64& destPos, const int& destAPos)
{
  copyMemInGameFile(startPos(sourceFirst),endPos(sourceLast),destPos);
  qint64 delta=destPos-startPos(sourceFirst);
  for(int i=sourceFirst; i<=sourceLast; ++i)
  {
    writeToGameAFile(destAPos+i-sourceFirst,startPos(i)+delta);
  }
}



