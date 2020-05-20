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

#include "hfilter.h"
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <vector>

namespace {

const std::vector<byte> data1 = {1, 4, 255, 0, 0, 13};
const std::vector<byte> data2 = {43, 87, 0, 1, 0, 0};
const std::vector<byte> dataEmpty = {0, 0, 0, 0, 0, 0};
const std::vector<byte> dataFull = {1, 1, 1, 1, 1, 1};
const std::vector<byte> noGames;

using testParam = std::pair<const std::vector<byte>*, const std::vector<byte>*>;
testParam test_cases[] = { // clang-format off
    {&data1, nullptr},     {&data1, &data2},     {&data1, &dataEmpty}, {&data1, &dataFull},
    {&data2, nullptr},     {&data2, &data1},     {&data2, &dataEmpty}, {&data2, &dataFull},
    {&dataEmpty, nullptr}, {&dataEmpty, &data1}, {&dataEmpty, &data2}, {&dataEmpty, &dataFull},
    {&dataFull, nullptr},  {&dataFull, &data1},  {&dataFull, &data2},  {&dataFull, &dataEmpty},
    {&noGames, nullptr},   {&noGames, &noGames}
}; // clang-format on

} // namespace

TEST(Test_Filter, Resize) {
	Filter filter(10);
	ASSERT_EQ(10, filter.Count());
	ASSERT_EQ(10, filter.Size());
	ASSERT_EQ(nullptr, filter.data());

	auto test_resize = [&](gamenumT size, gamenumT expectCount,
	                       gamenumT expectSize) {
		filter.Resize(size);
		ASSERT_EQ(expectCount, filter.Count());
		ASSERT_EQ(expectSize, filter.Size());
	};

	test_resize(20, 20, 20);
	ASSERT_EQ(nullptr, filter.data());
	test_resize(5, 5, 5);
	ASSERT_EQ(nullptr, filter.data());
	filter.Set(1, 0);
	ASSERT_NE(nullptr, filter.data());
	filter.Set(2, 3);
	filter.Set(3, 0);
	ASSERT_EQ(3, filter.Count());
	ASSERT_EQ(5, filter.Size());

	test_resize(20, 3, 20);
	test_resize(300, 3, 300);
	test_resize(10, 3, 10);

	filter.Set(1, 10);
	filter.Set(3, 10);
	ASSERT_EQ(5, filter.Count());
	ASSERT_EQ(10, filter.Size());

	test_resize(5, 5, 5);
	test_resize(40, 40, 40);
}

TEST(Test_Filter, Filter) {
	Filter filter(10);

	for (byte val : {1, 0, 10, 1, 10}) {
		filter.Fill(val);
		ASSERT_EQ(10, filter.Size());
		byte expect = (val == 0) ? 0 : 10;
		ASSERT_EQ(expect, filter.Count());
	}
}

class Test_HFilter : public ::testing::TestWithParam<testParam> {
protected:
	const std::vector<byte>* main_;
	const std::vector<byte>* mask_;
	size_t mainSz_;
	size_t maskSz_;
	size_t numGames_;
	std::map<gamenumT, uint8_t> equivMap_;
	std::set<gamenumT> equivInv_;

	virtual void SetUp() {
		auto param = GetParam();
		main_ = param.first;
		mask_ = param.second;

		numGames_ = main_->size();
		mainSz_ = numGames_ - std::count(main_->begin(), main_->end(), 0);
		if (mask_ == nullptr)
			maskSz_ = 0;
		else
			maskSz_ = numGames_ - std::count(mask_->begin(), mask_->end(), 0);

		auto mask = (mask_ != nullptr) ? mask_ : main_;
		gamenumT i = 0;
		std::transform(main_->begin(), main_->end(), mask->begin(),
		               std::inserter(equivMap_, equivMap_.end()),
		               [&i](auto& main, auto& mask) {
			               auto key = i++;
			               uint8_t val = (main == 0) ? 0 : mask;
			               if (val-- == 0)
				               key = std::numeric_limits<gamenumT>::max();
			               return std::make_pair(key, val);
		               });
		equivMap_.erase(std::numeric_limits<gamenumT>::max());

		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			if (equivMap_.end() == equivMap_.find(gnum))
				equivInv_.insert(gnum);
		}
	}

	template <typename T> std::unique_ptr<Filter> makeFilter(const T* data) {
		std::unique_ptr<Filter> res = nullptr;
		if (data != nullptr) {
			auto n = static_cast<gamenumT>(data->size());
			res = std::make_unique<Filter>(n);
			for (gamenumT i = 0; i < n; ++i) {
				res->Set(i, data->operator[](i));
			}
		}
		return res;
	}

	template <typename T> bool equal(const T& map, const HFilter& filter) {
		return std::equal(map.begin(), map.end(), filter->begin(),
		                  filter->end(), [&](auto& a, auto& b) {
			                  auto value = filter->get(b) - 1;
			                  return a.first == b && a.second == value;
		                  });
	}
};

