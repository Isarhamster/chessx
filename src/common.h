//////////////////////////////////////////////////////////////////////
//
//  FILE:       common.h
//              Common macros, structures and constants.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.6.6
//
//  Notice:     Copyright (c) 2000-2004  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//              Copyright (c) 2006-2007 Pascal Georges
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_COMMON_H
#define SCID_COMMON_H

#include <cstddef>
#if defined(_MSC_VER) && _MSC_VER <= 1600
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
typedef __int32  int32_t;
#else
#include <stdint.h>
#endif // _MSC_VER <= 1600

#include "board_def.h"
#include "error.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CONSTANTS:

typedef unsigned short versionT;

// Version: div by 100 for major number, modulo 100 for minor number
// so 109 = 1.9, 110 = 1.10, etc.

const versionT SCID_VERSION = 400;     // Current file format version = 4.0
const versionT SCID_OLDEST_VERSION = 300; // Oldest readable file format: 3.0

const char SCID_VERSION_STRING[] = "4.7.1";     // Current Scid version
const char SCID_WEBSITE[] = "http://scid.sourceforge.net/";

const char PGN_SUFFIX[] = ".pgn";


//////////////////////////////////////////////////////////////////////
// ASSERT macro: asserts an expression. Differs from the standard
//    assert in that it does NOT print the expression (this is a waste,
//    if an assert fails you can go to the code to see why) and that
//    it MUST be a statement, not part of a larger expression.
//    Adapted from the book "Writing Solid Code".
#include <assert.h>
#define ASSERT(f) assert(f)


// Bit Manipulations

#define BIT_7(x)            ((x) & 128)
#define BIT_6(x)            ((x) &  64)
#define BIT_5(x)            ((x) &  32)
#define BIT_4(x)            ((x) &  16)
#define BIT_3(x)            ((x) &   8)
#define BIT_2(x)            ((x) &   4)
#define BIT_1(x)            ((x) &   2)
#define BIT_0(x)            ((x) &   1)

// Upper or lower 4 bits of a byte, in the range 0..15

#define UPPER_4_BITS(x)     (((x) & 240) >> 4)      // 240 is (15 * 16)
#define LOWER_4_BITS(x)     ((x) &  15)

//  Upper or lower 12 bits of an integer, in the range 0..4095
//
#define UPPER_12_BITS(x)    (((x) & (4096 * 4095)) >> 12)
#define LOWER_12_BITS(x)    ((x) &  4095)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TYPE DEFINITIONS

//  General types

typedef unsigned char byte;      //  8 bit unsigned
typedef uint16_t ushort;         // 16 bit unsigned
typedef uint32_t uint;           // 32 bit unsigned
typedef int32_t  sint;           // 32 bit signed


//  compareT: comparison type

typedef signed int    compareT;
const compareT
    LESS_THAN = -1,   EQUAL_TO = 0,   GREATER_THAN = 1;

//  Chess Types
typedef byte                    directionT;  // e.g. UP_LEFT
typedef byte                    leftDiagT;   // Up-left diagonals
typedef byte                    rightDiagT;  // Up-right diagonals

// boardT: 64 squares plus two extra squares: one for storing the side
//   to move as a byte and one for the string terminator, so boards can
//   be compared using regular string functions:
typedef pieceT                  boardT [66];

typedef byte                    smallBoardT [32];
                                    // A more densely packed board, 2 squares
                                    // per byte.

typedef byte                    castleDirT;  // LEFT or RIGHT

//  Other Small Types

typedef ushort                  statusT;

//  Fixed String Types

typedef char    sanStringT [ 10];   // SAN Move Notation

//  File-related Types

typedef char    fileNameT [512];
typedef uint    fileLengthT;

enum fileModeT {
    FMODE_None = 0,
    FMODE_ReadOnly,
    FMODE_WriteOnly,
    FMODE_Both,
    FMODE_Create
};

//  Date type: see date.h and date.cpp

typedef uint    dateT;


// There are four name types: PLAYER, EVENT, SITE and ROUND tags.
// Names are accessed through IDs.
typedef uint32_t idNumberT; // Should be idNameT
typedef unsigned nameT;
enum {
    NAME_PLAYER, NAME_EVENT, NAME_SITE, NAME_ROUND,
    NUM_NAME_TYPES,
    NAME_INVALID = 99
};


