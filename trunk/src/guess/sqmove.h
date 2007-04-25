//////////////////////////////////////////////////////////////////////
//
//  FILE:       sqmove.h
//              Square movement lookup table
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 1999-2002  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_SQMOVE_H
#define SCID_SQMOVE_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sqMove
//   Array indexed by square value and direction, giving the square
//   obtained by moving from the square in that direction.
  const squareT
sqMove[66][11] = {
                /* UP DOWN    LEFT UL  DL    RIGHT UR  DR */
   { /* A1 */  NS, A2, NS, NS, NS, NS, NS, NS, B1, B2, NS   },
   { /* B1 */  NS, B2, NS, NS, A1, A2, NS, NS, C1, C2, NS   },
   { /* C1 */  NS, C2, NS, NS, B1, B2, NS, NS, D1, D2, NS   },
   { /* D1 */  NS, D2, NS, NS, C1, C2, NS, NS, E1, E2, NS   },
   { /* E1 */  NS, E2, NS, NS, D1, D2, NS, NS, F1, F2, NS   },
   { /* F1 */  NS, F2, NS, NS, E1, E2, NS, NS, G1, G2, NS   },
   { /* G1 */  NS, G2, NS, NS, F1, F2, NS, NS, H1, H2, NS   },
   { /* H1 */  NS, H2, NS, NS, G1, G2, NS, NS, NS, NS, NS   },
   { /* A2 */  NS, A3, A1, NS, NS, NS, NS, NS, B2, B3, B1   },
   { /* B2 */  NS, B3, B1, NS, A2, A3, A1, NS, C2, C3, C1   },
   { /* C2 */  NS, C3, C1, NS, B2, B3, B1, NS, D2, D3, D1   },
   { /* D2 */  NS, D3, D1, NS, C2, C3, C1, NS, E2, E3, E1   },
   { /* E2 */  NS, E3, E1, NS, D2, D3, D1, NS, F2, F3, F1   },
   { /* F2 */  NS, F3, F1, NS, E2, E3, E1, NS, G2, G3, G1   },
   { /* G2 */  NS, G3, G1, NS, F2, F3, F1, NS, H2, H3, H1   },
   { /* H2 */  NS, H3, H1, NS, G2, G3, G1, NS, NS, NS, NS   },
   { /* A3 */  NS, A4, A2, NS, NS, NS, NS, NS, B3, B4, B2   },
   { /* B3 */  NS, B4, B2, NS, A3, A4, A2, NS, C3, C4, C2   },
   { /* C3 */  NS, C4, C2, NS, B3, B4, B2, NS, D3, D4, D2   },
   { /* D3 */  NS, D4, D2, NS, C3, C4, C2, NS, E3, E4, E2   },
   { /* E3 */  NS, E4, E2, NS, D3, D4, D2, NS, F3, F4, F2   },
   { /* F3 */  NS, F4, F2, NS, E3, E4, E2, NS, G3, G4, G2   },
   { /* G3 */  NS, G4, G2, NS, F3, F4, F2, NS, H3, H4, H2   },
   { /* H3 */  NS, H4, H2, NS, G3, G4, G2, NS, NS, NS, NS   },
   { /* A4 */  NS, A5, A3, NS, NS, NS, NS, NS, B4, B5, B3   },
   { /* B4 */  NS, B5, B3, NS, A4, A5, A3, NS, C4, C5, C3   },
   { /* C4 */  NS, C5, C3, NS, B4, B5, B3, NS, D4, D5, D3   },
   { /* D4 */  NS, D5, D3, NS, C4, C5, C3, NS, E4, E5, E3   },
   { /* E4 */  NS, E5, E3, NS, D4, D5, D3, NS, F4, F5, F3   },
   { /* F4 */  NS, F5, F3, NS, E4, E5, E3, NS, G4, G5, G3   },
   { /* G4 */  NS, G5, G3, NS, F4, F5, F3, NS, H4, H5, H3   },
   { /* H4 */  NS, H5, H3, NS, G4, G5, G3, NS, NS, NS, NS   },
   { /* A5 */  NS, A6, A4, NS, NS, NS, NS, NS, B5, B6, B4   },
   { /* B5 */  NS, B6, B4, NS, A5, A6, A4, NS, C5, C6, C4   },
   { /* C5 */  NS, C6, C4, NS, B5, B6, B4, NS, D5, D6, D4   },
   { /* D5 */  NS, D6, D4, NS, C5, C6, C4, NS, E5, E6, E4   },
   { /* E5 */  NS, E6, E4, NS, D5, D6, D4, NS, F5, F6, F4   },
   { /* F5 */  NS, F6, F4, NS, E5, E6, E4, NS, G5, G6, G4   },
   { /* G5 */  NS, G6, G4, NS, F5, F6, F4, NS, H5, H6, H4   },
   { /* H5 */  NS, H6, H4, NS, G5, G6, G4, NS, NS, NS, NS   },
   { /* A6 */  NS, A7, A5, NS, NS, NS, NS, NS, B6, B7, B5   },
   { /* B6 */  NS, B7, B5, NS, A6, A7, A5, NS, C6, C7, C5   },
   { /* C6 */  NS, C7, C5, NS, B6, B7, B5, NS, D6, D7, D5   },
   { /* D6 */  NS, D7, D5, NS, C6, C7, C5, NS, E6, E7, E5   },
   { /* E6 */  NS, E7, E5, NS, D6, D7, D5, NS, F6, F7, F5   },
   { /* F6 */  NS, F7, F5, NS, E6, E7, E5, NS, G6, G7, G5   },
   { /* G6 */  NS, G7, G5, NS, F6, F7, F5, NS, H6, H7, H5   },
   { /* H6 */  NS, H7, H5, NS, G6, G7, G5, NS, NS, NS, NS   },
   { /* A7 */  NS, A8, A6, NS, NS, NS, NS, NS, B7, B8, B6   },
   { /* B7 */  NS, B8, B6, NS, A7, A8, A6, NS, C7, C8, C6   },
   { /* C7 */  NS, C8, C6, NS, B7, B8, B6, NS, D7, D8, D6   },
   { /* D7 */  NS, D8, D6, NS, C7, C8, C6, NS, E7, E8, E6   },
   { /* E7 */  NS, E8, E6, NS, D7, D8, D6, NS, F7, F8, F6   },
   { /* F7 */  NS, F8, F6, NS, E7, E8, E6, NS, G7, G8, G6   },
   { /* G7 */  NS, G8, G6, NS, F7, F8, F6, NS, H7, H8, H6   },
   { /* H7 */  NS, H8, H6, NS, G7, G8, G6, NS, NS, NS, NS   },
   { /* A8 */  NS, NS, A7, NS, NS, NS, NS, NS, B8, NS, B7   },
   { /* B8 */  NS, NS, B7, NS, A8, NS, A7, NS, C8, NS, C7   },
   { /* C8 */  NS, NS, C7, NS, B8, NS, B7, NS, D8, NS, D7   },
   { /* D8 */  NS, NS, D7, NS, C8, NS, C7, NS, E8, NS, E7   },
   { /* E8 */  NS, NS, E7, NS, D8, NS, D7, NS, F8, NS, F7   },
   { /* F8 */  NS, NS, F7, NS, E8, NS, E7, NS, G8, NS, G7   },
   { /* G8 */  NS, NS, G7, NS, F8, NS, F7, NS, H8, NS, H7   },
   { /* H8 */  NS, NS, H7, NS, G8, NS, G7, NS, NS, NS, NS   },
   { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS, NS, NS   },
   { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS, NS, NS   }
 };

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sqLast
//   Array indexed by square value and direction, giving the last
//   square reached by moving from the square in that direction.
//   The last square is the same as the original square if moving
//   in the specified direction would move off the board.
  const squareT
