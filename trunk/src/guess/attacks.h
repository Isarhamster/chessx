//////////////////////////////////////////////////////////////////////
//
//  FILE:       attacks.h
//              Lists of attack squares
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.0
//
//  Notice:     Copyright (c) 1999  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_ATTACKS_H
#define SCID_ATTACKS_H

#ifndef SCID_COMMON_H
#include "common.h"
#endif

namespace Guess
{

// NOTE: These arrays have been automatically generated. Don't change
//       them unless the square or piece representations change!

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Knight Moves:
//    knightAttacks[sq] is an array of the squares a knight on sq
//    could move to. There are 9 target squares even though a knight
//    can only have at most 8 legal moves, so we can end the list with
//    an "NS" (null square).
//
//    Example code to iterate through the knight moves for a square:
//
//    squareT *dest = knightAttacks[fromSquare];
//    while (*dest != NULL_SQUARE) {
//        /* do desired action.... */
//        dest++;
//    }
//

const squareT
knightAttacks[66][9] =
{
    { /* A1 */  C2, B3, NS, NS, NS, NS, NS, NS, NS },
    { /* B1 */  D2, A3, C3, NS, NS, NS, NS, NS, NS },
    { /* C1 */  A2, E2, B3, D3, NS, NS, NS, NS, NS },
    { /* D1 */  B2, F2, C3, E3, NS, NS, NS, NS, NS },
    { /* E1 */  C2, G2, D3, F3, NS, NS, NS, NS, NS },
    { /* F1 */  D2, H2, E3, G3, NS, NS, NS, NS, NS },
    { /* G1 */  E2, F3, H3, NS, NS, NS, NS, NS, NS },
    { /* H1 */  F2, G3, NS, NS, NS, NS, NS, NS, NS },
    { /* A2 */  C1, C3, B4, NS, NS, NS, NS, NS, NS },
    { /* B2 */  D1, D3, A4, C4, NS, NS, NS, NS, NS },
    { /* C2 */  A1, E1, A3, E3, B4, D4, NS, NS, NS },
    { /* D2 */  B1, F1, B3, F3, C4, E4, NS, NS, NS },
    { /* E2 */  C1, G1, C3, G3, D4, F4, NS, NS, NS },
    { /* F2 */  D1, H1, D3, H3, E4, G4, NS, NS, NS },
    { /* G2 */  E1, E3, F4, H4, NS, NS, NS, NS, NS },
    { /* H2 */  F1, F3, G4, NS, NS, NS, NS, NS, NS },
    { /* A3 */  B1, C2, C4, B5, NS, NS, NS, NS, NS },
    { /* B3 */  A1, C1, D2, D4, A5, C5, NS, NS, NS },
    { /* C3 */  B1, D1, A2, E2, A4, E4, B5, D5, NS },
    { /* D3 */  C1, E1, B2, F2, B4, F4, C5, E5, NS },
    { /* E3 */  D1, F1, C2, G2, C4, G4, D5, F5, NS },
    { /* F3 */  E1, G1, D2, H2, D4, H4, E5, G5, NS },
    { /* G3 */  F1, H1, E2, E4, F5, H5, NS, NS, NS },
    { /* H3 */  G1, F2, F4, G5, NS, NS, NS, NS, NS },
    { /* A4 */  B2, C3, C5, B6, NS, NS, NS, NS, NS },
    { /* B4 */  A2, C2, D3, D5, A6, C6, NS, NS, NS },
    { /* C4 */  B2, D2, A3, E3, A5, E5, B6, D6, NS },
    { /* D4 */  C2, E2, B3, F3, B5, F5, C6, E6, NS },
    { /* E4 */  D2, F2, C3, G3, C5, G5, D6, F6, NS },
    { /* F4 */  E2, G2, D3, H3, D5, H5, E6, G6, NS },
    { /* G4 */  F2, H2, E3, E5, F6, H6, NS, NS, NS },
    { /* H4 */  G2, F3, F5, G6, NS, NS, NS, NS, NS },
    { /* A5 */  B3, C4, C6, B7, NS, NS, NS, NS, NS },
    { /* B5 */  A3, C3, D4, D6, A7, C7, NS, NS, NS },
    { /* C5 */  B3, D3, A4, E4, A6, E6, B7, D7, NS },
    { /* D5 */  C3, E3, B4, F4, B6, F6, C7, E7, NS },
    { /* E5 */  D3, F3, C4, G4, C6, G6, D7, F7, NS },
    { /* F5 */  E3, G3, D4, H4, D6, H6, E7, G7, NS },
    { /* G5 */  F3, H3, E4, E6, F7, H7, NS, NS, NS },
    { /* H5 */  G3, F4, F6, G7, NS, NS, NS, NS, NS },
    { /* A6 */  B4, C5, C7, B8, NS, NS, NS, NS, NS },
    { /* B6 */  A4, C4, D5, D7, A8, C8, NS, NS, NS },
    { /* C6 */  B4, D4, A5, E5, A7, E7, B8, D8, NS },
    { /* D6 */  C4, E4, B5, F5, B7, F7, C8, E8, NS },
    { /* E6 */  D4, F4, C5, G5, C7, G7, D8, F8, NS },
    { /* F6 */  E4, G4, D5, H5, D7, H7, E8, G8, NS },
    { /* G6 */  F4, H4, E5, E7, F8, H8, NS, NS, NS },
    { /* H6 */  G4, F5, F7, G8, NS, NS, NS, NS, NS },
    { /* A7 */  B5, C6, C8, NS, NS, NS, NS, NS, NS },
    { /* B7 */  A5, C5, D6, D8, NS, NS, NS, NS, NS },
    { /* C7 */  B5, D5, A6, E6, A8, E8, NS, NS, NS },
    { /* D7 */  C5, E5, B6, F6, B8, F8, NS, NS, NS },
    { /* E7 */  D5, F5, C6, G6, C8, G8, NS, NS, NS },
    { /* F7 */  E5, G5, D6, H6, D8, H8, NS, NS, NS },
    { /* G7 */  F5, H5, E6, E8, NS, NS, NS, NS, NS },
    { /* H7 */  G5, F6, F8, NS, NS, NS, NS, NS, NS },
    { /* A8 */  B6, C7, NS, NS, NS, NS, NS, NS, NS },
    { /* B8 */  A6, C6, D7, NS, NS, NS, NS, NS, NS },
    { /* C8 */  B6, D6, A7, E7, NS, NS, NS, NS, NS },
    { /* D8 */  C6, E6, B7, F7, NS, NS, NS, NS, NS },
    { /* E8 */  D6, F6, C7, G7, NS, NS, NS, NS, NS },
    { /* F8 */  E6, G6, D7, H7, NS, NS, NS, NS, NS },
    { /* G8 */  F6, H6, E7, NS, NS, NS, NS, NS, NS },
    { /* H8 */  G6, F7, NS, NS, NS, NS, NS, NS, NS },
    { /* COL*/  NS, NS, NS, NS, NS, NS, NS, NS, NS },
    { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS }
};



// King moves:
//    As for the knight moves, the secondary array size is 9 even
//    though there are only 8 possible king moves, so there can
//    always be a NULL_SQUARE terminating the list of moves for each
//    square.  Castling moves are not included here.

const squareT
kingAttacks[66][9] =
{
    { /* A1 */  B1, A2, B2, NS, NS, NS, NS, NS, NS },
    { /* B1 */  A1, C1, A2, B2, C2, NS, NS, NS, NS },
    { /* C1 */  B1, D1, B2, C2, D2, NS, NS, NS, NS },
    { /* D1 */  C1, E1, C2, D2, E2, NS, NS, NS, NS },
    { /* E1 */  D1, F1, D2, E2, F2, NS, NS, NS, NS },
    { /* F1 */  E1, G1, E2, F2, G2, NS, NS, NS, NS },
    { /* G1 */  F1, H1, F2, G2, H2, NS, NS, NS, NS },
    { /* H1 */  G1, G2, H2, NS, NS, NS, NS, NS, NS },
    { /* A2 */  A1, B1, B2, A3, B3, NS, NS, NS, NS },
    { /* B2 */  A1, B1, C1, A2, C2, A3, B3, C3, NS },
    { /* C2 */  B1, C1, D1, B2, D2, B3, C3, D3, NS },
    { /* D2 */  C1, D1, E1, C2, E2, C3, D3, E3, NS },
    { /* E2 */  D1, E1, F1, D2, F2, D3, E3, F3, NS },
    { /* F2 */  E1, F1, G1, E2, G2, E3, F3, G3, NS },
    { /* G2 */  F1, G1, H1, F2, H2, F3, G3, H3, NS },
    { /* H2 */  G1, H1, G2, G3, H3, NS, NS, NS, NS },
    { /* A3 */  A2, B2, B3, A4, B4, NS, NS, NS, NS },
    { /* B3 */  A2, B2, C2, A3, C3, A4, B4, C4, NS },
    { /* C3 */  B2, C2, D2, B3, D3, B4, C4, D4, NS },
    { /* D3 */  C2, D2, E2, C3, E3, C4, D4, E4, NS },
    { /* E3 */  D2, E2, F2, D3, F3, D4, E4, F4, NS },
    { /* F3 */  E2, F2, G2, E3, G3, E4, F4, G4, NS },
    { /* G3 */  F2, G2, H2, F3, H3, F4, G4, H4, NS },
    { /* H3 */  G2, H2, G3, G4, H4, NS, NS, NS, NS },
    { /* A4 */  A3, B3, B4, A5, B5, NS, NS, NS, NS },
    { /* B4 */  A3, B3, C3, A4, C4, A5, B5, C5, NS },
    { /* C4 */  B3, C3, D3, B4, D4, B5, C5, D5, NS },
    { /* D4 */  C3, D3, E3, C4, E4, C5, D5, E5, NS },
    { /* E4 */  D3, E3, F3, D4, F4, D5, E5, F5, NS },
    { /* F4 */  E3, F3, G3, E4, G4, E5, F5, G5, NS },
    { /* G4 */  F3, G3, H3, F4, H4, F5, G5, H5, NS },
    { /* H4 */  G3, H3, G4, G5, H5, NS, NS, NS, NS },
    { /* A5 */  A4, B4, B5, A6, B6, NS, NS, NS, NS },
    { /* B5 */  A4, B4, C4, A5, C5, A6, B6, C6, NS },
    { /* C5 */  B4, C4, D4, B5, D5, B6, C6, D6, NS },
    { /* D5 */  C4, D4, E4, C5, E5, C6, D6, E6, NS },
    { /* E5 */  D4, E4, F4, D5, F5, D6, E6, F6, NS },
    { /* F5 */  E4, F4, G4, E5, G5, E6, F6, G6, NS },
    { /* G5 */  F4, G4, H4, F5, H5, F6, G6, H6, NS },
    { /* H5 */  G4, H4, G5, G6, H6, NS, NS, NS, NS },
    { /* A6 */  A5, B5, B6, A7, B7, NS, NS, NS, NS },
    { /* B6 */  A5, B5, C5, A6, C6, A7, B7, C7, NS },
    { /* C6 */  B5, C5, D5, B6, D6, B7, C7, D7, NS },
    { /* D6 */  C5, D5, E5, C6, E6, C7, D7, E7, NS },
    { /* E6 */  D5, E5, F5, D6, F6, D7, E7, F7, NS },
    { /* F6 */  E5, F5, G5, E6, G6, E7, F7, G7, NS },
    { /* G6 */  F5, G5, H5, F6, H6, F7, G7, H7, NS },
    { /* H6 */  G5, H5, G6, G7, H7, NS, NS, NS, NS },
    { /* A7 */  A6, B6, B7, A8, B8, NS, NS, NS, NS },
    { /* B7 */  A6, B6, C6, A7, C7, A8, B8, C8, NS },
    { /* C7 */  B6, C6, D6, B7, D7, B8, C8, D8, NS },
    { /* D7 */  C6, D6, E6, C7, E7, C8, D8, E8, NS },
    { /* E7 */  D6, E6, F6, D7, F7, D8, E8, F8, NS },
    { /* F7 */  E6, F6, G6, E7, G7, E8, F8, G8, NS },
    { /* G7 */  F6, G6, H6, F7, H7, F8, G8, H8, NS },
    { /* H7 */  G6, H6, G7, G8, H8, NS, NS, NS, NS },
    { /* A8 */  A7, B7, B8, NS, NS, NS, NS, NS, NS },
    { /* B8 */  A7, B7, C7, A8, C8, NS, NS, NS, NS },
    { /* C8 */  B7, C7, D7, B8, D8, NS, NS, NS, NS },
    { /* D8 */  C7, D7, E7, C8, E8, NS, NS, NS, NS },
    { /* E8 */  D7, E7, F7, D8, F8, NS, NS, NS, NS },
    { /* F8 */  E7, F7, G7, E8, G8, NS, NS, NS, NS },
    { /* G8 */  F7, G7, H7, F8, H8, NS, NS, NS, NS },
    { /* H8 */  G7, H7, G8, NS, NS, NS, NS, NS, NS },
    { /* COL*/  NS, NS, NS, NS, NS, NS, NS, NS, NS },
    { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS }
};

} // End namespace Guess

#endif  // #ifndef SCID_ATTACKS_H

//////////////////////////////////////////////////////////////////////
//   EOF:  attacks.h
//////////////////////////////////////////////////////////////////////

