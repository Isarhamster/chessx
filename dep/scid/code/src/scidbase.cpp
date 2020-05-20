/*
* Copyright (C) 2014-2016  Fulvio Benini

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

#include "scidbase.h"
#include "codec_memory.h"
#include "codec_pgn.h"
#include "codec_scid4.h"
#include "common.h"
#include "sortcache.h"
#include "stored.h"
#include <algorithm>

std::pair<ICodecDatabase*, errorT>
ICodecDatabase::open(Codec codec, fileModeT fMode, const char* filename,
                     const Progress& progress, Index* idx, NameBase* nb) {
	auto createCodec = [](auto codec) -> ICodecDatabase* {
		switch (codec) {
		case ICodecDatabase::MEMORY:
			return new CodecMemory();
		case ICodecDatabase::SCID4:
			return new CodecSCID4();
		case ICodecDatabase::PGN:
			return new CodecPgn();
		}
		ASSERT(0);
		return nullptr;
	};

	auto obj = createCodec(codec);
	auto err = obj->dyn_open(fMode, filename, progress, idx, nb);
	if (err != OK && err != ERROR_NameDataLoss) {
		delete obj;
		obj = nullptr;
	}
	return {obj, err};
}

scidBaseT::scidBaseT() {
	idx = new Index;
	nb_ = new NameBase;
	game = new Game;
	gameNumber = -1;
	gameAltered = false;
	inUse = false;
	fileMode_ = FMODE_None;
	dbFilter = new Filter(0);
	treeFilter = new Filter(0);
	stats_ = NULL;
}

scidBaseT::~scidBaseT() {
	if (inUse)
		Close();

	delete idx;
	delete nb_;
	delete game;
	delete stats_;
	delete dbFilter;
	delete treeFilter;
}

errorT scidBaseT::openHelper(ICodecDatabase::Codec dbtype, fileModeT fMode,
                             const char* filename, const Progress& progress) {
	assert(filename);

	if (inUse)
		return ERROR_FileInUse;

	auto obj = ICodecDatabase::open(dbtype, fMode, filename, progress, idx, nb_);
	if (obj.first) {
		codec_.reset(obj.first);
		inUse = true;
		fileMode_ = (fMode == FMODE_Create) ? FMODE_Both : fMode;
		fileName_ = filename;
		gameNumber = -1;

		// Initialize the filters: all the games are included by default.
		dbFilter->Init(numGames());
		treeFilter->Init(numGames());
		ASSERT(filters_.empty());

		// Default treeCache size: 250
		treeCache.CacheResize(250);
	} else {
		idx->Close();
		nb_->Clear();
	}

	return obj.second;
}

errorT scidBaseT::Close () {
	ASSERT(inUse);

	for (auto& sortCache : sortCaches_) {
		delete sortCache.second;
	}
	sortCaches_.clear();

	errorT errIdx = idx->Close();
	nb_->Clear();
	codec_ = nullptr;

	clear();
	game->Clear();
	fileMode_ = FMODE_None;
	fileName_ = "<empty>";
	gameNumber = -1;
	gameAltered = false;
	dbFilter->Init(0);
	treeFilter->Init(0);
	for (size_t i=0, n = filters_.size(); i < n; i++) delete filters_[i].second;
	filters_.clear();
	inUse = false;

	return errIdx;
}


void scidBaseT::clear() {
	if (stats_ != NULL) { delete stats_; stats_ = NULL;}
	duplicates_.reset();
	treeCache.Clear();
	for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
		nameFreq_[nt].resize(0);
	}
}

errorT scidBaseT::beginTransaction() {
	if (isReadOnly())
		return ERROR_FileReadOnly;

	for (auto& sortCache : sortCaches_) {
		sortCache.second->prepareForChanges();
	}
	return OK;
}

errorT scidBaseT::endTransaction(gamenumT gNum) {
	clear();
	errorT res = codec_->flush();

	auto n_games = numGames();
	if (dbFilter->Size() != n_games) {
		dbFilter->Resize(n_games);
		treeFilter->Resize(n_games);
		for (auto& filter : filters_) {
			filter.second->Resize(n_games);
		}
	}

	for (auto& sortCache : sortCaches_) {
		sortCache.second->checkForChanges(gNum);
	}

	return res;
}

errorT scidBaseT::saveGame(Game* game, gamenumT replacedGameId) {
	if (auto errModify = beginTransaction())
		return errModify;

	errorT err = (replacedGameId < numGames())
	                 ? codec_->saveGame(game, replacedGameId)
	                 : codec_->addGame(game);
	errorT errClear = endTransaction(replacedGameId);
	return (err != OK) ? err : errClear;
}

errorT scidBaseT::importGames(const scidBaseT* srcBase, const HFilter& filter, const Progress& progress) {
	ASSERT(srcBase != 0);
	ASSERT(filter != 0);
	if (srcBase == this) return ERROR_BadArg;

	if (auto errModify = beginTransaction())
		return errModify;

	errorT err = OK;
	size_t iProgress = 0;
	size_t totGames = filter->size();
	for (const auto gNum : filter) {
		err = importGameHelper(srcBase, gNum);
		if (err != OK)
			break;

		if (++iProgress % 8192 == 0) {
			if (!progress.report(iProgress, totGames))
				break;
		}
	}
	errorT errClear = endTransaction();
	return (err == OK) ? errClear : err;
}

errorT scidBaseT::importGameHelper(const scidBaseT* srcBase, gamenumT gNum) {
	auto srcIe = srcBase->getIndexEntry(gNum);
	auto dataSz = srcIe->GetLength();
	auto data = srcBase->codec_->getGameData(srcIe->GetOffset(), dataSz);
	if (data == nullptr)
		return ERROR_FileRead;

	return codec_->addGame(srcIe, srcBase->getNameBase(), data, dataSz);
}

errorT scidBaseT::importGames(ICodecDatabase::Codec dbtype,
                              const char* filename, const Progress& progress,
                              std::string& errorMsg) {
	ASSERT(dbtype == ICodecDatabase::PGN);

	if (auto errModify = beginTransaction())
		return errModify;

	CodecPgn pgn;
	auto res = pgn.open(filename, FMODE_ReadOnly);
	if (res == OK) {
		res = CodecPgn::parseGames(
		    progress, pgn, [&](Game& game) { return codec_->addGame(&game); });
		errorMsg = pgn.parseErrors();
	}

	auto res_endTrans = endTransaction();
	return (res != OK) ? res : res_endTrans;
}

errorT scidBaseT::invertFlag(uint flag, uint gNum) {
	return setFlag(!getFlag(flag, gNum), flag, gNum);
}

errorT scidBaseT::invertFlags(uint flag, const HFilter& filter) {
	return transformIndex(filter, Progress(),
	                      [&](IndexEntry& ie) {
		                      const auto value = ie.GetFlag(flag);
		                      ie.SetFlag(flag, !value);
		                      return true;
	                      })
	    .first;
}

errorT scidBaseT::setFlag(bool value, uint flag, uint gNum) {
	ASSERT(gNum < idx->GetNumGames());

	IndexEntry ie = *getIndexEntry(gNum);
	ie.SetFlag(flag, value);

	if (auto errModify = beginTransaction())
		return errModify;

	const auto res = codec_->saveIndexEntry(ie, gNum);
	const auto err = endTransaction(gNum);
	return res != OK ? res : err;
}

errorT scidBaseT::setFlags(bool value, uint flag, const HFilter& filter) {
	return transformIndex(filter, Progress(),
	                      [&](IndexEntry& ie) {
		                      ie.SetFlag(flag, value);
		                      return true;
	                      })
	    .first;
}

/**
 * Filters
 */
