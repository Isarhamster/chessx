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
 * Implements the SortCache class, which sorts the games of an @e Index.
 */

#include "sortcache.h"
#include "hfilter.h"
#include "index.h"
#include "misc.h"
#include <algorithm>
#include <climits>
#include <cstring>
#include <numeric>

#ifndef MULTITHREADING_OFF
#include <thread>

/**
 * Blocks the current thread until the thread @e th_ finishes its execution.
 */
void SortCache::th_join() {
	if (th_ != nullptr) {
		static_cast<std::thread*>(th_)->join();
		delete static_cast<std::thread*>(th_);
		th_ = nullptr;
	}
}

/**
 * Populate @e fullMap_ with the sorted ids of all the games contained into @e
 * index_. This function can run in a worker thread and can be interrupted.
 * It is necessary to invoke th_interrupt() or th_join() before modifying the
 * SortCache object, or the associated Index and NameBase objects.
 */
void SortCache::th_sort() {
	size_t nGames = this->nGames_;
	gamenumT* v = this->fullMap_;
	gamenumT* begin = v;
	gamenumT* end = v + nGames;
	auto comp = SortCache::CmpLess(this);

	std::iota(begin, end, 0);

	// An interruptible implementation of:
	// std::make_heap(v.begin(), v.end(), comp);
	ASSERT(nGames < size_t(INT_MAX / 2));
	const int lastNode = static_cast<int>(nGames) - 1;
	const auto lastRoot = (lastNode - 1) / 2;
	for (auto node = lastRoot; node >= 0; --node) {
		if (this->th_interrupt_)
			return;
		// Sift down @e node
		for (auto toSift = node;;) {
			const auto leftChild = 2 * toSift + 1;
			const auto rightChild = 2 * toSift + 2;
			if (leftChild > lastNode)
				break;
			int maxChild = (rightChild <= lastNode && comp(v[leftChild], v[rightChild]))
			                   ? rightChild
			                   : leftChild;
			if (!comp(v[toSift], v[maxChild]))
				break;
			std::swap(v[toSift], v[maxChild]);
			toSift = maxChild;
		}
	}
	ASSERT(std::is_heap(begin, end, comp));

	// An interruptible implementation of:
	// std::sort_heap(v.begin(), v.end(), comp);
	for (auto it = end; it != begin; --it) {
		if (this->th_interrupt_)
			return;
		std::pop_heap(begin, it, comp);
	}
	ASSERT(std::is_sorted(begin, end, comp));

	this->valid_fullMap_ = true;
}

#else
void SortCache::th_join() {}
void SortCache::th_sort() {}
#endif


SortCache::SortCache(const Index* idx, const NameBase* nbase)
    : nGames_(0), valid_fullMap_(false), th_interrupt_(false),
      partialHash_(false), fullMap_(NULL), th_(NULL), hash_(NULL), index_(idx),
      nbase_(nbase), refCount_(0) {}

SortCache::~SortCache() {
	th_interrupt();
	delete[] hash_;
	delete[] fullMap_;
}

SortCache* SortCache::create(const Index* idx, const NameBase* nb,
                             const char* criteria) {
	ASSERT(idx != NULL && nb != NULL && criteria != NULL);

	static const char fields[] = {
	    SORTING_date,       SORTING_year,         SORTING_event,
	    SORTING_site,       SORTING_round,        SORTING_white,
	    SORTING_black,      SORTING_eco,          SORTING_result,
	    SORTING_moveCount,  SORTING_avgElo,       SORTING_country,
	    SORTING_deleted,    SORTING_eventdate,    SORTING_whiteelo,
	    SORTING_blackelo,   SORTING_commentcount, SORTING_varcount,
	    SORTING_nagcount,   SORTING_resultwin,    SORTING_resultdraw,
	    SORTING_resultloss, SORTING_rating,       SORTING_number,
	    SORTING_sentinel};
	static const char* fields_end = fields + sizeof(fields);

	if (*criteria == '\0') // Invalid empty criteria.
		return NULL;

	SortCache* sc = new SortCache(idx, nb);

	// Parse the sorting criteria.
	size_t i = 0;
	for (const char *it = criteria; *it != 0; ++it) {
		bool valid = std::find(fields, fields_end, *it) != fields_end;
		sc->criteria_[i++] = *it++;
		bool reverse = (*it != '+');
		sc->criteria_[i++] = reverse ? 1 : 0;
		if (!valid                         // Unknown field
		    || (reverse && *it != '-')     // Invalid asc/desc param
		    || i >= sizeof(sc->criteria_)) // No space left for SORTING_sentinel
		{
			delete sc;
			return NULL;
		}
	}
	sc->criteria_[i] = SORTING_sentinel;

	sc->generateHashCache();
	sc->sortAsynchronously();

	return sc;
}