//  Game Information types

typedef uint            gamenumT;
typedef ushort          eloT;
typedef ushort          ecoT;
typedef char            ecoStringT [6];   /* "A00j1" */

const ecoT ECO_None = 0;

// Rating types:

const byte RATING_Elo = 0;
const byte RATING_Rating = 1;
const byte RATING_Rapid = 2;
const byte RATING_ICCF = 3;
const byte RATING_USCF = 4;
const byte RATING_DWZ = 5;
const byte RATING_BCF = 6;

extern const char * ratingTypeNames [17];   // Defined in game.cpp



// Result Type

const uint NUM_RESULT_TYPES = 4;
typedef byte    resultT;
const resultT
    RESULT_None  = 0,
    RESULT_White = 1,
    RESULT_Black = 2,
    RESULT_Draw  = 3;

const uint RESULT_SCORE[4] = { 1, 2, 0, 1 };
const char RESULT_CHAR [4]       = { '*',  '1',    '0',    '='       };
const char RESULT_STR [4][4]     = { "*",  "1-0",  "0-1",  "=-="     };
const char RESULT_LONGSTR [4][8] = { "*",  "1-0",  "0-1",  "1/2-1/2" };
const resultT RESULT_OPPOSITE [4] = {
    RESULT_None, RESULT_Black, RESULT_White, RESULT_Draw
};


const castleDirT  QSIDE = 0,  KSIDE = 1;


// Minor piece definitions, used for searching by material only:
const pieceT  WM = 16, BM = 17;

const uint MAX_PIECE_TYPES = 18;


// PIECE_FLIP[]: array of pieces, with colors reversed.

const pieceT PIECE_FLIP [MAX_PIECE_TYPES] = {
    END_OF_BOARD,
    BK, BQ, BR, BB, BN, BP,
    EMPTY, EMPTY,
    WK, WQ, WR, WB, WN, WP,
    EMPTY, BM, WM
};

const bool PIECE_IS_SLIDER [8] = {
    false,
    false, true, true, true, false, false,
    false,
};

// PIECE_VALUE: Piece values, K=1000, Q=9, R=5, B=N=3, P=1

const int PIECE_VALUE [MAX_PIECE_TYPES] = {
    0,
    100, 9, 5, 3, 3, 1,
    0, 0,
    -100, -9, -5, -3, -3, -1,
    0, 3, -3
};

//
// INLINE FUNCTIONS for pieces
//


  inline bool
piece_IsWhite(pieceT p)  { return (p>=WK && p<=WP); }

  inline bool
piece_IsBlack(pieceT p)  { return (p>=BK && p<=BP); }

  inline bool
piece_IsKing(pieceT p)  { return (piece_Type(p) == KING); }

  inline bool
piece_IsQueen(pieceT p)  { return (piece_Type(p) == QUEEN); }

  inline bool
piece_IsRook(pieceT p)  { return (piece_Type(p) == ROOK); }

  inline bool
piece_IsBishop(pieceT p)  { return (piece_Type(p) == BISHOP); }

  inline bool
piece_IsKnight(pieceT p)  { return (piece_Type(p) == KNIGHT); }

  inline bool
piece_IsPawn(pieceT p)  { return (piece_Type(p) == PAWN); }

  inline bool
piece_IsSlider(pieceT p) { return PIECE_IS_SLIDER[piece_Type(p)]; }


  inline pieceT
piece_FromChar(int x)
{
    switch (x) {
    case 'K': return KING;
    case 'Q': return QUEEN;
    case 'R': return ROOK;
    case 'N': return KNIGHT;
    case 'B': return BISHOP;
    default:  return EMPTY;
    }
}

inline int
piece_Value (pieceT p)  { return PIECE_VALUE[p]; }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SQUARES AND SQUARE MACROS

  inline leftDiagT
square_LeftDiag (squareT sq)
{
    return square_Rank(sq) + square_Fyle(sq);
}

  inline rightDiagT
square_RightDiag (squareT sq)
{
    return (7 + square_Rank(sq) - square_Fyle(sq));
}

// square_Color:
//   Return WHITE for a light square, BLACK for a dark square.
  inline colorT
square_Color (squareT sq)
{
    return 1 - (square_LeftDiag(sq) & 1);
}