std::string scidBaseT::newFilter() {
	std::string newname = (filters_.size() == 0)
		? "a_"
		: filters_.back().first;
	if (newname[0] == 'z') {
		newname = 'a' + newname;
	} else {
		newname = ++(newname[0]) + newname.substr(1);
	}
	filters_.push_back(std::make_pair(newname, new Filter(numGames())));
	return newname;
}

std::string scidBaseT::composeFilter(std::string_view mainFilter,
                                     std::string_view maskFilter) const {
	std::string res;
	if (mainFilter.empty()) return res;

	if (mainFilter[0] != '+') {
		res = mainFilter;
	} else {
		size_t maskName = mainFilter.find('+', 1);
		if (maskName != std::string::npos)
			res = mainFilter.substr(1, maskName - 1);
	}

	if (!maskFilter.empty()) {
		res = '+' + res + "+";
		res.append(maskFilter);
	}

	if (getFilter(res) == 0) res.clear();
	return res;
}

void scidBaseT::deleteFilter(const char* filterId) {
	for (size_t i = 0, n = filters_.size(); i < n; i++) {
		if (filters_[i].first == filterId) {
			delete filters_[i].second;
			filters_.erase(filters_.begin() + i);
			break;
		}
	}
}

HFilter scidBaseT::getFilter(std::string_view filterId) const {
	const auto findFilter = [&](auto const& id) -> Filter* {
		if (id == "dbfilter")
			return dbFilter;
		if (id == "tree")
			return treeFilter;

		for (auto const& [name, filter] : filters_) {
			if (name == id)
				return filter;
		}
		return nullptr;
	};

	Filter* main = nullptr;
	const Filter* mask = nullptr;
	if (filterId.empty() || filterId[0] != '+') {
		main = findFilter(filterId);
	} else {
		size_t maskName = filterId.find('+', 1);
		if (maskName != std::string::npos) {
			main = findFilter(filterId.substr(1, maskName - 1));
			mask = findFilter(filterId.substr(maskName + 1));
		}
	}
	return HFilter(main, mask);
}

