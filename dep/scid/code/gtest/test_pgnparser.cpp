/*
* Copyright (C) 2016-2018 Fulvio Benini

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

#include "game.h"
#include "pgnparse.h"
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
#include <random>
#include <string>

namespace {

const char* gameUTF8 = SCID_TESTDIR "res_gameUTF8.pgn";
const char* gameLatin1 = SCID_TESTDIR "res_gameLatin1.pgn";
const char* gameLatin1Conv = SCID_TESTDIR "res_gameLatin1expected.pgn";

auto readFile(const char* filename) {
	std::string res;
	std::filebuf file;
	if (file.open(filename, std::ios::in | std::ios::binary)) {
		auto fileSz = file.pubseekoff(0, std::ios::end);
		if (fileSz > 0) {
			res.resize(static_cast<size_t>(fileSz));
			file.pubseekpos(0);
			file.sgetn(&res.front(), fileSz);
		}
	}
	return res;
}

} // end of anonymous namespace

TEST(Test_PgnParser, UTF8_char) {
	auto pgnUTF8 = readFile(gameUTF8);
	ASSERT_TRUE(pgnUTF8.size() > 0);

	Game game;
	PgnParseLog errors;
	ASSERT_TRUE(pgnParseGame(pgnUTF8.data(), pgnUTF8.size(), game, errors));
	EXPECT_TRUE(errors.log.empty());
	game.SetPgnFormat(PGN_FORMAT_Plain);
	game.ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS | PGN_STYLE_COMMENTS |
	                   PGN_STYLE_SCIDFLAGS);
	auto pgn = game.WriteToPGN(75, true);

	ASSERT_STREQ(pgnUTF8.c_str(), pgn.first);
}

TEST(Test_PgnParser, Latin1_char) {
	auto pgnLatin1 = readFile(gameLatin1);
	ASSERT_TRUE(pgnLatin1.size() > 0);

	Game game;
	PgnParseLog errors;
	ASSERT_TRUE(pgnParseGame(pgnLatin1.data(), pgnLatin1.size(), game, errors));
	EXPECT_TRUE(errors.log.empty());
	game.SetPgnFormat(PGN_FORMAT_Plain);
	game.ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS | PGN_STYLE_COMMENTS |
	                   PGN_STYLE_SCIDFLAGS);
	auto pgn = game.WriteToPGN(75, true);

	auto pgnUTF8 = readFile(gameLatin1Conv);
	ASSERT_TRUE(pgnUTF8.size() > 0);

	ASSERT_STREQ(pgnUTF8.c_str(), pgn.first);
}

TEST(Test_PgnParser, EPD) {
	const char* pgn = // clang-format off
		"rnbqkb1r/1ppppppp/5n2/p7/2P5/4P3/PP1P1PPP/RNBQKBNR b KQkq - 0 1;\n"
		"%escape line\n"
		"rq2r1k1/1bbn1pp1/1pp2n1p/p2p4/N2P3B/P2BP2P/1PQ1NPP1/2R2R1K b - -\n"
		"1B2K3/4b3/3pk3/5R2/8/7B/8/8 w - - bm Bb8-c7; ce +M3; pv Bb8-c7 Be7-f8 Ke8xf8 d6-d5 Rf5-f7+;\n"
		"1B2K3/4b3/3pk3/5R2/8/7B/8/8 w - - bm Bc7 Rf3+\n"
		";pre-game comment, no warnings or errors\n"
		"unknown, fatal error\n"
		"[Event \"F/S Return Match\"]\n"
		"[Site \"Bel]gr\\\"ade], Serbia JUG\"]\n"
		"[Date \"1992.11.04\"]\n"
		"[Round \"29\"]\n"
		"[White \"Fischer, Robert J.\"]\r\n"
		"[Black \"Spassky, Boris V.\"]\n\r"
		";29th game of the 1992 match\n"
		"1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 {This opening is called\n"
		"the; Ruy% Lopez.}\n"
		"4. Ba4 Nf6 5. O-O\n"
		"; rnbqkb1r/1ppppppp/5n2/p7/2P5/4P3/PP1P1PPP/RNBQKBNR b KQkq -1; %comm\r\n"
		"Be7 ;6. Re1 b5 7. Bb3 d6 8. c3 O-O 9. h3 Nb8 10. d4 Nbd7\n"
		"[Event \"Partial game\"]\n"
		"[Site \"With few tags\"]\n"
		"1-0\n\n";
	const char* expected_game =
		"[Event \"F/S Return Match\"]\n"
		"[Site \"Bel]gr\"ade], Serbia JUG\"]\n"
		"[Date \"1992.11.04\"]\n"
		"[Round \"29\"]\n"
		"[White \"Fischer, Robert J.\"]\n"
		"[Black \"Spassky, Boris V.\"]\n"
		"[Result \"*\"]\n"
		"\n"
		"{29th game of the 1992 match} \n"
		"1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 {This opening is called the; Ruy% Lopez.} 4.Ba4\n"
		"Nf6 5.O-O {rnbqkb1r/1ppppppp/5n2/p7/2P5/4P3/PP1P1PPP/RNBQKBNR b KQkq -1; \n"
		"%comm} 5...Be7 {6. Re1 b5 7. Bb3 d6 8. c3 O-O 9. h3 Nb8 10. d4 Nbd7}"
		" *\n\n";
	// clang-format on

	char buf[128];
	Game game;
	game.SetPgnFormat(PGN_FORMAT_Plain);
	game.ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS | PGN_STYLE_COMMENTS);

	auto len = std::strlen(pgn);
	PgnParseLog parseLog;
	game.Clear();
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_TRUE(parseLog.log.empty());
	game.GetCurrentPos()->PrintFEN(buf, FEN_CASTLING_EP);
	EXPECT_STREQ("rnbqkb1r/1ppppppp/5n2/p7/2P5/4P3/PP1P1PPP/RNBQKBNR b KQkq -",
	             buf);
	EXPECT_STREQ("0 1;", game.GetMoveComment());

	game.Clear();
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_TRUE(parseLog.log.empty());
	game.GetCurrentPos()->PrintFEN(buf, FEN_CASTLING_EP);
	EXPECT_STREQ(
	    "rq2r1k1/1bbn1pp1/1pp2n1p/p2p4/N2P3B/P2BP2P/1PQ1NPP1/2R2R1K b - -",
	    buf);
	EXPECT_STREQ("", game.GetMoveComment());

	game.Clear();
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_TRUE(parseLog.log.empty());
	game.GetCurrentPos()->PrintFEN(buf, FEN_CASTLING_EP);
	EXPECT_STREQ("1B2K3/4b3/3pk3/5R2/8/7B/8/8 w - -", buf);
	EXPECT_STREQ("bm Bb8-c7; ce +M3; pv Bb8-c7 Be7-f8 Ke8xf8 d6-d5 Rf5-f7+;",
	             game.GetMoveComment());

	game.Clear();
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_TRUE(parseLog.log.empty());
	game.GetCurrentPos()->PrintFEN(buf, FEN_CASTLING_EP);
	EXPECT_STREQ("1B2K3/4b3/3pk3/5R2/8/7B/8/8 w - -", buf);
	EXPECT_STREQ("bm Bc7 Rf3+", game.GetMoveComment());

	game.Clear();
	ASSERT_FALSE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                          game, parseLog));
	EXPECT_FALSE(parseLog.log.empty());
	EXPECT_NE(game.GetMoveComment(), nullptr);

	game.Clear();
	std::string last_log = parseLog.log;
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_TRUE(parseLog.log.size() > last_log.size());
	EXPECT_STREQ(expected_game, game.WriteToPGN(75, true).first);

	game.Clear();
	last_log = parseLog.log;
	ASSERT_TRUE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                         game, parseLog));
	EXPECT_STREQ(last_log.c_str(), parseLog.log.c_str());
	EXPECT_STREQ("Partial game", game.GetEventStr());

	game.Clear();
	ASSERT_FALSE(pgnParseGame(pgn + parseLog.n_bytes, len - parseLog.n_bytes,
	                          game, parseLog));
	ASSERT_EQ(parseLog.n_bytes, len);
}

TEST(Test_PgnParser, is_PGNsymbol) {
	bool chars[256] = {false};
	for (unsigned ch = 'A'; ch <= 'Z'; ++ch) {
		chars[ch] = true;
	}
	for (unsigned ch = 'a'; ch <= 'z'; ++ch) {
		chars[ch] = true;
	}
	for (unsigned ch = '0'; ch <= '9'; ++ch) {
		chars[ch] = true;
	}
	const unsigned extra[] = {'_', '+', '#', '=', ':', '-'};
	for (unsigned ch : extra) {
		chars[ch] = true;
	}
	const unsigned drawresult_unclear[] = {'/', '~'};
	for (unsigned ch : drawresult_unclear) {
		chars[ch] = true;
	}
	const unsigned chess_variants[] = {',', '@'};
	for (unsigned ch : chess_variants) {
		chars[ch] = true;
	}

	for (int i = 0; i < 256; i++) {
		EXPECT_EQ(chars[i],
		          pgn_impl::is_PGNsymbol(static_cast<signed char>(i)));
		EXPECT_EQ(chars[i],
		          pgn_impl::is_PGNsymbol(static_cast<unsigned char>(i)));
	}
}

TEST(Test_PgnParser, pgn_normalize) {
	std::string buf;
	auto make_str = [&buf](size_t pos, const char* str) {
		buf.assign(pos, '^');
		buf.append(str, str + std::strlen(str));
		size_t n_newlines = std::count(buf.begin(), buf.end(), '\n');
		return n_newlines == pgn::normalize(buf, pos) &&
		       buf.substr(0, pos) == std::string(pos, '^');
	};

	std::pair<const char*, const char*> tests[] = {
	    {"Surname, Name (2800)", "Surname, Name (2800)"},
	    {"    Surname,     Name (2800)       ",
	     "    Surname,     Name (2800)       "},
	    {"Surname,\nName (2800)", "Surname, Name (2800)"},
	    {"Surname,\r\nName (2800)", "Surname, Name (2800)"},
	    {"Surname, \r\nName (2800)", "Surname, Name (2800)"},
	    {"Surname,\r\n Name (2800)", "Surname, Name (2800)"},
	    {"Surname, \r\n Name (2800)", "Surname,  Name (2800)"},
	    {"\n\tSurname, \v\r\t\nName\r\n(2800)\n", "Surname, Name (2800)"},
	    {"  \n \r Surname,  \v\n   \tName\t\v(2800)        \r\n",
	     "    Surname,     Name (2800)        "}};

	for (int pos = 0; pos <= 30; pos = (pos + 1) * 2) { // 0, 2, 6, 14, 30
		for (auto str : tests) {
			EXPECT_TRUE(make_str(pos, str.first));
			EXPECT_STREQ(buf.substr(pos).c_str(), str.second);
		}
	}
}

TEST(Test_PgnParser, pgn_trim) {
	const char* tests[] = {
	    "Surname, Name  (2800)",                //
	    "Surname, Name  (2800)       ",         //
	    "    Surname, Name  (2800)",            //
	    "    Surname, Name  (2800)       ",     //
	    "\vSurname, Name  (2800)\r\n",          //
	    " \t\n   Surname, Name  (2800)  \r\v  " //
	};

	for (auto str : tests) {
		auto str_view = std::make_pair(str, str + std::strlen(str));
		size_t n_newlines = std::count(str_view.first, str_view.second, '\n');
		EXPECT_EQ(n_newlines, pgn::trim(str_view));
		EXPECT_TRUE(std::equal(str_view.first, str_view.second, tests[0]));
	}
}

TEST(Test_PgnParser, date_parsePGNTag) {
	auto test = [](const char* str, const char* expected) {
		char buf[16];
		date_DecodeToString(date_parsePGNTag(str, std::strlen(str)), buf);
		EXPECT_STREQ(buf, expected);
	};

	test("2018.01.01", "2018.01.01");
	test("2018.01.31", "2018.01.31");
	test("2018.08.31", "2018.08.31");
	test("2018.08.32", "2018.08.??");
	test("2018.01.00", "2018.01.??");
	test("2018.01.32", "2018.01.??");
	test("2018.02.30", "2018.02.??");
	test("2018.04.31", "2018.04.??");
	test("2018.11.31", "2018.11.??");
	test("2018.01aaa", "2018.01.??");
	test("2018.01", "2018.01.??");

	test("2018.12.10", "2018.12.10");
	test("2018.13.10", "2018.??.10");
	test("2018.00.10", "2018.??.10");
	test("2018aaaa10", "2018.??.10");
	test("2018.13.33", "2018.??.??");
	test("2018.12", "2018.12.??");
	test("2018.00", "2018.??.??");
	test("2018.13", "2018.??.??");
	test("2018", "2018.??.??");

	test("2018.07.17", "2018.07.17");
	test("0001/01/01", "0001.01.01");
	test("2047/12/31", "2047.12.31");
	test("0000/12/31", "????.12.31");
	test("2048/01/01", "????.01.01");
	test("0000", "????.??.??");
	test("192", "1920.??.??");
	test("", "????.??.??");

	test("2020.02.28", "2020.02.28");
	test("2018.02.28", "2018.02.28");
	test("2012.02.28", "2012.02.28");
	test("2000.02.28", "2000.02.28");
	test("1000.02.28", "1000.02.28");
	test("2020.02.29", "2020.02.29");
	test("2018.02.29", "2018.02.??");
	test("2012.02.29", "2012.02.29");
	test("2000.02.29", "2000.02.29");
	test("1900.02.29", "1900.02.??");
	test("1000.02.29", "1000.02.??");
}

TEST(Test_PgnParser, TagPairs) {
	std::vector<const char*> v(11);
	v[0] = "[Event \"Rated Bul]let game\"]";
	v[1] = "[Site \"Test Site\"]";
	v[2] = "[Date \"2017.08.31\"]";
	v[3] = "[Round \"Test Round\"]";
	v[4] = R"([White "White "Senpai"])";
	v[5] = R"([Black "Black \ "Minister"])";
	v[6] = "[Result \"0-1\"]";
	v[7] = "[WhiteElo \"1106\"]";
	v[8] = "[BlackElo \"971\"]";
	v[9] = "[ECO \"B01\"]";
	v[10] = "[EventDate \"2017.07.30\"]";
	v.push_back("[UTCDate \"2017.08.31\"]");
	v.push_back("[UTCTime \"22:00:00\"]");
	v.push_back("[WhiteRatingDiff \"+28\"]");
	v.push_back("[BlackRatingDiff \"-7\"]");
	v.push_back("[WhiteCountry \"Italy\"]");
	v.push_back("[BlackCountry \" Test Country \"]");
	v.push_back("[Annotator \" none\"]");
	v.push_back("[PlyCount \"-5\"]");
	v.push_back("[Opening \"Scandinavian Defense\"]");
	v.push_back("[Variation \"\"]");
	v.push_back("[Setup \"\"]");
	v.push_back("[Source \"____\"]");
	v.push_back("[SetUp \"test_case_setup\"]");
	v.push_back("[TimeControl \"60+0\"]");
	v.push_back("[Termination \"Time forfeit\"]");
	v.push_back("[WhiteRating \"10\"]");
	v.push_back("[BlackRating \"20\"]");
	v.push_back("[WhiteRapid \"100\"]");
	v.push_back("[BlackRapid \"200\"]");
	v.push_back("[WhiteICCF \"999\"]");
	v.push_back("[BlackICCF \"666\"]");
	v.push_back("[WhiteDWZ \"1800\"]");
	v.push_back("[BlackDWZ \"1735\"]");
	v.push_back("[WhiteECF \"2777\"]");
	v.push_back("[BlackECF \"2999\"]");
	v.push_back("");
	v.push_back("0-1");
	v.push_back("");

	auto test = [](const auto& v_src, const auto& v_expect, bool errors) {
		std::string expect;
		for (auto elem : v_expect) {
			expect.append(elem);
			expect.push_back('\n');
		}
		std::string src;
		for (auto elem : v_src) {
			src.append(elem);
			src.push_back('\n');
		}

		PgnParseLog parseLog;
		Game game;
		game.SetPgnFormat(PGN_FORMAT_Plain);
		game.ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS |
		                   PGN_STYLE_COMMENTS);

		ASSERT_TRUE(pgnParseGame(src.c_str(), src.size(), game, parseLog));
		ASSERT_EQ(!parseLog.log.size(), !errors);
		auto pgn = game.WriteToPGN(75, true);
		src.assign(pgn.first, pgn.second);
		ASSERT_STREQ(src.c_str(), expect.c_str());
	};

	test(v, v, false);

	{ // Correct order of the Seven Tag Roster.
		auto tmp = v;
		std::shuffle(tmp.begin(), tmp.begin() + 8,
		             std::mt19937{std::random_device{}()});
		test(tmp, v, false);
	}
	{ // Use UTCDate only if Date is missing
		auto tmp_e = v;
		tmp_e[2] = "[Date \"2017.08.30\"]";
		auto tmp_s = tmp_e;
		test(tmp_s, tmp_e, false);
		tmp_s.erase(tmp_s.begin() + 2);
		test(tmp_s, v, false);
	}
	{ // Trim white spaces not in the string token.
		auto tmp_s = v;
		auto tmp_e = v;
		tmp_e[1] = "[Site \"  Site test  \"]";
		tmp_s[1] = "[Site\n\"  Site test  \"]";
		test(tmp_s, tmp_e, false);
		tmp_s[1] = "[Site  \n\t\v\r\n\n   \"  Site\ntest  \"   \n \t \v \r  ]";
		test(tmp_s, tmp_e, false);
	}
	{ // Unescape \\ to \ and \" to "
		auto tmp_s = v;
		auto tmp_e = v;
		tmp_s[4] = R"([White "White\"Senpai"])";
		tmp_e[4] = R"([White "White"Senpai"])";
		tmp_s[5] = R"([Black "Black\"]Minister\\"])";
		tmp_e[5] = R"([Black "Black"]Minister\"])";
		test(tmp_s, tmp_e, false);
	}
	{ // Wrong result.
		auto tmp_e = v;
		tmp_e[6] = "[Result \"*\"]";
		tmp_e[tmp_e.size() - 2] = "*";
		test(tmp_e, tmp_e, false);
		auto tmp_s = tmp_e;
		tmp_s.resize(tmp_e.size() - 2);
		tmp_s[6] = "[Result \"unknown\"]";
		test(tmp_s, tmp_e, true);
		tmp_s[6] = "[Result \"\"]";
		test(tmp_s, tmp_e, true);
		tmp_s.erase(tmp_s.begin() + 6);
		test(tmp_s, tmp_e, false);
	}
	{ // Max tag pair length.
		auto test_ok = [&](const char* str) {
			auto tmp_s = v;
			tmp_s.insert(tmp_s.begin() + 11, str);
			test(tmp_s, tmp_s, false);
		};
		auto test_fail = [&](const char* str) {
			auto tmp_s = v;
			tmp_s.insert(tmp_s.begin() + 11, str);
			test(tmp_s, v, true);
		};

		auto max_value = 239;
		std::string str("[_ \"\"]");
		str.insert(str.end() - 2, max_value, 'c');
		test_ok(str.c_str());
		str.insert(str.end() - 2, 1, 'c');
		test_fail(str.c_str());

		auto max_tagname = 240;
		str = "[ \"\"]";
		str.insert(str.begin() + 1, max_tagname, 'c');
		test_ok(str.c_str());
		str.insert(str.begin() + 1, 1, 'c');
		test_fail(str.c_str());

		auto max_total = 120;
		str = "[ \"\"]";
		str.insert(str.begin() + 1, max_total, 'c');
		str.insert(str.end() - 2, max_total, 'c');
		test_ok(str.c_str());
		str.insert(str.end() - 2, 1, 'c');
		test_fail(str.c_str());

		str = "[ \"no empty name\"]";
		test_fail(str.c_str());
	}
	{ // Tag on multiple lines
		auto tmp_s = v;
		tmp_s[1] = "\n[\n\n Site \n\n\n \"Test Site\"  \n \n  ]   \n";
		test(tmp_s, v, false);
	}
	{ // Multiple tags on the same line and [Unclosed_string_token "value\"]
		auto tmp_s = v;
		auto tmp_e = v;

		tmp_e.insert(tmp_e.begin() + 11, "[Bad \"tag on the same line\"]");
		tmp_s[3] = "[Round \"Test Round\"] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);
		tmp_s[3] = "[Round Test Round] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);
		tmp_s[3] = "[Round Test Round\"] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);

		tmp_e[3] = "[Round \"Test Round\\\"]";
		tmp_s[3] = "[Round Test Round\\] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);
		tmp_s[3] = "[Round Test Round\\\"] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);

		tmp_e.erase(tmp_e.begin() + 11);
		tmp_s[3] = "[Round Test Round\\]";
		test(tmp_s, tmp_e, false);
		tmp_s[3] = "[Round Test Round\\\"]";
		test(tmp_s, tmp_e, false);
		tmp_s[3] = "[Round \"Test Round\\\"]";
		test(tmp_s, tmp_e, false);

		tmp_e[3] = "[Round \"Test Round\"] [Bad tag on the same line\"]";
		tmp_s[3] = "[Round \"Test Round\\\"] [Bad tag on the same line]";
		test(tmp_s, tmp_e, false);
	}
}
