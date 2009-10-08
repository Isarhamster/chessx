/***************************************************************************
 *   (C) 2003 Sune Fischer                                                 *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include "movelist.h"

enum BoardStatus
{
	Valid, NoWhiteKing, NoBlackKing, DoubleCheck,
	OppositeCheck, TooManyBlackPawns, TooManyWhitePawns,
	PawnsOn18, TooManyKings, TooManyWhite, TooManyBlack,
	BadCastlingRights, InvalidEnPassant, MultiCheck
};

/** @ingroup Core
 * Keep track of the pieces on a chessboard, provide a Move() factory.
 */
class BitBoard
{
public:
	BitBoard();

	// Play moves on board
	//
	/** Play given move, updating board state appropriately */
	bool doMove(const Move&);
	/** Return board to state prior to given move */
	void undoMove(const Move&);

	// Setup board
	//
	/** Remove all pieces and state from board */
	void clear();
	/** Set move number in game */
	void setMoveNumber(uint moveNumber);
	/** Set initial chess game position on the board */
	void setStandardPosition();
	/** Set the given piece on the board at the given square */
	void setAt(const Square s, const Piece p);
	/** Remove any piece sitting on given square */
	void removeAt(const Square s);
	/** Set side to move as that of given color */
	void setToMove(const Color& c);
	/** Swap the side to move */
	void swapToMove();
	/** Parse given FEN, return true if loaded properly otherwise false */
	bool fromFen(const QString& fen);
	/** Set En Passant Square */
	void setEnPassantSquare(const Square s);
	/** Remove En Passant privilege */
	void clearEnPassantSquare();

	// Move factories
	//
	/** parse SAN or LAN representation of move, and return proper Move() object */
	Move parseMove(const QString& algebraic) const;
	/** Return a proper Move() object given only a from-to move specification */
	Move prepareMove(const Square& from, const Square& to) const;

	// Query
	//
	/** Is piece sitting on given square moveable ? */
	bool isMovable(const Square s) const;
	/** @return piece sitting at given square on the board */
	Piece pieceAt(Square s) const;
	/** @return number of ply since a pawn move or capture */
	uint halfMoveClock() const;
	/** Set number of ply since a pawn move or capture */
	void setHalfMoveClock(uint i);
	/** @return the current move number in the game */
	uint moveNumber() const;
	/** @return color of side next to move */
	Color toMove() const;
	/** @return true if its possible for this position to follow target position */
	bool canBeReachedFrom(const BitBoard& target) const;
	/** @return true if position is same, but don't consider Move # in determination */
	bool positionIsSame(const BitBoard& target) const;

	// Query other formats
	//
	/** Return a FEN string based on current board position */
	QString toFen() const;
	/** Return a SAN string representation of given move */
	QString moveToSan(const Move& move) const;

	// Validation
	//
	/** Check current position and return "Valid" or problem */
	BoardStatus validate() const;
	/** Return true if given FEN can be parsed */
	bool isValidFen(const QString& fen) const;
	/** validate that move is valid in position, only used for debugging */
	bool debugCheckMove(const Move& m) const;

	/** Return true if fastbits option was compiled in */
	static const bool fastbitsOption;

	/** Set castling rights. */
	void setCastlingRights(CastlingRights cr);
	/** Return the internal castling rights data (used by hash function) */
	CastlingRights castlingRights() const;
	/** Return square where En passant capture may occur, or "NoEPSquare" */
	Square enPassantSquare() const;

private:
	/** Return true if side to move is in check */
	bool isCheck() const;
	/** Return true if the side to move is in checkmate */
	bool isCheckmate() const;
	/** Return true if the side to move is stalemated */
	bool isStalemate() const;
	/** Test to see if given color has the right to castle on kingside */
	bool canCastleShort(const uint c) const;
	/** Test to see if given color has the right to castle on queenside */
	bool canCastleLong(const uint c) const;
	/** Test to see if given color has any castling rights remaining */
	bool canCastle(const uint c) const;

	/** Return true if making move would put oneself into check */
	bool isIntoCheck(const Move& move) const;
	/** Return true if the given square is attacked by the given color */
	bool isAttackedBy(const uint color, const uint square) const;