std::pair<std::string, std::string>
scidBaseT::getFilterComponents(std::string_view filterID) const {
	if (filterID.empty())
		return {};

	if (filterID[0] != '+')
		return {std::string(filterID), {}};

	size_t maskName = filterID.find('+', 1);
	ASSERT(maskName != std::string::npos);
	ASSERT(getFilter(filterID.substr(1, maskName - 1)) != nullptr);
	ASSERT(getFilter(filterID.substr(maskName + 1)) != nullptr);

	return {std::string(filterID.substr(1, maskName - 1)),
	        std::string(filterID.substr(maskName + 1))};
}

/**
 * Statistics
 */
const scidBaseT::Stats& scidBaseT::getStats() const {
	if (stats_ == NULL) stats_ = new scidBaseT::Stats(this);
	return *stats_;
}

scidBaseT::Stats::Eco::Eco()
: count(0) {
	std::fill_n(results, NUM_RESULT_TYPES, 0);
}

scidBaseT::Stats::Stats(const scidBaseT* dbase) {
	std::fill(flagCount, flagCount + IndexEntry::IDX_NUM_FLAGS, 0);
	minDate = ZERO_DATE;
	maxDate = ZERO_DATE;
	nYears = 0;
	sumYears = 0;
	std::fill_n(nResults, NUM_RESULT_TYPES, 0);
	nRatings = 0;
	sumRatings = 0;
	minRating = 0;
	maxRating = 0;

	// Read stats from index entry of each game:
	for (gamenumT gnum=0, n = dbase->numGames(); gnum < n; gnum++) {
		const IndexEntry* ie = dbase->getIndexEntry(gnum);
		nResults[ie->GetResult()]++;
		eloT elo = ie->GetWhiteElo();
		if (elo > 0) {
			nRatings++;
			sumRatings += elo;
			if (minRating == 0) { minRating = elo; }
			if (elo < minRating) { minRating = elo; }
			if (elo > maxRating) { maxRating = elo; }
		}
		elo = ie->GetBlackElo();
		if (elo > 0) {
			nRatings++;
			sumRatings += elo;
			if (minRating == 0) { minRating = elo; }
			if (elo < minRating) { minRating = elo; }
			if (elo > maxRating) { maxRating = elo; }
		}
		dateT date = ie->GetDate();
		if (gnum == 0) {
			maxDate = minDate = date;
		}
		if (date_GetYear(date) > 0) {
			if (date < minDate) { minDate = date; }
			if (date > maxDate) { maxDate = date; }
			nYears++;
			sumYears += date_GetYear (date);
		}

		for (uint flag = 0; flag < IndexEntry::IDX_NUM_FLAGS; flag++) {
			bool value = ie->GetFlag (1 << flag);
			if (value) {
				flagCount[flag]++;
			}
		}

		resultT result = ie->GetResult();
		ecoT eco = ie->GetEcoCode();
		if (eco == 0) {
			ecoEmpty_.count++;
			ecoEmpty_.results[result]++;
		} else {
			ecoValid_.count++;
			ecoValid_.results[result]++;
			eco = eco_Reduce(eco);
			ecoStats_[eco].count++;
			ecoStats_[eco].results[result]++;
			eco /= 27;
			ecoGroup3_[eco].count++;
			ecoGroup3_[eco].results[result]++;
			eco /= 10;
			ecoGroup2_[eco].count++;
			ecoGroup2_[eco].results[result]++;
			eco /= 10;
			ecoGroup1_[eco].count++;
			ecoGroup1_[eco].results[result]++;
		}
	}
}

