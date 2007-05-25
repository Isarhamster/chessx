/*==========================================================================

	A Compact Rotated BitBoard Move Generator - for chess

	Copyright (c) 2003, Sune Fischer.  All rights reserved.

-------------------------------------------------------------------------

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

===========================================================================*/
/***************************************************************************
                   bitboard.cpp - bitboard based position
                   --------------------------------------
  additional   : sob maj 7 2005
  copyrights   : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                 (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                 (C) 2005, 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                 (C) 2005 Marius Roets <roets.marius@gmail.com>
 ***************************************************************************/

#include "movelist.h"
#include "bitboard.h"

// Global data that is initialized early on and only read afterward
static bool BitBoardInitRun;
quint64 PawnBB[2][64];       // 8* 2*64 =  1024
quint64 PawnF1[2][64];       // 8* 2*64 =  1024
quint64 PawnF2[2][64];       // 8* 2*64 =  1024
quint64 PawnALL[2][64];      // 8* 2*64 =  1024
quint64 PawnPM[2];           //    2*64 =   128
quint64 KnightBB[64];        // 8*64    =   512
quint64 BishopR45[64][64];   // 8*64*64 = 32768
quint64 BishopL45[64][64];   // 8*64*64 = 32768
quint64 KingBB[64];          // 8*64    =   512
quint64 RookL00[64][64];     // 8*64*64 = 32768
quint64 RookL90[64][64];     // 8*64*64 = 32768
quint64 files[8];
quint64 ranks[8];
quint64 Mask[64];
quint64 MaskL90[64];
quint64 MaskL45[64];
quint64 MaskR45[64];         // 8* 4*64 =  2048  => 2 kB
static BitBoard standardPosition;
static BitBoard clearedPosition;
void BitBoardInit();


const quint64 A1=1,     B1=A1<<1, C1=B1<<1, D1=C1<<1, E1=D1<<1, F1=E1<<1, G1=F1<<1, H1=G1<<1;
const quint64 A2=H1<<1, B2=A2<<1, C2=B2<<1, D2=C2<<1, E2=D2<<1, F2=E2<<1, G2=F2<<1, H2=G2<<1;
const quint64 A3=H2<<1, B3=A3<<1, C3=B3<<1, D3=C3<<1, E3=D3<<1, F3=E3<<1, G3=F3<<1, H3=G3<<1;
const quint64 A4=H3<<1, B4=A4<<1, C4=B4<<1, D4=C4<<1, E4=D4<<1, F4=E4<<1, G4=F4<<1, H4=G4<<1;
const quint64 A5=H4<<1, B5=A5<<1, C5=B5<<1, D5=C5<<1, E5=D5<<1, F5=E5<<1, G5=F5<<1, H5=G5<<1;
const quint64 A6=H5<<1, B6=A6<<1, C6=B6<<1, D6=C6<<1, E6=D6<<1, F6=E6<<1, G6=F6<<1, H6=G6<<1;
const quint64 A7=H6<<1, B7=A7<<1, C7=B7<<1, D7=C7<<1, E7=D7<<1, F7=E7<<1, G7=F7<<1, H7=G7<<1;
const quint64 A8=H7<<1, B8=A8<<1, C8=B8<<1, D8=C8<<1, E8=D8<<1, F8=E8<<1, G8=F8<<1, H8=G8<<1;

const uint RotateL90[64] = {
	h1, h2, h3, h4, h5, h6, h7, h8,
	g1, g2, g3, g4, g5, g6, g7, g8,
	f1, f2, f3, f4, f5, f6, f7, f8,
	e1, e2, e3, e4, e5, e6, e7, e8,
	d1, d2, d3, d4, d5, d6, d7, d8,
	c1, c2, c3, c4, c5, c6, c7, c8,
	b1, b2, b3, b4, b5, b6, b7, b8,
	a1, a2, a3, a4, a5, a6, a7, a8,
};

const uint RotateR45[64] = {
	a1, b8, c7, d6, e5, f4, g3, h2,
	a2, b1, c8, d7, e6, f5, g4, h3,
	a3, b2, c1, d8, e7, f6, g5, h4,
	a4, b3, c2, d1, e8, f7, g6, h5,
	a5, b4, c3, d2, e1, f8, g7, h6,
	a6, b5, c4, d3, e2, f1, g8, h7,
	a7, b6, c5, d4, e3, f2, g1, h8,
	a8, b7, c6, d5, e4, f3, g2, h1
};

const uint RotateL45[64] = {
	a2, b3, c4, d5, e6, f7, g8, h1,
	a3, b4, c5, d6, e7, f8, g1, h2,
	a4, b5, c6, d7, e8, f1, g2, h3,
	a5, b6, c7, d8, e1, f2, g3, h4,
	a6, b7, c8, d1, e2, f3, g4, h5,
	a7, b8, c1, d2, e3, f4, g5, h6,
	a8, b1, c2, d3, e4, f5, g6, h7,
	a1, b2, c3, d4, e5, f6, g7, h8
};

const uchar Castle[64] = {
	~4, 255, 255, 255, ~5, 255, 255,  ~1,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	~8, 255, 255, 255,  ~10, 255, 255,  ~2
};

const quint64 fileA       = A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8;
const quint64 fileB       = B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8;
const quint64 fileG       = G1 | G2 | G3 | G4 | G5 | G6 | G7 | G8;
const quint64 fileH       = H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8;
const quint64 rank4       = A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4;
const quint64 rank5       = A5 | B5 | C5 | D5 | E5 | F5 | G5 | H5;
const quint64 fileNotA    = ~ fileA;
const quint64 fileNotH    = ~ fileH;
const quint64 fileNotAB   = ~(fileA | fileB);
const quint64 fileNotGH   = ~(fileG | fileH);

#define SetBitL90(s)      (MaskL90[s])
#define SetBitL45(s)      (MaskL45[s])
#define SetBitR45(s)      (MaskR45[s])
#define ShiftDown(b)      ((b)>>8)
#define Shift2Down(b)     ((b)>>16)
#define ShiftUp(b)        ((b)<<8)
#define Shift2Up(b)       ((b)<<16)
#define ShiftRight(b)     (((b)<<1)&fileNotA)
#define Shift2Right(b)    (((b)<<2)&fileNotAB)
#define ShiftLeft(b)      (((b)>>1)&fileNotH)
#define Shift2Left(b)     (((b)>>2)&fileNotGH)
#define ShiftUpLeft(b)    (((b)<<7)&fileNotH)
#define ShiftUpRight(b)   (((b)<<9)&fileNotA)
#define ShiftDownLeft(b)  (((b)>>9)&fileNotH)
#define ShiftDownRight(b) (((b)>>7)&fileNotA)

