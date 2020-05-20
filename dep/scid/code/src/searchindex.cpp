/*
# Copyright (C) 2015 Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid. If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "misc.h"
#include "scidbase.h"
#include <algorithm>
#include <string>
#include <vector>

namespace {

class SearchName {
	const scidBaseT* base_;
	idNumberT (IndexEntry::* f1_) () const;
	idNumberT (IndexEntry::* f2_) () const;
	std::vector<bool> mask_;

public:
	SearchName(const scidBaseT* base,
	           std::string pattern,
	           nameT name_type,
	           idNumberT (IndexEntry::* f1) () const,
	           idNumberT (IndexEntry::* f2) () const = 0)
	: base_(base), f1_(f1), f2_(f2) {
		idNumberT n = base->getNameBase()->GetNumNames(name_type);
		mask_.resize(n);

		size_t l = pattern.length();
		bool exact = (l > 2 && pattern[l -1] == '"' && pattern[0] == '"');
		if (exact) {
			Init_exact(n, name_type, pattern.substr(1, l -2));
		} else {
			Init_icase_ignoreSpaces(n, name_type, pattern.c_str());
		}
	}

	void Init_exact (idNumberT n, nameT name_type, std::string pattern) {
		const NameBase* nb = base_->getNameBase();
		for (idNumberT i=0; i < n; i++) {
			const char* name = nb->GetName (name_type, i);
			mask_[i] = (name == pattern);
		}
	}

	void Init_icase_ignoreSpaces(idNumberT n, nameT name_type, const char* pattern) {
		const NameBase* nb = base_->getNameBase();
		for (idNumberT i=0; i < n; i++) {
			const char* name = nb->GetName (name_type, i);
			mask_[i] = strAlphaContains(name, pattern);
		}
	}

	bool operator() (gamenumT gnum) const {
		bool res = mask_[(base_->getIndexEntry(gnum)->*f1_)()];
		if (!res && f2_ != 0) {
			return mask_[(base_->getIndexEntry(gnum)->*f2_)()];
		}
		return res;
	}
};

class SearchSiteCountry {
	const scidBaseT* base_;
	std::vector<bool> mask_;

public:
	SearchSiteCountry(const scidBaseT* base,
	                  std::string country)
	: base_(base) {
		idNumberT n = base->getNameBase()->GetNumNames(NAME_SITE);
		mask_.resize(n);

		std::transform(country.begin(), country.end(), country.begin(), ::toupper);
		const NameBase* nb = base_->getNameBase();
		for (idNumberT i=0; i < n; i++) {
			const char* site = nb->GetName(NAME_SITE, i);
			const char* it = site;
			while (*it != 0) it++;
			if (std::distance(site, it) > 3) it -= 3;
			std::string tmp = it;
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
			mask_[i] = (tmp == country);
		}
	}

	bool operator() (gamenumT gnum) const {
		return mask_[base_->getIndexEntry(gnum)->GetSite()];
	}
};

class SearchFlag {
	const scidBaseT* base_;
	uint32_t flagMask_;

public:
	SearchFlag(const scidBaseT* base,
	           const char* flags)
	: base_(base) {
		flagMask_ = IndexEntry::StrToFlagMask(flags);
		ASSERT(flagMask_ != 0);
	}

	bool operator() (gamenumT gnum) const {
		return base_->getIndexEntry(gnum)->GetFlag(flagMask_);
	}
};

class SearchResult {
	const scidBaseT* base_;
	bool result_[NUM_RESULT_TYPES];

public:
	SearchResult(const scidBaseT* base,
	             const char* results)
	: base_(base) {
		std::fill_n(result_, NUM_RESULT_TYPES, false);
		const char* end = RESULT_CHAR + NUM_RESULT_TYPES;
		while (*results != 0) {
			const char* it = std::find(RESULT_CHAR, end, *results);
			if (it != end) result_[std::distance(RESULT_CHAR, it)] = true;
			results++;
		}
	}

	bool operator() (gamenumT gnum) const {
		resultT r = base_->getIndexEntry(gnum)->GetResult();
		return result_[r];
	}
};

template <typename T>
class SearchRange : public StrRange {
protected:
	const scidBaseT* base_;
	T (IndexEntry::* f_) () const;

protected:
	SearchRange(const scidBaseT* base,
	            T (IndexEntry::* f) () const)
	: base_(base), f_(f) {}

public:
	SearchRange(const scidBaseT* base,
	            const char* range,
	            T (IndexEntry::* f) () const)
	: StrRange(range), base_(base), f_(f) {}

	bool operator() (gamenumT gnum) const {
		long v = (base_->getIndexEntry(gnum)->*f_)();
		return inRange(v);
	}
};

class SearchRangeDate : public SearchRange<dateT> {
public:
	SearchRangeDate(const scidBaseT* base,
	                const char* range,
	                dateT (IndexEntry::* f) () const)
	: SearchRange<dateT>(base, f) {
		// Extract two whitespace-separated dates:
		const char* v = strFirstWord(range);
		min_ = date_EncodeFromString (v);
		const char* next = strNextWord(v);
		max_ = (*next == 0) ? min_ : date_EncodeFromString (next);
		if (min_ > max_) std::swap(min_, max_);
	}
};

class SearchRangeEco : public SearchRange<ecoT> {
public:
	SearchRangeEco(const scidBaseT* base,
	               const char* range,
	               ecoT (IndexEntry::* f) () const)
	: SearchRange<ecoT>(base, f) {
		// Extract two whitespace-separated ECO codes:
		const char* v = strFirstWord(range);
		min_ = eco_FromString(v);
		const char* next = strNextWord(v);
		max_ = (*next == 0) ? min_ : eco_FromString(next);
		if (min_ > max_) std::swap(min_, max_);
		// Set eco maximum to be the largest subcode, for example,
		// "B07" -> "B07z4" to make sure subcodes are included in the range:
		max_ = eco_LastSubCode(static_cast<ecoT>(max_));
	}
};

class SearchRangeGamenum : public SearchRange<gamenumT> {
public:
	SearchRangeGamenum(const scidBaseT* base,
	                   const char* range)
	: SearchRange<gamenumT>(base, range, 0) {
		// Set up game number range:
		// Note that a negative number means a count from the end,
		// so -1 = last game, -2 = second to last, etc.
		if (min_ < 0) min_ += base->numGames();
		else min_ -=1;
		if (max_ < 0) max_ += base->numGames();
		else max_ -=1;
		if (min_ > max_) std::swap(min_, max_);
	}

	bool operator() (gamenumT gnum) const {
		if (static_cast<long>(gnum) < min_ ||
			static_cast<long>(gnum) > max_) return false;
		return true;
	}
};

class SearchRangeElo : public SearchRange<eloT> {
protected:
	eloT (IndexEntry::* fElo1_) (const NameBase*) const;
	eloT (IndexEntry::* fElo2_) (const NameBase*) const;
	const NameBase* nb_;

public:
	SearchRangeElo(const scidBaseT* base,
	               const char* range,
	               eloT (IndexEntry::* f1) (const NameBase*) const,
	               eloT (IndexEntry::* f2) (const NameBase*) const = 0)
	: SearchRange<eloT>(base, range, 0), fElo1_(f1), fElo2_(f2) {
		nb_ = base_->getNameBase();
	}

	bool operator() (gamenumT gnum) const {
		long v1 = (base_->getIndexEntry(gnum)->*fElo1_)(nb_);
		long v2 = min_;
		if (fElo2_ != 0) v2 = (base_->getIndexEntry(gnum)->*fElo2_)(nb_);
		if (v1 < min_ || v1 > max_ || v2 < min_ || v2 > max_) return false;
		return true;
	}
};

class SearchRangeEloDiff : public SearchRangeElo {
public:
	SearchRangeEloDiff(const scidBaseT* base,
	                   const char* range,
	                   eloT (IndexEntry::* f1) (const NameBase*) const,
	                   eloT (IndexEntry::* f2) (const NameBase*) const)
	: SearchRangeElo(base, range, f1, f2) {}

	bool operator() (gamenumT gnum) const {
		long v1 = (base_->getIndexEntry(gnum)->*fElo1_)(nb_);
		long v2 = (base_->getIndexEntry(gnum)->*fElo2_)(nb_);
		long v = v1 - v2;
		if (v < min_ || v > max_) return false;
		return true;
	}
};

/**
 * class SearchParam - criteria for the search
 *
 * a SearchParam is a string pair <criteria,value>.
 * criteria should start with '-' and can include optional trailing chars:
 *     '!' look for games that do _not_ match the criteria
 *     '|' look for games that matches the previous criteria _or_ this critera
 *         _or_ operations can be concatenated, so -player a -player| b -player| c
 *         means player contains 'a' or 'b' or 'c'
 * This class do not check if criteria is valid.
 */
