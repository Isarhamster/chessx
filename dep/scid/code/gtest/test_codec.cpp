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

#include "bytebuf.h"
#include "codec.h"
#include "game.h"
#include "index.h"
#include "misc.h"
#include "namebase.h"
#include <cstring>
#include <gtest/gtest.h>
#include <iterator>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

namespace {

fileModeT fmodes[] = {
	FMODE_Create, FMODE_ReadOnly, FMODE_WriteOnly, FMODE_Both
};
const char* filename = "codecbase";

ICodecDatabase::Codec codecs[] = {
	ICodecDatabase::MEMORY, ICodecDatabase::SCID4, ICodecDatabase::PGN
};

std::vector<std::pair<ICodecDatabase::Codec, std::string> > unsupportedVec = {
	{ ICodecDatabase::MEMORY, "FMODE" + std::to_string(FMODE_None) },
	{ ICodecDatabase::MEMORY, "FMODE" + std::to_string(FMODE_ReadOnly) },
	{ ICodecDatabase::MEMORY, "FMODE" + std::to_string(FMODE_WriteOnly) },
	{ ICodecDatabase::MEMORY, "FMODE" + std::to_string(FMODE_Both) },

	{ ICodecDatabase::SCID4, "FMODE" + std::to_string(FMODE_None) },
	{ ICodecDatabase::SCID4, "FMODE" + std::to_string(FMODE_WriteOnly) },
	{ ICodecDatabase::SCID4, "empty_filename" },

	{ ICodecDatabase::PGN, "FMODE" + std::to_string(FMODE_None) },
	{ ICodecDatabase::PGN, "saveGame_game" },
	{ ICodecDatabase::PGN, "empty_filename" }
};

class Supports {
	ICodecDatabase::Codec dbtype_;

public:
	Supports(ICodecDatabase::Codec dbtype) : dbtype_(dbtype) {}

	bool operator()(const std::string& feature) const {
		auto it = std::find(unsupportedVec.begin(), unsupportedVec.end(),
		                    std::make_pair(dbtype_, feature));
		return it == unsupportedVec.end();
	}
};

template <int nGames, int maxMoves, int maxCommentLen>
class GameGenerator {
	typedef std::vector<std::unique_ptr<Game> > Vec;
	Vec v_;
	std::vector<std::vector<byte> > encoded_;
	std::mt19937 mt_;

public:
	const Vec& get() {
		if (v_.empty()) {
			for (int i = 0; i < nGames; i++) {
				v_.push_back(genGame());
			}
			encodeGames();
		}
		return v_;
	}

	const std::vector<std::vector<byte> >& getNative() {
		if (encoded_.empty())
			get();

		return encoded_;
	}

	void cmp(ICodecDatabase* codec, const Index& idx) {
		auto encoded = getNative();
		ASSERT_EQ(encoded.size(), size_t(idx.GetNumGames()));
		int g = 0;
		for (auto& game : encoded) {
			auto entry = idx.GetEntry(g++);
			auto data = codec->getGameData(entry->GetOffset(), entry->GetLength());
			ASSERT_NE(nullptr, data);
			ASSERT_EQ(game.size(), entry->GetLength());
			EXPECT_TRUE(std::equal(data, data + entry->GetLength(), game.data()));
		}
	}

private:
	void encodeGames() {
		for (auto& game : v_) {
			auto ie = IndexEntry();
			ASSERT_EQ(OK, game->Encode(encoded_.emplace_back(), ie));
		}
	}

	std::unique_ptr<Game> genGame() {
		auto res = std::unique_ptr<Game>(new Game);
		res->GetCurrentPos()->StdStart();
		MoveList mlist;
		for (auto i = rand(0, maxMoves); i > 0; --i) {
			res->GetCurrentPos()->GenerateMoves(&mlist, EMPTY, GEN_ALL_MOVES, true);
			if (mlist.Size() == 0)
				break;
			res->AddMove(mlist.Get(rand(0, mlist.Size() - 1)));

			if (rand(0, 6) == 0)
				res->SetMoveComment(rand_comment().c_str());

			int varOp = rand(0, 80 + int(res->GetVarLevel()) * 20);
			if (varOp < 20) {
				res->AddVariation();
			} else if (varOp > 80) {
				res->MoveExitVariation();
				res->MoveForward();
			}
		}
		return res;
	}

	int rand(int low, int up) {
		return std::uniform_int_distribution<int>(low, up)(mt_);
	}