TEST_P(Test_HFilter, hfilter_constFunc) {
	auto f1 = makeFilter(main_);
	auto f2 = makeFilter(mask_);
	HFilter filter(f1.get(), f2.get());
	ASSERT_TRUE(filter != 0);
	ASSERT_FALSE(filter == 0);

	EXPECT_EQ(equivMap_.size(), filter->size());
	EXPECT_EQ(equivInv_.size(), filter->sizeInverted());
	EXPECT_EQ(equivInv_.size(), HFilterInverted(filter).size());
	EXPECT_TRUE(equal(equivMap_, filter));

	// Test HFilter::get()
	for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
		auto val = filter->get(gnum);
		ASSERT_EQ(static_cast<const HFilter&>(filter)->get(gnum), val);
		if (val == 0) {
			EXPECT_EQ(equivMap_.end(), equivMap_.find(gnum));
			EXPECT_NE(equivInv_.end(), equivInv_.find(gnum));
		} else {
			EXPECT_EQ(equivInv_.end(), equivInv_.find(gnum));
			auto it = equivMap_.find(gnum);
			ASSERT_NE(equivMap_.end(), it);
			EXPECT_EQ(1 + it->second, val);
		}
	}

	// Test HFilter::begin() and HFilter::end()
	HFilter::const_iterator it = filter->begin();
	for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
		if (filter->get(gnum) == 0)
			continue;

		ASSERT_TRUE(it != filter->end());
		EXPECT_EQ(gnum, *it);
		++it;
	}
	EXPECT_FALSE(it != filter->end());

	// Test HFilter::beginInverted() and HFilter::endInverted()
	HFilter::const_iterator itInv = filter->beginInverted();
	for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
		if (filter->get(gnum) != 0)
			continue;

		ASSERT_TRUE(itInv != filter->end());
		EXPECT_EQ(gnum, *itInv);
		++itInv;
	}
	EXPECT_FALSE(itInv != filter->endInverted());

	// Test c++11 for-ranged loop
	auto itMap = equivMap_.begin();
	for (auto& gnum : *filter) {
		EXPECT_EQ(itMap->first, gnum);
		EXPECT_EQ(1 + itMap->second, filter->get(gnum));
		++itMap;
	}
	EXPECT_FALSE(itMap != equivMap_.end());

	auto itMapInv = equivInv_.begin();
	for (auto& gnum : HFilterInverted(filter)) {
		EXPECT_EQ(*itMapInv, gnum);
		EXPECT_EQ(0, filter->get(gnum));
		++itMapInv;
	}
	EXPECT_FALSE(itMapInv != equivInv_.end());

	// Test STL algorithms
	std::vector<gamenumT> included;
	std::copy(filter->begin(), filter->end(), std::back_inserter(included));
	std::vector<gamenumT> excluded;
	HFilterInverted inverted(filter);
	std::copy(inverted.begin(), inverted.end(), std::back_inserter(excluded));
	auto itIn = included.begin();
	auto itOut = excluded.begin();
	for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
		if (equivMap_.find(gnum) != equivMap_.end())
			EXPECT_EQ(gnum, *itIn++);
		else
			EXPECT_EQ(gnum, *itOut++);
	}
	EXPECT_FALSE(itIn != included.end());
	EXPECT_FALSE(itOut != excluded.end());
}