class SearchParam {
	std::string op_;
	const char* value_;
	bool opNot_;
	bool opOr_;

public:
	SearchParam(const char* op, const char* value)
	: value_(value), opNot_(false), opOr_(false) {
		if (op != 0 && value_ != 0 && value_[0] != 0) {
			op_ = op;
			if (op_.length() > 1 && op_[0] == '-') {
				op_.erase(0, 1);

				size_t extra = op_.find_last_not_of("!|");
				if (extra == std::string::npos) {
					op_.clear();
				} else if (++extra < op_.length()) {
					if (op_.find('!', extra) != std::string::npos) opNot_ = true;
					if (op_.find('|', extra) != std::string::npos) opOr_ = true;
					op_.erase(extra);
				}
			}
		}
	}

	void invalidate() {
		op_.clear();
		opNot_ = false;
		opOr_ = false;
	}

	bool operator!() const { return op_.empty(); }
	bool operator==(const char* s) const {
		// Compares @s with op_ (op_ do not contain the starting '-' char
		// and the optional trailing '!' and '|' chars.
		return op_ == s;
	}

	const char* getValue() const { return value_; }
	bool isNot() const { return opNot_; }
	bool isOr() const { return opOr_; }
};

/**
 * parseParams() - parse search criteria
 *
 * Return: a vector containing the SearchParams for the search
 * and set @filterOp to the requested operation.
 */