size_t SortCache::select(size_t row_offset, size_t row_count,
                         const HFilter& filter, gamenumT* result) const {
	ASSERT(filter != NULL && filter->size() <= nGames_);
	ASSERT(result != NULL);

	const size_t maxResults = filter->size();
	if (row_count == 0 || row_offset >= maxResults)
		return 0;

	size_t row_end = std::min(row_offset + row_count, maxResults);

	if (!valid_fullMap_) {
		gamenumT* v = new gamenumT[maxResults];
		gamenumT* v_end = v + maxResults;
		if (maxResults == nGames_) {
			// std::iota(v, v_end, 0);
			for (gamenumT i = 0; i < maxResults; ++i) {
				v[i] = i;
			}
		} else {
			std::copy(filter->begin(), filter->end(), v);
		}

		size_t skip = 0;
		if (row_offset > 1000) {
			skip = row_offset;
			std::nth_element(v, v + row_offset, v_end, CmpLess(this));
		}
		std::partial_sort(v + skip, v + row_end, v_end, CmpLess(this));
		std::copy(v + row_offset, v + row_end, result);
		delete[] v;
	} else {
		if (maxResults == nGames_) {
			std::copy(fullMap_ + row_offset, fullMap_ + row_end, result);
		} else {
			size_t filterCount = 0;
			size_t i = 0;
			for (; filterCount < row_offset; i++) {
				if (filter->get(fullMap_[i]) != 0)
					filterCount++;
			}
			for (; filterCount != row_end; i++) {
				if (filter->get(fullMap_[i]) != 0) {
					*result++ = fullMap_[i];
					filterCount++;
				}
			}
		}
	}

	return row_end - row_offset;
}

size_t SortCache::sortedPosition(gamenumT gameId, const HFilter& filter) const {
	ASSERT(filter != 0 && filter->size() <= nGames_);
	ASSERT(gameId < nGames_ && filter->get(gameId) != 0);

	size_t res = 0;
	if (valid_fullMap_) {
		for (gamenumT i = 0; i < nGames_; i++) {
			if (filter->get(fullMap_[i]) == 0)
				continue;
			if (fullMap_[i] == gameId)
				break;
			res++;
		}
	} else {
		CmpLess comp(this);
		for (gamenumT i = 0; i < nGames_; i++) {
			if (filter->get(i) == 0)
				continue;
			if (comp(i, gameId))
				++res;
		}
	}
	return res;
}

void SortCache::checkForChanges(gamenumT id) {
	th_interrupt();
	if (id >= nGames_) {
		generateHashCache();
		sortAsynchronously();
	} else {
		hash_[id] = calcHash(id);
		if (valid_fullMap_) {
			gamenumT* begin = fullMap_;
			gamenumT* end = fullMap_ + nGames_;
			gamenumT* it = std::find(begin, end, id);
			ASSERT(it != end);
			// Reposition the game if necessary:
			// - to the left if it compares less than the previous element;
			// - to the right if the next element is lower.
			CmpLess comp(this);
			if (it != begin && comp(*it, *(it - 1))) {
				std::rotate(
					std::upper_bound(begin, it, *it, comp),
					it, it + 1);
			} else if (it + 1 != end && comp(*(it + 1), *it)) {
				std::rotate(
					it, it + 1,
					std::lower_bound(it + 1, end, *it, comp));
			}
			ASSERT(std::is_sorted(begin, end, comp));
		} else {
			sortAsynchronously();
		}
	}
}

