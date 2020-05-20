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

#ifndef SPELLCHK_H
#define SPELLCHK_H

#include "namebase.h"
#include "date.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#ifdef SPELLCHKVALIDATE
#include <fstream>
#endif

/*
* A "spelling" file contains the correct names for players, events, sites and rounds.
* Optionally it can provide further informations for players like elo, birthdate, etc..
* See the header of spelling.ssp for a more detailed description of the format.
*/


/**
 * class NameNormalizer - apply general corrections to a name
 *
 * Spelling files can provide general corrections in the form:
 * %Prefix "wrong prefix" "correct prefix"
 * %Infix "wrong suffix" "correct suffix"
 * %Suffix "wrong suffix" "correct suffix"
 *
 * Example:
 * %Prefix "II " "2. "
 * %Infix "3rd " "3. "
 * %Suffix "(Italy)" "ITA"
 * "II champ 3rd II 3rd (Italy) (Italy)" --> "2. champ 3. II 3. (Italy) ITA"
 */
class NameNormalizer {
	typedef std::vector< std::pair<std::string,std::string> > Cont;
	Cont prefix_;
	Cont infix_ ;
	Cont suffix_;

public:
	/**
	 * normalize() - correct a name
	 * @name: the name to be corrected
	 *
	 * Return: count of corrections applied
	 */
	size_t normalize(std::string* name) const {
		size_t corrections = 0;
		Cont::const_iterator it;

		for (it = prefix_.begin(); it != prefix_.end(); it++) {
			const std::string& s = it->first;
			if (name->compare(0, s.length(), s) == 0) {
				corrections++;
				name->replace(0, s.length(), it->second);
				break;
			}
		}

		for (it = infix_.begin(); it != infix_.end(); it++) {
			const std::string& s = it->first;
			size_t pos = name->find(s);
			while (pos != std::string::npos) {
				corrections++;
				name->replace(pos, s.length(), it->second);
				pos = name->find(s, pos + it->second.length());
			}
		}

		for (it = suffix_.begin(); it != suffix_.end(); it++) {
			const std::string& s = it->first;
			if (name->length() < s.length()) continue;
			size_t pos = name->length() - s.length();
			if (name->compare(pos, s.length(), s) == 0) {
				corrections++;
				name->replace(pos, s.length(), it->second);
				break;
			}
		}

		return corrections;
	}

	/**
	 * add*fix() - add a general correction
	 *
	 * Adds a general prefix, infix or suffix correction.
	 * Syntax for @e s is:
	 * %Suffix "wrong suffix" "correct suffix"
	 * Return: OK if successful
	 */
	errorT addPrefix(const char* s) { return add(prefix_, s); }
	errorT addInfix (const char* s) { return add(infix_, s); }
	errorT addSuffix(const char* s) { return add(suffix_,s); }

private:
	errorT add(Cont& v, const char* s) {
		ASSERT(s != 0);
		std::vector<size_t> parse;
		for (size_t i=0; *(s+i) != 0; i++) {
			if (*(s+i) == '"') parse.push_back(i);
		}
		if (parse.size() != 4) return ERROR_CorruptData;
		parse[0] += 1; //skip "
		parse[1] -= parse[0]; //n_chars
		if (parse[1] == 0) return ERROR_CorruptData;
		parse[2] += 1; //skip "
		parse[3] -= parse[2]; //n_chars
		v.push_back(std::make_pair(
			std::string(s + parse[0], parse[1]),
			std::string(s + parse[2], parse[3])
		));
		return OK;
	}
};


/**
 * class PlayerElo - elo ratings of a player
 *
 * Spelling files can provide elo ratings of a player in the form:
 * %Elo YEAR:ELO_1PERIOD,ELO_2PERIOD,ELO_3PERIOD,... YEAR:ELO_1PERIOD,...
 */
class PlayerElo {
	std::vector< std::pair<uint16_t, eloT> > elo_;

public:
	void AddEloData(const char* str);

	eloT getElo (dateT date) const {
		uint year = date_GetYear (date);
		auto itBegin = std::find_if(elo_.begin(), elo_.end(),
		                            [&](const std::pair<uint16_t, eloT>& e) {
			                            return e.first == year;
		                            });
		auto itEnd = std::find_if(itBegin, elo_.end(),
		                          [&](const std::pair<uint16_t, eloT>& e) {
			                          return e.first != year;
		                          });

		size_t n = std::distance(itBegin, itEnd);
		if (n == 0) return 0; // No data for that year

		uint month = date_GetMonth (date);
		if (month == 0 || month > 12) month = 0;
		else month -= 1;

		size_t idx;
		if (year == 2009 && n == 5) {
			//2 trimonthly + 3 bimonthly
			idx = (month < 6) ? month / 3 : (month - 2)/2;

		} else if (year == 2012 && n == 9) {
			//3 bimonthly + 6 monthly
			idx = (month < 6) ? month / 2 : month - 3;

		} else if (year > 2012) {
			// monthly
			if (month >= n) return 0;
			idx = month;

		} else {
			idx = month * n / 12;
		}

		return (itBegin + idx)->second;
	}

#ifdef SPELLCHKVALIDATE
	std::string isValid() const {
		for (size_t i=1, n=elo_.size(); i < n; i++) {
			if (elo_[i].first < elo_[i -1].first) return "unsorted";
		}

		auto count = [this](uint year) {
			return std::count_if(this->elo_.begin(), this->elo_.end(),
				[&](const std::pair<uint16_t, eloT>& e) { return e.first == year; });
		};

		auto expected = [](uint year) {
			if (year < 1990) return 1;
			if (year < 2001) return 2;
			if (year < 2009) return 4;
			if (year < 2010) return 5;
			if (year < 2012) return 6;
			if (year < 2013) return 9;
			return 12;
		};

		for (uint y=1970; y<2015; y++) {
			auto n = count(y);
			if (n == 0) continue;
			if (n != expected(y))
				return std::to_string(y) + ": " + std::to_string(n) + "(" +
				       std::to_string(expected(y)) + ")";
		}

		return std::string();
	}
#endif
};


