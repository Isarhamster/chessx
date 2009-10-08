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

enum Result { Unknown, WhiteWin, Draw, BlackWin };

#define MIN(a,b) (a<b) ? a : b
#define MAX(a,b) (a>b) ? a : b


/** Checkes whether piece has given color. Note that there is no pieceColor
   function due to Empty piece value. */
bool isPieceColor(Piece piece, Color color);

/** Returns a string representation of a result */
QString resultString(Result result);

enum Tag
{
	// mandatory tags
	TagEvent, TagSite, TagDate, TagRound, TagPlayerName, TagResult,
	// the rest is optional tags
	// player
	TagPlayerTitle, TagPlayerElo, TagPlayerUSCF, TagPlayerNA, TagPlayerType,
	//event
	TagEventDate, TagEventSponsor, TagSection, TagStage, TagBoard,
	//opening
	TagOpening, TagVariation, TagSubvariation, TagECO, TagNIC,
	//time/date
	TagTime, TagUTCTime, TagUTCDate,
	//time control
	TagTimeControl,
	//starting position
	TagSetUp, TagFEN,
	//game conclusion
	TagTermination,
	//misc
	TagAnnotator, TagMode, TagPlyCount,
	TagSource,
	// And now we add player specific tags. These tags will map to
	// one of the previous ones for storage. TagWhite must always
	// be the first of these, and TagLastTag, last.
	TagWhite, TagBlack, TagWhiteTitle, TagBlackTitle,
	TagWhiteElo, TagBlackElo, TagWhiteUSCF, TagBlackUSCF,
	TagWhiteNA, TagBlackNA, TagWhiteType, TagBlackType
	// The last tag, to know where the list ends
	// Allow to iterate over tags.
	, TagLastTag
};
// Please ensure that the order and number of these names are the
// same as that of the Tag enum.
static QStringList TagNames  = (QStringList() << "Event" << "Site"
				<<  "Date" <<  "Round" <<  "Name" << "Result" << "Title"
				<<  "Elo" <<  "USCF" <<  "NA" <<  "Type" <<  "EventDate"
				<<  "EventSponsor" <<  "Section" <<  "Stage" <<  "Board"
				<< "Opening" <<  "Variation" <<  "Subvariation" <<  "ECO"
				<<  "NIC" <<  "Time" <<  "UTCTime" <<  "UTCDate" <<  "TimeControl"
				<<  "SetUp" <<  "FEN" <<  "Termination" << "Annotator" <<  "Mode"
				<<  "PlyCount" <<  "Source"
				<< "White" << "Black" << "WhiteTitle" << "BlackTitle"
				<< "WhiteElo" << "BlackElo" << "WhiteUSCF" << "BlackUSCF"
				<< "WhiteNA" << "BlackNA" << "WhiteType" << "BlackType");

extern QMap<Tag, Tag> BasicTagMap;
//PlayerTagMap[TagWhite] = PlayerName;

#endif

