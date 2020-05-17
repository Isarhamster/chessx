/*
 * Copyright (C) 2011  Gerd Lorscheid
 * Copyright (C) 2011-2017  Fulvio Benini
 *
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
 * Defines the SortCache class, which sorts the games of an @e Index.
 */

#ifndef SORTCACHE_H
#define SORTCACHE_H

#include "common.h"

#ifndef MULTITHREADING_OFF
#include <atomic>
using std::atomic_bool;
#else
typedef bool atomic_bool;
#endif

class HFilter;
class Index;
class IndexEntry;
class NameBase;

/**
 * This class sorts games contained into an Index.
 * Multiple SortCache objects can be created for a single Index, allowing to
 * simultaneously sort the games by multiple criteria in an independent way.
 */
class SortCache {
	gamenumT nGames_;
	atomic_bool valid_fullMap_;
	atomic_bool th_interrupt_;
	bool partialHash_;
	gamenumT* fullMap_;
	void* th_;
	uint32_t* hash_;
	const Index* index_;
	const NameBase* nbase_;
	char criteria_[32];
	int refCount_;

	// Valid fields that can be used to sort the games.
	enum {
		SORTING_date =         'd',
		SORTING_year =         'y',
		SORTING_event =        'e',
		SORTING_site =         's',
		SORTING_round =        'n',
		SORTING_white =        'w',
		SORTING_black =        'b',
		SORTING_eco =          'o',
		SORTING_result =       'r',
		SORTING_moveCount =    'm',
		SORTING_avgElo =       'R',
		SORTING_country =      'c',
		SORTING_deleted =      'D',
		SORTING_eventdate =    'E',
		SORTING_whiteelo =     'W',
		SORTING_blackelo =     'B',
		SORTING_commentcount = 'C',
		SORTING_varcount =     'V',
		SORTING_nagcount =     'A',
		SORTING_resultwin =    '1',
		SORTING_resultdraw =   '5',
		SORTING_resultloss =   '0',
		SORTING_rating =       'i',
		SORTING_number =       'N',
		SORTING_sentinel =    '\0'
	};

public:
	/**
	 * Create a new SortCache object, builds the hash table, and asynchronously
	 * sorts all the games.
	 * @param idx:      valid pointer to an Index object, witch contains the
	 *                  header data of the games to be sorted.
	 * @param nb:       valid pointer to the NameBase corresponding to @e idx.
	 * @param criteria: the list of fields by which games will be ordered.
	 *                  Each field should be followed by '+' to indicate an
	 *                  ascending order or by '-' for a descending order.
	 * @returns a pointer to the new object in case of success, NULL otherwise.
	 */
	static SortCache* create(const Index* idx, const NameBase* nb,
	                         const char* criteria);
	~SortCache();

	/**
	 * Notify the object that a game's header data has changed.
	 * @param gameId: the id of the game whose data has been changed.
	 */
	void checkForChanges(gamenumT gameId);

	/**
	 * Interrupt any asynchronous operation. This function must be called before
	 * modifying the Index or the NameBase associated with the SortCache.
	 */
	void prepareForChanges() { th_interrupt(); }

	/**
	 * Retrieve the sorted list of games' ids.
	 * The behavior of this function is similar to the mySQL statement:
	 *     SELECT gameId FROM idx WHERE filter(gameId) != 0 ORDER BY criteria
	 *     LIMIT offset, row_count
	 * @param row_offset:  the offset of the first row to return.
	 *                     The offset of the initial row is 0.
	 * @param row_count:   maximum number of rows to return.
	 * @param filter:      a reference to a valid (!= NULL) HFilter object.
	 *                     Games not included into the filter will be ignored.
	 * @param[out] result: valid pointer to an array where the sorted list of
	 *                     games will be stored (should be able to contain at
	 *                     least @e row_count elements).
	 * @returns the number of games' ids stored into @e result.
	 */
	size_t select(size_t row_offset, size_t row_count, const HFilter& filter,
	              gamenumT* result) const;

	/**
	 * Get the sorted position of a game.
	 * @param gameId: the id of the game.
	 * @param filter: a reference to a valid (!= NULL) HFilter object.
	 *                Games not included into the filter will be ignored,
	 *                and @e gameId must be included into the filter.
	 * @returns the sorted position of @e gameId.
	 */
	size_t sortedPosition(gamenumT gameId, const HFilter& filter) const;

	int incrRef(int incr) { return refCount_ += incr; }

private:
	SortCache(const Index* idx, const NameBase* nbase);
	SortCache(const SortCache&);
	SortCache& operator=(const SortCache&);

	class CmpLess {
		const SortCache* sc_;
	public:
		CmpLess(const SortCache* sc) : sc_(sc) {}
		bool operator()(gamenumT g1, gamenumT g2) const;
	};
	int fullCompare(gamenumT left, gamenumT right) const;

	uint32_t calcHash(gamenumT gameId);
	void generateHashCache();
	void sortAsynchronously();

	void th_interrupt() {
		th_interrupt_ = true;
		th_join();
		th_interrupt_ = false;
	}
	void th_join();
	void th_sort();
};

#endif
