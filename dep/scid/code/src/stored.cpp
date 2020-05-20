/*
* Copyright (C) 2014  Fulvio Benini

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

#include "stored.h"
#include <cstring>

// Stored line codes: used to speed up tree searches.

// The basic idea is: keep a fixed dictionary of popular opening lines where
// move order is significant (e.g. 1.d4 e6 2.e4 is NOT equal to 1.e4 e6 2.d4).
// For each game, the longest sequence of initial moves found in the
// dictionary is stored in the index file, as an integer value.

// When tree-searching for a position, each stored line is searched
// first and if an exact match is found for one (and it has a further
// move), all games with that stored line code can be found as matches
// without decoding.
// Furthermore, if we detect that a line cannot possibly lead to a match
// because of its end material value or pawn configuration, all games with
// that stored line code can be excluded and skipped without decoding.

namespace {
	
class Board {
	pieceT b_[64];

public:
	Board(const pieceT* b) {
		for (int i=0; i < 64; i++) b_[i] = b[i];
	}
	void doMove(FullMove m) {
		//No promo, no null moves, no queenside castle
		if (! m.isCastle()) {
			b_[m.getFrom()] = EMPTY;
			b_[m.getTo()] = piece_Make(m.getColor(), m.getPiece());
		} else {
			b_[m.getFrom()] = EMPTY;
			b_[m.getTo()] = EMPTY;
			int black = (m.getColor() == BLACK) ? 56 : 0;
			b_[black + G1] = piece_Make(m.getColor(), KING);
			b_[black + F1] = piece_Make(m.getColor(), ROOK);
		}
	}
	bool operator==(const Board& b) const {
		return (std::memcmp(b_, b.b_, sizeof b_) == 0);
	}
	bool neverMatch(const Board& m) const {
		// Pawns allows to exclude some games:
		int p[3][8] = {{0}};
		for (int i=0; i < 64; i++) {
			pieceT a = b_[i];
			pieceT b = m.b_[i];
			// 1) because a pawn will never go back to home position
			if (b == WP && a != WP && (i/8) == 1) return true;
			if (b == BP && a != BP && (i/8) == 6) return true;
			p[piece_Color(a)][piece_Type(a)] += 1;
			p[piece_Color(b)][piece_Type(b)] -= 1;
		}
		int df[2] = {0};
		for (pieceT i=QUEEN; i < PAWN; i++) {
			if (p[WHITE][i] < 0) df[WHITE] -= p[WHITE][i];
			if (p[BLACK][i] < 0) df[BLACK] -= p[BLACK][i];
		}
		// 2) because only extra pawns can be promoted to create other pieces.
		if (p[WHITE][PAWN] < df[WHITE] || p[BLACK][PAWN] < df[BLACK]) return true;
		return false;
	}
};

constexpr FullMove fm[] = {
 // "1.b3"
 0x6000251,
 // "1.c4"
 0x600029a,
 // "1.c4 c5"
 0x600029a, 0xe000ca2,
 // "1.c4 c5 2.Nf3"
 0x600029a, 0xe000ca2, 0x5000195,
 // "1.c4 e5"
 0x600029a, 0xe000d24,
 // "1.c4 e5 2.Nc3"
 0x600029a, 0xe000d24, 0x5000052,
 // "1.c4 e5 2.Nc3 Nf6"
 0x600029a, 0xe000d24, 0x5000052, 0xd000fad,
 // "1.c4 e6"
 0x600029a, 0xe000d2c,
 // "1.c4 e6 2.Nf3"
 0x600029a, 0xe000d2c, 0x5000195,
 // "1.c4 g6"
 0x600029a, 0xe000dae,
 // "1.c4 Nf6"
 0x600029a, 0xd000fad,
 // "1.c4 Nf6 2.Nc3"
 0x600029a, 0xd000fad, 0x5000052,
 // "1.c4 Nf6 2.Nc3 e6"
 0x600029a, 0xd000fad, 0x5000052, 0xe000d2c,
 // "1.c4 Nf6 2.Nc3 g6"
 0x600029a, 0xd000fad, 0x5000052, 0xe000dae,
 // "1.d4"
 0x60002db,
 // "1.d4 d5"
 0x60002db, 0xe000ce3,
 // "1.d4 d5 2.c4"
 0x60002db, 0xe000ce3, 0x600029a,
 // "1.d4 d5 2.c4 c6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa,
 // "1.d4 d5 2.c4 c6 3.Nc3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000052,
 // "1.d4 d5 2.c4 c6 3.Nc3 Nf6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000052, 0xd000fad,
 // "1.d4 d5 2.c4 c6 3.Nc3 Nf6 4.Nf3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000052, 0xd000fad, 0x5000195,
 // "1.d4 d5 2.c4 c6 3.Nf3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000195,
 // "1.d4 d5 2.c4 c6 3.Nf3 Nf6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000195, 0xd000fad,
 // "1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000195, 0xd000fad, 0x5000052,
 // "1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3 e6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000caa, 0x5000195, 0xd000fad, 0x5000052, 0xe000d2c,
 // "1.d4 d5 2.c4 dxc4"
 0x60002db, 0xe000ce3, 0x600029a, 0xec008da,
 // "1.d4 d5 2.c4 dxc4 3.Nf3"
 0x60002db, 0xe000ce3, 0x600029a, 0xec008da, 0x5000195,
 // "1.d4 d5 2.c4 dxc4 3.Nf3 Nf6"
 0x60002db, 0xe000ce3, 0x600029a, 0xec008da, 0x5000195, 0xd000fad,
 // "1.d4 d5 2.c4 e6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000d2c,
 // "1.d4 d5 2.c4 e6 3.Nc3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000d2c, 0x5000052,
 // "1.d4 d5 2.c4 e6 3.Nc3 c6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000d2c, 0x5000052, 0xe000caa,
 // "1.d4 d5 2.c4 e6 3.Nc3 Nf6"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000d2c, 0x5000052, 0xd000fad,
 // "1.d4 d5 2.c4 e6 3.Nf3"
 0x60002db, 0xe000ce3, 0x600029a, 0xe000d2c, 0x5000195,
 // "1.d4 d5 2.Nf3"
 0x60002db, 0xe000ce3, 0x5000195,
 // "1.d4 d5 2.Nf3 Nf6"
 0x60002db, 0xe000ce3, 0x5000195, 0xd000fad,
 // "1.d4 d5 2.Nf3 Nf6 3.c4"
 0x60002db, 0xe000ce3, 0x5000195, 0xd000fad, 0x600029a,
 // "1.d4 d5 2.Nf3 Nf6 3.c4 c6"
 0x60002db, 0xe000ce3, 0x5000195, 0xd000fad, 0x600029a, 0xe000caa,
 // "1.d4 d5 2.Nf3 Nf6 3.c4 e6"
 0x60002db, 0xe000ce3, 0x5000195, 0xd000fad, 0x600029a, 0xe000d2c,
 // "1.d4 d6"
 0x60002db, 0xe000ceb,
 // "1.d4 d6 2.Nf3"
 0x60002db, 0xe000ceb, 0x5000195,
 // "1.d4 e6"
 0x60002db, 0xe000d2c,
 // "1.d4 e6 2.c4"
 0x60002db, 0xe000d2c, 0x600029a,
 // "1.d4 e6 2.c4 Nf6"
 0x60002db, 0xe000d2c, 0x600029a, 0xd000fad,
 // "1.d4 f5"
 0x60002db, 0xe000d65,
 // "1.d4 f5 2.g3 Nf6 3.Bg2"
 0x60002db, 0xe000d65, 0x6000396, 0xd000fad, 0x400014e,
 // "1.d4 g6"
 0x60002db, 0xe000dae,
 // "1.d4 g6 2.c4 Bg7"
 0x60002db, 0xe000dae, 0x600029a, 0xc000f76,
 // "1.d4 Nf6"
 0x60002db, 0xd000fad,
 // "1.d4 Nf6 2.Bg5"
 0x60002db, 0xd000fad, 0x40000a6,
 // "1.d4 Nf6 2.Bg5 Ne4"
 0x60002db, 0xd000fad, 0x40000a6, 0xd000b5c,
 // "1.d4 Nf6 2.c4"
 0x60002db, 0xd000fad, 0x600029a,
 // "1.d4 Nf6 2.c4 c5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000ca2,
 // "1.d4 Nf6 2.c4 c5 3.d5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000ca2, 0x60006e3,
 // "1.d4 Nf6 2.c4 c5 3.d5 b5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000ca2, 0x60006e3, 0xe000c61,
 // "1.d4 Nf6 2.c4 c5 3.d5 b5 4.cxb5 a6"
 0x60002db, 0xd000fad, 0x600029a, 0xe000ca2, 0x60006e3, 0xe000c61, 0x6c006a1, 0xe000c28,
 // "1.d4 Nf6 2.c4 e6 3.g3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x6000396,
 // "1.d4 Nf6 2.c4 e6 3.g3 d5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x6000396, 0xe000ce3,
 // "1.d4 Nf6 2.c4 e6 3.Nc3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.e3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59, 0x6000314,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.e3 O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59, 0x6000314, 0x900cf3f,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.Qc2"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59, 0x20000ca,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.Qc2 O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59, 0x20000ca, 0x900cf3f,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.Qc2 O-O 5.a3 Bxc3+ 6.Qxc3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xc000f59, 0x20000ca, 0x900cf3f, 0x6000210, 0x4ca00652, 0x2800292,
 // "1.d4 Nf6 2.c4 e6 3.Nc3 d5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000052, 0xe000ce3,
 // "1.d4 Nf6 2.c4 e6 3.Nf3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 b6"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000c69,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 b6 4.a3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000c69, 0x6000210,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 b6 4.g3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000c69, 0x6000396,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 b6 4.g3 Ba6"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000c69, 0x6000396, 0xc000ea8,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 Bb4+"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0x4c000f59,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 d5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000ce3,
 // "1.d4 Nf6 2.c4 e6 3.Nf3 d5 4.Nc3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000d2c, 0x5000195, 0xe000ce3, 0x5000052,
 // "1.d4 Nf6 2.c4 g6"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Be2 O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x400014c, 0x900cf3f,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Be2 O-O 6.Nf3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x400014c, 0x900cf3f, 0x5000195,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.f3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x6000355,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.f3 O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x6000355, 0x900cf3f,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.f3 O-O 6.Be3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x6000355, 0x900cf3f, 0x4000094,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x5000195, 0x900cf3f,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O 6.Be2"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x5000195, 0x900cf3f, 0x400014c,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O 6.Be2 e5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x5000195, 0x900cf3f, 0x400014c, 0xe000d24,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O 6.Be2 e5 7.O-O"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x5000195, 0x900cf3f, 0x400014c, 0xe000d24, 0x100c107,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O 6.Be2 e5 7.O-O Nc6 8.d5 Ne7"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xc000f76, 0x600031c, 0xe000ceb, 0x5000195, 0x900cf3f, 0x400014c, 0xe000d24, 0x100c107, 0xd000e6a, 0x60006e3, 0xd000ab4,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 d5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xe000ce3,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.Nf3"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xe000ce3, 0x5000195,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.cxd5 Nxd5"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xe000ce3, 0x6c006a3, 0xdc00b63,
 // "1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.cxd5 Nxd5 5.e4 Nxc3 6.bxc3 Bg7"
 0x60002db, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052, 0xe000ce3, 0x6c006a3, 0xdc00b63, 0x600031c, 0xda008d2, 0x6a00252, 0xc000f76,
 // "1.d4 Nf6 2.Nf3"
 0x60002db, 0xd000fad, 0x5000195,
 // "1.d4 Nf6 2.Nf3 c5"
 0x60002db, 0xd000fad, 0x5000195, 0xe000ca2,
 // "1.d4 Nf6 2.Nf3 d5"
 0x60002db, 0xd000fad, 0x5000195, 0xe000ce3,
 // "1.d4 Nf6 2.Nf3 e6"
 0x60002db, 0xd000fad, 0x5000195, 0xe000d2c,
 // "1.d4 Nf6 2.Nf3 e6 3.Bg5"
 0x60002db, 0xd000fad, 0x5000195, 0xe000d2c, 0x40000a6,
 // "1.d4 Nf6 2.Nf3 e6 3.c4"
 0x60002db, 0xd000fad, 0x5000195, 0xe000d2c, 0x600029a,
 // "1.d4 Nf6 2.Nf3 g6"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae,
 // "1.d4 Nf6 2.Nf3 g6 3.Bg5"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x40000a6,
 // "1.d4 Nf6 2.Nf3 g6 3.c4"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x600029a,
 // "1.d4 Nf6 2.Nf3 g6 3.c4 Bg7"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x600029a, 0xc000f76,
 // "1.d4 Nf6 2.Nf3 g6 3.c4 Bg7 4.Nc3"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x600029a, 0xc000f76, 0x5000052,
 // "1.d4 Nf6 2.Nf3 g6 3.c4 Bg7 4.Nc3 O-O"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x600029a, 0xc000f76, 0x5000052, 0x900cf3f,
 // "1.d4 Nf6 2.Nf3 g6 3.g3"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x6000396,
 // "1.d4 Nf6 2.Nf3 g6 3.g3 Bg7 4.Bg2"
 0x60002db, 0xd000fad, 0x5000195, 0xe000dae, 0x6000396, 0xc000f76, 0x400014e,
 // "1.e4"
 0x600031c,
 // "1.e4 c5"
 0x600031c, 0xe000ca2,
 // "1.e4 c5 2.c3"
 0x600031c, 0xe000ca2, 0x6000292,
 // "1.e4 c5 2.c3 d5 3.exd5 Qxd5 4.d4"
 0x600031c, 0xe000ca2, 0x6000292, 0xe000ce3, 0x6c00723, 0xac00ee3, 0x60002db,
 // "1.e4 c5 2.c3 d5 3.exd5 Qxd5 4.d4 Nf6"
 0x600031c, 0xe000ca2, 0x6000292, 0xe000ce3, 0x6c00723, 0xac00ee3, 0x60002db, 0xd000fad,
 // "1.e4 c5 2.c3 Nf6 3.e5 Nd5"
 0x600031c, 0xe000ca2, 0x6000292, 0xd000fad, 0x6000724, 0xd000b63,
 // "1.e4 c5 2.c3 Nf6 3.e5 Nd5 4.d4 cxd4"
 0x600031c, 0xe000ca2, 0x6000292, 0xd000fad, 0x6000724, 0xd000b63, 0x60002db, 0xec0089b,
 // "1.e4 c5 2.d4 cxd4"
 0x600031c, 0xe000ca2, 0x60002db, 0xec0089b,
 // "1.e4 c5 2.Nc3"
 0x600031c, 0xe000ca2, 0x5000052,
 // "1.e4 c5 2.Nc3 Nc6"
 0x600031c, 0xe000ca2, 0x5000052, 0xd000e6a,
 // "1.e4 c5 2.Nc3 Nc6 3.g3"
 0x600031c, 0xe000ca2, 0x5000052, 0xd000e6a, 0x6000396,
 // "1.e4 c5 2.Nc3 Nc6 3.g3 g6"
 0x600031c, 0xe000ca2, 0x5000052, 0xd000e6a, 0x6000396, 0xe000dae,
 // "1.e4 c5 2.Nc3 Nc6 3.g3 g6 4.Bg2 Bg7"
 0x600031c, 0xe000ca2, 0x5000052, 0xd000e6a, 0x6000396, 0xe000dae, 0x400014e, 0xc000f76,
 // "1.e4 c5 2.Nc3 Nc6 3.g3 g6 4.Bg2 Bg7 5.d3"
 0x600031c, 0xe000ca2, 0x5000052, 0xd000e6a, 0x6000396, 0xe000dae, 0x400014e, 0xc000f76, 0x60002d3,
 // "1.e4 c5 2.Nf3"
 0x600031c, 0xe000ca2, 0x5000195,
 // "1.e4 c5 2.Nf3 d6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb,
 // "1.e4 c5 2.Nf3 d6 3.Bb5+"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x44000161,
 // "1.e4 c5 2.Nf3 d6 3.d4"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bc4"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28, 0x400015a,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Be2"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28, 0x400014c,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Be3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28, 0x4000094,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28, 0x40000a6,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6 6.Bg5 e6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000c28, 0x40000a6, 0xe000d2c,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000dae,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6 6.Be3 Bg7 7.f3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000dae, 0x4000094, 0xc000f76, 0x6000355,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6 6.Be3 Bg7 7.f3 O-O"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000dae, 0x4000094, 0xc000f76, 0x6000355, 0x900cf3f,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xd000e6a,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6 6.Bg5"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xd000e6a, 0x40000a6,
 // "1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6 6.Bg5 e6 7.Qd2"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000ceb, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xd000e6a, 0x40000a6, 0xe000d2c, 0x20000cb,
 // "1.e4 c5 2.Nf3 e6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c,
 // "1.e4 c5 2.Nf3 e6 3.d3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002d3,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 a6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xe000c28,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 a6 5.Bd3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xe000c28, 0x4000153,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nc6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000e6a,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nc6 5.Nc3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000e6a, 0x5000052,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nc6 5.Nc3 Qc7"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000e6a, 0x5000052, 0xa000ef2,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nf6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052,
 // "1.e4 c5 2.Nf3 e6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 d6"
 0x600031c, 0xe000ca2, 0x5000195, 0xe000d2c, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000ceb,
 // "1.e4 c5 2.Nf3 Nc6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a,
 // "1.e4 c5 2.Nf3 Nc6 3.Bb5"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x4000161,
 // "1.e4 c5 2.Nf3 Nc6 3.Bb5 g6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x4000161, 0xe000dae,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 e5"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xe000d24,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 g6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xe000dae,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 d6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000ceb,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 d6 6.Bg5"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000ceb, 0x40000a6,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e5"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000d24,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e5 6.Ndb5 d6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000d24, 0x150006e1, 0xe000ceb,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e5 6.Ndb5 d6 7.Bg5 a6"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000d24, 0x150006e1, 0xe000ceb, 0x40000a6, 0xe000c28,
 // "1.e4 c5 2.Nf3 Nc6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e5 6.Ndb5 d6 7.Bg5 a6 8.Na3 b5"
 0x600031c, 0xe000ca2, 0x5000195, 0xd000e6a, 0x60002db, 0xec0089b, 0x5c0055b, 0xd000fad, 0x5000052, 0xe000d24, 0x150006e1, 0xe000ceb, 0x40000a6, 0xe000c28, 0x5000850, 0xe000c61,
 // "1.e4 c6"
 0x600031c, 0xe000caa,
 // "1.e4 c6 2.d4 d5"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3,
 // "1.e4 c6 2.d4 d5 3.e5"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x6000724,
 // "1.e4 c6 2.d4 d5 3.e5 Bf5"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x6000724, 0xc000ea5,
 // "1.e4 c6 2.d4 d5 3.exd5 cxd5"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x6c00723, 0xec00aa3,
 // "1.e4 c6 2.d4 d5 3.exd5 cxd5 4.c4 Nf6 5.Nc3"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x6c00723, 0xec00aa3, 0x600029a, 0xd000fad, 0x5000052,
 // "1.e4 c6 2.d4 d5 3.Nc3"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x5000052,
 // "1.e4 c6 2.d4 d5 3.Nc3 dxe4 4.Nxe4"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x5000052, 0xec008dc, 0x5c0049c,
 // "1.e4 c6 2.d4 d5 3.Nd2 dxe4 4.Nxe4"
 0x600031c, 0xe000caa, 0x60002db, 0xe000ce3, 0x500004b, 0xec008dc, 0x5c002dc,
 // "1.e4 d5 2.exd5 Nf6"
 0x600031c, 0xe000ce3, 0x6c00723, 0xd000fad,
 // "1.e4 d5 2.exd5 Qxd5"
 0x600031c, 0xe000ce3, 0x6c00723, 0xac00ee3,
 // "1.e4 d5 2.exd5 Qxd5 3.Nc3"
 0x600031c, 0xe000ce3, 0x6c00723, 0xac00ee3, 0x5000052,
 // "1.e4 d5 2.exd5 Qxd5 3.Nc3 Qa5"
 0x600031c, 0xe000ce3, 0x6c00723, 0xac00ee3, 0x5000052, 0xa0008e0,
 // "1.e4 d6"
 0x600031c, 0xe000ceb,
 // "1.e4 d6 2.d4"
 0x600031c, 0xe000ceb, 0x60002db,
 // "1.e4 d6 2.d4 Nf6"
 0x600031c, 0xe000ceb, 0x60002db, 0xd000fad,
 // "1.e4 d6 2.d4 Nf6 3.Nc3"
 0x600031c, 0xe000ceb, 0x60002db, 0xd000fad, 0x5000052,
 // "1.e4 d6 2.d4 Nf6 3.Nc3 g6"
 0x600031c, 0xe000ceb, 0x60002db, 0xd000fad, 0x5000052, 0xe000dae,
 // "1.e4 d6 2.d4 Nf6 3.Nc3 g6 4.f4 Bg7 5.Nf3"
 0x600031c, 0xe000ceb, 0x60002db, 0xd000fad, 0x5000052, 0xe000dae, 0x600035d, 0xc000f76, 0x5000195,
 // "1.e4 d6 2.d4 Nf6 3.Nc3 g6 4.Nf3 Bg7"
 0x600031c, 0xe000ceb, 0x60002db, 0xd000fad, 0x5000052, 0xe000dae, 0x5000195, 0xc000f76,
 // "1.e4 e5"
 0x600031c, 0xe000d24,
 // "1.e4 e5 2.f4"
 0x600031c, 0xe000d24, 0x600035d,
 // "1.e4 e5 2.Nc3"
 0x600031c, 0xe000d24, 0x5000052,
 // "1.e4 e5 2.Nf3"
 0x600031c, 0xe000d24, 0x5000195,
 // "1.e4 e5 2.Nf3 Nc6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O b5 6.Bb3"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xe000c61, 0x4000611,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74, 0x3000144, 0xe000c61, 0x4000611, 0xe000ceb,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6 8.c3 O-O"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74, 0x3000144, 0xe000c61, 0x4000611, 0xe000ceb, 0x6000292, 0x900cf3f,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6 8.c3 O-O 9.h3"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74, 0x3000144, 0xe000c61, 0x4000611, 0xe000ceb, 0x6000292, 0x900cf3f, 0x60003d7,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 d6 8.c3 O-O 9.h3 Na5 10.Bc2 c5 11.d4"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74, 0x3000144, 0xe000c61, 0x4000611, 0xe000ceb, 0x6000292, 0x900cf3f, 0x60003d7, 0xd000aa0, 0x400044a, 0xe000ca2, 0x60002db,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7 6.Re1 b5 7.Bb3 O-O"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xe000c28, 0x4000858, 0xd000fad, 0x100c107, 0xc000f74, 0x3000144, 0xe000c61, 0x4000611, 0x900cf3f,
 // "1.e4 e5 2.Nf3 Nc6 3.Bb5 Nf6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x4000161, 0xd000fad,
 // "1.e4 e5 2.Nf3 Nc6 3.Bc4"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x400015a,
 // "1.e4 e5 2.Nf3 Nc6 3.Bc4 Nf6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x400015a, 0xd000fad,
 // "1.e4 e5 2.Nf3 Nc6 3.d4 exd4"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x60002db, 0xec0091b,
 // "1.e4 e5 2.Nf3 Nc6 3.d4 exd4 4.Nxd4"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x60002db, 0xec0091b, 0x5c0055b,
 // "1.e4 e5 2.Nf3 Nc6 3.Nc3"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x5000052,
 // "1.e4 e5 2.Nf3 Nc6 3.Nc3 Nf6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000e6a, 0x5000052, 0xd000fad,
 // "1.e4 e5 2.Nf3 Nf6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000fad,
 // "1.e4 e5 2.Nf3 Nf6 3.Nxe5 d6"
 0x600031c, 0xe000d24, 0x5000195, 0xd000fad, 0x5c00564, 0xe000ceb,
 // "1.e4 e6"
 0x600031c, 0xe000d2c,
 // "1.e4 e6 2.d3"
 0x600031c, 0xe000d2c, 0x60002d3,
 // "1.e4 e6 2.d4"
 0x600031c, 0xe000d2c, 0x60002db,
 // "1.e4 e6 2.d4 d5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3,
 // "1.e4 e6 2.d4 d5 3.e5 c5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x6000724, 0xe000ca2,
 // "1.e4 e6 2.d4 d5 3.e5 c5 4.c3"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x6000724, 0xe000ca2, 0x6000292,
 // "1.e4 e6 2.d4 d5 3.Nc3"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052,
 // "1.e4 e6 2.d4 d5 3.Nc3 Bb4"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xc000f59,
 // "1.e4 e6 2.d4 d5 3.Nc3 Bb4 4.e5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xc000f59, 0x6000724,
 // "1.e4 e6 2.d4 d5 3.Nc3 Bb4 4.e5 c5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xc000f59, 0x6000724, 0xe000ca2,
 // "1.e4 e6 2.d4 d5 3.Nc3 Bb4 4.e5 c5 5.a3 Bxc3+ 6.bxc3"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xc000f59, 0x6000724, 0xe000ca2, 0x6000210, 0x4ca00652, 0x6800252,
 // "1.e4 e6 2.d4 d5 3.Nc3 Nf6"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xd000fad,
 // "1.e4 e6 2.d4 d5 3.Nc3 Nf6 4.Bg5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x5000052, 0xd000fad, 0x40000a6,
 // "1.e4 e6 2.d4 d5 3.Nd2"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x500004b,
 // "1.e4 e6 2.d4 d5 3.Nd2 c5"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x500004b, 0xe000ca2,
 // "1.e4 e6 2.d4 d5 3.Nd2 Nf6"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x500004b, 0xd000fad,
 // "1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x500004b, 0xd000fad, 0x6000724, 0x1d000b73,
 // "1.e4 e6 2.d4 d5 3.Nd2 Nf6 4.e5 Nfd7 5.Bd3 c5 6.c3 Nc6 7.Ne2"
 0x600031c, 0xe000d2c, 0x60002db, 0xe000ce3, 0x500004b, 0xd000fad, 0x6000724, 0x1d000b73, 0x4000153, 0xe000ca2, 0x6000292, 0xd000e6a, 0x500018c,
 // "1.e4 g6"
 0x600031c, 0xe000dae,
 // "1.e4 g6 2.d4"
 0x600031c, 0xe000dae, 0x60002db,
 // "1.e4 g6 2.d4 Bg7"
 0x600031c, 0xe000dae, 0x60002db, 0xc000f76,
 // "1.e4 g6 2.d4 Bg7 3.Nc3"
 0x600031c, 0xe000dae, 0x60002db, 0xc000f76, 0x5000052,
 // "1.e4 g6 2.d4 Bg7 3.Nc3 d6"
 0x600031c, 0xe000dae, 0x60002db, 0xc000f76, 0x5000052, 0xe000ceb,
 // "1.e4 Nf6"
 0x600031c, 0xd000fad,
 // "1.e4 Nf6 2.e5 Nd5"
 0x600031c, 0xd000fad, 0x6000724, 0xd000b63,
 // "1.e4 Nf6 2.e5 Nd5 3.d4 d6"
 0x600031c, 0xd000fad, 0x6000724, 0xd000b63, 0x60002db, 0xe000ceb,
 // "1.e4 Nf6 2.e5 Nd5 3.d4 d6 4.Nf3"
 0x600031c, 0xd000fad, 0x6000724, 0xd000b63, 0x60002db, 0xe000ceb, 0x5000195,
 // "1.f4"
 0x600035d,
 // "1.g3"
 0x6000396,
 // "1.Nf3"
 0x5000195,
 // "1.Nf3 c5"
 0x5000195, 0xe000ca2,
 // "1.Nf3 c5 2.c4"
 0x5000195, 0xe000ca2, 0x600029a,
 // "1.Nf3 d5"
 0x5000195, 0xe000ce3,
 // "1.Nf3 d5 2.c4"
 0x5000195, 0xe000ce3, 0x600029a,
 // "1.Nf3 d5 2.d4"
 0x5000195, 0xe000ce3, 0x60002db,
 // "1.Nf3 d5 2.g3"
 0x5000195, 0xe000ce3, 0x6000396,
 // "1.Nf3 g6"
 0x5000195, 0xe000dae,
 // "1.Nf3 Nf6"
 0x5000195, 0xd000fad,
 // "1.Nf3 Nf6 2.c4"
 0x5000195, 0xd000fad, 0x600029a,
 // "1.Nf3 Nf6 2.c4 c5"
 0x5000195, 0xd000fad, 0x600029a, 0xe000ca2,
 // "1.Nf3 Nf6 2.c4 e6"
 0x5000195, 0xd000fad, 0x600029a, 0xe000d2c,
 // "1.Nf3 Nf6 2.c4 g6"
 0x5000195, 0xd000fad, 0x600029a, 0xe000dae,
 // "1.Nf3 Nf6 2.c4 g6 3.Nc3"
 0x5000195, 0xd000fad, 0x600029a, 0xe000dae, 0x5000052,
 // "1.Nf3 Nf6 2.g3"
 0x5000195, 0xd000fad, 0x6000396,
 // "1.Nf3 Nf6 2.g3 g6"
 0x5000195, 0xd000fad, 0x6000396, 0xe000dae
};
} // End of anonymous namespace

const FullMove* StoredLine::Moves_ [STORED_LINES +1] = {
// index zero is unused
// last index ( Moves_[STORED_LINES] ) is used to detect the end of the array
fm +   0, fm +   0, fm +   1, fm +   2, fm +   4, fm +   7, fm +   9, fm +  12,
fm +  16, fm +  18, fm +  21, fm +  23, fm +  25, fm +  28, fm +  32, fm +  36,
fm +  37, fm +  39, fm +  42, fm +  46, fm +  51, fm +  57, fm +  64, fm +  69,
fm +  75, fm +  82, fm +  90, fm +  94, fm +  99, fm + 105, fm + 109, fm + 114,
fm + 120, fm + 126, fm + 131, fm + 134, fm + 138, fm + 143, fm + 149, fm + 155,
fm + 157, fm + 160, fm + 162, fm + 165, fm + 169, fm + 171, fm + 176, fm + 178,
fm + 182, fm + 184, fm + 187, fm + 191, fm + 194, fm + 198, fm + 203, fm + 209,
fm + 217, fm + 222, fm + 228, fm + 233, fm + 239, fm + 246, fm + 254, fm + 261,
fm + 269, fm + 280, fm + 286, fm + 291, fm + 297, fm + 304, fm + 311, fm + 319,
fm + 325, fm + 331, fm + 338, fm + 342, fm + 348, fm + 355, fm + 363, fm + 373,
fm + 384, fm + 393, fm + 403, fm + 414, fm + 424, fm + 435, fm + 447, fm + 460,
fm + 476, fm + 482, fm + 489, fm + 497, fm + 509, fm + 512, fm + 516, fm + 520,
fm + 524, fm + 529, fm + 534, fm + 538, fm + 543, fm + 548, fm + 554, fm + 561,
fm + 569, fm + 574, fm + 581, fm + 582, fm + 584, fm + 587, fm + 594, fm + 602,
fm + 608, fm + 616, fm + 620, fm + 623, fm + 627, fm + 632, fm + 638, fm + 646,
fm + 655, fm + 658, fm + 662, fm + 667, fm + 672, fm + 678, fm + 685, fm + 693,
fm + 702, fm + 712, fm + 723, fm + 734, fm + 745, fm + 756, fm + 768, fm + 778,
fm + 791, fm + 805, fm + 815, fm + 826, fm + 839, fm + 843, fm + 848, fm + 855,
fm + 863, fm + 872, fm + 880, fm + 889, fm + 899, fm + 907, fm + 916, fm + 926,
fm + 930, fm + 935, fm + 941, fm + 948, fm + 956, fm + 964, fm + 973, fm + 983,
fm + 994, fm +1004, fm +1016, fm +1030, fm +1046, fm +1048, fm +1052, fm +1057,
fm +1063, fm +1069, fm +1078, fm +1083, fm +1090, fm +1097, fm +1101, fm +1105,
fm +1110, fm +1116, fm +1118, fm +1121, fm +1125, fm +1130, fm +1136, fm +1145,
fm +1153, fm +1155, fm +1158, fm +1161, fm +1164, fm +1168, fm +1173, fm +1179,
fm +1186, fm +1194, fm +1203, fm +1214, fm +1224, fm +1238, fm +1254, fm +1271,
fm +1292, fm +1306, fm +1312, fm +1317, fm +1323, fm +1329, fm +1336, fm +1341,
fm +1347, fm +1351, fm +1357, fm +1359, fm +1362, fm +1365, fm +1369, fm +1375,
fm +1382, fm +1387, fm +1393, fm +1400, fm +1408, fm +1419, fm +1425, fm +1432,
fm +1437, fm +1443, fm +1449, fm +1457, fm +1470, fm +1472, fm +1475, fm +1479,
fm +1484, fm +1490, fm +1492, fm +1496, fm +1502, fm +1509, fm +1510, fm +1511,
fm +1512, fm +1514, fm +1517, fm +1519, fm +1522, fm +1525, fm +1528, fm +1530,
fm +1532, fm +1535, fm +1539, fm +1543, fm +1547, fm +1552, fm +1555, fm +1559
};

StoredLine::StoredLine(const pieceT* board, colorT toMove)
{
	Board search(board);
	matches_[0] = -1;
	matches_[STORED_LINES] = -1;
	for (int line = 1; line < STORED_LINES; line++) {
		Board b(START_BOARD);
		const FullMove* end = Moves_[line +1];
		for (int ply=0; ply < 99; ply++) {
			if (((ply %2) == toMove) && b == search) {
				matches_[line] = ply;
				break;
			}
			const FullMove* m = Moves_[line] + ply;
			if (m == end) {
				matches_[line] = b.neverMatch(search) ? -2 : -1;
				break;
			}
			b.doMove(*m);
		}
	}
}