// This makes a pretty big difference on BitBoard speed...
// To use the assembler version type: qmake -r "CONFIG+=fastbits"
inline uint GetFirstBitAndClear64(quint64& bb)
{
#if defined(Q_OS_WIN32) && defined(FASTBITS)
	// thanks to Gerd Isenberg
	__asm
	{
		xor	edx, edx
		mov	ebx, [bb]
		xor	eax, eax
		inc	edx
		bsf	ecx, [ebx]
		jnz	found
		bsf	ecx, [ebx+4]
		lea	ebx, [ebx+4]
		xor	eax, 32
	found:
		shl	edx, cl
		xor	eax, ecx
		xor	[ebx], edx
	}
#elif defined(FASTBITS)
	// Assuming x86 hardware...
	// Testing showed this to be about 20 times faster than the C++ code
	register uint ret;
	asm volatile (
		"pushl  %%ebx \n"
		"mov	%0, %%ebx \n"
		"xor	%%edx, %%edx \n"
		"xor	%%eax, %%eax \n"
		"inc	%%edx \n"
		"bsf	(%%ebx), %%ecx \n"
		"jnz	1f \n"
		"bsf	4(%%ebx), %%ecx \n"
		"lea	4(%%ebx), %%ebx \n"
		"xor	$32, %%eax \n"
		"1: \n"
	        "shl	%%cl, %%edx \n"
		"xor	%%ecx, %%eax \n"
		"xor	%%edx, (%%ebx) \n"
		"popl   %%ebx \n"
	: "=a"(ret), "=m"(bb) : "0"(&bb) : "%ecx", "%edx");
	return ret;
#else
	// SBE - After a fair bit of testing, this is the fastest portable version
	// i could come up with, it's about twice as fast as shift-testing 64 times.
	register quint64 x = bb & -bb;
	register uint r =  0;
	bb ^= x;
	if (!(x & 0xffffffff)) { x >>= 32; r |= 32; }
	if (!(x & 0xffff)) { x >>= 16; r |= 16; }
	if (!(x & 0xff)) { x >>= 8; r |= 8; }
	if (!(x & 0xf)) { x >>= 4; r |= 4; }
	if (!(x & 0x3)) { x >>= 2; r |= 2; }
	if (!(x & 0x1)) { r |= 1; }
	return r;
#endif
}

BitBoard::BitBoard()
{
	memset(this, 0, sizeof(BitBoard));
	if (!BitBoardInitRun)
		BitBoardInit();
}

bool BitBoard::isCheckmate() const
{
	MoveList moves(GenMoves());
	for (int i = 0; i < moves.size(); ++i)
		if (!isIntoCheck(moves[i]))
			return false;
	return isCheck();
}

bool BitBoard::isStalemate() const
{
	MoveList moves(GenMoves());
	for (int i = 0; i < moves.size(); ++i)
		if (!isIntoCheck(moves[i]))
			return false;
	return !isCheck();
}

void BitBoard::removeIllegal(quint64& b, const Move& move) const
{
	quint64 mask = 1;
	Move m = move;
	for (int sq = 0; sq < 64; sq++) {
		if (b & mask) {
			m.setFrom(sq);
			if (isIntoCheck(m))
				b &= ~mask;
		}
		mask <<= 1;
	}
}

inline char sanPiece(const int piece) { return " KQRBN"[piece]; }
QString BitBoard::moveToSan(const Move& move) const
{
	QString san;
	int from = move.from();
	int to = move.to();
	bool isPawn = piece[from] == Pawn;

	if (move.isCastling()) {
		if (from < to)
			san = "O-O";
		else	san = "O-O-O";
	} else {
		if (!isPawn) {
			san = sanPiece(piece[from]);

			// We may need disambiguation
			quint64 others = 0;
			switch (piece[from]) {
			case Knight:
				others = knights & KnightAttacksFrom(to);
				break;
			case Bishop:
				others = bishops & BishopAttacksFrom(to);
				break;
			case Rook:
				others = rooks & RookAttacksFrom(to);
				break;
			case Queen:
				others = queens & QueenAttacksFrom(to);
				break;
			case King:
				others = kings & KingAttacksFrom(to);
				break;
			default:
				break; // Something really wrong
			}

			others ^= SetBit(from);
			others &= occupied_co[stm];
			// Do not disambiguate with moves that put oneself in check.
			//    This is an expensive operation of dubious value, but people seem to want it
			if (others)
				removeIllegal(others, move);
			if (others) {
				bool row = false, column = false;
				if (others & ranks[Rank(from)])
					column = true;
				if (others & files[File(from)])
					row = true;
				else	column = true;
				if (column)
					san += 'a' + File(from);
				if (row)
					san += '1' + Rank(from);
			}
		}

		//capture x
		if (piece[to] || (move.isEnPassant())) {
			if (isPawn)
				san += 'a' + File(from);
			san += 'x';
		}

		//destination square
		san += 'a' + File(to);
		san += '1' + Rank(to);
	}

	if (move.isPromotion()) {
		san += '=';
		san += sanPiece(move.promoted());
	}

	BitBoard check(*this);
	check.doMove(move);
	if (check.isCheck()) {
		if (check.isCheckmate())
			san += '#';
		else	san += '+';
	}

	return san;
}

/** Remove all pieces and state from board */
void BitBoard::clear()
{
	*this = clearedPosition;
}

void BitBoard::setStandardPosition()
{
	*this = standardPosition;
}

bool BitBoard::isMovable(const Square from) const
{
	Q_ASSERT(from < 64);

	if (occupied_co[stm] & SetBit(from)) {
		quint64 squares = 0;
		switch (piece[from]) {
		case Pawn:	squares=pawnMovesFrom(from); break;
		case Knight:	squares=KnightAttacksFrom(from); break;
		case Bishop:	squares=BishopAttacksFrom(from); break;
		case Rook:	squares=RookAttacksFrom(from); break;
		case Queen:	squares=QueenAttacksFrom(from); break;
		case King:	squares=KingAttacksFrom(ksq[stm]); break;
		default: break;
		}
		squares &= ~occupied_co[stm];
		while (squares) {
			Square to = GetFirstBitAndClear64(squares);
			if (!isIntoCheck(Move(from,to)))
				return true;
		}
	}
	return false;
}

