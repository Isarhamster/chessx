/*
 * Copyright (C) 2017 Fulvio Benini
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

#include "scidbase.h"
#include "sortcache.h"
#include <array>
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <stdint.h>

class Test_SortCache : public ::testing::Test {};

TEST_F(Test_SortCache, select_sortedPosition) {
	// Open the test database
	scidBaseT dbase;
	static const char* database = SCID_TESTDIR "res_database";
	ASSERT_EQ(OK, dbase.open("SCID4", FMODE_ReadOnly, database));
	ASSERT_NE(0U, dbase.numGames());
	const NameBase* nb = dbase.getNameBase();
	std::uniform_int_distribution<> rndID(0, dbase.numGames() - 1);
	std::vector<gamenumT> buf(dbase.numGames());

	// Create filters
	auto dbfilter = dbase.getFilter("dbfilter");
	auto emptyfilter = dbase.getFilter(dbase.newFilter());
	emptyfilter.clear();
	auto filter = dbase.getFilter(dbase.newFilter());
	std::mt19937 re;
	std::uniform_int_distribution<> rndBool(0, 1);
	std::uniform_int_distribution<> filterVal(0, 255);
	for (gamenumT i = 0, n = dbase.numGames(); i < n; ++i) {
		filter.set(i, static_cast<byte>(rndBool(re) * filterVal(re)));
	}

	// Valid fields
	const char fields[] = "NdyesnwbormRcDEWBCVA150i";
	auto getFieldIdx = [fields](char ch) -> std::ptrdiff_t {
		auto it = std::find(std::begin(fields), std::end(fields), ch);
		if (it != std::end(fields))
			return std::distance(std::begin(fields), it);
		return -1;
	};

	// Create a Vector with the Index data
	using IndexRec = std::array<int64_t, sizeof fields>;
	std::vector<IndexRec> vIndex(dbase.numGames());
	gamenumT gameId = 0;
	for (auto& rec : vIndex) {
		static const int RESULT_SORT[] = {0, 3, 1, 2};

		auto ie = dbase.getIndexEntry(gameId);
		rec[0] = gameId++;
		rec[1] = ie->GetDate();
		rec[2] = ie->GetYear();
		rec[3] = ie->GetEvent(); // NAMEBASE
		rec[4] = ie->GetSite();  // NAMEBASE
		rec[5] = ie->GetRound(); // NAMEBASE
		rec[6] = ie->GetWhite(); // NAMEBASE
		rec[7] = ie->GetBlack(); // NAMEBASE
		rec[8] = ie->GetEcoCode();
		rec[9] = RESULT_SORT[ie->GetResult()];
		rec[10] = ie->GetNumHalfMoves();
		rec[11] = ie->GetWhiteElo(nb) + ie->GetBlackElo(nb);
		rec[12] = ie->GetSite(); // NAME COUNTRY
		rec[13] = ie->GetDeleteFlag() ? 1 : 0;
		rec[14] = ie->GetEventDate();
		rec[15] = ie->GetWhiteElo(nb);
		rec[16] = ie->GetBlackElo(nb);
		rec[17] = ie->GetCommentCount();
		rec[18] = ie->GetVariationCount();
		rec[19] = ie->GetNagCount();
		rec[20] = ie->GetResult() == RESULT_White ? 1 : 0;
		rec[21] = ie->GetResult() == RESULT_Draw ? 1 : 0;
		rec[22] = ie->GetResult() == RESULT_Black ? 1 : 0;
		rec[23] = ie->GetRating(nb);
	}

	auto sort_vIndex = [&vIndex, nb, &getFieldIdx](std::string criteria) {
		sort(vIndex.begin(), vIndex.end(),
		     [&](auto& a, auto& b) { return a[0] < b[0]; });

		auto it = criteria.rbegin();
		while (it != criteria.rend()) {
			auto reverse = *it++;
			auto field = getFieldIdx(*it++);
			stable_sort(vIndex.begin(), vIndex.end(), [&](auto& a, auto& b) {
				int64_t res = a[field] - b[field];
				if (res == 0)
					return false;

				// clang-format off
				nameT nt = field == 3 ? NAME_EVENT :
				           field == 4 ? NAME_SITE :
				           field == 6 ? NAME_PLAYER:
				           field == 7 ? NAME_PLAYER:
				           NAME_INVALID;
				// clang-format on
				if (nt != NAME_INVALID) {
					res = strCaseCompare(
					    nb->GetName(nt, static_cast<idNumberT>(a[field])),
					    nb->GetName(nt, static_cast<idNumberT>(b[field])));
				} else if (field == 5) {
					nt = NAME_ROUND;
					res = strCompareRound(
					    nb->GetName(nt, static_cast<idNumberT>(a[field])),
					    nb->GetName(nt, static_cast<idNumberT>(b[field])));
				} else if (field == 12) {
					auto strA = nb->GetName(NAME_SITE,
					                        static_cast<idNumberT>(a[field]));
					auto strB = nb->GetName(NAME_SITE,
					                        static_cast<idNumberT>(b[field]));
					if (std::strlen(strA) > 3)
						strA += std::strlen(strA) - 3;
					if (std::strlen(strB) > 3)
						strB += std::strlen(strB) - 3;
					res = strCaseCompare(strA, strB);
				}

				return (reverse == '+') ? res < 0 : res > 0;
			});
		}
	};

	auto test_listGames = [&dbase, &buf, &vIndex](auto crit, auto flt,
	                                              size_t start, size_t count) {
		EXPECT_EQ(count,
		          dbase.listGames(crit, start, buf.size(), flt, buf.data()));
		EXPECT_TRUE(
		    std::equal(buf.begin(), buf.begin() + count, vIndex.begin() + start,
		               [](auto& id, auto& rec) { return id == rec[0]; }))
		    << "Criteria: " << crit;
	};

	auto test_sortedPosition = [&](auto crit, auto flt) {
		gamenumT id;
		do {
			id = rndID(re);
		} while (flt.get(id) == 0);
		auto it = std::find_if(vIndex.begin(), vIndex.end(),
		                       [id](auto& rec) { return id == rec[0]; });
		size_t pos = distance(vIndex.begin(), it);
		EXPECT_EQ(pos, dbase.sortedPosition(crit, flt, id));
	};

	std::vector<std::string> testCrit(30);

	// Test some random compound criteria.
	std::uniform_int_distribution<> rndField(0, sizeof(fields) - 2);
	std::uniform_int_distribution<> rndCritLen(1, 10);
	for (auto& criteria : testCrit) {
		for (auto i = 0, n = rndCritLen(re); i < n; i++) {
			criteria += fields[rndField(re)];
			criteria += rndBool(re) ? '+' : '-';
		}
	}

	// Test all criteria composed by a single char
	for (auto ch = 1; ch < 256; ch++) {
		for (auto dir : {'+', '-', '*'}) {
			char crit[] = {static_cast<char>(ch), dir, '\0'};
			testCrit.emplace_back(crit);
		}
	}

	for (int pass = 0; pass < 2; ++pass) {
		for (auto& criteria : testCrit) {
			const char* crit = criteria.c_str();
			bool validCriteria = (getFieldIdx(*crit) != -1) &&
			                     (crit[1] == '+' || crit[1] == '-');

			// sort the vIndex
			if (validCriteria)
				sort_vIndex(crit);

			// For all games, random selection of games, no games:
			for (auto flt : {dbfilter, filter, emptyfilter}) {
				// move the games excluded from filter to the end of vIndex
				if (validCriteria) {
					stable_partition(
					    vIndex.begin(), vIndex.end(), [flt](auto& e) {
						    return flt->get(static_cast<gamenumT>(e[0])) != 0;
					    });
				}

				// Test scidBaseT::listGames()
				for (size_t off = 0, n = flt.size(); off < n; off += n / 4) {
					size_t expCount = validCriteria ? flt.size() - off : 0;
					test_listGames(crit, flt, off, expCount);
				}

				// Test scidBaseT::sortedPosition()
				if (validCriteria && flt.size()) {
					for (int j = 0; j < 10; j++)
						test_sortedPosition(crit, flt);
				}
			}

			// Test scidBaseT::createSortCache()
			if (!pass) {
				auto sc = dbase.createSortCache(crit);
				EXPECT_EQ(validCriteria, sc);
			}

			// Test scidBaseT::releaseSortCache()
			if (pass)
				dbase.releaseSortCache(crit);
		}
	}

	// Test scidBaseT::Close() when background threads are running.
	EXPECT_TRUE(dbase.createSortCache("w+s-"));
	EXPECT_TRUE(dbase.createSortCache("i-d-n+"));
}
