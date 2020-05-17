/*
# Copyright (C) 2014-2019 Fulvio Benini

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

#ifndef SCIDBASE_H
#define SCIDBASE_H

#include "bytebuf.h"
#include "codec.h"
#include "containers.h"
#include "fastgame.h"
#include "game.h"
#include "index.h"
#include "namebase.h"
#include "tree.h"
#include <array>
#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

class SortCache;


const gamenumT INVALID_GAMEID = 0xffffffff;

struct scidBaseT {
	struct Stats {
		uint flagCount[IndexEntry::IDX_NUM_FLAGS]; // Num of games with each flag set.
		dateT minDate;
		dateT maxDate;
		uint64_t nYears;
		uint64_t sumYears;
		uint nResults [NUM_RESULT_TYPES];
		uint nRatings;
		uint64_t sumRatings;
		uint minRating;
		uint maxRating;

		Stats(const scidBaseT* dbase);

		struct Eco {
			uint count;
			uint results [NUM_RESULT_TYPES];

			Eco();
		};
		const Eco* getEcoStats(const char* ecoStr) const;

	private:
		Eco ecoEmpty_;
		Eco ecoValid_;
		Eco ecoStats_ [(1 + (1<<16)/131)*27];
		Eco ecoGroup1_[(1 + (1<<16)/131)/100];
		Eco ecoGroup2_[(1 + (1<<16)/131)/10];
		Eco ecoGroup3_[(1 + (1<<16)/131)];
	};

	scidBaseT();
	~scidBaseT();

	errorT open(std::string_view dbType, fileModeT fMode, const char* filename,
	            const Progress& progress = {}) {
		auto codec = ICodecDatabase::SCID4;
		if (dbType == "PGN") {
			codec = ICodecDatabase::PGN;
		} else if (dbType == "MEMORY") {
			codec = ICodecDatabase::MEMORY;
		} else if (dbType != "SCID4") {
			return ERROR_BadArg;
		}
		return openHelper(codec, fMode, filename, progress);
	}

	errorT Close ();

	const std::string& getFileName() const { return fileName_; }
	bool isReadOnly() const { return fileMode_ == FMODE_ReadOnly; }
	gamenumT numGames() const { return idx->GetNumGames(); }

	/// Returns a vector of tag pairs containing extra information about the
	/// database (type, description, autoload, etc..)
	std::vector<std::pair<const char*, std::string> > getExtraInfo() const {
		return codec_->getExtraInfo();
	}

	/// Store an extra information about the database (type, description, etc..)
	errorT setExtraInfo(const char* tagname, const char* new_value) {
		if (isReadOnly())
			return ERROR_FileReadOnly;

		const auto res = codec_->setExtraInfo(tagname, new_value);
		return (res != OK) ? res : codec_->flush();
	}

	const IndexEntry* getIndexEntry(gamenumT g) const {
		assert(g < numGames());
		return idx->GetEntry(g);
	}
	const IndexEntry* getIndexEntry_bounds(gamenumT g) const {
		static_assert(std::is_unsigned_v<gamenumT>);
		return g < numGames() ? getIndexEntry(g) : nullptr;
	}
	const NameBase* getNameBase() const {
		return nb_;
	}
	GameView getGame(const IndexEntry* ie) const {
		auto length = ie->GetLength();
		auto data = codec_->getGameData(ie->GetOffset(), length);
		if (data) {
			auto bbuf = ByteBuffer(data, length);
			auto err = bbuf.decodeTags([](auto, auto) {});
			if (err == OK) {
				auto [errPos, fen] = bbuf.decodeStartBoard();
				if (errPos == OK) {
					if (fen) {
						Position startPos;
						if (startPos.ReadFromFEN(fen) == OK) {
							return GameView(bbuf, startPos);
						}
					} else {
						return GameView(bbuf);
					}
				}
			}
		}
		return GameView({nullptr, 0});
	}
	ByteBuffer getGame(const IndexEntry& ie) const {
		auto length = ie.GetLength();
		auto data = codec_->getGameData(ie.GetOffset(), length);
		if (!data)
			length = 0; // Error

		return {data, length};
	}
	errorT getGame(const IndexEntry& ie, Game& dest) const {
		auto length = ie.GetLength();
		auto data = codec_->getGameData(ie.GetOffset(), length);
		if (!data)
			return ERROR_FileRead;

		auto bbuf = ByteBuffer(data, length);
		errorT err = dest.Decode(bbuf);
		if (err == OK)
			dest.LoadStandardTags(&ie, getNameBase());

		return err;
	}

	errorT importGames(const scidBaseT* srcBase, const HFilter& filter,
	                   const Progress& progress);
	errorT importGames(ICodecDatabase::Codec dbtype, const char* filename,
	                   const Progress& progress, std::string& errorMsg);

	/**
	 * Add or replace a game into the database.
	 * @param game: valid pointer to a Game object with the data of the game.
	 * @param replacedGameId: id of the game to replace.
	 *                        If >= numGames(), a new game will be added.
	 * @returns OK if successful or an error code.
	 */
	errorT saveGame(Game* game, gamenumT replacedGameId = INVALID_GAMEID);

	bool getFlag(uint flag, uint gNum) const {
		return idx->GetEntry(gNum)->GetFlag (flag);
	}
	errorT setFlag(bool value, uint flag, uint gNum);
	errorT setFlags(bool value, uint flag, const HFilter& filter);
	errorT invertFlag(uint flag, uint gNum);
	errorT invertFlags(uint flag, const HFilter& filter);

	/**
	 * A Filter is a selection of games, usually obtained searching the
	 * database. A new Filter is created calling the function newFilter()
	 * and must be released calling the function deleteFilter().
	 */
	std::string newFilter();
	void deleteFilter(const char* filterId);
	HFilter getFilter(std::string_view filterId) const;

	/// A composed filter is a special construct created combining two filters
	/// and includes only the games contained in both filters. It should NOT be
	/// deleted and became invalid if any of its components is deleted.
	/// @mainFilter: valid identifier of the main filter (the filter which is
	///              modified by non-const operations).
	/// @maskFilter: valid identifier of the mask filter (const).
	/// @return the id of the composed filter.
	std::string composeFilter(std::string_view mainFilter,
	                          std::string_view maskFilter) const;

	/// Get the components of a composed filter.
	/// @filterId: valid identifier of a filter.
	/// @return the components (second is empty if its not a a composed filter).
	std::pair<std::string, std::string>
	getFilterComponents(std::string_view filterId) const;

	const Stats& getStats() const;
	std::vector<TreeNode> getTreeStat(const HFilter& filter) const;
	uint getNameFreq (nameT nt, idNumberT id) {
		if (nameFreq_[nt].size() == 0)
			nameFreq_ = idx->calcNameFreq(*getNameBase());
		return nameFreq_[nt][id];
	}

	errorT getCompactStat(unsigned long long* n_deleted,
	                      unsigned long long* n_unused,
	                      unsigned long long* n_sparse,
	                      unsigned long long* n_badNameId);
	errorT compact(const Progress& progress);


	/**
	 * Increment the reference count of a SortCache object matching @e criteria.
	 * @param criteria: the list of fields by which games will be ordered.
	 *                  Each field should be followed by '+' to indicate an
	 *                  ascending order or by '-' for a descending order.
	 * @returns true on success
	 */
	bool createSortCache(const char* criteria);

	/**
	 * Decrement the reference count of the SortCache object matching @e
	 * criteria. Cached objects with refCount <= 0 are destroyed independently
	 * from the value of @e criteria.
	 * @param criteria: the list of fields by which games will be ordered.
	 *                  Each field should be followed by '+' to indicate an
	 *                  ascending order or by '-' for a descending order.
	 */
	void releaseSortCache(const char* criteria);

	/**
	 * Retrieve a list of ordered game indexes sorted by @e criteria.
	 * This function will be much faster if a SortCache object matching @e
	 * criteria already exists (previously created with @e createSortCache).
	 * @param criteria: the list of fields by which games will be ordered.
	 *                  Each field should be followed by '+' to indicate an
	 *                  ascending order or by '-' for a descending order.
	 * @param start:    the offset of the first row to return.
	 *                  The offset of the initial row is 0.
	 * @param count:    maximum number of rows to return.
	 * @param filter:   a reference to a valid (!= NULL) HFilter object.
	 *                  Games not included into the filter will be ignored.
	 * @param[out] destCont: valid pointer to an array where the sorted list of
	 *                       games will be stored (should be able to contain at
	 *                       least @e count elements).
	 * @returns the number of games' ids stored into @e destCont.
	 */
	size_t listGames(const char* criteria, size_t start, size_t count,
	                 const HFilter& filter, gamenumT* destCont);

	/**
	 * Get the sorted position of a game.
	 * This function will be much faster if a SortCache object matching @e
	 * criteria already exists (previously created with @e createSortCache).
	 * @param criteria: the list of fields by which games will be ordered.
	 *                  Each field should be followed by '+' to indicate an
	 *                  ascending order or by '-' for a descending order.
	 * @param filter:   a reference to a valid (!= NULL) HFilter object.
	 *                  Games not included into the filter will be ignored.
	 * @param gameId:   the id of the game.
	 * @returns the sorted position of @e gameId.
	 */
	size_t sortedPosition(const char* criteria, const HFilter& filter,
	                      gamenumT gameId);

	/**
	 * Transform the IndexEntries of the games included in @e hfilter.
	 * The @e entry_op must accept a IndexEntry& parameter and return true when
	 * the IndexEntry was modified.
	 * @param hfilter:  HFilter containing the games to be transformed.
	 * @param progress: a Progress object used for GUI communications.
	 * @param entry_op: operator that will be applied to games' IndexEntry.
	 * @returns a std::pair containing OK (or an error code) and the number of
	 * games modified.
	 */
	template <typename TOper>
	std::pair<errorT, size_t>
	transformIndex(HFilter hfilter, const Progress& progress, TOper entry_op) {
		if (auto errModify = beginTransaction())
			return {errModify, 0};

		auto res = transformIndex_(hfilter, progress, entry_op);
		auto err = endTransaction();
		res.first = (res.first == OK) ? err : res.first;
		return res;
	}

	/**
	 * Transform the names of the games included in @e hfilter.
	 * The function @e getID maps all the old idNumberT to the new idNumberT.
	 * It's invoked for each game and must accept as parameters a idNumberT and
	 * a const IndexEntry&; must return the (eventually different) idNumberT.
	 * @param nt:       type of the names to be modified.
	 * @param hfilter:  HFilter containing the games to be transformed.
	 * @param progress: a Progress object used for GUI communications.
	 * @param newNames: optional vector of names to be added to the database.
	 * @param fnInit:   function that is invoked before beginning the
	 *                  transformation; must accept a vector that contains the
	 *                  idNumberTs of the names in @e newNames.
	 * @param getID:    function that maps the old idNumberTs to the new ones.
	 * @returns a std::pair containing OK (or an error code) and the number of
	 * games modified.
	 */
	template <typename TInitFunc, typename TMapFunc>
	std::pair<errorT, size_t>
	transformNames(nameT nt, HFilter hfilter, const Progress& progress,
	               const std::vector<std::string>& newNames, TInitFunc fnInit,
	               TMapFunc getID);

	/**
	 * Transform the games included in @e hfilter.
	 * The @e entry_op must accept a Game& parameter and return true when
	 * the object was modified.
	 * @param hfilter:  HFilter containing the games to be transformed.
	 * @param progress: a Progress object used for GUI communications.
	 * @param entry_op: operator that will be applied to games.
	 * @returns a std::pair containing OK (or an error code) and the number of
	 * games modified.
	 */
	template <typename TOper>
	std::pair<errorT, size_t>
	transformGames(HFilter hfilter, const Progress& progress, TOper entry_op) {
		if (auto errModify = beginTransaction())
			return {errModify, 0};

		Game game;
		size_t nCorrections = 0;
		size_t iProg = 0;
		const size_t totProg = hfilter->size();
		errorT err = OK;
		for (const auto gnum : hfilter) {
			if ((++iProg % 1024 == 0) && !progress.report(iProg, totProg)) {
				err = ERROR_UserCancel;
				break;
			}

			const IndexEntry* ie = getIndexEntry(gnum);
			err = getGame(*ie, game);
			if (err != OK)
				break;

			if (!entry_op(game))
				continue;

			err = codec_->saveGame(&game, gnum);
			if (err != OK)
				break;

			++nCorrections;
		}
		const auto err_trans = endTransaction();
		if (err == OK)
			err = err_trans;
		return {err, nCorrections};
	}

	std::unique_ptr<gamenumT[]> extractDuplicates() {
		return std::move(duplicates_);
	}
	void setDuplicates(std::unique_ptr<gamenumT[]> duplicates) {
		duplicates_ = std::move(duplicates);
	}
	gamenumT getDuplicates(gamenumT gNum) const {
		return duplicates_ ? duplicates_[gNum] : 0;
	}

