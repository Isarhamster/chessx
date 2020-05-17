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

#include "scidbase.h"
#include "pgnparse.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <gtest/gtest.h>

template <typename TCont>
std::string encodePgn(const TCont& game)
{
	std::string res;

	struct {
		const char sep = ' ';
		const char endl = '\n';
		const char RAVstart = '(';
		const char RAVend = ')';
		const char commentStart = '{';
		const char commentEnd = '}';
		const char* MoveNumEndW = ".";
		const char* MoveNumEndB = "...";
	} token;

	// 4.3: Lines with 80 or more printing characters are strongly discouraged
	// 8.2.1: Neither the first or the last character on an export format PGN line is a	space.
	const size_t lineLen = 80;
	auto formatLine = [&](std::string& src, std::string& dest) {
		size_t skip_start = src.find_first_not_of(token.sep);
		src.erase(0, skip_start);
		while (src.size() > lineLen) {
			size_t line_break = src.rfind(token.sep, lineLen);
			if (line_break == std::string::npos) {
				//TODO: break last comment
				line_break = src.find(token.sep, lineLen);
				if (line_break == std::string::npos) break;
			}

			dest.append(src.begin(), src.begin() + line_break);
			dest += token.endl;
			src.erase(0, line_break + 1);
		}
	};

	auto FEN_getColor = [](const std::string& FEN) {
		colorT res = NOCOLOR;
		size_t toMove = FEN.find(' ');
		if (toMove != std::string::npos && ++toMove < FEN.size()) {
			if (FEN[toMove] == 'w') res = WHITE;
			else if (FEN[toMove] == 'b') res = BLACK;
		}
		return res;
	};
	auto FEN_getMoveNum = [](const std::string& FEN) {
		std::string res;
		size_t moveNum = FEN.rfind(' ');
		if (moveNum != std::string::npos && ++moveNum < FEN.size()) {
			res = FEN.substr(moveNum, std::string::npos);
		}
		return res;
	};

	std::vector<uint> RAVid = { 0 };
	auto handleRAV = [&](uint RAVdepth, const uint& RAVnum) {
		std::pair<uint, uint> res = { 0,0 };
		if (++RAVdepth > RAVid.size()) {
			RAVid.push_back(RAVnum);
			ASSERT(RAVdepth == RAVid.size());
			res.first++;
		} else {
			while (RAVdepth < RAVid.size()) {
				RAVid.pop_back();
				res.second++;
			}

			if (RAVnum != RAVid.back()) {
				// New variation at the same depth
				RAVid.back() = RAVnum;
				res.first++;
				res.second++;
			}
		}
		return res;
	};

	bool forceMoveNum = false;
	auto needMoveNum = [&forceMoveNum](colorT lastCol) {
		// 8.2.2.2: Export format move number indications
		//
		// There are two export format move number indication formats, one for use
		// appearing immediately before a white move element and one for use appearing
		// immediately before a black move element.A white move number indication is
		// formed from the integer giving the fullmove number with a single period
		// character appended.A black move number indication is formed from the integer
		// giving the fullmove number with three period characters appended.
		//
		// All white move elements have a preceding move number indication.A black move
		// element has a preceding move number indication only in two cases : first, if
		// there is intervening annotation or commentary between the black move and the
		// previous white move; and second, if there is no previous white move in the
		// special case where a game starts from a position where Black is the active
		// player.
		//
		// There are no other cases where move number indications appear in PGN export
		// format.
		bool res = (lastCol == BLACK || forceMoveNum);
		forceMoveNum = false;
		return res;
	};

	std::string line;
	for (auto& pos : game) {
		auto ravs = handleRAV(pos.RAVdepth, pos.RAVnum);
		for (uint i = 0; i < ravs.second; i++) {
			line += token.sep;
			line += token.RAVend;
			forceMoveNum = true;
		}
		for (uint i = 0; i < ravs.first; i++) {
			line += token.sep;
			line += token.RAVstart;
			forceMoveNum = true;
		}

		if (pos.lastMoveSAN == "") {
			if (pos.FEN != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
				if (!res.empty() || !line.empty()) return "error";
				res += "[FEN \"";
				res += pos.FEN;
				res += '\"';
				res += token.endl;
				res += token.endl;
			}
		} else {
			colorT lastCol = FEN_getColor(pos.FEN);
			ASSERT(lastCol != NOCOLOR);
			bool printMoveNum = needMoveNum(lastCol);
			if (printMoveNum) {
				std::string num = FEN_getMoveNum(pos.FEN);
				const char* endMoveNum = token.MoveNumEndW;
				if (lastCol == WHITE) {
					endMoveNum = token.MoveNumEndB;
					int temp = atoi(num.c_str());
					num = std::to_string(--temp);
				}
				line += token.sep;
				line += num;
				line += endMoveNum;
			}

			line += token.sep;
			line += pos.lastMoveSAN;

			for (auto& e : pos.NAGs) {
				line += token.sep;
				line += '$';
				line += std::to_string(e);
				forceMoveNum = true;
			}
		}

		if (!pos.comment.empty()) {
			line += token.sep;
			line += token.commentStart;
			size_t prevLen = line.size();
			line += pos.comment;

			std::string::iterator strip_begin = line.begin() + prevLen;
			line.erase(std::remove(strip_begin, line.end(), token.endl), line.end());
			// 4.2: Tab characters, both horizontal and vertical, are not
			//      permitted in the export	format.
			line.erase(std::remove(strip_begin, line.end(), '\t'), line.end());
			line.erase(std::remove(strip_begin, line.end(), '\v'), line.end());
			// 5:   Brace comments do not nest;
			line.erase(std::remove(strip_begin, line.end(), token.commentEnd), line.end());

			line += token.commentEnd;
			forceMoveNum = true;
		}

		formatLine(line, res);
	}
	auto ravs = handleRAV(0, 0);
	for (uint i = 0; i < ravs.second; i++) {
		line += token.sep;
		line += token.RAVend;
	}

	formatLine(line, res);
	res.append(line.begin(), line.end());

	return res;
}