std::vector<SearchParam> parseParams(int argc, const char ** argv, filterOpT& filterOp) {
	std::vector<SearchParam> res;

	for (int i=0; (i+1) < argc; i += 2) {
		SearchParam p(argv[i], argv[i+1]);
		if (!p) continue;

		if (p == "filter")
			filterOp = strGetFilterOp(p.getValue());
		else
			res.push_back(p);
	}

	return res;
}

/**
 * gamesToSearch() - create the vector of games to be searched
 * @base:      the scidBaseT to search
 * @filter:    games to be searched
 * @filterOp:  FILTEROP_RESET -> reset @filter to include all games
 *             FILTEROP_AND   -> search only games in @filter
 *             FILTEROP_OR    -> search only games not in @filter
 *
 * Return: a vector containing the games (game number) to be searched.
 * If @filterOp == FILTEROP_RESET then
 *         @filter is reset to include all games
 *         @filterOp is changed to FILTEROP_AND
 */
std::vector<gamenumT> collectGames(HFilter& filter, filterOpT& filterOp) {
	ASSERT(filter != 0);

	std::vector<gamenumT> res;
	switch (filterOp) {
	case FILTEROP_RESET:
		filter->includeAll();
		filterOp = FILTEROP_AND;
		/* FALLTHRU */
	case FILTEROP_AND:
		res.resize(filter->size());
		std::copy(filter->begin(), filter->end(), res.begin());
		break;
	case FILTEROP_OR:
		HFilterInverted excluded(filter);
		res.resize(excluded.size());
		std::copy(excluded.begin(), excluded.end(), res.begin());
		break;
	}
	return res;
}

