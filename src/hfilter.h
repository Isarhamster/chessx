/*
# Copyright (C) 2016-2018 Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
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

#ifndef SCID_HFILTER_H
#define SCID_HFILTER_H

#include "common.h"
#include <algorithm>
#include <iterator>
#include <memory>

/*
 * A database can be searched according to different criteria and the list of
 * matching games is stored into a Filter object.
 * A value of 0 indicates the game is excluded, or 1-255 indicates
 * the game is included, and what position to show when the game
 * is loaded: 1 means the start position, 2 means the position after
 * Whites first move, etc.
 */
class Filter {
	std::unique_ptr<byte[]> data_; // The actual filter data.
	gamenumT size_;                // Number of values in filter.
	gamenumT nonzero_;             // Number of nonzero values in filter.
	size_t capacity_;              // Number of values allocated for data_.

public:
	explicit Filter(gamenumT size)
	    : size_(size), nonzero_(size), capacity_(0) {}

	void Init(gamenumT size) {
		data_ = nullptr;
		nonzero_ = size_ = size;
	}

	/// Return a pointer to the allocated data.
	byte* data() { return data_.get(); }

	/// Return the number of nonzero values in filter.
	gamenumT Count() const { return nonzero_; }

	/// Return the number of elements in filter.
	gamenumT Size() const { return size_; }

	/// Changes the number of elements stored.
	void Resize(gamenumT size) {
		if (!data_) {
			nonzero_ = size;
		} else if (size < size_) {
			nonzero_ = size - static_cast<gamenumT>(std::count(
			                      data_.get(), data_.get() + size, 0));
		} else if (size > size_) {
			if (size > capacity_) {
				auto tmp(std::move(data_));
				allocate(size);
				std::copy_n(tmp.get(), size_, data_.get());
			}
			byte val = 0;
			if (Count() == Size()) {
				val = 1;
				nonzero_ = size;
			}
			std::fill(data_.get() + size_, data_.get() + size, val);
		}
		size_ = size;
	}

	/// Gets the value at index.
	byte Get(gamenumT index) const {
		ASSERT(index < Size());
		return data_ ? data_[index] : 1;
	}

	/// Sets the value at index.
	void Set(gamenumT index, byte value) {
		ASSERT(index < Size());
		if (data_) {
			if (value == 0) {
				if (data_[index] != 0)
					--nonzero_;
			} else if (data_[index] == 0) {
				++nonzero_;
			}
			data_[index] = value;
		} else if (value != 1) {
			allocate(size_);
			std::fill(data_.get(), data_.get() + size_, 1);
			data_[index] = value;
			if (value == 0)
				--nonzero_;
		}
	}

	/// Sets all values.
	void Fill(byte value) {
		if (value == 1) {
			data_ = nullptr;
			nonzero_ = size_;
		} else {
			if (!data_) {
				allocate(size_);
			}
			std::fill(data_.get(), data_.get() + size_, value);
			nonzero_ = (value == 0) ? 0 : size_;
		}
	}

private:
	void allocate(size_t size) {
		auto capacity = (size | 63) + 1;
		data_ = std::make_unique<byte[]>(capacity);
		capacity_ = capacity;
	}
};

/*
 * This class abstracts the Filter class providing an interface equivalent to a
 * pointer to a std::map<gamenumT, uint8_t> object, where the keys are the
 * gamenumT of matching games and the mapped_types are the number of half-moves
 * necessary to reach the matching position.
 * Searches that use only header informations (player names, dates, ...) match
 * at the starting position (0 half-moves).
 *
 * It is also possible to combine two filters in an efficient and transparent
 * way. If a secondary "mask" filter is supplied, the functions get(), size()
 * and the const_iterator consider only the games included in both filters.
 * Their behavior is equal to:
 * using Filter = std::map<gamenumT, uint8_t>;
 * Filter tmp_combined;
 * std::set_intersection(mask->begin(), mask->end(), main->begin(), main->end(),
 *     std::inserter(tmp_combined, tmp_combined.end()),
 *     [](auto& a, auto& b) { return a.first < b.first; });
 * return HFilter(&tmp_combined).begin/get/size();
 */
class HFilter {
	Filter* main_;
	const Filter* mask_;

public:
	/**
	 * class const_iterator - iterator for HFilter objects
	 *
	 * This class and the relative functions begin() and end() allow to use
	 * HFilter objects with STL algorithms and c++11 for-ranged loops.
	 * For example:
	 * for (auto& gnum : *hfilter_obj) {}
	 * is equal to:
	 * for (gamenumT gnum = 0, gnum < scidBaseT::numGames(); gnum++) {
	 *     if (hfilter_obj->get(gnum) == 0) continue;
	 * }
	 */
	class const_iterator {
		gamenumT gnum_;
		gamenumT end_;
		const HFilter* hfilter_;
		bool inFilter_;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;
		typedef gamenumT value_type;
		typedef const gamenumT* pointer;
		typedef const gamenumT& reference;

