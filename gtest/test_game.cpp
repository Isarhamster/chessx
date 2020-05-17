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

#include "game.h"
#include "pgnparse.h"
#include "scidbase.h"
#include <algorithm>
#include <bytebuf.h>
#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <random>

namespace {

const char* gameUTF8 = SCID_TESTDIR "res_gameUTF8.pgn";
const char* gameLatin1 = SCID_TESTDIR "res_gameLatin1.pgn";
const char* gameLatin1Conv = SCID_TESTDIR "res_gameLatin1expected.pgn";

} // namespace

TEST(Test_Game, clone) {
	for (auto filename : {gameUTF8, gameLatin1, gameLatin1Conv}) {

		scidBaseT dbase;
		ASSERT_EQ(OK, dbase.open("PGN", FMODE_Both, filename));
		ASSERT_NE(nullptr, dbase.getIndexEntry_bounds(0));

		Game game;
		ASSERT_EQ(OK, dbase.getGame(*dbase.getIndexEntry(0), game));

		std::mt19937 re(std::random_device{}());
		game.MoveToLocationInPGN(std::uniform_int_distribution<>{0, 500}(re));

		std::unique_ptr<Game> clone{game.clone()};

		ASSERT_EQ(clone->GetPgnOffset(), game.GetPgnOffset());

		auto board = game.currentPos()->GetBoard();
		ASSERT_TRUE(
		    std::equal(board, board + 66, clone->currentPos()->GetBoard()));

		char sanGame[12] = {};
		game.GetSAN(sanGame);
		char sanClone[12] = {};
		clone->GetSAN(sanClone);
		ASSERT_STREQ(sanGame, sanClone);

		game.SetPgnFormat(PGN_FORMAT_Plain);
		game.ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS |
		                   PGN_STYLE_COMMENTS | PGN_STYLE_SCIDFLAGS);
		auto pgnGame = game.WriteToPGN(75, true);

		clone->SetPgnFormat(PGN_FORMAT_Plain);
		clone->ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS |
		                     PGN_STYLE_COMMENTS | PGN_STYLE_SCIDFLAGS);
		auto pgnClone = clone->WriteToPGN(75, true);

		ASSERT_TRUE(std::equal(pgnClone.first, pgnClone.first + pgnClone.second,
		                       pgnGame.first, pgnGame.first + pgnGame.second));
	}
}

TEST(Test_Game, locationInPGN) {
	for (auto filename : {gameUTF8, gameLatin1, gameLatin1Conv}) {

		scidBaseT dbase;
		ASSERT_EQ(OK, dbase.open("PGN", FMODE_Both, filename));
		ASSERT_NE(nullptr, dbase.getIndexEntry_bounds(0));

		Game game;
		auto bufGame = dbase.getGame(*dbase.getIndexEntry(0));
		ASSERT_TRUE(bufGame);
		ASSERT_EQ(OK, game.Decode(bufGame));

		unsigned location = 1;
		game.MoveToStart();
		while (true) {
			++location;
			errorT errForward = game.MoveForwardInPGN();
			if (errForward != OK) {
				ASSERT_EQ(errForward, game.MoveToLocationInPGN(location));
				break;
			}

			ASSERT_EQ(location, game.GetLocationInPGN());
			if (!game.AtVarStart()) {
				ASSERT_EQ(location, game.GetPgnOffset());
			}

			std::string san = game.GetNextSAN();
			auto ply1 = game.GetCurrentPly();
			game.MoveToLocationInPGN(location);
			auto ply2 = game.GetCurrentPly();
			ASSERT_EQ(ply1, ply2);
			ASSERT_EQ(location, game.GetLocationInPGN());
			ASSERT_TRUE(san == game.GetNextSAN());
		}
	}
}

TEST(Test_Game, MoveToStart_MoveToEnd) {
	scidBaseT dbase;
	ASSERT_EQ(OK, dbase.open("PGN", FMODE_Both, gameUTF8));
	auto ie = dbase.getIndexEntry_bounds(0);
	ASSERT_NE(nullptr, ie);

	auto randomEngine = std::mt19937(std::random_device{}());
	auto distribution = std::uniform_int_distribution<>{2, 500};
	Game game;
	ASSERT_EQ(OK, dbase.getGame(*ie, game));

	for (int i = 0; i < 10; i++) {
		game.MoveToLocationInPGN(distribution(randomEngine));
		ASSERT_NE(0, game.GetCurrentPly());
		game.MoveToStart(); // Move to start from any position
		EXPECT_EQ(0, game.GetCurrentPly());
	}
	game.MoveToStart(); // Move to start from start
	EXPECT_EQ(0, game.GetCurrentPly());
	game.MoveToEnd(); // Move to end from start
	EXPECT_EQ(149, game.GetCurrentPly());
	game.MoveToEnd(); // Move to end from end
	EXPECT_EQ(149, game.GetCurrentPly());
	for (int i = 0; i < 10; i++) {
		game.MoveToLocationInPGN(distribution(randomEngine));
		game.MoveToEnd(); // Move to end from any position
		EXPECT_EQ(149, game.GetCurrentPly());
	}
}

