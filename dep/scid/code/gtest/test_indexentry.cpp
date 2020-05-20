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

#include "codec_scid4.h"
#include "date.h"
#include "indexentry.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <stdint.h>
#include <vector>

void decodeIndexEntry(const char* buf_it, versionT version, IndexEntry* ie);

namespace v4_6 {
#include "test_indexentry_v4_6.h"
}

namespace {

class Buffer {
	char buf_[1024];
	char* it_ = buf_;

public:
	void ToStart() { it_ = buf_; }

	bool equal(const Buffer& b, size_t count) {
		return std::equal(buf_, buf_ + count, b.buf_);
	}

	const char* getBuffer() {
		return buf_;
	}

	size_t sputn(const char* s, size_t count) {
		it_ = std::copy_n(s, count, it_);
		return count;
	}

	byte ReadOneByte() { return *it_++; }
	uint16_t ReadTwoBytes() { return static_cast<uint16_t>(read<2>()); }
	uint32_t ReadThreeBytes() { return read<3>(); }
	uint32_t ReadFourBytes() { return read<4>(); }

	int WriteOneByte(byte value) {
		*it_ = static_cast<char>(value);
		++it_;
		return 1;
	}

	int WriteTwoBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}
	int WriteThreeBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 16)) +
		       WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}
	int WriteFourBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 24)) +
		       WriteOneByte(static_cast<byte>(value >> 16)) +
		       WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}

private:
	template <int nBytes> uint32_t read() {
		uint32_t res = 0;
		if (nBytes > 3)
			res += ReadOneByte() << 24;
		if (nBytes > 2)
			res += ReadOneByte() << 16;
		if (nBytes > 1)
			res += ReadOneByte() << 8;
		return res + ReadOneByte();
	}
};

} // namespace

