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

#ifndef SCID_STORED_H
#define SCID_STORED_H

#include "fullmove.h"

class StoredLine {
	static constexpr int STORED_LINES = 255;
	static const FullMove* Moves_[STORED_LINES + 1];

	int8_t matches_[STORED_LINES + 1];

public:
	StoredLine(const pieceT* board, colorT toMove);

	// Result:
	//-2 : the game cannot reach the searched position
	//-1 : the game can reach the searched position
	//>=0: the game reach the searched position at the returned ply
	int match(byte code) const { return matches_[code]; }

	template <typename CompareOp> static byte classify(CompareOp comp) {
		int res = 0;
		std::ptrdiff_t longest = 0;
		for (int i = 1; i < STORED_LINES; ++i) {
			const auto begin = Moves_[i];
			const auto end = Moves_[i + 1];
			if (std::distance(begin, end) > longest && comp(begin, end)) {
				res = i;
				longest = std::distance(begin, end);
			}
		}
		return static_cast<byte>(res);
	}

	static FullMove getMove (byte code, uint ply = 0) {
		if ((code < STORED_LINES) && (Moves_[code] + ply) < Moves_[code +1]) {
			return Moves_[code][ply];
		}
		return FullMove();
	}
};

#endif  // #ifndef SCID_STORED_H

//////////////////////////////////////////////////////////////////////
//  EOF:    stored.h
//////////////////////////////////////////////////////////////////////
