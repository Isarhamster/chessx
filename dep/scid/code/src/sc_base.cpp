/*
 * Copyright (C) 2015-2019 Fulvio Benini

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
#include "dbasepool.h"
#include "misc.h"
#include "scidbase.h"
#include "searchtournaments.h"
#include "ui.h"
#include <algorithm>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace {
/*
* This "sc_base" functions are used by the UI to access the databases.
* To encapsulate database internal complexity this functions should only
* parse arguments and call other functions/objects.
*
* Command names are case sensitive
* Optional parameter are indicated using [value_opt]
* Alternative values are indicated using <value_a|value_b|value_c>
* BaseId is the handle used to select the database to work on
* "dbfilter" is the name of the default filter of database
* Games are numbered starting from "1"
* Example:
*     sc_base open "filename" -> on success returns a baseId handle to the opened database
*     sc_base gameslist baseId "1" "10" "dbfilter" "N+"
*     -> returns the list of the first 10 games of the database
*/


/**
 * doOpenBase() - open/create a database
 * @filename:    the filename of the database to open/create
 * @fMode:       open the database read-only|read-write|create|in_memory
 * @codec:       the type of the database
 *
 * If @fMode == FMODE_Both, and the file cannot be opened for writing, this
 * function will try to open the database read-only.
 */
static UI_res_t doOpenBase(UI_handle_t ti, const char* codec, fileModeT fMode,
                           const char* filename) {
	if (DBasePool::find(filename))
		return UI_Result(ti, ERROR_FileInUse);

	scidBaseT* dbase = DBasePool::getFreeSlot();
	if (!dbase)
		return UI_Result(ti, ERROR_Full);

	Progress progress = UI_CreateProgress(ti);
	errorT err = dbase->open(codec, fMode, filename, progress);

	if ((err == ERROR_FileOpen || err == ERROR_FileMode) &&
	    fMode == FMODE_Both) {
		err = dbase->open(codec, FMODE_ReadOnly, filename, progress);
	}
	progress.report(1, 1);

	if (err != OK && err != ERROR_NameDataLoss)
		return UI_Result(ti, err);

	int res = DBasePool::switchCurrent(dbase);
	return UI_Result(ti, err, res);
}


/**
 * sc_base_close() - close a database
 */
UI_res_t sc_base_close(scidBaseT* dbase, UI_handle_t ti, int, const char**) {
	if (dbase->getFileName() == "<clipbase>") {
		return UI_Result(ti, ERROR_BadArg, "Cannot close clipbase.");
	}
	return UI_Result(ti, dbase->Close());
}


/**
 * sc_base_compact() - compact a database
 * @stats: if used do not compact the database but calculate stats
 *
 * Compacting a database:
 * - remove all the games marked as "deleted"
 * - remove unused names
 * - makes search operations faster
 * - replace bad names with "???"
 */
UI_res_t sc_base_compact(scidBaseT* dbase, UI_handle_t ti, int argc,
                         const char** argv) {
	const char* usage = "Usage: sc_base compact baseId [stats]";

	if (argc == 3) {
		errorT res = dbase->compact(UI_CreateProgress(ti));
		return UI_Result(ti, res);
	} else if (argc == 4 && std::strcmp("stats", argv[3]) == 0) {
		unsigned long long n_deleted, n_unused, n_sparse, n_badNameId;
		errorT res = dbase->getCompactStat(&n_deleted, &n_unused, &n_sparse,
		                                   &n_badNameId);
		UI_List val(4);
		val.push_back(n_deleted);
		val.push_back(n_unused);
		val.push_back(n_sparse);
		val.push_back(n_badNameId);
		return UI_Result(ti, res, val);
	}

	return UI_Result(ti, ERROR_BadArg, usage);
}


/**
 * sc_base_copygames() - copy games from a database to another
 */
UI_res_t sc_base_copygames(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base copygames baseId <gameNum|filterName> targetBaseId";
	if (argc != 5) return UI_Result(ti, ERROR_BadArg, usage);

	scidBaseT* targetBase = DBasePool::getBase(strGetUnsigned(argv[4]));
	if (targetBase == 0)
		return UI_Result(ti, ERROR_BadArg, "sc_base copygames error: wrong targetBaseId");

	errorT err = OK;
	const HFilter filter = dbase->getFilter(argv[3]);
	if (filter != 0) {
		err = targetBase->importGames(dbase, filter, UI_CreateProgress(ti));
	} else {
		uint gNum = strGetUnsigned(argv[3]);
		const IndexEntry* ie = (gNum > 0)
		                           ? dbase->getIndexEntry_bounds(gNum - 1)
		                           : nullptr;
		if (ie == nullptr)
			return UI_Result(
			    ti, ERROR_BadArg,
			    "sc_base copygames error: wrong <gameNum|filterName>");

		Game game;
		err = dbase->getGame(*ie, game);
		if (err == OK) {
			err = targetBase->saveGame(&game);
		}
	}
	return UI_Result(ti, err);
}


/**
 * sc_base_create() - crate a database
 * @codec : the type of the database
 * @filename: the filename of the wanted database.
 *            Database in native Scid format do not use extension ("example").
 *            Other databases require file extension ("example.pgn").
 *
 * Return:
 * - on success the handle assigned to the database
 */
