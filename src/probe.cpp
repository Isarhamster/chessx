//////////////////////////////////////////////////////////////////////
//
//  FILE:       probe.cpp
//              Scid interface to Nalimov Tablebase decoder
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2000-2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "probe.h"
#include "sqmove.h"

#ifdef SCID_USE_TB

#define NEW
#define XX 127       // Invalid EP square. Can be any value greater than 63,
                     // which represents H8.
#define C_PIECES  3  // Maximum number of pieces of one color and type.

// PROBE_MAX_PER_SIDE = Maximum number of pieces per side, including Kings.
// It will be 3, unless T41_INCLUDE is defines which allows use of the
// 3-plus-king vs lone king bases.
#ifdef T41_INCLUDE
#define PROBE_MAX_PER_SIDE 4
#else
#define PROBE_MAX_PER_SIDE 3
#endif


typedef unsigned int INDEX;
typedef unsigned int square;

#define SqFindKing(psq) (psq[C_PIECES * (x_pieceKing - 1)])
#define SqFindOne(psq,pce) (psq[C_PIECES * (pce - 1)])
#define SqFindFirst(psq,pce) (psq[C_PIECES * (pce - 1)])
#define SqFindSecond(psq,pce) (psq[C_PIECES * (pce - 1) + 1])
#define SqFindThird(psq,pce) (psq[C_PIECES * (pce - 1) + 2])


#include "egtb/tbindex.cpp"


// Default, minimum and maximum Tablebase cache size:
static const uint EGTB_CACHE_SIZE_MIN     =        512 * 1024;    // 0.5 MB
static const uint EGTB_CACHE_SIZE_DEFAULT =        512 * 1024;    // 0.5 MB
static const uint EGTB_CACHE_SIZE_MAX     = 128 * 1024 * 1024;    // 128 MB

static void * EGTB_cache = NULL;
static uint EGTB_maxpieces = 0;
static uint EGTB_cachesize = EGTB_CACHE_SIZE_DEFAULT;

// scid_TB_compiled:
//    Returns true if Tablebase support has been compiled, false otherwise.
bool
scid_TB_compiled (void) {
    return true;
}

// scid_TB_MaxPieces:
//    Returns the largest number of pieces in any registered tablebase,
//    including kings and pawns (e.g. kpkp tablebase has 4 pieces).
uint
scid_TB_MaxPieces (void) {
    return EGTB_maxpieces;
}

uint
scid_TB_CacheSize (void)
{
    return EGTB_cachesize;
}

void
scid_TB_SetCacheSize (uint cachesize)
{
    EGTB_cachesize = cachesize;
    if (cachesize < EGTB_CACHE_SIZE_MIN) {
        EGTB_cachesize = EGTB_CACHE_SIZE_MIN;
    }
    if (cachesize > EGTB_CACHE_SIZE_MAX) {
        EGTB_cachesize = EGTB_CACHE_SIZE_MAX;
    }
}

// scid_TB_init:
//    Initialises the tablebases given a directory string. All the tables
//    to be used must be in the directory; subdirectories are not
//    scanned. However, the directory string may have more than one
//    dircetory in it, separated by commas (,) or semicolons (;).
//    Returns the same value as scid_TB_MaxPieces().
uint
scid_TB_Init (const char * egtb_path)
{
    EGTB_maxpieces = (uint) IInitializeTb ((char *) egtb_path);
#ifdef WINCE
    if (EGTB_cache != NULL) { my_Tcl_Free( (char *) EGTB_cache); }
    EGTB_cache = (byte *)my_Tcl_Alloc(sizeof(byte [EGTB_cachesize]));
#else
    if (EGTB_cache != NULL) { delete[] (byte *) EGTB_cache; }
    EGTB_cache = new byte [EGTB_cachesize];
#endif
    FTbSetCacheSize (EGTB_cache, EGTB_cachesize);
    return EGTB_maxpieces;
}

