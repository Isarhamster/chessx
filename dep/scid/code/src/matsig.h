//////////////////////////////////////////////////////////////////////
//
//  FILE:       matsig.h
//              Material signatures and home-pawn signatures.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.9
//
//  Notice:     Copyright (c) 2000  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_MATSIG_H
#define SCID_MATSIG_H

#include "common.h"
#include <algorithm>
#include <string>

// Matsigs are 32-bit unsigned ints.  We only use 24 bits of this.

typedef uint32_t matSigT;

// From most significant bits down to least, the matsig layout is:
// Bits 22-33:  WQ    Bits 10-11:  BQ
// Bits 20-21:  WN    Bits 08-09:  BR
// Bits 18-19:  WB    Bits 06-07:  BB
// Bits 16-17:  WN    Bits 04-05:  BN
// Bits 12-15:  WP    Bits 00-03:  BP

// This means that pawn counts from 0 to 8 are possible, but for other
// pieces only counts up to 3 are possible.


// Shifts:

#define SHIFT_BP 0
#define SHIFT_BN 4
#define SHIFT_BB 6
#define SHIFT_BR 8
#define SHIFT_BQ 10
#define SHIFT_WP 12
#define SHIFT_WN 16
#define SHIFT_WB 18
#define SHIFT_WR 20
#define SHIFT_WQ 22


// Masks:
                            //         28   24   20   16   12    8    4    0
#define MASK_BP 0x0000000F  // 0- 3: .... .... .... .... .... .... .... 1111
#define MASK_BN 0x00000030  // 4- 5: .... .... .... .... .... .... ..11 ....
#define MASK_BB 0x000000C0  // 6- 7: .... .... .... .... .... .... 11.. ....
#define MASK_BR 0x00000300  // 8- 9: .... .... .... .... .... ..11 .... ....
#define MASK_BQ 0x00000C00  //10-11: .... .... .... .... .... 11.. .... ....
#define MASK_WP 0x0000F000  //12-15: .... .... .... .... 1111 .... .... ....
#define MASK_WN 0x00030000  //16-17: .... .... .... ..11 .... .... .... ....
#define MASK_WB 0x000C0000  //18-19: .... .... .... 11.. .... .... .... ....
#define MASK_WR 0x00300000  //20-21: .... .... ..11 .... .... .... .... ....
#define MASK_WQ 0x00C00000  //29-31: .... .... 11.. .... .... .... .... ....


// The arrays MASK_BY_PIECE and SHIFT_BY_PIECE are useful for setting
// matsigs by piece type:

const matSigT
MASK_BY_PIECE [16] = {
    0,        //  0: Empty
    0,        //  1: WK
    MASK_WQ,  //  2: WQ
    MASK_WR,  //  3: WR
    MASK_WB,  //  4: WB
    MASK_WN,  //  5: WN
    MASK_WP,  //  6: WP
    0, 0,     //  7, 8: Invalid pieces
    0,        //  9: BK
    MASK_BQ,  // 10: BQ
    MASK_BR,  // 11: BR
    MASK_BB,  // 12: BB
    MASK_BN,  // 13: BN
    MASK_BP,  // 14: BP
    0         // 15: Invalid piece
};

const uint
SHIFT_BY_PIECE[16] = {
    0, 0,      //  0: Empty,  1: WK
    SHIFT_WQ,  //  2: WQ
    SHIFT_WR,  //  3: WR
    SHIFT_WB,  //  4: WB
    SHIFT_WN,  //  5: WN
    SHIFT_WP,  //  6: WP
    0, 0, 0,   //  7, 8: Invalid pieces,  9: BK
    SHIFT_BQ,  // 10: BQ
    SHIFT_BR,  // 11: BR
    SHIFT_BB,  // 12: BB
    SHIFT_BN,  // 13: BN
    SHIFT_BP,  // 14: BP
    0          // 15: Invalid piece
};


// Quick way to flip colors: just switch the upper/lower 12 bits

#define MATSIG_FlipColor(x) ((x) >> 12) | (((x) & 0x00000FFF) << 12)


// Quick tests for non-zero counts of a piece type:

#define MATSIG_Has_WQ(x)  ((x) & MASK_WQ)
#define MATSIG_Has_BQ(x)  ((x) & MASK_BQ)
#define MATSIG_Has_WR(x)  ((x) & MASK_WR)
#define MATSIG_Has_BR(x)  ((x) & MASK_BR)
#define MATSIG_Has_WB(x)  ((x) & MASK_WB)
#define MATSIG_Has_BB(x)  ((x) & MASK_BB)
#define MATSIG_Has_WN(x)  ((x) & MASK_WN)
#define MATSIG_Has_BN(x)  ((x) & MASK_BN)
#define MATSIG_Has_WP(x)  ((x) & MASK_WP)
#define MATSIG_Has_BP(x)  ((x) & MASK_BP)

