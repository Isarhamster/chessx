/*
# Copyright (C) 2016 Fulvio Benini

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

#include "dbasepool.h"
#include "scidbase.h"
#include "ui.h"

namespace {
/*
* Filters are used to select games, usually when performing searches.
* To encapsulate database internal complexity this functions should only
* parse arguments and call other functions/objects.
*
* Command names are case sensitive
* Optional parameter are indicated using [value_opt]
* Alternative values are indicated using <value_a|value_b|value_c>
* BaseId is the handle used to select the database to work on
* filterId is the unique ID of the filter to work on
* Games are numbered starting from "1"
*
* Every database has a default filter with id "dbfilter".
*/

/// @baseId: valid database identifier
/// @filterId: valid identifier of a filter.
/// @return the ids of the "main" and "mask" filters of the composed filter
///         @e filterId (if it is not composed returns @filterId and empty).
UI_res_t sc_filter_components(UI_handle_t ti, const scidBaseT& dbase, int argc,
                              const char** argv) {
	const char* usage = "Usage: sc_filter components baseId filterId";
	if (argc != 4)
		return UI_Result(ti, ERROR_BadArg, usage);

	auto filters = dbase.getFilterComponents(argv[3]);
	UI_List res(2);
	res.push_back(filters.first);
	res.push_back(filters.second);
	return UI_Result(ti, OK, res);
}

/**
 * sc_filter_compose() - compose a new filter
 * @baseId: valid database identifier
 * @filterId: valid identifier of the "main" filter. If it is already a composed
 *            one, only the "main" part will be used.
 * @maskfilterId: identifier of the "mask" Filter, can be an empty "" string.
 *
 * A composed filter include only the games contained in both @filterId and
 * @maskfilterId. This function allows to compose a new filter, if @filterId and
 * @maskfilterId are two valid filter identifiers, or to obtain the "main"
 * filter of a composed one, if @maskfilterId is an empty "" string.
 * A composed filter should not be released.
 * Return:
 * - the identifier of the composed filter, which can be used as a "normal"
 *   filter (modifying functions will affect only the "main" @filterId)
 * - on error, an empty string.
 */
UI_res_t sc_filter_compose(UI_handle_t ti, const scidBaseT& dbase, int argc,
                           const char** argv) {
	const char* usage = "Usage: sc_filter compose baseId filterId maskfilterId";
	if (argc != 5) return UI_Result(ti, ERROR_BadArg, usage);

	std::string res = dbase.composeFilter(argv[3], argv[4]);
	if (res.empty())
		return UI_Result(ti, ERROR_BadArg, "sc_filter: invalid filterId");

	return UI_Result(ti, OK, res);
}

/**
 * sc_filter_remove() - remove games from the filter
 * @gnumber: the id of the game to be removed
 * @+: also remove games after @gnumber
 * @-: also remove games before @gnumber
 * @sortCrit: criteria used to determine before/after
 */
UI_res_t sc_filter_remove(UI_handle_t ti, scidBaseT& dbase, HFilter& filter,
                          int argc, const char** argv) {
	const char* usage = "Usage: sc_filter remove baseId filterId gnumber [<+|-> sortCrit]";
	if (argc != 5 && argc != 7) return UI_Result(ti, ERROR_BadArg, usage);

	uint gNum = strGetUnsigned(argv[4]);
	if (gNum == 0 || gNum > dbase.numGames())
		return UI_Result(ti, ERROR_BadArg);

	if (argc == 5) {
		filter.erase(gNum - 1);
	} else {
		const char* crit = argv[6];
		size_t start = dbase.sortedPosition(crit, filter, gNum - 1);
		if (start == INVALID_GAMEID)
			return UI_Result(ti, ERROR_BadArg, usage);

		size_t count;
		switch (argv[5][0]) {
		case '+':
			count = filter->size() - start;
			break;
		case '-':
			count = start + 1;
			start = 0;
			break;
		default:
			return UI_Result(ti, ERROR_BadArg, usage);
		}

		gamenumT* idxList = new gamenumT[count];
		count = dbase.listGames(crit, start, count, filter, idxList);
		for (size_t i = 0; i < count; ++i) {
			filter.erase(idxList[i]);
		}
		delete[] idxList;
	}
	return UI_Result(ti, OK);
}

/**
 * sc_filter_reset() - reset a filter to be empty or include all the games
 * @full: reset the filter to include all the games
 * @empty: reset the filter to exclude all the games
 */
UI_res_t sc_filter_reset(UI_handle_t ti, HFilter& filter, int argc,
                         const char** argv) {
	const char* usage = "Usage: sc_filter reset baseId filterId <full|empty>";
	if (argc != 5) return UI_Result(ti, ERROR_BadArg, usage);

	if (strcmp("full", argv[4]) == 0) {
		filter->includeAll();
	} else if (strcmp("empty", argv[4]) == 0) {
		filter->clear();
	} else {
		return UI_Result(ti, ERROR_BadArg, usage);
	}
	return UI_Result(ti, OK);
}

/**
 * sc_filter_sizes() - get the sizes of a filter
 *
 * Return a list containing:
 * - the number of games included in the filter @filterId
 * - the total number of games contained in the database @baseId
 * - the number of games included in the "main" filter composing @filterId
 *   (if @filterId is not a combined filter, this value is equal to the first).
 */
UI_res_t sc_filter_sizes(UI_handle_t ti, const scidBaseT& dbase,
                         HFilter& filter) {
	// "Usage: sc_filter sizes baseId filterId";
	UI_List res(3);
	res.push_back(filter.size());
	res.push_back(dbase.numGames());
	res.push_back(filter.mainSize());
	return UI_Result(ti, OK, res);
}

} // End of anonymous namespace

int sc_filter_old(ClientData cd, Tcl_Interp* ti, int argc, const char** argv);

UI_res_t sc_filter(UI_extra_t cd, UI_handle_t ti, int argc, const char** argv) {
	const char* usage = "Usage: sc_filter <cmd> baseId filterId [args]";
	if (argc < 2) return UI_Result(ti, ERROR_BadArg, usage);

	static const char* options[] = {"components", "compose", "remove", "reset", "sizes", NULL};
	if (strUniqueMatch(argv[1], options) == - 1)
		return sc_filter_old(cd, ti, argc, argv);

	if (argc < 3)
		return UI_Result(ti, ERROR_BadArg, usage);

	scidBaseT* dbase = DBasePool::getBase(strGetUnsigned(argv[2]));
	if (!dbase)
		return UI_Result(ti, ERROR_BadArg, usage);

	HFilter filter = dbase->getFilter(argv[3]);
	if (filter == nullptr)
		return UI_Result(ti, ERROR_BadArg, usage);

	const char* cmd = argv[1];
	if (strcmp("components", cmd) == 0)
		return sc_filter_components(ti, *dbase, argc, argv);
	if (strcmp("compose", cmd) == 0)
		return sc_filter_compose(ti, *dbase, argc, argv);
	if (strcmp("remove", cmd) == 0)
		return sc_filter_remove(ti, *dbase, filter, argc, argv);
	if (strcmp("reset", cmd) == 0)
		return sc_filter_reset(ti, filter, argc, argv);
	if (strcmp("sizes", cmd) == 0)
		return sc_filter_sizes(ti, *dbase, filter);

	std::string err = "sc_filter\nInvalid minor command: ";
	return UI_Result(ti, ERROR_BadArg, err + cmd);
}
