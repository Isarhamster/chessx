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

#include "movelist.h"
#include "bitboard.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

#include <QtCore>

// Global data that is initialized early on and only read afterward
quint64 bb_PawnAttacks[2][64];
quint64 bb_PawnF1[2][64];
quint64 bb_PawnF2[2][64];
quint64 bb_PawnALL[2][64];
quint64 bb_PromotionRank[2];
quint64 bb_KnightAttacks[64];
quint64 bb_R45Attacks[64][64];
quint64 bb_L45Attacks[64][64];
quint64 bb_KingAttacks[64];
quint64 bb_RankAttacks[64][64];
quint64 bb_FileAttacks[64][64];
quint64 bb_fileMask[8];
quint64 bb_rankMask[8];
quint64 bb_Mask[64];
quint64 bb_MaskL90[64];
quint64 bb_MaskL45[64];
quint64 bb_MaskR45[64];

BitBoard getStandardPosition();
// Calling the function getStandardPosition for initialization avoids
// initialization side effects when using BitBoards in other translation
// units.
BitBoard standardPosition = getStandardPosition();
BitBoard clearedPosition;


bool bitBoardInitRun;
void bitBoardInit();


const quint64 A1 = 1,     B1 = A1 << 1, C1 = B1 << 1, D1 = C1 << 1, E1 = D1 << 1, F1 = E1 << 1, G1 = F1 << 1, H1 = G1 << 1;
const quint64 A2 = H1 << 1, B2 = A2 << 1, C2 = B2 << 1, D2 = C2 << 1, E2 = D2 << 1, F2 = E2 << 1, G2 = F2 << 1, H2 = G2 << 1;
const quint64 A3 = H2 << 1, B3 = A3 << 1, C3 = B3 << 1, D3 = C3 << 1, E3 = D3 << 1, F3 = E3 << 1, G3 = F3 << 1, H3 = G3 << 1;
const quint64 A4 = H3 << 1, B4 = A4 << 1, C4 = B4 << 1, D4 = C4 << 1, E4 = D4 << 1, F4 = E4 << 1, G4 = F4 << 1, H4 = G4 << 1;
const quint64 A5 = H4 << 1, B5 = A5 << 1, C5 = B5 << 1, D5 = C5 << 1, E5 = D5 << 1, F5 = E5 << 1, G5 = F5 << 1, H5 = G5 << 1;
const quint64 A6 = H5 << 1, B6 = A6 << 1, C6 = B6 << 1, D6 = C6 << 1, E6 = D6 << 1, F6 = E6 << 1, G6 = F6 << 1, H6 = G6 << 1;
const quint64 A7 = H6 << 1, B7 = A7 << 1, C7 = B7 << 1, D7 = C7 << 1, E7 = D7 << 1, F7 = E7 << 1, G7 = F7 << 1, H7 = G7 << 1;
const quint64 A8 = H7 << 1, B8 = A8 << 1, C8 = B8 << 1, D8 = C8 << 1, E8 = D8 << 1, F8 = E8 << 1, G8 = F8 << 1, H8 = G8 << 1;

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

#define SetBitL90(s)      (bb_MaskL90[s])
#define SetBitL45(s)      (bb_MaskL45[s])
#define SetBitR45(s)      (bb_MaskR45[s])
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
#define SetBit(s)         (bb_Mask[s])
#define File(s)           ((s)&7)
#define Rank(s)           ((s)>>3)

// This C++ version is as fast as the assembly
inline uint getFirstBitAndClear64(quint64& bb)
{
    register quint64 x = bb & -(qint64)bb;
    bb ^= x;
#ifdef __GNUG__
    return 63-__builtin_clzll(x);
#elif _MSC_VER
    register uint r =  0;
    _BitScanReverse(&r, x);
    return r;
#else
    // SBE - After a fair bit of testing, this is the fastest portable version
    // i could come up with, it's about twice as fast as shift-testing 64 times.
    register uint r =  0;
    if (!(x & 0xffffffff)) { x >>= 32; r |= 32; }
    if (!(x & 0xffff)) { x >>= 16; r |= 16; }
    if (!(x & 0xff)) { x >>= 8; r |= 8; }
    if (!(x & 0xf)) { x >>= 4; r |= 4; }
    if (!(x & 0x3)) { x >>= 2; r |= 2; }
    if (!(x & 0x1)) { r |= 1; }
    return r;
#endif
}

/** Initialize a new bitboard, and ensure global data has been initialized */
BitBoard::BitBoard()
{
    memset(this, 0, sizeof(BitBoard));
    if (!bitBoardInitRun)
        bitBoardInit();
    m_epSquare = NoEPSquare;
}

bool BitBoard::isCheckmate() const
{
    MoveList moves(generateMoves());
    for (int i = 0; i < moves.size(); ++i)
        if (!isIntoCheck(moves[i]))
            return false;
    return isCheck();
}

bool BitBoard::isStalemate() const
{
    MoveList moves(generateMoves());
    for (int i = 0; i < moves.size(); ++i)
        if (!isIntoCheck(moves[i]))
            return false;
    return !isCheck();
}

void BitBoard::removeIllegal(const Move& move, quint64& b) const
{
    quint64 mask = 1;
    Move m = move;
    for (int sq = 0; sq < 64; ++sq) {
        if (b & mask) {
            m.setFrom(sq);
            if (isIntoCheck(m))
                b &= ~mask;
        }
        mask <<= 1;
    }
}

enum LangOutput { LO_EN, LO_DE, LO_FR, LO_PL, LO_ES };
/** Return the ASCII character for a given piece type */
inline char sanPiece(const int piece, int lang=0)
{
    switch (lang)
    {
    case LO_EN: return " KQRBN"[piece];
    case LO_DE: return " KDTLS"[piece];
    case LO_FR: return " RDTFC"[piece];
    case LO_PL: return " KHWGS"[piece];
    case LO_ES: return " RDTAC"[piece];
    default:    return " KQRBN"[piece];
    }
}