UI_res_t sc_base_create(UI_handle_t ti, int argc, const char** argv) {
	const char* usage = "Usage: sc_base create <MEMORY|SCID4|PGN> filename";

	if (argc == 3) // Old interface defaults to SCID4
		return doOpenBase(ti, "SCID4", FMODE_Create, argv[2]);

	if (argc == 4)
		return doOpenBase(ti, argv[2], FMODE_Create, argv[3]);

	return UI_Result(ti, ERROR_BadArg, usage);
}


/**
 * sc_base_extra() - get/set a database extra tag (i.e. "description" or "type")
 */
UI_res_t sc_base_extra(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base extra baseId [tagname new_value]";

	if (argc == 3) {
		auto list = dbase->getExtraInfo();
		UI_List res(list.size() * 2);
		for (const auto& e : list) {
			res.push_back(e.first);
			res.push_back(e.second.c_str());
		}
		return UI_Result(ti, OK, res);
	}
	if (argc == 5) {
		return UI_Result(ti, dbase->setExtraInfo(argv[3], argv[4]));
	}

	return UI_Result(ti, ERROR_BadArg, usage);
}


/**
 * sc_base_filename() - get the filename of a database
 *
 * Return:
 *   the full path filename for non native databases (like pgn)
 *   the full path filename without the .si4 extension for native Scid databases
 *   <clipbase> for the clipbase
 */
UI_res_t sc_base_filename(scidBaseT* dbase, UI_handle_t ti, int argc,
                          const char**) {
	const char* usage = "Usage: sc_base filename baseId";
	if (argc != 3) return UI_Result(ti, ERROR_BadArg, usage);

	return UI_Result(ti, OK, dbase->getFileName());
}


/**
 * sc_base_gameflag() - get/set a game flag (i.e. "D" for deleted)
 *
 * Return: a boolean value if the requested operation is "get"
 */
UI_res_t sc_base_gameflag(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base gameflag baseId <gameNum|filterName|all> <get|set|unset|invert> flagType";
	if (argc != 6) return UI_Result(ti, ERROR_BadArg, usage);

	int cmd = 0;
	if (std::strcmp("get", argv[4]) == 0)
		cmd = 1;
	else if (std::strcmp("set", argv[4]) == 0)
		cmd = 2;
	else if (std::strcmp("unset", argv[4]) == 0)
		cmd = 3;
	else if (std::strcmp("invert", argv[4]) == 0)
		cmd = 4;
	uint flagType = IndexEntry::CharToFlagMask(argv[5][0]);
	if (flagType != 0 && cmd != 0) {
		Filter filter_all(dbase->numGames());
		const HFilter filter = std::strcmp("all", argv[3]) == 0
		                           ? HFilter(&filter_all)
		                           : dbase->getFilter(argv[3]);
		if (filter != 0) {
			switch (cmd) {
			case 2: return UI_Result(ti, dbase->setFlags(true, flagType, filter));
			case 3: return UI_Result(ti, dbase->setFlags(false, flagType, filter));
			case 4: return UI_Result(ti, dbase->invertFlags(flagType, filter));
			}
		} else {
			gamenumT gNum = strGetUnsigned(argv[3]);
			if (gNum > 0 && gNum <= dbase->numGames()) {
				gNum--;
				switch (cmd) {
				case 1: return UI_Result(ti, OK, dbase->getFlag(flagType, gNum));
				case 2: return UI_Result(ti, dbase->setFlag(true, flagType, gNum));
				case 3: return UI_Result(ti, dbase->setFlag(false, flagType, gNum));
				case 4: return UI_Result(ti, dbase->invertFlag(flagType, gNum));
				}
			}
		}
	}

	return UI_Result(ti, ERROR_BadArg, usage);
}


/**
 * sc_base_gamelocation() - find the position of a game
 * @sortCrit:      the order for the list of games to search
 * @gnumber:       the number of the game to search for
 * @text:          if (@gnumber == 0) search the first game that contains @text in
 *                 white name or black name or event name or site name
 * @start_pos:     used only with @text, start searching from a zero-based position.
 * @forward_dir:   <true|false>, used only with @start_pos.
 *                 if @e true search in the range [start_pos, numGames)
 *                 if @e false search in the range [0, start_pos)
 *
 * Return: the position (0 == first) of the first match or "none" if not found
 */