void BitBoard::setAt(const Square s, const Piece p)
{
	Q_ASSERT(s < 64);
	Q_ASSERT(isValidPiece(p));

	quint64 bit = SetBit(s);
	if (occupied & bit)
		removeAt(s);

	PieceType pt = pieceType(p);
	if (pt == None)
		return;

	Color _color = pieceColor(p);
	switch (pt) {
	case Pawn:	m_pawns   |= bit; break;
	case Knight:	knights |= bit; break;
	case Bishop:	bishops |= bit; break;
	case Rook:	rooks   |= bit; break;
	case Queen:	queens  |= bit; break;
	case King:	if (kings & occupied_co[_color])	// SBE -- FIXME - This is buggy
				removeAt(ksq[_color]);
			kings   |= bit;
			ksq[_color]=s;
			break;
	default: break; // ERROR
	}

	piece[s] = pt;
	occupied ^= bit;
	occupied_co[_color] ^= bit;
	occupied_l90 ^= SetBitL90(s);
	occupied_l45 ^= SetBitL45(s);
	occupied_r45 ^= SetBitR45(s);
}

void BitBoard::removeAt(const Square s)
{
	Q_ASSERT(s < 64);

	quint64 bit = SetBit(s);
	if (!(occupied & bit))
		return;

	Color _color = occupied_co[White] & bit ? White : Black;
	switch (piece[s]) {
	case Pawn:	m_pawns   ^= bit; break;
	case Knight:	knights ^= bit; break;
	case Bishop:	bishops ^= bit; break;
	case Rook:	rooks   ^= bit; break;
	case Queen:	queens  ^= bit; break;
	case King:	kings   ^= bit;
			ksq[_color]= InvalidSquare;
			break;
	default: break; // ERROR
	}

	piece[s] = Empty;
	occupied ^= bit;
	occupied_co[_color] ^= bit;
	occupied_l90 ^= SetBitL90(s);
	occupied_l45 ^= SetBitL45(s);
	occupied_r45 ^= SetBitR45(s);
}

void BitBoard::fixupBoards()
{
	occupied = whiteBB() + blackBB();
	for (int i = 0; i < 64; i++) {
		if (SetBit(i)&occupied) {
			occupied_l90 |= SetBitL90(i);
			occupied_l45 |= SetBitL45(i);
			occupied_r45 |= SetBitR45(i);
		}
	}
}

bool BitBoard::isValidFen(const QString& fen) const
{
	return BitBoard().fromGoodFen(fen);
}

int BitBoard::fromFen(const QString& fen)
{
	if (isValidFen(fen))
		return fromGoodFen(fen);
	return false;
}

BoardStatus BitBoard::validate() const
{
	uint wk=0, bk=0, wp=0, bp=0, bo=0, wo=0;
	for (uint i=0; i < 64; ++i) {
		Piece p = pieceAt(i);
		if (p == WhiteKing) {
			++wk;
		} else if (p == BlackKing) {
			++bk;
		} else if (p == WhitePawn) {
			if (i <= h1 || i >= a8) // Pawns on 1st or 8th
				return PawnsOn18;
			else	++wp;
		} else if (p == BlackPawn) {
			if (i <= h1 || i >= a8) // Pawns on 1st or 8th
				return PawnsOn18;
			else	++bp;
		} else if (isWhite(p)) {
			++wo;
		} else if (isBlack(p)) {
			++bo;
		}
	}

	// Exactly one king per side
	if (wk == 0) return NoWhiteKing;
	if (bk == 0) return NoBlackKing;
	if (wk != 1 || bk != 1) return TooManyKings;

	// No more than 8 pawns per side
	if (wp > 8) return TooManyWhitePawns;
	if (bp > 8) return TooManyBlackPawns;

	// Maximum 16 pieces per side
	if ((wk + wp + wo) > 16) return TooManyWhite;
	if ((bk + bp + bo) > 16) return TooManyBlack;

	// Bad checks
	bool check =  IsAttackedBy(stm^1, ksq[stm]);
	bool check2 = IsAttackedBy(stm, ksq[stm^1]);
	if (check && check2) return DoubleCheck;
	if (check2) return OppositeCheck;

	// Can't castle with missing rook
	if (canCastleLong(White) && pieceAt(a1) != WhiteRook)
		return BadCastlingRights;
	if (canCastleShort(White) && pieceAt(h1) != WhiteRook)
		return BadCastlingRights;
	if (canCastleLong(Black) && pieceAt(a8) != BlackRook)
		return BadCastlingRights;
	if (canCastleShort(Black) && pieceAt(h8) != BlackRook)
		return BadCastlingRights;

	// Can't castle if king has moved
	if (canCastle(White) && pieceAt(e1) != WhiteKing)
		return BadCastlingRights;
	if (canCastle(Black) && pieceAt(e8) != BlackKing)
		return BadCastlingRights;

	// Detect unreasonable ep square
	// FIXME -- need code here to return InvalidEnPassant

	// Don't allow triple checks.

	return Valid;
}


// Why QString throws asserts for access past end of string and
// refuses to return a real c++ char type is beyond me...
class SaneString : public QString
{
public:
	SaneString(const QString& q) : QString(q) {};
	char operator[] (const int index) const
	{
		if (index < this->length())
			return QString::operator[](index).toAscii();
		return 0;
	}
};

// SBE -- FIXME -- We really shouldn't be so anal, we should just accept marginal FEN's
int BitBoard::fromGoodFen(const QString& qfen)
{
	SaneString fen(qfen);
	int i;
	uint s;
	char c = fen[0];
	int moveNum=0;

	memset(this, 0, sizeof(BitBoard));

	// Piece position
	i = 0;
	s = 56;
	while (c != ' ' && s <= 64) {
		switch (c) {
		case '/': s -= 16; break;
		case '1': s += 1; break;
		case '2': s += 2; break;
		case '3': s += 3; break;
		case '4': s += 4; break;
		case '5': s += 5; break;
		case '6': s += 6; break;
		case '7': s += 7; break;
		case '8': s += 8; break;
		case 'p':
			piece[s] = Pawn;
			m_pawns |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			s++;
			break;
		case 'n':
			piece[s] = Knight;
			knights |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			s++;
			break;
		case 'b':
			piece[s] = Bishop;
			bishops |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			s++;
			break;
		case 'r':
			piece[s] = Rook;
			rooks |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			s++;
			break;
		case 'q':
			piece[s] = Queen;
			queens |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			s++;
			break;
		case 'k':
			piece[s] = King;
			kings |= SetBit(s);
			occupied_co[Black] |= SetBit(s);
			ksq[Black] = s;
			s++;
			break;
		case 'P':
			piece[s] = Pawn;
			m_pawns |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			s++;
			break;
		case 'N':
			piece[s] = Knight;
			knights |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			s++;
			break;
		case 'B':
			piece[s] = Bishop;
			bishops |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			s++;
			break;
		case 'R':
			piece[s] = Rook;
			rooks |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			s++;
			break;
		case 'Q':
			piece[s] = Queen;
			queens |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			s++;
			break;
		case 'K':
			piece[s] = King;
			kings |= SetBit(s);
			occupied_co[White] |= SetBit(s);
			ksq[White] = s;
			s++;
			break;
		default:
			return 0;
		}
		c = fen[++i];
	}
	if (s != 8)
		return 0;

	// Side to move
	c = fen[++i];
	if (c == 'w') stm = White;
	else if (c == 'b') stm = Black;
	else return 0;
	c = fen[++i];

	// Castling Rights
	c = fen[++i];
	if (c != '-') {
		while (c != ' ') {
			if (c == 'K')  SetCastleShort(White);
			else if (c == 'Q')  SetCastleLong(White);
			else if (c == 'k')  SetCastleShort(Black);
			else if (c == 'q')  SetCastleLong(Black);
			else return 0;
			c = fen[++i];
		}
	} else	++i;  // Bypass space

	// EnPassant Square
	epfile = 0;
	c = fen[++i];
	if (c != '-') {
		if (c >= 'a' && c <= 'h')
			epfile = c - 'a' + 1;
		else if (c >= 'A' && c <= 'H')
			epfile = c - 'A' + 1;
		else return 0;
		c = fen[++i];
		if (stm == White && c != '6')
			return 0;
		else if (stm == Black && c != '3')
			return 0;
	}
	epFile2Square();
	c = fen[++i];

	// Half move clock
	c = fen[++i];
	if (c < '0' || c > '9')
		return 0;
	m_halfMoves = fen.mid(i).toInt();
	while (c >= '0' && c <= '9')
		c = fen[++i];

	// Move number
	c = fen[++i];
	if (c < '0' || c > '9')
		return 0;
	moveNum = fen.mid(i).toInt();
	while (c >= '0' && c <= '9')
		c = fen[++i];
	if (moveNum < 1)
		return 0;

	// All okay, let's play ball.
	fixupBoards();
	return moveNum;
}