sqLast[66][11] = {
                /* UP DOWN    LEFT UL  DL    RIGHT UR  DR */
   { /* A1 */  NS, A8, A1, NS, A1, A1, A1, NS, H1, H8, A1   },
   { /* B1 */  NS, B8, B1, NS, A1, A2, B1, NS, H1, H7, B1   },
   { /* C1 */  NS, C8, C1, NS, A1, A3, C1, NS, H1, H6, C1   },
   { /* D1 */  NS, D8, D1, NS, A1, A4, D1, NS, H1, H5, D1   },
   { /* E1 */  NS, E8, E1, NS, A1, A5, E1, NS, H1, H4, E1   },
   { /* F1 */  NS, F8, F1, NS, A1, A6, F1, NS, H1, H3, F1   },
   { /* G1 */  NS, G8, G1, NS, A1, A7, G1, NS, H1, H2, G1   },
   { /* H1 */  NS, H8, H1, NS, A1, A8, H1, NS, H1, H1, H1   },
   { /* A2 */  NS, A8, A1, NS, A2, A2, A2, NS, H2, G8, B1   },
   { /* B2 */  NS, B8, B1, NS, A2, A3, A1, NS, H2, H8, C1   },
   { /* C2 */  NS, C8, C1, NS, A2, A4, B1, NS, H2, H7, D1   },
   { /* D2 */  NS, D8, D1, NS, A2, A5, C1, NS, H2, H6, E1   },
   { /* E2 */  NS, E8, E1, NS, A2, A6, D1, NS, H2, H5, F1   },
   { /* F2 */  NS, F8, F1, NS, A2, A7, E1, NS, H2, H4, G1   },
   { /* G2 */  NS, G8, G1, NS, A2, A8, F1, NS, H2, H3, H1   },
   { /* H2 */  NS, H8, H1, NS, A2, B8, G1, NS, H2, H2, H2   },
   { /* A3 */  NS, A8, A1, NS, A3, A3, A3, NS, H3, F8, C1   },
   { /* B3 */  NS, B8, B1, NS, A3, A4, A2, NS, H3, G8, D1   },
   { /* C3 */  NS, C8, C1, NS, A3, A5, A1, NS, H3, H8, E1   },
   { /* D3 */  NS, D8, D1, NS, A3, A6, B1, NS, H3, H7, F1   },
   { /* E3 */  NS, E8, E1, NS, A3, A7, C1, NS, H3, H6, G1   },
   { /* F3 */  NS, F8, F1, NS, A3, A8, D1, NS, H3, H5, H1   },
   { /* G3 */  NS, G8, G1, NS, A3, B8, E1, NS, H3, H4, H2   },
   { /* H3 */  NS, H8, H1, NS, A3, C8, F1, NS, H3, H3, H3   },
   { /* A4 */  NS, A8, A1, NS, A4, A4, A4, NS, H4, E8, D1   },
   { /* B4 */  NS, B8, B1, NS, A4, A5, A3, NS, H4, F8, E1   },
   { /* C4 */  NS, C8, C1, NS, A4, A6, A2, NS, H4, G8, F1   },
   { /* D4 */  NS, D8, D1, NS, A4, A7, A1, NS, H4, H8, G1   },
   { /* E4 */  NS, E8, E1, NS, A4, A8, B1, NS, H4, H7, H1   },
   { /* F4 */  NS, F8, F1, NS, A4, B8, C1, NS, H4, H6, H2   },
   { /* G4 */  NS, G8, G1, NS, A4, C8, D1, NS, H4, H5, H3   },
   { /* H4 */  NS, H8, H1, NS, A4, D8, E1, NS, H4, H4, H4   },
   { /* A5 */  NS, A8, A1, NS, A5, A5, A5, NS, H5, D8, E1   },
   { /* B5 */  NS, B8, B1, NS, A5, A6, A4, NS, H5, E8, F1   },
   { /* C5 */  NS, C8, C1, NS, A5, A7, A3, NS, H5, F8, G1   },
   { /* D5 */  NS, D8, D1, NS, A5, A8, A2, NS, H5, G8, H1   },
   { /* E5 */  NS, E8, E1, NS, A5, B8, A1, NS, H5, H8, H2   },
   { /* F5 */  NS, F8, F1, NS, A5, C8, B1, NS, H5, H7, H3   },
   { /* G5 */  NS, G8, G1, NS, A5, D8, C1, NS, H5, H6, H4   },
   { /* H5 */  NS, H8, H1, NS, A5, E8, D1, NS, H5, H5, H5   },
   { /* A6 */  NS, A8, A1, NS, A6, A6, A6, NS, H6, C8, F1   },
   { /* B6 */  NS, B8, B1, NS, A6, A7, A5, NS, H6, D8, G1   },
   { /* C6 */  NS, C8, C1, NS, A6, A8, A4, NS, H6, E8, H1   },
   { /* D6 */  NS, D8, D1, NS, A6, B8, A3, NS, H6, F8, H2   },
   { /* E6 */  NS, E8, E1, NS, A6, C8, A2, NS, H6, G8, H3   },
   { /* F6 */  NS, F8, F1, NS, A6, D8, A1, NS, H6, H8, H4   },
   { /* G6 */  NS, G8, G1, NS, A6, E8, B1, NS, H6, H7, H5   },
   { /* H6 */  NS, H8, H1, NS, A6, F8, C1, NS, H6, H6, H6   },
   { /* A7 */  NS, A8, A1, NS, A7, A7, A7, NS, H7, B8, G1   },
   { /* B7 */  NS, B8, B1, NS, A7, A8, A6, NS, H7, C8, H1   },
   { /* C7 */  NS, C8, C1, NS, A7, B8, A5, NS, H7, D8, H2   },
   { /* D7 */  NS, D8, D1, NS, A7, C8, A4, NS, H7, E8, H3   },
   { /* E7 */  NS, E8, E1, NS, A7, D8, A3, NS, H7, F8, H4   },
   { /* F7 */  NS, F8, F1, NS, A7, E8, A2, NS, H7, G8, H5   },
   { /* G7 */  NS, G8, G1, NS, A7, F8, A1, NS, H7, H8, H6   },
   { /* H7 */  NS, H8, H1, NS, A7, G8, B1, NS, H7, H7, H7   },
   { /* A8 */  NS, A8, A1, NS, A8, A8, A8, NS, H8, A8, H1   },
   { /* B8 */  NS, B8, B1, NS, A8, B8, A7, NS, H8, B8, H2   },
   { /* C8 */  NS, C8, C1, NS, A8, C8, A6, NS, H8, C8, H3   },
   { /* D8 */  NS, D8, D1, NS, A8, D8, A5, NS, H8, D8, H4   },
   { /* E8 */  NS, E8, E1, NS, A8, E8, A4, NS, H8, E8, H5   },
   { /* F8 */  NS, F8, F1, NS, A8, F8, A3, NS, H8, F8, H6   },
   { /* G8 */  NS, G8, G1, NS, A8, G8, A2, NS, H8, G8, H7   },
   { /* H8 */  NS, H8, H1, NS, A8, H8, A1, NS, H8, H8, H8   },
   { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS, NS, NS   },
   { /* NS */  NS, NS, NS, NS, NS, NS, NS, NS, NS, NS, NS   }
};

#endif

//////////////////////////////////////////////////////////////////////
//  EOF: sqmove.h
//////////////////////////////////////////////////////////////////////

