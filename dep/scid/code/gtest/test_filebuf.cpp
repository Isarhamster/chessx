/*
* Copyright (C) 2015-2017 Fulvio Benini

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

#include "filebuf.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <numeric>
#include <random>

class Test_Filebuf : public ::testing::Test {};
class Test_FilebufGetline : public ::testing::TestWithParam<size_t> {};

TEST_F(Test_Filebuf, FilebufAppend) {
	static const char* fname = "test_filebufappend";
	struct Cleanup {
		~Cleanup() { std::remove(fname); }
	} cleanup;

	// Generate random data
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> randch(0, 255);
	std::vector<unsigned char> v(300 * 1024);
	for (auto& e : v)
		e = static_cast<unsigned char>(randch(gen));

	try {
		{ // Test sequential write: write half of the data.
			FilebufAppend file;
			ASSERT_EQ(OK, file.open(fname, FMODE_Create));
			auto buf = reinterpret_cast<const char*>(v.data());
			file.append(buf, v.size() / 2);
		}
		{ // Test random read/write: write the remaining part of the data,
		  // alternating with random-access read.
			FilebufAppend file;
			ASSERT_EQ(OK, file.open(fname, FMODE_Both));

			std::vector<unsigned char> v2(v.size());
			auto buf2 = reinterpret_cast<char*>(v2.data());
			auto buf = reinterpret_cast<const char*>(v.data());
			buf += v.size() / 2;
			size_t nLeft = v.size() - v.size() / 2;
			while (nLeft > 0) {
				auto nCh = std::uniform_int_distribution<size_t>(1, nLeft)(gen);
				file.append(buf, nCh);
				buf += nCh;
				nLeft -= nCh;
				EXPECT_EQ(file.size(), v.size() - nLeft);

				auto pos = std::uniform_int_distribution<size_t>(
				    0, static_cast<size_t>(file.size()) - 1)(gen);
				auto nRead = std::uniform_int_distribution<size_t>(
				    1, static_cast<size_t>(file.size()) - pos)(gen);
				file.pubseekpos(pos);
				file.sgetn(buf2, nRead);
				EXPECT_TRUE(std::equal(v2.begin(), v2.begin() + nRead,
				                       v.begin() + pos));
			}
		}
		{ // Test sequential read: read all the file.
			FilebufAppend file;
			ASSERT_EQ(OK, file.open(fname, FMODE_ReadOnly));
			std::vector<unsigned char> v2(v.size());
			auto buf2 = reinterpret_cast<char*>(v2.data());
			file.pubseekpos(0);
			file.sgetn(buf2, v2.size());
			EXPECT_TRUE(std::equal(v.begin(), v.end(), v2.begin()));
		}
	} catch (...) {
		FAIL();
	}
}

TEST_F(Test_Filebuf, read_write_uint32_t) {
	static const char* fileName32 = "test_filebuf_uint32_t";
	struct Cleanup {
		~Cleanup() { std::remove(fileName32); }
	} cleanup;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dis24(0, (1 << 24) - 1);
	std::uniform_int_distribution<uint32_t> dis32;

	// Build a vector for each type of unsigned int.
	std::vector<uint32_t> v8(1 << 8);
	std::iota(v8.begin(), v8.end(), 0);
	std::shuffle(v8.begin(), v8.end(), gen);

	std::vector<uint32_t> v16(1 << 16);
	std::iota(v16.begin(), v16.end(), 0);
	std::shuffle(v16.begin(), v16.end(), gen);

	std::vector<uint32_t> v24 = {0, 1, (1 << 24) - 1};
	for (int i = 0; i < 1000; ++i)
		v24.push_back(dis24(gen));

	std::vector<uint32_t> v32 = {0, 1, (uint64_t(1) << 32) - 1};
	for (int i = 0; i < 1000; ++i)
		v32.push_back(dis32(gen));

	{ // Test sequential write
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fileName32, FMODE_Create));

		for (auto& e : v8) {
			ASSERT_EQ(1, file.WriteOneByte(static_cast<byte>(e)));
		}
		for (auto& e : v16) {
			ASSERT_EQ(2, file.WriteTwoBytes(e));
		}
		for (auto& e : v24) {
			ASSERT_EQ(3, file.WriteThreeBytes(e));
		}
		for (auto& e : v32) {
			ASSERT_EQ(4, file.WriteFourBytes(e));
		}
	}
	{ // Test random write
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fileName32, FMODE_Both));

		std::uniform_int_distribution<> rpos(0, 1000);
		for (int i = 0; i < 1000; ++i) {
			int pos = rpos(gen);

			file.pubseekpos(pos / 4);
			ASSERT_EQ(1, file.WriteOneByte(static_cast<byte>(v8[pos / 4])));

			file.pubseekpos(v8.size() + pos * 2);
			ASSERT_EQ(2, file.WriteTwoBytes(v16[pos]));

			file.pubseekpos(v8.size() + v16.size() * 2 + pos * 3);
			ASSERT_EQ(3, file.WriteThreeBytes(v24[pos]));

			file.pubseekpos(v8.size() + v16.size() * 2 + v24.size() * 3 +
			                pos * 4);
			ASSERT_EQ(4, file.WriteFourBytes(v32[pos]));
		}
	}
	{ // Test sequential read
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fileName32, FMODE_Both));

		for (int i = 0; i < 2; ++i) {
			for (auto& e : v8)
				EXPECT_EQ(e, (size_t)file.ReadOneByte());

			for (auto& e : v16)
				EXPECT_EQ(e, (size_t)file.ReadTwoBytes());

			for (auto& e : v24)
				EXPECT_EQ(e, (size_t)file.ReadThreeBytes());

			for (auto& e : v32)
				EXPECT_EQ(e, (size_t)file.ReadFourBytes());

			// Read past the end, no error should occur.
			file.ReadFourBytes();
			file.pubseekpos(0);
		}
	}
	{ // Test random read
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fileName32, FMODE_Both));

		std::uniform_int_distribution<> rpos(0, 1000);
		for (int i = 0; i < 1000; ++i) {
			int pos = rpos(gen);

			file.pubseekpos(pos / 4);
			EXPECT_EQ(v8[pos / 4], (size_t)file.ReadOneByte());

			file.pubseekpos(v8.size() + pos * 2);
			EXPECT_EQ(v16[pos], (size_t)file.ReadTwoBytes());

			file.pubseekpos(v8.size() + v16.size() * 2 + pos * 3);
			EXPECT_EQ(v24[pos], (size_t)file.ReadThreeBytes());

			file.pubseekpos(v8.size() + v16.size() * 2 + v24.size() * 3 +
			                pos * 4);
			EXPECT_EQ(v32[pos], (size_t)file.ReadFourBytes());
		}
	}
}

static const char* filename = SCID_TESTDIR "res_utf8.txt";

TEST_F(Test_Filebuf, readAll) {
	std::streamsize fileSize = -1;
	Filebuf file;
	if (file.open(filename, std::ios::in | std::ios::binary | std::ios::ate) !=
	    0) {
		fileSize = file.pubseekoff(0, std::ios::cur, std::ios::in);
		file.pubseekoff(0, std::ios::beg, std::ios::in);
	}
	ASSERT_NE(-1, fileSize);

	std::unique_ptr<char[]> buf(new char[static_cast<size_t>(fileSize + 1)]);
	char* line = buf.get();
	char* bEnd = buf.get() + fileSize + 1;
	std::streamsize nTot = 0;
	size_t nRead;
	while ((nRead = file.getline(line, std::distance(line, bEnd))) != 0) {
		nTot += nRead;
		line += nRead;
	}
	EXPECT_EQ(EOF, file.sgetc());
	EXPECT_EQ(fileSize, nTot);
	EXPECT_EQ(bEnd, line + 1);
	{ // Verify that Filebuf::getline behaves like std::fstream::getline
		const char* s = buf.get();
		std::fstream stl(filename);
		char stlBuf[1024];
		while (stl.getline(stlBuf, sizeof stlBuf)) {
			std::string tmp(stlBuf);
			ASSERT_EQ(tmp, s);
			s += 1 + tmp.size();
		}
	}
}

TEST_F(Test_Filebuf, Filebuf_open) {
	static const char* fname = "tmp_filebuf_open";
	struct Cleanup {
		~Cleanup() { std::remove(fname); }
	} cleanup;

	{
		Filebuf file;
		ASSERT_NE(OK, file.Open(fname, FMODE_None));
	}
	{
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fname, FMODE_Create));
		ASSERT_EQ(1, file.WriteOneByte(0x01));
		ASSERT_EQ(2, file.WriteTwoBytes(0x0202));
		ASSERT_EQ(3, file.WriteThreeBytes(0x030303));
		ASSERT_EQ(4, file.WriteFourBytes(0x04040404));
		file.pubseekpos(0);
		ASSERT_EQ(0x01U, file.ReadOneByte());
		ASSERT_EQ(0x0202U, file.ReadTwoBytes());
		ASSERT_EQ(0x030303U, file.ReadThreeBytes());
		ASSERT_EQ(0x04040404U, file.ReadFourBytes());
	}
	{
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fname, FMODE_ReadOnly));
		ASSERT_EQ(0x01U, file.ReadOneByte());
		ASSERT_EQ(0x0202U, file.ReadTwoBytes());
		ASSERT_EQ(0x030303U, file.ReadThreeBytes());
		ASSERT_EQ(0x04040404U, file.ReadFourBytes());
		file.pubseekpos(0);
		ASSERT_EQ(0, file.WriteOneByte(0x01));
		/* libc++ bug
		ASSERT_EQ(0, file.WriteTwoBytes(0x0202));
		ASSERT_EQ(0, file.WriteThreeBytes(0x030303));
		ASSERT_EQ(0, file.WriteFourBytes(0x04040404));
		*/
		file.pubseekpos(0);
		ASSERT_EQ(0x01U, file.ReadOneByte());
		ASSERT_EQ(0x0202U, file.ReadTwoBytes());
		ASSERT_EQ(0x030303U, file.ReadThreeBytes());
		ASSERT_EQ(0x04040404U, file.ReadFourBytes());
	}
	{
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fname, FMODE_WriteOnly));
		ASSERT_EQ(4, file.WriteFourBytes(0x04040404));
		ASSERT_EQ(3, file.WriteThreeBytes(0x030303));
		ASSERT_EQ(2, file.WriteTwoBytes(0x0202));
		ASSERT_EQ(1, file.WriteOneByte(0x01));
		file.pubseekpos(0);
		ASSERT_EQ(static_cast<byte>(EOF), file.ReadOneByte());
		ASSERT_EQ(static_cast<uint16_t>(EOF), file.ReadTwoBytes());
		ASSERT_EQ(static_cast<uint32_t>(EOF), file.ReadFourBytes());
	}
	{
		Filebuf file;
		ASSERT_EQ(OK, file.Open(fname, FMODE_Both));
		ASSERT_EQ(0x04040404U, file.ReadFourBytes());
		ASSERT_EQ(0x030303U, file.ReadThreeBytes());
		ASSERT_EQ(0x0202U, file.ReadTwoBytes());
		ASSERT_EQ(0x01U, file.ReadOneByte());
		file.pubseekoff(0, std::ios::end);
		ASSERT_EQ(1, file.WriteOneByte(0xF1));
		ASSERT_EQ(2, file.WriteTwoBytes(0xF2F2));
		ASSERT_EQ(3, file.WriteThreeBytes(0xF3F3F3F3));
		ASSERT_EQ(4, file.WriteFourBytes(0xF4F4F4F4));
		file.pubseekpos(10);
		ASSERT_EQ(0xF1U, file.ReadOneByte());
		ASSERT_EQ(0xF2F2U, file.ReadTwoBytes());
		ASSERT_EQ(0xF3F3F3U, file.ReadThreeBytes());
		ASSERT_EQ(0xF4F4F4F4U, file.ReadFourBytes());
	}
}


