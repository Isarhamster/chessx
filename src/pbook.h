/*
 * Copyright (C) 1999-2000  Shane Hudson
 * Copyright (C) 2017  Fulvio Benini

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
 *
 */

#ifndef SCID_PBOOK_H
#define SCID_PBOOK_H

#include "common.h"
#include "misc.h"
#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
class Position;

/**
 * A PBook is a collection of chess positions, each with the corresponding ECO
 * code, a mnemonic name, and the list of moves to reach the position.
 */
class PBook {
	struct bookDataT {
		std::unique_ptr<char[]> compactStr;
		std::unique_ptr<char[]> comment;

		bookDataT(char* compact, char* comm)
		    : compactStr(compact), comment(comm) {}
	};
	std::unordered_multimap<unsigned, bookDataT> pos_;
	std::vector<const char*> comments_;
	unsigned LineCount = 0;
	unsigned LeastMaterial = 32; // The smallest amount of material in any
	                             // position in the book. In the range 0..32.
public:
	/**
	 * Read a file with a list of ECO codes and creates a PBook object.
	 * The file is composed of lines like this:
	 * C50a "Italian Game"  1.e4 e5 2.Nf3 Nc6 3.Bc4 *
	 * @param FileName: the name of the file to be read.
	 * @returns
	 * - on success, a @e std::pair containing OK and the pointer to the newly
	 *   created object.
	 * - on failure, a @e std::pair containing an error code and nullptr.
	 */
	static std::pair<errorT, std::unique_ptr<PBook> >
	ReadEcoFile(const char* FileName);

	/**
	 * Retrieve an ECO string containing the ECO code and the mnemonic name.
	 * @param pos: the position to search for.
	 * @returns an empty string_view if the position is not found
	 */
	std::string_view findECOstr(Position* pos) const;

	/**
	 * Retrieve the ECO code of a position.
	 * @param pos: the position to search for.
	 * @returns the corresponding ECO code or ECO_None if not found.
	 */
	ecoT findECO(Position* pos) const {
		auto it = findECOstr(pos);
		if (it.empty())
			return ECO_None;

		char buf[8] = {0};
		it.copy(buf, 6);
		return eco_FromString(buf);
	}

	std::string EcoSummary(std::string_view ecoPrefix) const;

	unsigned GetLineNumber() const { return LineCount; }
	unsigned FewestPieces() const { return LeastMaterial; }
	size_t Size() const { return pos_.size(); }
};

#endif // SCID_PBOOK_H