class Test_Scidbase : public ::testing::Test {
	virtual void SetUp() {
		test_GamePos.push_back(makeGamePos
			(0, 0, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", ""));
		test_GamePos.push_back(makeGamePos
			(0, 0, "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1", "d4"));
		test_GamePos.push_back(makeGamePos
			(0, 0, "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 2", "d5"));
		test_GamePos.push_back(makeGamePos
			(0, 0, "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq - 0 2", "c4"));
		test_GamePos.push_back(makeGamePos
			(1, 0, "rnbqkbnr/ppp1pppp/8/3p4/3P4/5N2/PPP1PPPP/RNBQKB1R b KQkq - 1 2", "Nf3"));
		test_GamePos.push_back(makeGamePos
			(1, 0, "rnbqkb1r/ppp1pppp/5n2/3p4/3P4/5N2/PPP1PPPP/RNBQKB1R w KQkq - 2 3", "Nf6"));
		test_GamePos.push_back(makeGamePos
			(2, 0, "rn1qkbnr/ppp1pppp/8/3p4/3P2b1/5N2/PPP1PPPP/RNBQKB1R w KQkq - 2 3", "Bg4"));
		test_GamePos.push_back(makeGamePos
			(1, 0, "rnbqkb1r/ppp1pppp/5n2/3p4/3P4/2P2N2/PP2PPPP/RNBQKB1R b KQkq - 0 3", "c3"));
		test_GamePos.push_back(makeGamePos
			(1, 1, "rnbqkbnr/ppp1pppp/8/3p4/3P4/6P1/PPP1PP1P/RNBQKBNR b KQkq - 0 2", "g3"));
		test_GamePos.push_back(makeGamePos
			(1, 1, "rnbqkb1r/ppp1pppp/5n2/3p4/3P4/6P1/PPP1PP1P/RNBQKBNR w KQkq - 1 3", "Nf6"));
		test_GamePos.push_back(makeGamePos
			(1, 1, "rnbqkb1r/ppp1pppp/5n2/3p4/3P4/6P1/PPP1PPBP/RNBQK1NR b KQkq - 2 3", "Bg2"));
		test_GamePos.push_back(makeGamePos
			(2, 0, "rnbqkb1r/ppp1pppp/5n2/3p4/3P4/5NP1/PPP1PP1P/RNBQKB1R b KQkq - 2 3", "Nf3"));

		auto& s = test_pgnLong;
		s += "{This time the money for the world championship was no problem. The Berliner\n";
		s += "Schachgesellschaft supplied 2500 Mark and Nardus 5000 Frank.} 1. d4 d5 2. c4 e6\n";
		s += "3. Nc3 c5 4. cxd5 exd5 5. Nf3 Be6 6. e4 $6 ( 6. Bg5 Nf6 7. e3 {led to a\n";
		s += "variation of the Tarrasch (Lasker-Lawrence, Cambridge Springs 1904).} ) 6...\n";
		s += "dxe4 7. Nxe4 Nc6 $1 8. Be3 ( 8. Nxc5 Bxc5 9. dxc5 Qxd1+ 10. Kxd1 O-O-O+ {Black\n";
		s += "has the better ending.} ) 8... cxd4 9. Nxd4 ( 9. Bxd4 Bb4+ $1 ( 9... Nxd4\n";
		s += "{(Tarrasch)} ) 10. Bc3 Qe7 ) 9... Qa5+ 10. Nc3 $2 ( 10. Qd2 Bb4 11. Nc3 O-O-O\n";
		s += "12. Nxc6 $1 12... bxc6 13. Qc1 Nf6 14. a3 {and a hard defence.} ) 10... O-O-O $1\n";
		s += "11. a3 {White is lost, because knight d4 lacks a sufficient support.} 11... Nh6\n";
		s += "$2 ( 11... Bc5 $1 12. b4 Bxd4 $1 13. Bxd4 Qg5 14. h4 ( 14. Ne2 Nxd4 15. Nxd4\n";
		s += "Qe5+ ) 14... Qg6 $19 {(Tarrasch).} ) 12. b4 Qe5 ( 12... Bxb4 $6 13. axb4 Qxb4\n";
		s += "14. Qc1 Rxd4 15. Ra4 $5 ( 15. Bxd4 Qxd4 16. Be2 ) 15... Rd1+ $1 16. Qxd1 Qxc3+\n";
		s += "17. Bd2 Qe5+ 18. Be2 {White takes the initiative.} ) 13. Ncb5 Nf5 $1 ( 13... a6\n";
		s += "14. Qc1 $1 14... axb5 15. Nxc6 bxc6 16. Qxc6+ Qc7 17. Qa8+ Qb8 $1 ( 17... Kd7\n";
		s += "18. Bxb5+ {is risky} ) 18. Qc6+ $1 $11 ( 18. Rc1+ $2 18... Bc4 ) ) 14. Rc1 $1\n";
		s += "14... Nxe3 15. fxe3 Qxe3+ 16. Be2 Be7 $1 ( 16... Rxd4 $6 17. Nxd4 Bxb4+\n";
		s += "{sacrifices too much.} 18. axb4 Rd8 19. b5 Rxd4 20. Qc2 Rc4 21. Qxc4 Bxc4 22.\n";
		s += "Rxc4 {and 23.bxc6 is good for White.} ) ( 16... Bb3 $2 17. Nxa7+ $1 ) 17. Rc3 $1\n";
		s += "( 17. Nxa7+ Kb8 18. Naxc6+ bxc6 19. Rxc6 Kb7 20. Qc2 $1 20... Rc8 $1 ( 20...\n";
		s += "Bh4+ $11 ) 21. Qc3 Qxc3+ 22. Rxc3 Kb6 {leads to a favourable endgame for Black.}\n";
		s += ") 17... Bh4+ $6 ( 17... Qxc3+ $1 18. Nxc3 Nxd4 {and a fine game (Tarrasch).} )\n";
		s += "18. g3 Qe4 $2 ( 18... Qxc3+ {is still okay.} ) 19. O-O Bf6 20. Rxf6 $1 {Suddenly\n";
		s += "White strikes.} 20... gxf6 21. Bf3 Qe5 22. Nxa7+ Kc7 23. Naxc6 bxc6 24. Rxc6+\n";
		s += "Kb8 25. Rb6+ Kc8 26. Qc1+ Kd7 27. Nxe6 fxe6 28. Rb7+ Ke8 29. Bc6+ {Lasker came\n";
		s += "back from a lost opening.}";
	}

protected:
	std::vector<gamepos::GamePos> test_GamePos;
	std::string test_pgnLong;
	const std::string test_pgnShort =
		"1. d4 d5 2. c4 ( 2. Nf3 Nf6 ( 2... Bg4 ) 3. c3 ) ( 2. g3 Nf6 3. Bg2 ( 3. Nf3 ) )";

	gamepos::GamePos makeGamePos(uint RAVdepth, uint RAVnum, const char* FEN, const char* SAN)
	{
		gamepos::GamePos res;
		res.RAVdepth = RAVdepth;
		res.RAVnum = RAVnum;
		res.FEN = FEN;
		res.lastMoveSAN = SAN;
		return res;
	}
};

auto collectPositions(const scidBaseT& dbase, gamenumT gnum) {
	auto ie_bounds = dbase.getIndexEntry_bounds(gnum);
	auto ie = dbase.getIndexEntry(gnum);
	Game game;
	if (ie_bounds && ie && dbase.getGame(*ie_bounds, game) == OK)
		return gamepos::collectPositions(game);

	return decltype(gamepos::collectPositions(game))();
}

TEST_F(Test_Scidbase, getGamePos1) {
	Game game;
	PgnParseLog parseLog;
	ASSERT_TRUE(pgnParseGame(test_pgnShort.c_str(), test_pgnShort.size(), game,
	                         parseLog));
	ASSERT_STREQ(parseLog.log.c_str(), "");

	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));
	ASSERT_EQ(OK, dbase.saveGame(&game));
	ASSERT_NE(nullptr, dbase.getIndexEntry_bounds(0));