UI_res_t sc_base_gamelocation(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base gamelocation baseId filterName sortCrit <gnumber | 0 text start_pos forward_dir>";
	if (argc < 6) return UI_Result(ti, ERROR_BadArg, usage);

	const HFilter filter = dbase->getFilter(argv[3]);
	if (filter == 0) return UI_Result(ti, ERROR_BadArg, usage);

	const char* sort = argv[4];
	gamenumT gnumber = strGetUnsigned(argv[5]);
	size_t location = INVALID_GAMEID;
	if (gnumber == 0) {
		if (argc != 9)
			return UI_Result(ti, ERROR_BadArg, usage);
		const char* text = argv[6];
		size_t start = strGetUnsigned(argv[7]);
		const NameBase* nb = dbase->getNameBase();
		auto contains = [dbase, nb, text](gamenumT g) {
			const IndexEntry* ie = dbase->getIndexEntry(g);
			return strAlphaContains(ie->GetWhiteName(nb), text) ||
			       strAlphaContains(ie->GetBlackName(nb), text) ||
			       strAlphaContains(ie->GetEventName(nb), text) ||
			       strAlphaContains(ie->GetSiteName(nb), text);
		};
		if (strGetBoolean(argv[8])) {
			std::vector<gamenumT> buf(
			    start >= filter->size() ? 1 : filter->size() - start);
			buf.resize(
			    dbase->listGames(sort, start, buf.size(), filter, buf.data()));
			auto it = std::find_if(buf.begin(), buf.end(), contains);
			if (it != buf.end())
				location = start + std::distance(buf.begin(), it);
		} else {
			std::vector<gamenumT> buf(start ? start : 1);
			buf.resize(dbase->listGames(sort, 0, start, filter, buf.data()));
			auto it = std::find_if(buf.rbegin(), buf.rend(), contains);
			if (it != buf.rend())
				location = std::distance(it, buf.rend()) - 1;
		}
	} else {
		location = dbase->sortedPosition(sort, filter, gnumber - 1);
	}
	if (location == INVALID_GAMEID)
		return UI_Result(ti, OK, "none"); // Not found
	return UI_Result(ti, OK, location);
}


/**
 * sc_base_gameslist() - returns the sorted list of games of a database
 * @start:    0 for the first game
 */
UI_res_t sc_base_gameslist(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base gameslist baseId start count filterName sortCrit";
	if (argc != 7) return UI_Result(ti, ERROR_BadArg, usage);

	size_t start = strGetUnsigned(argv[3]);
	size_t count = strGetUnsigned(argv[4]);
	const HFilter filter = dbase->getFilter(argv[5]);
	if (filter == NULL)
		return UI_Result(ti, ERROR_BadArg, usage);
	gamenumT* idxList = new gamenumT[count];
	count = dbase->listGames(argv[6], start, count, filter, idxList);

	UI_List res (count * 3);
	UI_List ginfo(24);
	const NameBase* nb = dbase->getNameBase();
	for (uint i = 0; i < count; ++i) {
		uint idx = idxList[i];

		ASSERT(filter->get(idx) != 0);
		uint ply = filter->get(idx) -1;

		const IndexEntry* ie = dbase->getIndexEntry(idx);

		ginfo.clear();
		ginfo.push_back(idx +1);
		ginfo.push_back(RESULT_STR[ie->GetResult()]);
		ginfo.push_back((ie->GetNumHalfMoves() + 1) / 2);
		ginfo.push_back(ie->GetWhiteName(nb));
		std::string eloStr;
		eloT welo = ie->GetWhiteElo();
		if (welo != 0) {
			eloStr = std::to_string(welo);
		} else {
			welo = ie->GetWhiteElo(nb);
			eloStr = std::to_string(welo);
			if (welo != 0) {
				eloStr.insert(eloStr.begin(), '(');
				eloStr.insert(eloStr.end(), ')');
			}
		}
		ginfo.push_back(eloStr);
		ginfo.push_back(ie->GetBlackName(nb));
		eloT belo = ie->GetBlackElo();
		if (belo != 0) {
			eloStr = std::to_string(belo);
		} else {
			belo = ie->GetBlackElo(nb);
			eloStr = std::to_string(belo);
			if (belo != 0) {
				eloStr.insert(eloStr.begin(), '(');
				eloStr.insert(eloStr.end(), ')');
			}
		}
		ginfo.push_back(eloStr);
		char buf_date[16];
		date_DecodeToString (ie->GetDate(), buf_date);
		ginfo.push_back(buf_date);
		ginfo.push_back(ie->GetEventName(nb));
		ginfo.push_back(ie->GetRoundName(nb));
		ginfo.push_back(ie->GetSiteName(nb));
		ginfo.push_back(ie->GetNagCount());
		ginfo.push_back(ie->GetCommentCount());
		ginfo.push_back(ie->GetVariationCount());
		char deleted[2] = {0};
		deleted[0] = (ie->GetDeleteFlag()) ? 'D' : ' ';
		ginfo.push_back(deleted);
		char flags[16];
		ie->GetFlagStr (flags, "WBMENPTKQ!?U123456");
		ginfo.push_back(flags);
		ecoStringT ecoStr;
		eco_ToExtendedString (ie->GetEcoCode(), ecoStr);
		ginfo.push_back(ecoStr);
		std::string endMaterial = matsig_makeString(ie->GetFinalMatSig());
		ginfo.push_back(endMaterial);
		char startpos[2] = {0};
		startpos[0] = (ie->GetStartFlag()) ? 'S' : ' ';
		ginfo.push_back(startpos);
		char buf_eventdate[16];
		date_DecodeToString (ie->GetEventDate(), buf_eventdate);
		ginfo.push_back(buf_eventdate);
		ginfo.push_back(ie->GetYear());
		ginfo.push_back((welo + belo)/2);
		ginfo.push_back(ie->GetRating(nb));
		ginfo.push_back(dbase->getGame(ie).getMoveSAN(ply, 10));

		res.push_back(std::to_string(idx+1) + "_" + std::to_string(ply));
		res.push_back(ginfo);
		res.push_back(deleted);
	}

	delete [] idxList;
	return UI_Result(ti, OK, res);
}