public:
	bool inUse;       // true if the database is open (in use).
	TreeCache treeCache;
	Filter* dbFilter;
	Filter* treeFilter;

	//TODO: this vars do not belong to scidBaseT class
	Game* game;       // the active game for this base.
	int gameNumber;   // game number of active game.
	bool gameAltered; // true if game is modified
	UndoRedo<Game, 100> gameAlterations;
	std::pair<Game*, bool> deprecated_push_pop;

private:
	std::unique_ptr<ICodecDatabase> codec_;
	Index* idx;
	NameBase* nb_;
	std::string fileName_; // File name without ".si" suffix
	fileModeT fileMode_; // Read-only, write-only, or both.
	std::vector< std::pair<std::string, Filter*> > filters_;
	mutable Stats* stats_;
	std::array<std::vector<int>, NUM_NAME_TYPES> nameFreq_;
	std::unique_ptr<gamenumT[]> duplicates_; // For each game: idx of duplicate game + 1 (0 if there is no duplicate).
	std::vector< std::pair<std::string, SortCache*> > sortCaches_;

private:
	errorT openHelper(ICodecDatabase::Codec dbtype, fileModeT mode,
	                  const char* filename, const Progress& progress = {});

	void clear();

	/// This function must be called before modifying the games of the database.
	/// Currently this function do not guarantees that the database is not
	/// altered in case of errors.
	errorT beginTransaction();

	/// Update caches and flush the database's files.
	/// This function must be called after changing one or more games.
	/// @param gameId: id of the modified game
	///                INVALID_GAMEID to update all games.
	/// @returns OK if successful or an error code.
	errorT endTransaction(gamenumT gameId = INVALID_GAMEID);

	errorT importGameHelper(const scidBaseT* sourceBase, uint gNum);

	SortCache* getSortCache(const char* criteria);

	/**
	 * Apply a transform operator to games' IndexEntry included in @e hfilter.
	 * The @entry_op should accept a IndexEntry& parameter and return true when
	 * the IndexEntry was modified.
	 * @param hfilter:  HFilter containing the games to be transformed.
	 * @param progress: a Progress object used for GUI communications.
	 * @param entry_op: operator that will be applied to games' IndexEntry.
	 * @returns a std::pair containing OK (or an error code) and the number of
	 * games modified.
	 */
	template <typename TOper>
	std::pair<errorT, size_t>
	transformIndex_(HFilter hfilter, const Progress& progress, TOper entry_op) {
		size_t nCorrections = 0;
		size_t iProg = 0;
		size_t totProg = hfilter->size();
		for (auto& gnum : hfilter) {
			if ((++iProg % 8192 == 0) && !progress.report(iProg, totProg))
				return std::make_pair(ERROR_UserCancel, nCorrections);

			IndexEntry newIE = *getIndexEntry(gnum);
			if (!entry_op(newIE))
				continue;

			auto err = codec_->saveIndexEntry(newIE, gnum);
			if (err != OK)
				return std::make_pair(err, nCorrections);

			++nCorrections;
		}
		return std::make_pair(OK, nCorrections);
	}
};

