/*
* Copyright (C) 2016 Fulvio Benini

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

#include "namebase.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <map>
#include <random>
#include <string>
#include <vector>

TEST(Test_Namebase, id_limits) {
	const size_t MAX_LEN = 255;                 // Max 255 chars;
	const idNumberT MAX_ID[] = {(1 << 20) - 1,  // MAX_PLAYER_ID
	                            (1 << 19) - 1,  // MAX_EVENT_ID
	                            (1 << 19) - 1,  // MAX_SIDE_ID
	                            (1 << 18) - 1}; // MAX_ROUND_ID

	std::vector<std::pair<nameT, idNumberT> > v;
	for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
		for (idNumberT i = 0; i < MAX_ID[nt]; i++) {
			v.emplace_back(nt, i);
			v.emplace_back(nt, i);
		}
	}
	std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));

	NameBase nb;
	for (auto e : v) {
		EXPECT_EQ(OK, nb.addName(e.first, std::to_string(e.second).c_str(),
		                       MAX_LEN, MAX_ID[e.first])
		                  .first);
	}

	for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
		EXPECT_NE(OK, nb.addName(nt, "full", MAX_LEN, MAX_ID[nt]).first);
	}
}

TEST(Test_Namebase, max_chars) {
	// .pgn and .sn4 files cannot store names longer than 255 bytes
	const size_t MAX_LEN = 255; // Max 255 chars;
	std::string s = u8"utf8 κόσμε\t\r ♚♛♜ \n ";
	s = s + s + s + s + s + s + s + s + s + s + s;
	ASSERT_TRUE(s.size() > MAX_LEN);

	NameBase nb;
	for (size_t i = 0; i < s.size(); i++) {
		for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
			auto test = nb.addName(nt, s.substr(0, i).c_str(), MAX_LEN, 1000);
			if (i <= MAX_LEN)
				EXPECT_EQ(OK, test.first);
			else
				EXPECT_NE(OK, test.first);
		}
	}
}

TEST(Test_Namebase, FindExactName) {
	// clang-format off
	std::vector<const char*> test_names = {
		"Empty", "",
		"Spaces", " ", " Spaces", "Spaces ", " Spaces ",
		"German:", "Ä" "ä" "Ü" "ü" "ß",
		"Russian:", "Я", "Б", "Г", "Д", "Ж", "Й"
		"Polish:", "Ł", "Ą", "Ż", "Ę", "Ć", "Ń", "Ś", "Ź"
		"Kanji:", "てすと", "(te-su-to)",
		"Hankaku:", "ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃ"
	};
	// clang-format on

	NameBase nb;
	std::map<idNumberT, const char*> ids[NUM_NAME_TYPES];
	for (const char* name : test_names) {
		for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
			auto id = nb.addName(nt, name, 255, 1000);
			EXPECT_EQ(OK, id.first);
			EXPECT_TRUE(ids[nt].emplace(id.second, name).second);
		}
	}

	for (const char* name : test_names) {
		for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
			idNumberT id = 0;
			EXPECT_EQ(OK, nb.FindExactName(nt, name, &id));
			EXPECT_STREQ(ids[nt][id], name);
		}
	}
}

TEST(Test_Namebase, sort_order) {
	// Older Scid versions (< 4.6) used a class StrTree that
	// implemented an hybrid hash map/sorted binary tree.
	// The code was written in 1999 and have at least 2 known issues
	// with strings starting with a unicode char:
	// - a unicode char use multiple bytes, usually 2.
	//   The StrTree class will treat the first byte as unsigned (-1 > 127)
	//   and the other bytes as signed (-1 < 127).
	//   This leads to strings that are not consistently sorted.
	// - If strings are inserted using a different order the StrTree class have
	//   side-effects, including the possibility to create duplicate entries.
	// To minimize issues with older versions it is necessary to
	// keep using that sorting criteria.
	NameBase nb;
	nameT nt = NAME_PLAYER;
	EXPECT_EQ(OK, nb.addName(nt, "AA", 255, 1000).first);
	EXPECT_EQ(OK, nb.addName(nt, "AÜ", 255, 1000).first);
	EXPECT_EQ(OK, nb.addName(nt, "ÜA", 255, 1000).first);
	EXPECT_EQ(OK, nb.addName(nt, "ÜÜ", 255, 1000).first);
	auto names = nb.getFirstMatches(nt, "", 4);
	if (std::is_signed_v<char>) {
		EXPECT_STREQ(nb.GetName(nt, names[0]), "AÜ");
		EXPECT_STREQ(nb.GetName(nt, names[1]), "AA");
		EXPECT_STREQ(nb.GetName(nt, names[2]), "ÜÜ");
		EXPECT_STREQ(nb.GetName(nt, names[3]), "ÜA");
	} else {
		EXPECT_STREQ(nb.GetName(nt, names[1]), "AA");
		EXPECT_STREQ(nb.GetName(nt, names[0]), "AÜ");
		EXPECT_STREQ(nb.GetName(nt, names[3]), "ÜA");
		EXPECT_STREQ(nb.GetName(nt, names[2]), "ÜÜ");
	}
}