		const_iterator(gamenumT gnum, gamenumT end, const HFilter* hfilter,
		               bool inFilter = true)
		    : gnum_(gnum), end_(end), hfilter_(hfilter), inFilter_(inFilter) {
			ASSERT(hfilter != 0);
			if (gnum_ != end_) {
				bool included = (hfilter_->get(gnum_) != 0);
				if (included != inFilter_)
					operator++();
			}
		}

		reference operator*() const { return gnum_; }

		const_iterator& operator++() {
			while (++gnum_ != end_) {
				bool included = (hfilter_->get(gnum_) != 0);
				if (included == inFilter_)
					break;
			}
			return *this;
		}

		bool operator!=(const const_iterator& b) const {
			return gnum_ != b.gnum_ || hfilter_ != b.hfilter_;
		}
		bool operator==(const const_iterator& b) const {
			return !operator!=(b);
		}
	};

	const_iterator begin() const {
		return const_iterator(0, main_->Size(), this);
	}
	const_iterator end() const {
		return const_iterator(main_->Size(), main_->Size(), this);
	}
	const_iterator beginInverted() const {
		return const_iterator(0, main_->Size(), this, false);
	}
	const_iterator endInverted() const {
		return const_iterator(main_->Size(), main_->Size(), this, false);
	}
	size_t sizeInverted() const { return main_->Size() - size(); }

public: // Pointer interface
	bool operator==(const Filter* b) const { return main_ == b; }
	bool operator!=(const Filter* b) const { return main_ != b; }
	HFilter* operator->() { return this; }
	const HFilter* operator->() const { return this; }
	HFilter& operator*() { return *this; }
	const HFilter& operator*() const { return *this; }

public:
	explicit HFilter(Filter* main, const Filter* mask = 0)
	    : main_(main), mask_(mask) {}

	void clear() { return main_->Fill(0); }
	void erase(gamenumT gnum) { return main_->Set(gnum, 0); }
	void insert_or_assign(gamenumT gnum, uint8_t ply) {
		return main_->Set(gnum, ply + 1);
	}
	gamenumT size() const {
		if (!mask_ || mask_->Count() == mask_->Size())
			return main_->Count();
		if (main_->Count() == main_->Size())
			return mask_->Count();
		return static_cast<gamenumT>(std::distance(begin(), end()));
	}

	/// Returns the number of games included in the main filter
	size_t mainSize() const { return main_->Count(); }

	/* Convenience function, behave like:
	 * for (gamenumT gnum = 0; gnum < scidBaseT::numGames(); gnum++)
	 *		std:map::insert_or_assign(gnum, 0);
	 */
	void includeAll() { return main_->Fill(1); }

	/* Convenience function, behave like:
	 * auto it = std::map::find(gnum);
	 * if (it == std::map::end()) return 0;
	 * return 1 + it->second;
	 */
	byte get(gamenumT gnum) const {
		byte res = main_->Get(gnum);
		if (res != 0 && mask_ != 0)
			res = mask_->Get(gnum);

		return res;
	}

	/* Convenience function, behave like:
	 * if (value == 0)
	 *     erase(gnum);
	 * else
	 *     insert_or_assign(gnum, value - 1);
	 */
	void set(gamenumT gnum, byte value) { return main_->Set(gnum, value); }
};

/**
 * class HFilterInverted - iterate through games excluded from a filter
 *
 * This class allow to iterate through games not included in HFilter objects
 * using STL algorithms and c++11 for-ranged loops.
 * For example:
 * for (auto& gnum : HFilterInverted(hfilter_obj)) {}
 * is equal to:
 * for (gamenumT gnum = 0, gnum < scidBaseT::numGames(); gnum++) {
 *     if (hfilter_obj->get(gnum) != 0) continue;
 * }
 */
class HFilterInverted {
	const HFilter& hfilter_;

public:
	explicit HFilterInverted(const HFilter& hfilter) : hfilter_(hfilter) {
		ASSERT(hfilter != 0);
	}
	HFilter::const_iterator begin() const { return hfilter_.beginInverted(); }
	HFilter::const_iterator end() const { return hfilter_.endInverted(); }
	size_t size() const { return hfilter_.sizeInverted(); }
};

#endif