template <typename TInitFunc, typename TMapFunc>
std::pair<errorT, size_t>
scidBaseT::transformNames(nameT nt, HFilter hfilter, const Progress& progress,
                          const std::vector<std::string>& newNames,
                          TInitFunc initFunc, TMapFunc getNewID) {
	if (auto errModify = beginTransaction())
		return {errModify, 0};

	std::vector<idNumberT> nameIDs(newNames.size());
	auto it = nameIDs.begin();
	for (auto& name : newNames) {
		auto id = codec_->addName(nt, name.c_str());
		if (id.first != OK) {
			endTransaction();
			return std::make_pair(id.first, size_t(0));
		}
		*it++ = id.second;
	}

	initFunc(nameIDs);

	auto res = transformIndex_(hfilter, progress, [&](IndexEntry& ie) {
		const IndexEntry& ie_const = ie;
		idNumberT oldID;
		idNumberT oldBlackID = 0;
		idNumberT newBlackID = 0;
		switch (nt) {
		case NAME_PLAYER:
			oldID = ie_const.GetWhite();
			oldBlackID = ie_const.GetBlack();
			newBlackID = getNewID(oldBlackID, ie_const);
			break;
		case NAME_EVENT:
			oldID = ie_const.GetEvent();
			break;
		case NAME_SITE:
			oldID = ie_const.GetSite();
			break;
		default:
			ASSERT(nt == NAME_ROUND);
			oldID = ie_const.GetRound();
		}
		const auto newID = getNewID(oldID, ie_const);
		if (oldID == newID && oldBlackID == newBlackID)
			return false;

		switch (nt) {
		case NAME_PLAYER:
			ie.SetWhite(newID);
			ie.SetBlack(newBlackID);
			break;
		case NAME_EVENT:
			ie.SetEvent(newID);
			break;
		case NAME_SITE:
			ie.SetSite(newID);
			break;
		default:
			ASSERT(nt == NAME_ROUND);
			ie.SetRound(newID);
		}
		return true;
	});

	auto err = endTransaction();
	res.first = (res.first == OK) ? err : res.first;
	return res;
}

#endif
