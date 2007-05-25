#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include "movelist.h"

enum BoardStatus
{
	Valid, NoWhiteKing, NoBlackKing, DoubleCheck,
	OppositeCheck, TooManyBlackPawns, TooManyWhitePawns,
	PawnsOn18, TooManyKings, TooManyWhite, TooManyBlack,
	BadCastlingRights, InvalidEnPassant
};

/** @ingroup Core
 * Keep track of the pieces on a chessboard, provide a Move() factory.
 */
class BitBoard
{
public:
	BitBoard();

	// Play moves
	bool doMove(const Move&);
	void undoMove(const Move&);

	// Setup board
	void clear();
	void setStandardPosition();
	void setAt(const Square s, const Piece p);
	void removeAt(const Square s);
	void setToMove(const Color& c);
	void swapToMove();
	int fromFen(const QString& fen);

	// Move factories
	Move parseMove(const QString& algebraic) const;
	Move prepareMove(const Square& from, const Square& to) const;

	// Query
	bool isMovable(const Square s) const;
	bool canCastle(const uint c) const;
	bool canCastleShort(const uint c) const;
	bool canCastleLong(const uint c) const;
	bool isSquareEmpty(const Square s) const;
	bool isCheck() const;
	Piece pieceAt(Square s) const;
	uint halfMoveClock() const;
	Color toMove() const;
	Square enPassantSquare() const;
	CastlingRights castlingRights() const;

	// Query other formats
	QString toFen(int move=0) const;
	QString moveToSan(const Move& move) const;

	// Validation
	BoardStatus validate() const;
	bool isValidFen(const QString& fen) const;

private:
	bool isCheckmate() const;
	bool isStalemate() const;
	bool isIntoCheck(const Move& move) const;
	bool isAttacked(const uint sq) const;
	bool blackOnSquare(const Square& sq) const;

	int fromGoodFen(const QString& fen);
	MoveList  GenMoves() const;

	void SetCastleShort(uint c);
	void SetCastleLong(uint c);
	void DestroyCastle(uint c);
	void removeIllegal(quint64& b, const Move& move) const;
	void fixupBoards();

	quint64 whiteBB() const;
	quint64 blackBB() const;
	quint64 Occupied(uint co) const;
	quint64 KnightAttacksFrom(const int s) const;
	quint64 BishopAttacksFrom(const int s) const;
	quint64 RookAttacksFrom(const int s) const;
	quint64 QueenAttacksFrom(const int s) const;
	quint64 KingAttacksFrom(const int s) const;
	QChar pieceToChar(const Piece piece) const;

	/* only used once: ??? */
	quint64 AttackL45(const int s, const quint64 o) const;
	quint64 AttackR45(const int s, const quint64 o) const;
	quint64 AttackFile(const int s, const quint64 o) const;
	quint64 AttackRank(const int s, const quint64 o) const;

	uint File(const uint sq) const;
	uint Rank(const uint sq) const;
	quint64 SetBit(const uint sq) const;

	/* Update move with castling details, return false if no castle possible */
	bool prepareCastle(Move& move) const;
	void epFile2Square();
	bool IsAttackedBy(const uint co, const uint sq) const;
	quint64 pawnMovesFrom(const Square s) const;

	quint64 m_pawns, knights, bishops, rooks, queens, kings;
	quint64 occupied_co[2];
	quint64 occupied;
	quint64 occupied_l90;       // rotated counter clockwise 90 deg
	quint64 occupied_l45;       // an odd transformation, to straighten out diagonals
	quint64 occupied_r45;       // the opposite odd transformation, just as messy

	uchar piece[64];             // type of piece on this square
	uchar stm;                   // side to move
	uchar ksq[2];                // square of the kings
	uchar epfile;                // file of a possible ep capture
	uchar epSquare;              // This is requested by hash routine enough that we keep it pre calculated
	uchar castle;                // flags for castle legality  (these can be merged)

	ushort m_halfMoves;
};

extern quint64 PawnBB[2][64];
extern quint64 KnightBB[64];
extern quint64 BishopR45[64][64];
extern quint64 BishopL45[64][64];
extern quint64 KingBB[64];
extern quint64 RookL00[64][64];
extern quint64 RookL90[64][64];
extern quint64 files[8];
extern quint64 ranks[8];
extern quint64 Mask[64];
extern quint64 MaskL90[64];
extern quint64 MaskL45[64];
extern quint64 MaskR45[64];

const uint ShiftR45[64] = {
	1, 58, 51, 44, 37, 30, 23, 16,
	9, 1, 58, 51, 44, 37, 30, 23,
	17, 9, 1, 58, 51, 44, 37, 30,
	25, 17, 9, 1, 58, 51, 44, 37,
	33, 25, 17, 9, 1, 58, 51, 44,
	41, 33, 25, 17, 9, 1, 58, 51,
	49, 41, 33, 25, 17, 9, 1, 58,
	57, 49, 41, 33, 25, 17, 9, 1
};