/*
 * Calculate the hashes of all games and store them into @e hash_.
 */
void SortCache::generateHashCache() {
	ASSERT(th_ == nullptr);

	valid_fullMap_ = false;
	nGames_ = index_->GetNumGames();

	// Generate the hash table.
	delete[] hash_;
	hash_ = new uint32_t[nGames_];
	for (gamenumT i = 0; i < nGames_; i++) {
		hash_[i] = calcHash(i);
	}
}

/*
 * Start a background thread that will sort the gameIds and will populate @e
 * fullMap_.
 */
void SortCache::sortAsynchronously() {
	ASSERT(th_ == nullptr);

#ifndef MULTITHREADING_OFF
	delete[] fullMap_;
	fullMap_ = new gamenumT[nGames_];
	th_ = new std::thread(&SortCache::th_sort, this);
#endif
}

/*
 * Compare two games according to @e criteria_.
 * The @e index_ is accessed only if the games' hashes are equal.
 * @param g1: the id of the first game.
 * @param g2: the id of the second game.
 * @returns true if @e g1 is ordered before @e g2.
 */
bool SortCache::CmpLess::operator()(gamenumT g1, gamenumT g2) const {
	ASSERT(g1 < sc_->nGames_ && g2 < sc_->nGames_);

	if (sc_->hash_[g1] != sc_->hash_[g2])
		return sc_->hash_[g1] < sc_->hash_[g2];
	if (!sc_->partialHash_)
		return g1 < g2;

	int cmp = sc_->fullCompare(g1, g2);
	if (cmp != 0)
		return cmp < 0;

	return g1 < g2;
}

static const int RESULT_SORT[] = { 0, 3, 1, 2 };
static int nameComp(const NameBase* nbase, nameT nt, idNumberT id1,
                    idNumberT id2) {
	ASSERT(nbase != NULL);
	return (id1 == id2) ? 0
	                    : strCaseCompare(nbase->GetName(nt, id1),
	                                     nbase->GetName(nt, id2));
}

/*
 * Compare two games according to @e criteria_.
 * @param left: the id of the first game.
 * @param right: the id of the second game.
 * @returns
 * - <0 if @e left is ordered before @e right.
 * - >0 if @e right is ordered before @e left.
 * - 0 otherwise.
 */
