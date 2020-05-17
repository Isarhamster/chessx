/*
* Copyright (C) 2015 Fulvio Benini

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

#include "misc.h"
#include "timer.h"
#include <gtest/gtest.h>
#include <random>

namespace v4_6_2 {
	uint strTrimRight(char * target, const char * trimChars);
}


static std::vector<std::string> strings_;
static std::vector<std::string> createRandom(int maxLen, int nLines) {
	std::mt19937 re(std::random_device{}());
	std::uniform_int_distribution<> lineLenght(0, maxLen - 1);
	std::uniform_int_distribution<> randomChar(1, 255);
	std::vector<std::string> res;
	for (int i = 0; i < nLines; i++) {
		res.emplace_back();
		for (int j = 0, len = lineLenght(re); j < len; j++) {
			res.back().append(1, static_cast<unsigned char>(randomChar(re)));
		}
	}
	return res;
}

class Test_StrLib : public ::testing::Test {
protected:
	char buf[1024];
	struct Data {
		const char* static_input;
		char* input1;
		char* input2;
		const char* expected;
		uint nTrimmed;
	};
	std::vector<Data> data_ = {
		{ ""    , nullptr, nullptr, ""   , 0 } ,
		{ " "   , nullptr, nullptr, ""   , 1 } ,
		{ "a"   , nullptr, nullptr, "a"  , 0 } ,
		{ "a "  , nullptr, nullptr, "a"  , 1 } ,
		{ " a"  , nullptr, nullptr, " a" , 0 } ,
		{ "  "  , nullptr, nullptr, ""   , 2 } ,
		{ "a  " , nullptr, nullptr, "a"  , 2 } ,
		{ "a\t ", nullptr, nullptr, "a"  , 2 } ,
		{ " a " , nullptr, nullptr, " a" , 1 } ,
		{ "  a" , nullptr, nullptr, "  a", 0 } ,
		{ " a  ", nullptr, nullptr, " a" , 2 } ,
		{ " \rare\n\t\r  ", nullptr, nullptr, " \rare" , 5 } ,
		{ u8"utf8 κόσμε\t\r ♚♛♜ \n ", nullptr, nullptr, u8"utf8 κόσμε\t\r ♚♛♜" , 3 }
	};

protected:
	static void SetUpTestCase() {
		strings_ = createRandom(300, 3000);
	}

	// strTrimRight testing
	virtual void SetUp() {
		char* s = buf;
		for (auto& e : data_) {
			auto sz = strlen(e.static_input) + 1;

			e.input1 = s;
			strcpy(e.input1, e.static_input);
			s += sz;

			e.input2 = s;
			strcpy(e.input2, e.static_input);
			s += sz;
		}
	}
};


TEST_F(Test_StrLib, strTrimRight_correctness) {
	for (auto& e : data_) {
		strTrimRight(e.input1);
		EXPECT_STREQ(e.expected, e.input1);
	}
}

TEST_F(Test_StrLib, strTrimRight_consistency) {
	for (auto& e : data_) {
		strTrimRight(e.input1);
		::v4_6_2::strTrimRight(e.input2, " \t\r\n");
		EXPECT_STREQ(e.input2, e.input1);
	}
}

namespace v4_6_2 {
	uint strTrimRight(char * target, const char * trimChars) {
		uint trimCount = 0;
		char * s = target;
		char * lastNonTrim = NULL;
		while (*s) {
			if (strContainsChar(trimChars, *s)) {
				trimCount++;
			}
			else {
				lastNonTrim = s;
				trimCount = 0;
			}
			s++;
		}
		if (lastNonTrim != NULL) {
			// End the string after the last nontrimmable char:
			lastNonTrim++;
			*lastNonTrim = 0;
		}
		else {
			// The string only contained trimmable characters:
			*target = 0;
		}
		return trimCount;
	}
}

TEST_F(Test_StrLib, strCaseCompare) {
	auto v = strings_;
	for (auto& s : v) {
		unsigned char* it = (unsigned char*)s.data();
		std::transform(it, it + s.size(), it, ::tolower);
	}

	for (size_t i = 0, n = v.size(); i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			int a = v[i].compare(v[j]);
			int b = strCaseCompare(strings_[i].c_str(), strings_[j].c_str());
			EXPECT_EQ(a < 0, b < 0);
			EXPECT_EQ(a > 0, b > 0);
			EXPECT_EQ(a == 0, b == 0);
		}
	}
}

TEST_F(Test_StrLib, strStartHash) {
	for (const auto& s1 : strings_) {
		for (const auto& s2 : strings_) {
			auto hash1 = strStartHash(s1.c_str());
			auto hash2 = strStartHash(s2.c_str());
			int cmp = strCaseCompare(s1.c_str(), s2.c_str());
			if (hash1 < hash2) {
				EXPECT_LT(cmp, 0);
			} else if (hash1 > hash2) {
				EXPECT_GT(cmp, 0);
			} else {
				if (cmp != 0) {
					ASSERT_TRUE(s1.size() >= 4);
					ASSERT_TRUE(s2.size() >= 4);
					auto s1_tmp = s1.substr(0, 4);
					auto s2_tmp = s2.substr(0, 4);
					ASSERT_EQ(0, strCaseCompare(s1_tmp.c_str(), s2_tmp.c_str()));
				}
			}
		}
	}
}