	std::string rand_comment() {
		size_t len = rand(0, maxCommentLen);
		std::string res(len, ' ');
		std::uniform_int_distribution<int> dist{33, 122};
		std::generate_n(res.begin(), res.size(), [&] () {
			return static_cast<char>(dist(mt_));
		});
		return res;
	}
};
GameGenerator<1000, 2000, 300> gameGenerator;

template <typename Oper>
void makeDatabase(ICodecDatabase::Codec dbtype, const char* test, Oper op) {
	Supports supports(dbtype);
	if (!supports(test))
		return;

	fileModeT fMode = FMODE_Create;

	struct Cleanup {
		std::vector<std::string> filenames;

		~Cleanup() {
			for (const auto& fname : filenames) {
				EXPECT_EQ(0, std::remove(fname.c_str()));
			}
		}
	} cleanup;

	{
		Index idx;
		NameBase nb;
		auto err = ICodecDatabase::open(dbtype, fMode, filename, Progress(),
		                                &idx, &nb);
		auto codec = std::unique_ptr<ICodecDatabase>(err.first);
		ASSERT_NE(nullptr, codec);
		cleanup.filenames = codec->getFilenames();
		ASSERT_EQ(OK, err.second);

		op(codec.get(), idx, nb);

		ASSERT_EQ(gameGenerator.get().size(), size_t(idx.GetNumGames()));
		ASSERT_EQ(OK, codec->flush());
		gameGenerator.cmp(codec.get(), idx);
	}

	if (supports("FMODE" + std::to_string(FMODE_ReadOnly))) {
		Index idx;
		NameBase nb;
		auto err = ICodecDatabase::open(dbtype, FMODE_ReadOnly, filename,
		                                Progress(), &idx, &nb);
		auto codec = std::unique_ptr<ICodecDatabase>(err.first);
		ASSERT_NE(nullptr, codec);
		ASSERT_EQ(OK, err.second);
		ASSERT_EQ(gameGenerator.get().size(), size_t(idx.GetNumGames()));

		gameGenerator.cmp(codec.get(), idx);
	}
}

} // end of anonymous namespace

class Test_Codec : public ::testing::TestWithParam<ICodecDatabase::Codec> {};

TEST_P(Test_Codec, addGame_game) {
	makeDatabase(GetParam(), "addGame_game",
	             [](ICodecDatabase* codec, Index&, NameBase&) {
		for (auto& game : gameGenerator.get()) {
			ASSERT_EQ(OK, codec->addGame(game.get()));
		}
	});
}

TEST_P(Test_Codec, addGame_native) {
	makeDatabase(GetParam(), "addGame_native",
	             [](ICodecDatabase* codec, Index&, NameBase& nb) {
		for (const auto& game : gameGenerator.getNative()) {
			std::pair<errorT, idNumberT> names[] = {
			    nb.addName(NAME_PLAYER, "Dummy White", 255, 1000),
			    nb.addName(NAME_PLAYER, "Dummy Black", 255, 1000),
			    nb.addName(NAME_EVENT, "Dummy Event", 255, 1000),
			    nb.addName(NAME_SITE, "Dummy Site", 255, 1000),
			    nb.addName(NAME_ROUND, "Dummy Round", 255, 1000)};
			for (auto& e : names)
				ASSERT_EQ(OK, e.first);

			IndexEntry ie;
			std::memset(&ie, 0, sizeof(IndexEntry));
			ie.SetWhite(names[0].second);
			ie.SetBlack(names[1].second);
			ie.SetEvent(names[2].second);
			ie.SetSite(names[3].second);
			ie.SetRound(names[4].second);
			ASSERT_EQ(OK, codec->addGame(&ie, &nb, game.data(), game.size()));
		}
	});
}

TEST_P(Test_Codec, saveGame_game) {
	makeDatabase(GetParam(), "saveGame_game",
	             [](ICodecDatabase* codec, Index&, NameBase&) {
		const auto& games = gameGenerator.get();
		for (size_t i = 0, n = games.size(); i < n; ++i) {
			ASSERT_EQ(OK, codec->addGame(games[0].get()));
		}
		codec->flush();

		std::vector<int> randIdx(games.size());
		std::iota(randIdx.begin(), randIdx.end(), 0);
		std::shuffle(randIdx.begin(), randIdx.end(), std::mt19937());
		for (auto idx : randIdx) {
			ASSERT_EQ(OK, codec->saveGame(games[idx].get(), idx));
		}
	});
}

