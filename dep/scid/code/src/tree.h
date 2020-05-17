/*
* Copyright (C) 1999 Shane Hudson
* Copyright (C) 2015-2020 Fulvio Benini

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

#pragma once

#include "fullmove.h"
#include "hfilter.h"
#include "position.h"
#include <algorithm>
#include <vector>

struct TreeNode {
	unsigned long long eloWhiteSum = 0;   // Sum of white Elos.
	unsigned long long eloBlackSum = 0;   // Sum of bLack Elos.
	unsigned long long yearSum = 0;       // Sum of years.
	gamenumT freq[NUM_RESULT_TYPES] = {}; // freq[0] is the total count.
	gamenumT eloCount = 0;                // Count of games with an Elo.
	gamenumT yearCount = 0;               // Count of games with year != 0.
	FullMove move;

public:
	explicit TreeNode(FullMove m) : move(m) {}

	void add(resultT result, int eloW, int eloB, unsigned year) {
		static_assert(RESULT_None == 0);
		freq[0]++; // total count of games
		if (result != RESULT_None) {
			freq[result]++;
		}
		if (eloW > 0 && eloB > 0) {
			++eloCount;
			eloWhiteSum += eloW;
			eloBlackSum += eloB;
		}
		if (year > 0) {
			yearSum += year;
			++yearCount;
		}
	}

	/// @return a value in the range [0, 1000] representing the score percentage
	/// from the white prospective (999 = white won 99.9% of the games).
	int score() const {
		auto n = freq[RESULT_White] + freq[RESULT_Draw] + freq[RESULT_Black];
		auto res = 1000ull * freq[RESULT_White] + 500ull * freq[RESULT_Draw];
		return n ? static_cast<int>(res / n) : 500;
	}

	double eloPerformance() const {
		if (eloCount == 0)
			return 0;

		int score = (this->score() + 5) / 10;
		auto eloOpp = eloBlackSum;
		if (move.getColor() != WHITE) {
			score = 100 - score;
			eloOpp = eloWhiteSum;
		}
		return 1.0 * eloOpp / eloCount + FIDE_ratingTable[score];
	}

	double avgElo() const {
		if (eloCount == 0)
			return 0;

		auto elo = (move.getColor() == WHITE) ? eloWhiteSum : eloBlackSum;
		return 1.0 * elo / eloCount;
	}

	double avgYear() const { return yearCount ? 1.0 * yearSum / yearCount : 0; }

	double percDraws() const {
		return freq[0] ? 100.0 * freq[RESULT_Draw] / freq[0] : 0;
	}

	static auto cmp_ngames_desc() {
		return
		    [](auto const& a, auto const& b) { return a.freq[0] > b.freq[0]; };
	}

private:
	// FIDE table 8.1a of conversion from fractional score (from 0 to 1 with
	// 0.01 increments) into rating differences.
	static constexpr short FIDE_ratingTable[] = {
	    -800, -677, -589, -538, -501, -470, -444, -422, -401, -383, -366, -351,
	    -336, -322, -309, -296, -284, -273, -262, -251, -240, -230, -220, -211,
	    -202, -193, -184, -175, -166, -158, -149, -141, -133, -125, -117, -110,
	    -102, -95,  -87,  -80,  -72,  -65,  -57,  -50,  -43,  -36,  -29,  -21,
	    -14,  -7,   0,    7,    14,   21,   29,   36,   43,   50,   57,   65,
	    72,   80,   87,   95,   102,  110,  117,  125,  133,  141,  149,  158,
	    166,  175,  184,  193,  202,  211,  220,  230,  240,  251,  262,  273,
	    284,  296,  309,  322,  336,  351,  366,  383,  401,  422,  444,  470,
	    501,  538,  589,  677,  800};
};

//////////////////////////////////////////////////////////////////////
// CompressedFilter class:
//    Holds the same data as a filter, in compressed format.
//    Random access to individual values is not possible.
//    A CompressedFilter is created from, or restored to, a regular
//    filter with the methods CompressFrom() and UncompressTo().
class CompressedFilter {
	byte* CompressedData = nullptr;
	gamenumT CFilterSize = 0;
	gamenumT CompressedLength = 0;

public:
	CompressedFilter() = default;
	CompressedFilter(CompressedFilter&&) = default;
	~CompressedFilter() { delete[] CompressedData; }

	void CompressFrom(Filter* filter);
	errorT UncompressTo(Filter* filter) const;

private:
	errorT Verify(Filter* filter);
};

struct CachedFilter {
	CompressedFilter cfilter_;
	pieceT board_[64];
	colorT toMove_;
};

class TreeCache {
	std::vector<CachedFilter> cache_;
	std::vector<uint32_t> cacheTime_;
	uint32_t cacheTimeCounter_ = 0;

public:
	void Clear() {
		cache_.clear();
		cacheTime_.clear();
	}

	size_t Size() const { return cache_.capacity(); }

	void CacheResize(size_t max_size) {
		Clear();
		cache_.reserve(max_size);
		cacheTime_.reserve(max_size);
	}

	template <typename PosT> void cacheAdd(PosT const& pos, Filter& filter) {
		size_t idx;
		if (cache_.size() < Size() || cache_.empty()) {
			idx = cache_.size();
			cache_.emplace_back();
			cacheTime_.emplace_back();
		} else {
			auto it = std::min_element(cacheTime_.begin(), cacheTime_.end());
			idx = std::distance(cacheTime_.begin(), it);
		}
		auto board = pos.GetBoard();
		std::copy(board, board + 64, cache_[idx].board_);
		cache_[idx].toMove_ = pos.GetToMove();
		cache_[idx].cfilter_.CompressFrom(&filter);
		cacheTime_[idx] = cacheTimeCounter_++;
	}

	template <typename PosT>
	bool cacheRestore(PosT const& pos, Filter& filter) {
		auto it = std::find_if(
		    cache_.begin(), cache_.end(), [&pos](auto const& e) {
			    return e.toMove_ == pos.GetToMove() &&
			           std::equal(e.board_, e.board_ + 64, pos.GetBoard());
		    });
		if (it == cache_.end())
			return false;

		auto idx = std::distance(cache_.begin(), it);
		if (it->cfilter_.UncompressTo(&filter) != OK) {
			ASSERT(false); // corrupted data: should not happen
			return false;
		}
		cacheTime_[idx] = cacheTimeCounter_++;
		return true;
	}
};
