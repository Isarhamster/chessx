/***************************************************************************
                          common.h  -  common chess code
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
		                       (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <QMap>
#include <QString>
#include <QStringList>

typedef unsigned char Square;
typedef unsigned char Coord;

const QString ChessXVersion = "0.3";
const uint ChessXVersionNumber = 00300;

enum Color {White, Black};
enum Piece {Empty, WhiteKing, WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight, WhitePawn,
  BlackKing, BlackQueen, BlackRook, BlackBishop, BlackKnight, BlackPawn,
  King = WhiteKing, Queen = WhiteQueen, Rook = WhiteRook, Bishop =WhiteBishop, Knight = WhiteKnight, Pawn = WhitePawn, InvalidPiece = 255};

const int ConstPieceTypes = BlackPawn + 1;
const Square InvalidSquare = 255;
const Square NoEPSquare = 255;

enum MoveType {StandardMove,
  PromotionWhiteQueen = WhiteQueen, PromotionWhiteRook = WhiteRook,
  PromotionWhiteBishop = WhiteBishop, PromotionWhiteKnight = WhiteKnight,
  PromotionBlackQueen = BlackQueen, PromotionBlackRook = BlackRook,
  PromotionBlackBishop = BlackBishop, PromotionBlackKnight = BlackKnight,
  Castling = 128, EnPassant, DoubleAdvance};
	
typedef int CastlingRights;
enum { NoRights = 0, WhiteKingside = 1, WhiteQueenside = 2, WhiteBothSides = 3, BlackKingside = 4, BlackQueenside = 8, BlackBothSides = 12, AllRights = 15 } ;

enum Result { Unknown, WhiteWin, Draw, BlackWin };

#define MIN(a,b) (a<b) ? a : b
#define MAX(a,b) (a>b) ? a : b

/** Converts a QChar to a Piece */
Piece charToPiece(const QChar& letter);
/** Converts a Piece to a QChar */
QChar pieceToChar(Piece piece);
/** Checkes wheter piece has given color. Note that there is no pieceColor
   function due to Empty piece value. */
bool isPieceColor(Piece piece, Color color);
/** Return opposite color */
Color oppositeColor(Color c);
/** Returns a string representation of a result */
QString resultString(Result result);

enum Tag {
   // mandatory tags
   TagEvent, TagSite, TagDate, TagRound, TagPlayerName,TagResult,
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
   ,TagLastTag
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

static QMap<Tag, Tag> BasicTagMap;
//PlayerTagMap[TagWhite] = PlayerName;

#endif