MoveList BitBoard::GenMoves() const
{
	register uint from, to;
	quint64 moves, movers;

	MoveList p;

	if (stm == White) {

		// castle moves
		if (canCastle(White)) {
			if (canCastleShort(White) && !((F1 | G1)&occupied))
				if (!IsAttackedBy(Black, e1) && !IsAttackedBy(Black, f1) && !IsAttackedBy(Black, g1))
					p.add().GenWhiteOO();
			if (canCastleLong(White)  && !((B1 | C1 | D1)&occupied))
				if (!IsAttackedBy(Black, c1) && !IsAttackedBy(Black, d1) && !IsAttackedBy(Black, e1))
					p.add().GenWhiteOOO();
		}

		// pawn en passant moves
		movers = m_pawns & whiteBB();
		if (epSquare != NoEPSquare) {
			moves = PawnBB[Black][epSquare] & movers;
			while (moves) {
				from = GetFirstBitAndClear64(moves);
				p.add().GenEnPassant(from, epSquare);
			}
		}

		// pawn captures
		moves = ShiftUpRight(movers) & blackBB();
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 7) {
				p.add().GenPawnMove(to - 9, to, piece[to]);
			} else {
				p.add().GenCapturePromote(to - 9, to, Queen, piece[to]);
				p.add().GenCapturePromote(to - 9, to, Knight, piece[to]);
				p.add().GenCapturePromote(to - 9, to, Rook, piece[to]);
				p.add().GenCapturePromote(to - 9, to, Bishop, piece[to]);
			}
		}
		moves = ShiftUpLeft(movers) & blackBB();
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 7) {
				p.add().GenPawnMove(to - 7, to, piece[to]);
			} else {
				p.add().GenCapturePromote(to - 7, to, Queen, piece[to]);
				p.add().GenCapturePromote(to - 7, to, Knight, piece[to]);
				p.add().GenCapturePromote(to - 7, to, Rook, piece[to]);
				p.add().GenCapturePromote(to - 7, to, Bishop, piece[to]);
			}
		}

		// pawns 1 forward
		moves = ShiftUp(movers) & ~occupied;
		movers = moves;
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 7) {
				p.add().GenOneForward(to - 8, to);
			} else {
				p.add().GenPromote(to - 8, to, Queen);
				p.add().GenPromote(to - 8, to, Knight);
				p.add().GenPromote(to - 8, to, Rook);
				p.add().GenPromote(to - 8, to, Bishop);
			}
		}
		// pawns 2 forward
		moves = ShiftUp(movers) & rank4 & ~occupied;
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenTwoForward(to - 16, to);
		}

	} else {

		// castle moves
		if (canCastle(Black)) {
			if (canCastleShort(Black) && !((F8 | G8)&occupied))
				if (!IsAttackedBy(White, e8) && !IsAttackedBy(White, f8) && !IsAttackedBy(White, g8))
					p.add().GenBlackOO();
			if (canCastleLong(Black)  && !((B8 | C8 | D8)&occupied))
				if (!IsAttackedBy(White, e8) && !IsAttackedBy(White, d8) && !IsAttackedBy(White, c8))
					p.add().GenBlackOOO();
		}

		// pawn en passant moves
		movers = m_pawns & blackBB();
		if (epSquare != NoEPSquare) {
			moves = PawnBB[White][epSquare] & movers;
			while (moves) {
				from = GetFirstBitAndClear64(moves);
				p.add().GenEnPassant(from, epSquare);
			}
		}

		// pawn captures
		moves = ShiftDownLeft(movers) & whiteBB();
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 0) {
				p.add().GenPawnMove(to + 9, to, piece[to]);
			} else {
				p.add().GenCapturePromote(to + 9, to, Queen, piece[to]);
				p.add().GenCapturePromote(to + 9, to, Knight, piece[to]);
				p.add().GenCapturePromote(to + 9, to, Rook, piece[to]);
				p.add().GenCapturePromote(to + 9, to, Bishop, piece[to]);
			}
		}
		moves = ShiftDownRight(movers) & whiteBB();
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 0) {
				p.add().GenPawnMove(to + 7, to, piece[to]);
			} else {
				p.add().GenCapturePromote(to + 7, to, Queen, piece[to]);
				p.add().GenCapturePromote(to + 7, to, Knight, piece[to]);
				p.add().GenCapturePromote(to + 7, to, Rook, piece[to]);
				p.add().GenCapturePromote(to + 7, to, Bishop, piece[to]);
			}
		}

		// pawns 1 forward
		moves = ShiftDown(movers) & ~occupied;
		movers = moves;
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			if (Rank(to) != 0) {
				p.add().GenOneForward(to + 8, to);
			} else {
				p.add().GenPromote(to + 8, to, Queen);
				p.add().GenPromote(to + 8, to, Knight);
				p.add().GenPromote(to + 8, to, Rook);
				p.add().GenPromote(to + 8, to, Bishop);
			}
		}
		// pawns 2 forward
		moves = ShiftDown(movers) & rank5 & ~occupied;
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenTwoForward(to + 16, to);
		}
	}

	// knight moves
	movers = knights & Occupied(stm);
	while (movers) {
		from = GetFirstBitAndClear64(movers);
		moves = KnightAttacksFrom(from) & ~Occupied(stm);
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenKnightMove(from, to, piece[to]);
		}
	}
	// bishop moves
	movers = bishops & Occupied(stm);
	while (movers) {
		from = GetFirstBitAndClear64(movers);
		moves = BishopAttacksFrom(from) & ~Occupied(stm);
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenBishopMove(from, to, piece[to]);
		}
	}
	// rook moves
	movers = rooks & Occupied(stm);
	while (movers) {
		from = GetFirstBitAndClear64(movers);
		moves = RookAttacksFrom(from) & ~Occupied(stm);
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenRookMove(from, to, piece[to]);
		}
	}
	// queen moves
	movers = queens & Occupied(stm);
	while (movers) {
		from = GetFirstBitAndClear64(movers);
		moves = QueenAttacksFrom(from) & ~Occupied(stm);
		while (moves) {
			to = GetFirstBitAndClear64(moves);
			p.add().GenQueenMove(from, to, piece[to]);
		}
	}
	// king moves
	moves = KingAttacksFrom(ksq[stm]) & ~Occupied(stm);
	while (moves) {
		to = GetFirstBitAndClear64(moves);
		if (!IsAttackedBy(stm ^ 1, to))
			p.add().GenKingMove(ksq[stm], to, piece[to]);
	}

	return p;
}

