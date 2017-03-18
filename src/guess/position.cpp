//////////////////////////////////////////////////////////////////////
//
//  FILE:       position.cpp
//              Position class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 1999-2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "misc.h"
#include "position.h"
#include "attacks.h"
#include "sqlist.h"
#include "sqset.h"
#include "hash.h"
#include "bitfind.h"

#include <stdio.h>
#include <ctype.h>

#define ASSERTIONS

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

static unsigned int hashVal [16][64];
static unsigned int stdStartHash = 0;
static unsigned int stdStartPawnHash = 0;

// HASH and UNHASH are identical: XOR the hash value for a (piece,square).
#define HASH(h,p,sq)    (h) ^= hashVal[(p)][(sq)]
#define UNHASH(h,p,sq)  (h) ^= hashVal[(p)][(sq)]

using namespace Guess;

bool Position::getChess960Castling() const
{
    return Chess960Castling;
}

void Position::setChess960Castling(bool value, quint64 castlingRooks)
{
    Chess960Castling = value;
    CastlingRooks = castlingRooks;
}

inline void
Position::AddHash(pieceT p, squareT sq)
{
    HASH(Hash, p, sq);
    if(piece_Type(p) == PAWN)
    {
        HASH(PawnHash, p, sq);
    }
}

inline void
Position::UnHash(pieceT p, squareT sq)
{
    UNHASH(Hash, p, sq);
    if(piece_Type(p) == PAWN)
    {
        UNHASH(PawnHash, p, sq);
    }
}

inline void
Position::AddToBoard(pieceT p, squareT sq)
{
    ASSERT(Board[sq] == EMPTY);
    Board[sq] = p;
    NumOnRank[p][square_Rank(sq)]++;
    NumOnFyle[p][square_Fyle(sq)]++;
    NumOnLeftDiag[p][square_LeftDiag(sq)]++;
    NumOnRightDiag[p][square_RightDiag(sq)]++;
    NumOnSquareColor[p][square_Color(sq)]++;
    AddHash(p, sq);
}

inline void
Position::RemoveFromBoard(pieceT p, squareT sq)
{
    ASSERT(Board[sq] == p);
    Board[sq] = EMPTY;
    NumOnRank[p][square_Rank(sq)]--;
    NumOnFyle[p][square_Fyle(sq)]--;
    NumOnLeftDiag[p][square_LeftDiag(sq)]--;
    NumOnRightDiag[p][square_RightDiag(sq)]--;
    NumOnSquareColor[p][square_Color(sq)]--;
    UnHash(p, sq);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// initHashValues:
//    Initialises the table of Zobrist hash values.
void
initHashValues(void)
{
    // Ensure we set up the hash values only once:
    static int firstCall = 1;
    if(! firstCall)
    {
        return;
    }
    firstCall = 0;


    // First, set all values to 0:
    unsigned int sq;
    for(unsigned int p = 0; p < 16; p++)
    {
        for(sq = A1; sq <= H8; sq++)
        {
            hashVal[p][sq] = 0;
        }
    }

    // Fill in the hash values for each valid [piece][square] index,
    // using a table of pre-generated good values:
    const unsigned int * hash = goodHashValues;
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WK][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WQ][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WR][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WB][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WN][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[WP][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BK][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BQ][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BR][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BB][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BN][sq] = *hash;
        ++hash;
    }
    for(sq = A1; sq <= H8; ++sq)
    {
        hashVal[BP][sq] = *hash;
        ++hash;
    }

    // Compute the hash values for the standard starting position:
    unsigned int h = 0;
    // First the pawns:
    HASH(h, WP, A2);
    HASH(h, WP, B2);
    HASH(h, WP, C2);
    HASH(h, WP, D2);
    HASH(h, WP, E2);
    HASH(h, WP, F2);
    HASH(h, WP, G2);
    HASH(h, WP, H2);
    HASH(h, BP, A7);
    HASH(h, BP, B7);
    HASH(h, BP, C7);
    HASH(h, BP, D7);
    HASH(h, BP, E7);
    HASH(h, BP, F7);
    HASH(h, BP, G7);
    HASH(h, BP, H7);
    stdStartPawnHash = h;
    // Now the nonpawns:
    HASH(h, WR, A1);
    HASH(h, WN, B1);
    HASH(h, WB, C1);
    HASH(h, WQ, D1);
    HASH(h, WK, E1);
    HASH(h, WB, F1);
    HASH(h, WN, G1);
    HASH(h, WR, H1);
    HASH(h, BR, A8);
    HASH(h, BN, B8);
    HASH(h, BB, C8);
    HASH(h, BQ, D8);
    HASH(h, BK, E8);
    HASH(h, BB, F8);
    HASH(h, BN, G8);
    HASH(h, BR, H8);
    stdStartHash = h;
}