/**
 * class PlayerInfo - player informations
 *
 * Spelling files can provide player informations like titles/gender,
 * countries, highest elo, date of birth, date of death. For example:
 * Polgar, Judit           #gm+w HUN [2735] 1976
 *
 * Generic information can be provided in the form:
 * %Bio This is a generic information
 */
class PlayerInfo {
	const char* comment_;
	std::vector<const char*> bio_;

	friend class SpellChkLoader;
	friend class SpellChecker;

public:
	PlayerInfo(const char* s) : comment_(s) {}
	const char* getTitle() const;
	const char* getLastCountry() const;
	dateT getBirthdate() const;
	dateT getDeathdate() const;
	eloT getPeakRating() const;
	const char* GetComment() const {
		return (comment_ != 0) ? comment_ : "";
	}
};


/**
 * class SpellChecker - name spelling
 *
 * Read a spell file and allow to retrieve corrected names and players data.
 * if SPELLCHKVALIDATE is defined also check the spell file for errors.
 */
class SpellChecker {
	struct Idx {
		std::string alias;
		int32_t idx;

		Idx() {}
		Idx(const std::string& a, int32_t i) : alias(a), idx(i) {}
		bool operator<(const Idx& b) const { return alias < b.alias; }
		bool operator<(const std::string& b)  const { return alias < b; }
	};
	typedef std::vector<Idx>::const_iterator IdxIt;

	NameNormalizer general_[NUM_NAME_TYPES];
	std::string excludeChars_[NUM_NAME_TYPES];
	std::vector<Idx> idx_[NUM_NAME_TYPES];
	std::vector<const char*> names_[NUM_NAME_TYPES];
	std::vector<PlayerInfo> pInfo_;
	std::vector<PlayerElo>  pElo_;
	char* staticStrings_;

	friend class SpellChkLoader;

public:
	~SpellChecker() {
		free(staticStrings_);
	}

	/**
	 * Create() - Create a new SpellChecker object
	 *
	 * Create a new SpellChecker reading from @e filename.
	 * It's the caller's responsibility to free the object with "delete".
	 * Return:
	 * - OK and a pointer to the new object
	 * - on error the ERROR_*CODE* and NULL
	 */
	static std::pair<errorT, SpellChecker*> Create(const char* filename,
	                                               const Progress& progress) {
		SpellChecker* res = new SpellChecker;
		errorT err = res->read(filename, progress);
		if (err != OK) {
			delete res;
			res = NULL;
		}
		return std::make_pair(err, res);
	}

	/**
	 * find() - search for correct names
	 * @nt:      the type of the name to be corrected
	 * @name:    the name to be corrected
	 * @nMaxRes: max size of the returned vector
	 *
	 * Return: a vector of correct names.
	 * @name will be normalized removing excludeChars_[@nt].
	 * If an exact match for normalized @name is found the result vector will
	 * contain only the corresponding correct name, otherwise will contain all
	 * the correct names that have @name as a prefix.
	 */
	std::vector<const char*> find(const nameT& nt, const char* name, uint nMaxRes = 10) const {
		ASSERT(nt < NUM_NAME_TYPES);
		ASSERT(name != 0);
		std::vector<const char*> res;
		std::pair<IdxIt, IdxIt> it;
		if (nt != NAME_PLAYER) it = idxFind(nt, name);
		else it = idxFindPlayer(name);
		for (; it.first != it.second && res.size() < nMaxRes; it.first++) {
			const char* corrected = names_[nt][it.first->idx];
			if (std::find(res.begin(), res.end(), corrected) == res.end()) {
				res.push_back(corrected);
			}
		}
		return res;
	}

	const NameNormalizer& getGeneralCorrections(const nameT& nt) const {
		ASSERT(nt < NUM_NAME_TYPES);
		return general_[nt];
	}