// square_FlipFyle:
//   Return the square with its file flipped: a1 <-> h1, b1 <-> g1, etc.
  inline squareT
square_FlipFyle (squareT sq)
{
    return square_Make (A_FYLE + H_FYLE - square_Fyle(sq), square_Rank(sq));
}

// square_FlipRank:
//   Return the square with its rank flipped: a1 <-> a8, a2 <-> a7, etc.
  inline squareT
square_FlipRank (squareT sq)
{
    return square_Make (square_Fyle(sq), RANK_1 + RANK_8 - square_Rank(sq));
}

// square_FlipDiag:
//   Return the square flipped along the a1-h8 diagonal.
  inline squareT
square_FlipDiag (squareT sq)
{
    return square_Make (square_Rank(sq), square_Fyle(sq));
}

const uint
rankFyleDist[64] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    1, 0, 1, 2, 3, 4, 5, 6,
    2, 1, 0, 1, 2, 3, 4, 5,
    3, 2, 1, 0, 1, 2, 3, 4,
    4, 3, 2, 1, 0, 1, 2, 3,
    5, 4, 3, 2, 1, 0, 1, 2,
    6, 5, 4, 3, 2, 1, 0, 1,
    7, 6, 5, 4, 3, 2, 1, 0
};

// square_Distance:
//   Return the distance in king moves between two squares.
  inline uint
square_Distance (squareT from, squareT to)
{
    ASSERT (from <= H8  &&  to <= H8);
    uint rankd = rankFyleDist[(square_Rank(from) << 3) | square_Rank(to)];
    uint fyled = rankFyleDist[(square_Fyle(from) << 3) | square_Fyle(to)];
    return (rankd > fyled) ? rankd : fyled;
}

// square_NearestCorner:
//   Return the corner (A1/H1/A8/H8) closest to the specified square.
  inline squareT
square_NearestCorner (squareT sq)
{
    if (square_Rank(sq) <= RANK_4) {
        return (square_Fyle(sq) <= D_FYLE)? A1 : H1;
    } else {
        return (square_Fyle(sq) <= D_FYLE)? A8 : H8;
    }
}

  inline bool
square_IsCornerSquare (squareT sq)
{
    return (sq == A1  ||  sq == H1  ||  sq == A8  ||  sq == H8);
}

  inline bool
square_IsEdgeSquare (squareT sq)
{
    rankT rank = square_Rank(sq);
    if (rank == RANK_1  ||  rank == RANK_8) { return true; }
    fyleT fyle = square_Fyle(sq);
    if (fyle == A_FYLE  ||  fyle == H_FYLE) { return true; }
    return false;
}

const int edgeDist[66] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 1, 2, 3, 3, 2, 1, 0,
    0, 1, 2, 3, 3, 2, 1, 0,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1
};

  inline int
square_EdgeDistance (squareT sq)
{
    return edgeDist[sq];
}

  inline bool
square_IsKnightHop (squareT from, squareT to)
{
    ASSERT (from <= H8  &&  to <= H8);
    uint rdist = rankFyleDist [(square_Rank(from) << 3) | square_Rank(to)];
    uint fdist = rankFyleDist [(square_Fyle(from) << 3) | square_Fyle(to)];
    // It is a knight hop only if one distance is two squares and the
    // other is one square -- that is, only if their product equals two.
    return ((rdist * fdist) == 2);
}

  inline char
square_FyleChar (squareT sq)
{
    return square_Fyle(sq) + 'a';
}

  inline char
square_RankChar (squareT sq)
{
    return square_Rank(sq) + '1';
}

  inline void
square_Print (squareT sq, char * str)
{
    if (sq <= H8) {
        str[0] = square_FyleChar(sq);
        str[1] = square_RankChar(sq);
        str[2] = 0;
    } else if (sq == NULL_SQUARE) {
        str[0] = 'N'; str[1] = 'S'; str[2] = 0;
    } else {
        str[0] = 'X'; str[1] = 'X'; str[2] = 0;
    }
    return;
}

// Directions:
// Up = 1, Down = 2, Left = 4, Right = 8, UpLeft = 5, UpRight = 9,
// DownLeft = 6, DownRight = 10

const directionT
    NULL_DIR = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
    UP_LEFT = (UP | LEFT),
    UP_RIGHT = (UP | RIGHT),
    DOWN_LEFT = (DOWN | LEFT),
    DOWN_RIGHT = (DOWN | RIGHT);