///////////////////////////////////////////////////////////////////////////
//  PRIVATE FUNCTIONS -- small ones are inline for speed

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AssertPos():
//      Does a slow, thorough check of the integrity of the
//      data structures, ensuring everything is valid.
//
errorT
Position::AssertPos()
{
    unsigned char mat[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(colorT c = WHITE; c <= BLACK; ++c)
    {
        for(unsigned int i = 0; i < Count[c]; ++i)
        {
            if(ListPos[List[c][i]] != i
                    ||  piece_Color(Board[List[c][i]]) != c)
            {
                DumpBoard(stderr);
                DumpLists(stderr);
                return ERROR_General;
            }
            mat[Board[List[c][i]]]++;
        }
    }
    for(unsigned int i = WK; i < BP; i++)
    {
        if(mat[i] != Material[i])
        {
            DumpBoard(stderr);
            DumpLists(stderr);
            return ERROR_General;
        }
    }
    return OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CalcPinsDir():
//      Look for a pinned piece in the direction 'dir' relative to
//      the position of the king to move.
//
inline void
Position::CalcPinsDir(directionT dir, pieceC attacker)
{
    // Two pieces can pin along any path. A queen is always one,
    // the other is a bishop or rook. To save calculating it here, the
    // appropriate piece (BISHOP) or (ROOK) is passed along with the
    // direction.

    squareT king = GetKingSquare(ToMove);
    squareT friendly = NULL_SQUARE;
    squareT x = king;
    squareT last = square_Last(king, dir);
    int delta = direction_Delta(dir);

    while(x != last)
    {
        x += delta;
        pieceT p = Board[x];
        if(p == EMPTY)
        {
            // Empty square, so keep searching.
        }
        else if(piece_Color_NotEmpty(p) == ToMove)
        {
            // Found a friendly piece.
            if(friendly == NULL_SQUARE)
            {
                // Found first friendly in this direction
                friendly = x;
            }
            else
            {
                // Found second friendly in this direction, so stop.
                return;
            }
        }
        else
        {
            // Found an enemy piece
            if(friendly != NULL_SQUARE)
            {
                // Potential pin:
                pieceC ptype = piece_Type(p);
                if(ptype == QUEEN  ||  ptype == attacker)
                {
                    Pinned[ListPos[friendly]] = dir;
                }
            }
            return;     // found an enemy piece, so end search
        }
    }
    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AddLegalMove():
//      Add a legal move to the move list.
//
inline void
Position::AddLegalMove(MoveList * mlist, squareT from, squareT to, pieceC promotion, bool castle) const
{
    ASSERT(mlist != NULL);
    simpleMoveT sm;

    // We do NOT set the pre-move castling/ep flags, or the captured
    // piece info, here since that is ONLY needed if the move is
    // going to be executed with DoSimpleMove() and then undone.

    sm.from = from;
    sm.to = to;
    sm.promote = promotion;
    sm.movingPiece = Board[from];
    sm.capturedPiece = castle ? EMPTY : Board[to];
    sm.moveCastles = castle;
    if (sm.moveCastles)
    {
        sm.m_visualTo = Chess960Castling ? CastlingRook(to) : to;
    }
    mlist->append(sm);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenSliderMoves():
//      Generate slider moves along a direction, for a sliding
//      piece of the specified color from the specified square.
//      If sqset != NULL, moves must be to a square in sqset.
inline void
Position::GenSliderMoves(MoveList * mlist, colorT color, squareT fromSq,
                         directionT dir, SquareSet * sqset, bool capturesOnly) const
{
    squareT dest = fromSq;
    squareT last = square_Last(fromSq, dir);
    int delta = direction_Delta(dir);

    while(dest != last)
    {
        dest += delta;
        pieceT p = Board[dest];
        if(p == EMPTY)
        {
            if(! capturesOnly)
            {
                if(sqset == NULL  ||  sqset->Contains(dest))
                {
                    AddLegalMove(mlist, fromSq, dest);
                }
            }
            continue;
        }
        // We have reached a piece. Add the capture if it is an enemy.
        if(piece_Color_NotEmpty(p) != color)
        {
            if(sqset == NULL  ||  sqset->Contains(dest))
            {
                AddLegalMove(mlist, fromSq, dest);
            }
        }
        break;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenKnightMoves():
//      Generate knight moves.
//      If sqset != NULL, moves must be to a square in sqset.
inline void
Position::GenKnightMoves(MoveList * mlist, colorT c, squareT fromSq,
                         SquareSet * sqset, bool capturesOnly) const
{
    const squareT * destPtr = knightAttacks[fromSq];
    while(true)
    {
        squareT dest = *destPtr;
        if(dest == NULL_SQUARE)
        {
            break;
        }
        destPtr++;
        pieceT p = Board[dest];
        if(capturesOnly  &&  p == EMPTY)
        {
            continue;
        }
        if(piece_Color(p) != c)
        {
            if(sqset == NULL  ||  sqset->Contains(dest))
            {
                AddLegalMove(mlist, fromSq, dest);
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenCastling():
//    Generate the legal castling moves.
//    Assumes the side to move is NOT in check, so the caller
//    should verify this first.
//
void
Position::GenCastling(MoveList * mlist) const
{
    if (Chess960Castling)
    {
        GenCastling960(mlist);
        return;
    }
    ASSERT(! IsKingInCheck());
    squareT from = GetKingSquare(ToMove);
    squareT enemyKingSq = GetEnemyKingSquare();
    squareT target, skip, rookSq;
    pieceT rookPiece;

    // Queenside Castling:
    if(GetCastling(ToMove, QSIDE))
    {
        if(ToMove == WHITE)
        {
            target = C1;
            skip = D1;
            rookSq = A1;
            rookPiece = WR;
        }
        else
        {
            target = C8;
            skip = D8;
            rookSq = A8;
            rookPiece = BR;
        }

        if(Board[target] == EMPTY  &&  Board[skip] == EMPTY
                &&  Board[rookSq] == rookPiece
                &&  Board[target - 1] == EMPTY // B1 or B8 must be empty too!
                &&  CalcNumChecks(target) == 0
                &&  CalcNumChecks(skip) == 0
                &&  ! square_Adjacent(target, enemyKingSq))
        {
            AddLegalMove(mlist, from, target, C_EMPTY, true);
        }
    }

    // Kingside Castling:
    if(GetCastling(ToMove, KSIDE))
    {
        if(ToMove == WHITE)
        {
            target = G1;
            skip = F1;
            rookSq = H1;
            rookPiece = WR;
        }
        else
        {
            target = G8;
            skip = F8;
            rookSq = H8;
            rookPiece = BR;
        }
        if(Board[target] == EMPTY  &&  Board[skip] == EMPTY
                &&  Board[rookSq] == rookPiece
                &&  CalcNumChecks(target) == 0
                &&  CalcNumChecks(skip) == 0
                &&  ! square_Adjacent(target, enemyKingSq))
        {
            AddLegalMove(mlist, from, target, C_EMPTY, true);
        }
    }
}

pieceT Position::pieceAt(squareT square) const
{
    return (Board[square]);
}

squareT Position::CastlingRook(int index) const
{
    quint64 cr = CastlingRooks;
    squareT x = NULL_SQUARE;
    for (int i=0; i<=index; ++i)
    {
        x = getFirstBitAndClear64<squareT>(cr);
    }
    return x;
}

squareT Position::CastlingRook(squareT target) const
{
    int n = 0;
    switch (target)
    {
    case C1: n = 0; break;
    case G1: n = 1; break;
    case C8: n = 2; break;
    case G8: n = 3; break;
    default: return INVALID_SQUARE;
    }

    return CastlingRook(n);
}

bool Position::isFreeForCastling960(squareT from, squareT to, squareT rook_from, squareT rook_to, squareT enemyKingSq) const
{
    squareT square = from;

    while(square!=to)
    {
        if (square_Adjacent(square, enemyKingSq)) return false;

        if ((square != from) && (square != rook_from))
        {
            if (pieceAt(square) != EMPTY) return false;
        }
        if (CalcNumChecks(square) != 0) return false;
        if (square!=to) square += (from<=to) ? 1:-1;
    }

    if (square_Adjacent(to, enemyKingSq)) return false;
    if ((to != from) && (to != rook_from))
    {
        if (pieceAt(to) != EMPTY) return false;
    }
    if (CalcNumChecks(to) != 0) return false;

    square = rook_from;

    while(square!=rook_to)
    {
        if ((square != rook_from) && (square != from))
        {
            if (pieceAt(square) != EMPTY) return false;
        }
        if (square!=rook_to) square += (rook_from<=rook_to) ? 1:-1;
    }

    if ((rook_from != rook_to) && (rook_to != from))
    {
        if (pieceAt(rook_to) != EMPTY) return false;
    }

    return true; // Both ways, king and rook to target are free except for king/rook themselves
}

void
Position::GenCastling960(MoveList * mlist) const
{
    // todo - this method needs to be fixed!
    ASSERT(! IsKingInCheck());
    squareT from = GetKingSquare(ToMove);
    squareT enemyKingSq = GetEnemyKingSquare();
    squareT target, rookSq, rookTarget;

    // Queenside Castling:
    if(GetCastling(ToMove, QSIDE))
    {
        if(ToMove == WHITE)
        {
            target = C1;
            rookSq = CastlingRook(0);
            rookTarget = D1;
        }
        else
        {
            target = C8;
            rookSq = CastlingRook(2);
            rookTarget = D8;
        }
        if (isFreeForCastling960(from, target, rookSq, rookTarget, enemyKingSq))
        {
           AddLegalMove(mlist, from, target, C_EMPTY, true);
        }
    }

    if(GetCastling(ToMove, KSIDE))
    {
        if(ToMove == WHITE)
        {
            target = G1;
            rookSq = CastlingRook(1);
            rookTarget = F1;
        }
        else
        {
            target = G8;
            rookSq = CastlingRook(3);
            rookTarget = F8;
        }
        if (isFreeForCastling960(from, target, rookSq, rookTarget, enemyKingSq))
        {
           AddLegalMove(mlist, from, target, C_EMPTY, true);
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenKingMoves():
//      Generate the legal King moves. Castling is generated as well, if
//      the specified flag is true.
//
void
Position::GenKingMoves(MoveList * mlist, genMovesT genType, bool castling)
{
    squareT kingSq = GetKingSquare();
    squareT enemyKingSq = GetEnemyKingSquare();
    colorT enemy = color_Flip(ToMove);
    const squareT * destPtr;
    pieceT king = piece_Make(ToMove, KING);
    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);

    if (Board[kingSq] != king)
        return; // TODO - happens - why?
    ASSERT(Board[kingSq] == king);

    destPtr = kingAttacks[kingSq];
    while(*destPtr != NULL_SQUARE)
    {
        // Try this move and see if it legal:
        squareT destSq = *destPtr;
        bool addThisMove = false;

        // Only try this move if the target square has an enemy piece,
        // or if it is empty and noncaptures are to be generated:
        if((genNonCaptures  &&  Board[destSq] == EMPTY)
                ||  piece_Color(Board[destSq]) == enemy)
        {

            // Empty or enemy piece there, so try the move:
            pieceT captured = Board[destSq];
            Board[destSq] = king;
            Board[kingSq] = EMPTY;

            // It is legal if the two kings will not be adjacent and the
            // moving king will not be in check on its new square:
            if(! square_Adjacent(destSq, enemyKingSq))
            {
                if(CalcNumChecks(destSq) == 0)
                {
                    addThisMove = true;
                }
            }
            Board[kingSq] = king;
            Board[destSq] = captured;
        }
        if(addThisMove)
        {
            AddLegalMove(mlist, kingSq, destSq);
        }
        ++destPtr;
    }
    // Now generate castling moves, if possible:
    if(genNonCaptures && castling)
    {
        GenCastling(mlist);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AddPromotions():
//      Add promotion moves.
//      Called by GenPawnMoves() when a pawn can be promoted.
//
inline void
Position::AddPromotions(MoveList * mlist, squareT from, squareT dest) const
{
    ASSERT(piece_Type(Board[from]) == PAWN);
    ASSERT(square_Rank(dest) == RANK_1  ||  square_Rank(dest) == RANK_8);

    AddLegalMove(mlist, from, dest, QUEEN);
    AddLegalMove(mlist, from, dest, ROOK);
    AddLegalMove(mlist, from, dest, BISHOP);
    AddLegalMove(mlist, from, dest, KNIGHT);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsValidEnPassant
//   Used to verify that an en passant  pawn capture is valid.
//   This is needed because illegal en passant captures can appear
//   legal according to calculations of pinned pieces. For example,
//   consider WK d5, WP e5, BP f5 (just moved there), BR h5 and
//   the en passant capture exf6 would be illegal.
inline bool
Position::IsValidEnPassant(squareT from, squareT to)
{
    ASSERT(from <= H8  &&  to <= H8);
    ASSERT(to == EPTarget);

    // Check that this en passant capture is legal:
    pieceT ownPawn = piece_Make(ToMove, PAWN);
    pieceT enemyPawn = piece_Make(color_Flip(ToMove), PAWN);
    squareT enemyPawnSq = (ToMove == WHITE) ? to - 8 : to + 8;
    ASSERT(Board[from] == ownPawn);
    ASSERT(Board[enemyPawnSq] == enemyPawn);
    Board[from] = EMPTY;
    Board[enemyPawnSq] = EMPTY;
    Board[to] = ownPawn;
    bool isValid = ! IsKingInCheck();
    Board[from] = ownPawn;
    Board[enemyPawnSq] = enemyPawn;
    Board[to] = EMPTY;
    return isValid;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenPawnMoves():
//      Generate legal pawn moves.
//      If dir != NULL_DIR, pawn MUST move in direction dir or its opposite.
//      If sqset != NULL, pawn MUST move to a square in sqset.
//      The dir and sq parameters are for pinned pawns and check evasions.
void
Position::GenPawnMoves(MoveList * mlist, squareT from,
                       directionT dir, SquareSet * sqset, genMovesT genType)
{
    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);
    directionT oppdir = direction_Opposite(dir);
    directionT forward;
    rankT promoRank;
    rankT secondRank;
    if(ToMove == WHITE)
    {
        forward = UP;
        promoRank = RANK_8;
        secondRank = RANK_2;
    }
    else
    {
        forward = DOWN;
        promoRank = RANK_1;
        secondRank = RANK_7;
    }
    squareT dest;

    ASSERT(Board[from] == piece_Make(ToMove, PAWN));

    if(genNonCaptures
            && (dir == NULL_DIR  ||  dir == forward  ||  oppdir == forward))
    {
        dest = square_Move(from, forward);
        if(Board[dest] == EMPTY && (sqset == NULL || sqset->Contains(dest)))
        {
            if(square_Rank(dest) == promoRank)
            {
                AddPromotions(mlist, from, dest);
            }
            else
            {
                AddLegalMove(mlist, from, dest);
            }
        }
        if(square_Rank(from) == secondRank  &&  Board[dest] == EMPTY)
        {
            dest = square_Move(dest, forward);
            if(Board[dest] == EMPTY  && (sqset == NULL || sqset->Contains(dest)))
            {
                AddLegalMove(mlist, from, dest);
            }
        }
    }

    // Now do captures: left, then right
    // To be a possible capture, dest square must be EPTarget or hold
    // an enemy piece.
#define POSSIBLE_CAPTURE(d) ((d != NULL_SQUARE)   \
            &&  ((piece_Color (Board[d]) == (color_Flip(ToMove)))  \
                    ||  (d == EPTarget  &&  IsValidEnPassant(from,d))))

    directionT capdir = forward | LEFT;
    if(dir == NULL_DIR  ||  dir == capdir  ||  oppdir == capdir)
    {
        dest = square_Move(from, capdir);
        if(POSSIBLE_CAPTURE(dest)  && (sqset == NULL || sqset->Contains(dest)))
        {
            if(square_Rank(dest) == promoRank)
            {
                AddPromotions(mlist, from, dest);
            }
            else
            {
                AddLegalMove(mlist, from, dest);
            }
        }
    }
    capdir = forward | RIGHT;
    if(dir == NULL_DIR  ||  dir == capdir  ||  oppdir == capdir)
    {
        dest = square_Move(from, capdir);
        if(POSSIBLE_CAPTURE(dest)  && (sqset == NULL || sqset->Contains(dest)))
        {
            if(square_Rank(dest) == promoRank)
            {
                AddPromotions(mlist, from, dest);
            }
            else
            {
                AddLegalMove(mlist, from, dest);
            }
        }
    }
    return;
}


//////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GetHPSig():
//      Return the position's home pawn signature.
//
unsigned int
Position::GetHPSig(void)
{
    unsigned int hpSig = 0;
    pieceT * b = &(Board[A2]);
    if(*b == WP)
    {
        hpSig |= 0x8000;
    }
    b++;  /* a2 */
    if(*b == WP)
    {
        hpSig |= 0x4000;
    }
    b++;  /* b2 */
    if(*b == WP)
    {
        hpSig |= 0x2000;
    }
    b++;  /* c2 */
    if(*b == WP)
    {
        hpSig |= 0x1000;
    }
    b++;  /* d2 */
    if(*b == WP)
    {
        hpSig |= 0x0800;
    }
    b++;  /* e2 */
    if(*b == WP)
    {
        hpSig |= 0x0400;
    }
    b++;  /* f2 */
    if(*b == WP)
    {
        hpSig |= 0x0200;
    }
    b++;  /* g2 */
    if(*b == WP)
    {
        hpSig |= 0x0100;    /* h2 */
    }
    b = &(Board[A7]);
    if(*b == BP)
    {
        hpSig |= 0x0080;
    }
    b++;  /* a7 */
    if(*b == BP)
    {
        hpSig |= 0x0040;
    }
    b++;  /* b7 */
    if(*b == BP)
    {
        hpSig |= 0x0020;
    }
    b++;  /* c7 */
    if(*b == BP)
    {
        hpSig |= 0x0010;
    }
    b++;  /* d7 */
    if(*b == BP)
    {
        hpSig |= 0x0008;
    }
    b++;  /* e7 */
    if(*b == BP)
    {
        hpSig |= 0x0004;
    }
    b++;  /* f7 */
    if(*b == BP)
    {
        hpSig |= 0x0002;
    }
    b++;  /* g7 */
    if(*b == BP)
    {
        hpSig |= 0x0001;    /* h7 */
    }
    return hpSig;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Init():
//      Initialise the position after it is constructed.
//
void
Position::Init(void)
{
    // Setting up a valid board is left to StdStart() or Clear().
    Board [COLOR_SQUARE] = EMPTY;
    Board [NULL_SQUARE] = END_OF_BOARD;
    LegalMoves = NULL;

    // Make sure all tables used for move generation, hashing,
    // square tests, etc have been computed:
    scid_Init();
    initHashValues();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Clear():
//      Clear the board and associated structures.
//
void
Position::Clear(void)
{
    int i;
    for(i = A1; i <= H8; ++i)
    {
        Board[i] = EMPTY;
    }
    for(i = WK; i <= BP; ++i)
    {
        Material[i] = 0;
        for(unsigned int j = 0; j < 8; ++j)
        {
            NumOnRank[i][j] = NumOnFyle[i][j] = 0;
        }
        for(unsigned int d = 0; d < 15; ++d)
        {
            NumOnLeftDiag[i][d] = NumOnRightDiag[i][d] = 0;
        }
        NumOnSquareColor[i][WHITE] = NumOnSquareColor[i][BLACK] = 0;
    }
    Count[WHITE] = Count[BLACK] = 0;
    EPTarget = NULL_SQUARE;
    Castling = 0;
    Board [NULL_SQUARE] = END_OF_BOARD;
    PlyCounter = 0;
    HalfMoveClock = 0;
    Hash = 0;
    PawnHash = 0;
    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::StdStart():
//      Set up the standard chess starting position.
//
void
Position::StdStart(void)
{
    Clear();
    Material[WK] = Material[BK] = 1;
    Material[WQ] = Material[BQ] = 1;
    Material[WR] = Material[BR] = 2;
    Material[WB] = Material[BB] = 2;
    Material[WN] = Material[BN] = 2;
    Material[WP] = Material[BP] = 8;
    Count[WHITE] = Count[BLACK] = 16;

    AddToBoard(WK, E1);
    List[WHITE][0] = E1;
    ListPos[E1] = 0;
    AddToBoard(BK, E8);
    List[BLACK][0] = E8;
    ListPos[E8] = 0;
    AddToBoard(WR, A1);
    List[WHITE][1] = A1;
    ListPos[A1] = 1;
    AddToBoard(BR, A8);
    List[BLACK][1] = A8;
    ListPos[A8] = 1;
    AddToBoard(WN, B1);
    List[WHITE][2] = B1;
    ListPos[B1] = 2;
    AddToBoard(BN, B8);
    List[BLACK][2] = B8;
    ListPos[B8] = 2;
    AddToBoard(WB, C1);
    List[WHITE][3] = C1;
    ListPos[C1] = 3;
    AddToBoard(BB, C8);
    List[BLACK][3] = C8;
    ListPos[C8] = 3;
    AddToBoard(WQ, D1);
    List[WHITE][4] = D1;
    ListPos[D1] = 4;
    AddToBoard(BQ, D8);
    List[BLACK][4] = D8;
    ListPos[D8] = 4;
    AddToBoard(WB, F1);
    List[WHITE][5] = F1;
    ListPos[F1] = 5;
    AddToBoard(BB, F8);
    List[BLACK][5] = F8;
    ListPos[F8] = 5;
    AddToBoard(WN, G1);
    List[WHITE][6] = G1;
    ListPos[G1] = 6;
    AddToBoard(BN, G8);
    List[BLACK][6] = G8;
    ListPos[G8] = 6;
    AddToBoard(WR, H1);
    List[WHITE][7] = H1;
    ListPos[H1] = 7;
    AddToBoard(BR, H8);
    List[BLACK][7] = H8;
    ListPos[H8] = 7;

    for(squareT i = squareT(0); i < 8; i++)
    {
        AddToBoard(WP, A2 + i);
        List[WHITE][i + 8] = A2 + i;
        ListPos[A2 + i] = i + 8;
        AddToBoard(BP, A7 + i);
        List[BLACK][i + 8] = A7 + i;
        ListPos[A7 + i] = i + 8;
    }

    Castling = 0;
    SetCastling(WHITE, QSIDE, true);
    SetCastling(WHITE, KSIDE, true);
    SetCastling(BLACK, QSIDE, true);
    SetCastling(BLACK, KSIDE, true);
    EPTarget = NULL_SQUARE;
    ToMove = WHITE;
    PlyCounter = 0;
    HalfMoveClock = 0;
    Board [NULL_SQUARE] = END_OF_BOARD;
    Hash = stdStartHash;
    PawnHash = stdStartPawnHash;
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsStdStart
//   Returns true if the position is the standard starting position.
bool
Position::IsStdStart()
{
    if(ToMove != WHITE
            ||  Hash != stdStartHash  ||  PawnHash != stdStartPawnHash
            ||  GetCount(WHITE) != 16  ||  GetCount(BLACK) != 16
            ||  RankCount(WP, RANK_2) != 8  ||  RankCount(BP, RANK_7) != 8
            ||  RankCount(WN, RANK_1) != 2  ||  RankCount(BN, RANK_8) != 2
            ||  !GetCastling(WHITE, KSIDE)  ||  !GetCastling(WHITE, QSIDE)
            ||  !GetCastling(BLACK, KSIDE)  ||  !GetCastling(BLACK, QSIDE))
    {
        return false;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AddPiece():
//      Add a piece to the board and piecelist.
//      Checks that a side cannot have more than 16 pieces or more
//      than one king.
//
errorT
Position::AddPiece(pieceT p, squareT sq)
{
    ASSERT(p != EMPTY);
    colorT c = piece_Color(p);

    if(Count[c] == 16)
    {
        return ERROR_PieceCount;
    }
    ASSERT(Count[c] <= 15);
    if(piece_Type(p) == KING)
    {
        // Check there is not already a King:
        if(Material[p] > 0)
        {
            return ERROR_PieceCount;
        }

        // King is always at the start of the piecelist, so move the piece
        // already at location 0 if there is one:
        if(Count[c] > 0)
        {
            squareT oldsq = List[c][0];
            List[c][Count[c]] = oldsq;
            ListPos[oldsq] = Count[c];
        }
        List[c][0] = sq;
        ListPos[sq] = 0;
    }
    else
    {
        ListPos[sq] = Count[c];
        List[c][Count[c]] = sq;
    }
    Count[c]++;
    Material[p]++;
    AddToBoard(p, sq);
    return OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CalcPins():
//      Calculate the pieces for the side to move that are
//      pinned to their king. The array Pinned[] stores, for
//      each piece, the direction in which it is pinned.
//
//      For example WK on e1, WQ on e2, BQ on e7 on the e-fyle
//      means the WQ is Pinned in the direction UP.
//
void
Position::CalcPins(void)
{
    Pinned[ 0] = Pinned[ 1] = Pinned[ 2] = Pinned[ 3] = Pinned[ 4] =
            Pinned[ 5] = Pinned[ 6] = Pinned[ 7] = Pinned[ 8] = Pinned[ 9] =
                                          Pinned[10] = Pinned[11] = Pinned[12] = Pinned[13] = Pinned[14] =
                                                  Pinned[15] = NULL_DIR;

    squareT kingSq = GetKingSquare(ToMove);
    colorT enemy = color_Flip(ToMove);
    pieceT enemyQueen  = piece_Make(enemy, QUEEN);
    pieceT enemyRook   = piece_Make(enemy, ROOK);
    pieceT enemyBishop = piece_Make(enemy, BISHOP);

    // Pins and checks from Bishops/Queens/Rooks:
    fyleT fyle = square_Fyle(kingSq);
    if(FyleCount(enemyQueen, fyle) + FyleCount(enemyRook, fyle) > 0)
    {
        CalcPinsDir(UP, ROOK);
        CalcPinsDir(DOWN, ROOK);
    }
    rankT rank = square_Rank(kingSq);
    if(RankCount(enemyQueen, rank) + RankCount(enemyRook, rank) > 0)
    {
        CalcPinsDir(LEFT, ROOK);
        CalcPinsDir(RIGHT, ROOK);
    }
    leftDiagT ld = square_LeftDiag(kingSq);
    if(LeftDiagCount(enemyQueen, ld) + LeftDiagCount(enemyBishop, ld) > 0)
    {
        CalcPinsDir(UP_LEFT, BISHOP);
        CalcPinsDir(DOWN_RIGHT, BISHOP);
    }
    rightDiagT rd = square_RightDiag(kingSq);
    if(RightDiagCount(enemyQueen, rd) + RightDiagCount(enemyBishop, rd) > 0)
    {
        CalcPinsDir(UP_RIGHT, BISHOP);
        CalcPinsDir(DOWN_LEFT, BISHOP);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenPieceMoves():
//      Generates moves for a nonpawn, nonking piece.
//      If sqset != NULL, moves must be to a square in sqset.<
void
Position::GenPieceMoves(MoveList * mlist, squareT fromSq,
                        SquareSet * sqset, bool capturesOnly) const
{
    colorT c = ToMove;
    pieceT p = Board[fromSq];
    pieceC ptype = piece_Type(p);
    ASSERT(p != EMPTY  &&  ptype != KING  &&  ptype != PAWN);

    if(ptype == KNIGHT)
    {
        GenKnightMoves(mlist, c, fromSq, sqset, capturesOnly);
        return;
    }
    if(ptype != BISHOP)
    {
        GenSliderMoves(mlist, c, fromSq, UP, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, DOWN, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, LEFT, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, RIGHT, sqset, capturesOnly);
    }
    if(ptype != ROOK)
    {
        GenSliderMoves(mlist, c, fromSq, UP_LEFT, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, DOWN_LEFT, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, UP_RIGHT, sqset, capturesOnly);
        GenSliderMoves(mlist, c, fromSq, DOWN_RIGHT, sqset, capturesOnly);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenerateMoves
//    Generate the legal moves list.
//    If the specified pieceType is not EMPTY, then only legal
//    moves for that type of piece are generated.
void
Position::GenerateMoves(MoveList * mlist, pieceC pieceType,
                        genMovesT genType, bool maybeInCheck)
{
    bool capturesOnly = ((genType & GEN_NON_CAPS) == 0);

    unsigned int mask;
    if(pieceType != C_EMPTY)
    {
        mask = 1 << pieceType;
    }
    else
    {
        mask = (1 << KING) | (1 << QUEEN) | (1 << ROOK)
               | (1 << BISHOP) | (1 << KNIGHT) | (1 << PAWN);
    }

    // Use the objects own move list if none was provided:
    if(mlist == NULL)
    {
        ClearLegalMoves();
        mlist = LegalMoves;
    }
    mlist->clear();

    // Compute which pieces of the side to move are pinned to the king:
    CalcPins();

    // Determine if the side to move is in check and find where the
    // checking pieces are, unless the caller has passed maybeInCheck=false
    // indicating it is CERTAIN the side to move is not in check here.

    unsigned int numChecks = 0;
    if(maybeInCheck)
    {
        SquareList checkSquares;
        numChecks = CalcNumChecks(GetKingSquare(ToMove), &checkSquares);
        if(numChecks > 0)
        {
            // The side to move IS in check:
            GenCheckEvasions(mlist, pieceType, genType, &checkSquares);
            return;
        }
    }

    // The side to move is NOT in check. Iterate over each non-king
    // piece, and then generate King moves last of all:

    unsigned int npieces = Count[ToMove];
    for(unsigned int x = 1; x < npieces; x++)
    {
        squareT sq = List[ToMove][x];
        pieceT p = Board[sq];
        pieceC ptype = piece_Type(p);
        if(!(mask & (1 << ptype)))
        {
            continue;
        }
        directionT pinned = Pinned[x];

        // If Pinned[x] == dir (not NULL_DIR), x can ONLY move along
        // that direction or its opposite.

        if(pinned != NULL_DIR)     // piece x is pinned to king
        {
            if(ptype == PAWN)
            {
                GenPawnMoves(mlist, sq, pinned, NULL, genType);
            }
            else if(ptype == KNIGHT)
            {
                // do nothing -- pinned knights cannot move
            }
            else
            {
                // Piece is a pinned Queen/Rook/Bishop. Only generate
                // moves along the pinned direction and its opposite:
                if(ptype == QUEEN
                        || (ptype == ROOK && !direction_IsDiagonal(pinned))
                        || (ptype == BISHOP && direction_IsDiagonal(pinned)))
                {
                    GenSliderMoves(mlist, ToMove, sq, pinned, NULL, capturesOnly);
                    GenSliderMoves(mlist, ToMove, sq, dirOpposite[pinned],
                                   NULL, capturesOnly);
                }
            }
        }
        else
        {
            // This piece is free to move anywhere
            if(ptype == PAWN)
            {
                GenPawnMoves(mlist, sq, NULL_DIR, NULL, genType);
            }
            else
            {
                // Knight/Queen/Bishop/Rook:
                GenPieceMoves(mlist, sq, NULL, capturesOnly);
            }
        }
    }

    // Lastly, king moves...
    if(mask & (1 << KING))
    {
        bool castling = !numChecks;
        GenKingMoves(mlist, genType, castling);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsLegalMove
//   Determines whether the specified move is legal in this position,
//   without requiring move generation (except for castling moves).
bool
Position::IsLegalMove(simpleMoveT * sm)
{
    squareT from = sm->from;
    squareT to = sm->to;
    if(from > H8  ||  to > H8)
    {
        return false;
    }
    if(from == to)
    {
        return false;
    }
    pieceT mover = Board[from];
    pieceT captured = Board[to];
    if(piece_Color(mover) != ToMove)
    {
        return false;
    }
    if(piece_Color(captured) == ToMove)
    {
        return false;
    }
    if(sm->movingPiece != mover)
    {
        return false;
    }
    pieceC cmover = piece_Type(mover);
    if(sm->promote != C_EMPTY && cmover != PAWN)
    {
        return false;
    }

    if(sm->moveCastles && cmover != KING)
    {
        return false;
    }

    if(cmover == PAWN)
    {
        rankT rfrom = square_Rank(from);
        rankT rto = square_Rank(to);
        if(ToMove == BLACK)
        {
            rfrom = RANK_8 - rfrom;
            rto = RANK_8 - rto;
        }
        int rdiff = (int)rto - (int)rfrom;
        int fdiff = (int)square_Fyle(to) - (int)square_Fyle(from);
        if(rdiff < 1  ||  rdiff > 2)
        {
            return false;
        }
        if(fdiff < -1  ||  fdiff > 1)
        {
            return false;
        }
        if(fdiff == 0)     // Pawn push:
        {
            if(captured != EMPTY)
            {
                return false;
            }
            if(rdiff == 2)     // Two-square push:
            {
                if(rfrom != RANK_2)
                {
                    return false;
                }
                // Make sure the square in between is empty:
                squareT midsquare = from + ((to - from) / 2);
                if(Board[midsquare] != EMPTY)
                {
                    return false;
                }
            }
        }
        else      // Pawn capture:
        {
            if(rdiff != 1)
            {
                return false;
            }
            if(captured == EMPTY)
            {
                // It must be en passant, or illegal
                if(to != EPTarget)
                {
                    return false;
                }
            }
        }
        // Check the promotion piece:
        if(rto == RANK_8)
        {
            pieceC p = sm->promote;
            if(p != QUEEN  &&  p != ROOK  &&  p != BISHOP  &&  p != KNIGHT)
            {
                return false;
            }
        }
        else
        {
            if(sm->promote != C_EMPTY)
            {
                return false;
            }
        }

    }
    else if(piece_IsSlider(cmover))
    {
        // Make sure the direction is valid:
        directionT dir = sqDir[from][to];
        if(dir == NULL_DIR)
        {
            return false;
        }
        if(cmover == ROOK  &&  direction_IsDiagonal(dir))
        {
            return false;
        }
        if(cmover == BISHOP  &&  !direction_IsDiagonal(dir))
        {
            return false;
        }
        int delta = direction_Delta(dir);
        // Make sure all the in-between squares are empty:
        squareT dest = from + delta;
        while(dest != to)
        {
            if(Board[dest] != EMPTY)
            {
                return false;
            }
            dest += delta;
        }

    }
    else if(cmover == KNIGHT)
    {
        if(! square_IsKnightHop(from, to))
        {
            return false;
        }

    }
    else /* (cmover == KING) */
    {
        colorT enemy = color_Flip(ToMove);
        if(square_Adjacent(to, GetKingSquare(enemy)))
        {
            return false;
        }
        if(sm->moveCastles)
        {
            // The move must be castling, or illegal.
            if(IsKingInCheck())
            {
                return false;
            }
            MoveList mlist;
            GenCastling(&mlist);
            return (mlist.Find(sm) >= 0);
        }
    }

    // The move looks good, but does it leave the king in check?
    squareT kingSq = (cmover == KING) ? to : GetKingSquare(ToMove);
    colorT enemy = color_Flip(ToMove);
    DoSimpleMove(sm);
    unsigned int nchecks = CalcAttacks(enemy, kingSq, NULL);
    UndoSimpleMove(sm);
    return (nchecks == 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MatchLegalMove():
//      Sets the LegalMoves list to contain all the moves of pieces
//      of type "mask" that move to the square "target".
//      The type must be QUEEN, ROOK, BISHOP, or KNIGHT.
//      Pawn and King moves are handled in separate functions,
//      MatchKingMove() and MatchPawnMove().
//
void
Position::MatchLegalMove(MoveList * mlist, pieceC mask, squareT target)
{
    unsigned int x;

    // This function isn't for Pawn or King moves!
    ASSERT(mask != PAWN  &&  mask != KING);

    if(mlist == NULL)
    {
        ClearLegalMoves();
        mlist = LegalMoves;
    }
    mlist->clear();

    unsigned int count = 0;
    unsigned int total = Material[piece_Make(ToMove, mask)];

    pieceT p, captured;
    pieceC pt;
    squareT kingSq = GetKingSquare(ToMove);
    directionT dir;

    unsigned int tryMove = 0;

    // First, verify that the target square is empty or contains
    // an enemy piece:

    p = Board[target];
    if(p != EMPTY  &&  piece_Color(p) == ToMove)
    {
        return;
    }

    // Loop through looking for pieces of type "mask". We start at 1
    // since the King is always the piece at position 0 in the list.

    squareT * sqPtr = &(List[ToMove][1]);
    for(x = 1;  x < Count[ToMove]  &&  count < total;  ++x, ++sqPtr)
    {
        p = Board[*sqPtr];
        pt = piece_Type(p);
        if(pt == mask)
        {

            // Increment count so we stop when we've seen all the
            // Material[p] pieces of this type.

            tryMove = 0;
            ++count;
            squareT sq;

            switch(pt)
            {
            case KNIGHT:
                if(square_IsKnightHop(*sqPtr, target))
                {
                    tryMove = 1;
                }
                break;

            case ROOK:
                dir = sqDir[*sqPtr][target];
                if(dir != NULL_DIR  &&  !direction_IsDiagonal(dir))
                {
                    sq = square_Move(*sqPtr, dir);
                    tryMove = 1;
                    while(sq != target)
                    {
                        if(Board[sq] != EMPTY)    // oops, piece in the way
                        {
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move(sq, dir);
                    }
                }
                break;

            case BISHOP:
                dir = sqDir[*sqPtr][target];
                if(direction_IsDiagonal(dir))
                {
                    sq = square_Move(*sqPtr, dir);
                    tryMove = 1;
                    while(sq != target)
                    {
                        if(Board[sq] != EMPTY)    // oops, piece in the way
                        {
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move(sq, dir);
                    }
                }
                break;

            case QUEEN:
                dir = sqDir[*sqPtr][target];
                if(dir != NULL_DIR)     // Try the move!
                {
                    sq = square_Move(*sqPtr, dir);
                    tryMove = 1;
                    while(sq != target)
                    {
                        if(Board[sq] != EMPTY)    // oops, piece in the way
                        {
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move(sq, dir);
                    }
                }
                break;

            default:  // Should never happen
                ASSERT(0);
            }

            // Now, if tryMove is 1, the piece can get to target. We need
            // to see if the move is legal or leaves the king in check.

            if(tryMove == 1)
            {
                captured = Board[target];
                Board[target] = p;
                Board[*sqPtr] = EMPTY;
                if(CalcNumChecks(kingSq) > 0)
                {
                    tryMove = 0;
                }
                Board[*sqPtr] = p;
                Board[target] = captured;
                if(tryMove == 1)
                {
                    AddLegalMove(mlist, *sqPtr, target);
                }
            }
        }
    }
    // Ok, we've searched all the pieces.
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MatchPawnMove():
//      Sets the LegalMoves list to contain the matching pawn move,
//      if there is one.
//
errorT
Position::MatchPawnMove(MoveList * mlist, fyleT fromFyle, squareT to, pieceC promote)
{
    mlist->clear();

    signed int diff = (int)square_Fyle(to) - (int)fromFyle;
    if(diff < -1  ||  diff > 1)
    {
        return ERROR_InvalidMove;
    }
    pieceT pawn;
    rankT toRank = square_Rank(to);
    fyleT toFyle = square_Fyle(to);
    rankT promoteRank;

    // from is the from square; backup is the alternative from square
    // for a pawn move two squares forward.

    squareT from, backup = NS;

    if(ToMove == WHITE)
    {
        pawn = WP;
        promoteRank = RANK_8;
        if(toRank < RANK_3)
        {
            return ERROR_InvalidMove;
        }
        from = square_Make(fromFyle, toRank - 1);
        if(toRank == RANK_4  &&  fromFyle == toFyle)
        {
            backup = to - 16;
        }
    }
    else
    {
        pawn = BP;
        promoteRank = RANK_1;
        if(toRank > RANK_6)
        {
            return ERROR_InvalidMove;
        }
        from = square_Make(fromFyle, toRank + 1);
        if(toRank == RANK_5  &&  fromFyle == toFyle)
        {
            backup = to + 16;
        }
    }

    // See if the promotion piece is valid:

    if(toRank == promoteRank)
    {
        if(promote == C_EMPTY)
        {
            return ERROR_InvalidMove;
        }
    }
    else
    {
        if(promote != C_EMPTY)
        {
            return ERROR_InvalidMove;
        }
    }

    if(Board[from] != pawn)
    {
        // No match; but it could be a foward-two-squares move:
        if(backup == NS || Board[from] != EMPTY || Board[backup] != pawn)
        {
            // A forward-two-squares move is impossible.
            return ERROR_InvalidMove;
        }
        from = backup;
    }

    // OK, now 'from' is the only possible from-square. Is the move legal?
    // We make the move on the board and see if the King is in check.

    unsigned int legal = 0;
    if(fromFyle == toFyle)
    {
        // Not a capture:

        if(Board[to] != EMPTY)
        {
            return ERROR_InvalidMove;
        }
        Board[to] = pawn;
        Board[from] = EMPTY;
        if(CalcNumChecks(GetKingSquare()) == 0)
        {
            legal = 1;
        }
        Board[to] = EMPTY;
        Board[from] = pawn;

    }
    else
    {
        // It is a capture -- is it legal?

        pieceT captured = Board[to];
        if(captured == EMPTY)
        {
            // Must be an en passant or illegal move.
            if(to != EPTarget)
            {
                return ERROR_InvalidMove;
            }
            squareT epSquare = square_Make(toFyle, square_Rank(from));

            pieceT enemyPawn = piece_Make(color_Flip(ToMove), PAWN);
            // If following assert fails, eptarget was corrupt
            ASSERT(Board[epSquare] == enemyPawn);

            Board[to] = pawn;
            Board[from] = EMPTY;
            Board[epSquare] = EMPTY;
            Material[enemyPawn] --;
            if(CalcNumChecks(GetKingSquare()) == 0)
            {
                legal = 1;
            }
            Board[epSquare] = enemyPawn;
            Board[to] = EMPTY;
            Board[from] = pawn;
            Material[enemyPawn]++;

        }
        else
        {
            if(piece_Color(captured) == ToMove)
            {
                // Capturing a friendly!
                return ERROR_InvalidMove;
            }
            else
            {
                // A regular capture. See if it leaves King in check:
                Board[to] = pawn;
                Board[from] = EMPTY;
                Material[captured]--;
                if(CalcNumChecks(GetKingSquare()) == 0)
                {
                    legal = 1;
                }
                Material[captured]++;
                Board[to] = captured;
                Board[from] = pawn;
            }
        }
    }

    if(legal == 1)
    {
        AddLegalMove(mlist, from, to, promote);
        return OK;
    }
    return ERROR_InvalidMove;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MatchKingMove():
//      Sets the LegalMoves list to contain the matching king move,
//      if there is one.
//
errorT
Position::MatchKingMove(MoveList * mlist, squareT target)
{
    mlist->clear();
    squareT kingSq = GetKingSquare(ToMove);
    signed int diff = (int)target - (int) kingSq;

    // Valid diffs are: -9, -8, -7, -2, -1, 1, 2, 7, 8, 9. (-2,2: Castling)

    if(diff < -9  ||  diff > 9)
    {
        return ERROR_InvalidMove;
    }
    if(diff > -7  &&  diff < -2)
    {
        return ERROR_InvalidMove;
    }
    if(diff > 2  &&  diff < 7)
    {
        return ERROR_InvalidMove;
    }
    if(diff == 0)
    {
        return ERROR_InvalidMove;
    }

    if(diff == 2)    // KingSide Castling
    {
        if(kingSq != (ToMove == WHITE ? E1 : E8))
        {
            return ERROR_InvalidMove;
        }
        if(!GetCastling(ToMove, KSIDE))
        {
            return ERROR_InvalidMove;
        }

        // XXX We also need to verify that the target square does not
        //     lie adjacent to the location of the enemy king!

        if(Board[kingSq + 1] != EMPTY  ||  Board[kingSq + 2] != EMPTY
                ||  CalcNumChecks(kingSq) > 0
                ||  CalcNumChecks(kingSq + 1) > 0
                ||  CalcNumChecks(kingSq + 2) > 0)
        {
            return ERROR_InvalidMove;
        }
        AddLegalMove(mlist, kingSq, target);
        return OK;
    }

    if(diff == -2)    // Queenside Castling
    {
        if(kingSq != (ToMove == WHITE ? E1 : E8))
        {
            return ERROR_InvalidMove;
        }
        if(!GetCastling(ToMove, QSIDE))
        {
            return ERROR_InvalidMove;
        }
        if(Board[kingSq - 1] != EMPTY  ||  Board[kingSq - 2] != EMPTY
                ||  Board[kingSq - 3] != EMPTY
                ||  CalcNumChecks(kingSq) > 0
                ||  CalcNumChecks(kingSq - 1) > 0
                ||  CalcNumChecks(kingSq - 2) > 0)
        {
            return ERROR_InvalidMove;
        }
        AddLegalMove(mlist, kingSq, target);
        return OK;
    }
    pieceT captured = Board[target];
    if(piece_Color(captured) == ToMove)
    {
        // Capturing a friendly piece!
        return ERROR_InvalidMove;
    }

    // Now make the move on the Board and Material lists, and see if it
    // leaves the King in check:
    // XXX We should also check for adjacency to enemy King!!

    Board[target] = piece_Make(ToMove, KING);
    Board[kingSq] = EMPTY;
    if(captured != EMPTY)
    {
        Material[captured]--;
    }
    unsigned int legal = 0;
    if(CalcNumChecks(target) == 0)
    {
        legal = 1;
    }
    if(captured != EMPTY)
    {
        Material[captured]++;
    }
    Board[target] = captured;
    Board[kingSq] = piece_Make(ToMove, KING);
    if(legal == 1)
    {
        AddLegalMove(mlist, kingSq, target);
        return OK;
    }
    return ERROR_InvalidMove;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenCheckEvasions():
//      Generate legal moves for the side to move when the
//      King is in check.
//
void
Position::GenCheckEvasions(MoveList * mlist, pieceC mask, genMovesT genType,
                           SquareList * checkSquares)
{
    unsigned int numChecks = checkSquares->Size();

    // Assert that king IS actually in check:
    ASSERT(numChecks > 0);

    bool capturesOnly = ((genType & GEN_NON_CAPS) == 0);

    mlist->clear();

    squareT king = GetKingSquare(ToMove);

    // if it's double check, we can ONLY move the king
    if(numChecks == 1)
    {
        // OK, it is NOT a double check
        // Try to block piece/capture piece. Remember en passant!
        // First, generate a list of targets: squares between the king
        // and attacker to block, and the attacker's square.

        squareT attackSq = checkSquares->Get(0);
        directionT dir = sqDir[king][attackSq];
        SquareSet targets;  // Set of blocking/capturing squares.
        targets.Add(attackSq);

        // Now add squares we can might be able to block on.
        if(dir != NULL_DIR)
        {
            squareT sq = square_Move(king, dir);
            while(sq != attackSq)
            {
                if(Board[sq] == EMPTY)
                {
                    targets.Add(sq);
                }
                sq = square_Move(sq, dir);
            }
        }

        // Try each non-King piece in turn. If a piece is pinned to
        // the king, don't bother since it cannot possibly block or
        // capture the piece that is giving check!

        unsigned int numPieces = Count[ToMove];
        for(unsigned int p2 = 1; p2 < numPieces; ++p2)
        {
            squareT from = List[ToMove][p2];
            pieceT p2piece = Board[from];
            if(Pinned[p2] != NULL_DIR)
            {
                continue;
            }
            if(mask == C_EMPTY  ||  mask == piece_Type(p2piece))
            {
                if(piece_Type(p2piece) == PAWN)
                {
                    GenPawnMoves(mlist, from, NULL_DIR, &targets, genType);
                    // Capturing a pawn en passant will only get out
                    // of check if the pawn that moved two squares
                    // is doing the checking. If it is not, that means
                    // a discovered check with the last pawn move so
                    // taking en passant cannot get out of check.
                    if(EPTarget != NULL_SQUARE)
                    {
                        squareT pawnSq = (ToMove == WHITE ? EPTarget - 8 : EPTarget + 8);
                        if(pawnSq == attackSq)
                        {
                            SquareSet epset;
                            epset.Add(EPTarget);
                            GenPawnMoves(mlist, from, NULL_DIR, &epset, genType);
                        }
                    }
                }
                else
                {
                    GenPieceMoves(mlist, from, &targets, capturesOnly);
                }
            }
        }  // end of search for captures/blocks
    }

    // Now king moves -- just compute them normally:
    if(mask == C_EMPTY  ||  mask == KING)
    {
        GenKingMoves(mlist, genType, false);
    }

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CalcAttacks():
//      Calculate the number of attacks by a side on a square.
//      This function also puts a list of the attacking piece squares
//      in the fromSqs parameter if it is non-NULL.
//
//      It ONLY uses the Board[] and Material[] lists of the Position, and
//      ASSUMES they are correct with respect to each other, but it does
//      NOT use the List[] or ListPos[] information.
//      This allows us to move pieces quickly (altering only Board[] and
//      Material[]) and detect whether they leave the king in check,
//      without having to update other information.
unsigned int
Position::CalcAttacks(colorT side, squareT target, SquareList * fromSquares) const
{
    // If squares is NULL, caller doesn't want a list of the squares of
    // attacking pieces. To avoid comparing fromSquares with NULL every time
    // we find a check, we set up a local array to use instead if fromSquares
    // is NULL.
    SquareList fromSqs;
    if(fromSquares == NULL)
    {
        fromSquares = &fromSqs;
    }

    fromSquares->Clear();
    squareT sq;

    // Bishop/Queen/Rook attacks: look at each of the 8 directions
    pieceT queen, rook, bishop, knight;
    if(side == WHITE)
    {
        queen = WQ;
        rook = WR;
        bishop = WB;
        knight = WN;
    }
    else
    {
        queen = BQ;
        rook = BR;
        bishop = BB;
        knight = BN;
    }

    unsigned int numQueensRooks = Material[queen] + Material[rook];
    unsigned int numQueensBishops = Material[queen] + Material[bishop];

    // We only bother if there are any sliding pieces of each type:
    if(numQueensRooks > 0)
    {
        fyleT fyle = square_Fyle(target);
        rankT rank = square_Rank(target);
        directionT dirs[4];
        unsigned int ndirs = 0;
        if(FyleCount(queen, fyle) + FyleCount(rook, fyle) > 0)
        {
            dirs[ndirs++] = UP;
            dirs[ndirs++] = DOWN;
        }
        if(RankCount(queen, rank) + RankCount(rook, rank) > 0)
        {
            dirs[ndirs++] = LEFT;
            dirs[ndirs++] = RIGHT;
        }
        for(unsigned int i = 0; i < ndirs; ++i)
        {
            directionT dir = dirs[i];
            int delta = direction_Delta(dir);
            squareT dest = target;
            squareT last = square_Last(target, dir);

            while(dest != last)
            {
                dest += delta;
                pieceT p = Board[dest];
                if(p == EMPTY)
                {
                    // empty square: keep searching
                }
                else if(p == queen  ||  p == rook)
                {
                    // Found an attacking piece
                    fromSquares->Add(dest);
                    break;
                }
                else
                {
                    // Found a piece, but not a queen or rook
                    break;
                }
            }
        }
    }

    // Now diagonal sliders: Queens/Bishops:
    if(numQueensBishops > 0)
    {
        leftDiagT left = square_LeftDiag(target);
        rightDiagT right = square_RightDiag(target);
        directionT dirs[4];
        unsigned int ndirs = 0;
        if(LeftDiagCount(queen, left) + LeftDiagCount(bishop, left) > 0)
        {
            dirs[ndirs++] = UP_LEFT;
            dirs[ndirs++] = DOWN_RIGHT;
        }
        if(RightDiagCount(queen, right) + RightDiagCount(bishop, right) > 0)
        {
            dirs[ndirs++] = UP_RIGHT;
            dirs[ndirs++] = DOWN_LEFT;
        }
        for(unsigned int i = 0; i < ndirs; i++)
        {
            directionT dir = dirs[i];
            int delta = direction_Delta(dir);
            squareT dest = target;
            squareT last = square_Last(target, dir);

            while(dest != last)
            {
                dest += delta;
                pieceT p = Board[dest];
                if(p == EMPTY)
                {
                    // empty square: keep searching
                }
                else if(p == queen  ||  p == bishop)
                {
                    // Found an attacking piece
                    fromSquares->Add(dest);
                    break;
                }
                else
                {
                    // Found a piece, but not an enemy queen or bishop
                    break;
                }
            }
        }
    }

    // Now knight checks: we only bother if there is a knight on the
    // opposite square color of the target square color.
    if(Material[knight] > 0
            &&  SquareColorCount(knight, color_Flip(square_Color(target))) > 0)
    {
        const squareT * destPtr = knightAttacks[target];
        while(true)
        {
            squareT dest = *destPtr;
            if(dest == NULL_SQUARE)
            {
                break;
            }
            if(Board[dest] == knight)
            {
                fromSquares->Add(dest);
            }
            destPtr++;
        }
    }

    // Now pawn attacks:
    if(side == WHITE)
    {
        if(square_Rank(target) != RANK_1)     //if (Material[WP] > 0) {
        {
            sq = square_Move(target, DOWN_LEFT);
            if(Board[sq] == WP)
            {
                fromSquares->Add(sq);
            }
            sq = square_Move(target, DOWN_RIGHT);
            if(Board[sq] == WP)
            {
                fromSquares->Add(sq);
            }
        }
    }
    else
    {
        if(square_Rank(target) != RANK_8)     //if (Material[BP] > 0) {
        {
            sq = square_Move(target, UP_LEFT);
            if(Board[sq] == BP)
            {
                fromSquares->Add(sq);
            }
            sq = square_Move(target, UP_RIGHT);
            if(Board[sq] == BP)
            {
                fromSquares->Add(sq);
            }
        }
    }

    return fromSquares->Size();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsKingInCheckDir
//   Returns true if the King of the side to move is attacked
//   by an enemy sliding piece (Queen/Rook/Bishop) from the
//   specified direction.
bool
Position::IsKingInCheckDir(directionT dir) const
{
    ASSERT(dir != NULL_DIR);
    squareT kingSq = GetKingSquare(ToMove);
    colorT enemy = color_Flip(ToMove);
    bool isDiagonal = direction_IsDiagonal(dir);
    pieceT queen = piece_Make(enemy, QUEEN);
    pieceT slider = piece_Make(enemy, (isDiagonal ? BISHOP : ROOK));

    // First, make sure the enemy has sliding pieces that could give check:
    unsigned int nSliders = PieceCount(queen) + PieceCount(slider);
    if(nSliders == 0)
    {
        return false;
    }

    // Now make sure the enemy has a sliding piece on the appropriate
    // rank, file or diagonal:
    fyleT fyle = square_Fyle(kingSq);
    rankT rank = square_Rank(kingSq);
    leftDiagT ldiag = square_LeftDiag(kingSq);
    rightDiagT rdiag = square_RightDiag(kingSq);

    switch(dir)
    {
    case UP:
    case DOWN:
        nSliders = FyleCount(queen, fyle) + FyleCount(slider, fyle);
        break;
    case LEFT:
    case RIGHT:
        nSliders = RankCount(queen, rank) + RankCount(slider, rank);
        break;
    case UP_LEFT:
    case DOWN_RIGHT:
        nSliders = LeftDiagCount(queen, ldiag) + LeftDiagCount(slider, ldiag);
        break;
    case UP_RIGHT:
    case DOWN_LEFT:
        nSliders = RightDiagCount(queen, rdiag) + RightDiagCount(slider, rdiag);
        break;
    }
    if(nSliders == 0)
    {
        return false;
    }

    // Now move along the specified direction looking for a checking piece:
    squareT dest = kingSq;
    squareT last = square_Last(kingSq, dir);
    int delta = direction_Delta(dir);

    while(dest != last)
    {
        dest += delta;
        pieceT p = Board[dest];
        if(p == EMPTY)
        {
            // empty square: keep searching
        }
        else if(p == queen  ||  p == slider)
        {
            // Found an checking slider piece
            return true;
        }
        else
        {
            // Found a piece, but not an enemy queen or rook/bishop
            break;
        }
    }

    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsKingInCheck
//   Returns true if the king of the side to move is in check.
//   If the specified move is not NULL, it must be the legal move
//   that the opponent played to reach this position. This will
//   be used as a speed optimization, by skipping cases where the
//   move could not have left the king in check.
//
bool
Position::IsKingInCheck(simpleMoveT * sm) const
{
    if(sm == NULL)
    {
        return IsKingInCheck();
    }

    squareT kingSq = GetKingSquare(ToMove);
    pieceC pt = piece_Type(sm->movingPiece);
    if(sm->promote != C_EMPTY)
    {
        pt = sm->promote;
    }

    // No optimization of the last move was castling:
    if(pt == KING && sm->moveCastles)
    {
        fyleT toFyle = square_Fyle(sm->to);
        if(toFyle == C_FYLE  ||  toFyle == G_FYLE)
        {
            return IsKingInCheck();
        }
    }
    // No optimization for en passant capture:
    if(pt == PAWN  &&  piece_Type(sm->capturedPiece) == PAWN)
    {
        rankT fromRank = square_Rank(sm->from);
        rankT capturedRank = square_Rank(sm->capturedSquare);
        if(fromRank == capturedRank)
        {
            return IsKingInCheck();
        }
    }

    if(pt == PAWN)
    {
        if(ToMove == WHITE)
        {
            if(Material[BP] > 0)
            {
                squareT sq = square_Move(kingSq, UP_LEFT);
                if(Board[sq] == BP)
                {
                    return true;
                }
                sq = square_Move(kingSq, UP_RIGHT);
                if(Board[sq] == BP)
                {
                    return true;
                }
            }
        }
        else
        {
            if(Material[WP] > 0)
            {
                squareT sq = square_Move(kingSq, DOWN_LEFT);
                if(Board[sq] == WP)
                {
                    return true;
                }
                sq = square_Move(kingSq, DOWN_RIGHT);
                if(Board[sq] == WP)
                {
                    return true;
                }
            }
        }
    }
    else if(pt == KNIGHT)
    {
        if(square_IsKnightHop(kingSq, sm->to))
        {
            return true;
        }
    }
    else if(pt == KING)
    {
        // A king cannot directly check its adversary.
    }
    else
    {
        // A sliding piece:
        directionT toDir = sqDir[kingSq][sm->to];
        if(toDir != NULL_DIR  &&  IsKingInCheckDir(toDir))
        {
            return true;
        }
    }

    // Now look for a discovered check from a sliding piece:
    directionT dir = sqDir[kingSq][sm->from];
    if(dir != NULL_DIR  &&  IsKingInCheckDir(dir))
    {
        return true;
    }

    ASSERT(IsKingInCheck() == false);
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Mobility
//    Returns the number of squares a rook or bishop of the specified
//    color would attack from the specified square.
unsigned int
Position::Mobility(pieceC p, colorT color, squareT from)
{
    ASSERT(p == ROOK  ||  p == BISHOP);
    unsigned int mobility = 0;
    directionT rookDirs[4] = { UP, DOWN, LEFT, RIGHT };
    directionT bishopDirs[4]
        = { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
    directionT * dirPtr = (p == ROOK ? rookDirs : bishopDirs);

    for(unsigned int i = 0; i < 4; ++i)
    {
        directionT dir = dirPtr[i];
        int delta = direction_Delta(dir);
        squareT dest = from;
        squareT last = square_Last(from, dir);

        while(dest != last)
        {
            dest += delta;
            pieceT p = Board[dest];
            if(p == EMPTY)     // Empty square
            {
                ++mobility;
            }
            else if(piece_Color(p) == color)       // Friendly piece
            {
                break;  // Finished with this direction.
            }
            else      // Enemy piece
            {
                ++mobility;
                break;  // Finished with this direction.
            }
        }
    }
    return mobility;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::SmallestDefender()
//    Returns the type of the lowest-valued piece of the specified
//    color that could capture to the specified square. Pins to the
//    king are ignored. Checks pieces in the order Pawn, Knight,
//    Bishop, Rook, Queen then King. If the specified square is
//    undefended, EMPTY is returned.
//
pieceC
Position::SmallestDefender(colorT color, squareT target) const
{
    SquareList defenderSquares;
    pieceT defenders [16];
    unsigned int numDefenders = CalcAttacks(color, target, &defenderSquares);

    // If the square is undefended, just return EMPTY:
    if(numDefenders == 0)
    {
        return C_EMPTY;
    }

    unsigned int i;
    for(i = 0; i < numDefenders; ++i)
    {
        defenders[i] = Board[defenderSquares.Get(i)];
    }
    // Look for pawns first:
    for(i = 0; i < numDefenders; ++i)
    {
        if(piece_Type(defenders[i]) == PAWN)
        {
            return PAWN;
        }
    }
    // Look for knights then bishops:
    for(i = 0; i < numDefenders; ++i)
    {
        if(piece_Type(defenders[i]) == KNIGHT)
        {
            return KNIGHT;
        }
    }
    for(i = 0; i < numDefenders; ++i)
    {
        if(piece_Type(defenders[i]) == BISHOP)
        {
            return BISHOP;
        }
    }
    // Look for rooks then queens:
    for(i = 0; i < numDefenders; ++i)
    {
        if(piece_Type(defenders[i]) == ROOK)
        {
            return ROOK;
        }
    }
    for(i = 0; i < numDefenders; ++i)
    {
        if(piece_Type(defenders[i]) == QUEEN)
        {
            return QUEEN;
        }
    }

    // If we get here, it must be defended only by the king:
    ASSERT(numDefenders == 1  &&  piece_Type(defenders[0]) == KING);
    return KING;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsKingInMate():
//      Quick check if king is in mate.
//
bool
Position::IsKingInMate(void)
{
    SquareList checkSquares;
    unsigned int numChecks = CalcNumChecks(GetKingSquare(ToMove), &checkSquares);
    if(numChecks == 0)
    {
        return false;
    }
    CalcPins();
    MoveList mlist;
    GenCheckEvasions(&mlist, C_EMPTY, GEN_ALL_MOVES, &checkSquares);
    if(mlist.size() == 0)
    {
        return true;
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsLegal()
//   Verifies the position as being legal.
//   Returns false for any of the following:
//     - if a king is missing;
//     - if the two kings are adjacent;
//     - if there are any pawns on the 1st/8th rank;
//     - if the side to move is already checking the enemy king.
bool
Position::IsLegal(void) const
{
    squareT stmKing = GetKingSquare();
    squareT enemyKing = GetEnemyKingSquare();
    if (!(stmKing <= H8  && enemyKing <= H8))
    {
        printf("Kings mssing\n");
        return false;
    }
    if(square_Adjacent(stmKing, enemyKing))
    {
        printf("Kings adjacent\n");
        return false;
    }
    if(RankCount(WP, RANK_1) != 0)
    {
        return false;
    }
    if(RankCount(WP, RANK_8) != 0)
    {
        return false;
    }
    if(RankCount(BP, RANK_1) != 0)
    {
        return false;
    }
    if(RankCount(BP, RANK_8) != 0)
    {
        return false;
    }
    if(CalcAttacks(ToMove, enemyKing, NULL) > 0)
    {
        return false;
    }
    return true;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsPromoMove():
//      Returns true if the move is a promotion move.
//      NOTE that the move may not actually be legal!
//      The arguments 'from' and 'to' can be in either order.
bool
Position::IsPromoMove(squareT from, squareT to)
{
    rankT seventh, eighth;
    pieceT pawn;
    if(ToMove == WHITE)
    {
        seventh = RANK_7;
        eighth = RANK_8;
        pawn = WP;
    }
    else
    {
        seventh = RANK_2;
        eighth = RANK_1;
        pawn = BP;
    }
    rankT fromR, toR;
    fromR = square_Rank(from);
    toR = square_Rank(to);
    if((fromR == seventh  &&  toR == eighth  &&  Board[from] == pawn)  ||
            (toR == seventh  &&  fromR == eighth  &&  Board[to] == pawn))
    {
        return 1;
    }
    return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DoSimpleMove():
//      Make the move on the board and update all the necessary
//      fields in the simpleMove structure so it can be undone.
//
void
Position::DoSimpleMove(simpleMoveT * sm)
{
    ASSERT(sm != NULL);
    squareT from = sm->from;
    squareT to = sm->to;
    pieceT p = Board[from];
    pieceC ptype = piece_Type(p);
    colorT enemy = color_Flip(ToMove);
    ASSERT(p != EMPTY);

    // update move fields that (maybe) have not yet been set:

    sm->pieceNum = ListPos[from];
    sm->capturedPiece = Board[to];
    sm->capturedSquare = to;
    sm->castleFlags = Castling;
    sm->epSquare = EPTarget;
    sm->oldHalfMoveClock = HalfMoveClock;

    HalfMoveClock++;
    PlyCounter++;

    // Check for a null (empty) move:
    if(isNullMove(sm))
    {
        ToMove = enemy;
        EPTarget = NULL_SQUARE;
        return;
    }

    // Handle en passant capture:

    if(ptype == PAWN  &&  sm->capturedPiece == EMPTY
            && square_Fyle(from) != square_Fyle(to))
    {

        // This was an EP capture. We do not need to check it was a capture
        // since if a pawn lands on EPTarget it must capture to get there.

        pieceT enemyPawn = piece_Make(enemy, PAWN);
        sm->capturedSquare = (ToMove == WHITE ? (to - 8) : (to + 8));
        ASSERT(Board[sm->capturedSquare] == enemyPawn);
        sm->capturedPiece = enemyPawn;
    }

    // handle captures:

    if(!sm->moveCastles && sm->capturedPiece != EMPTY)
    {
        ASSERT(piece_Type(sm->capturedPiece) != KING);
        sm->capturedNum = ListPos[sm->capturedSquare];
        // update opponents List of pieces
        Count[enemy]--;
        ListPos[List[enemy][Count[enemy]]] = sm->capturedNum;
        List[enemy][sm->capturedNum] = List[enemy][Count[enemy]];
        Material[sm->capturedPiece]--;
        HalfMoveClock = 0;
        RemoveFromBoard(sm->capturedPiece, sm->capturedSquare);
    }

    // handle promotion:

    if(sm->promote != C_EMPTY)
    {
        ASSERT(p == piece_Make(ToMove, PAWN));
        Material[p]--;
        RemoveFromBoard(p, from);
        p = piece_Make(ToMove, sm->promote);
        Material[p]++;
        AddToBoard(p, from);
    }

    // handle Castling:

    if(sm->moveCastles)
    {
        ASSERT(ptype == KING);
        squareT rookfrom, rookto;
        pieceT rook = piece_Make(ToMove, ROOK);
        if(square_Fyle(to) == C_FYLE)
        {
            int n = (ToMove==WHITE) ? 0:2;
            rookfrom = CastlingRook(n);
            rookto = to + 1;
        }
        else
        {
            int n = (ToMove==WHITE) ? 1:3;
            rookfrom = CastlingRook(n);
            rookto = to - 1;
        }
        if (rookfrom != rookto)
        {
            ListPos[rookto] = ListPos[rookfrom];
            List[ToMove][ListPos[rookto]] = rookto;
            RemoveFromBoard(rook, rookfrom);
            if (rookto == from)
            {
                RemoveFromBoard(p, from);
            }
            AddToBoard(rook, rookto);
        }
        if (to != from)
        {
            List[ToMove][sm->pieceNum] = to;
            ListPos[to] = sm->pieceNum;
            if (rookto != from)
            {
                RemoveFromBoard(p, from);
            }
            AddToBoard(p, to);
        }
    }
    else
    {
        // now make the move:
        List[ToMove][sm->pieceNum] = to;
        ListPos[to] = sm->pieceNum;
        RemoveFromBoard(p, from);
        AddToBoard(p, to);
    }

    EPTarget = NULL_SQUARE;
    // Handle clearing of castling flags:

    if(Castling)
    {
        if(ptype == KING)      // The king moved.
        {
            SetCastling(ToMove, QSIDE, false);
            SetCastling(ToMove, KSIDE, false);
        }
        // See if a rook moved or was captured:
        else if(ToMove == WHITE)
        {
            if(from == CastlingRook(0))
            {
                SetCastling(WHITE, QSIDE, false);
            }
            else if(from == CastlingRook(1))
            {
                SetCastling(WHITE, KSIDE, false);
            }
            if(to == CastlingRook(2))
            {
                SetCastling(BLACK, QSIDE, false);
            }
            else if(to == CastlingRook(3))
            {
                SetCastling(BLACK, KSIDE, false);
            }
        }
        else
        {
            if(from == CastlingRook(2))
            {
                SetCastling(BLACK, QSIDE, false);
            }
            else if(from == CastlingRook(3))
            {
                SetCastling(BLACK, KSIDE, false);
            }
            if(to == CastlingRook(0))
            {
                SetCastling(WHITE, QSIDE, false);
            }
            if(to == CastlingRook(1))
            {
                SetCastling(WHITE, KSIDE, false);
            }
        }
    }
    else if(ptype == PAWN)
    {
        // Set the EPTarget square, if a pawn advanced two squares and an
        // enemy pawn is on a square where en passant may be possible.

        rankT fromRank = square_Rank(from);
        rankT toRank = square_Rank(to);
        if(fromRank == RANK_2  &&  toRank == RANK_4
                && (Board[square_Move(to, LEFT)] == BP
                    ||  Board[square_Move(to, RIGHT)] == BP))
        {
            EPTarget = square_Move(from, UP);
        }
        if(fromRank == RANK_7  &&  toRank == RANK_5
                && (Board[square_Move(to, LEFT)] == WP
                    ||  Board[square_Move(to, RIGHT)] == WP))
        {
            EPTarget = square_Move(from, DOWN);
        }
        HalfMoveClock = 0; // 50-move clock resets on pawn moves.
    }

    ToMove = enemy;

#ifdef ASSERTIONS
    // Do a SLOW, careful check for corruption:
    if(AssertPos() != OK)
    {
        abort();
    }
#endif

    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::UndoSimpleMove():
//      Take back a simple move that has been made with DoSimpleMove().
//
void
Position::UndoSimpleMove(simpleMoveT * m)
{
    ASSERT(m != NULL);
    squareT from = m->from;
    squareT to = m->to;
    pieceT p = Board[to];
    EPTarget = m->epSquare;
    Castling = m->castleFlags;
    HalfMoveClock = m->oldHalfMoveClock;
    PlyCounter--;
    ToMove = color_Flip(ToMove);
    m->pieceNum = ListPos[to];

    // Check for a null move:
    if(isNullMove(m))
    {
        return;
    }

    // Handle a capture: insert piece back into piecelist.
    // This works for EP captures too, since the square of the captured
    // piece is in the "capturedSquare" field rather than assuming the
    // value of the "to" field. The only time these two fields are
    // different is for an en passant move.

    if(!m->moveCastles && m->capturedPiece != EMPTY)
    {
        colorT c = color_Flip(ToMove);
        ListPos[List[c][m->capturedNum]] = Count[c];
        ListPos[m->capturedSquare] = m->capturedNum;
        List[c][Count[c]] = List[c][m->capturedNum];
        List[c][m->capturedNum] = m->capturedSquare;
        Material[m->capturedPiece]++;
        Count[c]++;
    }

    // handle promotion:

    if(m->promote != C_EMPTY)
    {
        Material[p]--;
        RemoveFromBoard(p, to);
        p = piece_Make(ToMove, PAWN);
        Material[p]++;
        AddToBoard(p, to);
    }

    // now make the move:
    if(m->moveCastles)
    {
        squareT rookfrom, rookto;
        pieceT rook = (ToMove == WHITE ? WR : BR);
        if(square_Fyle(to) == C_FYLE)
        {
            rookfrom = CastlingRook(ToMove==WHITE ? 0:2);
            rookto = to + 1;
        }
        else
        {
            rookfrom = CastlingRook(ToMove==WHITE ? 1:3);
            rookto = to - 1;
        }
        if (rookfrom != rookto)
        {
            ListPos[rookfrom] = ListPos[rookto];
            List[ToMove][ListPos[rookto]] = rookfrom;
            RemoveFromBoard(rook, rookto);
            if (rookfrom == to)
            {
                RemoveFromBoard(p, to);
            }
            AddToBoard(rook, rookfrom);
        }
        if (to != from)
        {
            List[ToMove][m->pieceNum] = from;
            ListPos[from] = m->pieceNum;
            if (rookfrom != to)
            {
                RemoveFromBoard(p, to);
            }
            AddToBoard(p, from);
        }
    }
    else
    {
        List[ToMove][m->pieceNum] = from;
        ListPos[from] = m->pieceNum;
        RemoveFromBoard(p, to);
        AddToBoard(p, from);
        if(m->capturedPiece != EMPTY)
        {
            AddToBoard(m->capturedPiece, m->capturedSquare);
        }
    }


#ifdef ASSERTIONS
    if(AssertPos() != OK)
    {
        abort();
    }
#endif

    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::RelocatePiece():
//    Given a from-square and to-square, modifies the position so
//    the piece on the from-square is relocated to the to-square.
//    Returns an error if the from square is empty, or the target
//    square is not empty, or the relocation would otherwise
//    produce an illegal position (e.g. pawn on the 1st or 8th rank
//    or a King in check).
//
errorT
Position::RelocatePiece(squareT fromSq, squareT toSq)
{
    // Must have on-board squares:
    if(fromSq == NS ||  toSq == NS)
    {
        return ERROR_General;
    }

    // If squares are identical, just return success:
    if(fromSq == toSq)
    {
        return OK;
    }

    pieceT piece = Board[fromSq];
    pieceC ptype = piece_Type(piece);
    colorT pcolor = piece_Color(piece);

    // Must be relocating a nonempty piece to an empty square:
    if(piece == EMPTY  ||  Board[toSq] != EMPTY)
    {
        return ERROR_General;
    }

    // Pawns cannot relocate to the first or last rank:
    if(ptype == PAWN)
    {
        rankT toRank = square_Rank(toSq);
        if(toRank == RANK_1  ||  toRank == RANK_8)
        {
            return ERROR_General;
        }
    }

    // Locate the piece index in the appropriate list of pieces:
    unsigned int index = ListPos[fromSq];
    ASSERT(List[pcolor][index] == fromSq);

    // Relocate the piece:
    List[pcolor][index] = toSq;
    ListPos[toSq] = index;
    RemoveFromBoard(piece, fromSq);
    AddToBoard(piece, toSq);

    // Check for adjacent kings or side to move giving check:
    if(! IsLegal())
    {
        // Undo the relocation and return error:
        List[pcolor][index] = fromSq;
        RemoveFromBoard(piece, toSq);
        AddToBoard(piece, fromSq);
        return ERROR_General;
    }

    // Relocation successful:
    return OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MaterialValue():
//    Returns the sum value of material for a particular side,
//    where piece values are:
//    King: 0 (since both sides always have one)
//    Queen: 9
//    Rook: 5
//    Bishop, Knight: 3 each
//    Pawn: 1
unsigned int
Position::MaterialValue(colorT c)
{
    ASSERT(c == WHITE  ||  c == BLACK);
    unsigned int value = 0;
    if(c == WHITE)
    {
        value += 9 * PieceCount(WQ);
        value += 5 * PieceCount(WR);
        value += 3 * PieceCount(WB);
        value += 3 * PieceCount(WN);
        value += 1 * PieceCount(WP);
    }
    else
    {
        value += 9 * PieceCount(BQ);
        value += 5 * PieceCount(BR);
        value += 3 * PieceCount(BB);
        value += 3 * PieceCount(BN);
        value += 1 * PieceCount(BP);
    }
    return value;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MakeSANString():
//      Make the SAN string for a simpleMove.
//      The parameter 'sanFlag' indicates whether '+' and '#' symbols
//      should be added to checking or mating moves.
//
void
Position::MakeSANString(simpleMoveT * m, char * s, sanFlagT flag)
{
    ASSERT(m != NULL  &&  s != NULL);

    // Make sure m->pieceNum is updated:
    m->pieceNum = ListPos[m->from];
    pieceC  pt    = piece_Type(Board[List[ToMove][m->pieceNum]]);
    squareT from = List[ToMove][m->pieceNum];
    squareT to   = m->to;
    char * c     = s;

    if(isNullMove(m))
    {
        //*c++ = 'n'; *c++ = 'u'; *c++ = 'l'; *c++ = 'l';
        *c++ = '-';
        *c++ = '-';
    }
    else if(pt == PAWN)
    {
        if(square_Fyle(from) != square_Fyle(to))     // pawn capture
        {
            *c++ = square_FyleChar(from);
            *c++ = 'x';
        }
        *c++ = square_FyleChar(to);
        *c++ = square_RankChar(to);
        if((square_Rank(to) == RANK_1) || (square_Rank(to) == RANK_8))
        {
            *c++ = '=';
            *c++ = piece_Char(m->promote);
        }
    }
    else if(pt == KING)
    {
        if(m->moveCastles && (square_Fyle(to) == G_FYLE))
        {
            *c++ = 'O';
            *c++ = '-';
            *c++ = 'O';
        }
        else if(m->moveCastles && (square_Fyle(to) == C_FYLE))
        {
            *c++ = 'O';
            *c++ = '-';
            *c++ = 'O';
            *c++ = '-';
            *c++ = 'O';
        }
        else      // regular King move
        {
            *c++ = 'K';
            if(Board[to] != EMPTY)
            {
                *c++ = 'x';
            }
            *c++ = square_FyleChar(to);
            *c++ = square_RankChar(to);
        }

    }
    else        // Queen/Rook/Bishop/Knight
    {
        *c++ = piece_Char(pt);

        // We only need to calculate legal moves to disambiguate if there
        // are more than one of this type of piece.

        if(Material[Board[m->from]] < 2)
        {
            if(Board[to] != EMPTY)
            {
                *c++ = 'x';
            }
            *c++ = square_FyleChar(to);
            *c++ = square_RankChar(to);

        }
        else
        {
            // disambiguate moves here:
            // SHOULD handle 3-way ambiguity!  Looks like it does ok.
            bool ambiguous_fyle = false;
            bool ambiguous_rank = false;
            char r, f, f2;
            f = square_FyleChar(from);
            r = square_RankChar(from);
            MoveList mlist;
            MatchLegalMove(&mlist, pt, to);

            for(unsigned int i = 0; (int) i < mlist.size(); i++)
            {
                simpleMoveT * m2 = mlist.Get(i);
                squareT from2 = m2->from;
                pieceC p2 = piece_Type(Board[from2]);
                if((to == m2->to) && (from != from2) && (p2 == pt))
                {
                    /* we have an ambiguity */
                    f2 = square_FyleChar(from2);
                    if(f == f2)     // ambiguous fyle, so print rank
                    {
                        ambiguous_fyle = true;
                    }
                    else            // ambiguous rank, so print fyle
                    {
                        ambiguous_rank = true;
                    }
                }
            }
            if(ambiguous_rank)
            {
                *c++ = f;    // print from-fyle
            }
            if(ambiguous_fyle)
            {
                *c++ = r;    // print from-rank
            }

            if(Board[to] != EMPTY)
            {
                *c++ = 'x';
            }
            *c++ = square_FyleChar(to);
            *c++ = square_RankChar(to);
        }
    }

    // Now do the check or mate symbol:
    if(flag != SAN_NO_CHECKTEST)
    {
        // Now we make the move to test for check:
        DoSimpleMove(m);
        if(CalcNumChecks(GetKingSquare()) > 0)
        {
            char ch = '+';
            if(flag == SAN_MATETEST)
            {
                MoveList mlist;
                GenerateMoves(&mlist);
                if(mlist.size() == 0)
                {
                    ch = '#';
                }
            }
            *c++ = ch;
        }
        UndoSimpleMove(m);
    }
    *c = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpBoard():
//      Dump the board to an open file.
//
void
Position::DumpBoard(FILE * fp)
{
    ASSERT(fp != NULL);
    squareT s;
    for(int i = 7; i >= 0; i--)
    {
        fputs("   ", fp);
        for(int j = 0; j < 8; ++j)
        {
            s = squareT((i * 8) + j);
            putc(PIECE_CHAR[Board[s]], fp);
            putc(' ', fp);
        }
        putc('\n', fp);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpLists():
//      Dump the piece lists to an open file.
//
void
Position::DumpLists(FILE * fp)
{
    ASSERT(fp != NULL);
    unsigned int i;
    for(colorT c = WHITE; c <= BLACK; ++c)
    {
        for(i = 0; i < Count[c]; ++i)
        {
            pieceT p = Board[List[c][i]];
            fprintf(fp, "%2d:", ListPos[List[c][i]]);
            putc(PIECE_CHAR[p], fp);
            putc(square_FyleChar(List[c][i]), fp);
            putc(square_RankChar(List[c][i]), fp);
            putc(' ', fp);
        }
        putc('\n', fp);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadFromFEN():
//      Setup the position from a FEN string.
//      Note: the slashes usually found in Fen strings to mark the start
//      of a new row do not need to be present, but if they are, they must
//      appear at the actual start of a new row or the string will be
//      considered corrupt.
//
//      IMPORTANT: the shortcut of having a two-digit number to represent
//      a number of empty rows (e.g. "/24/" instead of "/8/8/8/") is NOT
//      accepted by this function.
//
//      It is not considered an error for the halfmove clock or fullmove
//      counter to be invalid, so this routine can also read positions
//      from EPD lines (which only share the first four fields with FEN).
errorT
Position::ReadFromFEN(const char * str)
{
    // pieceFromByte[] converts a character to its piece, e.g. 'k' -> BK.
    static pieceT pieceFromByte [256];

    // fenSqToRealSquare[] converts a fen square (0 to 63) to its real
    // square. E.g: [0] -> A8, [1] -> B8, .... [63] -> H1.
    static squareT fenSqToRealSquare [64];

    // Note the first Call to set up the static arrays only once:
    static int firstCall = 1;

    ASSERT(str != NULL);
    const char * s = str;
    int count = 0;

    if(firstCall)
    {
        firstCall = 0;

        // Set up pieceFromByte[]:
        for(int i = 0; i < 256; ++i)
        {
            pieceFromByte[i] = EMPTY;
        }
        pieceFromByte [(int) 'K'] = WK;
        pieceFromByte [(int) 'k'] = BK;
        pieceFromByte [(int) 'Q'] = WQ;
        pieceFromByte [(int) 'q'] = BQ;
        pieceFromByte [(int) 'R'] = WR;
        pieceFromByte [(int) 'r'] = BR;
        pieceFromByte [(int) 'B'] = WB;
        pieceFromByte [(int) 'b'] = BB;
        pieceFromByte [(int) 'N'] = WN;
        pieceFromByte [(int) 'n'] = BN;
        pieceFromByte [(int) 'P'] = WP;
        pieceFromByte [(int) 'p'] = BP;

        // Set up fenSqToRealSq[]:
        for(int sq = 0; sq < 64; ++sq)
        {
            fenSqToRealSquare [sq] = (squareT)((7 - (sq) / 8) * 8 + ((sq) % 8));
        }
    }

    Clear();
    while(count < 64)
    {
        if(*s == '/')
        {
            // A FEN string does not have to contain '/'s but if one
            // appears anywhere except the start of a row, it is an error:

            if(count % 8)
            {
                return ERROR_InvalidFEN;
            }

        }
        else if(*s > '0'  &&  *s < '9')
        {
            count += (*s - '0');

        }
        else
        {
            pieceT p = pieceFromByte [(unsigned char) * s];
            if(p == EMPTY)
            {
                return ERROR_InvalidFEN;
            }
            if(AddPiece(p, fenSqToRealSquare[count]) != OK)
            {
                return ERROR_InvalidFEN;
            }
            count++;
        }
        s++;
    }
    if(Material[WK] != 1  ||  Material[BK] != 1)
    {
        return ERROR_InvalidFEN;
    }

    // Now the side to move:
    while(isspace(*s))
    {
        s++;
    }
    switch(*s)
    {
    case 'w':
        SetToMove(WHITE);
        break;
    case 'b':
        SetToMove(BLACK);
        break;
    default:
        return ERROR_InvalidFEN;
    }
    s++;

    if(! IsLegal())
    {
        return ERROR_InvalidFEN;
    }

    // Now the castling flags:
    while(isspace(*s))
    {
        s++;
    }
    if(*s == '-')
    {
        s++;  // do nothing
    }
    else if(*s == 0)
    {
        // The FEN has no castling field, so just guess that
        // castling is possible whenever a king and rook are
        // still on their starting squares:
        if(Board[E1] == WK)
        {
            if(Board[A1] == WR)
            {
                SetCastling(WHITE, QSIDE, true);
            }
            if(Board[H1] == WR)
            {
                SetCastling(WHITE, KSIDE, true);
            }
        }
        if(Board[E8] == BK)
        {
            if(Board[A8] == BR)
            {
                SetCastling(BLACK, QSIDE, true);
            }
            if(Board[H8] == BR)
            {
                SetCastling(BLACK, KSIDE, true);
            }
        }
    }
    else
    {
        while(!isspace(*s)  &&  *s != 0)
        {
            switch(*s)
            {
            case 'Q':
                SetCastling(WHITE, QSIDE, true);
                break;
            case 'q':
                SetCastling(BLACK, QSIDE, true);
                break;
            case 'K':
                SetCastling(WHITE, KSIDE, true);
                break;
            case 'k':
                SetCastling(BLACK, KSIDE, true);
                break;
            default:
                return ERROR_InvalidFEN;
            }
            s++;
        }
    }

    // Now the EP target:
    while(isspace(*s))
    {
        s++;
    }
    if(*s == 0)
    {
        // do nothing
    }
    else if(*s == '-')
    {
        EPTarget = NULL_SQUARE;
        s++;  // No EP target
    }
    else
    {
        char fylec = *s;
        s++;
        if(fylec < 'a'  ||  fylec > 'h')
        {
            return ERROR_InvalidFEN;
        }
        char rankc = *s;
        s++;
        if(rankc != '3'  &&  rankc != '6')
        {
            return ERROR_InvalidFEN;
        }
        EPTarget = square_Make(fyle_FromChar(fylec), rank_FromChar(rankc));
    }

    // Now the capture/pawn halfmove clock:
    while(isspace(*s))
    {
        s++;
    }
    if(*s)
    {
        HalfMoveClock = (unsigned short) atoi(s);
    }
    while(!isspace(*s)  && *s != 0)
    {
        s++;
    }

    // Finally, the fullmove counter:
    while(isspace(*s))
    {
        s++;
    }
    if(*s)
    {
        int i = atoi(s);
        if(i >= 1)
        {
            PlyCounter = (i - 1) * 2;
        }
    }
    if(ToMove == BLACK)
    {
        PlyCounter++;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CopyFrom():
//      Copy another position to this one.
//
void
Position::CopyFrom(Position * src)
{
    for(squareT p = A1; p <= NS; ++p)
    {
        Board[p] = src->Board[p];
    };
    Count[WHITE] = src->Count[WHITE];
    Count[BLACK] = src->Count[BLACK];
    unsigned int i;
    for(i = 0; i < 64; ++i)
    {
        ListPos[i] = src->ListPos[i];
    }
    for(i = 0; i < 16; ++i)
    {
        Material[i] = src->Material[i];
        List[WHITE][i] = src->List[WHITE][i];
        List[BLACK][i] = src->List[BLACK][i];
        for(unsigned int j = 0; j < 8; ++j)
        {
            NumOnFyle[i][j] = src->NumOnFyle[i][j];
            NumOnRank[i][j] = src->NumOnRank[i][j];
        }
        for(unsigned int d = 0; d < 15; ++d)
        {
            NumOnLeftDiag[i][d] = src->NumOnLeftDiag[i][d];
            NumOnRightDiag[i][d] = src->NumOnRightDiag[i][d];
        }
        NumOnSquareColor[i][WHITE] = src->NumOnSquareColor[i][WHITE];
        NumOnSquareColor[i][BLACK] = src->NumOnSquareColor[i][BLACK];
    }
    EPTarget = src->EPTarget;
    ToMove = src->ToMove;
    PlyCounter = src->PlyCounter;
    HalfMoveClock = src->HalfMoveClock;
    Castling = src->Castling;
    Hash = src->Hash;
    PawnHash = src->PawnHash;
    Chess960Castling = src->Chess960Castling;
    CastlingRooks = src->CastlingRooks;
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GetSquares
//    Adds to the provided square list all squares containing the specified
//    piece, and return the number of pieces of that type on the board.
unsigned int
Position::GetSquares(pieceT piece, SquareList * sqlist)
{
    colorT color = piece_Color(piece);
    squareT * squares = GetList(color);
    unsigned int npieces = GetCount(color);
    for(unsigned int i = 0; i < npieces; i++)
    {
        squareT sq = squares[i];
        pieceT p = Board[sq];
        if(p == piece)
        {
            sqlist->Add(sq);
        }
    }
    return Material[piece];
}

//////////////////////////////////////////////////////////////////////
//  EOF: position.cpp
//////////////////////////////////////////////////////////////////////