const scidBaseT::Stats::Eco* scidBaseT::Stats::getEcoStats(const char* ecoStr) const {
	ASSERT(ecoStr != 0);

	if (*ecoStr == 0) return &ecoValid_;

	ecoT eco = eco_FromString(ecoStr);
	if (eco == 0) return 0;
	eco = eco_Reduce(eco);

	switch(strlen(ecoStr)) {
	case 0:
		return &ecoValid_;
	case 1:
		return &(ecoGroup1_[eco / 2700]);
	case 2:
		return &(ecoGroup2_[eco / 270]);
	case 3:
		return &(ecoGroup3_[eco / 27]);
	case 4:
	case 5:
		return &(ecoStats_[eco]);
	}

	return 0;
}


std::vector<TreeNode> scidBaseT::getTreeStat(const HFilter& filter) const {
	std::vector<TreeNode> res;
	auto nb = getNameBase();
	for (gamenumT gnum = 0, n = numGames(); gnum < n; gnum++) {
		uint ply = filter.get(gnum);
		if (ply == 0) continue;
		else ply--;

		const IndexEntry* ie = getIndexEntry(gnum);
		FullMove move = StoredLine::getMove(ie->GetStoredLineCode(), ply);
		if (!move)
			move = getGame(ie).getMove(ply);

		auto it = std::find_if(
		    res.begin(), res.end(),
		    [move](auto const& stat) { return stat.move == move; });

		auto& node = (it != res.end()) ? *it : res.emplace_back(move);
		node.add(ie->GetResult(), ie->GetWhiteElo(nb), ie->GetBlackElo(nb),
		         ie->GetYear());
	}

	std::sort(res.begin(), res.end(), TreeNode::cmp_ngames_desc());
	return res;
}

errorT scidBaseT::getCompactStat(unsigned long long* n_deleted,
                                 unsigned long long* n_unused,
                                 unsigned long long* n_sparse,
                                 unsigned long long* n_badNameId) {
	std::vector<uint> nbFreq[NUM_NAME_TYPES];
	for (nameT n = NAME_PLAYER; n < NUM_NAME_TYPES; n++) {
		nbFreq[n].resize(getNameBase()->GetNumNames(n), 0);
	}

	uint64_t last_offset = 0;
	*n_sparse = 0;
	*n_deleted = 0;
	for (gamenumT i=0, n = numGames(); i < n; i++) {
		const IndexEntry* ie = getIndexEntry (i);
		if (ie->GetDeleteFlag()) { *n_deleted += 1; continue; }

		auto offset = ie->GetOffset();
		if (offset < last_offset) *n_sparse += 1;
		last_offset = offset;

		nbFreq[NAME_PLAYER][ie->GetWhite()] += 1;
		nbFreq[NAME_PLAYER][ie->GetBlack()] += 1;
		nbFreq[NAME_EVENT][ie->GetEvent()] += 1;
		nbFreq[NAME_SITE][ie->GetSite()] += 1;
		nbFreq[NAME_ROUND][ie->GetRound()] += 1;
	}

	*n_unused = 0;
	for (nameT n = NAME_PLAYER; n < NUM_NAME_TYPES; n++) {
		*n_unused += std::count(nbFreq[n].begin(), nbFreq[n].end(), 0);
	}

	*n_badNameId = idx->GetBadNameIdCount();
	return OK;
}