TEST(Test_IndexEntry, Limits_SCID4) {
	const std::vector<uint64_t> limits_SCID4 = {
	    uint64_t(1) << 32,           // bits for gamefile offset
	    uint64_t(1) << 17,           // bits for game length
	    uint64_t(1) << 20,           // bits for white name
	    uint64_t(1) << 20,           // bits for black name
	    uint64_t(1) << 19,           // bits for event name
	    uint64_t(1) << 19,           // bits for site name
	    uint64_t(1) << 18,           // bits for round name
	    DATE_MAKE(YEAR_MAX, 12, 31), // max date (20 bits)
	    DATE_MAKE(YEAR_MAX, 12, 31), // max event date (12 bits)
	    uint64_t(1) << 4,            // bits for result
	    MAX_ELO,                     // max white elo (12 bits)
	    MAX_ELO,                     // max black elo (12 bits)
	    uint64_t(1) << 4,            // bits for white elo type
	    uint64_t(1) << 4,            // bits for black elo type
	    uint64_t(1) << 16,           // bits for eco code
	    uint64_t(1) << 10,           // bits for numHalfMoves
	    uint64_t(1) << 22,           // bits for flags
	    50,                          // max variation count (4 bits)
	    50,                          // max comment count (4 bits)
	    50,                          // max nag count (4 bits)
	    uint64_t(1) << 24,           // bits for FinalMatSig
	    uint64_t(1) << 8,            // bits for StoredLineCode
	    uint64_t(1) << 6,            // bits for HomePawnData[0]
	    uint64_t(1) << 8,            // bits for HomePawnData[1]
	    uint64_t(1) << 8,            // bits for HomePawnData[2]
	    uint64_t(1) << 8,            // bits for HomePawnData[3]
	    uint64_t(1) << 8,            // bits for HomePawnData[4]
	    uint64_t(1) << 8,            // bits for HomePawnData[5]
	    uint64_t(1) << 8,            // bits for HomePawnData[6]
	    uint64_t(1) << 8,            // bits for HomePawnData[7]
	    uint64_t(1) << 8             // bits for HomePawnData[8]
	}; // In total a SCID4 IndexEntry uses 375 bits (47 bytes)

	auto setEntry = [](auto& entry, auto it) {
		entry.SetOffset(*it++);
		entry.SetLength(static_cast<size_t>(*it++));
		entry.SetWhite(static_cast<idNumberT>(*it++));
		entry.SetBlack(static_cast<idNumberT>(*it++));
		entry.SetEvent(static_cast<idNumberT>(*it++));
		entry.SetSite(static_cast<idNumberT>(*it++));
		entry.SetRound(static_cast<idNumberT>(*it++));
		entry.SetDate(static_cast<dateT>(*it++));
		entry.SetEventDate(static_cast<dateT>(*it++));
		entry.SetResult(static_cast<resultT>(*it++));
		entry.SetWhiteElo(static_cast<eloT>(*it++));
		entry.SetBlackElo(static_cast<eloT>(*it++));
		entry.SetWhiteRatingType(static_cast<byte>(*it++));
		entry.SetBlackRatingType(static_cast<byte>(*it++));
		entry.SetEcoCode(static_cast<ecoT>(*it++));
		entry.SetNumHalfMoves(static_cast<ushort>(*it++));
		entry.clearFlags();
		entry.SetFlag(static_cast<uint32_t>(*it++), true);
		entry.SetVariationCount(static_cast<unsigned>(*it++));
		entry.SetCommentCount(static_cast<unsigned>(*it++));
		entry.SetNagCount(static_cast<unsigned>(*it++));
		entry.SetFinalMatSig(static_cast<matSigT>(*it++));
		entry.SetStoredLineCode(static_cast<byte>(*it++));
		return it;
	};

	auto chkEntry = [](auto& entry, auto it, bool fullEDate = false) {
		ASSERT_EQ(*it++, entry.GetOffset());
		ASSERT_EQ(*it++, entry.GetLength());
		ASSERT_EQ(*it++, entry.GetWhite());
		ASSERT_EQ(*it++, entry.GetBlack());
		ASSERT_EQ(*it++, entry.GetEvent());
		ASSERT_EQ(*it++, entry.GetSite());
		ASSERT_EQ(*it++, entry.GetRound());
		int64_t year = date_GetYear(static_cast<dateT>(*it));
		ASSERT_EQ(*it++, entry.GetDate());
		dateT eventDate = static_cast<dateT>(*it++);
		// Due to a compact encoding format, the EventDate
		// must be within a few years of the Date.
		if (!fullEDate && std::abs(year - date_GetYear(eventDate)) > 3)
			eventDate = 0;
		ASSERT_EQ(eventDate, entry.GetEventDate());
		ASSERT_EQ(*it++, entry.GetResult());
		ASSERT_EQ(*it++, entry.GetWhiteElo());
		ASSERT_EQ(*it++, entry.GetBlackElo());
		ASSERT_EQ(*it++, entry.GetWhiteRatingType());
		ASSERT_EQ(*it++, entry.GetBlackRatingType());
		ASSERT_EQ(*it++, entry.GetEcoCode());
		ASSERT_EQ(*it++, entry.GetNumHalfMoves());
		ASSERT_TRUE(entry.GetFlag(static_cast<uint32_t>(*it++)));
		ASSERT_TRUE(std::abs(int64_t(*it++) - entry.GetVariationCount()) <= 5);
		ASSERT_TRUE(std::abs(int64_t(*it++) - entry.GetCommentCount()) <= 5);
		ASSERT_TRUE(std::abs(int64_t(*it++) - entry.GetNagCount()) <= 5);
		ASSERT_EQ(*it++, entry.GetFinalMatSig());
		ASSERT_EQ(*it++, entry.GetStoredLineCode());
		for (int i = 0; i < 9; i++) {
			ASSERT_EQ(*it++, entry.GetHomePawnData()[i]);
		}
	};

	std::mt19937 re(std::random_device{}());
	for (int i = 0; i < 100000; ++i) {
		auto v = limits_SCID4;
		for (auto& e : v) {
			if (i == 0) // First loop with max values
				e -= 1;
			else
				e = std::uniform_int_distribution<uint64_t>{0, e - 1}(re);
		}

		Buffer ie_buf;
		{ // Build the current IndexEntry and serialize it to ie_buf
			IndexEntry ie;
			auto it = setEntry(ie, v.cbegin());
			byte hpBuf[8];
			std::copy_n(it + 1, 8, hpBuf);
			ie.SetHomePawnData(*it, hpBuf);
			chkEntry(ie, v.cbegin(), true);
			ie.Write(&ie_buf, 400);
			ie_buf.ToStart();
		}

		Buffer ie_buf_v4_6;
		{ // Build the v4_6::IndexEntry and serialize it to ie_buf_v4_6
			v4_6::IndexEntry ie4_6;
			ie4_6.Init();
			auto it = setEntry(ie4_6, v.cbegin());
			for (int i = 0; i < 9; i++) {
				ie4_6.GetHomePawnData()[i] = (static_cast<byte>(*it++));
			}
			chkEntry(ie4_6, v.cbegin());
			ie4_6.Write(&ie_buf_v4_6, 400);
			ie_buf_v4_6.ToStart();
		}

		ASSERT_TRUE(ie_buf.equal(ie_buf_v4_6, v4_6::INDEX_ENTRY_SIZE));

		{ // Read a current IndexEntry from ie_buf and check it
			IndexEntry ie;
			decodeIndexEntry(ie_buf.getBuffer(), 400, &ie);
			chkEntry(ie, v.cbegin());
		}

		{ // Read a current IndexEntry from ie_buf_v4_6 and check it
			IndexEntry ie;
			decodeIndexEntry(ie_buf_v4_6.getBuffer(), 400, &ie);
			chkEntry(ie, v.cbegin());
		}

		{ // Read a v4_6::IndexEntry from ie_buf and check it
			v4_6::IndexEntry ie4_6;
			ie4_6.Read(&ie_buf, 400);
			chkEntry(ie4_6, v.cbegin());
			ie_buf.ToStart();
		}

		{ // Read a v4_6::IndexEntry from ie_buf_v4_6 and check it
			v4_6::IndexEntry ie4_6;
			ie4_6.Read(&ie_buf_v4_6, 400);
			chkEntry(ie4_6, v.cbegin());
			ie_buf_v4_6.ToStart();
		}
	}
}