/**
 * doSearch() - partition the input range
 * @itB:    begin of the range
 * @itR:    previous partition point
 * @itE:    end of the range
 * @base:   the scidBaseT to search
 * @param:  parameters for the search
 *
 * Return: the new partition point so that the range [itB, result) contains the matching games
 * If @param is not found, invalidate it and return the previous partition point (@itR)
 */
template<typename I>
I doSearch(I itB, I itR, I itE, const scidBaseT* base, SearchParam& param) {
	if (param == "player") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_PLAYER, &IndexEntry::GetWhite, &IndexEntry::GetBlack)
	);
	if (param == "white") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_PLAYER, &IndexEntry::GetWhite)
	);
	if (param == "black") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_PLAYER, &IndexEntry::GetBlack)
	);
	if (param == "event") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_EVENT, &IndexEntry::GetEvent)
	);
	if (param == "site") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_SITE, &IndexEntry::GetSite)
	);
	if (param == "sitecountry") return std::stable_partition(itB, itE,
		SearchSiteCountry(base, param.getValue())
	);
	if (param == "round") return std::stable_partition(itB, itE,
		SearchName(base, param.getValue(), NAME_ROUND, &IndexEntry::GetRound)
	);
	if (param == "date") return std::stable_partition(itB, itE,
		SearchRangeDate(base, param.getValue(), &IndexEntry::GetDate)
	);
	if (param == "eventdate") return std::stable_partition(itB, itE,
		SearchRangeDate(base, param.getValue(), &IndexEntry::GetEventDate)
	);
	if (param == "elo") return std::stable_partition(itB, itE,
		SearchRangeElo(base, param.getValue(), &IndexEntry::GetWhiteElo, &IndexEntry::GetBlackElo)
	);
	if (param == "welo") return std::stable_partition(itB, itE,
		SearchRangeElo(base, param.getValue(), &IndexEntry::GetWhiteElo)
	);
	if (param == "belo") return std::stable_partition(itB, itE,
		SearchRangeElo(base, param.getValue(), &IndexEntry::GetBlackElo)
	);
	if (param == "delo") return std::stable_partition(itB, itE,
		SearchRangeEloDiff(base, param.getValue(), &IndexEntry::GetWhiteElo, &IndexEntry::GetBlackElo)
	);
	if (param == "eco") return std::stable_partition(itB, itE,
		SearchRangeEco(base, param.getValue(), &IndexEntry::GetEcoCode)
	);
	if (param == "gnum") return std::stable_partition(itB, itE,
		SearchRangeGamenum(base, param.getValue())
	);
	if (param == "length") return std::stable_partition(itB, itE,
		SearchRange<ushort>(base, param.getValue(), &IndexEntry::GetNumHalfMoves)
	);
	if (param == "n_variations") return std::stable_partition(itB, itE,
		SearchRange<uint>(base, param.getValue(), &IndexEntry::GetVariationCount)
	);
	if (param == "n_comments") return std::stable_partition(itB, itE,
		SearchRange<uint>(base, param.getValue(), &IndexEntry::GetCommentCount)
	);
	if (param == "n_nags") return std::stable_partition(itB, itE,
		SearchRange<uint>(base, param.getValue(), &IndexEntry::GetNagCount)
	);
	if (param == "flag") return std::stable_partition(itB, itE,
		SearchFlag(base, param.getValue())
	);
	if (param == "result") return std::stable_partition(itB, itE,
		SearchResult(base, param.getValue())
	);

	param.invalidate();
	return itR;
}

} // End of anonymous namespace