using lines = std::vector<std::pair<std::streamsize, std::string> >;

lines getlineSTL(size_t bufSize) {
	lines vStl;
	std::unique_ptr<char[]> unique_buf(new char[bufSize]);
	char* buf = unique_buf.get();
	std::ifstream stl(filename);
	size_t i = 0;
	for (; !stl.eof(); i++) {
		if (stl.fail()) {
			// Failed because the buffer is too small
			stl.clear();
			vStl.emplace_back(stl.gcount(), buf);
		}
		while (stl.getline(buf, bufSize)) {
			vStl.emplace_back(stl.gcount(), buf);
		}
	}
	vStl.emplace_back(i, "loops");
	return vStl;
}

lines getlineScid(Filebuf& file, size_t bufSize) {
	lines vScid;
	std::unique_ptr<char[]> unique_buf(new char[bufSize]);
	char* buf = unique_buf.get();
	*buf = 0;
	size_t i = 0;
	for (; file.sgetc() != EOF; i++) {
		size_t n;
		while ((n = file.getline(buf, bufSize)) != 0) {
			vScid.emplace_back(n, buf);
		}
		if (*buf != 0) {
			// Failed because the buffer is too small
			vScid.emplace_back(0, buf);
			vScid.back().first = vScid.back().second.size();
		}
	}
	vScid.emplace_back(i, "loops");
	return vScid;
}

