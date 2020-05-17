/*
 * Copyright (C) 2000-2004  Shane Hudson..
 *
 * This file is part of Scid (Shane's Chess Information Database).
 *
 * Scid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * Scid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scid.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 * Constants and definitions of the chess board.
 */

#pragma once

typedef unsigned char pieceT;  // e.g ROOK or WK
typedef unsigned char colorT;  // WHITE or BLACK
typedef unsigned char squareT; // e.g. A3
typedef unsigned char rankT;   // Chess board rank
typedef unsigned char fyleT;   // Chess board file

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PIECES COLORS
const unsigned NUM_COLOR_TYPES = 2;
const colorT WHITE = 0, BLACK = 1, NOCOLOR = 2;
const char COLOR_CHAR[3] = {'W', 'B', '_'};

inline colorT color_Flip(colorT c) { return 1 - c; }

inline char color_Char(colorT c) { return COLOR_CHAR[c]; }

// PIECE TYPES (without color; same value as a white piece)
const pieceT INVALID_PIECE = 0, KING = 1, QUEEN = 2, ROOK = 3, BISHOP = 4,
             KNIGHT = 5, PAWN = 6;

// PIECES:
//   Note that color(x) == ((x & 0x8) >> 3)  and  type(x) == (x & 0x7)
//   EMPTY is deliberately nonzero, and END_OF_BOARD is zero, so that
//   a board can be used as a regular 0-terminated string, provided
//   that board[NULL_SQUARE] == END_OF_BOARD, as it always should be.
const pieceT EMPTY = 7;
const pieceT END_OF_BOARD = 0;
const pieceT WK = 1, WQ = 2, WR = 3, WB = 4, WN = 5, WP = 6;
const pieceT BK = 9, BQ = 10, BR = 11, BB = 12, BN = 13, BP = 14;

inline colorT piece_Color(pieceT p) {
	return (p == EMPTY) ? NOCOLOR : ((p & 8) >> 3);
}
// Slightly faster piece_Color when we are sure the piece is not empty:
inline colorT piece_Color_NotEmpty(pieceT p) { return (p & 8) >> 3; }

inline pieceT piece_Type(pieceT p) { return (p & 7); }

inline pieceT piece_Make(colorT c, pieceT p) { return ((c << 3) | (p & 7)); }

// PIECE_CHAR[]: array of piece characters, capitals for White pieces.
const char PIECE_CHAR[] = "xKQRBNP.xkqrbnpxMm";

inline char piece_Char(pieceT p) { return PIECE_CHAR[piece_Type(p)]; }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SQUARES
const squareT A1 = 0, B1 = 1, C1 = 2, D1 = 3, E1 = 4, F1 = 5, G1 = 6, H1 = 7,
              A2 = 8, B2 = 9, C2 = 10, D2 = 11, E2 = 12, F2 = 13, G2 = 14,
              H2 = 15, A3 = 16, B3 = 17, C3 = 18, D3 = 19, E3 = 20, F3 = 21,
              G3 = 22, H3 = 23, A4 = 24, B4 = 25, C4 = 26, D4 = 27, E4 = 28,
              F4 = 29, G4 = 30, H4 = 31, A5 = 32, B5 = 33, C5 = 34, D5 = 35,
              E5 = 36, F5 = 37, G5 = 38, H5 = 39, A6 = 40, B6 = 41, C6 = 42,
              D6 = 43, E6 = 44, F6 = 45, G6 = 46, H6 = 47, A7 = 48, B7 = 49,
              C7 = 50, D7 = 51, E7 = 52, F7 = 53, G7 = 54, H7 = 55, A8 = 56,
              B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63,
              COLOR_SQUARE = 64, NULL_SQUARE = 65,
              NS = 65; // NS is abbreviation for NULL_SQUARE.

const rankT RANK_1 = 0, RANK_2 = 1, RANK_3 = 2, RANK_4 = 3, RANK_5 = 4,
            RANK_6 = 5, RANK_7 = 6, RANK_8 = 7, NO_RANK = 64;

const fyleT
    // we use "fyle" instead of "file" to avoid confusion with disk files.
    A_FYLE = 0,
    B_FYLE = 1, C_FYLE = 2, D_FYLE = 3, E_FYLE = 4, F_FYLE = 5, G_FYLE = 6,
    H_FYLE = 7, NO_FYLE = 64;

inline rankT rank_FromChar(char c) {
	if (c < '1' || c > '8') {
		return NO_RANK;
	} else
		return (c - '1');
}

inline fyleT fyle_FromChar(char c) {
	if (c < 'a' || c > 'h') {
		return NO_FYLE;
	} else
		return (c - 'a');
}

inline squareT square_Make(fyleT f, rankT r) { return ((r << 3) | f); }

inline fyleT square_Fyle(squareT sq) { return (sq & 0x7); }

inline rankT square_Rank(squareT sq) { return ((sq >> 3) & 0x7); }

constexpr squareT square_Relative(colorT c, squareT sq) {
	return static_cast<squareT>(sq ^ (c * 56));
}

constexpr rankT rank_Relative(colorT c, rankT r) {
	return static_cast<rankT>(r ^ (c * 7));
}
