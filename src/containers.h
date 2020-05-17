/*
# Copyright (C) 2014-2017 Fulvio Benini

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

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <cassert>
#include <vector>

/**
 * A container useful for implementing a undo-redo behavior.
 * @e UNDOMAX: max number of copies to store.
 * Typical use:
 * store(obj_ptr); // (1)
 * modify obj;
 * obj_ptr = undo(obj_ptr); // obj_ptr now contains the copy of (1)
 */
template <class TElem, size_t UNDOMAX> class UndoRedo {
	std::vector<TElem*> undo_;
	std::vector<TElem*> redo_;

public:
	~UndoRedo() { clear(); }
	void clear() {
		clear(undo_);
		clear(redo_);
	}
	size_t undoSize() const { return undo_.size(); }
	size_t redoSize() const { return redo_.size(); }

	/**
	 * Stores a copy of an element into the undo queue.
	 * Deletes all the objects of the redo queue.
	 * @param current: the element to clone and store.
	 */
	void store(TElem* current) {
		clear(redo_);
		undo_.push_back(current->clone());
		if (undo_.size() > UNDOMAX) {
			delete undo_.front();
			undo_.erase(undo_.begin());
		}
	}

	/**
	 * Retrieve the last element from the undo queue.
	 * @param current: pointer to the current element; it will be stored into
	 *                 the redo queue.
	 * @returns the pointer to the last element of the undo queue (the pointer
	 * will be removed from the queue). If the queue is empty returns @e current
	 */
	TElem* undo(TElem* current) { return doUndoRedo(undo_, redo_, current); }

	/**
	 * Retrieve the last element from the redo queue.
	 * @param current: pointer to the current element; it will be stored into
	 *                 the undo queue.
	 * @returns the pointer to the last element of the redo queue (the pointer
	 * will be removed from the queue). If the queue is empty returns @e current
	 */
	TElem* redo(TElem* current) { return doUndoRedo(redo_, undo_, current); }

private:
	// Store current into cont2; remove and return the last element of cont1
	template <typename TCont>
	TElem* doUndoRedo(TCont& cont1, TCont& cont2, TElem* current) {
		if (cont1.empty())
			return current;

		if (cont2.empty() || cont2.back() != current)
			cont2.push_back(current);

		auto res = cont1.back();
		cont1.pop_back();
		return res;
	}

	template <typename TCont> void clear(TCont& cont) {
		for (auto& e : cont)
			delete e;
		cont.clear();
	}
};

/**
 * A vector-like container. Not all the elements are stored contiguously like in
 * a normal vector, but are allocated in separate chunks so that:
 * - growing in size do not move the elements and old references remain valid;
 * - very large containers can be created.
 * @e CHUNKSHIFT: is the base-2 logarithm of the number of T entries per chunk.
 *                Total size of a chunk: (2^CHUNKSHIFT)*sizeof(T)
 */
template <class T, size_t CHUNKSHIFT> class VectorChunked {
	std::vector<T*> chunks_;
	size_t size_ = 0;

	static constexpr size_t low_mask = ((1ULL << CHUNKSHIFT) - 1);

public:
	VectorChunked() = default;
	VectorChunked(const VectorChunked&) = delete;
	VectorChunked& operator=(const VectorChunked&) = delete;
	~VectorChunked() {
		for (auto& chunk : chunks_)
			delete[] chunk;
	}

	const T& operator[](size_t idx) const {
		assert(idx < size_);
		return chunks_[idx >> CHUNKSHIFT][idx & low_mask];
	}
	T& operator[](size_t idx) {
		assert(idx < size_);
		return chunks_[idx >> CHUNKSHIFT][idx & low_mask];
	}

	size_t capacity() const { return chunks_.size() << CHUNKSHIFT; }

	/**
	 * @returns
	 * the count of contiguously allocated objects starting at @e pos (included)
	 */
	size_t contiguous(size_t pos) const {
		assert(pos < size());
		return 1 + (~pos & low_mask);
	}

	void push_back(const T& e) {
		size_t idx = size_;
		resize(size_ + 1);
		operator[](idx) = e;
	}

	void resize(size_t count) {
		size_ = count;
		size_t newSize = (count > 0) ? 1 + (count >> CHUNKSHIFT) : 0;
		size_t chunksSz = chunks_.size();
		if (newSize == chunksSz)
			return;

		if (newSize > chunksSz) {
			chunks_.resize(newSize);
			for (auto i = chunksSz; i < newSize; ++i) {
				chunks_[i] = new T[1ULL << CHUNKSHIFT];
			}
		} else {
			for (auto i = newSize; i < chunksSz; ++i) {
				delete[] chunks_[i];
			}
			chunks_.resize(newSize);
		}
	}

	size_t size() const { return size_; }
};

#endif
