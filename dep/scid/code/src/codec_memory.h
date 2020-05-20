/*
 * Copyright (C) 2016-2017  Fulvio Benini

 * This file is part of Scid (Shane's Chess Information Database).
 *
 * Scid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * Scid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scid.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** @file
 * Implements the CodecMemory class, which represent a memory database.
 */

#ifndef CODEC_MEMORY_H
#define CODEC_MEMORY_H

#include "codec_native.h"

/**
 * Manages memory databases that do not have associated files.
 * Every open database should have a native representation in memory: to satisfy
 * this requirement non-native codecs should be derived from this class.
 */
class CodecMemory : public CodecNative<CodecMemory> {
	VectorChunked<byte, 24> v_;
	unsigned baseType_ = 0;

	enum : uint64_t {
		LIMIT_GAMEOFFSET = 1ULL << 46,
		LIMIT_GAMELEN = 1ULL << 18,
		LIMIT_NUMGAMES = (1ULL << 32) - 2,
		LIMIT_UNIQUENAMES = 1ULL << 28,
		LIMIT_NAMELEN = 255
	};

public: // ICodecDatabase interface
	Codec getType() const override { return ICodecDatabase::MEMORY; }

	std::vector<std::string> getFilenames() const override {
		return std::vector<std::string>();
	}

	std::vector<std::pair<const char*, std::string>>
	getExtraInfo() const override {
		std::vector<std::pair<const char*, std::string>> res;
		res.emplace_back("type", std::to_string(baseType_));
		return res;
	}

	errorT setExtraInfo(const char* tagname, const char* new_value) override {
		if (std::strcmp(tagname, "type") == 0) {
			baseType_ = strGetUnsigned(new_value);
			return OK;
		}
		return ERROR_CodecUnsupFeat;
	}

	const byte* getGameData(uint64_t offset, uint32_t length) override {
		ASSERT(offset < v_.size());
		ASSERT(length <= v_.size() - offset);
		ASSERT(v_.contiguous(static_cast<size_t>(offset)) >= length);
		return &v_[offset];
	}

	errorT saveIndexEntry(const IndexEntry& ie, gamenumT replaced) override {
		return dyn_saveIndexEntry(ie, replaced);
	}

	std::pair<errorT, idNumberT> addName(nameT nt, const char* name) override {
		return dyn_addName(nt, name);
	}

	errorT flush() override { return OK; }

	errorT dyn_open(fileModeT fMode, const char*, const Progress&, Index* idx,
	                NameBase* nb) override {
		if (idx == 0 || nb == 0)
			return ERROR;
		if (fMode != FMODE_Create)
			return ERROR;
		idx_ = idx;
		nb_ = nb;
		return OK;
	}

public: // CodecNative CRTP
	/**
	 * Stores the data of a game into memory.
	 * @param src:    valid pointer to a buffer that contains the game data
	 *                (encoded in native format).
	 * @param length: the length of the buffer @p src (in bytes).
	 * @returns
	 * - on success, a @e std::pair containing OK and the offset of the stored
	 * data (usable to retrieve the data with getGameData()).
	 * - on failure, a @e std::pair containing an error code and 0.
	 */
	std::pair<errorT, uint64_t> dyn_addGameData(const byte* src,
	                                            size_t length) {
		ASSERT(src != 0);

		if (length >= LIMIT_GAMELEN)
			return std::make_pair(ERROR_GameLengthLimit, 0);

		auto offset = v_.size();
		auto capacity = v_.capacity();
		if (capacity - offset < length) // Doesn't fit in the current chunk
			offset = capacity;
		if (offset >= LIMIT_GAMEOFFSET)
			return std::make_pair(ERROR_OffsetLimit, 0);

		v_.resize(offset + length);
		ASSERT(v_.contiguous(offset) >= length);
		std::copy_n(src, length, &v_[offset]);
		return {OK, offset};
	}

	/**
	 * Given a name (string), retrieve the corresponding ID.
	 * The name is added to @e nb_ if do not already exists in the NameBase.
	 * @param nt:   nameT type of the name to retrieve.
	 * @param name: the name to retrieve.
	 * @returns
	 * - on success, a @e std::pair containing OK and the ID.
	 * - on failure, a @e std::pair containing an error code and 0.
	 */
	std::pair<errorT, idNumberT> dyn_addName(nameT nt, const char* name) {
		return nb_->addName(nt, name, LIMIT_NAMELEN, LIMIT_UNIQUENAMES);
	}

	/**
	 * Add an IndexEntry to @e idx_.
	 * @param ie: the IndexEntry object to add.
	 * @returns OK if successful or an error code.
	 */
	errorT dyn_addIndexEntry(const IndexEntry& ie) {
		const auto nGames = idx_->GetNumGames();
		if (nGames >= LIMIT_NUMGAMES)
			return ERROR_NumGamesLimit;

		idx_->addEntry(ie);
		return OK;
	}

	/**
	 * Replace an IndexEntry.
	 * @param ie:       the IndexEntry with the new data.
	 * @param replaced: valid gamenumT of the game to be replaced.
	 * @returns OK if successful or an error code.
	 */
	errorT dyn_saveIndexEntry(const IndexEntry& ie, gamenumT replaced) {
		idx_->replaceEntry(ie, replaced);
		return OK;
	}
};

#endif
