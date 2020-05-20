/*
* Copyright (C) 2013-2018  Fulvio Benini

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
 * Defines the classes used to search for positions.
 */

#ifndef SEARCHPOS_H
#define SEARCHPOS_H

#include "common.h"
#include "fastgame.h"
#include "matsig.h"
#include "position.h"
#include "scidbase.h"
#include "stored.h"
#include <algorithm>
#include <memory>

/// Return true if there is a piece's count in @e a which is less than its
/// counterpart in @e b.
/// @param promo: pawns' difference is considered when comparing queen.
/// @param upromo: minor pieces' count is not compared.
template <typename TMaterialCount>
bool less_mat(const TMaterialCount& a, matSigT b, bool promo, bool upromo) {
	int wp_diff = a.count(WHITE, PAWN) - static_cast<int>(MATSIG_Count_WP(b));
	int bp_diff = a.count(BLACK, PAWN) - static_cast<int>(MATSIG_Count_BP(b));
	if (wp_diff < 0 || bp_diff < 0)
		return true;

	int wq_diff = a.count(WHITE, QUEEN) - static_cast<int>(MATSIG_Count_WQ(b));
	int bq_diff = a.count(BLACK, QUEEN) - static_cast<int>(MATSIG_Count_BQ(b));
	if (promo) {
		wq_diff += wp_diff;
		bq_diff += bp_diff;
	}
	if (wq_diff < 0 || bq_diff < 0)
		return true;

	if (upromo)
		return false;

	return a.count(WHITE, ROOK) < static_cast<int>(MATSIG_Count_WR(b)) ||
	       a.count(WHITE, BISHOP) < static_cast<int>(MATSIG_Count_WB(b)) ||
	       a.count(WHITE, KNIGHT) < static_cast<int>(MATSIG_Count_WN(b)) ||
	       a.count(BLACK, ROOK) < static_cast<int>(MATSIG_Count_BR(b)) ||
	       a.count(BLACK, BISHOP) < static_cast<int>(MATSIG_Count_BB(b)) ||
	       a.count(BLACK, KNIGHT) < static_cast<int>(MATSIG_Count_BN(b));
}

/// Search for an exact position (same material in the same squares).
class SearchPos {
	MaterialCount nPieces_;
	pieceT board_[64];
	std::unique_ptr<StoredLine> storedLine_;
	std::pair<uint16_t, uint16_t> hpSig_;
	colorT toMove_;
	bool isStdStard_;

public:
	explicit SearchPos(const Position* pos) {
		std::copy_n(pos->GetBoard(), 64, board_);

		for (auto piece : board_) {
			if (piece != EMPTY) {
				nPieces_.incr(piece_Color_NotEmpty(piece), piece_Type(piece));
			}
		}

		hpSig_ = hpSig_make(board_);
		toMove_ = pos->GetToMove();
		isStdStard_ = pos->IsStdStart();

		if ((board_[E1] == WK || board_[G1] == WK) &&
		    (board_[E8] == BK || board_[G8] == BK)) {
			storedLine_ = std::make_unique<StoredLine>(board_, toMove_);
		}
	}

	/// Disable the stored lines optimization
	void disableOptStoredLine() { storedLine_ = nullptr; }

	/// Disable the home pawn optimization
	void disableOptHpSig() { hpSig_ = {0, 0}; }

	/// Search for the position using the optimizations in a game's index.
	/// @returns
	/// -2 : the game cannot reach the searched position
	/// -1 : the game can reach the searched position
	/// >=0: the game reach the searched position at the returned ply
	int index_match(const IndexEntry& ie) const {
		if (!ie.GetStartFlag()) {
			if (storedLine_) {
				int ply = storedLine_->match(ie.GetStoredLineCode());
				if (ply != -1)
					return ply;
			}
			if (!hpSig_match(hpSig_.first, hpSig_.second, ie.GetHomePawnData()))
				return -2;
		}
		if (less_mat(nPieces_, ie.GetFinalMatSig(), ie.GetPromotionsFlag(),
		             ie.GetUnderPromoFlag())) {
			return -2;
		}
		return -1;
	}

	/// Reset @e filter to include only the games that reached the searched
	/// position in their main line.
	bool setFilter(scidBaseT const& base, HFilter& filter,
	               const Progress& progress) const {
		if (toMove_ == BLACK)
			return SetFilter<BLACK>(base, filter, progress);

		if (!isStdStard_)
			return SetFilter<WHITE>(base, filter, progress);

		return setFilterStdStart(base, filter);
	}

private:
	bool setFilterStdStart(scidBaseT const& base, HFilter& filter) const {
		filter->includeAll();
		for (gamenumT i = 0, n = base.numGames(); i < n; i++) {
			const IndexEntry* ie = base.getIndexEntry(i);
			if (ie->GetStartFlag()) {
				int ply = base.getGame(ie).search<WHITE>(board_, nPieces_);
				filter.set(i, (ply > 255) ? 255 : ply);
			}
		}
		return true;
	}

	template <colorT TOMOVE>
	bool SetFilter(scidBaseT const& base, HFilter& filter,
	               const Progress& prg) const {
		filter->clear();
		long long progress = 0;
		for (gamenumT i = 0, n = base.numGames(); i < n; i++) {
			const IndexEntry* ie = base.getIndexEntry(i);
			int ply = index_match(*ie);
			if (ply >= 0) {
				filter.set(i, static_cast<byte>(ply + 1));
			} else if (ply == -1) {
				ply = base.getGame(ie).search<TOMOVE>(board_, nPieces_);
				if (ply != 0)
					filter.set(i, (ply > 255) ? 255 : ply);

				if ((progress++ % 256) == 0 && !prg.report(i, n))
					return false;
			}
		}
		return true;
	}
};

#endif