	auto gamepos = collectPositions(dbase, 0);
	EXPECT_EQ(test_GamePos.size(), gamepos.size());
	size_t n = std::min(test_GamePos.size(), gamepos.size());
	for (size_t i = 0; i < n; i++) {
		const auto& e1 = test_GamePos[i];
		const auto& e2 = gamepos[i];
		EXPECT_EQ(e1.RAVdepth, e2.RAVdepth);
		EXPECT_EQ(e1.RAVnum, e2.RAVnum);
		EXPECT_EQ(e1.FEN, e2.FEN);
		EXPECT_EQ(e1.NAGs, e2.NAGs);
		EXPECT_EQ(e1.comment, e2.comment);
		EXPECT_EQ(e1.lastMoveSAN, e2.lastMoveSAN);
	}
}

TEST_F(Test_Scidbase, getGamePos2) {
	Game game;
	PgnParseLog parseLog;
	ASSERT_TRUE(pgnParseGame(test_pgnShort.c_str(), test_pgnShort.size(), game,
	                         parseLog));
	ASSERT_STREQ(parseLog.log.c_str(), "");

	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));
	ASSERT_EQ(OK, dbase.saveGame(&game));
	ASSERT_NE(nullptr, dbase.getIndexEntry_bounds(0));

	auto gamepos = collectPositions(dbase, 0);
	EXPECT_EQ(test_pgnShort.c_str(), encodePgn(gamepos));
}