static UI_res_t sc_base_getGameHelper(UI_handle_t ti, Game& game) {
	auto positions = gamepos::collectPositions(game);
	UI_List res(positions.size());
	UI_List posInfo(6);
	for (const auto& pos : positions) {
		posInfo.clear();
		posInfo.push_back(pos.RAVdepth);
		posInfo.push_back(pos.RAVnum);
		posInfo.push_back(pos.FEN);
		std::string nags;
		for (const auto& nag : pos.NAGs) {
			char temp[16];
			game_printNag(nag, temp, true, PGN_FORMAT_Plain);
			if (!nags.empty())
				nags += ' ';
			nags += temp;
		}
		posInfo.push_back(nags);
		posInfo.push_back(pos.comment);
		posInfo.push_back(pos.lastMoveSAN);
		res.push_back(posInfo);
	}
	return UI_Result(ti, OK, res);
}

/**
 * sc_base_getGame() - return all the positions of a game
 * @param gameNum: the number of the requested game
 * @param live: optional parameter which specify the behavior when the requested
 * game have unsaved changes. If present the latest unsaved version is returned,
 * otherwise the saved copy in the database is used.
 *
 * Return a list containing all the positions of a game, including variations.
 * The positions are sorted according to pgn standard:
 * "The alternate move sequence given by an RAV is one that may be legally
 * played by first unplaying the move that appears immediately prior to the
 * RAV". For each position the following informations are provided: RAVdepth:
 * current variation depth. RAVnum: current variation num. FEN: "Forsyth-Edwards
 * Notation" describing the current position. NAGS: "Numeric Annotation Glyph"
 * is a non-negative integer from 0 to 255 used to indicate a simple annotation
 * in a language independent manner. comment: text annotation of the
 * current position. lastMoveSAN: the last move that was played to reach the
 * current position. The move is indicated using English "Standard Algebraic
 * Notation".
 */
UI_res_t sc_base_getGame(scidBaseT* dbase, UI_handle_t ti, int argc,
                         const char** argv) {
	const char* usage = "Usage: sc_base getGame baseId gameNum [live]";
	bool live = (argc == 5 && std::strcmp("live", argv[4]) == 0);
	if (!live && argc != 4)
		return UI_Result(ti, ERROR_BadArg, usage);

	gamenumT gNum = strGetUnsigned(argv[3]);
	if (live && dbase->gameNumber == (static_cast<long long>(gNum) - 1)) {
		auto location = dbase->game->currentLocation();
		auto res = sc_base_getGameHelper(ti, *(dbase->game));
		dbase->game->restoreLocation(location);
		return res;
	}

	auto ie = (gNum > 0) ? dbase->getIndexEntry_bounds(gNum - 1) : nullptr;
	if (!ie)
		return UI_Result(ti, ERROR_BadArg, usage);

	Game game;
	errorT err = dbase->getGame(*ie, game);
	if (err != OK)
		return UI_Result(ti, err);
	return sc_base_getGameHelper(ti, game);
}

/**
 * sc_base_import() - import games from non-native database
 *
 * Return:
 *   On success, returns a list of two elements: the number of
 *   games imported and a string containing import errors
 *   or warnings.
 */
UI_res_t sc_base_import(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base import baseId filename";
	if (argc != 4) return UI_Result(ti, ERROR_BadArg, usage);

	// if (pgn)
	auto codec = ICodecDatabase::PGN;

	auto nImported = dbase->numGames();
	std::string errorMsg;
	auto res_imp =
	    dbase->importGames(codec, argv[3], UI_CreateProgress(ti), errorMsg);
	if (res_imp != OK)
		return UI_Result(ti, res_imp);

	UI_List res(2);
	res.push_back(dbase->numGames() - nImported);
	res.push_back(errorMsg);
	return UI_Result(ti, OK, res);
}


/**
 * sc_base_list() - return a baseId list of opened databases
 */
UI_res_t sc_base_list(UI_handle_t ti, int argc, const char**) {
	const char* usage = "Usage: sc_base list";
	if (argc != 2) return UI_Result(ti, ERROR_BadArg, usage);

	std::vector<int> l = DBasePool::getHandles();
	UI_List res(l.size());
	for (size_t i=0, n=l.size(); i < n; i++) res.push_back(l[i]);
	return UI_Result(ti, OK, res);
}


/**
 * sc_base_numGames() - return the number of games in the database
 */
UI_res_t sc_base_numGames(scidBaseT* dbase, UI_handle_t ti, int argc,
                          const char**) {
	const char* usage = "Usage: sc_base numGames baseId";
	if (argc != 3) return UI_Result(ti, ERROR_BadArg, usage);

	return UI_Result(ti, OK, dbase->numGames());
}