TEST(Test_Game, gamevisit) {
	Game game;

	// Expect to visit the STR even for an empty game
	std::vector<std::pair<std::string, std::string>> expected_STR = {
	    {"Event", ""}, {"Site", ""},  {"Date", "????.??.??"}, {"Round", ""},
	    {"White", ""}, {"Black", ""}, {"Result", "*"}};
	std::vector<std::pair<std::string, std::string>> result_STR;
	gamevisit::tags_STR(game, [&](const char* tag, const char* value) {
		result_STR.emplace_back(tag, value);
	});
	EXPECT_TRUE(std::equal(expected_STR.begin(), expected_STR.end(),
	                       result_STR.begin(), result_STR.end()));

	// Expect no extra tags for an empty game
	std::vector<std::pair<std::string, std::string>> expected_extra;
	std::vector<std::pair<std::string, std::string>> result_extra;
	gamevisit::tags_extra(game, [&](const char* tag, const char* value) {
		result_extra.emplace_back(tag, value);
	});
	EXPECT_TRUE(result_extra.size() == 0);

	// Set all possible tag-pair types and expect to visit them all
	expected_STR[4].second = "white player";
	game.SetWhiteStr(expected_STR[4].second.c_str());
	expected_STR[5].second = "black \\player\"";
	game.SetBlackStr(expected_STR[5].second.c_str());
	expected_STR[2].second = "2018.06.11";
	game.SetDate(date_parsePGNTag("2018.06.11", 10));
	const char* white_elo = "2800";
	expected_extra.emplace_back("WhiteRapid", white_elo);
	game.setRating(WHITE, "Rapid", 5, {white_elo, white_elo + 4});
	expected_extra.emplace_back("BlackElo", "2650");
	game.SetBlackElo(2650);
	expected_extra.emplace_back("ECO", "A01");
	game.SetEco(eco_FromString("A01"));
	expected_extra.emplace_back("EventDate", "2018.06.01");
	game.SetEventDate(date_parsePGNTag("2018.06.01", 10));
	expected_extra.emplace_back("UTCDate", "2018.06.10");
	game.AddPgnTag("UTCDate", "2018.06.10");
	expected_extra.emplace_back("UTF-8", u8"Hell\u00F6");
	game.AddPgnTag(expected_extra.back().first.c_str(),
	               expected_extra.back().second.c_str());
	expected_extra.emplace_back("special chars", R"(Escape\\\" and \n {}\")");
	game.AddPgnTag(expected_extra.back().first.c_str(),
	               expected_extra.back().second.c_str());
	expected_STR[6].second = "0-1";
	game.SetResult(RESULT_Black);
	expected_extra.emplace_back(
	    "FEN", "8/N2P1pk1/2n2q2/1P2pp2/5PN1/QKPp1P2/8/8 w - - 0 1");
	game.SetStartFen(expected_extra.back().second.c_str());
	expected_STR[0].second = "event nAme";
	game.SetEventStr("event nAme");
	expected_STR[3].second = "round 4";
	game.SetRoundStr("round 4");
	expected_STR[1].second = "a long site maybe in a long country";
	game.SetSiteStr("a long site maybe in a long country");

	// Expect to visit the STR (in order)
	result_STR.clear();
	gamevisit::tags_STR(game, [&](const char* tag, const char* value) {
		result_STR.emplace_back(tag, value);
	});
	auto it = result_STR.begin();
	for (auto& exp : expected_STR) {
		EXPECT_EQ(exp, *it++);
	}

	// Expect to visit all the extra tags
	result_extra.clear();
	gamevisit::tags_extra(game, [&](const char* tag, const char* value) {
		result_extra.emplace_back(tag, value);
	});
	it = result_extra.begin();
	for (auto& exp : expected_extra) {
		EXPECT_EQ(exp, *it++);
	}
}