bool BitBoard::isIntoCheck(const Move& move) const
{
	BitBoard peek(*this);
	peek.doMove(move);
	peek.swapToMove();
	return peek.isCheck();
}

#include <string.h>
inline bool isFile(const char c) {return c >= 'a' && c <= 'h';}
inline bool isRank(const char c) {return c >= '1' && c <= '8';}
// This handles both SAN and LAN
Move BitBoard::parseMove(const QString& algebraic) const
{
	const QByteArray& bs(algebraic.toAscii()); // Working with QStrings is painful
	const char *san = bs.constData();
	const char* s = san;
	register char c = *(s++);
	quint64 match;     // Try making this "register" some day
	int fromSquare = -1;
	int toSquare = -1;
	int fromFile = -1;
	int fromRank = -1;
	uchar promotePiece = Empty;
	Move move;
	uint type;

	// Castling
	if (c == 'O' || c == '0') {
		if (strncmp(san, "O-O-O", 5) == 0 || strncmp(san, "0-0-0", 5)  == 0) {
			if (stm == White)
				return prepareMove(e1, c1);
			else	return prepareMove(e8, c8);
		} else if (strncmp(san, "O-O", 3) == 0 || strncmp(san, "0-0", 3)  == 0) {
			if (stm == White)
				return prepareMove(e1, g1);
			else	return prepareMove(e8, g8);
		}
		return move;
	}

	// Piece
	switch (c) {
	case 'Q': type = Queen; c = *(s++); break;
	case 'R': type = Rook; c = *(s++); break;
	case 'B': type = Bishop; c = *(s++); break;
	case 'N': type = Knight; c = *(s++); break;
	case 'K': type = King; c = *(s++); break;
	case 'P': c = *(s++); // Fall through
	default: type = Pawn; break;
	}

	// Check for disambiguation
	if (isFile(c)) {
		fromFile = c - 'a';
		c = *(s++);
		if (isRank(c)) {
			fromSquare = (c - '1') * 8 + fromFile;
			fromFile = -1;
			c = *(s++);
		}
	} else if (isRank(c)) {
		fromRank = c - '1';
		c = *(s++);
	}

	// Capture indicator (or dash in the case of a LAN move)
	if (c == 'x' || c == '-')
		c = *(s++);

	// Destination square
	if (isFile(c)) {
		int f = c - 'a';
		c = *(s++);
		if (!isRank(c))
			return move;
		toSquare = (c - '1') * 8 + f;
		c = *(s++);
	} else {
		toSquare = fromSquare;
		fromSquare = -1;
	}

	if (toSquare < 0)
		return move;

	if (type == Pawn) {
		// Promotion as in bxc8=Q or bxc8(Q)
		if (c == '=' || c == '(') {
			c = *(s++);
			switch (c) {
				case 'Q': promotePiece = Queen; break;
				case 'R': promotePiece = Rook; break;
				case 'B': promotePiece = Bishop; break;
				case 'N': promotePiece = Knight; break;
				default: return move;
			}
		}
		if (fromSquare < 0) {
			int base = (stm == White ? -8 : 8);
			if (fromFile < 0) {
				fromSquare = toSquare + base;
				quint64 bit = SetBit(fromSquare);
				if (!(occupied_co[stm] & bit))
					fromSquare += base;
			} else if (fromFile <= (int) File(toSquare))
				fromSquare = toSquare + base - 1;
			else	fromSquare = toSquare + base + 1;
		}
		// This isn't a hugely efficient way to go from here
		move = prepareMove(fromSquare, toSquare);
		if (move.isPromotion() && promotePiece)
			move.setPromoted(promotePiece);
		return move;
	}

	if  (fromSquare < 0) {
		switch (type) {
		case Queen: match = QueenAttacksFrom(toSquare) & queens; break;
		case Rook: match = RookAttacksFrom(toSquare) & rooks; break;
		case Bishop: match = BishopAttacksFrom(toSquare) & bishops; break;
		case Knight: match = KnightAttacksFrom(toSquare) & knights; break;
		case King: match = KingAttacksFrom(toSquare) & kings; break;
		default:   return move;
		}

		match &= occupied_co[stm];
		if (fromRank >= 0)
			match &= ranks[fromRank];
		else if (fromFile >= 0)
			match &= files[fromFile];
		fromSquare = GetFirstBitAndClear64(match);

		// If not yet fully disambiguated, all but one move must be illegal
		//  Cycle through them, and pick the first legal move.
		while (match) {
			if (isIntoCheck(Move(fromSquare, toSquare)))
				fromSquare = GetFirstBitAndClear64(match);
			else	break;
		}
	}

	if (type != piece[fromSquare])
		return move;
	return prepareMove(fromSquare, toSquare);
}