	/** Return all squares attacked by a knight on given square */
	quint64 knightAttacksFrom(const Square s) const;
	/** Return all squares attacked by a bishop on given square */
	quint64 bishopAttacksFrom(const Square s) const;
	/** Return all squares attacked by a rook on given square */
	quint64 rookAttacksFrom(const Square s) const;
	/** Return all squares attacked by a queen on given square */
	quint64 queenAttacksFrom(const Square s) const;
	/** Return all squares attacked by a king on given square */
	quint64 kingAttacksFrom(const Square s) const;
	/** Return all possible pawn moves from given square */
	quint64 pawnMovesFrom(const Square s) const;

	/** Remove impossible moves from given bitboard to aid disambiguation */
	void removeIllegal(const Move& move, quint64& b) const;
	/** Update move with castling details, return false if no castle is possible */
	bool prepareCastle(Move& move) const;
	/** Generate all possible moves in a given position */
	MoveList generateMoves() const;

	/** Grant castling rights on the kingside to the given color */
	void setCastleShort(uint color);
	/** Grant castling rights on the queenside to the given color */
	void setCastleLong(uint color);
	/** Revoke all castling rights from the given color */
	void destroyCastle(uint color);
	/** Update the epSquare value based on a new epFile value */
	void epFile2Square();

	/** Setup board according to FEN string */
	bool fromGoodFen(const QString& fen);


	// Actual Bit-board data
	quint64 m_pawns, m_knights, m_bishops, m_rooks, m_queens, m_kings;
	quint64 m_occupied_co[2];     // Square mask of those occupied by each color
	quint64 m_occupied;           // Square is empty or holds a piece
	quint64 m_occupied_l90;       // rotated counter clockwise 90 deg
	quint64 m_occupied_l45;       // an odd transformation, to straighten out diagonals
	quint64 m_occupied_r45;       // the opposite odd transformation, just as messy

	// Extra state data
	uchar m_piece[64];             // type of piece on this square
	uchar m_stm;                   // side to move
	uchar m_ksq[2];                // square of the m_kings
	uchar m_epFile;                // file of a possible ep capture
	uchar m_epSquare;              // This is requested by hash routine enough that we keep it pre calculated
	uchar m_castle;                // flags for castle legality  (these can be merged)
	ushort m_halfMoves;            // Number of moves since last pawn move or capture
	uint m_moveNumber;             // Move number in game (incremented after each black move)
	uchar m_pawnCount[2];           // Number of pawns for each side
	uchar m_pieceCount[2];           // Number of pieces for each side
};

extern quint64 bb_PawnAttacks[2][64];
extern quint64 bb_KnightAttacks[64];
extern quint64 bb_R45Attacks[64][64];
extern quint64 bb_L45Attacks[64][64];
extern quint64 bb_KingAttacks[64];
extern quint64 bb_RankAttacks[64][64];
extern quint64 bb_FileAttacks[64][64];

const uint bb_ShiftR45[64] = {
	1, 58, 51, 44, 37, 30, 23, 16,
	9, 1, 58, 51, 44, 37, 30, 23,
	17, 9, 1, 58, 51, 44, 37, 30,
	25, 17, 9, 1, 58, 51, 44, 37,
	33, 25, 17, 9, 1, 58, 51, 44,
	41, 33, 25, 17, 9, 1, 58, 51,
	49, 41, 33, 25, 17, 9, 1, 58,
	57, 49, 41, 33, 25, 17, 9, 1
};

const uint bb_ShiftL45[64] = {
	9, 17, 25, 33, 41, 49, 57, 1,
	17, 25, 33, 41, 49, 57, 1, 10,
	25, 33, 41, 49, 57, 1, 10, 19,
	33, 41, 49, 57, 1, 10, 19, 28,
	41, 49, 57, 1, 10, 19, 28, 37,
	49, 57, 1, 10, 19, 28, 37, 46,
	57, 1, 10, 19, 28, 37, 46, 55,
	1, 10, 19, 28, 37, 46, 55, 64
};