/**
 * sc_base_open() - open a database
 * @codec : the type of the database
 * @filename: the filename of the wanted database.
 *            Database in native Scid format do not use extension ("example").
 *            Other databases require file extension ("example.pgn").
 *
 * Return:
 * - on success or with ERROR_NameDataLoss: the handle assigned to the database
 */
UI_res_t sc_base_open(UI_handle_t ti, int argc, const char** argv) {
	const char* usage = "Usage: sc_base open <SCID4|PGN> filename";

	if (argc == 3) // Old interface defaults to SCID4
		return doOpenBase(ti, "SCID4", FMODE_Both, argv[2]);

	if (argc == 4)
		return doOpenBase(ti, argv[2], FMODE_Both, argv[3]);

	return UI_Result(ti, ERROR_BadArg, usage);
}


/**
 * sc_base_slot() - get the handle of an opened database
 * @filename: the filename of the wanted database.
 *            Database in native Scid format do not use extension ("example").
 *            Other databases require file extension ("example.pgn").
 *
 * Return:
 * - the handle of the database corresponding to @filename.
 * - 0 if not found.
 */
UI_res_t sc_base_slot(UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base slot filename";
	if (argc != 3) return UI_Result(ti, ERROR_BadArg, usage);

	int res = DBasePool::find(argv[2]);
	return UI_Result(ti, OK, res);
}


/**
 * sc_base_sortcache() - create/release a sortcache
 *
 * A sortchace is used to speed up the other sc_base functions with the same "sortCrit"
 */
UI_res_t sc_base_sortcache(scidBaseT* dbase, UI_handle_t ti, int argc, const char** argv)
{
	const char* usage = "Usage: sc_base sortcache baseId <create|release> sortCrit";
	if (argc != 5) return UI_Result(ti, ERROR_BadArg, usage);

	if (std::strcmp("create", argv[3]) == 0) {
		if (!dbase->createSortCache(argv[4]))
			return UI_Result(ti, ERROR);
	} else {
		dbase->releaseSortCache(argv[4]);
	}
	return UI_Result(ti, OK);
}


/**
 * sc_base_stats() - return statistics about a database.
 *
 * The returned values are specific for every subcomand:
 * dates   -> {minimum year} {maximum year} {mean year}
 * eco ?   -> {number of games with for the ? eco} {white wins} {draws}
 *            {black wins} {n_games with empty result} {score in %}
 *            ? must be a valid ECO code or an abbreviation, for example:
 *            A returns the sum of all the A00,A00a...A99z4
 *            A1 returns the sum of all the A10,A10a,A10a1...A19z4
 *            A10 returns the sum of all the A10,A10a,A10a1...A10z4
 *            An empty string returns the sum of all valid ECO codes
 * flag ?  -> {number of games with the ? flag set}
 *            ? must be a valid flag char (see IndexEntry::CharToFlag())
 * flags   -> {n_games with D flag set} {n_games with W flag set} {n_games with B flag set}
 * ratings -> {minimum elo} {maximum elo {mean elo}
 * results -> {number of games won by white} {number of draws} {number of
 *             games won by black} {number of games with no result}
 */
UI_res_t sc_base_stats(const scidBaseT* dbase, UI_handle_t ti, int argc, const char ** argv)
{
	const char* usage = "Usage: sc_base stats baseId <dates|eco ?|flag ?|flags|ratings|results>";
	if (argc < 4) return UI_Result(ti, ERROR_BadArg, usage);

	const char* subcmd = argv[3];
	const scidBaseT::Stats& stats = dbase->getStats();
	UI_List res(6);

	enum { OPT_DATE, OPT_ECO, OPT_FLAG, OPT_FLAGS, OPT_RATINGS, OPT_RESULTS };
	const char * options[] = { "dates", "eco", "flag", "flags", "ratings", "results", NULL };
	switch (strExactMatch(subcmd, options)) {
	case OPT_DATE:
		res.push_back(date_GetYear(stats.minDate));
		res.push_back(date_GetYear(stats.maxDate));
		res.push_back(stats.nYears == 0 ? 0 : size_t(stats.sumYears / stats.nYears));
		break;
	case OPT_ECO: {
		const scidBaseT::Stats::Eco* eco = (argc != 5) ? 0 : stats.getEcoStats(argv[4]);
		if (eco == 0) return UI_Result(ti, ERROR_BadArg, usage);
		res.push_back(eco->count);
		res.push_back(eco->results[RESULT_White]);
		res.push_back(eco->results[RESULT_Draw]);
		res.push_back(eco->results[RESULT_Black]);
		res.push_back(eco->results[RESULT_None]);
		ASSERT(eco->count >= eco->results[RESULT_None]);
		uint count = eco->count - eco->results[RESULT_None];
		uint score = eco->results[RESULT_White] * 2;
		score += eco->results[RESULT_Draw];
		score *= 500;
		res.push_back(count == 0 ? 0.0 : score / count / 10.0);
		break; }
	case OPT_FLAG: {
		uint flag = (argc != 5) ? 0 : IndexEntry::CharToFlag(*(argv[4]));
		if (flag == 0) return UI_Result(ti, ERROR_BadArg, usage);
		res.push_back(stats.flagCount[flag]);
		break; }
	case OPT_FLAGS:
		res.push_back(stats.flagCount[IndexEntry::CharToFlag('D')]);
		res.push_back(stats.flagCount[IndexEntry::CharToFlag('W')]);
		res.push_back(stats.flagCount[IndexEntry::CharToFlag('B')]);
		break;
	case OPT_RATINGS:
		res.push_back(stats.minRating);
		res.push_back(stats.maxRating);
		res.push_back(stats.nRatings == 0 ? 0 : size_t(stats.sumRatings / stats.nRatings));
		break;
	case OPT_RESULTS:
		res.push_back(stats.nResults[RESULT_White]);
		res.push_back(stats.nResults[RESULT_Draw]);
		res.push_back(stats.nResults[RESULT_Black]);
		res.push_back(stats.nResults[RESULT_None]);
		break;
	default:
		return UI_Result(ti, ERROR_BadArg, usage);
	}

	return UI_Result(ti, OK, res);
}