TEST(Test_Game, empty_tag_name) {
	std::vector<unsigned char> encodedGame;
	{
		Game game;
		game.AddPgnTag("Normal tag ", "normal  value");
		game.AddPgnTag("", "empty tag name");
		game.AddPgnTag("Annotator", "common tag");
		EXPECT_EQ(game.GetExtraTags().size(), 3);

		IndexEntry ie;
		game.Encode(encodedGame, ie);
	}

	ByteBuffer bbuf(encodedGame.data(), encodedGame.size());
	int i = 0;
	bbuf.decodeTags([&i](auto tag_name, auto tag_value) {
		if (i++ == 0) {
			EXPECT_EQ(tag_name, "Normal tag ");
			EXPECT_EQ(tag_value, "normal  value");
		} else {
			EXPECT_EQ(tag_name, "Annotator");
			EXPECT_EQ(tag_value, "common tag");
		}
	});
	EXPECT_EQ(i, 2);
}

TEST(Test_Game, encodeFEN) {
	std::vector<unsigned char> encodedGame;
	const char* kiwipete =
	    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	{
		Game game;
		game.SetStartFen(kiwipete);
		IndexEntry ie;
		game.Encode(encodedGame, ie);
	}
	{
		ByteBuffer bbuf(encodedGame.data(), encodedGame.size());
		Game game;
		game.Decode(bbuf);
		game.MoveToStart();
		char str[1024];
		game.currentPos()->PrintFEN(str, FEN_ALL_FIELDS);
		EXPECT_STREQ(kiwipete, str);
	}
}

TEST(Test_Game, currentPosUCI_startpos) {
	std::string_view pgn = "1.d4 (1.e4 e5 ( 1...c5)) (1.c4) 1...d5 2.c4";
	Game game;
	pgn::parse_game({pgn.data(), pgn.data() + pgn.size()}, PgnVisitor{game});

	const std::pair<unsigned, const char*> expected[] = {
	    {0, "position startpos moves"},
	    {1, "position startpos moves"},
	    {2, "position startpos moves d2d4"},
	    {3, "position startpos moves"},
	    {4, "position startpos moves e2e4"},
	    {5, "position startpos moves e2e4 e7e5"},
	    {6, "position startpos moves e2e4"},
	    {7, "position startpos moves e2e4 c7c5"},
	    {8, "position startpos moves"},
	    {9, "position startpos moves c2c4"},
	    {10, "position startpos moves d2d4 d7d5"},
	    {11, "position startpos moves d2d4 d7d5 c2c4"}};
	for (auto [pos, str] : expected) {
		game.MoveToLocationInPGN(pos);
		EXPECT_EQ(str, game.currentPosUCI());
	}
}

