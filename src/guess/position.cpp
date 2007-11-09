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
#include "position.h"
#include "attacks.h"
#include "misc.h"
#include "sqlist.h"
#include "sqset.h"
#include "hash.h"

#include <stdio.h>
#include <ctype.h>

namespace Guess
{

static uint hashVal [16][64];
static uint stdStartHash = 0;
static uint stdStartPawnHash = 0;

// HASH and UNHASH are identical: XOR the hash value for a (piece,square).
#define HASH(h,p,sq)    (h) ^= hashVal[(p)][(sq)]
#define UNHASH(h,p,sq)  (h) ^= hashVal[(p)][(sq)]

inline void
Position::AddHash (pieceT p, squareT sq)
{
    HASH (Hash, p, sq);
    if (piece_Type(p) == PAWN) {
        HASH (PawnHash, p, sq);
    }
}

inline void
Position::UnHash (pieceT p, squareT sq)
{
    UNHASH (Hash, p, sq);
    if (piece_Type(p) == PAWN) {
        UNHASH (PawnHash, p, sq);
    }
}

inline void
Position::AddToBoard (pieceT p, squareT sq)
{
    ASSERT (Board[sq] == EMPTY);
    Board[sq] = p;
    NumOnRank[p][square_Rank(sq)]++;
    NumOnFyle[p][square_Fyle(sq)]++;
    NumOnLeftDiag[p][square_LeftDiag(sq)]++;
    NumOnRightDiag[p][square_RightDiag(sq)]++;
    NumOnSquareColor[p][square_Color(sq)]++;
    AddHash (p, sq);
}

inline void
Position::RemoveFromBoard (pieceT p, squareT sq)
{
    ASSERT (Board[sq] == p);
    Board[sq] = EMPTY;
    NumOnRank[p][square_Rank(sq)]--;
    NumOnFyle[p][square_Fyle(sq)]--;
    NumOnLeftDiag[p][square_LeftDiag(sq)]--;
    NumOnRightDiag[p][square_RightDiag(sq)]--;
    NumOnSquareColor[p][square_Color(sq)]--;
    UnHash (p, sq);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// initHashValues:
//    Initialises the table of Zobrist hash values.
void
initHashValues (void)
{
    // Ensure we set up the hash values only once:
    static int firstCall = 1;
    if (! firstCall) { return; }
    firstCall = 0;


    // First, set all values to 0:
    uint sq;
    for (uint p = 0; p < 16; p++) {
        for (sq = A1; sq <= H8; sq++) { hashVal[p][sq] = 0; }
    }

    // Fill in the hash values for each valid [piece][square] index,
    // using a table of pre-generated good values:
    const unsigned int * hash = goodHashValues;
    for (sq=A1; sq <= H8; sq++) { hashVal[WK][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[WQ][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[WR][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[WB][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[WN][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[WP][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BK][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BQ][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BR][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BB][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BN][sq] = *hash; hash++; }
    for (sq=A1; sq <= H8; sq++) { hashVal[BP][sq] = *hash; hash++; }

    // Compute the hash values for the standard starting position:
    uint h = 0;
    // First the pawns:
    HASH (h,WP,A2);  HASH (h,WP,B2);  HASH (h,WP,C2);  HASH (h,WP,D2);
    HASH (h,WP,E2);  HASH (h,WP,F2);  HASH (h,WP,G2);  HASH (h,WP,H2);
    HASH (h,BP,A7);  HASH (h,BP,B7);  HASH (h,BP,C7);  HASH (h,BP,D7);
    HASH (h,BP,E7);  HASH (h,BP,F7);  HASH (h,BP,G7);  HASH (h,BP,H7);
    stdStartPawnHash = h;
    // Now the nonpawns:
    HASH (h,WR,A1);  HASH (h,WN,B1);  HASH (h,WB,C1);  HASH (h,WQ,D1);
    HASH (h,WK,E1);  HASH (h,WB,F1);  HASH (h,WN,G1);  HASH (h,WR,H1);
    HASH (h,BR,A8);  HASH (h,BN,B8);  HASH (h,BB,C8);  HASH (h,BQ,D8);
    HASH (h,BK,E8);  HASH (h,BB,F8);  HASH (h,BN,G8);  HASH (h,BR,H8);
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
Position::AssertPos ()
{
    byte mat[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for (colorT c = WHITE; c <= BLACK; c++) {
        for (uint i=0; i < Count[c]; i++) {
            if (ListPos[List[c][i]] != i
                  ||  piece_Color(Board[List[c][i]]) != c) {
                DumpBoard (stderr);
                DumpLists (stderr);
                return ERROR;
            }
            mat[Board[List[c][i]]]++;
        }
    }
    for (uint i=WK; i < BP; i++) {
        if (mat[i] != Material[i]) {
            DumpBoard (stderr);
            DumpLists (stderr);
            return ERROR;
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
Position::CalcPinsDir (directionT dir, pieceT attacker)
{
    // Two pieces can pin along any path. A queen is always one,
    // the other is a bishop or rook. To save calculating it here, the
    // appropriate piece (BISHOP) or (ROOK) is passed along with the
    // direction.

    squareT king = GetKingSquare (ToMove);
    squareT friendly = NULL_SQUARE;
    squareT x = king;
    squareT last = square_Last (king, dir);
    int delta = direction_Delta (dir);

    while (x != last) {
        x += delta;
        pieceT p = Board[x];
        if (p == EMPTY) {
            // Empty square, so keep searching.
        } else if (piece_Color_NotEmpty(p) == ToMove) {
            // Found a friendly piece.
            if (friendly == NULL_SQUARE) {
                // Found first friendly in this direction
                friendly = x;
            } else {
                // Found second friendly in this direction, so stop.
                return;
            }
        } else {
            // Found an enemy piece
            if (friendly != NULL_SQUARE) {
                // Potential pin:
                pieceT ptype = piece_Type(p);
                if (ptype == QUEEN  ||  ptype == attacker) {
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
Position::AddLegalMove (MoveList * mlist, squareT from, squareT to, pieceT promo)
{
    ASSERT (mlist != NULL);
    simpleMoveT * sm = mlist->Add();

    // We do NOT set the pre-move castling/ep flags, or the captured
    // piece info, here since that is ONLY needed if the move is
    // going to be executed with DoSimpleMove() and then undone.

    sm->from = from;
    sm->to = to;
    sm->promote = promo;
    sm->movingPiece = Board[from];
    sm->capturedPiece = Board[to];
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenSliderMoves():
//      Generate slider moves along a direction, for a sliding
//      piece of the specified color from the specified square.
//      If sqset != NULL, moves must be to a square in sqset.
inline void
Position::GenSliderMoves (MoveList * mlist, colorT color, squareT fromSq,
                          directionT dir, SquareSet * sqset, bool capturesOnly)
{
    squareT dest = fromSq;
    squareT last = square_Last (fromSq, dir);
    int delta = direction_Delta (dir);

    while (dest != last) {
        dest += delta;
        pieceT p = Board[dest];
        if (p == EMPTY) {
            if (! capturesOnly) {
                if (sqset == NULL  ||  sqset->Contains(dest)) {
                    AddLegalMove (mlist, fromSq, dest, EMPTY);
                }
            }
            continue;
        }
        // We have reached a piece. Add the capture if it is an enemy.
        if (piece_Color_NotEmpty(p) != color) {
            if (sqset == NULL  ||  sqset->Contains(dest)) {
                AddLegalMove (mlist, fromSq, dest, EMPTY);
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
Position::GenKnightMoves (MoveList * mlist, colorT c, squareT fromSq,
                          SquareSet * sqset, bool capturesOnly)
{
    const squareT * destPtr = knightAttacks[fromSq];
    while (true) {
        squareT dest = *destPtr;
        if (dest == NULL_SQUARE) { break; }
        destPtr++;
        pieceT p = Board[dest];
        if (capturesOnly  &&  p == EMPTY) { continue; }
        if (piece_Color(p) != c) {
            if (sqset == NULL  ||  sqset->Contains(dest)) {
                AddLegalMove (mlist, fromSq, dest, EMPTY);
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
Position::GenCastling (MoveList * mlist)
{
    ASSERT (! IsKingInCheck());
    squareT from = GetKingSquare(ToMove);
    if (from != (ToMove == WHITE ? E1 : E8))  { return; }
    squareT enemyKingSq = GetEnemyKingSquare();
    squareT target, skip, rookSq;
    pieceT rookPiece;

    // Queenside Castling:
    if (!StrictCastling  ||  GetCastling (ToMove, QSIDE)) {
        if (ToMove == WHITE) {
            target = C1; skip = D1; rookSq = A1; rookPiece = WR;
        } else {
            target = C8; skip = D8; rookSq = A8; rookPiece = BR;
        }
        if (Board[target] == EMPTY  &&  Board[skip] == EMPTY
                &&  Board[rookSq] == rookPiece
                &&  Board[target - 1] == EMPTY // B1 or B8 must be empty too!
                &&  CalcNumChecks (target) == 0
                &&  CalcNumChecks (skip) == 0
                &&  ! square_Adjacent (target, enemyKingSq)) {
            AddLegalMove (mlist, from, target, EMPTY);
        }
    }

    // Kingside Castling:
    if (!StrictCastling  ||  GetCastling (ToMove, KSIDE)) {
        if (ToMove == WHITE) {
            target = G1; skip = F1; rookSq = H1; rookPiece = WR;
        } else {
            target = G8; skip = F8; rookSq = H8; rookPiece = BR;
        }
        if (Board[target] == EMPTY  &&  Board[skip] == EMPTY
                &&  Board[rookSq] == rookPiece
                &&  CalcNumChecks (target) == 0
                &&  CalcNumChecks (skip) == 0
                &&  ! square_Adjacent (target, enemyKingSq)) {
            AddLegalMove (mlist, from, target, EMPTY);
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenKingMoves():
//      Generate the legal King moves. Castling is generated as well, if
//      the specified flag is true.
//
void
Position::GenKingMoves (MoveList * mlist, genMovesT genType, bool castling)
{
    squareT kingSq = GetKingSquare();
    squareT enemyKingSq = GetEnemyKingSquare();
    colorT enemy = color_Flip(ToMove);
    const squareT * destPtr;
    pieceT king = piece_Make (ToMove, KING);
    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);

    ASSERT (Board[kingSq] == king);

    destPtr = kingAttacks[kingSq];
    while (*destPtr != NULL_SQUARE) {
        // Try this move and see if it legal:

        squareT destSq = *destPtr;
        bool addThisMove = false;

        // Only try this move if the target square has an enemy piece,
        // or if it is empty and noncaptures are to be generated:
        if ( (genNonCaptures  &&  Board[destSq] == EMPTY)
             ||  piece_Color (Board[destSq]) == enemy) {

            // Empty or enemy piece there, so try the move:
            pieceT captured = Board[destSq];
            Board[destSq] = king;
            Board[kingSq] = EMPTY;

            // It is legal if the two kings will not be adjacent and the
            // moving king will not be in check on its new square:
            if (! square_Adjacent (destSq, enemyKingSq)) {
                if (CalcNumChecks (destSq) == 0) {
                    addThisMove = true;
                }
            }
            Board[kingSq] = king;
            Board[destSq] = captured;
        }
        if (addThisMove) { AddLegalMove (mlist, kingSq, destSq, EMPTY); }
        destPtr++;
    }
    // Now generate castling moves, if possible:
    if (genNonCaptures  &&  castling) { GenCastling (mlist); }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::AddPromotions():
//      Add promotion moves.
//      Called by GenPawnMoves() when a pawn can be promoted.
//
inline void
Position::AddPromotions (MoveList * mlist, squareT from, squareT dest)
{
    ASSERT (piece_Type (Board[from]) == PAWN);
    ASSERT (square_Rank (dest) == RANK_1  ||  square_Rank (dest) == RANK_8);

    AddLegalMove (mlist, from, dest, QUEEN);
    AddLegalMove (mlist, from, dest, ROOK);
    AddLegalMove (mlist, from, dest, BISHOP);
    AddLegalMove (mlist, from, dest, KNIGHT);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsValidEnPassant
//   Used to verify that an en passant  pawn capture is valid.
//   This is needed because illegal en passant captures can appear
//   legal according to calculations of pinned pieces. For example,
//   consider WK d5, WP e5, BP f5 (just moved there), BR h5 and
//   the en passant capture exf6 would be illegal.
inline bool
Position::IsValidEnPassant (squareT from, squareT to)
{
    ASSERT (from <= H8  &&  to <= H8);
    ASSERT (to == EPTarget);

    // Check that this en passant capture is legal:
    pieceT ownPawn = piece_Make(ToMove, PAWN);
    pieceT enemyPawn = piece_Make (color_Flip(ToMove), PAWN);
    squareT enemyPawnSq = (ToMove == WHITE) ? to - 8 : to + 8;
    ASSERT (Board[from] == ownPawn);
    ASSERT (Board[enemyPawnSq] == enemyPawn);
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
Position::GenPawnMoves (MoveList * mlist, squareT from,
                        directionT dir, SquareSet * sqset, genMovesT genType)
{
    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);
    directionT oppdir = direction_Opposite(dir);
    directionT forward;
    rankT promoRank;
    rankT secondRank;
    if (ToMove == WHITE) {
        forward = UP;    promoRank = RANK_8;  secondRank = RANK_2;
    } else  {
        forward = DOWN;  promoRank = RANK_1;  secondRank = RANK_7;
    }
    squareT dest;

    ASSERT (Board[from] == piece_Make (ToMove, PAWN));

    if (genNonCaptures
          &&  (dir == NULL_DIR  ||  dir == forward  ||  oppdir == forward)) {
        dest = square_Move (from, forward);
        if (Board[dest]==EMPTY && (sqset==NULL || sqset->Contains(dest))) {
            if (square_Rank(dest) == promoRank) {
                AddPromotions (mlist, from, dest);
            } else {
                AddLegalMove (mlist, from, dest, EMPTY);
            }
        }
        if (square_Rank(from) == secondRank  &&  Board[dest] == EMPTY) {
            dest = square_Move (dest, forward);
            if (Board[dest]==EMPTY  &&  (sqset==NULL || sqset->Contains(dest))) {
                AddLegalMove (mlist, from, dest, EMPTY);
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
    if (dir == NULL_DIR  ||  dir == capdir  ||  oppdir == capdir) {
        dest = square_Move (from, capdir);
        if (POSSIBLE_CAPTURE(dest)  &&  (sqset==NULL || sqset->Contains(dest))) {
            if (square_Rank(dest) == promoRank) {
                AddPromotions (mlist, from, dest);
            } else {
                AddLegalMove (mlist, from, dest, EMPTY);
            }
        }
    }
    capdir = forward | RIGHT;
    if (dir == NULL_DIR  ||  dir == capdir  ||  oppdir == capdir) {
        dest = square_Move (from, capdir);
        if (POSSIBLE_CAPTURE(dest)  &&  (sqset==NULL || sqset->Contains(dest))) {
            if (square_Rank(dest) == promoRank) {
                AddPromotions (mlist, from, dest);
            } else {
                AddLegalMove (mlist, from, dest, EMPTY);
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
uint
Position::GetHPSig (void)
{
    uint hpSig = 0;
    pieceT * b = &(Board[A2]);
    if (*b == WP) { hpSig |= 0x8000; }  b++;  /* a2 */
    if (*b == WP) { hpSig |= 0x4000; }  b++;  /* b2 */
    if (*b == WP) { hpSig |= 0x2000; }  b++;  /* c2 */
    if (*b == WP) { hpSig |= 0x1000; }  b++;  /* d2 */
    if (*b == WP) { hpSig |= 0x0800; }  b++;  /* e2 */
    if (*b == WP) { hpSig |= 0x0400; }  b++;  /* f2 */
    if (*b == WP) { hpSig |= 0x0200; }  b++;  /* g2 */
    if (*b == WP) { hpSig |= 0x0100; }        /* h2 */
    b = &(Board[A7]);
    if (*b == BP) { hpSig |= 0x0080; }  b++;  /* a7 */
    if (*b == BP) { hpSig |= 0x0040; }  b++;  /* b7 */
    if (*b == BP) { hpSig |= 0x0020; }  b++;  /* c7 */
    if (*b == BP) { hpSig |= 0x0010; }  b++;  /* d7 */
    if (*b == BP) { hpSig |= 0x0008; }  b++;  /* e7 */
    if (*b == BP) { hpSig |= 0x0004; }  b++;  /* f7 */
    if (*b == BP) { hpSig |= 0x0002; }  b++;  /* g7 */
    if (*b == BP) { hpSig |= 0x0001; }        /* h7 */
    return hpSig;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Init():
//      Initialise the position after it is constructed.
//
void
Position::Init (void)
{
    // Setting up a valid board is left to StdStart() or Clear().
    Board [COLOR_SQUARE] = EMPTY;
    Board [NULL_SQUARE] = END_OF_BOARD;
    LegalMoves = NULL;
    SANStrings = NULL;
    StrictCastling = true;

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
Position::Clear (void)
{
    int i;
    for (i=A1; i <= H8; i++) { Board[i] = EMPTY; }
    for (i=WK; i <= BP; i++) {
        Material[i] = 0;
        for (uint j=0; j < 8; j++) {
            NumOnRank[i][j] = NumOnFyle[i][j] = 0;
        }
        for (uint d=0; d < 15; d++) {
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
Position::StdStart (void)
{
    Clear();
    Material[WK] = Material[BK] = 1;
    Material[WQ] = Material[BQ] = 1;
    Material[WR] = Material[BR] = 2;
    Material[WB] = Material[BB] = 2;
    Material[WN] = Material[BN] = 2;
    Material[WP] = Material[BP] = 8;
    Count[WHITE] = Count[BLACK] = 16;

    AddToBoard(WK, E1);  List[WHITE][0] = E1;  ListPos[E1] = 0;
    AddToBoard(BK, E8);  List[BLACK][0] = E8;  ListPos[E8] = 0;
    AddToBoard(WR, A1);  List[WHITE][1] = A1;  ListPos[A1] = 1;
    AddToBoard(BR, A8);  List[BLACK][1] = A8;  ListPos[A8] = 1;
    AddToBoard(WN, B1);  List[WHITE][2] = B1;  ListPos[B1] = 2;
    AddToBoard(BN, B8);  List[BLACK][2] = B8;  ListPos[B8] = 2;
    AddToBoard(WB, C1);  List[WHITE][3] = C1;  ListPos[C1] = 3;
    AddToBoard(BB, C8);  List[BLACK][3] = C8;  ListPos[C8] = 3;
    AddToBoard(WQ, D1);  List[WHITE][4] = D1;  ListPos[D1] = 4;
    AddToBoard(BQ, D8);  List[BLACK][4] = D8;  ListPos[D8] = 4;
    AddToBoard(WB, F1);  List[WHITE][5] = F1;  ListPos[F1] = 5;
    AddToBoard(BB, F8);  List[BLACK][5] = F8;  ListPos[F8] = 5;
    AddToBoard(WN, G1);  List[WHITE][6] = G1;  ListPos[G1] = 6;
    AddToBoard(BN, G8);  List[BLACK][6] = G8;  ListPos[G8] = 6;
    AddToBoard(WR, H1);  List[WHITE][7] = H1;  ListPos[H1] = 7;
    AddToBoard(BR, H8);  List[BLACK][7] = H8;  ListPos[H8] = 7;

    for (uint i=0; i < 8; i++) {
        AddToBoard(WP, A2+i); List[WHITE][i+8] = A2+i; ListPos[A2+i] = i+8;
        AddToBoard(BP, A7+i); List[BLACK][i+8] = A7+i; ListPos[A7+i] = i+8;
    }

    Castling = 0;
    SetCastling (WHITE, QSIDE, true);  SetCastling (WHITE, KSIDE, true);
    SetCastling (BLACK, QSIDE, true);  SetCastling (BLACK, KSIDE, true);
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
Position::IsStdStart ()
{
    if (ToMove != WHITE
          ||  Hash != stdStartHash  ||  PawnHash != stdStartPawnHash
          ||  GetCount(WHITE) != 16  ||  GetCount(BLACK) != 16
          ||  RankCount(WP,RANK_2) != 8  ||  RankCount(BP,RANK_7) != 8
          ||  RankCount(WN,RANK_1) != 2  ||  RankCount(BN,RANK_8) != 2
          ||  !GetCastling(WHITE,KSIDE)  ||  !GetCastling(WHITE,QSIDE)
          ||  !GetCastling(BLACK,KSIDE)  ||  !GetCastling(BLACK,QSIDE)) {
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
Position::AddPiece (pieceT p, squareT sq)
{
    ASSERT (p != EMPTY);
    colorT c = piece_Color(p);

    if (Count[c] == 16) { return ERROR_PieceCount; }
    ASSERT(Count[c] <= 15);
    if (piece_Type(p) == KING) {
        // Check there is not already a King:
        if (Material[p] > 0) { return ERROR_PieceCount; }

        // King is always at the start of the piecelist, so move the piece
        // already at location 0 if there is one:
        if (Count[c] > 0) {
            squareT oldsq = List[c][0];
            List[c][Count[c]] = oldsq;
            ListPos[oldsq] = Count[c];
        }
        List[c][0] = sq;
        ListPos[sq] = 0;
    } else {
        ListPos[sq] = Count[c];
        List[c][Count[c]] = sq;
    }
    Count[c]++;
    Material[p]++;
    AddToBoard (p, sq);
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
Position::CalcPins (void)
{
    Pinned[ 0] = Pinned[ 1] = Pinned[ 2] = Pinned[ 3] = Pinned[ 4] =
    Pinned[ 5] = Pinned[ 6] = Pinned[ 7] = Pinned[ 8] = Pinned[ 9] =
    Pinned[10] = Pinned[11] = Pinned[12] = Pinned[13] = Pinned[14] =
    Pinned[15] = NULL_DIR;

    squareT kingSq = GetKingSquare (ToMove);
    colorT enemy = color_Flip (ToMove);
    pieceT enemyQueen  = piece_Make (enemy, QUEEN);
    pieceT enemyRook   = piece_Make (enemy, ROOK);
    pieceT enemyBishop = piece_Make (enemy, BISHOP);

    // Pins and checks from Bishops/Queens/Rooks:
    fyleT fyle = square_Fyle (kingSq);
    if (FyleCount(enemyQueen,fyle) + FyleCount(enemyRook,fyle) > 0) {
        CalcPinsDir (UP, ROOK);
        CalcPinsDir (DOWN, ROOK);
    }
    rankT rank = square_Rank (kingSq);
    if (RankCount(enemyQueen,rank) + RankCount(enemyRook,rank) > 0) {
        CalcPinsDir (LEFT, ROOK);
        CalcPinsDir (RIGHT, ROOK);
    }
    leftDiagT ld = square_LeftDiag (kingSq);
    if (LeftDiagCount(enemyQueen,ld) + LeftDiagCount(enemyBishop,ld) > 0) {
        CalcPinsDir (UP_LEFT, BISHOP);
        CalcPinsDir (DOWN_RIGHT, BISHOP);
    }
    rightDiagT rd = square_RightDiag (kingSq);
    if (RightDiagCount(enemyQueen,rd) + RightDiagCount(enemyBishop,rd) > 0) {
        CalcPinsDir (UP_RIGHT, BISHOP);
        CalcPinsDir (DOWN_LEFT, BISHOP);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenPieceMoves():
//      Generates moves for a nonpawn, nonking piece.
//      If sqset != NULL, moves must be to a square in sqset.<
void
Position::GenPieceMoves (MoveList * mlist, squareT fromSq,
                         SquareSet * sqset, bool capturesOnly)
{
    colorT c = ToMove;
    pieceT p = Board[fromSq];
    pieceT ptype = piece_Type(p);
    ASSERT (p != EMPTY  &&  ptype != KING  &&  ptype != PAWN);

    if (ptype == KNIGHT) {
        GenKnightMoves (mlist, c, fromSq, sqset, capturesOnly);
        return;
    }
    if (ptype != BISHOP) {
        GenSliderMoves (mlist, c, fromSq, UP, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, DOWN, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, LEFT, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, RIGHT, sqset, capturesOnly);
    }
    if (ptype != ROOK) {
        GenSliderMoves (mlist, c, fromSq, UP_LEFT, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, DOWN_LEFT, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, UP_RIGHT, sqset, capturesOnly);
        GenSliderMoves (mlist, c, fromSq, DOWN_RIGHT, sqset, capturesOnly);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GenerateMoves
//    Generate the legal moves list.
//    If the specified pieceType is not EMPTY, then only legal
//    moves for that type of piece are generated.
void
Position::GenerateMoves (MoveList * mlist, pieceT pieceType,
                         genMovesT genType, bool maybeInCheck)
{
    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);
    bool capturesOnly = !genNonCaptures;

    uint mask = 0;
    if (pieceType != EMPTY) {
        mask = 1 << pieceType;
    } else {
        mask = (1 << KING) | (1 << QUEEN) | (1 << ROOK)
             | (1 << BISHOP) | (1 << KNIGHT) | (1 << PAWN);
    }

    // Use the objects own move list if none was provided:
    if (mlist == NULL) {
        ClearLegalMoves();
        mlist = LegalMoves;
    }
    mlist->Clear();

    // Compute which pieces of the side to move are pinned to the king:
    CalcPins();

    // Determine if the side to move is in check and find where the
    // checking pieces are, unless the caller has passed maybeInCheck=false
    // indicating it is CERTAIN the side to move is not in check here.

    uint numChecks = 0;
    if (maybeInCheck) {
        SquareList checkSquares;
        numChecks = CalcNumChecks (GetKingSquare(ToMove), &checkSquares);
        if (numChecks > 0) {
            // The side to move IS in check:
            GenCheckEvasions (mlist, pieceType, genType, &checkSquares);
            return;
        }
    }

    // The side to move is NOT in check. Iterate over each non-king
    // piece, and then generate King moves last of all:

    uint npieces = Count[ToMove];
    for (uint x = 1; x < npieces; x++) {
        squareT sq = List[ToMove][x];
        pieceT p = Board[sq];
        pieceT ptype = piece_Type(p);
        if (! (mask & (1 << ptype))) { continue; }
        directionT pinned = Pinned[x];

        // If Pinned[x] == dir (not NULL_DIR), x can ONLY move along
        // that direction or its opposite.

        if (pinned != NULL_DIR) {  // piece x is pinned to king
            if (ptype == PAWN) {
                GenPawnMoves (mlist, sq, pinned, NULL, genType);
            } else if (ptype == KNIGHT) {
                // do nothing -- pinned knights cannot move
            } else {
                // Piece is a pinned Queen/Rook/Bishop. Only generate
                // moves along the pinned direction and its opposite:
                if (ptype == QUEEN
                      || (ptype == ROOK && !direction_IsDiagonal(pinned))
                      || (ptype == BISHOP && direction_IsDiagonal(pinned))) {
                    GenSliderMoves (mlist, ToMove, sq, pinned, NULL, capturesOnly);
                    GenSliderMoves (mlist, ToMove, sq, dirOpposite[pinned],
                                    NULL, capturesOnly);
                }
            }
        } else {
            // This piece is free to move anywhere
            if (ptype == PAWN) {
                GenPawnMoves (mlist, sq, NULL_DIR, NULL, genType);
            } else {
                // Knight/Queen/Bishop/Rook:
                GenPieceMoves (mlist, sq, NULL, capturesOnly);
            }
        }
    }

    // Lastly, king moves...
    if (mask & (1 << KING)) {
        bool castling = !numChecks;
        GenKingMoves (mlist, genType, castling);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsLegalMove
//   Determines whether the specified move is legal in this position,
//   without requiring move generation (except for castling moves).
bool
Position::IsLegalMove (simpleMoveT * sm) {
    squareT from = sm->from;
    squareT to = sm->to;
    if (from > H8  ||  to > H8) { return false; }
    if (from == to) { return false; }
    pieceT mover = Board[from];
    pieceT captured = Board[to];
    if (piece_Color(mover) != ToMove) { return false; }
    if (piece_Color(captured) == ToMove) { return false; }
    if (sm->movingPiece != mover) { return false; }
    mover = piece_Type (mover);
    if (sm->promote != EMPTY  &&  mover != PAWN) { return false; }

    if (mover == PAWN) {
        rankT rfrom = square_Rank(from);
        rankT rto = square_Rank(to);
        if (ToMove == BLACK) { rfrom = RANK_8 - rfrom; rto = RANK_8 - rto; }
        int rdiff = (int)rto - (int)rfrom;
        int fdiff = (int)square_Fyle(to) - (int)square_Fyle(from);
        if (rdiff < 1  ||  rdiff > 2) { return false; }
        if (fdiff < -1  ||  fdiff > 1) { return false; }
        if (fdiff == 0) {  // Pawn push:
            if (captured != EMPTY) { return false; }
            if (rdiff == 2) {  // Two-square push:
                if (rfrom != RANK_2) { return false; }
                // Make sure the square in between is empty:
                squareT midsquare = from + ((to - from) / 2);
                if (Board[midsquare] != EMPTY) { return false; }
            }
        } else {  // Pawn capture:
            if (rdiff != 1) { return false; }
            if (captured == EMPTY) {
                // It must be en passant, or illegal
                if (to != EPTarget) { return false; }
            }
        }
        // Check the promotion piece:
        if (rto == RANK_8) {
            pieceT p = sm->promote;
            if (p != QUEEN  &&  p != ROOK  &&  p != BISHOP  &&  p != KNIGHT) {
                return false;
            }
        } else {
            if (sm->promote != EMPTY) { return false; }
        }

    } else if (piece_IsSlider(mover)) {
        // Make sure the direction is valid:
        directionT dir = sqDir[from][to];
        if (dir == NULL_DIR) { return false; }
        if (mover == ROOK  &&  direction_IsDiagonal(dir)) { return false; }
        if (mover == BISHOP  &&  !direction_IsDiagonal(dir)) { return false; }
        int delta = direction_Delta (dir);
        // Make sure all the in-between squares are empty:
        squareT dest = from + delta;
        while (dest != to) {
            if (Board[dest] != EMPTY) { return false; }
            dest += delta;
        }

    } else if (mover == KNIGHT) {
        if (! square_IsKnightHop (from, to)) { return false; }

    } else /* (mover == KING) */ {
        colorT enemy = color_Flip(ToMove);
        if (square_Adjacent (to, GetKingSquare(enemy))) { return false; }
        if (! square_Adjacent (from, to)) {
            // The move must be castling, or illegal.
            if (IsKingInCheck()) { return false; }
            MoveList mlist;
            GenCastling (&mlist);
            return (mlist.Find(sm) >= 0);
        }
    }

    // The move looks good, but does it leave the king in check?
    squareT kingSq = (mover == KING) ? to : GetKingSquare(ToMove);
    colorT enemy = color_Flip(ToMove);
    DoSimpleMove (sm);
    uint nchecks = CalcAttacks (enemy, kingSq, NULL);
    UndoSimpleMove (sm);
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
Position::MatchLegalMove (MoveList * mlist, pieceT mask, squareT target)
{
    uint x;

    // This function isn't for Pawn or King moves!
    ASSERT (mask != PAWN  &&  mask != KING);

    if (mlist == NULL) {
        ClearLegalMoves();
        mlist = LegalMoves;
    }
    mlist->Clear();

    uint count = 0;
    uint total = Material[piece_Make(ToMove, mask)];

    pieceT p, pt, captured;
    squareT kingSq = GetKingSquare(ToMove);
    directionT dir;

    uint tryMove = 0;

    // First, verify that the target square is empty or contains
    // an enemy piece:

    p = Board[target];
    if (p != EMPTY  &&  piece_Color(p) == ToMove) {
        return;
    }

    // Loop through looking for pieces of type "mask". We start at 1
    // since the King is always the piece at position 0 in the list.

    squareT * sqPtr = &(List[ToMove][1]);
    for (x=1;  x < Count[ToMove]  &&  count < total;  x++, sqPtr++) {
        p = Board[*sqPtr];
        pt = piece_Type(p);
        if (pt == mask) {

            // Increment count so we stop when we've seen all the
            // Material[p] pieces of this type.

            tryMove = 0;
            count++;
            squareT sq;

            switch (pt) {
            case KNIGHT:
                if (square_IsKnightHop (*sqPtr, target)) { tryMove = 1; }
                break;

            case ROOK:
                dir = sqDir[*sqPtr][target];
                if (dir != NULL_DIR  &&  !direction_IsDiagonal(dir)) {
                    sq = square_Move (*sqPtr, dir);
                    tryMove = 1;
                    while (sq != target) {
                        if (Board[sq] != EMPTY) { // oops, piece in the way
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move (sq, dir);
                    }
                }
                break;

            case BISHOP:
                dir = sqDir[*sqPtr][target];
                if (direction_IsDiagonal(dir)) {
                    sq = square_Move (*sqPtr, dir);
                    tryMove = 1;
                    while (sq != target) {
                        if (Board[sq] != EMPTY) { // oops, piece in the way
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move (sq, dir);
                    }
                }
                break;

            case QUEEN:
                dir = sqDir[*sqPtr][target];
                if (dir != NULL_DIR) {  // Try the move!
                    sq = square_Move (*sqPtr, dir);
                    tryMove = 1;
                    while (sq != target) {
                        if (Board[sq] != EMPTY) { // oops, piece in the way
                            tryMove = 0;
                            break;
                        }
                        sq = square_Move (sq, dir);
                    }
                }
                break;

            default:  // Should never happen
                ASSERT(0);
            }

            // Now, if tryMove is 1, the piece can get to target. We need
            // to see if the move is legal or leaves the king in check.

            if (tryMove == 1) {
                captured = Board[target];
                Board[target] = p;
                Board[*sqPtr] = EMPTY;
                if (CalcNumChecks (kingSq) > 0)  { tryMove = 0; }
                Board[*sqPtr] = p;
                Board[target] = captured;
                if (tryMove == 1)  { AddLegalMove (mlist, *sqPtr, target, EMPTY); }
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
Position::MatchPawnMove (MoveList * mlist, fyleT fromFyle, squareT to, pieceT promote)
{
    mlist->Clear();

    sint diff = (int)square_Fyle(to) - (int)fromFyle;
    if (diff < -1  ||  diff > 1) { return ERROR_InvalidMove; }
    pieceT pawn;
    rankT toRank = square_Rank(to);
    fyleT toFyle = square_Fyle(to);
    rankT promoteRank = (ToMove == WHITE ? RANK_8 : RANK_1);

    // from is the from square; backup is the alternative from square
    // for a pawn move two squares forward.

    squareT from, backup = NS;

    if (ToMove == WHITE) {
        pawn = WP;
        promoteRank = RANK_8;
        if (toRank < RANK_3) { return ERROR_InvalidMove; }
        from = square_Make(fromFyle, toRank - 1);
        if (toRank == RANK_4  &&  fromFyle == toFyle) { backup = to - 16; }
    } else {
        pawn = BP;
        promoteRank = RANK_1;
        if (toRank > RANK_6) { return ERROR_InvalidMove; }
        from = square_Make(fromFyle, toRank + 1);
        if (toRank == RANK_5  &&  fromFyle == toFyle) { backup = to + 16; }
    }

    // See if the promotion piece is valid:

    if (toRank == promoteRank) {
        if (promote == EMPTY)  { return ERROR_InvalidMove; }
    } else {
        if (promote != EMPTY)  { return ERROR_InvalidMove; }
    }

    if (Board[from] != pawn) {
        // No match; but it could be a foward-two-squares move:
        if (backup == NS || Board[from] != EMPTY || Board[backup] != pawn) {
            // A forward-two-squares move is impossible.
            return ERROR_InvalidMove;
        }
        from = backup;
    }

    // OK, now 'from' is the only possible from-square. Is the move legal?
    // We make the move on the board and see if the King is in check.

    uint legal = 0;
    if (fromFyle == toFyle) {
        // Not a capture:

        if (Board[to] != EMPTY) { return ERROR_InvalidMove; }
        Board[to] = pawn;  Board[from] = EMPTY;
        if (CalcNumChecks (GetKingSquare()) == 0) {
            legal = 1;
        }
       Board[to] = EMPTY; Board[from] = pawn;

    } else {
        // It is a capture -- is it legal?

        pieceT captured = Board[to];
        if (captured == EMPTY) {
            // Must be an en passant or illegal move.
            if (to != EPTarget) { return ERROR_InvalidMove; }
            squareT epSquare = square_Make(toFyle, square_Rank(from));

            pieceT enemyPawn = piece_Make (color_Flip(ToMove), PAWN);
            // If following assert fails, eptarget was corrupt
            ASSERT (Board[epSquare] == enemyPawn);

            Board[to] = pawn; Board[from] = EMPTY;
            Board[epSquare] = EMPTY;
            Material[enemyPawn] --;
            if (CalcNumChecks (GetKingSquare()) == 0) { legal = 1; }
            Board[epSquare] = enemyPawn;
            Board[to] = EMPTY;
            Board[from] = pawn;
            Material[enemyPawn]++;

        } else {
            if (piece_Color(captured) == ToMove) {
                // Capturing a friendly!
                return ERROR_InvalidMove;
            } else {
                // A regular capture. See if it leaves King in check:
                Board[to] = pawn;  Board[from] = EMPTY;
                Material[captured]--;
                if (CalcNumChecks (GetKingSquare()) == 0) {
                    legal = 1;
                }
                Material[captured]++;
                Board[to] = captured; Board[from] = pawn;
            }
        }
    }

    if (legal == 1) {
        AddLegalMove (mlist, from, to, promote);
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
Position::MatchKingMove (MoveList * mlist, squareT target)
{
    mlist->Clear();
    squareT kingSq = GetKingSquare(ToMove);
    sint diff = (int)target - (int) kingSq;

    // Valid diffs are: -9, -8, -7, -2, -1, 1, 2, 7, 8, 9. (-2,2: Castling)

    if (diff < -9  ||  diff > 9) { return ERROR_InvalidMove; }
    if (diff > -7  &&  diff < -2) { return ERROR_InvalidMove; }
    if (diff > 2  &&  diff < 7) { return ERROR_InvalidMove; }
    if (diff == 0) { return ERROR_InvalidMove; }

    if (diff == 2) { // KingSide Castling
        if (kingSq != (ToMove == WHITE ? E1 : E8)) {
            return ERROR_InvalidMove;
        }
        if (StrictCastling  &&  ! GetCastling (ToMove, KSIDE)) {
            return ERROR_InvalidMove;
        }

        // XXX We also need to verify that the target square does not
        //     lie adjacent to the location of the enemy king!

        if (Board[kingSq + 1] != EMPTY  ||  Board[kingSq + 2] != EMPTY
            ||  CalcNumChecks(kingSq) > 0
            ||  CalcNumChecks(kingSq + 1) > 0
            ||  CalcNumChecks(kingSq + 2) > 0) {
            return ERROR_InvalidMove;
        }
        AddLegalMove (mlist, kingSq, target, EMPTY);
        return OK;
    }

    if (diff == -2) { // Queenside Castling
        if (kingSq != (ToMove == WHITE ? E1 : E8)) {
            return ERROR_InvalidMove;
        }
        if (StrictCastling  &&  ! GetCastling (ToMove, QSIDE)) {
            return ERROR_InvalidMove;
        }
        if (Board[kingSq - 1] != EMPTY  ||  Board[kingSq - 2] != EMPTY
            ||  Board[kingSq - 3] != EMPTY
            ||  CalcNumChecks(kingSq) > 0
            ||  CalcNumChecks(kingSq - 1) > 0
            ||  CalcNumChecks(kingSq - 2) > 0) {
            return ERROR_InvalidMove;
        }
        AddLegalMove (mlist, kingSq, target, EMPTY);
        return OK;
    }
    pieceT captured = Board[target];
    if (piece_Color(captured) == ToMove) {
        // Capturing a friendly piece!
        return ERROR_InvalidMove;
    }

    // Now make the move on the Board and Material lists, and see if it
    // leaves the King in check:
    // XXX We should also check for adjacency to enemy King!!

    Board[target] = piece_Make(ToMove, KING);
    Board[kingSq] = EMPTY;
    if (captured != EMPTY) { Material[captured]--; }
    uint legal = 0;
    if (CalcNumChecks(target) == 0) { legal = 1; }
    if (captured != EMPTY) { Material[captured]++; }
    Board[target] = captured;
    Board[kingSq] = piece_Make(ToMove, KING);
    if (legal == 1) {
        AddLegalMove (mlist, kingSq, target, EMPTY);
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
Position::GenCheckEvasions (MoveList * mlist, pieceT mask, genMovesT genType,
                            SquareList * checkSquares)
{
    uint numChecks = checkSquares->Size();
    
    // Assert that king IS actually in check:    
    ASSERT (numChecks > 0);

    bool genNonCaptures = ((genType & GEN_NON_CAPS) != 0);
    bool capturesOnly = !genNonCaptures;
    mlist->Clear();

    squareT king = GetKingSquare (ToMove);

    // if it's double check, we can ONLY move the king
    if (numChecks == 1) {
        // OK, it is NOT a double check
        // Try to block piece/capture piece. Remember en passant!
        // First, generate a list of targets: squares between the king
        // and attacker to block, and the attacker's square.

        squareT attackSq = checkSquares->Get(0);
        directionT dir = sqDir[king][attackSq];
        SquareSet targets;  // Set of blocking/capturing squares.
        targets.Add(attackSq);

        // Now add squares we can might be able to block on.
        if (dir != NULL_DIR) {
            squareT sq = square_Move (king, dir);
            while (sq != attackSq) {
                if (Board[sq] == EMPTY) { targets.Add(sq); }
                sq = square_Move (sq, dir);
            }
        }

        // Try each non-King piece in turn. If a piece is pinned to
        // the king, don't bother since it cannot possibly block or
        // capture the piece that is giving check!

        uint numPieces = Count[ToMove];
        for (uint p2 = 1; p2 < numPieces; p2++) {
            squareT from = List[ToMove][p2];
            pieceT p2piece = Board[from];
            if (Pinned[p2] != NULL_DIR) { continue; }
            if (mask == EMPTY  ||  mask == piece_Type(p2piece)) {
                if (piece_Type(p2piece) == PAWN) {
                    GenPawnMoves (mlist, from, NULL_DIR, &targets, genType);
                    // Capturing a pawn en passant will only get out
                    // of check if the pawn that moved two squares
                    // is doing the checking. If it is not, that means
                    // a discovered check with the last pawn move so
                    // taking en passant cannot get out of check.
                    if (EPTarget != NULL_SQUARE) {
                        squareT pawnSq = (ToMove == WHITE ? EPTarget - 8 : EPTarget + 8);
                        if (pawnSq == attackSq) {
                            SquareSet epset;
                            epset.Add(EPTarget);
                            GenPawnMoves (mlist, from, NULL_DIR, &epset, genType);
                        }
                    }
                } else {
                    GenPieceMoves (mlist, from, &targets, capturesOnly);
                }
            }
        }  // end of search for captures/blocks
    }

    // Now king moves -- just compute them normally:
    if (mask == EMPTY  ||  mask == KING) { GenKingMoves(mlist, genType, false); }

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
uint
Position::CalcAttacks (colorT side, squareT target, SquareList * fromSquares)
{
    // If squares is NULL, caller doesn't want a list of the squares of
    // attacking pieces. To avoid comparing fromSquares with NULL every time
    // we find a check, we set up a local array to use instead if fromSquares
    // is NULL.
    SquareList fromSqs;
    if (fromSquares == NULL) { fromSquares = &fromSqs; }

    fromSquares->Clear();
    squareT sq;

    // Bishop/Queen/Rook attacks: look at each of the 8 directions
    pieceT queen, rook, bishop, knight;
    if (side == WHITE) {
        queen = WQ; rook = WR; bishop = WB; knight = WN;
    } else {
        queen = BQ; rook = BR; bishop = BB; knight = BN;
    }

    uint numQueensRooks = Material[queen] + Material[rook];
    uint numQueensBishops = Material[queen] + Material[bishop];

    // We only bother if there are any sliding pieces of each type:
    if (numQueensRooks > 0) {
        fyleT fyle = square_Fyle (target);
        rankT rank = square_Rank (target);
        directionT dirs[4];
        uint ndirs = 0;
        if (FyleCount(queen,fyle) + FyleCount(rook,fyle) > 0) {
            dirs[ndirs++] = UP;  dirs[ndirs++] = DOWN;
        }
        if (RankCount(queen,rank) + RankCount(rook,rank) > 0) {
            dirs[ndirs++] = LEFT; dirs[ndirs++] = RIGHT;
        }
        for (uint i = 0; i < ndirs; i++) {
            directionT dir = dirs[i];
            int delta = direction_Delta (dir);
            squareT dest = target;
            squareT last = square_Last (target, dir);

            while (dest != last) {
                dest += delta;
                pieceT p = Board[dest];
                if (p == EMPTY) {
                    // empty square: keep searching
                } else if (p == queen  ||  p == rook) {
                    // Found an attacking piece
                    fromSquares->Add(dest);
                    break;
                } else {
                    // Found a piece, but not a queen or rook
                    break;
                }
            }
        }
    }

    // Now diagonal sliders: Queens/Bishops:
    if (numQueensBishops > 0) {
        leftDiagT left = square_LeftDiag (target);
        rightDiagT right = square_RightDiag (target);
        directionT dirs[4];
        uint ndirs = 0;
        if (LeftDiagCount(queen,left) + LeftDiagCount(bishop,left) > 0) {
            dirs[ndirs++] = UP_LEFT;  dirs[ndirs++] = DOWN_RIGHT;
        }
        if (RightDiagCount(queen,right) + RightDiagCount(bishop,right) > 0) {
            dirs[ndirs++] = UP_RIGHT;  dirs[ndirs++] = DOWN_LEFT;
        }
        for (uint i = 0; i < ndirs; i++) {
            directionT dir = dirs[i];
            int delta = direction_Delta (dir);
            squareT dest = target;
            squareT last = square_Last (target, dir);

            while (dest != last) {
                dest += delta;
                pieceT p = Board[dest];
                if (p == EMPTY) {
                    // empty square: keep searching
                } else if (p == queen  ||  p == bishop) {
                    // Found an attacking piece
                    fromSquares->Add(dest);
                    break;
                } else {
                    // Found a piece, but not an enemy queen or bishop
                    break;
                }
            }
        }
    }

    // Now knight checks: we only bother if there is a knight on the
    // opposite square color of the target square color.
    if (Material[knight] > 0
         &&  SquareColorCount(knight, color_Flip(square_Color(target))) > 0) {
        const squareT * destPtr = knightAttacks[target];
        while (true) {
            squareT dest = *destPtr;
            if (dest == NULL_SQUARE) { break; }
            if (Board[dest] == knight) {
                fromSquares->Add(dest);
            }
            destPtr++;
        }
    }

    // Now pawn attacks:
    if (side == WHITE) {
        if (square_Rank(target) != RANK_1) {  //if (Material[WP] > 0) {
            sq = square_Move (target, DOWN_LEFT);
            if (Board[sq] == WP)  {
                fromSquares->Add(sq);
            }
            sq = square_Move (target, DOWN_RIGHT);
            if (Board[sq] == WP)  {
                fromSquares->Add(sq);
            }
        }
    } else {
        if (square_Rank(target) != RANK_8) {  //if (Material[BP] > 0) {
            sq = square_Move (target, UP_LEFT);
            if (Board[sq] == BP)  {
                fromSquares->Add(sq);
            }
            sq = square_Move (target, UP_RIGHT);
            if (Board[sq] == BP)  {
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
Position::IsKingInCheckDir (directionT dir)
{
    ASSERT (dir != NULL_DIR);
    squareT kingSq = GetKingSquare(ToMove);
    colorT enemy = color_Flip(ToMove);
    bool isDiagonal = direction_IsDiagonal(dir);
    pieceT queen = piece_Make (enemy, QUEEN);
    pieceT slider = piece_Make (enemy, (isDiagonal ? BISHOP : ROOK));

    // First, make sure the enemy has sliding pieces that could give check:
    uint nSliders = PieceCount(queen) + PieceCount(slider);
    if (nSliders == 0) { return false; }

    // Now make sure the enemy has a sliding piece on the appropriate
    // rank, file or diagonal:
    fyleT fyle = square_Fyle (kingSq);
    rankT rank = square_Rank (kingSq);
    leftDiagT ldiag = square_LeftDiag (kingSq);
    rightDiagT rdiag = square_RightDiag (kingSq);

    switch (dir) {
    case UP:
    case DOWN:
        nSliders = FyleCount(queen,fyle) + FyleCount(slider,fyle);
        break;
    case LEFT:
    case RIGHT:
        nSliders = RankCount(queen,rank) + RankCount(slider,rank);
        break;
    case UP_LEFT:
    case DOWN_RIGHT:
        nSliders = LeftDiagCount(queen,ldiag) + LeftDiagCount(slider,ldiag);
        break;
    case UP_RIGHT:
    case DOWN_LEFT:
        nSliders = RightDiagCount(queen,rdiag) + RightDiagCount(slider,rdiag);
        break;
    }
    if (nSliders == 0) { return false; }

    // Now move along the specified direction looking for a checking piece:
    squareT dest = kingSq;
    squareT last = square_Last (kingSq, dir);
    int delta = direction_Delta (dir);

    while (dest != last) {
        dest += delta;
        pieceT p = Board[dest];
        if (p == EMPTY) {
             // empty square: keep searching
        } else if (p == queen  ||  p == slider) {
            // Found an checking slider piece
            return true;
        } else {
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
Position::IsKingInCheck (simpleMoveT * sm)
{
    if (sm == NULL) { return IsKingInCheck(); }

    squareT kingSq = GetKingSquare(ToMove);
    pieceT p = piece_Type (sm->movingPiece);
    if (sm->promote != EMPTY) { p = piece_Type(sm->promote); }

    // No optimization of the last move was castling:
    if (p == KING  &&  square_Fyle(sm->from) == E_FYLE) {
        fyleT toFyle = square_Fyle(sm->to);
        if (toFyle == C_FYLE  ||  toFyle == G_FYLE) {
            return IsKingInCheck();
        }
    }
    // No optimization for en passant capture:
    if (p == PAWN  &&  piece_Type(sm->capturedPiece) == PAWN) {
        rankT fromRank = square_Rank(sm->from);
        rankT capturedRank = square_Rank(sm->capturedSquare);
        if (fromRank == capturedRank) { return IsKingInCheck(); }
    }

    if (p == PAWN) {
        if (ToMove == WHITE) {
            if (Material[BP] > 0) {
                squareT sq = square_Move (kingSq, UP_LEFT);
                if (Board[sq] == BP)  { return true; }
                sq = square_Move (kingSq, UP_RIGHT);
                if (Board[sq] == BP)  { return true; }
            }
        } else {
            if (Material[WP] > 0) {
                squareT sq = square_Move (kingSq, DOWN_LEFT);
                if (Board[sq] == WP)  { return true; }
                sq = square_Move (kingSq, DOWN_RIGHT);
                if (Board[sq] == WP)  { return true; }
            }
        }
    } else if (p == KNIGHT) {
        if (square_IsKnightHop (kingSq, sm->to)) { return true; }
    } else if (p == KING) {
        // A king cannot directly check its adversary.
    } else {
        // A sliding piece:
        directionT toDir = sqDir[kingSq][sm->to];
        if (toDir != NULL_DIR  &&  IsKingInCheckDir(toDir)) { return true; }
    }

    // Now look for a discovered check from a sliding piece:
    directionT dir = sqDir[kingSq][sm->from];
    if (dir != NULL_DIR  &&  IsKingInCheckDir(dir)) { return true; }

    ASSERT (IsKingInCheck() == false);
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Mobility
//    Returns the number of squares a rook or bishop of the specified
//    color would attack from the specified square.
uint
Position::Mobility (pieceT p, colorT color, squareT from)
{
    ASSERT (p == ROOK  ||  p == BISHOP);
    uint mobility = 0;
    directionT rookDirs[4] = { UP, DOWN, LEFT, RIGHT };
    directionT bishopDirs[4]
        = { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
    directionT * dirPtr = (p == ROOK ? rookDirs : bishopDirs);

    for (uint i=0; i < 4; i++) {
        directionT dir = dirPtr[i];
        int delta = direction_Delta (dir);
        squareT dest = from;
        squareT last = square_Last (from, dir);

        while (dest != last) {
            dest += delta;
            pieceT p = Board[dest];
            if (p == EMPTY) {  // Empty square
                mobility++;
            } else if (piece_Color(p) == color) {  // Friendly piece
                break;  // Finished with this direction.
            } else {  // Enemy piece
                mobility++;
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
pieceT
Position::SmallestDefender (colorT color, squareT target)
{
    SquareList defenderSquares;
    pieceT defenders [16];
    uint numDefenders = CalcAttacks (color, target, &defenderSquares);

    // If the square is undefended, just return EMPTY:
    if (numDefenders == 0) { return EMPTY; }

    uint i;
    for (i=0; i < numDefenders; i++) {
       defenders[i] = Board[defenderSquares.Get(i)];
    }
    // Look for pawns first:
    for (i=0; i < numDefenders; i++) {
        if (piece_Type (defenders[i]) == PAWN) { return PAWN; }
    }
    // Look for knights then bishops:
    for (i=0; i < numDefenders; i++) {
        if (piece_Type (defenders[i]) == KNIGHT) { return KNIGHT; }
    }
    for (i=0; i < numDefenders; i++) {
        if (piece_Type (defenders[i]) == BISHOP) { return BISHOP; }
    }
    // Look for rooks then queens:
    for (i=0; i < numDefenders; i++) {
        if (piece_Type (defenders[i]) == ROOK) { return ROOK; }
    }
    for (i=0; i < numDefenders; i++) {
        if (piece_Type (defenders[i]) == QUEEN) { return QUEEN; }
    }

    // If we get here, it must be defended only by the king:
    ASSERT (numDefenders == 1  &&  piece_Type(defenders[i]) == KING);
    return KING;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsKingInMate():
//      Quick check if king is in mate.
//
bool
Position::IsKingInMate (void)
{
    SquareList checkSquares;
    uint numChecks = CalcNumChecks (GetKingSquare(ToMove), &checkSquares);
    if (numChecks == 0) { return false; }
    CalcPins ();
    MoveList mlist;
    GenCheckEvasions (&mlist, EMPTY, GEN_ALL_MOVES, &checkSquares);
    if (mlist.Size() == 0) { return true; }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::IsLegal()
//   Verifies the position as being legal.
//   Returns false for any of the following:
//     - if the two kings are adjacent;
//     - if there are any pawns on the 1st/8th rank;
//     - if the side to move is already checking the enemy king.
bool
Position::IsLegal (void)
{
    squareT stmKing = GetKingSquare();
    squareT enemyKing = GetEnemyKingSquare();
    if (square_Adjacent (stmKing, enemyKing)) { printf ("Kings adjacent\n"); return false; }
    if (RankCount (WP, RANK_1) != 0) { return false; }
    if (RankCount (WP, RANK_8) != 0) { return false; }
    if (RankCount (BP, RANK_1) != 0) { return false; }
    if (RankCount (BP, RANK_8) != 0) { return false; }
    if (CalcAttacks (ToMove, enemyKing, NULL) > 0) {
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
Position::IsPromoMove (squareT from, squareT to)
{
    rankT seventh, eighth;
    pieceT pawn;
    if (ToMove == WHITE) { seventh = RANK_7; eighth = RANK_8; pawn = WP; }
    else { seventh = RANK_2; eighth = RANK_1; pawn = BP; }
    rankT fromR, toR;
    fromR = square_Rank(from);
    toR = square_Rank(to);
    if ( (fromR == seventh  &&  toR == eighth  &&  Board[from] == pawn)  ||
         (toR == seventh  &&  fromR == eighth  &&  Board[to] == pawn) ) {
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
Position::DoSimpleMove (simpleMoveT * sm)
{
    ASSERT (sm != NULL);
    squareT from = sm->from;
    squareT to = sm->to;
    pieceT p = Board[from];
    pieceT ptype = piece_Type(p);
    colorT enemy = color_Flip(ToMove);
    ASSERT (p != EMPTY);

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
    if (isNullMove(sm)) {
        ToMove = enemy;
        EPTarget = NULL_SQUARE;
        return;
    }

    // Handle en passant capture:

    if (ptype == PAWN  &&  sm->capturedPiece == EMPTY
            && square_Fyle(from) != square_Fyle(to)) {

        // This was an EP capture. We do not need to check it was a capture
        // since if a pawn lands on EPTarget it must capture to get there.

        pieceT enemyPawn = piece_Make(enemy, PAWN);
        sm->capturedSquare = (ToMove == WHITE ? (to - 8) : (to + 8));
        ASSERT (Board[sm->capturedSquare] == enemyPawn);
        sm->capturedPiece = enemyPawn;
    }

    // handle captures:

    if (sm->capturedPiece != EMPTY) {
        ASSERT (piece_Type(sm->capturedPiece) != KING);
        sm->capturedNum = ListPos[sm->capturedSquare];
        // update opponents List of pieces
        Count[enemy]--;
        ListPos[List[enemy][Count[enemy]]] = sm->capturedNum;
        List[enemy][sm->capturedNum] = List[enemy][Count[enemy]];
        Material[sm->capturedPiece]--;
        HalfMoveClock = 0;
        RemoveFromBoard (sm->capturedPiece, sm->capturedSquare);
    }

    // handle promotion:

    if (sm->promote != EMPTY) {
        ASSERT (p == piece_Make(ToMove, PAWN));
        Material[p]--;
        RemoveFromBoard (p, from);
        p = piece_Make(ToMove, sm->promote);
        Material[p]++;
        AddToBoard (p, from);
    }

    // now make the move:
    List[ToMove][sm->pieceNum] = to;
    ListPos[to] = sm->pieceNum;
    RemoveFromBoard (p, from);
    AddToBoard (p, to);

    // handle Castling:

    if (ptype == KING  &&  square_Fyle(from) == E_FYLE  &&
            (square_Fyle(to) == C_FYLE  ||  square_Fyle(to) == G_FYLE)) {
        squareT rookfrom, rookto;
        pieceT rook = piece_Make (ToMove, ROOK);
        if (square_Fyle(to) == C_FYLE) {
            rookfrom = to - 2;
            rookto = to + 1;
        } else {
            rookfrom = to + 1;
            rookto = to - 1;
        }
        ListPos[rookto] = ListPos[rookfrom];
        List[ToMove][ListPos[rookto]] = rookto;
        RemoveFromBoard (rook, rookfrom);
        AddToBoard (rook, rookto);
    }

    // Handle clearing of castling flags:

    if (Castling) {
        if (ptype == KING) {   // The king moved.
            SetCastling (ToMove, QSIDE, false);
            SetCastling (ToMove, KSIDE, false);
        }
        // See if a rook moved or was captured:
        if (ToMove == WHITE) {
            if (from == A1)  { SetCastling (WHITE, QSIDE, false); }
            if (from == H1)  { SetCastling (WHITE, KSIDE, false); }
            if (to == A8)    { SetCastling (BLACK, QSIDE, false); }
            if (to == H8)    { SetCastling (BLACK, KSIDE, false); }
        } else {
            if (from == A8)  { SetCastling (BLACK, QSIDE, false); }
            if (from == H8)  { SetCastling (BLACK, KSIDE, false); }
            if (to == A1)    { SetCastling (WHITE, QSIDE, false); }
            if (to == H1)    { SetCastling (WHITE, KSIDE, false); }
        }
    }

    // Set the EPTarget square, if a pawn advanced two squares and an
    // enemy pawn is on a square where en passant may be possible.
    EPTarget = NULL_SQUARE;
    if (ptype == PAWN) {
        rankT fromRank = square_Rank(from);
        rankT toRank = square_Rank(to);
        if (fromRank == RANK_2  &&  toRank == RANK_4
              &&  (Board[square_Move(to,LEFT)] == BP
                     ||  Board[square_Move(to,RIGHT)] == BP)) {
            EPTarget = square_Move(from, UP);
        }
        if (fromRank == RANK_7  &&  toRank == RANK_5
              &&  (Board[square_Move(to,LEFT)] == WP
                     ||  Board[square_Move(to,RIGHT)] == WP)) {
            EPTarget = square_Move(from, DOWN);
        }
        HalfMoveClock = 0; // 50-move clock resets on pawn moves.
    }

    ToMove = enemy;

#ifdef ASSERTIONS
    // Do a SLOW, careful check for corruption:
    if (AssertPos() != OK) {
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
Position::UndoSimpleMove (simpleMoveT * m)
{
    ASSERT (m != NULL);
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
    if (isNullMove(m)) {
        return;
    }

    // Handle a capture: insert piece back into piecelist.
    // This works for EP captures too, since the square of the captured
    // piece is in the "capturedSquare" field rather than assuming the
    // value of the "to" field. The only time these two fields are
    // different is for an en passant move.

    if (m->capturedPiece != EMPTY) {
        colorT c = color_Flip(ToMove);
        ListPos[List[c][m->capturedNum]] = Count[c];
        ListPos[m->capturedSquare] = m->capturedNum;
        List[c][Count[c]] = List[c][m->capturedNum];
        List[c][m->capturedNum] = m->capturedSquare;
        Material[m->capturedPiece]++;
        Count[c]++;
    }

    // handle promotion:

    if (m->promote != EMPTY) {
        Material[p]--;
        RemoveFromBoard (p, to);
        p = piece_Make(ToMove, PAWN);
        Material[p]++;
        AddToBoard (p, to);
    }

    // now make the move:

    List[ToMove][m->pieceNum] = from;
    ListPos[from] = m->pieceNum;
    RemoveFromBoard (p, to);
    AddToBoard (p, from);
    if (m->capturedPiece != EMPTY) {
        AddToBoard (m->capturedPiece, m->capturedSquare);
    }

    // handle Castling:

    if ((piece_Type(p) == KING) && square_Fyle(from) == E_FYLE
            && (square_Fyle(to) == C_FYLE || square_Fyle(to) == G_FYLE)) {
        squareT rookfrom, rookto;
        pieceT rook = (ToMove == WHITE? WR : BR);
        if (square_Fyle(to) == C_FYLE) {
            rookfrom = to - 2;   rookto = to + 1;
        } else {
            rookfrom = to + 1;   rookto = to - 1;
        }
        ListPos[rookfrom] = ListPos[rookto];
        List[ToMove][ListPos[rookto]] = rookfrom;
        RemoveFromBoard (rook, rookto);
        AddToBoard (rook, rookfrom);
    }

#ifdef ASSERTIONS
    if (AssertPos() != OK) {
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
Position::RelocatePiece (squareT fromSq, squareT toSq)
{
    // Must have on-board squares:
    if (fromSq == NS ||  toSq == NS) { return ERROR; }

    // If squares are identical, just return success:
    if (fromSq == toSq) { return OK; }

    pieceT piece = Board[fromSq];
    pieceT ptype = piece_Type(piece);
    colorT pcolor = piece_Color(piece);

    // Must be relocating a nonempty piece to an empty square:
    if (piece == EMPTY  ||  Board[toSq] != EMPTY) { return ERROR; }

    // Pawns cannot relocate to the first or last rank:
    if (ptype == PAWN) {
        rankT toRank = square_Rank(toSq);
        if (toRank == RANK_1  ||  toRank == RANK_8) { return ERROR; }
    }

    // Locate the piece index in the appropriate list of pieces:
    uint index = ListPos[fromSq];
    ASSERT(List[pcolor][index] == fromSq);

    // Relocate the piece:
    List[pcolor][index] = toSq;
    ListPos[toSq] = index;
    RemoveFromBoard (piece, fromSq);
    AddToBoard (piece, toSq);

    // Check for adjacent kings or side to move giving check:
    if (! IsLegal()) {
        // Undo the relocation and return error:
        List[pcolor][index] = fromSq;
        RemoveFromBoard (piece, toSq);
        AddToBoard (piece, fromSq);
        return ERROR;
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
uint
Position::MaterialValue (colorT c)
{
    ASSERT (c == WHITE  ||  c == BLACK);
    uint value = 0;
    if (c == WHITE) {
        value += 9 * PieceCount(WQ);
        value += 5 * PieceCount(WR);
        value += 3 * PieceCount(WB);
        value += 3 * PieceCount(WN);
        value += 1 * PieceCount(WP);
    } else {
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
Position::MakeSANString (simpleMoveT * m, char * s, sanFlagT flag)
{
    ASSERT (m != NULL  &&  s != NULL);

    // Make sure m->pieceNum is updated:
    m->pieceNum = ListPos[m->from];
    pieceT  p    = piece_Type (Board[List[ToMove][m->pieceNum]]);
    squareT from = List[ToMove][m->pieceNum];
    squareT to   = m->to;
    char * c     = s;

    if (p == PAWN) {
        if (square_Fyle(from) != square_Fyle(to)) {  // pawn capture
            *c++ = square_FyleChar(from);
            *c++ = 'x';
        }
        *c++ = square_FyleChar(to);
        *c++ = square_RankChar(to);
        if ((square_Rank(to)==RANK_1) || (square_Rank(to)==RANK_8)) {
            *c++ = '=';
            *c++ = piece_Char(m->promote);
        }

    } else if (p == KING) {
        if (isNullMove(m)) {
            //*c++ = 'n'; *c++ = 'u'; *c++ = 'l'; *c++ = 'l';
            *c++ = '-'; *c++ = '-';
        } else
        if ((square_Fyle(from)==E_FYLE) && (square_Fyle(to)==G_FYLE)) {
            *c++ = 'O'; *c++ = '-'; *c++ = 'O';
        } else
        if ((square_Fyle(from)==E_FYLE) && (square_Fyle(to)==C_FYLE)) {
            *c++ = 'O'; *c++ = '-'; *c++ = 'O'; *c++ = '-'; *c++ = 'O';
        } else {  // regular King move
            *c++ = 'K';
            if (Board[to] != EMPTY)  *c++ = 'x';
            *c++ = square_FyleChar(to);
            *c++ = square_RankChar(to);
        }

    } else {    // Queen/Rook/Bishop/Knight
        *c++ = piece_Char(p);

        // We only need to calculate legal moves to disambiguate if there
        // are more than one of this type of piece.

        if (Material[Board[m->from]] < 2) {
            if (Board[to] != EMPTY)  { *c++ = 'x'; }
            *c++ = square_FyleChar(to);
            *c++ = square_RankChar(to);

        } else {
            // disambiguate moves here:
            // SHOULD handle 3-way ambiguity!  Looks like it does ok.
            bool ambiguous_fyle = false;
            bool ambiguous_rank = false;
            char r, f, r2, f2;
            f = square_FyleChar(from);
            r = square_RankChar(from);
            MoveList mlist;
            MatchLegalMove (&mlist, p, to);

            for (uint i=0; i < mlist.Size(); i++) {
                simpleMoveT * m2 = mlist.Get(i);
                squareT from2 = m2->from;
                pieceT p2 = piece_Type(Board[from2]);
                if ((to == m2->to) && (from != from2) && (p2 == p)) {
                    /* we have an ambiguity */
                    f2 = square_FyleChar (from2);
                    r2 = square_RankChar (from2);
                    if (f == f2) {  // ambiguous fyle, so print rank
                        ambiguous_fyle = true;
                    } else {        // ambiguous rank, so print fyle
                        ambiguous_rank = true;
                    }
                }
            }
            if (ambiguous_rank) { *c++ = f; }  // print from-fyle
            if (ambiguous_fyle) { *c++ = r; }  // print from-rank

            if (Board[to] != EMPTY) { *c++ = 'x'; }
            *c++ = square_FyleChar (to);
            *c++ = square_RankChar (to);
        }
    }

    // Now do the check or mate symbol:
    if (flag != SAN_NO_CHECKTEST) {
        // Now we make the move to test for check:
        DoSimpleMove (m);
        if (CalcNumChecks (GetKingSquare()) > 0) {
            char ch = '+';
            if (flag == SAN_MATETEST) {
                MoveList mlist;
                GenerateMoves (&mlist);
                if (mlist.Size() == 0) { ch = '#'; }
            }
            *c++ = ch;
        }
        UndoSimpleMove (m);
    }
    *c = 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadCoordMove():
//      Given a non-promotion move in coordinate notation,
//      e.g. "e2e4" or "g1f3", generates the legal move it represents.
//      Returns: OK or ERROR_InvalidMove.
//      If "reverse" is true, coordinates in reverse order are acceptable,
//      e.g. "f3g1" for 1.Nf3.
//
errorT
Position::ReadCoordMove (simpleMoveT * m, const char * str, bool reverse)
{
    ASSERT (m != NULL  &&  str != NULL);
    fyleT fromFyle, toFyle;
    rankT fromRank, toRank;
    squareT from, to;
    pieceT promo = EMPTY;

    uint slen = strLength(str);
    if (slen == 5) {
        promo = piece_FromChar(toupper(str[4]));
    } else if (slen != 4) { return ERROR_InvalidMove; }

    fromFyle = fyle_FromChar (str[0]);
    fromRank = rank_FromChar (str[1]);
    from = square_Make (fromFyle, fromRank);
    if (from == NS) { return ERROR_InvalidMove; }

    toFyle = fyle_FromChar (str[2]);
    toRank = rank_FromChar (str[3]);
    to = square_Make (toFyle, toRank);
    if (to == NS) { return ERROR_InvalidMove; }

    GenerateMoves();

    for (uint i=0; i < LegalMoves->Size(); i++) {
        simpleMoveT * sm = LegalMoves->Get(i);
        if (sm->promote == promo) {
            if (sm->from == from  &&  sm->to == to) {
                *m = *sm;
                return OK;
            }
            if (reverse  &&  sm->to == from  &&  sm->from == to) {
                *m = *sm;
                return OK;
            }
        }
    }
    return ERROR_InvalidMove;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadMove():
//      Given a move in (possibly sloppy) PGN notation,
//      generates the legal move it corresponds to.
//      Returns: OK or ERROR_InvalidMove.
//
errorT
Position::ReadMove (simpleMoveT * m, const char * str, tokenT token)
{
    ASSERT (m != NULL  &&  str != NULL);
    const char * s = str;
    char mStr [255];
    pieceT p;
    squareT from = NS;
    squareT to = NS;
    rankT frRank, toRank;
    fyleT frFyle, toFyle;

    MoveList mlist;
    mlist.Clear();

    // Check for a null move:
    if (token == TOKEN_Move_Null) {
        m->pieceNum = 0;
        m->from = GetKingSquare (ToMove);
        m->to = m->from;
        m->movingPiece = Board[m->from];
        m->promote = EMPTY;
        return OK;
    }

    // Strip out 'x', '-', etc leaving just pieces, files and ranks:
    char * s2 = mStr;
    uint slen = 0;
    while (!isspace(*s)  &&  *s != '\0') {
        if ((isalpha(*s)  && (*s != 'x'))  ||  isdigit(*s)  ||  *s == '=') {
            *s2 = *s;  s2++;  slen++;
        }
        s++;
    }
    *s2 = '\0';
    s = mStr;

    // Pawn moves:
    if (token == TOKEN_Move_Pawn  ||  token == TOKEN_Move_Promote) {
        pieceT promo = EMPTY;
        if (token == TOKEN_Move_Promote) {
            // Last char must be Q/R/B/N.
            // Accept the move even if it is of the form "a8Q" not "a8=Q":
            // if (s[slen-2] != '=') { return ERROR_InvalidMove; }
            promo = piece_FromChar(toupper(s[slen-1]));
            if (promo != QUEEN  &&  promo != ROOK  &&  promo != KNIGHT
                && promo != BISHOP) {
                return ERROR_InvalidMove;
            }
            slen--;
            if (s[slen-1] == '=') { slen--; }
        } else {
            // Check if it is a coordinates-style move, in which case it
            // could be any piece:
            if (slen >= 4  &&
                islower(s[0])  &&  isdigit(s[1])  &&
                islower(s[slen-2])  &&  isdigit(s[slen-1])) {
                return ReadCoordMove (m, str, false);
            }
        }
        // First char MUST be a fyle:
        if (*s < 'a'  ||  *s > 'h')  {  return ERROR_InvalidMove; }
        frFyle = fyle_FromChar (s[0]);

        // Check for the compact form of capture with no rank,
        // e.g. "ed" or "de=Q":
        if (slen == 2  &&  (s[1] >= 'a'  &&  s[1] <= 'h')) {
            toFyle = fyle_FromChar (s[1]);
            // Check each rank in turn, looking for the capture:
            for (rankT r = RANK_1; r <= RANK_8; r++) {
                to = square_Make (toFyle, r);
                if (MatchPawnMove (&mlist, frFyle, to, promo) == OK) {
                    *m = *(mlist.Get(0));
                    return OK;
                }
            }
            // It is NOT a valid capture with no rank:
            return ERROR_InvalidMove;
        }

        toFyle = fyle_FromChar (s[slen-2]);
        toRank = rank_FromChar (s[slen-1]);
        to = square_Make (toFyle, toRank);
        if (to == NS) { return ERROR_InvalidMove; }

        if (MatchPawnMove (&mlist, frFyle, to, promo) != OK) {
            return ERROR_InvalidMove;
        } else {
            *m = *(mlist.Get(0));
            return OK;
        }
    }

    // Here we handle piece moves, including castling
    if (token != TOKEN_Move_Piece) {  // Must be castling move
        ASSERT (token == TOKEN_Move_Castle_King  ||  token == TOKEN_Move_Castle_Queen);
        from = (ToMove == WHITE ? E1 : E8);
        if (GetKingSquare(ToMove) != from) { return ERROR_InvalidMove; }
        to = (token == TOKEN_Move_Castle_King ? (from + 2) : (from - 2));
        if (MatchKingMove (&mlist, to) != OK) {
            return ERROR_InvalidMove;
        } else {
            *m = *(mlist.Get(0));
            return OK;
        }
    }

    // If we reach here, it is a (non-castling, non-pawn) piece move.

    ASSERT (token == TOKEN_Move_Piece);
    p = piece_FromChar(*s);
    if (p == EMPTY) { return ERROR_InvalidMove; }
    if (slen < 3  ||  slen > 5) { return ERROR_InvalidMove; }
    toRank = rank_FromChar(s[slen-1]);
    toFyle = fyle_FromChar(s[slen-2]);
    to = square_Make(toFyle, toRank);
    if (to == NS) { return ERROR_InvalidMove; }
    frRank = NO_RANK;
    frFyle = NO_FYLE;
    if (slen > 3) {
        // There is some ambiguity information in the input string.

        for (uint i=1; i < slen-2; i++) {  // For each extra char:
            if (isdigit(s[i])) {
                frRank = rank_FromChar(s[i]);
            } else if (s[i] >= 'a'  &&  s[i] <= 'h') {
                frFyle = fyle_FromChar(s[i]);
            }
        }
    }

    // Calculate the matching legal move(s):
    if (p == KING) {
        if (MatchKingMove(&mlist, to) != OK) {
            return ERROR_InvalidMove;
        } else {
            *m = *(mlist.Get(0));
            return OK;
        }
    } else {  // A Queen/Rook/Bishop/Knight move
        MatchLegalMove (&mlist, p, to);
    }

    uint i;
    uint matchCount = 0;
    for (i=0; i < mlist.Size(); i++) {
        // We need to check: (a) that to-square matches, and
        //    (b), that from-square matches any ambiguity indicator.

        simpleMoveT * thisMove = mlist.Get(i);
        if (to == thisMove->to
              && (frFyle==NO_FYLE || frFyle == square_Fyle(thisMove->from))
              && (frRank==NO_RANK || frRank == square_Rank(thisMove->from))) {
            // We have a match!!
            *m = *thisMove;
            matchCount++;
        }
    }
    if (matchCount == 1) { return OK; }
    // No match, or too many (ambiguous) moves match:
    return ERROR_InvalidMove;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ParseMove():
//      Parse a single move from SAN-style notation.
//
errorT
Position::ParseMove (simpleMoveT * sm, const char * line)
{
    const char * s;
    char * s2;
    char mStr [255];
    uint length = 0;
    tokenT token = TOKEN_Invalid;
    errorT err = OK;

    s = line;
    ASSERT (line != NULL);

    // First, strip the move string down to its raw form with no
    // 'x' (capture symbols), etc:

    while (*s != 0  &&  !isalpha(*s)) { s++; }
    if (*s == '\0') { return ERROR_InvalidMove; }
    s2 = mStr; length = 0;
    while (!isspace(*s)  &&  *s != '\0') {
        if ((isalpha(*s)  && (*s != 'x'))  ||  isdigit(*s)  ||  *s == '=') {
            *s2 = *s;  s2++;  length++;
            if (length >= 10) { return ERROR_InvalidMove; }
        }
        s++;
    }
    if (length == 0 ||  length > 10) { return ERROR_InvalidMove; }
    *s2 = '\0';
    if (mStr[0] == 'O') {
        if (mStr[1] == 'O'  &&  mStr[2] == 'O' && mStr[3] == 0) {
            token = TOKEN_Move_Castle_Queen;
        } else if (mStr[1] == 'O'  &&  mStr[2] == 0) {
            token = TOKEN_Move_Castle_King;
        } else { return ERROR_InvalidMove; }
    } else if (mStr[0] == 'K'  ||  mStr[0] == 'Q'  ||  mStr[0] == 'R'  ||
               mStr[0] == 'B'  ||  mStr[0] == 'N'  ||  mStr[0] == 'r'  ||
               mStr[0] == 'k'  ||  mStr[0] == 'q'  ||  mStr[0] == 'n') {
        mStr[0] = toupper(mStr[0]);
        token = TOKEN_Move_Piece;
    } else if (mStr[0] >= 'a'  &&  mStr[0] <= 'h') {
        token = TOKEN_Move_Pawn;
        if (!isdigit (mStr[length - 1])) {
            token = TOKEN_Move_Promote;
        }
    } else { return ERROR_InvalidMove; }
    err = ReadMove (sm, mStr, token);
    // If not successful, and the move started with a lower case letter,
    // try treating it as a piece move instead. This only affects Bishop
    // moves where a lower-case 'b' is used instead of 'B'.
    if (err != OK  &&  token == TOKEN_Move_Pawn) {
        mStr[0] = toupper(mStr[0]);
        token = TOKEN_Move_Piece;
         err = ReadMove (sm, mStr, token);
    }
    if (err != OK) { return ERROR_InvalidMove; }
    return err;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadLine():
//      Parse a sequence of moves separated by whitespace and
//      move numbers, e.g. "1.e4 e5 2.Nf3" or "e4 e5 Nf3".
//
errorT
Position::ReadLine (const char * line)
{
    const char * s = line;
    char mStr[255];
    char * s2;
    uint length = 0;
    simpleMoveT sm;
    tokenT token = TOKEN_Invalid;
    errorT err;

    while (1) {
        while (*s != 0  &&  !isalpha(*s)) { s++; }
        if (*s == '\0') { return OK; }
        s2 = mStr; length = 0;
        while (!isspace(*s)  &&  *s != '\0') {
            if (isalpha(*s)  ||  isdigit(*s)  ||  *s == '=') {
                *s2 = *s;  s2++;  length++;
            }
            s++;
        }
        *s2 = '\0';
        if (*mStr == 'O') {
            if (mStr[1] == 'O'  &&  mStr[2] == 'O' && mStr[3] == 0) {
                token = TOKEN_Move_Castle_Queen;
            } else if (mStr[1] == 'O'  &&  mStr[2] == 0) {
                token = TOKEN_Move_Castle_King;
            }
        } else if (*mStr == 'K'  ||  *mStr == 'Q'  ||  *mStr == 'R'  ||
                   *mStr == 'B'  ||  *mStr == 'N'  ||  *mStr == 'r'  ||
                   *mStr == 'k'  ||  *mStr == 'q'  ||  *mStr == 'n') {
            *mStr = toupper(*mStr);
            token = TOKEN_Move_Piece;
        } else if (*mStr >= 'a'  &&  *mStr <= 'h') {
            token = TOKEN_Move_Pawn;
            if (!isdigit (mStr[length - 1])) {
                token = TOKEN_Move_Promote;
            }
        } else { return ERROR_InvalidMove; }
        err = ReadMove (&sm, mStr, token);
        if (err != OK) { return err; }
        DoSimpleMove (&sm);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CalcSANStrings():
//      Calculate the SAN string for each move in the legal moves list.
//
void
Position::CalcSANStrings (sanFlagT flag)
{
    if (SANStrings == NULL)  AllocSANStrings();
    //if (SANStrings->current) return;

    MoveList mlist;
    GenerateMoves (&mlist);
    for (ushort i=0; i < mlist.Size(); i++) {
        MakeSANString (mlist.Get(i), SANStrings->list[i], flag);
    }
    SANStrings->num = mlist.Size();
    SANStrings->current = true;
}

errorT
Position::ReadFromLongStr (const char * str)
{
    pieceT pieceFromByte [256] = {EMPTY};
    pieceFromByte [(int) 'K'] = WK;  pieceFromByte [(int) 'k'] = BK;
    pieceFromByte [(int) 'Q'] = WQ;  pieceFromByte [(int) 'q'] = BQ;
    pieceFromByte [(int) 'R'] = WR;  pieceFromByte [(int) 'r'] = BR;
    pieceFromByte [(int) 'B'] = WB;  pieceFromByte [(int) 'b'] = BB;
    pieceFromByte [(int) 'N'] = WN;  pieceFromByte [(int) 'n'] = BN;
    pieceFromByte [(int) 'P'] = WP;  pieceFromByte [(int) 'p'] = BP;

    Clear();
    for (squareT sq=A1; sq <= H8; sq++) {
        if (str[sq] == '.') { continue; }
        pieceT p = pieceFromByte [(byte) str[sq]];
        if (p == EMPTY) { return ERROR_Corrupt; }
        if (AddPiece (p,sq) != OK) { return ERROR_Corrupt; }
    }
    switch (str[65]) {
    case 'w':
        SetToMove (WHITE);
        break;
    case 'b':
        SetToMove (BLACK);
        break;
    default:
        return ERROR_Corrupt;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::MakeLongStr():
//      Make a string representing the board. It will be 66 characters
//      long, encoding the 64 squares (in the order a1,b1,...,g8,h8
//      with white pieces in upper case, black pieces in lower case,
//      and empty squares as dots) then a space, and finally "w" or "b"
//      indicating the side to move. Example for the starting position:
//      "RNBQKBNRPPPPPPPP................................pppppppprbnqkbnr w"
//
void
Position::MakeLongStr (char * str)
{
    ASSERT (str != NULL);
    char * s = str;
    for (squareT sq = A1; sq <= H8; sq++) {
        *s++ = PIECE_CHAR[Board[sq]];
    }
    *s++ = ' ';
    *s++ = (ToMove == WHITE ? 'w' : 'b');
    *s = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpBoard():
//      Dump the board to an open file.
//
void
Position::DumpBoard (FILE * fp)
{
    ASSERT (fp != NULL);
    squareT s;
    for (int i=7; i>=0; i--) {
        fputs ("   ", fp);
        for (int j=0; j<8; j++) {
            s = (i*8) + j;
            putc (PIECE_CHAR[Board[s]], fp);
            putc (' ', fp);
        }
        putc ('\n', fp);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpLists():
//      Dump the piece lists to an open file.
//
void
Position::DumpLists (FILE * fp)
{
    ASSERT (fp != NULL);
    uint i;
    for (colorT c = WHITE; c <= BLACK; c++) {
        for (i=0; i < Count[c]; i++) {
            pieceT p = Board[List[c][i]];
            fprintf (fp, "%2d:", ListPos[List[c][i]]);
            putc (PIECE_CHAR[p], fp);
            putc (square_FyleChar (List[c][i]), fp);
            putc (square_RankChar (List[c][i]), fp);
            putc (' ', fp);
        }
        putc ('\n', fp);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadFromCompactStr():
//    Sets the position from the provided Null-terminated 33-byte
//    compact string.
//    The first 32 bytes contain the square valued, 4 bits per value,
//    for the square order A1, B1, ...., G8, H8.
//    The next byte contains the side to move, 1 for White or 2 for Black.
//    The final two bytes contain castling and en passant rights.
//    To ensure no bytes within the staring are zero-valued (so it
//    can be used as a regular null-terminated string), the value 1
//    is added to the color, castling and en passant fields.
errorT
Position::ReadFromCompactStr (const byte * str)
{
    Clear();
    for (uint i=0; i < 32; i++) {
        pieceT p = str[i] >> 4;
        if (p != EMPTY) {
            if (AddPiece (p, i * 2) != OK) {
                return ERROR_Corrupt;
            }
        }
        p = str[i] & 15;
        if (p != EMPTY) {
            if (AddPiece (p, i * 2 + 1) != OK) {
                return ERROR_Corrupt;
            }
        }
    }
    colorT toMove = str[32] - 1;
    if (toMove != WHITE  &&  toMove != BLACK) {
        return ERROR_Corrupt;
    }
    ToMove = toMove;
    Castling = str[33] - 1;
    EPTarget = str[34] - 1;
    return OK;
}

void
Position::PrintCompactStr (char * cboard)
{
    for (uint i=0; i < 32; i++) {
        uint i2 = i << 1;
        cboard[i] = (byte)(Board[i2] << 4) | Board[i2+1];
    }
    cboard[32] = 1 + ToMove;
    cboard[33] = 1 + Castling;

    // Check that there really is an enemy pawn that might
    // be able to capture to the en passant square. For example,
    // if the EP square is c6 but there is no white pawn on
    // b5 or d5, then en passant should be ignored.

    squareT ep = EPTarget;
    if (ToMove == WHITE) {
        if (Board[square_Move (ep, DOWN_LEFT)] != WP  &&
            Board[square_Move (ep, DOWN_RIGHT)] != WP) { ep = NULL_SQUARE; }

    } else {
        if (Board[square_Move (ep, UP_LEFT)] != BP  &&
            Board[square_Move (ep, UP_RIGHT)] != BP) { ep = NULL_SQUARE; }

    }
    cboard[34] = 1 + ep;
    cboard[35] = 0;
}

void
Position::PrintCompactStrFlipped (char * cboard)
{
    for (uint i=0; i < 32; i++) {
        uint i2 = i << 1;
        // Flip 1st rank to 8th, etc:
        i2 = ((7 - (i2)/8) * 8 + ((i2) % 8));
        cboard[i] = (byte)(PIECE_FLIP[Board[i2]] << 4) |
            (byte)(PIECE_FLIP[Board[i2+1]]);
    }
    cboard[32] = 1 + color_Flip(ToMove);
    cboard[33] = 1 + Castling;
    cboard[34] = 1 + EPTarget;
    cboard[35] = 0;
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
Position::ReadFromFEN (const char * str)
{
    // pieceFromByte[] converts a character to its piece, e.g. 'k' -> BK.
    static pieceT pieceFromByte [256];

    // fenSqToRealSquare[] converts a fen square (0 to 63) to its real
    // square. E.g: [0] -> A8, [1] -> B8, .... [63] -> H1.
    static squareT fenSqToRealSquare [64];

    // Note the first Call to set up the static arrays only once:
    static int firstCall = 1;

    ASSERT (str != NULL);
    const char * s = str;
    int count = 0;

    if (firstCall) {
        firstCall = 0;

        // Set up pieceFromByte[]:
        for (int i=0; i < 256; i++) { pieceFromByte[i] = EMPTY; }
        pieceFromByte [(int) 'K'] = WK;  pieceFromByte [(int) 'k'] = BK;
        pieceFromByte [(int) 'Q'] = WQ;  pieceFromByte [(int) 'q'] = BQ;
        pieceFromByte [(int) 'R'] = WR;  pieceFromByte [(int) 'r'] = BR;
        pieceFromByte [(int) 'B'] = WB;  pieceFromByte [(int) 'b'] = BB;
        pieceFromByte [(int) 'N'] = WN;  pieceFromByte [(int) 'n'] = BN;
        pieceFromByte [(int) 'P'] = WP;  pieceFromByte [(int) 'p'] = BP;

        // Set up fenSqToRealSq[]:
        for (int sq=0; sq < 64; sq++) {
            fenSqToRealSquare [sq] = (squareT)((7 - (sq)/8) * 8 + ((sq) % 8));
        }
    }

    Clear ();
    while (count < 64) {
        if (*s == '/') {
            // A FEN string does not have to contain '/'s but if one
            // appears anywhere except the start of a row, it is an error:

            if (count % 8) { return ERROR_InvalidFEN; }

        } else if (*s > '0'  &&  *s < '9') {
            count += (*s - '0');

        } else {
            pieceT p = pieceFromByte [(byte) *s];
            if (p == EMPTY) { return ERROR_InvalidFEN; }
            if (AddPiece (p, fenSqToRealSquare[count]) != OK) {
                return ERROR_InvalidFEN;
            }
            count++;
        }
        s++;
    }
    if (Material[WK] != 1  ||  Material[BK] != 1) { return ERROR_InvalidFEN; }

    // Now the side to move:
    while (isspace(*s)) { s++; }
    switch (*s) {
    case 'w':
        SetToMove (WHITE);
        break;
    case 'b':
        SetToMove (BLACK);
        break;
    default:
        return ERROR_InvalidFEN;
    }
    s++;

    if (! IsLegal()) { return ERROR_InvalidFEN; }

    // Now the castling flags:
    while (isspace(*s)) { s++; }
    if (*s == '-') {
        s++;  // do nothing
    } else if (*s == 0) {
        // The FEN has no castling field, so just guess that
        // castling is possible whenever a king and rook are
        // still on their starting squares:
        if (Board[E1] == WK) {
            if (Board[A1] == WR) { SetCastling (WHITE, QSIDE, true); }
            if (Board[H1] == WR) { SetCastling (WHITE, KSIDE, true); }
        }
        if (Board[E8] == BK) {
            if (Board[A8] == BR) { SetCastling (BLACK, QSIDE, true); }
            if (Board[H8] == BR) { SetCastling (BLACK, KSIDE, true); }
        }
    } else {
        while (!isspace(*s)  &&  *s != 0) {
            switch (*s) {
            case 'Q':
                SetCastling (WHITE, QSIDE, true);
                break;
            case 'q':
                SetCastling (BLACK, QSIDE, true);
                break;
            case 'K':
                SetCastling (WHITE, KSIDE, true);
                break;
            case 'k':
                SetCastling (BLACK, KSIDE, true);
                break;
            default:
                return ERROR_InvalidFEN;
            }
            s++;
        }
    }

    // Now the EP target:
    while (isspace(*s)) { s++; }
    if (*s == 0) {
        // do nothing
    } else if (*s == '-') {
        EPTarget = NULL_SQUARE;
        s++;  // No EP target
    } else {
        char fylec = *s; s++;
        if (fylec < 'a'  ||  fylec > 'h') {
            return ERROR_InvalidFEN;
        }
        char rankc = *s; s++;
        if (rankc != '3'  &&  rankc != '6') {
            return ERROR_InvalidFEN;
        }
        EPTarget = square_Make(fyle_FromChar(fylec), rank_FromChar(rankc));
    }

    // Now the capture/pawn halfmove clock:
    while (isspace(*s)) { s++; }
    if (*s) {
        HalfMoveClock = (ushort) atoi(s);
    }
    while (!isspace(*s)  && *s != 0) { s++; }

    // Finally, the fullmove counter:
    while (isspace(*s)) { s++; }
    if (*s) {
        int i = atoi(s);
        if (i >= 1) {
            PlyCounter = (i - 1) * 2;
        }
    }
    if (ToMove == BLACK) { PlyCounter++; }
    return OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::PrintFEN():
//      Print the FEN representation of the position.
//      If flags == FEN_COMPACT, only the board and side-to-move fields
//              are printed, in compact form (no slashes between rows).
//      If flags == FEN_BOARD, only the board and side-to-move fields
//              are printed.
//      If flags == FEN_CASTLING_EP, the castling and en passant fields
//              are also printed.
//      If flags == FEN_ALL_FIELDS, all fields are printed including
//              the halfmove clock and ply counter.
//
void
Position::PrintFEN (char * str, uint flags)
{
    ASSERT (str != NULL);
    uint emptyRun, iRank, iFyle;
    pieceT * pBoard = Board;
    for (iRank = 0; iRank < 8; iRank++) {
        pBoard = &(Board[(7 - iRank) * 8]);
        emptyRun = 0;
        if (iRank > 0  &&  flags > FEN_COMPACT) { *str++ = '/'; }
        for (iFyle = 0; iFyle < 8; iFyle++, pBoard++) {
            if (*pBoard != EMPTY) {
                if (emptyRun) { *str++ = (byte) emptyRun + '0'; }
                emptyRun = 0;
                *str++ = PIECE_CHAR[*pBoard];
            } else {
                emptyRun++;
            }
        }
        if (emptyRun) { *str++ = (byte) emptyRun + '0'; }
    }

    if (flags > FEN_COMPACT) { *str++ = ' '; }
    *str++ = (ToMove == WHITE ? 'w' : 'b');
    *str = 0;

    if (flags >= FEN_CASTLING_EP) {
        // Add the castling flags and EP flag as well:
        *str++ = ' ';
        if (Castling == 0)  {
            *str++ = '-';
        } else {
            if (GetCastling (WHITE, KSIDE))  { *str++ = 'K'; }
            if (GetCastling (WHITE, QSIDE))  { *str++ = 'Q'; }
            if (GetCastling (BLACK, KSIDE))  { *str++ = 'k'; }
            if (GetCastling (BLACK, QSIDE))  { *str++ = 'q'; }
        }
        *str++ = ' ';

        // Now the EP target square:
        if (EPTarget == NULL_SQUARE) {
            *str++ = '-';
        } else {
            *str++ = square_FyleChar (EPTarget);
            *str++ = square_RankChar (EPTarget);
        }
        *str = 0;

        if (flags >= FEN_ALL_FIELDS) {
            // Also print the Halfmove and ply counters:
            *str++ = ' ';
            sprintf (str, "%d %d", HalfMoveClock, (PlyCounter / 2) + 1);
        }
    }
    return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpHtmlBoard():
//      Prints the board in a format for use in HTML documents.
//      Assumes that the HTML document will be in a directory that
//      has a subdirectory bitmapsDir with files "bwr.gif", etc.
//      The numeric arguments are the pixel width and height for each
//      square -- if zero, then the bitmaps are not scaled.

// The following values define the available HTML styles.
// Style 0 has 40x40 non-transparent images in the "bitmaps" directory.
// Style 1 has 36x35 non-transparent images in the "bitmaps2" directory.

struct htmlStyleT {
    const char * dir;  // directory containing images.
    uint width;        // width value specified in <img> tag.
    uint height;       // height value specified in <img> tag.
    bool transparent;  // True if the style uses transparent images,
                       // with square colors set by "bgcolor".
};

void
Position::DumpHtmlBoard (DString * dstr, uint style, const char * dir, bool flip)
{
    const uint HTML_DIAG_STYLES = 2;
    htmlStyleT hs [HTML_DIAG_STYLES];
    hs[0].dir = "bitmaps"; hs[0].width = 40; hs[0].height = 40;
    hs[1].dir = "bitmaps2"; hs[1].width = 36; hs[1].height = 35;
    if (style >= HTML_DIAG_STYLES) { style = 0; }

    uint width = hs[style].width;
    uint height = hs[style].height;
    uint iRank, iFyle;
    pieceT * pBoard;
    if (dir == NULL) { dir = hs[style].dir; }

    dstr->Append ("<br><br><center>\n");
    dstr->Append ("<table Border=1 CellSpacing=0 CellPadding=0>\n");
    for (iRank = 0; iRank < 8; iRank++) {
        dstr->Append ("<tr>\n");
        pBoard = &(Board[(7 - iRank) * 8]);
        for (iFyle = 0; iFyle < 8; iFyle++, pBoard++) {
            pieceT piece = *pBoard;
            if (flip) { piece = Board[iRank * 8 + (7 - iFyle)]; }
            dstr->Append ("  <td><img border=0 ");
            if (width > 0) {
                char temp[40];
                sprintf (temp, "width=%u ", width);
                dstr->Append (temp);
            }
            if (height > 0) {
                char temp[40];
                sprintf (temp, "height=%u ", height);
                dstr->Append (temp);
            }
            dstr->Append ("src=\"");
            dstr->Append (dir);
            dstr->AddChar ('/');
            bool lightSq = ((iRank % 2) == (iFyle % 2));
            if (lightSq) {
                dstr->AddChar ('w');
            } else {
                dstr->AddChar ('b');
            }
            if (piece == EMPTY) {
                dstr->Append ("sq.gif");
            } else {
                colorT c = piece_Color(piece);
                dstr->AddChar (c == WHITE ? 'w' : 'b');
                dstr->AddChar (tolower (PIECE_CHAR[piece]));
                dstr->Append (".gif");
            }
            dstr->Append ("\" alt=\"");
            if (piece == EMPTY) {
                if (! lightSq) { dstr->Append ("::"); }
            } else {
                colorT c = piece_Color(piece);
                dstr->AddChar (c == WHITE ? 'W' : 'B');
                dstr->AddChar (toupper (PIECE_CHAR[piece]));
            }
            dstr->Append ("\"></td>\n");
        }
        dstr->Append ("</tr>\n");
    }
    dstr->Append ("</table>\n");
    //if (ToMove == WHITE) {
    //    dstr->Append ("<br><b>White to move.</b>\n");
    //} else {
    //    dstr->Append ("<br><b>Black to move.</b>\n");
    //}
    dstr->Append("</center><br>");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::DumpLatexBoard():
//      Prints the board in a format used by a chess package that is
//      available for the LaTeX  or TeX typesetting language.
void
Position::DumpLatexBoard (DString * dstr, bool flip)
{
    uint iRank, iFyle;
    pieceT * pBoard;
    dstr->Append ("\\board{");
    for (iRank = 0; iRank < 8; iRank++) {
        pBoard = &(Board[(7 - iRank) * 8]);
        for (iFyle = 0; iFyle < 8; iFyle++, pBoard++) {
            pieceT piece = *pBoard;
            if (flip) { piece = Board[iRank * 8 + (7 - iFyle)]; }
            if (piece != EMPTY) {
                dstr->AddChar (PIECE_CHAR[piece]);
            } else { // put a space or a '*':
                dstr->AddChar (((iRank % 2) == (iFyle % 2)) ? ' ' : '*');
            }
        }
        if (iRank < 7) {
            dstr->Append ("}\n {");
        } else { dstr->AddChar ('}'); }
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Compare():
//      Compare another position with this one.
//
sint
Position::Compare (Position * p)
{
    int i = 32;
    byte *p1, *p2;
    p1 = Board;
    p2 = p->Board;
    while (i   &&  *p1 == *p2) {
        i--;  p1++;  p2++;
    }
    if (p1 < p2) { return -1; }
    if (p1 > p2) { return 1; }
    return (ToMove - p->GetToMove());
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::CopyFrom():
//      Copy another position to this one.
//
void
Position::CopyFrom (Position * src)
{
    for (pieceT p=A1; p <= NS; p++) {
        Board[p] = src->Board[p];
    };
    Count[WHITE] = src->Count[WHITE];
    Count[BLACK] = src->Count[BLACK];
    uint i;
    for (i=0; i < 64; i++) { ListPos[i] = src->ListPos[i]; }
    for (i=0; i < 16; i++) {
        Material[i] = src->Material[i];
        List[WHITE][i] = src->List[WHITE][i];
        List[BLACK][i] = src->List[BLACK][i];
        for (uint j=0; j < 8; j++) {
            NumOnFyle[i][j] = src->NumOnFyle[i][j];
            NumOnRank[i][j] = src->NumOnRank[i][j];
        }
        for (uint d=0; d < 15; d++) {
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
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::GetSquares
//    Adds to the provided square list all squares containing the specified
//    piece, and return the number of pieces of that type on the board.
uint
Position::GetSquares (pieceT piece, SquareList * sqlist)
{
    colorT color = piece_Color(piece);
    squareT * squares = GetList(color);
    uint npieces = GetCount(color);
    for (uint i=0; i < npieces; i++) {
        squareT sq = squares[i];
        pieceT p = Board[sq];
        if (p == piece) { sqlist->Add (sq); }
    }
    return Material[piece];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::Random
//    Given a string such as "KRPKR" or "KRP-kr", sets up a
//    random position with that material configuration.
inline squareT
randomSquare (void) { return random32() % 64; }

inline squareT
randomPawnSquare (void) { return (random32() % 48) + A2; }

errorT
Position::Random (const char * material)
{
    pieceT pieces [32];         // List of pieces excluding kings
    uint nPieces[2] = {0, 0};   // Number of pieces per side excluding kings.
    uint total = 0;             // Total number of pieces excluding kings.

    colorT side = WHITE;

    // The material string must start with a king:
    if (toupper(*material) != 'K') { return ERROR_Corrupt; }
    material++;

    // Read the material string:
    while (1) {
        char ch = toupper(*material);
        if (ch == 0) { break; }
        switch (ch) {
        case 'K':
            if (side == BLACK) { return ERROR_Corrupt; } // Seen third king!
            side = BLACK;
            break;
        case 'Q':  case 'R':  case 'B':  case 'N':  case 'P':
            if (nPieces[side] >= 15) { return ERROR_Corrupt; }
            nPieces[side]++;
            if (ch == 'P') {
                pieces[total] = piece_Make (side, PAWN);
            } else {
                pieces[total] = piece_Make (side, piece_FromChar(ch));
            }
            total++;
            break;
        case ' ':  case '-':  case '.':  case ',':  case ':':
            // Ignore spaces, commas, etc:
            break;
        default:
            return ERROR_Corrupt;
        }
        material++;
    }
    if (side != BLACK) { return ERROR_Corrupt; }  // Never saw Black king!

    // Generate two non-adjacent king squares:
    squareT wk = randomSquare();
    squareT bk = randomSquare();
    while (wk == bk  ||  square_Adjacent (wk, bk)) { bk = randomSquare(); }

    // Now add all other pieces to empty squares, looping until a legal
    // position is found:
    while (1) {
        Clear();
        ToMove = (random32() % 2) ? WHITE : BLACK;
        AddPiece (WK, wk);
        AddPiece (BK, bk);

        for (uint i=0; i < total; i++) {
            squareT sq;
            pieceT p = pieces[i];
            bool isPawn = (piece_Type(p) == PAWN);
            while (1) {
                sq = isPawn ? randomPawnSquare() : randomSquare();
                if (Board[sq] == EMPTY) { break; }
            }
            // Add this piece on the random empty square:
            AddPiece (p, sq);
        }
        // Generated a random position with kings not adjacent and
        // every piece on its own square. We can stop at this
        // attempt if the enemy king is not in check:
        squareT enemyKing = (ToMove == WHITE) ? bk : wk;
        if (CalcAttacks (ToMove, enemyKing, NULL) == 0) { break; }
    }
    return OK;
}

} // End namespace Guess

//////////////////////////////////////////////////////////////////////
//  EOF: position.cpp
//////////////////////////////////////////////////////////////////////