void BitBoard::undoMove(const Move& m)
{
	register uint from = m.from();
	register uint to = m.to();
	register uint sntm = stm ^ 1; // side not to move
	register quint64 bb_from = SetBit(from);
	register quint64 bb_to = SetBit(to);
	uint rook_from=0, rook_to=0; // =0 just to quiet compiler warnings

	uint action = m.Action();
	switch (action) {
	case Pawn:
	case Move::TWOFORWARD:
		m_pawns ^= bb_from ^ bb_to;
		piece[from] = Pawn;
		break;
	case Knight:
		knights ^= bb_from ^ bb_to;
		piece[from] = Knight;
		break;
	case Bishop:
		bishops ^= bb_from ^ bb_to;
		piece[from] = Bishop;
		break;
	case Rook:
		rooks ^= bb_from ^ bb_to;
		piece[from] = Rook;
		break;
	case Queen:
		queens ^= bb_from ^ bb_to;
		piece[from] = Queen;
		break;
	case King:
		kings ^= bb_from ^ bb_to;
		ksq[sntm] = from;
		piece[from] = King;
		break;
	case Move::CASTLE:
		kings ^= bb_from ^ bb_to;
		ksq[sntm] = from;
		piece[from] = King;
		switch (to) {
		case c1:
			rook_from = a1;
			rook_to = d1;
			break;
		case g1:
			rook_from = h1;
			rook_to = f1;
			break;
		case c8:
			rook_from = a8;
			rook_to = d8;
			break;
		case g8:
			rook_from = h8;
			rook_to = f8;
			break;
		}
		piece[rook_to] = Empty;
		piece[rook_from] = Rook;
		rooks ^= SetBit(rook_from) ^ SetBit(rook_to);
		occupied_co[sntm] ^= SetBit(rook_from) ^ SetBit(rook_to);
		occupied_l90 ^= SetBitL90(rook_from) ^ SetBitL90(rook_to);
		occupied_l45 ^= SetBitL45(rook_from) ^ SetBitL45(rook_to);
		occupied_r45 ^= SetBitR45(rook_from) ^ SetBitR45(rook_to);
		break;
	case Move::PROMOTE:
		m_pawns ^= bb_from;
		piece[from] = Pawn;
		switch (m.promoted()) {
		case Knight:
			knights ^= bb_to;
			break;
		case Bishop:
			bishops ^= bb_to;
			break;
		case Rook:
			rooks   ^= bb_to;
			break;
		case Queen:
			queens  ^= bb_to;
			break;
		default:  // can't promote to other piece types;
			break;
		}
		break;
	}

	uint replace = m.captureInternal();
	switch (m.Removal()) {  // Reverse captures
	case Empty:
		occupied_l90 ^= SetBitL90(to);     // extra cleanup needed for non-captures
		occupied_l45 ^= SetBitL45(to);
		occupied_r45 ^= SetBitR45(to);
		break;
	case Pawn:
		m_pawns ^= bb_to;
		occupied_co[stm] ^= bb_to;
		break;
	case Knight:
		knights ^= bb_to;
		occupied_co[stm] ^= bb_to;
		break;
	case Bishop:
		bishops ^= bb_to;
		occupied_co[stm] ^= bb_to;
		break;
	case Rook:
		rooks ^= bb_to;
		occupied_co[stm] ^= bb_to;
		break;
	case Queen:
		queens ^= bb_to;
		occupied_co[stm] ^= bb_to;
		break;
	case Move::ENPASSANT:
		replace = Empty;
		uint epsq = to + (sntm == White ? -8 : 8);  // annoying move, the capture is not on the 'to' square
		piece[epsq] = Pawn;
		m_pawns ^= SetBit(epsq);
		occupied_co[stm] ^= SetBit(epsq);
		occupied_l90 ^= SetBitL90(to) ^ SetBitL90(epsq);
		occupied_l45 ^= SetBitL45(to) ^ SetBitL45(epsq);
		occupied_r45 ^= SetBitR45(to) ^ SetBitR45(epsq);
		break;
	}  // ...no I did not forget the king :)


	piece[to] = replace;
	occupied_co[sntm] ^= bb_from ^ bb_to;
	occupied_l90 ^= SetBitL90(from);
	occupied_l45 ^= SetBitL45(from);
	occupied_r45 ^= SetBitR45(from);
	occupied = whiteBB() + blackBB();

	stm ^= 1;	// toggle side to move

	m_halfMoves = m.u & 0xFF;
	castle = (m.u >> 8) & 0xF;
	epfile = (m.u >> 12) & 0xF;
	epFile2Square();
}