QString BitBoard::moveToSan(const Move& move) const
{
    QString san;
    int from = move.from();
    int to = move.to();
    bool isPawn = m_piece[from] == Pawn;

    if (move.isNullMove()) {
        san = "--";
        return san;
    }

    if (move.isCastling()) {
        if (from < to)
            san = "O-O";
        else	san = "O-O-O";
    } else {
        if (!isPawn) {
            san = sanPiece(m_piece[from]);

            // We may need disambiguation
            quint64 others = 0;
            switch (m_piece[from]) {
            case Knight:
                others = m_knights & knightAttacksFrom(to);
                break;
            case Bishop:
                others = m_bishops & bishopAttacksFrom(to);
                break;
            case Rook:
                others = m_rooks & rookAttacksFrom(to);
                break;
            case Queen:
                others = m_queens & queenAttacksFrom(to);
                break;
            case King:
                others = m_kings & kingAttacksFrom(to);
                break;
            default:
                break; // Something really wrong
            }

            others ^= SetBit(from);
            others &= m_occupied_co[m_stm];
            // Do not disambiguate with moves that put oneself in check.
            //    This is an expensive operation of dubious value, but people seem to want it
            if (others)
                removeIllegal(move, others);
            if (others) {
                bool row = false, column = false;
                if (others & bb_rankMask[Rank(from)])
                    column = true;
                if (others & bb_fileMask[File(from)])
                    row = true;
                else	column = true;
                if (column)
                    san += 'a' + File(from);
                if (row)
                    san += '1' + Rank(from);
            }
        }

        //capture x
        if (m_piece[to] || (move.isEnPassant())) {
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

void BitBoard::clear()
{
    *this = clearedPosition;
}

void BitBoard::setStandardPosition()
{
    *this = standardPosition;
}

void BitBoard::clearEnPassantSquare()
{
    m_epFile = 0;
    m_epSquare = NoEPSquare;
}

void BitBoard::setEnPassantSquare(const Square s)
{
    m_epSquare = s;
    m_epFile = File(s);
}

bool BitBoard::isMovable(const Square from) const
{
    Q_ASSERT(from < 64);

    if (m_occupied_co[m_stm] & SetBit(from)) {
        quint64 squares = 0;
        switch (m_piece[from]) {
        case Pawn:	squares = pawnMovesFrom(from); break;
        case Knight:	squares = knightAttacksFrom(from); break;
        case Bishop:	squares = bishopAttacksFrom(from); break;
        case Rook:	squares = rookAttacksFrom(from); break;
        case Queen:	squares = queenAttacksFrom(from); break;
        case King:	squares = kingAttacksFrom(m_ksq[m_stm]); break;
        default: break;
        }
        squares &= ~m_occupied_co[m_stm];
        while (squares) {
            Square to = getFirstBitAndClear64(squares);
            if (prepareMove(from, to).isLegal())
                return true;
        }
    }
    return false;
}

void BitBoard::setAt(const Square s, const Piece p)
{
    Q_ASSERT(s < 64);
    Q_ASSERT(isValidPiece(p) || p == Empty);

    quint64 bit = SetBit(s);
    if (m_occupied & bit)
        removeAt(s);

    PieceType pt = pieceType(p);
    if (pt == None)
        return;

    Color _color = pieceColor(p);
    ++m_pieceCount[_color];
    switch (pt) {
    case Pawn:	m_pawns   |= bit; ++m_pawnCount[_color]; break;
    case Knight:	m_knights |= bit; break;
    case Bishop:	m_bishops |= bit; break;
    case Rook:	m_rooks   |= bit; break;
    case Queen:	m_queens  |= bit; break;
    case King:	if (m_kings & m_occupied_co[_color])
            removeAt(m_ksq[_color]);
        m_kings   |= bit;
        m_ksq[_color] = s;
        break;
    default: break; // ERROR
    }

    m_piece[s] = pt;
    m_occupied ^= bit;
    m_occupied_co[_color] ^= bit;
    m_occupied_l90 ^= SetBitL90(s);
    m_occupied_l45 ^= SetBitL45(s);
    m_occupied_r45 ^= SetBitR45(s);
}

void BitBoard::removeAt(const Square s)
{
    Q_ASSERT(s < 64);

    quint64 bit = SetBit(s);
    if (!(m_occupied & bit))
        return;

    Color _color = m_occupied_co[White] & bit ? White : Black;
    --m_pieceCount[_color];
    switch (m_piece[s]) {
    case Pawn:	m_pawns   ^= bit; --m_pawnCount[_color]; break;
    case Knight:	m_knights ^= bit; break;
    case Bishop:	m_bishops ^= bit; break;
    case Rook:	m_rooks   ^= bit;
        m_castle &= Castle[s];
        break;
    case Queen:	m_queens  ^= bit; break;
    case King:	m_kings   ^= bit;
        m_ksq[_color] = InvalidSquare;
        destroyCastle(_color);
        break;
    default: break; // ERROR
    }

    m_piece[s] = Empty;
    m_occupied ^= bit;
    m_occupied_co[_color] ^= bit;
    m_occupied_l90 ^= SetBitL90(s);
    m_occupied_l45 ^= SetBitL45(s);
    m_occupied_r45 ^= SetBitR45(s);
}

bool BitBoard::isValidFen(const QString& fen) const
{
    return BitBoard().fromGoodFen(fen);
}

bool BitBoard::fromFen(const QString& fen)
{
    if (isValidFen(fen))
        return fromGoodFen(fen);
    return false;
}

BoardStatus BitBoard::validate() const
{
    uint wk = 0, bk = 0, wp = 0, bp = 0, bo = 0, wo = 0;
    for (uint i = 0; i < 64; ++i) {
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

    Q_ASSERT(wp == m_pawnCount[White]);
    Q_ASSERT(bp == m_pawnCount[Black]);
    Q_ASSERT(wp + wk + wo == m_pieceCount[White]);
    Q_ASSERT(bp + bk + bo == m_pieceCount[Black]);

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
    bool check =  isAttackedBy(m_stm ^ 1, m_ksq[m_stm]);
    bool check2 = isAttackedBy(m_stm, m_ksq[m_stm^1]);
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
    if (m_epSquare != NoEPSquare) {
        if (m_stm == White && (m_epSquare < a6 || m_epSquare > h6))
            return InvalidEnPassant;
        if (m_stm == Black && (m_epSquare < a3 || m_epSquare > h3))
            return InvalidEnPassant;
        // According to PGN standard, ep should be set even if no capture is possible
        /*
        if (m_occupied & SetBit(m_epSquare))
            return InvalidEnPassant;
        if (m_occupied & bb_PawnF1[m_stm][m_epSquare])
            return InvalidEnPassant;
        if (!(bb_PawnAttacks[m_stm^1][m_epSquare] & m_pawns & m_occupied_co[m_stm]))
            return InvalidEnPassant;
        if (!(bb_PawnF1[m_stm^1][m_epSquare] & m_pawns & m_occupied_co[m_stm^1]))
            return InvalidEnPassant;
            */
    }

    // Don't allow triple(or more) checks.
    // FIXME -- need code here to return MultiCheck

    return Valid;
}


// Why QString throws asserts for access past end of string and
// refuses to return a real c++ char type is beyond me...
class SaneString : public QString
{
public:
    SaneString(const QString& q) : QString(q) {};
    char operator[](const int index) const {
        if (index < this->length())
            return QString::operator[](index).toLatin1();
        return 0;
    }
};

bool BitBoard::fromGoodFen(const QString& qfen)
{
    SaneString fen(qfen);
    int i;
    uint s;
    char c = fen[0];

    memset(this, 0, sizeof(BitBoard));
    m_moveNumber = 1;
    m_epSquare = NoEPSquare;

    // Piece position
    i = 0;
    s = 56;
    while (c && c != ' ' && s <= 64) {
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
            m_piece[s] = Pawn;
            m_pawns |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            ++m_pawnCount[Black];
            ++m_pieceCount[Black];
            s++;
            break;
        case 'n':
            m_piece[s] = Knight;
            m_knights |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            ++m_pieceCount[Black];
            s++;
            break;
        case 'b':
            m_piece[s] = Bishop;
            m_bishops |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            ++m_pieceCount[Black];
            s++;
            break;
        case 'r':
            m_piece[s] = Rook;
            m_rooks |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            ++m_pieceCount[Black];
            s++;
            break;
        case 'q':
            m_piece[s] = Queen;
            m_queens |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            ++m_pieceCount[Black];
            s++;
            break;
        case 'k':
            m_piece[s] = King;
            m_kings |= SetBit(s);
            m_occupied_co[Black] |= SetBit(s);
            m_ksq[Black] = s;
            ++m_pieceCount[Black];
            s++;
            break;
        case 'P':
            m_piece[s] = Pawn;
            m_pawns |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            ++m_pawnCount[White];
            ++m_pieceCount[White];
            s++;
            break;
        case 'N':
            m_piece[s] = Knight;
            m_knights |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            ++m_pieceCount[White];
            s++;
            break;
        case 'B':
            m_piece[s] = Bishop;
            m_bishops |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            ++m_pieceCount[White];
            s++;
            break;
        case 'R':
            m_piece[s] = Rook;
            m_rooks |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            ++m_pieceCount[White];
            s++;
            break;
        case 'Q':
            m_piece[s] = Queen;
            m_queens |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            ++m_pieceCount[White];
            s++;
            break;
        case 'K':
            m_piece[s] = King;
            m_kings |= SetBit(s);
            m_occupied_co[White] |= SetBit(s);
            m_ksq[White] = s;
            ++m_pieceCount[White];
            s++;
            break;
        default:
            return false;
        }
        c = fen[++i];
    }
    if (s != 8)
        return false;

    // Set remainder of bitboard data appropriately
    m_occupied = m_occupied_co[White] + m_occupied_co[Black];
    for (int i = 0; i < 64; ++i) {
        if (SetBit(i)&m_occupied) {
            m_occupied_l90 |= SetBitL90(i);
            m_occupied_l45 |= SetBitL45(i);
            m_occupied_r45 |= SetBitR45(i);
        }
    }

    // Side to move
    c = fen[++i];
    if (c == 'w') m_stm = White;
    else if (c == 'b') m_stm = Black;
    else if (c == 0) return true;
    else return false;
    c = fen[++i];

    // Castling Rights
    c = fen[++i];
    if (c == 0) return true;
    if (c != '-') {
        while (c != ' ') {
            switch (c)
            {
            case 'K': setCastleShort(White); break;
            case 'Q': setCastleLong(White); break;
            case 'k': setCastleShort(Black); break;
            case 'q': setCastleLong(Black); break;
            default:  return false;
            }

            c = fen[++i];
        }
    } else	++i;  // Bypass space

    // EnPassant Square
    c = fen[++i];
    if (c == 0) return true;
    if (c != '-') {
        bool epgood = true;
        if (c >= 'a' && c <= 'h')
            m_epFile = c - 'a' + 1;
        else if (c >= 'A' && c <= 'H')
            m_epFile = c - 'A' + 1;
        else if (c < '0' || c > '9')
            return false;
        else epgood = false;
        if (epgood)
        {
            c = fen[++i];
            if (m_stm == White && c != '6')
                return false;
            else if (m_stm == Black && c != '3')
                return false;
        }
    }
    epFile2Square();
    c = fen[++i];

    // Half move clock
    c = fen[++i];
    if (c == 0)
        return true;
    if (c < '0' || c > '9')
        return false;
    int j = i;
    while (c >= '0' && c <= '9')
        c = fen[++i];
    m_halfMoves = fen.mid(j, i - j).toInt();

    // Move number
    c = fen[++i];
    if (c == 0)
        return true;
    if (c < '0' || c > '9')
        return false;
    m_moveNumber = fen.mid(i).toInt();
    while (c >= '0' && c <= '9')
        c = fen[++i];

    if( m_moveNumber <= 0)
    {
        // Silently ignore illegal movenumber
        m_moveNumber = 1;
    }

    return true;
}

MoveList BitBoard::generateMoves() const
{
    register uint from, to;
    quint64 moves, movers;

    MoveList p;

    if (m_stm == White) {

        // castle moves
        if (canCastle(White)) {
            if (canCastleShort(White) && !((F1 | G1)&m_occupied))
                if (!isAttackedBy(Black, e1) &&
                        !isAttackedBy(Black, f1)
                        && !isAttackedBy(Black, g1))
                    p.add().genWhiteOO();
            if (canCastleLong(White)  && !((B1 | C1 | D1)&m_occupied))
                if (!isAttackedBy(Black, c1) &&
                        !isAttackedBy(Black, d1)
                        && !isAttackedBy(Black, e1))
                    p.add().genWhiteOOO();
        }

        // pawn en passant moves
        movers = m_pawns & m_occupied_co[White];
        if (m_epSquare != NoEPSquare) {
            moves = bb_PawnAttacks[Black][m_epSquare] & movers;
            while (moves) {
                from = getFirstBitAndClear64(moves);
                p.add().genEnPassant(from, m_epSquare);
            }
        }

        // pawn captures
        moves = ShiftUpRight(movers) & m_occupied_co[Black];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 7) {
                p.add().genPawnMove(to - 9, to, m_piece[to]);
            } else {
                p.add().genCapturePromote(to - 9, to, Queen, m_piece[to]);
                p.add().genCapturePromote(to - 9, to, Knight, m_piece[to]);
                p.add().genCapturePromote(to - 9, to, Rook, m_piece[to]);
                p.add().genCapturePromote(to - 9, to, Bishop, m_piece[to]);
            }
        }
        moves = ShiftUpLeft(movers) & m_occupied_co[Black];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 7) {
                p.add().genPawnMove(to - 7, to, m_piece[to]);
            } else {
                p.add().genCapturePromote(to - 7, to, Queen, m_piece[to]);
                p.add().genCapturePromote(to - 7, to, Knight, m_piece[to]);
                p.add().genCapturePromote(to - 7, to, Rook, m_piece[to]);
                p.add().genCapturePromote(to - 7, to, Bishop, m_piece[to]);
            }
        }

        // pawns 1 forward
        moves = ShiftUp(movers) & ~m_occupied;
        movers = moves;
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 7) {
                p.add().genOneForward(to - 8, to);
            } else {
                p.add().genPromote(to - 8, to, Queen);
                p.add().genPromote(to - 8, to, Knight);
                p.add().genPromote(to - 8, to, Rook);
                p.add().genPromote(to - 8, to, Bishop);
            }
        }
        // pawns 2 forward
        moves = ShiftUp(movers) & rank4 & ~m_occupied;
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genTwoForward(to - 16, to);
        }

    } else {

        // castle moves
        if (canCastle(Black)) {
            if (canCastleShort(Black) && !((F8 | G8)&m_occupied))
                if (!isAttackedBy(White, e8) &&
                        !isAttackedBy(White, f8) &&
                        !isAttackedBy(White, g8))
                    p.add().genBlackOO();
            if (canCastleLong(Black)  && !((B8 | C8 | D8)&m_occupied))
                if (!isAttackedBy(White, e8) &&
                        !isAttackedBy(White, d8) &&
                        !isAttackedBy(White, c8))
                    p.add().genBlackOOO();
        }

        // pawn en passant moves
        movers = m_pawns & m_occupied_co[Black];
        if (m_epSquare != NoEPSquare) {
            moves = bb_PawnAttacks[White][m_epSquare] & movers;
            while (moves) {
                from = getFirstBitAndClear64(moves);
                p.add().genEnPassant(from, m_epSquare);
            }
        }

        // pawn captures
        moves = ShiftDownLeft(movers) & m_occupied_co[White];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 0) {
                p.add().genPawnMove(to + 9, to, m_piece[to]);
            } else {
                p.add().genCapturePromote(to + 9, to, Queen, m_piece[to]);
                p.add().genCapturePromote(to + 9, to, Knight, m_piece[to]);
                p.add().genCapturePromote(to + 9, to, Rook, m_piece[to]);
                p.add().genCapturePromote(to + 9, to, Bishop, m_piece[to]);
            }
        }
        moves = ShiftDownRight(movers) & m_occupied_co[White];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 0) {
                p.add().genPawnMove(to + 7, to, m_piece[to]);
            } else {
                p.add().genCapturePromote(to + 7, to, Queen, m_piece[to]);
                p.add().genCapturePromote(to + 7, to, Knight, m_piece[to]);
                p.add().genCapturePromote(to + 7, to, Rook, m_piece[to]);
                p.add().genCapturePromote(to + 7, to, Bishop, m_piece[to]);
            }
        }

        // pawns 1 forward
        moves = ShiftDown(movers) & ~m_occupied;
        movers = moves;
        while (moves) {
            to = getFirstBitAndClear64(moves);
            if (Rank(to) != 0) {
                p.add().genOneForward(to + 8, to);
            } else {
                p.add().genPromote(to + 8, to, Queen);
                p.add().genPromote(to + 8, to, Knight);
                p.add().genPromote(to + 8, to, Rook);
                p.add().genPromote(to + 8, to, Bishop);
            }
        }
        // pawns 2 forward
        moves = ShiftDown(movers) & rank5 & ~m_occupied;
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genTwoForward(to + 16, to);
        }
    }

    // knight moves
    movers = m_knights & m_occupied_co[m_stm];
    while (movers) {
        from = getFirstBitAndClear64(movers);
        moves = knightAttacksFrom(from) & ~m_occupied_co[m_stm];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genKnightMove(from, to, m_piece[to]);
        }
    }
    // bishop moves
    movers = m_bishops & m_occupied_co[m_stm];
    while (movers) {
        from = getFirstBitAndClear64(movers);
        moves = bishopAttacksFrom(from) & ~m_occupied_co[m_stm];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genBishopMove(from, to, m_piece[to]);
        }
    }
    // rook moves
    movers = m_rooks & m_occupied_co[m_stm];
    while (movers) {
        from = getFirstBitAndClear64(movers);
        moves = rookAttacksFrom(from) & ~m_occupied_co[m_stm];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genRookMove(from, to, m_piece[to]);
        }
    }
    // queen moves
    movers = m_queens & m_occupied_co[m_stm];
    while (movers) {
        from = getFirstBitAndClear64(movers);
        moves = queenAttacksFrom(from) & ~m_occupied_co[m_stm];
        while (moves) {
            to = getFirstBitAndClear64(moves);
            p.add().genQueenMove(from, to, m_piece[to]);
        }
    }
    // king moves
    moves = kingAttacksFrom(m_ksq[m_stm]) & ~m_occupied_co[m_stm];
    while (moves) {
        to = getFirstBitAndClear64(moves);
        if (!isAttackedBy(m_stm ^ 1, to))
            p.add().genKingMove(m_ksq[m_stm], to, m_piece[to]);
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

inline bool isFile(const char c) {return c >= 'a' && c <= 'h';}
inline bool isRank(const char c) {return c >= '1' && c <= '8';}

// Create a null move
// A Null Move is a move by a king to its own square
// and is represented in pgn by a "--" although illegal
// it is often used in ebooks to annotate ideas
Move BitBoard::nullMove() const
{
    Square kingSquare = m_ksq[m_stm];
    Move m = prepareMove( kingSquare, kingSquare);
    if (m_stm == Black)
        m.setBlack();

    return m;
}

Move BitBoard::parseMove(const QString& algebraic) const
{
    const QByteArray& bs(algebraic.toLatin1());
    const char *san = bs.constData();
    const char* s = san;
    register char c = *(s++);
    quint64 match;
    int fromSquare = -1;
    int toSquare = -1;
    int fromFile = -1;
    int fromRank = -1;
    uchar promotePiece = Empty;
    Move move;
    uint type;

    // Castling
    if (c == 'o' || c == 'O' || c == '0') {
        if (strncmp(san, "o-o-o", 5) == 0 || strncmp(san, "O-O-O", 5) == 0 || strncmp(san, "0-0-0", 5)  == 0) {
            if (m_stm == White)
                return prepareMove(e1, c1);
            else	return prepareMove(e8, c8);
        } else if (strncmp(san, "o-o", 3) == 0 || strncmp(san, "O-O", 3) == 0 || strncmp(san, "0-0", 3)  == 0) {
            if (m_stm == White)
                return prepareMove(e1, g1);
            else	return prepareMove(e8, g8);
        }
        return move;
    }

    // Null Move
    if (c == '-') {
        if (strncmp(san, "--", 2) == 0) {
            return nullMove();
        }
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
        if (c == '=' || c == '(')
            c = *(s++);
        switch (toupper(c)) {
        case 'Q': promotePiece = Queen; break;
        case 'R': promotePiece = Rook; break;
        case 'B': promotePiece = Bishop; break;
        case 'N': promotePiece = Knight; break;
        default: ;//return move;
        }
        if (fromSquare < 0) {
            int base = (m_stm == White ? -8 : 8);
            if (fromFile < 0) {
                fromSquare = toSquare + base;
                quint64 bit = SetBit(fromSquare);
                if (!(m_occupied_co[m_stm] & bit))
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

    if (fromSquare < 0) {
        switch (type) {
        case Queen: match = queenAttacksFrom(toSquare) & m_queens; break;
        case Rook: match = rookAttacksFrom(toSquare) & m_rooks; break;
        case Bishop: match = bishopAttacksFrom(toSquare) & m_bishops; break;
        case Knight: match = knightAttacksFrom(toSquare) & m_knights; break;
        case King: match = kingAttacksFrom(toSquare) & m_kings; break;
        default:   return move;
        }

        match &= m_occupied_co[m_stm];
        if (fromRank >= 0)
            match &= bb_rankMask[fromRank];
        else if (fromFile >= 0)
            match &= bb_fileMask[fromFile];

        if (match)
            fromSquare = getFirstBitAndClear64(match);
        else	return move;

        // If not yet fully disambiguated, all but one move must be illegal
        //  Cycle through them, and pick the first legal move.
        while (match) {
            if (prepareMove(fromSquare, toSquare).isLegal())
                break;
            fromSquare = getFirstBitAndClear64(match);
        }
    }

    if (type != m_piece[fromSquare])
        return move;
    return prepareMove(fromSquare, toSquare);
}

bool BitBoard::doMove(const Move& m)
{
    register uint from = m.from();
    register uint to = m.to();
    register uint sntm = m_stm ^ 1; // side not to move
    register quint64 bb_from = SetBit(from);
    register quint64 bb_to = SetBit(to);
    uint rook_from = 0, rook_to = 0;

    m_epFile = 0;
    m_halfMoves++;	// Number of moves since last capture or pawn move

    uint action = m.action();
    switch (action) {
    case Pawn:
        m_halfMoves = 0;
        m_pawns ^= bb_from ^ bb_to;
        m_piece[to] = Pawn;
        break;
    case Knight:
        m_knights ^= bb_from ^ bb_to;
        m_piece[to] = Knight;
        break;
    case Bishop:
        m_bishops ^= bb_from ^ bb_to;
        m_piece[to] = Bishop;
        break;
    case Rook:
        m_rooks ^= bb_from ^ bb_to;
        m_piece[to] = Rook;
        if (canCastle(m_stm)) // a rook is moving, destroy castle flags if needed
            m_castle &= Castle[from]; // question is if always doing the table is just faster?
        break;
    case Queen:
        m_queens ^= bb_from ^ bb_to;
        m_piece[to] = Queen;
        break;
    case King:
        if (! m.isNullMove())
        {
            m_kings ^= bb_from ^ bb_to;
            m_ksq[m_stm] = to;
            m_piece[to] = King;
            destroyCastle(m_stm);	// king is moving so definitely destroy castle stuff!
        }
        break;
    case Move::CASTLE:
        m_kings ^= bb_from ^ bb_to;
        m_ksq[m_stm] = to;
        m_piece[to] = King;
        destroyCastle(m_stm);
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
        m_piece[rook_from] = Empty;
        m_piece[rook_to] = Rook;
        m_rooks ^= SetBit(rook_from) ^ SetBit(rook_to);
        m_occupied_co[m_stm] ^= SetBit(rook_from) ^ SetBit(rook_to);
        m_occupied_l90 ^= SetBitL90(rook_from) ^ SetBitL90(rook_to);
        m_occupied_l45 ^= SetBitL45(rook_from) ^ SetBitL45(rook_to);
        m_occupied_r45 ^= SetBitR45(rook_from) ^ SetBitR45(rook_to);
        break;
    case Move::TWOFORWARD:
        m_halfMoves = 0;
        m_pawns ^= bb_from ^ bb_to;
        m_piece[to] = Pawn;
        // According to PGN standard, ep square should be always set.
        m_epFile = File(to) + 1;
        break;
    case Move::PROMOTE:
        m_halfMoves = 0;
        m_pawns ^= bb_from;
        --m_pawnCount[m_stm];
        switch (m.promoted()) {
        case Knight:
            m_knights ^= bb_to;
            m_piece[to] = Knight;
            break;
        case Bishop:
            m_bishops ^= bb_to;
            m_piece[to] = Bishop;
            break;
        case Rook:
            m_rooks   ^= bb_to;
            m_piece[to] = Rook;
            break;
        case Queen:
            m_queens  ^= bb_to;
            m_piece[to] = Queen;
            break;
        default:  // can't promote to other piece types;
            break;
        }
        break;
    }

    switch (m.removal()) {
    case Empty:
        m_occupied_l90 ^= SetBitL90(to);     // extra cleanup needed for non-captures
        m_occupied_l45 ^= SetBitL45(to);
        m_occupied_r45 ^= SetBitR45(to);
        break;
    case Pawn:
        --m_pawnCount[sntm];
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        m_pawns ^= bb_to;
        m_occupied_co[sntm] ^= bb_to;
        break;
    case Knight:
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        m_knights ^= bb_to;
        m_occupied_co[sntm] ^= bb_to;
        break;
    case Bishop:
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        m_bishops ^= bb_to;
        m_occupied_co[sntm] ^= bb_to;
        break;
    case Rook:
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        m_rooks ^= bb_to;
        m_occupied_co[sntm] ^= bb_to;
        if (canCastle(sntm))
            m_castle &= Castle[to];
        break;
    case Queen:
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        m_queens ^= bb_to;
        m_occupied_co[sntm] ^= bb_to;
        break;
    case Move::ENPASSANT:
        --m_pawnCount[sntm];
        --m_pieceCount[sntm];
        m_halfMoves = 0;
        uint epsq = to + (m_stm == White ? -8 : 8);  // annoying move, the capture is not on the 'to' square
        m_piece[epsq] = Empty;
        m_pawns ^= SetBit(epsq);
        m_occupied_co[sntm] ^= SetBit(epsq);
        m_occupied_l90 ^= SetBitL90(to) ^ SetBitL90(epsq);
        m_occupied_l45 ^= SetBitL45(to) ^ SetBitL45(epsq);
        m_occupied_r45 ^= SetBitR45(to) ^ SetBitR45(epsq);
        break;
    }  // ...no I did not forget the king :)

    if(!m.isNullMove()) {
        m_piece[from] = Empty;
        m_occupied_co[m_stm] ^= bb_from ^ bb_to;
        m_occupied_l90 ^= SetBitL90(from);
        m_occupied_l45 ^= SetBitL45(from);
        m_occupied_r45 ^= SetBitR45(from);
        m_occupied = m_occupied_co[White] + m_occupied_co[Black];
    }

    if (m_stm == Black)
        ++m_moveNumber;

    m_stm ^= 1;	// toggle side to move
    epFile2Square();
    return true;
}

void BitBoard::undoMove(const Move& m)
{
    register uint from = m.from();
    register uint to = m.to();
    register uint sntm = m_stm ^ 1; // side not to move
    register quint64 bb_from = SetBit(from);
    register quint64 bb_to = SetBit(to);
    uint rook_from = 0, rook_to = 0; // =0 just to quiet compiler warnings

    uint action = m.action();
    switch (action) {
    case Pawn:
    case Move::TWOFORWARD:
        m_pawns ^= bb_from ^ bb_to;
        m_piece[from] = Pawn;
        break;
    case Knight:
        m_knights ^= bb_from ^ bb_to;
        m_piece[from] = Knight;
        break;
    case Bishop:
        m_bishops ^= bb_from ^ bb_to;
        m_piece[from] = Bishop;
        break;
    case Rook:
        m_rooks ^= bb_from ^ bb_to;
        m_piece[from] = Rook;
        break;
    case Queen:
        m_queens ^= bb_from ^ bb_to;
        m_piece[from] = Queen;
        break;
    case King:
        if(!m.isNullMove()) {
            m_kings ^= bb_from ^ bb_to;
            m_ksq[sntm] = from;
            m_piece[from] = King;
        }
        break;
    case Move::CASTLE:
        m_kings ^= bb_from ^ bb_to;
        m_ksq[sntm] = from;
        m_piece[from] = King;
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
        m_piece[rook_to] = Empty;
        m_piece[rook_from] = Rook;
        m_rooks ^= SetBit(rook_from) ^ SetBit(rook_to);
        m_occupied_co[sntm] ^= SetBit(rook_from) ^ SetBit(rook_to);
        m_occupied_l90 ^= SetBitL90(rook_from) ^ SetBitL90(rook_to);
        m_occupied_l45 ^= SetBitL45(rook_from) ^ SetBitL45(rook_to);
        m_occupied_r45 ^= SetBitR45(rook_from) ^ SetBitR45(rook_to);
        break;
    case Move::PROMOTE:
        m_pawns ^= bb_from;
        m_piece[from] = Pawn;
        ++m_pawnCount[sntm];
        switch (m.promoted()) {
        case Knight:
            m_knights ^= bb_to;
            break;
        case Bishop:
            m_bishops ^= bb_to;
            break;
        case Rook:
            m_rooks   ^= bb_to;
            break;
        case Queen:
            m_queens  ^= bb_to;
            break;
        default:  // can't promote to other piece types;
            break;
        }
        break;
    }

    uint replace = m.capturedType();
    switch (m.removal()) {  // Reverse captures
    case Empty:
        m_occupied_l90 ^= SetBitL90(to);     // extra cleanup needed for non-captures
        m_occupied_l45 ^= SetBitL45(to);
        m_occupied_r45 ^= SetBitR45(to);
        break;
    case Pawn:
        ++m_pawnCount[m_stm];
        ++m_pieceCount[m_stm];
        m_pawns ^= bb_to;
        m_occupied_co[m_stm] ^= bb_to;
        break;
    case Knight:
        ++m_pieceCount[m_stm];
        m_knights ^= bb_to;
        m_occupied_co[m_stm] ^= bb_to;
        break;
    case Bishop:
        ++m_pieceCount[m_stm];
        m_bishops ^= bb_to;
        m_occupied_co[m_stm] ^= bb_to;
        break;
    case Rook:
        ++m_pieceCount[m_stm];
        m_rooks ^= bb_to;
        m_occupied_co[m_stm] ^= bb_to;
        break;
    case Queen:
        ++m_pieceCount[m_stm];
        m_queens ^= bb_to;
        m_occupied_co[m_stm] ^= bb_to;
        break;
    case Move::ENPASSANT:
        ++m_pawnCount[m_stm];
        ++m_pieceCount[m_stm];
        replace = Empty;
        uint epsq = to + (sntm == White ? -8 : 8);  // annoying move, the capture is not on the 'to' square
        m_piece[epsq] = Pawn;
        m_pawns ^= SetBit(epsq);
        m_occupied_co[m_stm] ^= SetBit(epsq);
        m_occupied_l90 ^= SetBitL90(to) ^ SetBitL90(epsq);
        m_occupied_l45 ^= SetBitL45(to) ^ SetBitL45(epsq);
        m_occupied_r45 ^= SetBitR45(to) ^ SetBitR45(epsq);
        break;
    }  // ...no I did not forget the king :)

    if(!m.isNullMove()) {

        m_piece[to] = replace;
        m_occupied_co[sntm] ^= bb_from ^ bb_to;
        m_occupied_l90 ^= SetBitL90(from);
        m_occupied_l45 ^= SetBitL45(from);
        m_occupied_r45 ^= SetBitR45(from);
        m_occupied = m_occupied_co[White] + m_occupied_co[Black];
    }

    m_stm ^= 1;	// toggle side to move

    if (m_stm == Black)
        --m_moveNumber;

    m_halfMoves = m.u & 0xFF;
    m_castle = (m.u >> 8) & 0xF;
    m_epFile = (m.u >> 12) & 0xF;
    epFile2Square();
}

quint64 BitBoard::pawnMovesFrom(const Square s) const
{
    quint64 targets = bb_PawnF1[m_stm][s] & ~m_occupied;
    if (targets)
        targets |= bb_PawnF2[m_stm][s] & ~m_occupied;
    if (m_epSquare == NoEPSquare)
        targets |= bb_PawnAttacks[m_stm][s] & m_occupied_co[m_stm^1];
    else	targets |= bb_PawnAttacks[m_stm][s] & (m_occupied_co[m_stm^1] | SetBit(m_epSquare));
    return targets;
}

Move BitBoard::prepareMove(const Square& from, const Square& to) const
{
    Q_ASSERT(from < 64);
    Q_ASSERT(to < 64);

    quint64 src = SetBit(from);
    quint64 dest = SetBit(to);
    Move move(from, to);
    uchar p = m_piece[from];

    // Check for Illegal Move
    // first the source square must not be vacant
    if (!(m_occupied_co[m_stm] & src))
        return move;

    // Check for Illegal Move
    // If the destination square is a piece of the moving color
    if (m_occupied_co[m_stm] & dest)
    {
        // If Not a null move
        if( (src != dest) || p != King )
        {
            // move is neither legal nor a null move
            return move;
        }
    }

    move.setPieceType(p);
    uchar pCaptured = m_piece[to];
    move.setCaptureType(pCaptured);
    if (pCaptured == King)
    {
        // This test became necessary with Null-Moves
        if (m_stm == Black)
            move.setBlack();
        move.u = m_halfMoves;
        move.u |= (((ushort) m_castle & 0xF) << 8);
        move.u |= (((ushort) m_epFile & 0xF) << 12);

        return move;
    }
    if (p == King) {
        // if not a null Move
        if( src != dest )
        {
            if (!(kingAttacksFrom(to) & src) && !prepareCastle(move))
                return move;
        }
    } else if (p == Pawn) {
        if (!(pawnMovesFrom(from) & dest))
            return move;
        else if (to == m_epSquare)
            move.setEnPassant();
        else if (dest & bb_PawnF2[m_stm][from])
            move.setTwoForward();
        else if (dest & bb_PromotionRank[m_stm])
            move.setPromoted(Queen);
    } else {
        quint64 reach = 0;
        if (p == Queen)		reach = queenAttacksFrom(to);
        else if (p == Rook)	reach = rookAttacksFrom(to);
        else if (p == Bishop)	reach = bishopAttacksFrom(to);
        else if (p == Knight)	reach = knightAttacksFrom(to);
        if (!(reach & src))
            return move;
    }

    BitBoard peek(*this);
    peek.doMove(move);
    peek.swapToMove();
    if (peek.isCheck()) { // Don't allow move into check even if its a null move
        return move;
    }

    if (m_stm == Black)
        move.setBlack();
    move.u = m_halfMoves;
    move.u |= (((ushort) m_castle & 0xF) << 8);
    move.u |= (((ushort) m_epFile & 0xF) << 12);
    if( src != dest)
    {
        move.setLegalMove();
    }
    return move;
}

bool BitBoard::prepareCastle(Move& move) const
{
    if (!canCastle(m_stm))
        return false;

    Square to = move.to();
    if (m_stm == White) {
        if (to == g1 && canCastleShort(White) && !((F1 | G1)&m_occupied))
            if (!isAttackedBy(Black, e1) && !isAttackedBy(Black, f1)) {
                move.genWhiteOO();
                return true;
            }
        if (to == c1 && canCastleLong(White) && !((B1 | C1 | D1)&m_occupied))
            if (!isAttackedBy(Black, e1) && !isAttackedBy(Black, d1)) {
                move.genWhiteOOO();
                return true;
            }
    } else {
        if (to == g8 && canCastleShort(Black) && !((F8 | G8)&m_occupied))
            if (!isAttackedBy(White, e8) && !isAttackedBy(White, f8)) {
                move.genBlackOO();
                return true;
            }
        if (to == c8 && canCastleLong(Black) && !((B8 | C8 | D8)&m_occupied))
            if (!isAttackedBy(White, e8) && !isAttackedBy(White, d8)) {
                move.genBlackOOO();
                return true;
            }
    }

    return false;
}

bool BitBoard::canBeReachedFrom(const BitBoard& target) const
{
    if (m_pieceCount[White] > target.m_pieceCount[White] ||
            m_pieceCount[Black] > target.m_pieceCount[Black] ||
            m_pawnCount[White] > target.m_pawnCount[White] ||
            m_pawnCount[Black] > target.m_pawnCount[Black])
        return false;

    return true;
}

Piece BitBoard::pieceAt(Square s) const
{
    Q_ASSERT(s < 64);
    quint64 bit = SetBit(s);
    if (m_occupied & bit) {
        if (m_occupied_co[White] & bit)
            return Piece(m_piece[s]);
        else	return Piece(m_piece[s] + 6);
    }
    return Empty;
}

/** Return ASCII character for given piece to be used in FEN */
inline QChar pieceToChar(const Piece piece)
{
    return piece > BlackPawn ? '?' : " KQRBNPkqrbnp"[piece];
};

/** Return ASCII character for given piece to be used in human FEN */
inline QChar pieceToHumanChar(const Piece piece)
{
    return piece > BlackPawn ? '?' : " KQRBNPKQRBNP"[piece];
};

QString BitBoard::toFen() const
{
    QString fen = "";
    Piece piece;
    int empty = 0;

    //piece placement
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; ++col) {
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
    fen += m_stm == White ? " w " : " b ";

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
    if (m_epSquare == NoEPSquare) {
        fen += "- ";
    } else {
        fen += 'a' + (m_epSquare & 7);
        fen += '1' + ((m_epSquare & 56) >> 3);
        fen += ' ';
    }

    // half move clock
    fen += QString::number(halfMoveClock());

    // move number
    fen += m_moveNumber <= 0 ? " 1" : " " + QString::number(m_moveNumber);

    return fen;
}

QString BitBoard::toHumanFen() const
{
    QString fenFormat = QCoreApplication::translate("BitBoard","w%1\nb%2\n%3 to move.");
    QMap<Piece,QStringList> charLists;

    //piece placement
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = pieceAt(8 * row + col);
            if (piece != Empty)
            {
                charLists[piece].append(QString("%1%2").arg(QChar('a'+col)).arg(row+1));
            }
        }
    }

    //side to move
    QString toMove = (m_stm == White) ? QCoreApplication::translate("BitBoard","White") : QCoreApplication::translate("BitBoard","Black");

    QString w, b;
    for (Piece p=WhiteKing; p<=WhitePawn; ++p)
    {
        if (charLists.keys().contains(p))
        {
            if (!w.isEmpty()) w+=",";
            w.append(pieceToHumanChar(p));
            w.append(charLists[p].join(","));
        }
    }
    for (Piece p=BlackKing; p!=Empty; ++p)
    {
        if (charLists.keys().contains(p))
        {
            if (!b.isEmpty()) b+=",";
            b.append(pieceToHumanChar(p));
            b.append(charLists[p].join(","));
        }
    }

    QString fen=fenFormat.arg(w).arg(b).arg(toMove);

    return fen;
}

/** Calculate global bit board values before starting */
void bitBoardInit()
{
    bitBoardInitRun = true;
    int i, q;
    quint64 mask;

    // Square masks
    mask = 1;
    for (i = 0;i < 64;++i) {
        bb_Mask[i] = mask << i;
    }
    for (i = 0;i < 64;++i) {
        bb_MaskL90[i] = SetBit(RotateL90[i]);
        bb_MaskL45[i] = SetBit(RotateL45[i]);
        bb_MaskR45[i] = SetBit(RotateR45[i]);
    }

    // Pawn moves and attacks
    for (i = 0;i < 64;++i) {
        mask = SetBit(i);
        bb_PawnAttacks[White][i]  = ShiftUpLeft(mask);
        bb_PawnAttacks[White][i] |= ShiftUpRight(mask);

        bb_PawnAttacks[Black][i]  = ShiftDownLeft(mask);
        bb_PawnAttacks[Black][i] |= ShiftDownRight(mask);

        bb_PawnF1[White][i]  = ShiftUp(mask);
        bb_PawnF2[White][i]  = Shift2Up(mask) & rank4;

        bb_PawnF1[Black][i]  = ShiftDown(mask);
        bb_PawnF2[Black][i]  = Shift2Down(mask) & rank5;

        bb_PawnALL[White][i] = bb_PawnAttacks[White][i] |
                bb_PawnF1[White][i] |
                bb_PawnF2[White][i];

        bb_PawnALL[Black][i] = bb_PawnAttacks[Black][i] |
                bb_PawnF1[Black][i] |
                bb_PawnF2[Black][i];
    }

    // Knight attacks
    for (i = 0;i < 64;++i) {
        mask = SetBit(i);
        bb_KnightAttacks[i]  = ShiftLeft(Shift2Up(mask));
        bb_KnightAttacks[i] |= ShiftRight(Shift2Up(mask));
        bb_KnightAttacks[i] |= ShiftLeft(Shift2Down(mask));
        bb_KnightAttacks[i] |= ShiftRight(Shift2Down(mask));
        bb_KnightAttacks[i] |= Shift2Left(ShiftUp(mask));
        bb_KnightAttacks[i] |= Shift2Right(ShiftUp(mask));
        bb_KnightAttacks[i] |= Shift2Left(ShiftDown(mask));
        bb_KnightAttacks[i] |= Shift2Right(ShiftDown(mask));
    }

    // Diagonal attacks
    for (int s = 0;s < 64;++s) {
        for (int b = 0;b < 64;++b) {
            mask = 0;
            q = s;
            while (File(q) > 0 && Rank(q) < 7) {
                q += 7;
                mask |= SetBit(q);
                if (b&(SetBit(RotateL45[q]) >> bb_ShiftL45[s]))
                    break;
            }
            q = s;
            while (File(q) < 7 && Rank(q) > 0) {
                q -= 7;
                mask |= SetBit(q);
                if (b&(SetBit(RotateL45[q]) >> bb_ShiftL45[s]))
                    break;
            }
            bb_L45Attacks[s][b] = mask;

            mask = 0;
            q = s;
            while (File(q) < 7 && Rank(q) < 7) {
                q += 9;
                mask |= SetBit(q);
                if (b&(SetBit(RotateR45[q]) >> bb_ShiftR45[s]))
                    break;
            }
            q = s;
            while (File(q) > 0 && Rank(q) > 0) {
                q -= 9;
                mask |= SetBit(q);
                if (b&(SetBit(RotateR45[q]) >> bb_ShiftR45[s]))
                    break;
            }
            bb_R45Attacks[s][b] = mask;
        }
    }

    // Rank and File attacks
    memset(bb_RankAttacks, 0, sizeof(bb_RankAttacks));
    memset(bb_FileAttacks, 0, sizeof(bb_FileAttacks));
    int file, rank;
    for (int sq = 0;sq < 64;++sq) {
        for (int bitrow = 0;bitrow < 64;++bitrow) {
            file = File(sq);
            q = sq + 1;
            while (++file < 8) {
                bb_RankAttacks[sq][bitrow] |= SetBit(q);
                if ((1 << file)&(bitrow << 1))
                    break;
                ++q;
            }
            file = File(sq);
            q = sq - 1;
            while (--file >= 0) {
                bb_RankAttacks[sq][bitrow] |= SetBit(q);
                if ((1 << file)&(bitrow << 1))
                    break;
                --q;
            }
            rank = Rank(sq);
            q = sq + 8;
            while (++rank < 8) {
                bb_FileAttacks[sq][bitrow] |= SetBit(q);
                if ((1 << (7 - rank))&(bitrow << 1))
                    break;
                q += 8;
            }
            rank = Rank(sq);
            q = sq - 8;
            while (--rank >= 0) {
                bb_FileAttacks[sq][bitrow] |= SetBit(q);
                if ((1 << (7 - rank))&(bitrow << 1))
                    break;
                q -= 8;
            }
        }
    }

    // King:
    for (i = 0;i < 64;++i) {
        mask = SetBit(i);
        bb_KingAttacks[i]  = ShiftLeft(mask);
        bb_KingAttacks[i] |= ShiftRight(mask);
        bb_KingAttacks[i] |= ShiftUp(mask);
        bb_KingAttacks[i] |= ShiftDown(mask);
        bb_KingAttacks[i] |= ShiftUpLeft(mask);
        bb_KingAttacks[i] |= ShiftUpRight(mask);
        bb_KingAttacks[i] |= ShiftDownLeft(mask);
        bb_KingAttacks[i] |= ShiftDownRight(mask);
    }

    // File and rank masks
    quint64 rseed = 0xFF00000000000000ULL;
    quint64 fseed = 0x8080808080808080ULL;
    for (i = 7; i >= 0; --i) {
        bb_rankMask[i] = rseed;
        rseed >>= 8;
        bb_fileMask[i] = fseed;
        fseed >>= 1;
    }

    // Pawn promotion ranks
    bb_PromotionRank[White] = bb_rankMask[7];
    bb_PromotionRank[Black] = bb_rankMask[0];

    // Now that global data has been calculated, we can create a start position
    standardPosition.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

BitBoard getStandardPosition()
{
    BitBoard b;
    b.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    return b;
}

QString BitBoard::moveToFullSan(const Move &move) const
{
    QString dots = toMove() == White ? "." : "...";
    return QString("%1%2%3").arg(m_moveNumber).arg(dots).arg(moveToSan(move));
}