// Try to get a ICodecDatabase pointer for each supported file mode, then test
// the consistency of getType() and getFilenames().
TEST_P(Test_Codec, fileModeT) {
	ICodecDatabase::Codec dbtype = GetParam();
	Supports supports(dbtype);

	struct Cleanup {
		std::vector<std::string> filenames;

		~Cleanup() {
			for (const auto& fname : filenames) {
				EXPECT_EQ(0, std::remove(fname.c_str()));
			}
		}
	} cleanup;

	for (auto& fmode : fmodes) {
		Index idx;
		NameBase nb;
		auto err = ICodecDatabase::open(dbtype, fmode, filename, Progress(),
		                                &idx, &nb);
		auto codec = std::unique_ptr<ICodecDatabase>(err.first);

		if (supports("FMODE" + std::to_string(fmode))) {
			ASSERT_NE(nullptr, codec);
			EXPECT_EQ(dbtype, codec->getType());
			auto tmp = codec->getFilenames();
			if (cleanup.filenames.empty())
				cleanup.filenames = tmp;
			else
				EXPECT_TRUE(tmp == cleanup.filenames);
		} else {
			EXPECT_EQ(nullptr, codec);
		}
	}
}

TEST_P(Test_Codec, create_emptyfilename) {
	ICodecDatabase::Codec dbtype = GetParam();
	Supports supports(dbtype);

	if (!supports("FMODE" + std::to_string(FMODE_Create))) {
		return;
	}

	Index idx;
	NameBase nb;
	auto err =
	    ICodecDatabase::open(dbtype, FMODE_Create, "", Progress(), &idx, &nb);
	auto codec = std::unique_ptr<ICodecDatabase>(err.first);

	if (!supports("empty_filename")) {
		EXPECT_EQ(nullptr, codec);
	} else {
		EXPECT_NE(nullptr, codec);
		for (const auto& fname : codec->getFilenames()) {
			EXPECT_EQ(0, std::remove(fname.c_str()));
		}
	}
}

// Creates two databases; remove the first one and rename the second to the first.
// This test mimic the process perfomed to finalize the compaction of a database.
TEST_P(Test_Codec, rename) {
	ICodecDatabase::Codec dbtype = GetParam();
	Supports supports(dbtype);

	if (!supports("FMODE" + std::to_string(FMODE_Create))) {
		return;
	}
	struct Cleanup {
		std::vector<std::string> filenames1, filenames2;

		~Cleanup() {
			for (const auto& fname : filenames1) {
				EXPECT_EQ(0, std::remove(fname.c_str()));
			}
			for (const auto& fname : filenames2) {
				EXPECT_NE(0, std::remove(fname.c_str()));
			}
		}
	} cleanup;

	{
		Index idx1, idx2;
		NameBase nb1, nb2;
		auto err = ICodecDatabase::open(dbtype, FMODE_Create, filename,
		                                Progress(), &idx1, &nb1);
		auto codec1 = std::unique_ptr<ICodecDatabase>(err.first);
		EXPECT_EQ(OK, codec1->flush());
		ASSERT_NE(nullptr, codec1);
		ASSERT_EQ(OK, err.second);

		std::string renamed_name = std::string(filename) + "__renamed__";
		err = ICodecDatabase::open(dbtype, FMODE_Create, renamed_name.c_str(),
		                           Progress(), &idx2, &nb2);
		auto codec2 = std::unique_ptr<ICodecDatabase>(err.first);
		EXPECT_EQ(OK, codec2->flush());
		ASSERT_NE(nullptr, codec2);
		ASSERT_EQ(OK, err.second);

		cleanup.filenames1 = codec1->getFilenames();
		cleanup.filenames2 = codec2->getFilenames();
		EXPECT_EQ(cleanup.filenames1.size(), cleanup.filenames2.size());
	}

	for (const auto& fname : cleanup.filenames1) {
		EXPECT_EQ(0, std::remove(fname.c_str()));
	}
	for (size_t i = 0, n = cleanup.filenames2.size(); i < n; i++) {
		const char* s1 = cleanup.filenames1[i].c_str();
		const char* s2 = cleanup.filenames2[i].c_str();
		EXPECT_EQ(0, std::rename(s2, s1));
	}

	if (supports("FMODE" + std::to_string(FMODE_ReadOnly))) {
		Index idx_reopen;
		NameBase nb_reopen;
		auto err = ICodecDatabase::open(dbtype, FMODE_ReadOnly, filename,
		                                Progress(), &idx_reopen, &nb_reopen);
		auto codec3 = std::unique_ptr<ICodecDatabase>(err.first);
		ASSERT_NE(nullptr, codec3);
		ASSERT_EQ(OK, err.second);

		auto filenames3 = codec3->getFilenames();
		EXPECT_TRUE(cleanup.filenames1 == filenames3);
	}
}

INSTANTIATE_TEST_SUITE_P(CodecDatabase, Test_Codec, ::testing::ValuesIn(codecs));