int SortCache::fullCompare(gamenumT left, gamenumT right) const {
	const IndexEntry *ie1 = index_->GetEntry(left);
	const IndexEntry *ie2 = index_->GetEntry(right);

	for (const char* field = criteria_; *field != SORTING_sentinel; field += 2) {
		int res;
		switch (*field) {
		case SORTING_date:
			res = (int)ie1->GetDate() - (int)ie2->GetDate();
			break;

		case SORTING_year:
			res = (int)ie1->GetYear() - (int)ie2->GetYear();
			break;

		case SORTING_eco:
			res = (int)ie1->GetEcoCode() - (int)ie2->GetEcoCode();
			break;

		case SORTING_moveCount:
			res = (int)ie1->GetNumHalfMoves() - (int)ie2->GetNumHalfMoves();
			break;

		case SORTING_white:
			res = nameComp(nbase_, NAME_PLAYER, ie1->GetWhite(), ie2->GetWhite());
			break;

		case SORTING_black:
			res = nameComp(nbase_, NAME_PLAYER, ie1->GetBlack(), ie2->GetBlack());
			break;

		case SORTING_event:
			res = nameComp(nbase_, NAME_EVENT, ie1->GetEvent(), ie2->GetEvent());
			break;

		case SORTING_site:
			res = nameComp(nbase_, NAME_SITE, ie1->GetSite(), ie2->GetSite());
			break;

		case SORTING_round: {
			idNumberT id1 = ie1->GetRound();
			idNumberT id2 = ie2->GetRound();
			res = (id1 == id2)
			          ? 0
			          : strCompareRound(nbase_->GetName(NAME_ROUND, id1),
			                            nbase_->GetName(NAME_ROUND, id2));
			break;
		}

		case SORTING_resultwin:
			res = (ie1->GetResult() == RESULT_White ? 1 : 0) - (ie2->GetResult() == RESULT_White ? 1 : 0);
			break;

		case SORTING_resultdraw:
			res = (ie1->GetResult() == RESULT_Draw ? 1 : 0) - (ie2->GetResult() == RESULT_Draw ? 1 : 0);
			break;

		case SORTING_resultloss:
			res = (ie1->GetResult() == RESULT_Black ? 1 : 0) - (ie2->GetResult() == RESULT_Black ? 1 : 0);
			break;

		case SORTING_result:
			res = RESULT_SORT[ie1->GetResult()] - RESULT_SORT[ie2->GetResult()];
			break;

		case SORTING_avgElo:  // Average Elo rating:
			{
				int r1 = ie1->GetWhiteElo(nbase_) + ie1->GetBlackElo(nbase_);
				int r2 = ie2->GetWhiteElo(nbase_) + ie2->GetBlackElo(nbase_);
				res = r1 - r2;
			}
			break;

		case SORTING_country:  // Last 3 characters of site field:
			{
				const char* sOne = ie1->GetSiteName(nbase_);
				const char* sTwo = ie2->GetSiteName(nbase_);
				size_t slenOne = std::strlen(sOne);
				size_t slenTwo = std::strlen(sTwo);
				if (slenOne > 3) { sOne += slenOne - 3; }
				if (slenTwo > 3) { sTwo += slenTwo - 3; }
				res = strCaseCompare (sOne, sTwo);
			}
			break;

		case SORTING_deleted:
			res = (int)ie1->GetDeleteFlag() - (int)ie2->GetDeleteFlag();
			break;

		case SORTING_eventdate:
			res = (int)ie1->GetEventDate() - (int)ie2->GetEventDate();
			break;

		case SORTING_whiteelo:
			res = (int)ie1->GetWhiteElo(nbase_) - (int)ie2->GetWhiteElo(nbase_);
			break;

		case SORTING_blackelo:
			res = (int)ie1->GetBlackElo(nbase_) - (int)ie2->GetBlackElo(nbase_);
			break;

		case SORTING_commentcount:
			res = (int) ie1->GetCommentCount() - (int) ie2->GetCommentCount();
			break;

		case SORTING_varcount:
			res = (int) ie1->GetVariationCount() - (int) ie2->GetVariationCount();
			break;

		case SORTING_nagcount:
			res = (int) ie1->GetNagCount() - (int) ie2->GetNagCount();
			break;

		case SORTING_rating:
			res = (int)ie1->GetRating(nbase_) - (int)ie2->GetRating(nbase_);
			break;

		case SORTING_number:
			res = (int) left - (int) right;
			break;

		default:    // Should never happen:
			ASSERT(0);
			return 0;
		}

		if (res != 0)
			return *(field + 1) ? -res : res;
	}

    return 0;
}

/*
 * Calculate an order-preserving hash corresponding to the current criteria.
 * @param gameId: the id of the game whose hash should be calculated.
 * @returns the hash value.
 */