inline bool BitBoard::isAttackedBy(const uint color, const uint square) const
{
	if (bb_PawnAttacks[color^1][square] & (m_pawns | m_bishops) & m_occupied_co[color])
		return 1;
	if (knightAttacksFrom(square) & m_knights & m_occupied_co[color])
		return 1;
	if (bishopAttacksFrom(square) & (m_bishops | m_queens) & m_occupied_co[color])
		return 1;
	if (rookAttacksFrom(square) & (m_rooks|m_queens) & m_occupied_co[color])
		return 1;
	if (kingAttacksFrom(square) & (m_kings | m_queens) & m_occupied_co[color])
		return 1;
	return 0;
};

inline void BitBoard::setCastleShort(uint color)
{
	m_castle |= 1 << color;
}

inline void BitBoard::setCastleLong(uint color)
{
	m_castle |= 4 << color;
}

inline void BitBoard::destroyCastle(uint color)
{
	m_castle &= ~(5 << color);
}

inline quint64 BitBoard::knightAttacksFrom(const Square s) const
{
	return bb_KnightAttacks[s];
}

inline quint64 BitBoard::bishopAttacksFrom(const Square s) const
{
	return bb_R45Attacks[s][(m_occupied_r45 >> bb_ShiftR45[s]) & 63] |
		bb_L45Attacks[s][(m_occupied_l45 >> bb_ShiftL45[s]) & 63];
}

inline quint64 BitBoard::rookAttacksFrom(const Square s) const
{
	return bb_RankAttacks[s][(m_occupied >> ((s & ~7) + 1)) & 63] |
		 bb_FileAttacks[s][(m_occupied_l90 >> (((s & 7) << 3) + 1)) & 63];
}

inline quint64 BitBoard::queenAttacksFrom(const Square s) const
{
	return rookAttacksFrom(s) | bishopAttacksFrom(s);
}

inline quint64 BitBoard::kingAttacksFrom(const Square s)  const
{
	return bb_KingAttacks[s];
}

inline void BitBoard::epFile2Square()
{
	if (m_epFile)
		m_epSquare = m_epFile + (m_stm == White ? a6:a3) - 1;
	else	m_epSquare = NoEPSquare;
}

inline bool BitBoard::canCastle(const uint c) const
{
	return m_castle & (5 << c);
}

inline bool BitBoard::canCastleShort(const uint c) const
{
	return m_castle & (1 << c);
}

inline bool BitBoard::canCastleLong(const uint c)  const
{
	return m_castle&(4 << c);
}

inline bool BitBoard::isCheck() const
{
	return isAttackedBy(m_stm^1, m_ksq[m_stm]);
}

inline uint BitBoard::halfMoveClock() const
{
	return m_halfMoves;
}

inline void BitBoard::setHalfMoveClock(uint i)
{
	m_halfMoves = i;
}

inline uint BitBoard::moveNumber() const
{
	return m_moveNumber;
}

inline Color BitBoard::toMove() const
{
	return Color(m_stm);
}

inline Square BitBoard::enPassantSquare() const
{
	return m_epSquare;
}

inline CastlingRights BitBoard::castlingRights() const
{
	return m_castle;
}

inline void BitBoard::setCastlingRights(CastlingRights cr)
{
	m_castle = cr;
}

/** Set the side to move to the given color */
inline void BitBoard::setToMove(const Color& c)
{
	m_stm = c;
}

inline void BitBoard::swapToMove()
{
	m_stm ^= 1;
}

inline bool BitBoard::debugCheckMove(const Move& m) const
{
	return BitBoard(*this).prepareMove(m.from(), m.to()).isLegal();
}

inline void BitBoard::setMoveNumber(uint moveNumber)
{
	m_moveNumber = moveNumber;
}

inline bool BitBoard::positionIsSame(const BitBoard& target) const
{
	if (	m_occupied_co[White] != target.m_occupied_co[White] ||
		m_occupied_co[Black] != target.m_occupied_co[Black] ||
		m_pawns != target.m_pawns ||
		m_knights != target.m_knights ||
		m_bishops != target.m_bishops ||
		m_rooks != target.m_rooks ||
		m_queens != target.m_queens ||
		m_kings != target.m_kings ||
		m_stm != target.m_stm)
			return false;
	return true;
}

#endif // __BITBOARD_H__