/**
 * sc_base_switch() - change the current database and the current game
 *
 * DEPRECATED
 * Unfortunately Scid used to have only one database, one game, one filter, etc...
 * This function changes the current database and consequently the current game 
 * (sc_game functions works on the current game)
 *
 * Return: the current database ID after the switch
 */
UI_res_t sc_base_switch(scidBaseT* dbase, UI_handle_t ti)
{
	int res = DBasePool::switchCurrent(dbase);
	return UI_Result(ti, OK, res);
}

/// Remove all occurrences of the specified tags from the database.
/// @returns the number of changed games.
UI_res_t sc_base_strip(scidBaseT& dbase, UI_handle_t ti, int argc,
                       const char** argv) {
	const char* usage = "Usage: sc_base strip baseId tagNames...";
	if (argc < 4)
		return UI_Result(ti, ERROR_BadArg, usage);

	Filter filter_all(dbase.numGames());
	auto progress = UI_CreateProgress(ti);
	const auto res = dbase.transformGames(
	    HFilter(&filter_all), progress, [&](Game& game) {
		    bool modified = false;
		    for (int i = 3; i < argc; i++) {
			    const bool removed = game.RemoveExtraTag(argv[i]);
			    modified = modified || removed;
		    }
		    return modified;
	    });
	return UI_Result(ti, res.first, res.second);
}

/// Produce a list of PGN tags used in the database
/// @returns a even-sized list, where each pair of elements is a tag name and
/// its frequency
UI_res_t sc_base_taglist(scidBaseT& dbase, UI_handle_t ti) {
	static_assert(gamenumT() == 0);
	std::map<std::string, gamenumT> tag_freq;
	Filter filter_all(dbase.numGames());
	auto progress = UI_CreateProgress(ti);
	const auto err = dbase.transformGames(
	    HFilter(&filter_all), progress, [&tag_freq](const Game& game) {
		    for (const auto& tag : game.GetExtraTags()) {
			    tag_freq[tag.first] += 1;
		    }
		    return false;
	    });
	if (err.first != OK)
		return UI_Result(ti, err.first);

	UI_List res(tag_freq.size() * 2);
	for (const auto& [tag, freq] : tag_freq) {
		if (tag != "SetUp") {
			res.push_back(tag.c_str());
			res.push_back(freq);
		}
	}
	return UI_Result(ti, OK, res);
}

/**
 * sc_base_tournaments() - return a list of tournaments
 *
 * Games with the same [Event], [Site] and [EventDate] tags are considered
 * a tournament (also games with empty [EventDate] tag are included).
 * The list returned can be restricted based on the average elo of the
 * participants, the number of games, the number of players and the name
 * of a participant.
 * Ranges can be specified with one or two numbers separated by spaces (min max).
 * The returned list can be sorted according to the following criteria:
 * - "Date"    : date of the first games in the tournament
 * - "Players" : number of participants to the tournament
 * - "Games"   : number of games
 * - "Elo"     : average elo of the participants
 * - "Site"    : site name
 * - "Event"   : event name
 *
 * Return:
 *   On success, return a list of tournaments.
 *   For each tournament the following info are provided:
 *   date of the first games, site name, event name, number of players,
 *   number of games, average elo of the participants, lowest game number,
 *   winner name, winner elo, winner score, 2nd place name, 2nd elo, 2nd score.
 */