TEST(Test_Game, currentPosUCI_fen) {
	std::string_view pgn =
	    "[FEN 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198]\n"
	    "198.Kd2 ( 198.Nxd3 a1=R+ 199.Kd2 cxd3 )198...a1=Q 199.Ke3 Qe1+ 0-1";
	Game game;
	pgn::parse_game({pgn.data(), pgn.data() + pgn.size()}, PgnVisitor{game});

	const std::pair<unsigned, const char*> expected[] = {
	    // clang-format off
	    {0, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves"},
	    {1, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves"},
	    {2, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves c1d2"},
	    {3, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves"},
	    {4, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves f2d3"},
	    {5, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves f2d3 a2a1r"},
	    {6, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves f2d3 a2a1r c1d2"},
	    {7, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves f2d3 a2a1r c1d2 c4d3"},
	    {8, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves c1d2 a2a1q"},
	    {9, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves c1d2 a2a1q d2e3"},
	    {10, "position fen 8/8/8/8/2p5/1k1p4/p4N2/2K5 w - - 0 198 moves c1d2 a2a1q d2e3 a1e1"}
	    // clang-format on
	};
	for (auto [pos, str] : expected) {
		game.MoveToLocationInPGN(pos);
		EXPECT_EQ(str, game.currentPosUCI());
	}
}

TEST(Test_Game, illegalPGN_Castling) {
	std::string_view pgn = "1.e4 e5 2.Nf3 Nf6 3.Be2 Be7 4.O-O O-O 5.O-O";
	Game game;
	PgnParseLog pgnLog;
	EXPECT_FALSE(pgnParseGame(pgn.data(), pgn.size(), game, pgnLog));
	EXPECT_FALSE(pgnLog.log.empty());
	char fen[256];
	game.MoveToEnd();
	game.currentPos()->PrintFEN(fen, FEN_ALL_FIELDS);
	EXPECT_STREQ(
	    fen, "rnbq1rk1/ppppbppp/5n2/4p3/4P3/5N2/PPPPBPPP/RNBQ1RK1 w - - 6 5");
	game.MoveBackup();
	game.currentPos()->PrintFEN(fen, FEN_ALL_FIELDS);
	EXPECT_STREQ(
	    fen, "rnbqk2r/ppppbppp/5n2/4p3/4P3/5N2/PPPPBPPP/RNBQ1RK1 b kq - 5 4");
}

TEST(Test_Game, illegalPGN_KingCapture) {
	std::string_view pgn = "1.d4 e6 2.e4 Bb4+ 3.-- Be1";
	Game game;
	PgnParseLog pgnLog;
	EXPECT_FALSE(pgnParseGame(pgn.data(), pgn.size(), game, pgnLog));
	EXPECT_FALSE(pgnLog.log.empty());
	char fen[256];
	game.MoveToEnd();
	game.currentPos()->PrintFEN(fen, FEN_ALL_FIELDS);
	EXPECT_STREQ(
	    fen, "rnbqk1nr/pppp1ppp/4p3/8/1b1PP3/8/PPP2PPP/RNBQKBNR b KQkq - 2 3");
}

namespace {

/// Replace the move after the first comment with @e movecode
auto make_invalid(unsigned char movecode, std::string_view pgn) {
	std::vector<unsigned char> data;
	IndexEntry ie;
	Game g;
	pgn::parse_game({pgn.data(), pgn.data() + pgn.size()}, PgnVisitor{g});
	g.Encode(data, ie);
	auto comment_tag = std::find(data.begin(), data.end(), 12);
	if (comment_tag != data.end())
		*++comment_tag = movecode;
	return data;
}

template <typename DataT> std::string decode_gameview(DataT const& data) {
	auto bbuf = ByteBuffer{data.data() + 1, data.size()};
	auto fen = bbuf.decodeStartBoard().second;
	if (fen) {
		Position startPos;
		startPos.ReadFromFEN(fen);
		return GameView(bbuf, startPos).getMoveSAN(0, 99);
	}
	return GameView(bbuf).getMoveSAN(0, 99);
}

template <typename DataT> std::string decode_game(DataT const& data) {
	auto bbuf = ByteBuffer{data.data(), data.size()};
	Game game;
	game.Decode(bbuf);
	game.MoveToStart();
	std::string moves;
	do {
		moves += ' ';
		moves.append(game.GetNextSAN());
	} while (game.MoveForward() == OK);
	moves.erase(0, 1);
	return moves;
}
} // namespace

TEST(Test_Game, illegalSCID4_Castling) {
	// The castling rook's index may change when another piece is captured.
	auto change_idx = make_invalid(
	    0, // unchanged, valid sequence
	    "[FEN kr6/8/8/8/8/8/8/1NB1K2R b K - 0 1]\n 1...Rxb1 2.O-O Kb7 3.Rf7+");
	EXPECT_EQ(decode_game(change_idx), "Rxb1 O-O Kb7 Rf7+ ");
	EXPECT_EQ(decode_gameview(change_idx), "1...Rxb1  2.O-O Kb7  3.Rf7+");

	// Chess960. Allowed by gameview.
	auto chess960 = make_invalid(
	    9, // replace null-move with long castle
	    "[FEN bnrbkrqn/pppppppp/8/8/8/8/PPPPPPPP/BNRBKRQN w KQkq - 0 1]\n"
	    "1.b3 Ng6 2.e4 e5 3.Ng3 Nc6 4.f3 Bg5 5.Be2 a6 6.Nc3 d6 7.Nd5 {_} -- "
	    "8.Nf5");
	EXPECT_EQ(decode_game(chess960),
	          "b3 Ng6 e4 e5 Ng3 Nc6 f3 Bg5 Be2 a6 Nc3 d6 Nd5 ");
	EXPECT_EQ(decode_gameview(chess960),
	          "1.b3 Ng6  2.e4 e5  3.Ng3 Nc6  4.f3 Bg5  5.Be2 a6  6.Nc3 d6  "
	          "7.Nd5 O-O-O  8.Nf5");

	// Illegal castling. Allowed by both gameview and game.
	// The chess rules for castling (king not in check, empty squares between
	// the rook and the king final positions) are not enforced.
	auto obstacles = make_invalid(0, // unchanged,
	                              "1.d4 d5 2.Qd3 Nf6 3.Bg5 Nc6 4.O-O-O");
	EXPECT_EQ(decode_game(obstacles), "d4 d5 Qd3 Nf6 Bg5 Nc6 O-O-O ");
	EXPECT_EQ(decode_gameview(obstacles),
	          "1.d4 d5  2.Qd3 Nf6  3.Bg5 Nc6  4.O-O-O");

	auto check = make_invalid(0, // unchanged,
	                          "1.d4 d5 2.Nf3 e6 3.e3 Nf6 4.Nc3 Be7 5.Bb5+ O-O");
	EXPECT_EQ(decode_game(check), "d4 d5 Nf3 e6 e3 Nf6 Nc3 Be7 Bb5+ O-O ");
	EXPECT_EQ(decode_gameview(check),
	          "1.d4 d5  2.Nf3 e6  3.e3 Nf6  4.Nc3 Be7  5.Bb5+ O-O");

	// Castle twice. Allowed by gameview: no changes to the board; the notations
	// is wrongly reported as O-O-O because the rook is to the left of the king.
	auto twice = make_invalid(
	    10, // replace 5.a4 with O-O
	    "1.e4 e5 2.Nf3 Nf6 3.Be2 Be7 4.O-O O-O {_} 5.a4 a5 6.Kh1");
	EXPECT_EQ(decode_game(twice), "e4 e5 Nf3 Nf6 Be2 Be7 O-O O-O ");
	EXPECT_EQ(decode_gameview(twice),
	          "1.e4 e5  2.Nf3 Nf6  3.Be2 Be7  4.O-O O-O  5.O-O-O a5  6.Kh1");

	// Moved rook. Allowed by gameview: the rook is moved to D1
	auto moved_rook = make_invalid(
	    9, // replace 2.c4 with O-O-O
	    "[FEN r3k2r/2p2p2/2pq1p2/p2pp2p/P2PP2P/2PQ1P2/2P2P2/R3K2R w KQkq]\n"
	    "1.Ra3 Rh6 {_} 2.c4 O-O-O 3. Ra2");
	EXPECT_EQ(decode_game(moved_rook), "Ra3 Rh6 ");
	EXPECT_EQ(decode_gameview(moved_rook), "1.Ra3 Rh6  2.O-O O-O-O  3.Rd2");

	// No rook
	auto no_rook = make_invalid(
	    9, // replace 2.c4 with O-O-O
	    "[FEN 2k2r2/ppp5/8/8/8/2P1N2R/5PP1/4K3 b - - 0 1]\n"
	    "1...a5 {_} 2.c4 a4 3.Rh5");
	EXPECT_EQ(decode_game(no_rook), "a5 ");
	EXPECT_EQ(decode_gameview(no_rook), "1...a5");

	// Captured rook
	auto captured_rook = make_invalid(
	    9, // replace 4..f5 with O-O-O
	    "[FEN r3k2r/2p2p2/2pq1p2/p2pp2p/P2PP2P/2PQ1P2/2P2P2/R3K2R w KQkq]\n"
	    "1.Qa6 c5 2.Qxa8+ Qd8 3.Qc6+ Qd7 4.O-O {_} f5 5.Kh1 Kd8");
	EXPECT_EQ(decode_game(captured_rook), "Qa6 c5 Qxa8+ Qd8 Qc6+ Qd7 O-O ");
	EXPECT_EQ(decode_gameview(captured_rook),
	          "1.Qa6 c5  2.Qxa8+ Qd8  3.Qc6+ Qd7  4.O-O");

	// occupied squares. Allowed by gameview: both pieces will be present on the
	// same square. The previous piece can be moved.
	auto occ_king = make_invalid(
	    0, // replace 2.Nf3 with a null move
	    "1.e4 e5 {_} 2.Nf3 Nf6 3.Be2 Be7 4.O-O O-O 5.Kh1 a5 6.Nxe5");
	EXPECT_EQ(decode_game(occ_king), "e4 e5 -- Nf6 Be2 Be7 ");
	EXPECT_EQ(decode_gameview(occ_king),
	          "1.e4 e5  2.-- Nf6  3.Be2 Be7  4.O-O O-O  5.Kh1 a5  6.Nf3");

	auto occ_rook = make_invalid(
	    0, // replace 3...Be7 with a null move
	    "1.e4 e5 2.Nf3 Nf6 3.Be2 {_} Be7 4.O-O O-O 5.Kh1");
	EXPECT_EQ(decode_game(occ_rook), "e4 e5 Nf3 Nf6 Be2 -- O-O ");
	EXPECT_EQ(decode_gameview(occ_rook),
	          "1.e4 e5  2.Nf3 Nf6  3.Be2 --  4.O-O O-O  5.Kh1");
}

TEST(Test_Game, illegalSCID4_KingCapture) {
	auto data = make_invalid(0, // null move
	                         "1.d4 e6 2.e4 Bb4+ {_} 3.Ke2 Be1 4.Ke1");
	EXPECT_EQ(decode_gameview(data), "1.d4 e6  2.e4 Bb4+  3.-- Bxe1  4.a4");
	EXPECT_EQ(decode_game(data), "d4 e6 e4 Bb4+ -- ");
}