// Test with different buffer sizes.
TEST_P(Test_FilebufGetline, read) {
	const size_t bufSize = GetParam();
	lines vStl = getlineSTL(bufSize);

	Filebuf file;
	file.open(filename, std::ios::in);
	lines vScid = getlineScid(file, bufSize);
	EXPECT_EQ(EOF, file.sgetc());

	ASSERT_EQ(vStl.size(), vScid.size());
	for (size_t i = 0, n = vStl.size(); i < n; i++) {
		EXPECT_EQ(vStl[i], vScid[i]);
	}
}

TEST_P(Test_FilebufGetline, filesize) {
	const size_t bufSize = GetParam();

	std::streamsize fileSize = -1;
	{ // Verify that all the chars are read.
		Filebuf file;
		if (file.open(filename,
		              std::ios::in | std::ios::binary | std::ios::ate) != 0) {
			fileSize = file.pubseekoff(0, std::ios::cur, std::ios::in);
			file.pubseekoff(0, std::ios::beg, std::ios::in);
		}
		ASSERT_NE(-1, fileSize);

		lines vScid = getlineScid(file, bufSize);
		EXPECT_EQ(EOF, file.sgetc());

		vScid.pop_back();
		std::streamsize nTot = 0;
		for (auto& e : vScid)
			nTot += e.first;
		EXPECT_EQ(fileSize, nTot);
	}
	{ // Verify that FilebufAppend::size() is correct.
		FilebufAppend fileAppend;
		fileAppend.open(filename, FMODE_ReadOnly);
		EXPECT_EQ(fileSize, std::streamsize(fileAppend.size()));
	}
}

INSTANTIATE_TEST_SUITE_P(smallbuf, Test_FilebufGetline,
                         ::testing::Values(1000, 100, 10));