	/**
	* SpellChecker::getPlayerInfo() - get extra info about a player
	*
	* Get extra data like titles/gender, countries, highest elo,
	* date of birth, date of death or biographic informations.
	* Return:
	* - on success a pointer to a valid PlayerInfo object containing
	*   the available data. If @bio != 0 the vector is filled with
	*   the available biographic informations.
	* - if @name is not found or is ambiguous (match multiple players)
	*   returns NULL and @bio is untouched.
	*/
	const PlayerInfo* getPlayerInfo(const char* name,
	                                std::vector<const char*>* bio = 0) const {
		ASSERT(name != 0);
		IdxIt it = idxFindPlayerUnambiguous(name);
		if (it == idx_[NAME_PLAYER].end()) return 0; // not found

		if (bio != 0) *bio = pInfo_[it->idx].bio_;
		return &(pInfo_[it->idx]);
	}

	const PlayerElo* getPlayerElo(const char* name) const {
		ASSERT(name != 0);
		if (!hasEloData()) return 0;
		IdxIt it = idxFindPlayerUnambiguous(name);
		if (it == idx_[NAME_PLAYER].end()) return 0; // not found
		return &(pElo_[it->idx]);
	}

	bool hasEloData () const {
		return pElo_.size() != 0;
	}

	size_t numCorrectNames(const nameT& nt) const {
		ASSERT(nt < NUM_NAME_TYPES);
		return names_[nt].size();
	}

private:
	SpellChecker() : staticStrings_(NULL) {}
	SpellChecker(const SpellChecker&);
	SpellChecker& operator=(const SpellChecker&);

	errorT read(const char* filename, const Progress& progress);

	std::string normalizeAndTransform(const nameT& nt, const char* s) const { 
		std::string res;
		for (const char* i = s; *i != 0; i++) {
			if (excludeChars_[nt].find(*i) != std::string::npos) continue;

			res += *i;
		}
		return res;
	}

	std::pair<IdxIt, IdxIt> idxFind(const nameT& nt, const char* prefix) const {
		std::pair<IdxIt, IdxIt> res;
		std::string s = normalizeAndTransform(nt, prefix);
		res.first = std::lower_bound(idx_[nt].begin(), idx_[nt].end(), s);
		for (res.second = res.first; res.second != idx_[nt].end(); res.second++) {
			if (res.second->alias.compare(0, s.length(), s) != 0) break;
			if (res.second->alias == s) return std::make_pair(res.second, res.second +1);
		}
		return res;
	}

	std::pair<IdxIt, IdxIt> idxFindPlayer(const char* prefix) const {
		std::pair<IdxIt, IdxIt> res = idxFind(NAME_PLAYER, prefix);
		if (res.first == res.second) {
			// For spelling of player names (not other types), Scid will also try
			// to move the text after the last space in the name to the start of
			// the name for correction purposes, when it cannot find a correction.
			// This is done to correct names where the surname is last.
			std::string s = prefix;
			size_t pos = s.rfind(' ');
			if (pos != std::string::npos) {
				std::string inv = s.substr(pos);
				inv.append(s, 0, pos);
				return idxFind(NAME_PLAYER, inv.c_str());
			}
		}
		return res;
	}

	IdxIt idxFindPlayerUnambiguous(const char* name) const {
		std::pair<IdxIt, IdxIt> it = idxFindPlayer(name);
		if (it.first == it.second) return idx_[NAME_PLAYER].end();

		for (IdxIt i = it.first; i != it.second; i++) {
			if (i->idx != it.first->idx) //ambiguous
				return idx_[NAME_PLAYER].end();
		}
		return it.first;
	}


#ifndef SPELLCHKVALIDATE
	class SpellChkValidate {
	public:
		SpellChkValidate(const char*, const SpellChecker&) {}
		void ignoredLine(const char*) {}
		void idxDuplicates(const nameT&) {}
		void checkEloData() {}
	};
#else
	class SpellChkValidate {
		const SpellChecker& spell_;
		std::ofstream f_;

	public:
		SpellChkValidate(const char* spellfile, const SpellChecker& sp) : spell_(sp) {
			f_.open(spellfile + std::string(".validate"));
		}
		void ignoredLine(const char* line) {
			f_ << "Ignored line:" << std::endl;
			f_ << line << std::endl;
			f_ << std::endl;
		}
		static bool cmpIdxAlias(const Idx& a, const Idx& b) {
			return a.alias == b.alias;
		}
		void idxDuplicates(const nameT& nt) {
			IdxIt it = spell_.idx_[nt].begin();
			IdxIt it_end = spell_.idx_[nt].end();
			for (;;) {
				it = std::adjacent_find(it, it_end, cmpIdxAlias);
				if (it == it_end) return;

				IdxIt it_endDuplicates = std::upper_bound(it, it_end, *it);
				f_ << "Duplicate hash: " << it->alias << std::endl;
				for(; it != it_endDuplicates; it++) {
					f_ << spell_.names_[nt][it->idx];
					f_ << " - Idx:" << it->idx << std::endl;
				}
				f_ << std::endl;
			}
		}
		void checkEloData() {
			for (size_t i=0, n = spell_.pElo_.size(); i < n; i++) {
				std::string s = spell_.pElo_[i].isValid();
				if (! s.empty()) {
					f_ << "Elo error: " << s << " --- ";
					f_ << spell_.names_[NAME_PLAYER][i] << std::endl;
				}
			}
		}
	};
#endif

};


#endif