const uint ShiftL45[64] = {
	9, 17, 25, 33, 41, 49, 57, 1,
	17, 25, 33, 41, 49, 57, 1, 10,
	25, 33, 41, 49, 57, 1, 10, 19,
	33, 41, 49, 57, 1, 10, 19, 28,
	41, 49, 57, 1, 10, 19, 28, 37,
	49, 57, 1, 10, 19, 28, 37, 46,
	57, 1, 10, 19, 28, 37, 46, 55,
	1, 10, 19, 28, 37, 46, 55, 64
};

inline bool BitBoard::IsAttackedBy(const uint co, const uint sq) const
{
	if (KingBB[sq] & (kings | queens) & Occupied(co))
		return 1;
	if (PawnBB[co^1][sq] & (m_pawns | bishops) &Occupied(co))
		return 1;
	if (KnightBB[sq] & knights & Occupied(co))
		return 1;
	if (RookAttacksFrom(sq) & (rooks | queens) & Occupied(co))
		return 1;
	if (BishopAttacksFrom(sq) & (bishops | queens) & Occupied(co))
		return 1;
	return 0;
};

inline void BitBoard::SetCastleShort(uint c)
{
	castle |= 1 << c;
}

inline void BitBoard::SetCastleLong(uint c)
{
	castle |= 4 << c;
}

inline void BitBoard::DestroyCastle(uint c)
{
	castle &= ~(5 << c);
}

inline quint64 BitBoard::whiteBB() const
{
	return occupied_co[White];
}

inline quint64 BitBoard::blackBB() const
{
	return occupied_co[Black];
}

inline quint64 BitBoard::Occupied(uint co) const
{
	return occupied_co[co];
}

inline quint64 BitBoard::KnightAttacksFrom(const int s) const
{
	return KnightBB[s];
}

inline quint64 BitBoard::AttackL45(const int s,const quint64 o) const
{
	return BishopL45[s][(o >> ShiftL45[s]) & 63];
}

inline quint64 BitBoard::AttackR45(const int s, const quint64 o) const
{
	return BishopR45[s][(o >> ShiftR45[s]) & 63];
}

inline quint64 BitBoard::BishopAttacksFrom(const int s) const
{
	return AttackR45(s,occupied_r45) | AttackL45(s,occupied_l45);
}

inline quint64 BitBoard::AttackFile(const int s, const quint64 o) const
{
	return RookL90[s][(o >> (((s & 7) << 3) + 1)) & 63];
}

inline quint64 BitBoard::AttackRank(const int s, const quint64 o) const
{
	return RookL00[s][(o >> ((s & ~7) + 1)) & 63];
}

inline quint64 BitBoard::RookAttacksFrom(const int s) const
{
	return AttackRank(s,occupied) | AttackFile(s,occupied_l90);
}

inline quint64 BitBoard::QueenAttacksFrom(const int s) const
{
	return RookAttacksFrom(s) | BishopAttacksFrom(s);
}

inline quint64 BitBoard::KingAttacksFrom(const int s)  const
{
	return KingBB[s];
}

inline bool BitBoard::blackOnSquare(const Square& sq) const
{
	return occupied_co[Black] & SetBit(sq);
}

inline QChar BitBoard::pieceToChar(const Piece piece) const
{
	return piece > BlackPawn ? '?' : " KQRBNPkqrbnp"[piece];
};

inline void BitBoard::epFile2Square()
{
	if (epfile)
		epSquare = epfile + (stm == White ? a6:a3) - 1;
	else	epSquare = NoEPSquare;
}

inline bool BitBoard::isAttacked(const uint sq) const
{
	return IsAttackedBy(stm ^ 1, sq);
};

inline uint BitBoard::File(const uint sq) const
{
	return sq & 7;
}

inline uint BitBoard::Rank(const uint sq) const
{
	return sq >> 3;
}

inline quint64 BitBoard::SetBit(const uint sq) const
{
	return Mask[sq];
}

/** Test to see if given color has any castling rights remaining */
inline bool BitBoard::canCastle(const uint c) const
{
	return castle & (5 << c);
}

inline bool BitBoard::canCastleShort(const uint c) const
{
	return castle & (1 << c);
}

inline bool BitBoard::canCastleLong(const uint c)  const
{
	return castle&(4 << c);
}

inline bool BitBoard::isSquareEmpty(const Square s) const
{
	return !(occupied & SetBit(s));
}

inline bool BitBoard::isCheck() const
{
	return IsAttackedBy(stm^1, ksq[stm]);
}

inline uint BitBoard::halfMoveClock() const
{
	return m_halfMoves;
}

inline Color BitBoard::toMove() const
{
	return Color(stm);
}

inline Square BitBoard::enPassantSquare() const
{
	return epSquare;
}

inline CastlingRights BitBoard::castlingRights() const
{
	return castle;
}

inline void BitBoard::setToMove(const Color& c)
{
	stm = c;
}

inline void BitBoard::swapToMove()
{
	stm ^= 1;
}

#endif // __BITBOARD_H__