// scid_TB_Available:
//    Given a material configuration, returns a boolean indicating
//    if the tablebase for that material is registered for use.
//    Note: there are actually TWO tablebases for any material
//    combination, one for each side to move (file suffixes .nbw.emd
//    and .nbb.emd); this function returns true if EITHER one is 
//    registered (since having only one of the two is usually good 
//    enough to solve the endgame).
bool
scid_TB_Available (matSigT matsig)
{
    if (EGTB_maxpieces == 0) { return 0; }

    int counts [10];
    counts [0] = matsig_getCount (matsig, WP);
    counts [1] = matsig_getCount (matsig, WN);
    counts [2] = matsig_getCount (matsig, WB);
    counts [3] = matsig_getCount (matsig, WR);
    counts [4] = matsig_getCount (matsig, WQ);
    counts [5] = matsig_getCount (matsig, BP);
    counts [6] = matsig_getCount (matsig, BN);
    counts [7] = matsig_getCount (matsig, BB);
    counts [8] = matsig_getCount (matsig, BR);
    counts [9] = matsig_getCount (matsig, BQ);

    // Quickly check that there is not too much material:

    uint wc = 1 + counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
    uint bc = 1 + counts[5] + counts[6] + counts[7] + counts[8] + counts[9];
    uint bothc = wc + bc;
    if (bothc > EGTB_maxpieces  ||  wc > PROBE_MAX_PER_SIDE  ||
        bc > PROBE_MAX_PER_SIDE) { return false; }

    // If two lone Kings, just return true:
    if (bothc == 2) { return true; }

    // If KB-K or KN-K, return true because they are all-drawn tablebases:
    if (bothc == 3) {
        if (counts[1] == 1  ||  counts[2] == 1  ||
            counts[6] == 1  ||  counts[7] == 1) {
            return true;
        }
    }

    int iTb = IDescFindFromCounters (counts);
    if (iTb == 0) { return false; }
    if (iTb < 0) { iTb = -iTb; }

    // Return true if either of the two TBs for this material is available:

    if (FRegistered (iTb, 0)) { return true; }
    if (FRegistered (iTb, 1)) { return true; }
    return false;
}


