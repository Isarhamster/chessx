/*
 * Copyright (C) 2020 Fulvio Benini
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

#include "bytebuf.h"
#include <gtest/gtest.h>

TEST(Test_decodeMove, pawn_white) {
	const squareT from = C2;
	std::tuple<unsigned char, int, pieceT> data[] = {
	    {0, from + 7, INVALID_PIECE}, {1, from + 8, INVALID_PIECE},
	    {2, from + 9, INVALID_PIECE}, {3, from + 7, QUEEN},
	    {4, from + 8, QUEEN},         {5, from + 9, QUEEN},
	    {6, from + 7, ROOK},          {7, from + 8, ROOK},
	    {8, from + 9, ROOK},          {9, from + 7, BISHOP},
	    {10, from + 8, BISHOP},       {11, from + 9, BISHOP},
	    {12, from + 7, KNIGHT},       {13, from + 8, KNIGHT},
	    {14, from + 9, KNIGHT},       {15, from + 16, INVALID_PIECE},
	};
	ByteBuffer bbuf(nullptr, 0);
	for (auto [moveCode, expTo, expPromo] : data) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(PAWN, from, moveCode);
		EXPECT_EQ(to, expTo);
		EXPECT_EQ(promo, expPromo);
	}
}

TEST(Test_decodeMove, pawn_black) {
	const squareT from = D7;
	std::tuple<unsigned char, int, pieceT> data[] = {
	    {0, from - 7, INVALID_PIECE}, {1, from - 8, INVALID_PIECE},
	    {2, from - 9, INVALID_PIECE}, {3, from - 7, QUEEN},
	    {4, from - 8, QUEEN},         {5, from - 9, QUEEN},
	    {6, from - 7, ROOK},          {7, from - 8, ROOK},
	    {8, from - 9, ROOK},          {9, from - 7, BISHOP},
	    {10, from - 8, BISHOP},       {11, from - 9, BISHOP},
	    {12, from - 7, KNIGHT},       {13, from - 8, KNIGHT},
	    {14, from - 9, KNIGHT},       {15, from - 16, INVALID_PIECE},
	};
	ByteBuffer bbuf(nullptr, 0);
	for (auto [moveCode, expTo, expPromo] : data) {
		auto [to, promo] = bbuf.decodeMove<BLACK>(PAWN, from, moveCode);
		EXPECT_EQ(to, expTo);
		EXPECT_EQ(promo, expPromo);
	}
}

TEST(Test_decodeMove, bishop) {
	const squareT from = G7;
	ByteBuffer bbuf(nullptr, 0);
	for (unsigned char moveCode = 0; moveCode < 16; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(BISHOP, from, moveCode);
		EXPECT_EQ(promo, INVALID_PIECE);
		EXPECT_EQ(square_Fyle(to), square_Fyle(moveCode));
		int distRank = (to + 64) / 8 - (from + 64) / 8;
		int distFyle = (to + 64) % 8 - (from + 64) % 8;
		if (moveCode < 8)
			EXPECT_EQ(distRank, distFyle);
		else
			EXPECT_EQ(distRank, -distFyle);
	}
}

TEST(Test_decodeMove, knight) {
	const squareT from = B3;
	std::tuple<unsigned char, int> data[] = {
	    {1, from - 17}, {2, from - 15}, {3, from - 10}, {4, from - 6},
	    {5, from + 6},  {6, from + 10}, {7, from + 15}, {8, from + 17},
	};
	ByteBuffer bbuf(nullptr, 0);
	for (auto [moveCode, expTo] : data) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(KNIGHT, from, moveCode);
		EXPECT_EQ(to, expTo);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
	for (auto moveCode : {0, 9, 10, 11, 12, 13, 14, 15}) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(KNIGHT, from, moveCode);
		EXPECT_EQ(to, from);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
}

TEST(Test_decodeMove, rook) {
	const squareT from = B3;
	ByteBuffer bbuf(nullptr, 0);
	for (unsigned char moveCode = 0; moveCode < 8; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(ROOK, from, moveCode);
		EXPECT_EQ(square_Rank(to), square_Rank(from));
		EXPECT_EQ(square_Fyle(to), moveCode);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
	for (unsigned char moveCode = 8; moveCode < 16; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(ROOK, from, moveCode);
		EXPECT_EQ(square_Fyle(to), square_Fyle(from));
		EXPECT_EQ(square_Rank(to), moveCode - 8);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
}

TEST(Test_decodeMove, queen) {
	const squareT from = A1;
	unsigned char data[] = {C3 + 64};
	ByteBuffer bbuf(data, sizeof data);
	{
		ASSERT_TRUE(bbuf);
		auto [to, promo] = bbuf.decodeMove<WHITE>(QUEEN, from, 0);
		EXPECT_EQ(to, C3);
		EXPECT_EQ(promo, INVALID_PIECE);
		EXPECT_FALSE(bbuf);
	}
	for (unsigned char moveCode = 1; moveCode < 8; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(QUEEN, from, moveCode);
		EXPECT_EQ(square_Rank(to), square_Rank(from));
		EXPECT_EQ(square_Fyle(to), moveCode);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
	for (unsigned char moveCode = 8; moveCode < 16; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(QUEEN, from, moveCode);
		EXPECT_EQ(square_Fyle(to), square_Fyle(from));
		EXPECT_EQ(square_Rank(to), moveCode - 8);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
}

TEST(Test_decodeMove, king) {
	const squareT from = B3;
	ByteBuffer bbuf(nullptr, 0);
	{
		auto [to, promo] = bbuf.decodeMove<WHITE>(KING, from, 0);
		EXPECT_EQ(to, from);
		EXPECT_EQ(promo, PAWN);
	}
	std::tuple<unsigned char, int> data[] = {
	    {1, from - 9}, {2, from - 8}, {3, from - 7}, {4, from - 1},
	    {5, from + 1}, {6, from + 7}, {7, from + 8}, {8, from + 9},
	};
	for (auto [moveCode, expTo] : data) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(KING, from, moveCode);
		EXPECT_EQ(to, expTo);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
	{
		auto [to, promo] = bbuf.decodeMove<WHITE>(KING, from, 9);
		EXPECT_EQ(to, from);
		EXPECT_EQ(promo, QUEEN);
	}
	{
		auto [to, promo] = bbuf.decodeMove<WHITE>(KING, from, 10);
		EXPECT_EQ(to, from);
		EXPECT_EQ(promo, KING);
	}
	for (unsigned char moveCode = 11; moveCode < 16; ++moveCode) {
		auto [to, promo] = bbuf.decodeMove<WHITE>(KING, from, moveCode);
		EXPECT_EQ(to, from);
		EXPECT_EQ(promo, INVALID_PIECE);
	}
}