TEST_F(Test_Scidbase, getGamePos3) {
	Game game;
	PgnParseLog parseLog;
	ASSERT_TRUE(pgnParseGame(test_pgnLong.c_str(), test_pgnLong.size(), game,
	                         parseLog));
	ASSERT_STREQ(parseLog.log.c_str(), "");

	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));
	ASSERT_EQ(OK, dbase.saveGame(&game));
	ASSERT_NE(nullptr, dbase.getIndexEntry_bounds(0));

	auto gamepos = collectPositions(dbase, 0);
	EXPECT_EQ(test_pgnLong.c_str(), encodePgn(gamepos));
}

TEST_F(Test_Scidbase, saveGame) {
	Game game0;
	Game game1;
	PgnParseLog parseLog;
	ASSERT_TRUE(pgnParseGame(test_pgnShort.c_str(), test_pgnShort.size(), game0,
	                         parseLog));
	ASSERT_TRUE(pgnParseGame(test_pgnLong.c_str(), test_pgnLong.size(), game1,
	                         parseLog));
	ASSERT_STREQ(parseLog.log.c_str(), "");

	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));
	EXPECT_EQ(nullptr, dbase.getIndexEntry_bounds(0));
	HFilter dbfilter = dbase.getFilter("dbfilter");
	EXPECT_EQ(0U, dbfilter->size());

	{
		ASSERT_EQ(OK, dbase.saveGame(&game0));
		EXPECT_EQ(1U, dbase.numGames());
		EXPECT_EQ(1U, dbfilter->size());
		auto ie0 = dbase.getIndexEntry_bounds(0);
		auto ie1 = dbase.getIndexEntry_bounds(1);
		EXPECT_NE(nullptr, ie0);
		EXPECT_EQ(nullptr, ie1);
		EXPECT_NE(ie0, ie1);
		auto gamepos = collectPositions(dbase, 0);
		EXPECT_EQ(test_pgnShort, encodePgn(gamepos));
	}
	{
		ASSERT_EQ(OK, dbase.saveGame(&game1));
		EXPECT_EQ(2U, dbase.numGames());
		EXPECT_EQ(2U, dbfilter->size());
		auto ie0 = dbase.getIndexEntry_bounds(0);
		auto ie1 = dbase.getIndexEntry_bounds(1);
		EXPECT_NE(nullptr, ie0);
		EXPECT_NE(nullptr, ie1);
		EXPECT_NE(ie0, ie1);
		auto gamepos = collectPositions(dbase, 0);
		EXPECT_EQ(test_pgnShort, encodePgn(gamepos));
		gamepos = collectPositions(dbase, 1);
		EXPECT_EQ(test_pgnLong, encodePgn(gamepos));
	}
	{
		ASSERT_EQ(OK, dbase.saveGame(&game1, 0));
		EXPECT_EQ(2U, dbase.numGames());
		EXPECT_EQ(2U, dbfilter->size());
		auto ie0 = dbase.getIndexEntry_bounds(0);
		auto ie1 = dbase.getIndexEntry_bounds(1);
		EXPECT_NE(nullptr, ie0);
		EXPECT_NE(nullptr, ie1);
		EXPECT_NE(ie0, ie1);
		auto gamepos = collectPositions(dbase, 0);
		EXPECT_EQ(test_pgnLong, encodePgn(gamepos));
		gamepos = collectPositions(dbase, 1);
		EXPECT_EQ(test_pgnLong, encodePgn(gamepos));
	}
}