// scid_TB_Probe:
//    Given a position, probes the appropriate tablebase and puts the
//    result in the integer pointed to by <score>.
//    Returns OK if the probe was successful, or ERROR_NotFound otherwise.
//
//    The value placed in score is as follows, where STM is the side to move:
//        3   STM mates in 3, etc.
//        2   STM mates in 2.
//        1   STM mates in 1.
//        0   Draw.
//       -1   STM is checkmated.
//       -2   STM mated in 1.
//       -3   STM mated in 2, etc.
//
errorT
scid_TB_Probe (Position * pos, int * score)
{
    int pieceCounts [10];
    uint wSquares [C_PIECES * 6], bSquares [C_PIECES * 6];
    uint * wSqs, * bSqs;
    int iTb, color, flip;
    uint npieces = pos->GetCount(WHITE) + pos->GetCount(BLACK);

    // Check that position has few enough pieces on each side:

    if (npieces > EGTB_maxpieces) { return ERROR_NotFound; }
    if (pos->GetCount(WHITE) > PROBE_MAX_PER_SIDE) { return ERROR_NotFound; }
    if (pos->GetCount(BLACK) > PROBE_MAX_PER_SIDE) { return ERROR_NotFound; }

    // If just two Kings, return "draw" now:
    if (npieces <= 2) { *score = 0; return OK; }

    // If just a lone bishop or knight and kings, return draw now:
    if (npieces == 3) {
        if (pos->PieceCount(WB) == 1  ||  pos->PieceCount(BB) == 1  ||
            pos->PieceCount(WN) == 1  ||  pos->PieceCount(WN) == 1) {
            *score = 0;
            return OK;
        }
    }

    // Fill in array of piece counts and find if the tablebase for this
    // material configuration and side to move is registered:

    pieceCounts [0] = pos->PieceCount(WP);
    pieceCounts [1] = pos->PieceCount(WN);
    pieceCounts [2] = pos->PieceCount(WB);
    pieceCounts [3] = pos->PieceCount(WR);
    pieceCounts [4] = pos->PieceCount(WQ);
    pieceCounts [5] = pos->PieceCount(BP);
    pieceCounts [6] = pos->PieceCount(BN);
    pieceCounts [7] = pos->PieceCount(BB);
    pieceCounts [8] = pos->PieceCount(BR);
    pieceCounts [9] = pos->PieceCount(BQ);

    iTb = IDescFindFromCounters (pieceCounts);
    if (iTb == 0) { return ERROR_NotFound; }

    if (iTb > 0) {
        color = (pos->GetToMove() == WHITE) ? 0 : 1;
        flip = 0;
        wSqs = wSquares;
        bSqs = bSquares;
    } else {
        color = (pos->GetToMove() == WHITE) ? 1 : 0;
        flip = 1;
        wSqs = bSquares;
        bSqs = wSquares;
        iTb = - iTb;
    }

    // macro that returns true if corresponding TB was found during initializing
    if (! FRegistered (iTb, color)) { return ERROR_NotFound; }

    // Now we know the tablebase is registered. Fill in the array of
    // square values for each piece:

    uint * firstSq[16];
    firstSq[EMPTY] = NULL;
    firstSq[WK] = &(wSquares [C_PIECES * (x_pieceKing   - 1) ]);
    firstSq[BK] = &(bSquares [C_PIECES * (x_pieceKing   - 1) ]);
    firstSq[WQ] = &(wSquares [C_PIECES * (x_pieceQueen  - 1) ]);
    firstSq[BQ] = &(bSquares [C_PIECES * (x_pieceQueen  - 1) ]);
    firstSq[WR] = &(wSquares [C_PIECES * (x_pieceRook   - 1) ]);
    firstSq[BR] = &(bSquares [C_PIECES * (x_pieceRook   - 1) ]);
    firstSq[WB] = &(wSquares [C_PIECES * (x_pieceBishop - 1) ]);
    firstSq[BB] = &(bSquares [C_PIECES * (x_pieceBishop - 1) ]);
    firstSq[WN] = &(wSquares [C_PIECES * (x_pieceKnight - 1) ]);
    firstSq[BN] = &(bSquares [C_PIECES * (x_pieceKnight - 1) ]);
    firstSq[WP] = &(wSquares [C_PIECES * (x_piecePawn   - 1) ]);
    firstSq[BP] = &(bSquares [C_PIECES * (x_piecePawn   - 1) ]);

    const pieceT* board = pos->GetBoard();

    for (squareT sq = A1; sq <= H8; sq++) {
        pieceT pce = board[sq];
        if (pce != EMPTY) {
            *(firstSq[pce]) = (int) sq;
            firstSq[pce]++;
        }
    }

    // Set En Passant square it should only be a value other than XX if
    // there is an EP target square, AND there is a possible EP capture.
    // Specifying a target EP square (since a pawn has just moved two
    // squares) when there is no enemy pawn actually able to capture
    // en passant was able to cause the tablebase to give incorrect
    // results in testing, so that is why we must check here whether an
    // EP capture is possible.

    squareT enPassant = pos->GetEPTarget();
    if (enPassant != NULL_SQUARE) {
        bool possibleEP = false;
        if (pos->GetToMove() == BLACK) {
            // White just made a 2-square pawn move:
            squareT left = square_Move (enPassant, UP_LEFT);
            if (left != NULL_SQUARE  &&  board[left] == BP) {
                possibleEP = true;
            }
            squareT right = square_Move (enPassant, UP_RIGHT);
            if (right != NULL_SQUARE  &&  board[right] == BP) {
                possibleEP = true;
            }
        } else {
            // BLACK just made a 2-square pawn move:
            squareT left = square_Move (enPassant, DOWN_LEFT);
            if (left != NULL_SQUARE  &&  board[left] == WP) {
                possibleEP = true;
            }
            squareT right = square_Move (enPassant, DOWN_RIGHT);
            if (right != NULL_SQUARE  &&  board[right] == WP) {
                possibleEP = true;
            }
        }
        if (! possibleEP) { enPassant = NULL_SQUARE; }
    }
    int epTarget = (int) enPassant;
    if (enPassant == NULL_SQUARE) { epTarget = XX; }

    // Now probe the tablebase:

    INDEX index = PfnIndCalc(iTb,color) (wSqs, bSqs, epTarget, flip);
    int tbscore = L_TbtProbeTable (iTb, color, index);

    if (tbscore == bev_broken) { return ERROR_NotFound; }

    // Convert the tablebase score to the format we want and return it:

    int distance = tbscore;
    if (tbscore > 0) {
        distance = 32767 - tbscore;
    } else if (tbscore < 0) {
        distance = -32767 - tbscore;
    }
    *score = distance;
    return OK;
}


#else

////////////////////////////////////////////////////////////
//
// SCID_USE_TB is not defined, so compile empty functions:

bool
scid_TB_compiled (void) {
    return false;
}

uint
scid_TB_MaxPieces (void)
{ return 0; }

uint
scid_TB_CacheSize (void)
{ return 0; }

void
scid_TB_SetCacheSize (uint)
{ return; }

uint
scid_TB_Init (const char *)
{ return 0; }

bool
scid_TB_Available (matSigT)
{ return false; }

errorT
scid_TB_Probe (Position *, int *)
{ return ERROR_NotFound; }

#endif


//////////////////////////////////////////////////////////////////////
/// END of probe.cpp
//////////////////////////////////////////////////////////////////////

