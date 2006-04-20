/***************************************************************************
                          board.h - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>
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

#ifndef __BOARD_H__
#define __BOARD_H__

#include "move.h"
#include "history.h"
#include "movelist.h"
#include "common.h"

#include <qstring.h>
#include <qt34/qptrlist.h>

#define MAX_PIECES 12
#define MAX_SQUARES 64
#define MAX_EN_PASSANT_SQUARES 16


enum BoardState {Valid, NoWhiteKing, NoBlackKing, TooManyWhitePieces,
    TooManyBlackPieces, TooManyWhitePawns, TooManyBlackPawns, 
    TooManyWhiteKings, TooManyBlackKings, IncorrectCheck};

/**
   The Board class represents a chess position.
   Pieces are kept directly on the board which is internally represented as simple array[0..63].
   You can easily and quickly make and undo moves, although undoing moves requires keeping track
   of captured pieces.
*/


class Board
{
public:
  /** Empty constructor. Creates empty board with no pieces. */
  Board();
  /** Resets board, remove all pieces */
  void clear();
  /** Resets board, restoring startup position */
  void setStandardPosition();
  /** Sets position from FEN */
  void fromFEN(const QString& fen);
	/** @return FEN string for position */
	QString toFEN() const;
   /** @return true is the FEN string is valid */
   bool isValidFEN(const QString& fen) const;
	/** @return ASCII representation for debugging */
	QString toASCII();
	
  /* information about pieces position and types */
  /** @return piece on given square */
  Piece at(Square s) const;
  /** @return piece on given square (defined by x,y from 0-7 range) */
  Piece at(Coord x, Coord y) const;
  /** @return piece at given @p index (0..31) */
  Piece atIndex(int i) const;
  /** @return position of piece given by @p index (0..31) */
  Square piecePosition(int index) const;

  /** @return number of all pieces of given type. */
  int pieceCount(Piece p) const;

  /** @return position of king */
  Square kingPosition(Color c) const;

  /** @return side to move */
  Color toMove() const;
   /** Sets side to move - mostly useful when setting position */
  void setToMove(Color c); 
  /** Swaps side to move */
  void swapToMove();
  /** Sets a piece on the given square **/
  bool setAt(Square s, Piece p);
  /** Remove a piece from a given square **/
  void removeFrom(Square s);
  /** @return whether current position is valid. If not, error code is set to @p
      state variable, if it was provided */
  bool isValid(BoardState* state=0);
  bool findLegalMoves();
  QString getLegalMoves();


	/* move information methods */
	/** @return move object represented by the given short algerbraic notation */
	Move singleMove(const QString& SAN);
	/** @return string representing the given move in short algebraic notations */
	QString moveToSAN(const Move& move);
	/** @return move object represented by the given long algerbraic notation */
	Move singleLANMove(QString& LAN);
	/** @return whether is particular sqaure is attacked */
  bool isAttacked(Square sq,Color c) const;
	/** @return whether a given move is legal in the current position */
  bool isLegal(const Move& m);
	
	/* move / undo move methods */
  /** Make standard move. */
  HistoryItem doMove(const Move& m);
  /** Undo standard move (no captures). */
  void undoMove(const Move& m, const HistoryItem& historyItem);
	
	/* board state methods */
	/** @return whether the side to move is in check */
	bool isCheck();
	/** @return whether the position is a checkmate */
	bool isCheckmate();
	/** @return whether the position is a stalemate */
   bool isStalemate();
   bool canWhiteKingSideCastle() const;
   bool canWhiteQueenSideCastle() const;
   bool canBlackKingSideCastle() const;
   bool canBlackQueenSideCastle() const; 
   Square enPassantSquare() const; 
   Q_UINT64 getHashValue();
   Q_UINT64 getHashValue2();
   bool operator == (Board b) {
      return ((m_hashValue == b.getHashValue()) && 
             (m_hashValue2 == b.getHashValue2())); 
   }
   bool operator != (Board b) {
      return ((m_hashValue != b.getHashValue()) ||
              (m_hashValue2 != b.getHashValue2())); 
   }
   // *** This function is purely for debug purposes
   // *** When comparing 2 boards, it's usefull to have a name for each
   void setDebugName(QString debugName);

private:
  /** Move piece from @p from to @p to, leaving source square empty */
  void movePiece(Square from, Square to);
  /** Restore captured piece */
  void restorePiece(Square from, Piece piece, int index);
  /** Promotes a piece / changes its type*/
  void promotePiece(Square square, Piece promoted);

  QString m_debugName;
  unsigned char m_board[64];
  Piece m_pieceType[32];
  Square m_piecePosition[32];
  int m_pieceCount[ConstPieceTypes];
	
   Color m_toMove;
	Square m_epSquare;
	CastlingRights m_castlingRights;
	int m_halfMoveClock;
   QPtrList<Move> legalMoveList;

   Q_UINT64 m_hashValue;
   Q_UINT64 m_randomValues[MAX_PIECES][MAX_SQUARES];
   Q_UINT64 m_whiteCastlingKS;
   Q_UINT64 m_whiteCastlingQS;
   Q_UINT64 m_blackCastlingKS;
   Q_UINT64 m_blackCastlingQS;
   Q_UINT64 m_enPassant[MAX_EN_PASSANT_SQUARES];
   Q_UINT64 m_randToMove;
   Q_UINT64 m_hashValue2;
   Q_UINT64 m_randomValues2[MAX_PIECES][MAX_SQUARES];
   Q_UINT64 m_whiteCastlingKS2;
   Q_UINT64 m_whiteCastlingQS2;
   Q_UINT64 m_blackCastlingKS2;
   Q_UINT64 m_blackCastlingQS2;
   Q_UINT64 m_enPassant2[MAX_EN_PASSANT_SQUARES];
   Q_UINT64 m_randToMove2;
   void readRandomValues();
//public: //temporary public to help with debugging
   void createHash();
   void hashPiece(Square s, Piece p);
   void hashToMove();
   void hashWhiteKingSideCastle();
   void hashWhiteQueenSideCastle();
   void hashBlackKingSideCastle();
   void hashBlackQueenSideCastle();
   void hashEpSquare();
   void hashCastlingRights(CastlingRights oldCastlingRights);

};
#endif