TEST_P(Test_HFilter, hfilter_nonconstFunc) {
	{ // Test HFilter::clear()
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		filter->clear();
		EXPECT_EQ(0U, filter->size());
		for (auto& gnum : *filter) {
			// filter is empty and should not enter into this loop
			EXPECT_NE(gnum, gnum);
		}
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			EXPECT_EQ(0, filter->get(gnum));
		}
	}
	{ // Test HFilter::erase()
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		auto mapCopy = equivMap_;
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			filter->erase(gnum);
			mapCopy.erase(gnum);
			EXPECT_EQ(0, filter->get(gnum));
			EXPECT_EQ(mapCopy.size(), filter->size());
			EXPECT_TRUE(equal(mapCopy, filter));
		}
	}
	{ // Test HFilter::includeAll()
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		filter->includeAll();
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			auto expected = (mask_ == nullptr) ? 1 : (*mask_)[gnum];
			EXPECT_EQ(expected, filter->get(gnum));
		}
		if (mask_ == nullptr)
			EXPECT_EQ(numGames_, filter->size());
		else
			EXPECT_EQ(maskSz_, filter->size());
	}
	{ // Test HFilter::insert_or_assign(gnum, 0)
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		auto mapCopy = equivMap_;
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			filter->insert_or_assign(gnum, 0);
			uint8_t expected = (mask_ == nullptr) ? 1 : (*mask_)[gnum];
			if (expected == 0)
				mapCopy.erase(gnum);
			else
				mapCopy[gnum] = expected - 1;
			EXPECT_EQ(expected, filter->get(gnum));
			EXPECT_EQ(mapCopy.size(), filter->size());
			EXPECT_TRUE(equal(mapCopy, filter));
		}
	}
	{ // Test HFilter::insert_or_assign()
		auto tmp = makeFilter(&dataEmpty);
		auto f2 = makeFilter(mask_);
		HFilter filter(tmp.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			uint8_t value = (*main_)[gnum];
			if (value == 0)
				filter->erase(gnum);
			else
				filter->insert_or_assign(gnum, value - 1);
		}
		EXPECT_EQ(equivMap_.size(), filter.size());
		EXPECT_TRUE(equal(equivMap_, filter));
	}
	{ // Test HFilter::insert_or_assign() with mask == nullptr
		auto tmp = makeFilter(&dataEmpty);
		HFilter filter(tmp.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			uint8_t value = (*main_)[gnum];
			if (mask_ != nullptr && value != 0)
				value = (*mask_)[gnum];
			if (value == 0)
				filter->erase(gnum);
			else
				filter->insert_or_assign(gnum, value - 1);
		}
		EXPECT_EQ(equivMap_.size(), filter.size());
		EXPECT_TRUE(equal(equivMap_, filter));
	}
	{ // Test HFilter::set(gnum, 0)
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		auto mapCopy = equivMap_;
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			filter->set(gnum, 0);
			mapCopy.erase(gnum);
			EXPECT_EQ(mapCopy.size(), filter->size());
			EXPECT_TRUE(equal(mapCopy, filter));
			EXPECT_EQ(0, filter->get(gnum));
		}
	}
	{ // Test HFilter::set(gnum, 1)
		auto f1 = makeFilter(main_);
		auto f2 = makeFilter(mask_);
		HFilter filter(f1.get(), f2.get());
		ASSERT_TRUE(filter != 0);
		ASSERT_FALSE(filter == 0);

		auto mapCopy = equivMap_;
		for (gamenumT gnum = 0; gnum < numGames_; gnum++) {
			uint8_t value = 1;
			filter->set(gnum, value);
			uint8_t expected = (mask_ == nullptr) ? value : (*mask_)[gnum];
			if (expected == 0)
				mapCopy.erase(gnum);
			else
				mapCopy[gnum] = expected - 1;
			EXPECT_EQ(expected, filter->get(gnum));
			EXPECT_EQ(mapCopy.size(), filter->size());
			EXPECT_TRUE(equal(mapCopy, filter));
		}
	}
}

INSTANTIATE_TEST_SUITE_P(HFilter, Test_HFilter, ::testing::ValuesIn(test_cases));