errorT scidBaseT::compact(const Progress& progress) {
	std::vector<std::string> filenames = codec_->getFilenames();
	if (filenames.empty()) return ERROR_CodecUnsupFeat;

	if (fileMode_ != FMODE_Both) {
		//Older scid version to be upgraded are opened read only
		if (idx->GetVersion() == SCID_VERSION) return ERROR_FileMode;
	}

	//1) Create a new temporary database
	std::string filename = fileName_;
	std::string tmpfile = filename + "__COMPACT__";
	ICodecDatabase::Codec dbtype = codec_->getType();
	scidBaseT tmp;
	errorT err_Create = tmp.openHelper(dbtype, FMODE_Create, tmpfile.c_str());
	if (err_Create != OK) return err_Create;

	//2) Create the list of games to be copied
	std::vector< std::pair<uint64_t, gamenumT> > sort;
	uint n_deleted = 0;
	for (gamenumT i = 0, n = numGames(); i < n; i++) {
		const IndexEntry* ie = getIndexEntry(i);
		if (ie->GetDeleteFlag()) {
			n_deleted++;
			continue;
		}
		uint64_t order = static_cast<uint64_t>(ie->GetStoredLineCode()) << 56;
		const byte* hp = ie->GetHomePawnData();
		order |= static_cast<uint64_t>(hp[0]) << 48;
		order |= static_cast<uint64_t>(hp[1]) << 40;
		order |= static_cast<uint64_t>(hp[2]) << 32;
		order |= static_cast<uint64_t>(hp[3]) << 24;
		order |= ie->GetFinalMatSig() & 0xFFFFFF;
		sort.emplace_back(order, i);
	}
	if (sort.size() > 10000) // Reorder only larger databases
		std::stable_sort(sort.begin(), sort.end());

	//3) Copy the Index Header
	auto extraInfo = getExtraInfo();
	errorT err_Header = tmp.beginTransaction();
	for (auto& pair : extraInfo) {
		if (err_Header != OK)
			break;

		if (std::strcmp(pair.first, "autoload") == 0) {
			gamenumT autoloadOld = strGetUnsigned(pair.second.c_str());
			size_t autoloadNew = 1;
			for (size_t i = 0, n = sort.size(); i < n; ++i) {
				if (sort[i].second + 1 == autoloadOld) {
					autoloadNew = i + 1;
					break;
				}
			}
			pair.second = std::to_string(autoloadNew);
		}
		err_Header = tmp.codec_->setExtraInfo(pair.first, pair.second.c_str());
	}

	//4) Copy the games
	uint iProgress = 0;
	bool err_UserCancel = false;
	errorT err_AddGame = OK;
	for (auto it = sort.cbegin(); it != sort.cend(); ++it) {
		err_AddGame = tmp.importGameHelper(this, it->second);
		if (err_AddGame != OK) break;

		//TODO:
		//- update bookmarks game number
		//  (*it).second   == old game number
		//  tmp.numGames() == new game number
		if (++iProgress % 8192 == 0) {
			if (!progress.report(iProgress, sort.size())) {
				err_UserCancel = true;
				break;
			}
		}
	}

	//5) Finalize the new database
	std::vector<std::string> tmp_filenames = tmp.codec_->getFilenames();
	errorT err_NbWrite = tmp.endTransaction();
	errorT err_Close = tmp.Close();
	if (err_Close == OK) err_Close = (filenames.size() == tmp_filenames.size()) ? OK : ERROR;

	//6) Error: cleanup and report
	if (err_Header != OK || err_AddGame != OK || err_UserCancel ||
	    err_NbWrite != OK || err_Close != OK) {
		for (size_t i = 0, n = tmp_filenames.size(); i < n; i++) {
			std::remove(tmp_filenames[i].c_str());
		}
		if (err_Header != OK)
			return err_Header;
		if (err_AddGame != OK)
			return err_AddGame;
		if (err_UserCancel)
			return ERROR_UserCancel;
		if (err_NbWrite != OK)
			return err_NbWrite;

		return err_Close;
	}

	//7) Remember the active filters and SortCaches
	std::vector<std::string> filters(filters_.size());
	for (size_t i = 0, n = filters_.size(); i < n; i++) {
		filters[i] = filters_[i].first;
	}
	std::vector< std::pair<std::string, int> > oldSC;
	for (auto& sortCache : sortCaches_) {
		int refCount = sortCache.second->incrRef(0);
		if (refCount >= 0)
			oldSC.emplace_back(sortCache.first, refCount);
	}

	//8) Remove the old database
	if (Close() != OK) return ERROR_FileInUse;
	for (size_t i = 0, n = filenames.size(); i < n; i++) {
		if (std::remove(filenames[i].c_str()) != 0) return ERROR_CompactRemove;
	}

	//9) Success: rename the files and open the new database
	for (size_t i = 0, n = filenames.size(); i < n; i++) {
		const char* s1 = tmp_filenames[i].c_str();
		const char* s2 = filenames[i].c_str();
		std::rename(s1, s2);
	}
	errorT res = openHelper(dbtype, FMODE_Both, filename.c_str());

	//10) Re-create filters and SortCaches
	if (res == OK || res == ERROR_NameDataLoss) {
		for (size_t i = 0, n = filters.size(); i < n; i++) {
			filters_.push_back(
			    std::make_pair(filters[i], new Filter(numGames())));
		}
		for (size_t i = 0, n = oldSC.size(); i < n; i++) {
			const std::string& criteria = oldSC[i].first;
			SortCache* sc = SortCache::create(idx, nb_, criteria.c_str());
			if (sc != NULL) {
				sc->incrRef(oldSC[i].second);
				sortCaches_.emplace_back(criteria, sc);
			}
		}
	}

	return res;
}

