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

#include "bytebuf.h"
#include "containers.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <gtest/gtest.h>
#include <vector>

namespace {

int nObjects = 0;

struct RefCounted {
	char ch[3];

public:
	RefCounted() { ++nObjects; }

	RefCounted(const RefCounted& v) {
		std::copy_n(v.ch, sizeof ch, ch);
		++nObjects;
	}

	RefCounted& operator=(const RefCounted&) = default;

	RefCounted* clone() { return new RefCounted(*this); }

	~RefCounted() { --nObjects; }
};

} // namespace

TEST(Test_Containers, VectorChunked) {
	VectorChunked<RefCounted, 3> v; // 8 elements per chunk

	// Test push_back
	for (size_t i = 0; i < 30; ++i) {
		EXPECT_EQ(i, v.size());
		EXPECT_EQ(size_t(nObjects), v.capacity());
		size_t expCapacity = (i == 0) ? 0 : 8 + 8 * (i / 8);
		EXPECT_EQ(v.capacity(), expCapacity);
		RefCounted tmp;
		tmp.ch[0] = static_cast<char>(i);
		v.push_back(tmp);
	}

	// Test operator[]
	std::vector<RefCounted*> ref;
	for (size_t i = 0; i < 30; ++i) {
		ref.push_back(&v[i]);
	}
	std::vector<const RefCounted*> ref_const;
	for (size_t i = 0; i < 30; ++i) {
		const auto& v_const = v;
		ref_const.push_back(&v_const[i]);
	}

	// Test validity of pointers after grow
	v.resize(55);
	EXPECT_EQ(55U, v.size());
	EXPECT_EQ(size_t(nObjects), v.capacity());
	EXPECT_EQ(56U, v.capacity());
	for (size_t i = 0; i < 30; ++i) {
		EXPECT_EQ(ref[i], &v[i]);
		EXPECT_EQ(ref_const[i], &v[i]);
	}

	// Test access with iterators
	for (size_t i = 30; i < v.size();) {
		auto contiguous = v.contiguous(i);
		RefCounted* it = &v[i];
		for (size_t j = 0; j < contiguous; j++) {
			(*it++).ch[0] = (char)i;
			++i;
		}
	}

	// Test values correctness
	for (int i = 0; i < 55; ++i) {
		EXPECT_EQ(i, v[i].ch[0]);
	}

	// Test memory release
	v.resize(16);
	EXPECT_EQ(16U, v.size());
	EXPECT_EQ(size_t(nObjects), v.capacity());
	EXPECT_EQ(24U, v.capacity());
	EXPECT_EQ(15, v[15].ch[0]);

	v.resize(15);
	EXPECT_EQ(15U, v.size());
	EXPECT_EQ(size_t(nObjects), v.capacity());
	EXPECT_EQ(16U, v.capacity());
	EXPECT_EQ(14, v[14].ch[0]);

	v.resize(0);
	EXPECT_EQ(0U, v.size());
	EXPECT_EQ(size_t(nObjects), v.capacity());
	EXPECT_EQ(0U, v.capacity());
}

TEST(Test_Containers, UndoRedo) {
	UndoRedo<RefCounted, 10> cont; // max 10 elements

	RefCounted* cur = new RefCounted();

	for (char i = 0; i < 10; ++i) {
		cur->ch[0] = i;
		cont.store(cur);
		EXPECT_EQ(i + 2, nObjects);
	}

	// Test max n. of elements
	EXPECT_EQ(11, nObjects);
	EXPECT_EQ(10U, cont.undoSize());
	cur->ch[0] = 10;
	cont.store(cur);
	cur->ch[0] = 11;
	EXPECT_EQ(11, nObjects);
	EXPECT_EQ(10U, cont.undoSize());

	// Test undo
	for (char i = 10; i > 0; --i) {
		EXPECT_EQ(size_t(i), cont.undoSize());
		cur = cont.undo(cur);
		EXPECT_EQ(i, cur->ch[0]);
		EXPECT_EQ(11, nObjects);
	}

	// Test empty undo queue
	for (int i = 0; i < 5; ++i) {
		cur = cont.undo(cur);
		EXPECT_EQ(1, cur->ch[0]);
		EXPECT_EQ(11, nObjects);
		EXPECT_EQ(0U, cont.undoSize());
		EXPECT_EQ(10U, cont.redoSize());
	}

	// Test redo
	for (char i = 2; i < 6; ++i) {
		cur = cont.redo(cur);
		EXPECT_EQ(i, cur->ch[0]);
		EXPECT_EQ(11, nObjects);
	}

	// Test that store() clears redo queue
	cur->ch[0] = 'a';
	cont.store(cur);
	EXPECT_EQ(6, nObjects);
	EXPECT_EQ(5U, cont.undoSize());
	EXPECT_EQ(0U, cont.redoSize());

	// Test memory release
	cont.clear();
	EXPECT_EQ(0U, cont.undoSize());
	EXPECT_EQ(0U, cont.redoSize());
	EXPECT_EQ(1, nObjects);
	EXPECT_EQ('a', cur->ch[0]);

	delete cur;
}

TEST(Test_Containers, ByteBuffer_GetTerminatedString) {
	const char* test_data[] = {"abcd", "", "efg"};
	auto v = [&] {
		std::vector<char> res;
		for (auto str : test_data) {
			res.insert(res.end(), str, str + std::strlen(str) + 1);
		}
		return res;
	}();
	ByteBuffer buf(reinterpret_cast<unsigned char*>(v.data()), v.size());
	for (auto str : test_data) {
		EXPECT_STREQ(str, buf.GetTerminatedString());
	}
}
