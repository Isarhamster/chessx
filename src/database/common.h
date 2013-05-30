/***************************************************************************
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           * 
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <QtDebug>
#include <QMap>
#include <QString>
#include <QStringList>

typedef unsigned char Square;
typedef unsigned char Coord;

enum Piece
{
	Empty,
	WhiteKing, WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight, WhitePawn,
	BlackKing, BlackQueen, BlackRook, BlackBishop, BlackKnight, BlackPawn,
	InvalidPiece = 255
};

// Pre-Increment
Piece& operator++(Piece& w);

// Post-Increment
Piece operator++(Piece& w, int);

const int ConstPieceTypes = BlackPawn + 1;
enum PieceType { None, King, Queen, Rook, Bishop, Knight, Pawn };
enum Color {White, Black};

/** Return opposite color */
inline Color oppositeColor(const Color c) {return c == White ? Black : White;}

/** Return true if given piece is White */
inline bool isWhite(const Piece p) {return p >= WhiteKing && p <= WhitePawn;}

/** Return true if given piece is Black */
inline bool isBlack(const Piece p) {return p >= BlackKing && p <= BlackPawn;}

/** Return PieceType of given Piece */
inline PieceType pieceType(const Piece p)
	{return PieceType(isWhite(p) ? p : (isBlack(p) ? p-6 : 0));}

/** Return Color of given Piece */
inline Color pieceColor(const Piece p) {return isBlack(p) ? Black : White;}

/** Verify that given Piece is within range */
inline bool isValidPiece(const Piece p) {return isWhite(p) || isBlack(p);}

enum SquareNames
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

const char strSquareNames[64][3] =
{
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const Square InvalidSquare = 255;
const Square NoEPSquare = 255;

enum MoveType
{
	StandardMove,
	PromotionQueen = Queen, PromotionRook = Rook,
	PromotionBishop = Bishop, PromotionKnight = Knight,
	Castling = 128, EnPassant, DoubleAdvance, Promotion
};

typedef uchar CastlingRights;
enum
{
	NoRights = 0,
	WhiteKingside = 1, BlackKingside = 2,
	WhiteQueenside = 4, BlackQueenside = 8,
	WhiteBothSides = 5, BlackBothSides = 10,
	AllRights = 15
};

enum Result { ResultUnknown, WhiteWin, Draw, BlackWin };

#define MIN(a,b) (a<b) ? a : b
#define MAX(a,b) (a>b) ? a : b


/** Checkes whether piece has given color. Note that there is no pieceColor
   function due to Empty piece value. */
bool isPieceColor(Piece piece, Color color);

/** Returns a string representation of a result */
QString resultString(Result result);

static const QString TagNameFEN      = "FEN";
static const QString TagNameECO      = "ECO";
static const QString TagNameSetUp    = "Setup";
static const QString TagNameSource   = "Source";
static const QString TagNameResult   = "Result";
static const QString TagNameWhite    = "White";
static const QString TagNameBlack    = "Black";
static const QString TagNameEvent    = "Event";
static const QString TagNameSite     = "Site";
static const QString TagNameRound    = "Round";
static const QString TagNameDate     = "Date";
static const QString TagNamePlyCount = "PlyCount";
static const QString TagNameWhiteClock = "WhiteClock";
static const QString TagNameBlackClock = "BlackClock";
static const QString TagNameWhiteElo = "WhiteElo";
static const QString TagNameBlackElo = "BlackElo";
static const QString TagNameWhiteTeam = "WhiteTeam";
static const QString TagNameBlackTeam = "BlackTeam";

const QString StandardTags[7] = {"Event", "Site", "Date", "Round", "White", "Black", "Result"};

typedef quint32 TagIndex;
typedef quint32 ValueIndex;
typedef quint32 GameId;

#define TagNoIndex 0xFFFFFFFF

#endif