const directionT dirOpposite[11] = {
    NULL_DIR,
    DOWN,       // opposite of UP (1)
    UP,         // opposite of DOWN (2)
    NULL_DIR,
    RIGHT,      // opposite of LEFT (4)
    DOWN_RIGHT, // opposite of UP_LEFT (5)
    UP_RIGHT,   // opposite of DOWN_LEFT (6)
    NULL_DIR,
    LEFT,       // opposite of RIGHT (8)
    DOWN_LEFT,  // opposite of UP_RIGHT (9)
    UP_LEFT     // opposite of DOWN_RIGHT (10)
};

// direction_Opposite(): return the opposite direction to d
  inline directionT
direction_Opposite (directionT d)
{
    return dirOpposite[d];
}

// dirIsDiagonal[]: array listing the diagonal directions, for fast
//      lookup of whether a direction is a diagonal.
  const bool
dirIsDiagonal [11] = {
    false,   //  0 = NULL_DIR
    false,   //  1 = UP
    false,   //  2 = DOWN
    false,   //  3 = Invalid
    false,   //  4 = LEFT
    true,    //  5 = UP_LEFT
    true,    //  6 = DOWN_LEFT
    false,   //  7 = Invalid
    false,   //  8 = RIGHT
    true,    //  9 = UP_RIGHT
    true     // 10 = DOWN_RIGHT
};

  inline bool
direction_IsDiagonal (directionT dir)
{
    return dirIsDiagonal[dir];
}

// dirDelta:
//   Array giving the board delta of moving to the next square
//   in that direction.
  const int
dirDelta[11] = {
    0,    // NULL_DIR
    8,    // UP
   -8,    // DOWN
    0,    // Invalid
   -1,    // LEFT
    7,    // UP_LEFT
   -9,    // DOWN_LEFT
    0,    // Invalid
    1,    // RIGHT
    9,    // UP_RIGHT
   -7     // DOWN_RIGHT
};

  inline int
direction_Delta (directionT dir)
{
    return dirDelta[dir];
}

// The starting Board
//
  const boardT
START_BOARD =
{
    WR, WN, WB, WQ, WK, WB, WN, WR,    // A1--H1
    WP, WP, WP, WP, WP, WP, WP, WP,    // A2--H2
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    BP, BP, BP, BP, BP, BP, BP, BP,
    BR, BN, BB, BQ, BK, BB, BN, BR,
    EMPTY, // COLOR_SQUARE
    END_OF_BOARD  // NULL_SQUARE
};


// Square colors for the standard chess board:
//
  const colorT
BOARD_SQUARECOLOR[66] = {
    BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE,  // a1-h1
    WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK,  // a2-h2
    BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE,  // a3-h3
    WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK,  // a4-h4
    BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE,  // a5-h5
    WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK,  // a6-h6
    BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE,  // a7-h7
    WHITE, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK,  // a8-h8
    NOCOLOR, NOCOLOR  // Color square and Null square
};

  inline int
board_Compare (const pieceT * b1, const pieceT * b2)
{
    for (uint i=0; i < 64; i++) {
        if (*b1 < *b2) { return -1; }
        if (*b1 > *b2) { return 1; }
        b1++; b2++;
    }
    return 0;
}

// square_Adjacent: returns 1 if the two squares are adjacent. Note that
//    diagonal adjacency is included: a1 and b2 are adjacent.
//    Also note that a square is adjacent to itself.
  inline bool
square_Adjacent (squareT from, squareT to)
{
    ASSERT (from <= H8  &&  to <= H8);
    rankT fromRank = square_Rank(from);
    rankT toRank = square_Rank(to);
    int rdist = (int)fromRank - (int)toRank;
    if (rdist < -1  ||  rdist > 1) { return false; }
    fyleT fromFyle = square_Fyle(from);
    fyleT toFyle = square_Fyle(to);
    int fdist = (int)fromFyle - (int)toFyle;
    if (fdist < -1  ||  fdist > 1) { return false; }
    return true;
}

#endif  // #ifdef SCID_COMMON_H

//////////////////////////////////////////////////////////////////////
//  EOF:  common.h
//////////////////////////////////////////////////////////////////////