/**
 * Retrieve a SortCache object matching the supplied @e criteria.
 * A new SortCache with refCount equal to 0 is created if a suitable object is
 * not found in @e sortCaches_. Objects with refCount <= 0 are destroyed by the
 * @e releaseSortCache function independently from the provided @e criteria
 * argument (implementing a rudimentary garbage collector).
 * @param criteria: the list of fields by which games will be ordered.
 *                  Each field should be followed by '+' to indicate an
 *                  ascending order or by '-' for a descending order.
 * @returns a pointer to a SortCache object in case of success, NULL otherwise.
 */
SortCache* scidBaseT::getSortCache(const char* criteria) {
	ASSERT(criteria != NULL);

	for (auto& sortCache : sortCaches_) {
		if (sortCache.first == criteria)
			return sortCache.second;
	}

	SortCache* sc = SortCache::create(idx, getNameBase(), criteria);
	if (sc != NULL)
		sortCaches_.emplace_back(criteria, sc);

	return sc;
}

void scidBaseT::releaseSortCache(const char* criteria) {
	size_t i = 0;
	while (i < sortCaches_.size()) {
		const char* tmp = sortCaches_[i].first.c_str();
		int decr = std::strcmp(criteria, tmp) ? 0 : -1;
		if (sortCaches_[i].second->incrRef(decr) <= 0) {
			delete sortCaches_[i].second;
			sortCaches_.erase(sortCaches_.begin() + i);
			continue; //do not increment i
		}
		i += 1;
	}
}

bool scidBaseT::createSortCache(const char* criteria) {
	if (auto sc = getSortCache(criteria)) {
		sc->incrRef(1);
		return true;
	}
	return false;
}

size_t scidBaseT::listGames(const char* criteria, size_t start, size_t count,
                            const HFilter& filter, gamenumT* destCont) {
	const SortCache* sc = getSortCache(criteria);
	if (sc == NULL)
		return 0;

	return sc->select(start, count, filter, destCont);
}

size_t scidBaseT::sortedPosition(const char* criteria, const HFilter& filter,
                                 gamenumT gameId) {
	ASSERT(filter != NULL && filter->size() <= numGames());

	if (gameId >= numGames() || filter->get(gameId) == 0)
		return INVALID_GAMEID;

	SortCache* sc = getSortCache(criteria);
	if (sc == NULL)
		return INVALID_GAMEID;

	return sc->sortedPosition(gameId, filter);
}
