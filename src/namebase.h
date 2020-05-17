/*
 * Copyright (C) 2014-2017 Fulvio Benini

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

#ifndef SCID_NAMEBASE_H
#define SCID_NAMEBASE_H

#include "common.h"
#include "misc.h"
#include <algorithm>
#include <map>
#include <memory>
#include <vector>

/**
 * This class stores the database's names (players, events, sites and rounds).
 * Assigns a idNumberT (which will be used as reference) to each name.
 */
class NameBase {
	std::vector<std::unique_ptr<const char[]> > names_[NUM_NAME_TYPES];
	std::vector<eloT> eloV_;
	struct idxCmp {
		bool operator()(const char* str1, const char* str2) const {
			// *** Compatibility ***
			// Older code used a custom StrTree class with a peculiar sorting:
			// - the first char was interpreted as an unsigned char;
			// - the remaining part was compared with the function
			// strComapare(),
			//   which converts the chars to ints, and is not consistent with
			//   the standard function strcmp().
			// The old StrTree class did also have unpredictable behaviors when
			// fed with names not sorted according to that criteria, for example
			// it could create Namebase objects with duplicate entries.
			// ***
			if (*str1 == *str2)
				return strCompare(str1, str2) < 0;

			return static_cast<uint>(*str1) < static_cast<uint>(*str2);
		}
	};
	std::map<const char*, idNumberT, idxCmp> idx_[NUM_NAME_TYPES];

public:
	/**
	 * A NameBase stores the max ELO for each player. This functions updates
	 * the max ELO of a player if it's greater than the previous one.
	 * @param id:  a valid idNumberT corresponding to a NAME_PLAYER name.
	 * @param elo: the ELO.
	 */
	void AddElo(idNumberT id, eloT elo) {
		ASSERT(id < GetNumNames(NAME_PLAYER));
		if (elo > eloV_[id])
			eloV_[id] = elo;
	}

	/**
	 * Add a name (string) to the NameBase.
	 * If the name already exists the corresponding ID is returned.
	 * @param nt:      @e nameT type of the name to add.
	 * @param name:    the name to add.
	 * @param MAX_LEN: the max length for names of type @e nt
	 * @param MAX_ID:  the max ID allowed for names of type @e nt
	 * @returns
	 * - on success, a @e std::pair containing OK and the ID.
	 * - on failure, a @e std::pair containing an error code and 0.
	 */
	std::pair<errorT, idNumberT> addName(nameT nt, const char* name,
	                                     size_t MAX_LEN, idNumberT MAX_ID) {
		ASSERT(IsValidNameType(nt) && name != NULL);

		auto exists = idx_[nt].lower_bound(name);
		if (exists != idx_[nt].end() &&
		    !idx_[nt].key_comp()(name, exists->first))
			return std::make_pair(OK, exists->second);

		const size_t nameLen = strlen(name);
		if (nameLen > MAX_LEN)
			return std::make_pair(ERROR_NameTooLong, 0);

		if (names_[nt].size() >= MAX_ID)
			return std::make_pair(ERROR_NameLimit, 0);

		char* buf = new char[nameLen + 1];
		std::copy_n(name, nameLen + 1, buf);
		idNumberT newID = static_cast<idNumberT>(names_[nt].size());
		names_[nt].emplace_back(buf);
		idx_[nt].emplace_hint(exists, buf, newID);

		if (nt == NAME_PLAYER)
			eloV_.push_back(0);

		return std::make_pair(OK, newID);
	}

	/**
	 * Frees memory, leaving the object empty.
	 */
	void Clear() { *this = NameBase(); }

	/**
	 * @returns references to the NameBase's containers.
	 * (must be used only to read names from files)
	 */
	std::tuple<decltype(idx_) &, decltype(names_) &, decltype(eloV_) &>
	getData() {
		return std::tuple<decltype(idx_)&, decltype(names_)&, decltype(eloV_)&>(
		    idx_, names_, eloV_);
	}

	/**
	 * @param id: a valid idNumberT corresponding to a NAME_PLAYER name.
	 * @returns the max ELO of a player.
	 */
	eloT GetElo(idNumberT id) const {
		ASSERT(id < GetNumNames(NAME_PLAYER));
		return eloV_[id];
	}