/**
 * search_index() - search for games using game's IndexEntry info
 * @param base:     the scidBaseT to search
 * @param filter:   the filter to be modified with the result of the search
 *                  (RESET, AND, OR operations can be applied to the filter)
 * @param argc:     number of argv elements
 * @param argv:     an array of string pairs <criteria,value>
 * @param progress: report search progress to UI
 *
 * This function perform a fast games search using the IndexEntry info.
 * Criteria should start with the char '-' and can include the optional trailing chars
 *     '!': meaning that we want the games that do _not_ match the <criteria,value> pair
 *     '|': that allows to search for games that match at least one <criteria,value> in
 *          a group of <criteria,value> pairs
 *
 * Names searches (-player, -white, -black, -event, -site -round) have special rules:
 * - normal searches ignore case and spaces, so -player carlsen is equal to -player C a R L s en
 * - player surname and name are usually separated by the char ,
 * - exact searches (that do not ignore case and spaces) can be performed enclosing the searched
 *   value in "" like -player "Carlsen, Magnus"
 *
 * Examples:
 * -player "Carlsen, Magnus"
 * means search for games played by (either as white or black) Carlsen (surname) Magnus (name)
 *
 * -player carlsen -black kramnik
 * means (white name contains 'carlsen' || black name contains 'carlsen') && (black name contains 'kramnik')
 *
 * -white carlsen -black anand -black| kramnik -black| aronian
 * means (white == carlsen) && (black == anand || black == kramnik || black == aronian)
 * and will find the games played by carlsen as white against anand, or kramnik, or aronian
 *
 * -elo "2700 4000" -event! blitz
 * means elo > 2700 && elo < 4000 && event != blitz
 * and will find the games played by players with elo greater or equal than 2700, excluding the blitz events
 *
 * -welo "2700 4000" -belo|! "0 2700" -delo "-200 200"
 * means (white elo > 2700 && white elo < 4000) || (belo is not in the range 0-2700) && ((welo - belo) > -200 && (welo - belo) < 200)
 */
errorT search_index(const scidBaseT* base, HFilter& filter, int argc,
                    const char** argv, const Progress& progress) {
	ASSERT(base != 0);
	ASSERT(filter != 0);

	filterOpT filterOp = FILTEROP_RESET;
	std::vector<SearchParam> params = parseParams(argc, argv, filterOp);
	std::vector<gamenumT> glist = collectGames(filter, filterOp);

	// Partition glist so that the range [glist.begin(), it_res)
	// contains the matching games
	typedef std::vector<gamenumT>::iterator iter;
	iter it_begin = glist.begin();
	iter it_end = glist.end();
	iter it_res = glist.end();
	for (size_t i = 0, n = params.size(); i < n; i++) {
		if (!progress.report(i, n)) return ERROR_UserCancel;

		if (params[i].isOr()) {
			it_begin = it_res;
			// Now the range [it_begin, it_end) include the games
			// that did not matched the previous AND criteria
		} else {
			it_end = it_res;
			it_begin = glist.begin();
			// Now the range [glist.begin(), it_res) include the games
			// that matched all the criteria until now.
			// Nonmatching games are in the range [it_end, glist.end())
			// and excluded from subsequent searches
		}

		it_res = doSearch(it_begin, it_res, it_end, base, params[i]);

		if (params[i].isNot()) {
			// The nonmatching games are in the range [it_res, it_end)
			// Rotate glist so that nonmatching games are in
			// the range [it_begin, it_res)
			std::rotate(it_begin, it_res, it_end);
			it_res = it_begin + (it_end - it_res);
		}
	}

	if (filterOp == FILTEROP_AND) {
		// Remove nonmatching games from @filter
		for (iter it = it_res; it != glist.end(); ++it)
			filter->erase(*it);
	} else {
		// filterOp == FILTEROP_OR
		// Add matching games to @filter
		for (iter it = glist.begin(); it != it_res; ++it)
			filter->set(*it, 1);
	}
	progress.report(1,1);

	return OK;
}