uint32_t SortCache::calcHash(gamenumT gameId) {
	uint64_t retValue = 0;
	const size_t nHashBits = 32;
	size_t totalBitsUsed = 0;
	const IndexEntry* ie = index_->GetEntry(gameId);

	for (const char* field = criteria_; *field != SORTING_sentinel; ++field) {
		uint32_t value;
		size_t bitsUsed;
		switch (*field) {
			case SORTING_white:
				value = strStartHash(ie->GetWhiteName(nbase_));
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			case SORTING_black:
				value = strStartHash(ie->GetBlackName(nbase_));
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			case SORTING_site:
				value = strStartHash(ie->GetSiteName(nbase_));
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			case SORTING_event:
				value = strStartHash(ie->GetEventName(nbase_));
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			case SORTING_round:
				value = strGetUnsigned(ie->GetRoundName(nbase_));
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			case SORTING_country:
			{
				const char *scountry = ie->GetSiteName (nbase_);
				size_t slen = std::strlen(scountry);
				if (slen > 3) 
					scountry += slen - 3;
				value = strStartHash(scountry);
				bitsUsed = nHashBits;
				partialHash_ = true;
				break;
			}
			case SORTING_date:
				value = ie->GetDate();
				bitsUsed = 24;
				break;
			case SORTING_eventdate:
				value = ie->GetEventDate();
				bitsUsed = 32;
				break;
			case SORTING_year:
				value = ie->GetYear();
				bitsUsed = 16;
				break;
			case SORTING_whiteelo:
				value = ie->GetWhiteElo(nbase_);
				bitsUsed = 16;
				break;
			case SORTING_blackelo:
				value = ie->GetBlackElo(nbase_);
				bitsUsed = 16;
				break;
			case SORTING_avgElo:
				value = ie->GetWhiteElo(nbase_) + ie->GetBlackElo(nbase_);
				bitsUsed = 16;
				break;
			case SORTING_result:
				value = RESULT_SORT[ie->GetResult()];
				bitsUsed = 8;
				break;
			case SORTING_resultwin:
				value = ie->GetResult() == RESULT_White ? 1 : 0;
				bitsUsed = 8;
				break;
			case SORTING_resultdraw:
				value = ie->GetResult() == RESULT_Draw ? 1 : 0;
				bitsUsed = 8;
				break;
			case SORTING_resultloss:
				value = ie->GetResult() == RESULT_Black ? 1 : 0;
				bitsUsed = 8;
				break;
			case SORTING_moveCount:
				value = ie->GetNumHalfMoves();
				bitsUsed = 16;
				break;
			case SORTING_eco:
				value = ie->GetEcoCode();
				bitsUsed = 16;
				break;
			case SORTING_commentcount:
				value = ie->GetCommentCount();
				bitsUsed = 16;
				break;
			case SORTING_varcount:
				value = ie->GetVariationCount();
				bitsUsed = 16;
				break;
			case SORTING_nagcount:
				value = ie->GetNagCount();
				bitsUsed = 16;
				break;
			case SORTING_deleted:
				value = (ie->GetDeleteFlag() ? 1 : 0);
				bitsUsed = 8;
				break;
			case SORTING_rating:
				value = ie->GetRating(nbase_);
				bitsUsed = 8;
				break;
			case SORTING_number:
				value = gameId;
				bitsUsed = 32;
				break;
			default:    // Should never happen:
				ASSERT(0);
				partialHash_ = true;
				return 0;
		}

		// If reverse order, just negate the cache value
		if (*++field) {
			value = ~value;
			if (sizeof(value) * 8 > bitsUsed) {
				// Clear the unused top bits
				value <<= sizeof(value) * 8 - bitsUsed;
				value >>= sizeof(value) * 8 - bitsUsed;
			}
		}
		// Combine with previous hash value
		retValue <<= bitsUsed;
		retValue |= value;
		totalBitsUsed += bitsUsed;

		// If not all search attributes fit, then it is a partial hash
		if (totalBitsUsed > nHashBits) {
			retValue >>= totalBitsUsed - nHashBits;
			partialHash_ = true;
			break;
		}
		if (totalBitsUsed == nHashBits) {
			if (*(field + 1) != SORTING_sentinel)
				partialHash_ = true;
			break;
		}
	}

	return static_cast<uint32_t>(retValue);
}