bool BitBoard::doMove(const Move& m)
{
	register uint from = m.from();
	register uint to = m.to();
	register uint sntm = stm ^ 1; // side not to move
	register quint64 bb_from = SetBit(from);
	register quint64 bb_to = SetBit(to);
	uint rook_from=0, rook_to=0;

	epfile = 0;
	m_halfMoves++;	// Number of moves since last capture or pawn move

	uint action = m.Action();
	switch (action) {
	case Pawn:
		m_halfMoves = 0;
		m_pawns ^= bb_from ^ bb_to;
		piece[to] = Pawn;
		break;
	case Knight:
		knights ^= bb_from ^ bb_to;
		piece[to] = Knight;
		break;
	case Bishop:
		bishops ^= bb_from ^ bb_to;
		piece[to] = Bishop;
		break;
	case Rook:
		rooks ^= bb_from ^ bb_to;
		piece[to] = Rook;
		if (canCastle(stm)) // a rook is moving, destroy castle flags if needed
			castle &= Castle[from]; // question is if always doing the table is just faster?
		break;
	case Queen:
		queens ^= bb_from ^ bb_to;
		piece[to] = Queen;
		break;
	case King:
		kings ^= bb_from ^ bb_to;
		ksq[stm] = to;
		piece[to] = King;
		DestroyCastle(stm);	// king is moving so definitely destroy castle stuff!
		break;
	case Move::CASTLE:
		kings ^= bb_from ^ bb_to;
		ksq[stm] = to;
		piece[to] = King;
		DestroyCastle(stm);
		switch (to) {
		case c1:
			rook_from = a1;
			rook_to = d1;
			break;
		case g1:
			rook_from = h1;
			rook_to = f1;
			break;
		case c8:
			rook_from = a8;
			rook_to = d8;
			break;
		case g8:
			rook_from = h8;
			rook_to = f8;
			break;
		}
		piece[rook_from] = Empty;
		piece[rook_to] = Rook;
		rooks ^= SetBit(rook_from) ^ SetBit(rook_to);
		occupied_co[stm] ^= SetBit(rook_from) ^ SetBit(rook_to);
		occupied_l90 ^= SetBitL90(rook_from) ^ SetBitL90(rook_to);
		occupied_l45 ^= SetBitL45(rook_from) ^ SetBitL45(rook_to);
		occupied_r45 ^= SetBitR45(rook_from) ^ SetBitR45(rook_to);
		break;
	case Move::TWOFORWARD:
		m_halfMoves = 0;
		m_pawns ^= bb_from ^ bb_to;
		piece[to] = Pawn;
		if (stm == White) {
			if (PawnBB[White][to-8] & Occupied(sntm) & m_pawns)
				epfile = File(to) + 1;
		} else {
			if (PawnBB[Black][to+8] & Occupied(sntm) & m_pawns)
				epfile = File(to) + 1;
		}
		break;
	case Move::PROMOTE:
		m_halfMoves = 0;
		m_pawns ^= bb_from;
		switch (m.promoted()) {
		case Knight:
			knights ^= bb_to;
			piece[to] = Knight;
			break;
		case Bishop:
			bishops ^= bb_to;
			piece[to] = Bishop;
			break;
		case Rook:
			rooks   ^= bb_to;
			piece[to] = Rook;
			break;
		case Queen:
			queens  ^= bb_to;
			piece[to] = Queen;
			break;
		default:  // can't promote to other piece types;
			break;
		}
		break;
	}

	switch (m.Removal()) {
	case Empty:
		occupied_l90 ^= SetBitL90(to);     // extra cleanup needed for non-captures
		occupied_l45 ^= SetBitL45(to);
		occupied_r45 ^= SetBitR45(to);
		break;
	case Pawn:
		m_halfMoves = 0;
		m_pawns ^= bb_to;
		occupied_co[sntm] ^= bb_to;
		break;
	case Knight:
		m_halfMoves = 0;
		knights ^= bb_to;
		occupied_co[sntm] ^= bb_to;
		break;
	case Bishop:
		m_halfMoves = 0;
		bishops ^= bb_to;
		occupied_co[sntm] ^= bb_to;
		break;
	case Rook:
		m_halfMoves = 0;
		rooks ^= bb_to;
		occupied_co[sntm] ^= bb_to;
		if (canCastle(sntm))
			castle &= Castle[to];
		break;
	case Queen:
		m_halfMoves = 0;
		queens ^= bb_to;
		occupied_co[sntm] ^= bb_to;
		break;
	case Move::ENPASSANT:
		m_halfMoves = 0;
		uint epsq = to + (stm == White ? -8 : 8);  // annoying move, the capture is not on the 'to' square
		piece[epsq] = Empty;
		m_pawns ^= SetBit(epsq);
		occupied_co[sntm] ^= SetBit(epsq);
		occupied_l90 ^= SetBitL90(to) ^ SetBitL90(epsq);
		occupied_l45 ^= SetBitL45(to) ^ SetBitL45(epsq);
		occupied_r45 ^= SetBitR45(to) ^ SetBitR45(epsq);
		break;
	}  // ...no I did not forget the king :)

	piece[from] = Empty;
	occupied_co[stm] ^= bb_from ^ bb_to;
	occupied_l90 ^= SetBitL90(from);
	occupied_l45 ^= SetBitL45(from);
	occupied_r45 ^= SetBitR45(from);
	occupied = whiteBB() + blackBB();

	stm ^= 1;	// toggle side to move
	epFile2Square();
	return true;
}

quint64 BitBoard::pawnMovesFrom(const Square s) const
{
	quint64 targets = PawnF1[stm][s] & ~occupied;
	if (targets)
		targets |= PawnF2[stm][s] & ~occupied;
	targets |= PawnBB[stm][s] & (occupied_co[stm^1] | SetBit(epSquare));
	return targets;
}

// If from-to squares give a legal move, return a proper Move() object
Move BitBoard::prepareMove(const Square& from, const Square& to) const
{
	Q_ASSERT(from < 64);
	Q_ASSERT(to < 64);

	quint64 src = SetBit(from);
	quint64 dest = SetBit(to);
	Move move(from, to);
	uchar p = piece[from];

	if (!(occupied_co[stm] & src) || occupied_co[stm] & dest)
		return move;

	move.setPieceType(p);
	move.setCaptureType(piece[to]);
	if (p == King) {
		if (!(KingAttacksFrom(to) & src) && !prepareCastle(move))
			return move;
	} else if (p == Pawn) {
		if (!(pawnMovesFrom(from) & dest))
			return move;
		else if (to == epSquare)
			move.setEnPassant();
		else if (dest & PawnF2[stm][from])
			move.setTwoForward();
		else if (dest & PawnPM[stm])
			move.setPromoted(Queen);
	} else {
		quint64 reach = 0;
		if (p == Queen)		reach = QueenAttacksFrom(to);
		else if (p == Rook)	reach = RookAttacksFrom(to);
		else if (p == Bishop)	reach = BishopAttacksFrom(to);
		else if (p == Knight)	reach = KnightAttacksFrom(to);
		if (!(reach & src))
			return move;
	}

	BitBoard peek(*this);
	peek.doMove(move);
	peek.swapToMove();
	if (peek.isCheck())  // Don't allow move into check
		return move;

	if (stm == Black)
		move.setBlack();
	move.u = m_halfMoves;
	move.u |= (((ushort) castle & 0xF) << 8);
	move.u |= (((ushort) epfile & 0xF) << 12);
	move.setLegalMove();
	return move;
}

bool BitBoard::prepareCastle(Move& move) const
{
	if (!canCastle(stm))
		return false;

	Square to = move.to();
	if (stm == White) {
		if (to == g1 && canCastleShort(White) && !((F1|G1)&occupied))
			if (!IsAttackedBy(Black, e1) && !IsAttackedBy(Black, f1)) {
				move.GenWhiteOO();
				return true;
			}
		if (to == c1 && canCastleLong(White) && !((B1|C1|D1)&occupied))
			if (!IsAttackedBy(Black, e1) && !IsAttackedBy(Black, d1)) {
				move.GenWhiteOOO();
				return true;
			}
	} else {
		if (to == g8 && canCastleShort(Black) && !((F8|G8)&occupied))
			if (!IsAttackedBy(White, e8) && !IsAttackedBy(White, f8)) {
				move.GenBlackOO();
				return true;
			}
		if (to == c8 && canCastleLong(Black) && !((B8|C8|D8)&occupied))
			if (!IsAttackedBy(White, e8) && !IsAttackedBy(White, d8)) {
				move.GenBlackOOO();
				return true;
			}
	}

	return false;
}


Piece BitBoard::pieceAt(Square s) const
{
	Q_ASSERT(s < 64);
	quint64 bit = SetBit(s);
	if (occupied & bit) {
		if (occupied_co[White] & bit)
			return Piece(piece[s]);
		else	return Piece(piece[s] + 6);
	}
	return Empty;
}