#define MATSIG_HasQueens(x)   ((x) & (MASK_WQ | MASK_BQ))
#define MATSIG_HasRooks(x)    ((x) & (MASK_WR | MASK_BR))
#define MATSIG_HasBishops(x)  ((x) & (MASK_WB | MASK_BB))
#define MATSIG_HasKnights(x)  ((x) & (MASK_WN | MASK_BN))
#define MATSIG_HasPawns(x)    ((x) & (MASK_WP | MASK_BP))


// Macros to extract a particular count:

#define MATSIG_Count_WQ(x)  (((x) & MASK_WQ) >> SHIFT_WQ)
#define MATSIG_Count_BQ(x)  (((x) & MASK_BQ) >> SHIFT_BQ)
#define MATSIG_Count_WR(x)  (((x) & MASK_WR) >> SHIFT_WR)
#define MATSIG_Count_BR(x)  (((x) & MASK_BR) >> SHIFT_BR)
#define MATSIG_Count_WB(x)  (((x) & MASK_WB) >> SHIFT_WB)
#define MATSIG_Count_BB(x)  (((x) & MASK_BB) >> SHIFT_BB)
#define MATSIG_Count_WN(x)  (((x) & MASK_WN) >> SHIFT_WN)
#define MATSIG_Count_BN(x)  (((x) & MASK_BN) >> SHIFT_BN)
#define MATSIG_Count_WP(x)  (((x) & MASK_WP) >> SHIFT_WP)
#define MATSIG_Count_BP(x)  (((x) & MASK_BP) >> SHIFT_BP)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// matsig_getCount():
//      Inline routine to extract a count of a certain piece type.
//
inline uint
matsig_getCount (matSigT m, pieceT p)
{
    return (m & MASK_BY_PIECE[p]) >> SHIFT_BY_PIECE[p];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// matsig_setCount():
//      Inline routine to set a particular count.
//
inline matSigT
matsig_setCount (matSigT m, pieceT p, uint count)
{
    // First we clear the old mask for this piece:
    m &= ~(MASK_BY_PIECE[p]);

	 // Avoid overflow.
	 if (p != PAWN && count > 3)
		 count = 3;

    // Now we OR to add the new value in:
    m |= ((uint) count) << SHIFT_BY_PIECE[p];
    return m;
}


// Common constant matsigs:

const matSigT MATSIG_Empty = 0;

const matSigT MATSIG_StdStart =
   ((1 << SHIFT_WQ) | (1 << SHIFT_BQ) | (2 << SHIFT_WR) | (2 << SHIFT_BR) |
    (2 << SHIFT_WB) | (2 << SHIFT_BB) | (2 << SHIFT_WN) | (2 << SHIFT_BN) |
    (8 << SHIFT_WP) | (8 << SHIFT_BP));


//
// Public functions found in matsig.cpp:
//


// matsig_makeString: sets s to be a string representation of the sig,
std::string
matsig_makeString (matSigT matsig);


// matsig_isReachable: returns true if a game currently
//     at a position with the signature <start>, could possibly reach
//     the signature <target>. This is useful for quick tests for material
//     searches. For example, if we store the final matsig of every game,
//     we can speedup a material search by ONLY searching the games that
//     have matsig_isReachable(searchsig, finalsig) = 1, or have promotions.
//     Example: if searchsig requires neither side to have queens, but
//     finalsig for a game shows a WQ (and no promotions), the game could
//     not possibly match.
//     If promos is true, only the pawn counts are checked, since other
//     material could reappear on the board due to a promotion.
//     If upromo is true, there are underpromotions (to R, B or N) but
//     if only promos is true, all promotions are to Queens only.
bool
matsig_isReachable (matSigT mStart, matSigT mTarget, bool promos, bool upromo);

// matsig_isReachablePawns:
//    like matsig_isReachable, but considering pawns only.
inline bool
matsig_isReachablePawns (matSigT mStart, matSigT mTarget)
{
    if (MATSIG_Count_WP(mStart) < MATSIG_Count_WP(mTarget)) { return false; }
    if (MATSIG_Count_BP(mStart) < MATSIG_Count_BP(mTarget)) { return false; }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// matsig_Make():
//      Make a material sig, given an array of material counts as
//      stored in a Position.
//
inline matSigT matsig_Make(const byte* materialCounts) {
    matSigT m = 0;
    m |= std::min<matSigT>(3, materialCounts[WQ]) << SHIFT_WQ;
    m |= std::min<matSigT>(3, materialCounts[WR]) << SHIFT_WR;
    m |= std::min<matSigT>(3, materialCounts[WB]) << SHIFT_WB;
    m |= std::min<matSigT>(3, materialCounts[WN]) << SHIFT_WN;
    m |= matSigT(materialCounts[WP]) << SHIFT_WP;
    m |= std::min<matSigT>(3, materialCounts[BQ]) << SHIFT_BQ;
    m |= std::min<matSigT>(3, materialCounts[BR]) << SHIFT_BR;
    m |= std::min<matSigT>(3, materialCounts[BB]) << SHIFT_BB;
    m |= std::min<matSigT>(3, materialCounts[BN]) << SHIFT_BN;
    m |= matSigT(materialCounts[BP]) << SHIFT_BP;
    return m;
}


// Common HPSigs:
// 0 => no pawns still on their original 2nd/7th rank squares.
// 0xFFFF => all 16 pawns still on their original 2nd/7th rank squares.

const uint
HPSIG_Empty = 0x0;

const uint
HPSIG_StdStart = 0xFFFF;


bool
hpSig_PossibleMatch (uint hpSig, const byte * changeList);

bool
hpSig_Prefix (const byte * changeListA, const byte * changeListB);

uint
hpSig_Final (const byte * changeList);

// hpSig_bitMask[]: used to add or clear bits in an hpSig.
//
static const uint hpSig_bitMask [16] = {
    // a2 to h2:
    0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100,
    // a7 to h7:
    0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

inline uint
hpSig_AddPawn (uint hpSig, colorT color, fyleT fyle)
{
    ASSERT (color == WHITE  ||  color == BLACK);
    ASSERT (fyle <= H_FYLE);

    uint val = (uint) fyle;
    if (color == BLACK) val += 8;
    return hpSig | hpSig_bitMask [val];
}

inline uint
hpSig_ClearPawn (uint hpSig, colorT color, fyleT fyle)
{
    ASSERT (color == WHITE  ||  color == BLACK);
    ASSERT (fyle <= H_FYLE);

    uint val = (uint) fyle;
    if (color == BLACK) val += 8;
    return hpSig & ~(hpSig_bitMask [val]);
}

/**
 * Creates a 16-bits bitmap of the missing pawns in the home ranks.
 *
 * Used to speed up the searches of positions with the same pawn structure.
 * @returns a std::pair containing the bitmap and the number of moved pawns.
 */
inline std::pair<uint16_t, uint16_t> hpSig_make(const pieceT* board) {
	int hpSig = 0;
	int nMoved = 0;
	const pieceT* b = board + A2;
	// clang-format off
	if (*b != WP) { hpSig |= 0x8000; ++nMoved; }  b++;  /* a2 */
	if (*b != WP) { hpSig |= 0x4000; ++nMoved; }  b++;  /* b2 */
	if (*b != WP) { hpSig |= 0x2000; ++nMoved; }  b++;  /* c2 */
	if (*b != WP) { hpSig |= 0x1000; ++nMoved; }  b++;  /* d2 */
	if (*b != WP) { hpSig |= 0x0800; ++nMoved; }  b++;  /* e2 */
	if (*b != WP) { hpSig |= 0x0400; ++nMoved; }  b++;  /* f2 */
	if (*b != WP) { hpSig |= 0x0200; ++nMoved; }  b++;  /* g2 */
	if (*b != WP) { hpSig |= 0x0100; ++nMoved; }        /* h2 */
	b = board + A7;
	if (*b != BP) { hpSig |= 0x0080; ++nMoved; }  b++;  /* a7 */
	if (*b != BP) { hpSig |= 0x0040; ++nMoved; }  b++;  /* b7 */
	if (*b != BP) { hpSig |= 0x0020; ++nMoved; }  b++;  /* c7 */
	if (*b != BP) { hpSig |= 0x0010; ++nMoved; }  b++;  /* d7 */
	if (*b != BP) { hpSig |= 0x0008; ++nMoved; }  b++;  /* e7 */
	if (*b != BP) { hpSig |= 0x0004; ++nMoved; }  b++;  /* f7 */
	if (*b != BP) { hpSig |= 0x0002; ++nMoved; }  b++;  /* g7 */
	if (*b != BP) { hpSig |= 0x0001; ++nMoved; }        /* h7 */
	// clang-format on

	return {static_cast<uint16_t>(hpSig), static_cast<uint16_t>(nMoved)};
}

inline bool hpSig_match(int hpSig, int nMoved, const byte* changeList) {
	// The first byte of a changeList is the length (in halfbytes) of the
	// list, which can be any value from 0 to 16 inclusive.
	if (*changeList == 16 && nMoved == 16)
		return true;
	if (*changeList++ < nMoved)
		return false;

	int sig = 0;
	for (int i = 0, n = nMoved / 2; i < n; ++i) {
		sig |= 1 << (*changeList >> 4);
		sig |= 1 << (*changeList++ & 0x0F);
	}
	if (nMoved & 1)
		sig |= 1 << (*changeList >> 4);

	return sig == hpSig;
}

#endif

//////////////////////////////////////////////////////////////////////
//  EOF: matsig.h
//////////////////////////////////////////////////////////////////////