UI_res_t sc_base_tournaments(const scidBaseT* dbase, UI_handle_t ti, int argc, const char ** argv)
{
	const char* usage = "Usage: sc_base tournaments baseId filterName n_maxResults [-avgelo range] [-n_games range] [-n_players range] [-sort criteria] ";
	if (argc < 5) return UI_Result(ti, ERROR_BadArg, usage);

	const HFilter filter = dbase->getFilter(argv[3]);
	if (filter == 0) return UI_Result(ti, ERROR_BadArg, usage);

	SearchTournaments search(dbase, filter);

	const char* sortCriteria = 0;
	long nResults = strGetUnsigned(argv[4]);

	static const char* options[] = {
		"-avgelo", "-n_games", "-n_players", "-player", "-sort", NULL
	};
	enum { AVGELO, N_GAMES, N_PLAYERS, PLAYER, SORT };

	for (int i = 5; (i + 1) < argc; i += 2) {
		int index = strUniqueMatch(argv[i], options);
		const char* value = argv[i + 1];
		if (*value == 0) continue;
		switch (index) {
		case AVGELO:
			search.filterByAvgElo(StrRange(value));
			break;
		case N_GAMES:
			search.filterByNGames(StrRange(value));
			break;
		case N_PLAYERS:
			search.filterByNPlayers(StrRange(value));
			break;
		case PLAYER:
			search.filterByPlayer(value);
			break;
		case SORT:
			sortCriteria = value;
			break;
		default:
			return UI_Result(ti, ERROR_BadArg, usage);
		}
	}

	if (sortCriteria != 0) {
		if (!search.sort(sortCriteria, nResults))
			return UI_Result(ti, ERROR_BadArg, usage);
	}

	SearchTournaments::Iter it = search.begin();
	SearchTournaments::Iter it_end = search.end();
	if (std::distance(it, it_end) > nResults) {
		it_end = it + nResults;
	}

	char buf_date[16];
	const NameBase* nb = dbase->getNameBase();
	UI_List res(std::distance(it, it_end));
	UI_List tourney(14);
	for (; it != it_end; it++) {
		tourney.clear();
		date_DecodeToString(it->getStartDate(), buf_date);
		strTrimDate(buf_date);
		tourney.push_back(buf_date);
		tourney.push_back(nb->GetName(NAME_SITE, it->getSiteId()));
		tourney.push_back(nb->GetName(NAME_EVENT, it->getEventId()));
		tourney.push_back(it->nPlayers());
		tourney.push_back(it->nGames());
		tourney.push_back(it->getAvgElo());
		tourney.push_back(it->getStartGameNum() + 1);
		const char* name1st = "";
		eloT elo1st = 0;
		double score1st = 0.0;
		const char* name2nd = "";
		eloT elo2nd = 0;
		double score2nd = 0.0;
		if (it->nPlayers() > 0) {
			const Tourney::Player& p = it->getPlayer(0);
			name1st = nb->GetName(NAME_PLAYER, p.nameId);
			elo1st = p.elo;
			score1st = p.score / 2.0;
		}
		if (it->nPlayers() > 1) {
			const Tourney::Player& p = it->getPlayer(1);
			name2nd = nb->GetName(NAME_PLAYER, p.nameId);
			elo2nd = p.elo;
			score2nd = p.score / 2.0;
		}
		tourney.push_back(name1st);
		tourney.push_back(elo1st);
		tourney.push_back(score1st);
		tourney.push_back(name2nd);
		tourney.push_back(elo2nd);
		tourney.push_back(score2nd);

		res.push_back(tourney);
	}

	return UI_Result(ti, OK, res);
}

/**
 * sc_base_player_elo() - return a list of elo values of a player
 * Return:
 *   On success, return a list of years (the month is added in the fractional
 *   part: 2018.00 ==> gen 2018, 2018.75 ==> oct 2018, etc..) and elos.
 */
UI_res_t sc_base_player_elo(const scidBaseT& dbase, UI_handle_t ti, int argc,
                            const char** argv) {
	const char* usage = "Usage: sc_base player_elo baseId filterName playerName";
	if (argc != 5)
		return UI_Result(ti, ERROR_BadArg, usage);

	const HFilter filter = dbase.getFilter(argv[3]);
	if (filter == nullptr)
		return UI_Result(ti, ERROR_BadArg, usage);

	idNumberT id = 0;
	if (dbase.getNameBase()->FindExactName(NAME_PLAYER, argv[4], &id) != OK)
		return UI_Result(ti, OK);

	std::map<unsigned, eloT> eloByMonth;
	auto getPlayerElo = [](auto idx_entry, auto player_id) -> eloT {
		ASSERT(idx_entry);
		if (idx_entry->GetWhite() == player_id)
			return idx_entry->GetWhiteElo();
		if (idx_entry->GetBlack() == player_id)
			return idx_entry->GetBlackElo();
		return 0;
	};
	for (auto gnum : filter) {
		const IndexEntry* ie = dbase.getIndexEntry(gnum);
		if (auto elo = getPlayerElo(ie, id)) {
			auto date = ie->GetDate();
			auto year = date_GetYear(date);
			auto month = date_GetMonth(date);
			if (month > 0) {
				month--;
			}
			if (month > 11) {
				month = 0;
			}
			eloByMonth[year * 12 + month] = elo;
		}
	}

	UI_List res(eloByMonth.size() * 2);
	for (auto& e : eloByMonth) {
		res.push_back(e.first / 12.0); // year
		res.push_back(e.second);       // elo
	}
	return UI_Result(ti, OK, res);
}

} // End of anonymous namespace

//TODO: move this function here from tkscid.cpp
UI_res_t sc_base_inUse       (UI_extra_t, UI_handle_t, int argc, const char ** argv);
UI_res_t sc_base_export      (UI_extra_t, UI_handle_t, int argc, const char ** argv);
UI_res_t sc_base_piecetrack  (UI_extra_t, UI_handle_t, int argc, const char ** argv);
UI_res_t sc_base_duplicates  (scidBaseT* dbase, UI_handle_t, int argc, const char ** argv);
UI_res_t sc_base_gamesummary (const scidBaseT& dbase, UI_handle_t, int, const char**);