TEST_F(Test_Scidbase, importGames) {
	Game game0;
	Game game1;
	PgnParseLog parseLog;
	ASSERT_TRUE(pgnParseGame(test_pgnShort.c_str(), test_pgnShort.size(), game0,
	                         parseLog));
	ASSERT_TRUE(pgnParseGame(test_pgnLong.c_str(), test_pgnLong.size(), game1,
	                         parseLog));
	ASSERT_STREQ(parseLog.log.c_str(), "");


	scidBaseT srcBase;
	ASSERT_EQ(OK, srcBase.open("MEMORY", FMODE_Create, "Memory"));
	ASSERT_EQ(OK, srcBase.saveGame(&game0));
	ASSERT_EQ(OK, srcBase.saveGame(&game1));
	ASSERT_EQ(OK, srcBase.saveGame(&game1));
	ASSERT_EQ(3U, srcBase.numGames());
	HFilter srcFilter = srcBase.getFilter("dbfilter");
	srcFilter->erase(1);

	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));
	EXPECT_EQ(nullptr, dbase.getIndexEntry_bounds(0));
	HFilter dbfilter = dbase.getFilter("dbfilter");

	EXPECT_EQ(ERROR_BadArg, dbase.importGames(&dbase, dbfilter, Progress()));
	EXPECT_EQ(0U, dbase.numGames());
	EXPECT_EQ(0U, dbfilter->size());

	{
		EXPECT_EQ(OK, dbase.importGames(&srcBase, srcFilter, Progress()));
		EXPECT_EQ(2U, dbase.numGames());
		EXPECT_EQ(2U, dbfilter->size());
		auto ie0 = dbase.getIndexEntry_bounds(0);
		auto ie1 = dbase.getIndexEntry_bounds(1);
		auto ie2 = dbase.getIndexEntry_bounds(2);
		EXPECT_NE(nullptr, ie0);
		EXPECT_NE(nullptr, ie1);
		EXPECT_EQ(nullptr, ie2);
		EXPECT_NE(ie0, ie1);
		auto gamepos = collectPositions(dbase, 0);
		EXPECT_EQ(test_pgnShort, encodePgn(gamepos));
		gamepos = collectPositions(dbase, 1);
		EXPECT_EQ(test_pgnLong, encodePgn(gamepos));
	}
}

TEST_F(Test_Scidbase, new_compose_delete_get_Filter) {
	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("MEMORY", FMODE_Create, "Memory"));

	std::vector<std::pair<std::string, bool>> tests = {
	    {" dbfilter", false},   {"dbfilter ", false}, {"dbfilter", true},
	    {"tree", true},         {"+dbfilter", false}, {"++dbfilter", false},
	    {"++dbfilter+", false}, {"", false},          {"+", false},
	    {"++", false},          {"+++", false},       {" +", false},
	    {"+ ", false}
	};

	auto check = [&]() {
		std::string mask;
		for (const auto& e : tests) {
			bool valid = dbase.getFilter(e.first) != 0;
			EXPECT_EQ(e.second, valid);

			auto composed = dbase.composeFilter(e.first, mask);
			EXPECT_EQ(valid, dbase.getFilter(composed) != nullptr);
			if (valid) {
				auto [f1, f2] = dbase.getFilterComponents(composed);
				EXPECT_EQ(f1, e.first);
				EXPECT_EQ(f2, mask);
				EXPECT_EQ(e.first, dbase.composeFilter(composed, ""));

				mask = e.first;
			}
		}
	};

	check();

	for (size_t i = 0; i < 100; i++) {
		tests.emplace_back(dbase.newFilter(), true);
		auto unique = std::find(tests.begin(), tests.end(), tests.back());
		EXPECT_EQ(tests.end(), ++unique);
		check();
	}

	for (size_t i = 0, n = tests.size(); i < n; i += 4) {
		tests[i].second = false;
		dbase.deleteFilter(tests[i].first.c_str());
		check();
	}
}
