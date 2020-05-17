/*
 * Copyright (C) 2016-2018  Fulvio Benini

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
 * Implements the CodecSCID4 class that manages databases encoded in SCID
 * format v4.
 */

#ifndef CODEC_SCID4_H
#define CODEC_SCID4_H

#include "codec_native.h"
#include "filebuf.h"
#include <limits>

/**
 * This class manages databases encoded in SCID format v4.
 */
class CodecSCID4 : public CodecNative<CodecSCID4> {
	std::vector<std::string> filenames_;
	Filebuf idxfile_;
	FilebufAppend gfile_;
	char gamecache_[1ULL << 17];

	enum : uint64_t {
		LIMIT_GAMEOFFSET = 1ULL << 32,
		LIMIT_GAMELEN = 1ULL << 17,
		LIMIT_NUMGAMES = 16777214ULL, // Three bytes -1 because GetAutoLoad uses
		                              // 0 to mean "no autoload"
		LIMIT_NAMELEN = 255
	};

public: // ICodecDatabase interface
	Codec getType() const final { return ICodecDatabase::SCID4; }

	/**
	 * Returns the full path of the three files (index, namebase and gamefile)
	 * used by the database.
	 */
	std::vector<std::string> getFilenames() const final { return filenames_; };

	std::vector<std::pair<const char*, std::string>>
	getExtraInfo() const final {
		std::vector<std::pair<const char*, std::string>> res;
		res.emplace_back("type", std::to_string(idx_->Header.baseType));
		res.emplace_back("description", idx_->Header.description);
		const auto autoload = std::min(idx_->Header.autoLoad,
		                               idx_->GetNumGames());
		res.emplace_back("autoload", std::to_string(autoload));
		res.emplace_back("flag1", idx_->Header.customFlagDesc[0]);
		res.emplace_back("flag2", idx_->Header.customFlagDesc[1]);
		res.emplace_back("flag3", idx_->Header.customFlagDesc[2]);
		res.emplace_back("flag4", idx_->Header.customFlagDesc[3]);
		res.emplace_back("flag5", idx_->Header.customFlagDesc[4]);
		res.emplace_back("flag6", idx_->Header.customFlagDesc[5]);
		return res;
	}

	errorT setExtraInfo(const char* tagname, const char* new_value) override {
		if (std::strcmp(tagname, "type") == 0) {
			idx_->Header.baseType = strGetUnsigned(new_value);

		} else if (std::strcmp(tagname, "description") == 0) {
			strncpy(idx_->Header.description, new_value, SCID_DESC_LENGTH);
			idx_->Header.description[SCID_DESC_LENGTH] = 0;

		} else if (std::strcmp(tagname, "autoload") == 0) {
			idx_->Header.autoLoad = strGetUnsigned(new_value);

		} else {
			auto len = std::strlen(tagname);
			if (len != 5 || !std::equal(tagname, tagname + 4, "flag"))
				return ERROR_CodecUnsupFeat;

			uint flag = IndexEntry::CharToFlag(tagname[4]);
			if (flag < IndexEntry::IDX_FLAG_CUSTOM1 ||
			    flag > IndexEntry::IDX_FLAG_CUSTOM6)
				return ERROR_CodecUnsupFeat;

			const auto idx = flag - IndexEntry::IDX_FLAG_CUSTOM1;
			char* dest = idx_->Header.customFlagDesc[idx];
			strncpy(dest, new_value, CUSTOM_FLAG_DESC_LENGTH);
			dest[CUSTOM_FLAG_DESC_LENGTH] = 0;
		}

		idx_->Header.dirty_ = true;
		return OK;
	}

	const byte* getGameData(uint64_t offset, uint32_t length) final {
		if (offset >= gfile_.size())
			return NULL;
		if (length >= LIMIT_GAMELEN)
			return NULL;

		if (gfile_.pubseekpos(offset) == -1)
			return NULL;
		if (gfile_.sgetn(gamecache_, length) != std::streamsize(length))
			return NULL;

		return reinterpret_cast<const byte*>(gamecache_);
	}

	errorT saveIndexEntry(const IndexEntry& ie, gamenumT replaced) final {
		return dyn_saveIndexEntry(ie, replaced);
	}

	std::pair<errorT, idNumberT> addName(nameT nt, const char* name) final {
		return dyn_addName(nt, name);
	}

	errorT flush() final;

	errorT dyn_open(fileModeT, const char*, const Progress&, Index*,
	                NameBase*) final;

public: // CodecNative interface
	/**
	 * Stores the data into the .sg4 file.
	 * @param src:    valid pointer to a buffer that contains the game data
	 *                (encoded in native format).
	 * @param length: the length of the buffer @e src (in bytes).
	 * @returns
	 * - on success, a @e std::pair containing OK and the offset of the stored
	 *   data (needed for retrieving the data with getGameData()).
	 * - on failure, a @e std::pair containing an error code and 0.
	 */
	std::pair<errorT, uint64_t> dyn_addGameData(const byte* src,
	                                            size_t length) {
		ASSERT(src != 0);
		const char* data = reinterpret_cast<const char*>(src);

		if (length >= LIMIT_GAMELEN)
			return std::make_pair(ERROR_GameLengthLimit, 0);

		// The SCID4 format uses 32-bits to store games' offset.
		uint64_t offset = gfile_.size();
		if (offset >= LIMIT_GAMEOFFSET - length)
			return std::make_pair(ERROR_OffsetLimit, 0);

		// The SCID4 format stores games into blocks of 128KB.
		// If the current block does not have enough space, we fill it with
		// random data and use the next one.
		uint64_t blockSpace = LIMIT_GAMELEN - (offset % LIMIT_GAMELEN);
		if (blockSpace < length) {
			errorT err = gfile_.append(data, blockSpace);
			if (err != OK)
				return std::make_pair(err, 0);
			offset += blockSpace;
		}

		errorT err = gfile_.append(data, length);
		return std::make_pair(err, offset);
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
		const idNumberT MAX_ID[] = {
		    1048575, /* Player names: Maximum of 2^20 -1 = 1,048,575 */
		    524287,  /* Event names:  Maximum of 2^19 -1 =   524,287 */
		    524287,  /* Site names:   Maximum of 2^19 -1 =   524,287 */
		    262143   /* Round names:  Maximum of 2^18 -1 =   262,143 */
		};
		return nb_->addName(nt, name, LIMIT_NAMELEN, MAX_ID[nt]);
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
		idx_->Header.dirty_ = true;
		return writeEntry(ie, nGames);
	}

	/**
	 * Replace an IndexEntry.
	 * @param ie:       the IndexEntry with the new data.
	 * @param replaced: valid gamenumT of the game to be replaced.
	 * @returns OK if successful or an error code.
	 */
	errorT dyn_saveIndexEntry(const IndexEntry& ie, gamenumT replaced) {
		idx_->replaceEntry(ie, replaced);
		return writeEntry(ie, replaced);
	}

private:
	errorT readIndex(const Progress& progress);

	errorT writeEntry(const IndexEntry& ie, gamenumT gnum) {
		if (idx_->seqWrite_ == 0 || (gnum != idx_->seqWrite_ + 1)) {
			std::streampos pos = gnum;
			pos = pos * INDEX_ENTRY_SIZE + INDEX_HEADER_SIZE;
			if (idxfile_.pubseekpos(pos) != pos) {
				idx_->seqWrite_ = 0;
				return ERROR_FileWrite;
			}
		}
		errorT res = ie.Write(&idxfile_, idx_->Header.version);
		idx_->seqWrite_ = (res == OK) ? gnum : 0;
		return res;
	}
};

#endif