	/**
	 * Get the first few matches of a name prefix.
	 * @param nt:         @e nameT type of the name to be searched.
	 * @param str:        name prefix be searched.
	 * @param maxMatches: the max number of ID to return
	 * @returns a vector containing the ID of the matching names.
	 */
	std::vector<idNumberT> getFirstMatches(nameT nt, const char* str,
	                                       size_t maxMatches) const {
		ASSERT(IsValidNameType(nt) && str != NULL);

		std::vector<idNumberT> res;
		size_t len = strlen(str);
		for (auto it = idx_[nt].lower_bound(str);
		     it != idx_[nt].end() && res.size() < maxMatches; ++it) {
			const char* s = it->first;
			if (strlen(s) < len || !std::equal(str, str + len, s))
				break;
			res.emplace_back(it->second);
		}
		return res;
	}

	/**
	 * Retrieve a name.
	 * @param nt: the valid @e nameT type of the name to retrieve.
	 * @param id: the valid ID of the name to retrieve.
	 * @returns the name corresponding to @e id.
	 */
	const char* GetName(nameT nt, idNumberT id) const {
		ASSERT(IsValidNameType(nt) && id < GetNumNames(nt));
		return names_[nt][id].get();
	}

	/**
	 * @returns a reference to a container with all the names and IDs (given as
	 * std::pair<const char*, idNumberT>).
	 */
	const decltype(idx_) & getNames() const { return idx_; }

	/**
	 * @param nt: a valid @e nameT type.
	 * @returns the first invalid idNumberT (which is equal to the number of
	 * names stored).
	 */
	idNumberT GetNumNames(nameT nt) const {
		ASSERT(IsValidNameType(nt));
		return static_cast<idNumberT>(names_[nt].size());
	}

	/**
	 * Finds an exact full, case-sensitive name.
	 * @param nt:         @e nameT type of the name to be searched.
	 * @param str:        name to be be searched.
	 * @param[out] idPtr: pointer which will receive the ID of the name.
	 * @returns OK or ERROR_NameNotFound if the name does not exists.
	 */
	errorT FindExactName(nameT nt, const char* str, idNumberT* idPtr) const {
		ASSERT(IsValidNameType(nt) && str != NULL && idPtr != NULL);

		auto it = idx_[nt].find(str);
		if (it != idx_[nt].end()) {
			*idPtr = (*it).second;
			return OK;
		}
		return ERROR_NameNotFound;
	}

	/**
	 * For every name generates a 32bit hash with the first 4 chars.
	 * @param nt: @e nameT type of the names.
	 * @returns a vector containing the hashes.
	 */
	std::vector<uint32_t> generateHashMap(nameT nt) const {
		std::vector<uint32_t> res(names_[nt].size());
		std::transform(names_[nt].begin(), names_[nt].end(), res.begin(),
		               [](const std::unique_ptr<const char[]>& name) {
			               return strStartHash(name.get());
		               });
		return res;
	}

	/**
	 * Validate a @e nameT type.
	 * @param nt: @e nameT type to be validated.
	 * @returns true if @e nt is valid.
	 */
	static bool IsValidNameType(nameT nt) { return (nt < NUM_NAME_TYPES); }

	/**
	 * Match a string to a nameT.
	 * To match, the string should be a prefix of "player", "event", "site" or
	 * "round", or be a superstring of it, e.g. "player ...."
	 * @param str: the string to be matched.
	 * @returns a valid nameT, or NAME_INVALID.
	 */
	static nameT NameTypeFromString(const char* str) {
		if (*str == '\0')
			return NAME_INVALID;
		if (strIsAlphaPrefix(str, "player"))
			return NAME_PLAYER;
		if (strIsAlphaPrefix(str, "event"))
			return NAME_EVENT;
		if (strIsAlphaPrefix(str, "site"))
			return NAME_SITE;
		if (strIsAlphaPrefix(str, "round"))
			return NAME_ROUND;
		if (strIsAlphaPrefix("player", str))
			return NAME_PLAYER;
		if (strIsAlphaPrefix("event", str))
			return NAME_EVENT;
		if (strIsAlphaPrefix("site", str))
			return NAME_SITE;
		if (strIsAlphaPrefix("round", str))
			return NAME_ROUND;
		return NAME_INVALID;
	}
};

#endif // SCID_NAMEBASE_H