QString BitBoard::toFen(int move) const
{
	QString fen = "";
	Piece piece;
	int empty = 0;

	//piece placement
	for (int row = 7; row >= 0; row--) {
		for (int col = 0; col < 8; col++) {
			piece = pieceAt(8 * row + col);
			if (piece != Empty) {
				if (empty != 0) {
					fen += QString::number(empty);
					empty = 0;
				}
				fen += pieceToChar(piece);
			} else	empty++;
		}
		if (empty != 0) {
			fen += QString::number(empty);
			empty = 0;
		}
		if (row != 0)
			fen += '/';
	}

	//side to move
	fen += stm == White ? " w " : " b ";

	//castling rights
	if (castlingRights() == NoRights)
		fen += "- ";
	else {
		if (castlingRights() & WhiteKingside)
			fen += 'K';
		if (castlingRights() & WhiteQueenside)
			fen += 'Q';
		if (castlingRights() & BlackKingside)
			fen += 'k';
		if (castlingRights() & BlackQueenside)
			fen += 'q';
		fen += ' ';
	}

	//en passant square
	if (epSquare == NoEPSquare) {
		fen += "- ";
	} else {
		fen += 'a' + (epSquare & 7);
		fen += '1' + ((epSquare & 56) >> 3);
		fen += ' ';
	}

	//half move clock
	fen += QString::number(halfMoveClock());

	//full move number not used by board
	fen += move <= 0 ? " 1" : " " + QString::number(move);

	return fen;
}


#define SetBit(s)         (Mask[s])
#define File(s)           ((s)&7)
#define Rank(s)           ((s)>>3)
void BitBoardInit()
{
	BitBoardInitRun = true;
	int i, q;
	quint64 mask;

	mask = 1;
	for (i = 0;i < 64;i++) {
		Mask[i] = mask << i;
	}
	for (i = 0;i < 64;i++) {
		MaskL90[i] = SetBit(RotateL90[i]);
		MaskL45[i] = SetBit(RotateL45[i]);
		MaskR45[i] = SetBit(RotateR45[i]);
	}

	// pawns:
	for (i = 0;i < 64;i++) {
		mask = SetBit(i);
		PawnBB[White][i]  = ShiftUpLeft(mask);
		PawnBB[White][i] |= ShiftUpRight(mask);

		PawnBB[Black][i]  = ShiftDownLeft(mask);
		PawnBB[Black][i] |= ShiftDownRight(mask);

		PawnF1[White][i]  = ShiftUp(mask);
		PawnF2[White][i]  = Shift2Up(mask) & rank4;

		PawnF1[Black][i]  = ShiftDown(mask);
		PawnF2[Black][i]  = Shift2Down(mask) & rank5;

		PawnALL[White][i] = PawnBB[White][i] | PawnF1[White][i] | PawnF2[White][i];
		PawnALL[Black][i] = PawnBB[Black][i] | PawnF1[Black][i] | PawnF2[Black][i];
	}

	for (i = 0;i < 64;i++) {
		mask = SetBit(i);
		KnightBB[i]  = ShiftLeft(Shift2Up(mask));
		KnightBB[i] |= ShiftRight(Shift2Up(mask));
		KnightBB[i] |= ShiftLeft(Shift2Down(mask));
		KnightBB[i] |= ShiftRight(Shift2Down(mask));
		KnightBB[i] |= Shift2Left(ShiftUp(mask));
		KnightBB[i] |= Shift2Right(ShiftUp(mask));
		KnightBB[i] |= Shift2Left(ShiftDown(mask));
		KnightBB[i] |= Shift2Right(ShiftDown(mask));
	}

	// bishops
	for (int s = 0;s < 64;s++) {
		for (int b = 0;b < 64;b++) {
			mask = 0;
			q = s;
			while (File(q) > 0 && Rank(q) < 7) {
				q += 7;
				mask |= SetBit(q);
				if (b&(SetBit(RotateL45[q]) >> ShiftL45[s]))
					break;
			}
			q = s;
			while (File(q) < 7 && Rank(q) > 0) {
				q -= 7;
				mask |= SetBit(q);
				if (b&(SetBit(RotateL45[q]) >> ShiftL45[s]))
					break;
			}
			BishopL45[s][b] = mask;

			mask = 0;
			q = s;
			while (File(q) < 7 && Rank(q) < 7) {
				q += 9;
				mask |= SetBit(q);
				if (b&(SetBit(RotateR45[q]) >> ShiftR45[s]))
					break;
			}
			q = s;
			while (File(q) > 0 && Rank(q) > 0) {
				q -= 9;
				mask |= SetBit(q);
				if (b&(SetBit(RotateR45[q]) >> ShiftR45[s]))
					break;
			}
			BishopR45[s][b] = mask;
		}
	}

	// rooks
	memset(RookL00, 0, sizeof(RookL00));
	memset(RookL90, 0, sizeof(RookL90));
	int file, rank;
	for (int sq = 0;sq < 64;sq++) {
		for (int bitrow = 0;bitrow < 64;bitrow++) {
			file = File(sq);
			q = sq + 1;
			while (++file < 8) {
				RookL00[sq][bitrow] |= SetBit(q);
				if ((1 << file)&(bitrow << 1))
					break;
				++q;
			}
			file = File(sq);
			q = sq - 1;
			while (--file >= 0) {
				RookL00[sq][bitrow] |= SetBit(q);
				if ((1 << file)&(bitrow << 1))
					break;
				--q;
			}
			rank = Rank(sq);
			q = sq + 8;
			while (++rank < 8) {
				RookL90[sq][bitrow] |= SetBit(q);
				if ((1 << (7 - rank))&(bitrow << 1))
					break;
				q += 8;
			}
			rank = Rank(sq);
			q = sq - 8;
			while (--rank >= 0) {
				RookL90[sq][bitrow] |= SetBit(q);
				if ((1 << (7 - rank))&(bitrow << 1))
					break;
				q -= 8;
			}
		}
	}

	// king:
	for (i = 0;i < 64;i++) {
		mask = SetBit(i);
		KingBB[i]  = ShiftLeft(mask);
		KingBB[i] |= ShiftRight(mask);
		KingBB[i] |= ShiftUp(mask);
		KingBB[i] |= ShiftDown(mask);
		KingBB[i] |= ShiftUpLeft(mask);
		KingBB[i] |= ShiftUpRight(mask);
		KingBB[i] |= ShiftDownLeft(mask);
		KingBB[i] |= ShiftDownRight(mask);
	}

	// Files and ranks
	quint64 rseed = 0xFF00000000000000ULL;
	quint64 fseed = 0x8080808080808080ULL;
	for (i = 7; i >= 0; --i) {
		ranks[i] = rseed;
		rseed >>= 8;
		files[i] = fseed;
		fseed >>= 1;
	}

	PawnPM[White] = ranks[7];
	PawnPM[Black] = ranks[0];
	// Now that global data has been calculated, we can create a start position
	standardPosition.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	clearedPosition.fromFen("8/8/8/8/8/8/8/8 w KQkq - 0 1");
}