TEST(Test_IndexEntry, Flags_dedicatedGetSet) {
	IndexEntry ie;
	std::mt19937 re(std::random_device{}());
	std::uniform_int_distribution<> rnd{0, 1};
	for (int i = 0; i < 100000; ++i) {
		bool v[7];
		std::generate(std::begin(v), std::end(v), [&]() { return rnd(re); });

		ie.clearFlags();
		ie.SetStartFlag(v[0]);
		ie.SetPromotionsFlag(v[1]);
		ie.SetUnderPromoFlag(v[2]);
		ie.SetDeleteFlag(v[3]);
		ie.SetCommentCount(v[4]);
		ie.SetVariationCount(v[5]);
		ie.SetNagCount(v[6]);
		EXPECT_EQ(v[0], ie.GetStartFlag());
		EXPECT_EQ(v[1], ie.GetPromotionsFlag());
		EXPECT_EQ(v[2], ie.GetUnderPromoFlag());
		EXPECT_EQ(v[3], ie.GetDeleteFlag());
		EXPECT_EQ(v[4], ie.GetCommentsFlag());
		EXPECT_EQ(v[5], ie.GetVariationsFlag());
		EXPECT_EQ(v[6], ie.GetNagsFlag());
	}
}

TEST(Test_IndexEntry, Flags) {
	IndexEntry ie;
	ie.clearFlags();

	uint32_t flags = 0;
	std::mt19937 re(std::random_device{}());
	for (int i = 0; i < 100000; ++i) {
		auto rnd = std::uniform_int_distribution<uint32_t>{
		    0, (1UL << IndexEntry::IDX_NUM_FLAGS) - 1}(re);
		if (i & 0x01) {
			ie.clearFlags();
			flags = rnd; // Reset flags
		} else {
			flags |= rnd; // Add to previous flags
		}
		ie.SetFlag(rnd, true);

		for (int32_t j = 0; j <= IndexEntry::IDX_NUM_FLAGS; j++) {
			EXPECT_EQ((flags & (1 << j)) != 0, ie.GetFlag(1 << j));
		}

		EXPECT_TRUE(ie.GetFlag(flags));
	}
}