UI_res_t sc_base (UI_extra_t cd, UI_handle_t ti, int argc, const char ** argv)
{
	static const char * options [] = {
	    "close",           "compact",         "copygames",
	    "create",          "current",         "duplicates",
	    "export",          "extra",           "filename",        "gameflag",
	    "gamelocation",    "gameslist",       "getGame",         "import",
	    "inUse",           "isReadOnly",      "list",            "numGames",        "open",
	    "piecetrack",      "player_elo",      "slot",            "sortcache",       "stats",
	    "strip",           "switch",          "taglist",         "tournaments",     "type",
	    "gamesummary",
	    NULL
	};
	enum {
	    BASE_CLOSE,        BASE_COMPACT,      BASE_COPYGAMES,
	    BASE_CREATE,       BASE_CURRENT,      BASE_DUPLICATES,
	    BASE_EXPORT,       BASE_EXTRA,        BASE_FILENAME,     BASE_GAMEFLAG,
	    BASE_GAMELOCATION, BASE_GAMESLIST,    BASE_GETGAME,      BASE_IMPORT,
	    BASE_INUSE,        BASE_ISREADONLY,   BASE_LIST,         BASE_NUMGAMES,     BASE_OPEN,
	    BASE_PTRACK,       BASE_PLAYER_ELO,   BASE_SLOT,         BASE_SORTCACHE,    BASE_STATS,
	    BASE_STRIP,        BASE_SWITCH,       BASE_TAGLIST,      BASE_TOURNAMENTS,  BASE_TYPE,
	    BASE_GAMESUMMARY
	};

	if (argc <= 1) return UI_Result(ti, ERROR_BadArg, "Usage: sc_base <cmd>");

	int index = strUniqueMatch (argv[1], options);
	switch (index) {
	case BASE_CREATE:
		return sc_base_create(ti, argc, argv);

	case BASE_CURRENT:
		return sc_base_switch(0, ti);

	case BASE_EXPORT:
		return sc_base_export (cd, ti, argc, argv);

	case BASE_INUSE:
		return sc_base_inUse (cd, ti, argc, argv);

	case BASE_LIST:
		return sc_base_list(ti, argc, argv);

	case BASE_OPEN:
		return sc_base_open(ti, argc, argv);

	case BASE_PTRACK:
		return sc_base_piecetrack (cd, ti, argc, argv);

	case BASE_SLOT:
		return sc_base_slot (ti, argc, argv);
	}

	//New multi-base functions
	if (argc < 3) return UI_Result(ti, ERROR_BadArg, "Usage: sc_base <cmd> baseId [args]");
	scidBaseT* dbase = DBasePool::getBase(strGetUnsigned(argv[2]));
	if (dbase == 0) return UI_Result(ti, ERROR_FileNotOpen);

	switch (index) {
	case BASE_CLOSE:
		return sc_base_close(dbase, ti, argc, argv);

	case BASE_COMPACT:
		return sc_base_compact(dbase, ti, argc, argv);

	case BASE_COPYGAMES:
		return sc_base_copygames(dbase, ti, argc, argv);

	case BASE_DUPLICATES:
		return sc_base_duplicates(dbase, ti, argc, argv);

	case BASE_EXTRA:
		return sc_base_extra(dbase, ti, argc, argv);

	case BASE_FILENAME:
		return sc_base_filename(dbase, ti, argc, argv);

	case BASE_GAMEFLAG:
		return sc_base_gameflag(dbase, ti, argc, argv);

	case BASE_GAMELOCATION:
		return sc_base_gamelocation(dbase, ti, argc, argv);

	case BASE_GAMESLIST:
		return sc_base_gameslist(dbase, ti, argc, argv);

	case BASE_GETGAME:
		return sc_base_getGame(dbase, ti, argc, argv);

	case BASE_IMPORT:
		return sc_base_import (dbase, ti, argc, argv);

	case BASE_ISREADONLY:
		return UI_Result(ti, OK, dbase->isReadOnly());

	case BASE_NUMGAMES:
		return sc_base_numGames (dbase, ti, argc, argv);

	case BASE_PLAYER_ELO:
		return sc_base_player_elo(*dbase, ti, argc, argv);

	case BASE_SORTCACHE:
		return sc_base_sortcache(dbase, ti, argc, argv);

	case BASE_STATS:
		return sc_base_stats(dbase, ti, argc, argv);

	case BASE_SWITCH:
		return sc_base_switch (dbase, ti);

	case BASE_STRIP:
		return sc_base_strip(*dbase, ti, argc, argv);

	case BASE_TAGLIST:
		return sc_base_taglist(*dbase, ti);

	case BASE_TOURNAMENTS:
		return sc_base_tournaments (dbase, ti, argc, argv);

	case BASE_GAMESUMMARY:
		return sc_base_gamesummary(*dbase, ti, argc, argv);
	}

	std::string res = "sc_base\nInvalid minor command: ";
	res.append(argv[1]);
	return UI_Result(ti, ERROR_BadArg, res);
}
