/*
 * Copyright (C) 2017  Fulvio Benini

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
 * Implements the CodecSCID4 class, which manages the databases encoded
 * in Scid format version 4.
 */

#include "codec_scid4.h"
#include <algorithm>

namespace {

/**
 * A NameBase file starts with an header containing:
 * - header_magic (8 bytes): identify the file format
 * - unused (4 bytes):  obsolete timeStamp
 * - number of NAME_PLAYER names stored in the file (3 bytes)
 * - number of NAME_EVENT names stored in the file (3 bytes)
 * - number of NAME_SITE names stored in the file (3 bytes)
 * - number of NAME_ROUND names stored in the file (3 bytes)
 * - unused (12 bytes): obsolete max frequency
 * Names are stored in alphabetical order using front-coding and each record is
 * composed by:
 * - name_id (2-3 bytes): the idx (idNumberT) stored in the Index (.si4) file
 * - unused (1-3 bytes): obsolete frequency
 * - length (1 byte): the total number of bytes of the name (max 255)
 * - prefix (1 byte): the number of bytes in common with the previous name
 * - name (0-255 bytes): the part of the name that differs from the previous
 *   one.
 */
const char* NAMEBASE_MAGIC = "Scid.sn";

/**
 * Read a SCIDv4 NameBase file into memory.
 * @param filename: the full path of the file to open.
 * @param fMode:    a valid file mode.
 * @param nb:       reference to the object where the names will be stored.
 * @returns OK if successful or an error code.
 */
errorT namefileRead(const char* filename, fileModeT fmode, NameBase& nb) {
	auto nb_data = nb.getData();
	auto& map = std::get<0>(nb_data);
	auto& names = std::get<1>(nb_data);
	auto& eloV = std::get<2>(nb_data);

	Filebuf file;
	if (file.Open(filename, fmode) != OK)
		return ERROR_FileOpen;

	char Header_magic[9] = {0}; // magic identifier must be "Scid.sn"
	file.sgetn(Header_magic, 8);
	if (strcmp(Header_magic, NAMEBASE_MAGIC) != 0)
		return ERROR_BadMagic;

	// *** Compatibility ***
	// Even if timeStamp is not used we still need to read the bytes
	file.ReadFourBytes();
	// ***

	idNumberT Header_numNames[NUM_NAME_TYPES];
	Header_numNames[NAME_PLAYER] = file.ReadThreeBytes();
	Header_numNames[NAME_EVENT] = file.ReadThreeBytes();
	Header_numNames[NAME_SITE] = file.ReadThreeBytes();
	Header_numNames[NAME_ROUND] = file.ReadThreeBytes();

	// *** Compatibility ***
	// Even if frequency is no longer used we still need to read the bytes
	uint obsolete_maxFreq[NUM_NAME_TYPES];
	obsolete_maxFreq[NAME_PLAYER] = file.ReadThreeBytes();
	obsolete_maxFreq[NAME_EVENT] = file.ReadThreeBytes();
	obsolete_maxFreq[NAME_SITE] = file.ReadThreeBytes();
	obsolete_maxFreq[NAME_ROUND] = file.ReadThreeBytes();
	// ***

	eloV.resize(Header_numNames[NAME_PLAYER], 0);
	for (nameT nt : {NAME_PLAYER, NAME_EVENT, NAME_SITE, NAME_ROUND}) {
		names[nt].resize(Header_numNames[nt]);
		idNumberT id;
		std::string prevName;
		for (idNumberT i = 0; i < Header_numNames[nt]; i++) {
			if (Header_numNames[nt] >= 65536) {
				id = file.ReadThreeBytes();
			} else {
				id = file.ReadTwoBytes();
			}

			// *** Compatibility ***
			// Even if frequency is no longer used we still need to read the
			// bytes Frequencies can be stored in 1, 2 or 3 bytes:
			if (obsolete_maxFreq[nt] >= 65536) {
				file.ReadThreeBytes();
			} else if (obsolete_maxFreq[nt] >= 256) {
				file.ReadTwoBytes();
			} else { // Frequencies all <= 255: fit in one byte
				file.ReadOneByte();
			}
			// ***

			// Read the name string.
			// All strings EXCEPT the first are front-coded.
			int length = file.ReadOneByte();
			int prefix = (i > 0) ? file.ReadOneByte() : 0;
			if (prefix > length)
				return ERROR_Corrupt;

			char* name = new char[length + 1];
			std::copy_n(prevName.c_str(), prefix, name);
			std::streamsize extra_chars = length - prefix;
			if (extra_chars != file.sgetn(name + prefix, extra_chars)) {
				delete[] name;
				return ERROR_FileRead;
			}
			name[length] = 0;
			prevName.assign(name, length);

			if (id < Header_numNames[nt] && names[nt][id] == 0) {
				names[nt][id].reset(name);
				map[nt].insert(map[nt].end(), std::make_pair(name, id));
			} else {
				delete[] name;
				return ERROR_Corrupt;
			}
		}

		if (map[nt].size() != names[nt].size())
			return ERROR_Corrupt;
	}

	return OK;
}

/**
 * Write a SCIDv4 NameBase file.
 * @param filename: the full path of the file to open.
 * @param nb:       reference to the object where the names will be stored.
 * @returns OK if successful or an error code.
 */
template <typename TCont, typename TFreq>
errorT namefileWrite(const char* filename, const TCont& names_ids,
                     const TFreq& freq) {
	Filebuf file;
	if (file.Open(filename, FMODE_WriteOnly) != OK)
		return ERROR_FileOpen;

	file.sputn(NAMEBASE_MAGIC, 8);

	// *** Compatibility ***
	// Even if timeStamp is not used we still need to write these bytes
	file.WriteFourBytes(0);
	// ***

	ASSERT(1ULL << 24 > names_ids[NAME_PLAYER].size());
	ASSERT(1ULL << 24 > names_ids[NAME_EVENT].size());
	ASSERT(1ULL << 24 > names_ids[NAME_SITE].size());
	ASSERT(1ULL << 24 > names_ids[NAME_ROUND].size());
	file.WriteThreeBytes((uint32_t)names_ids[NAME_PLAYER].size());
	file.WriteThreeBytes((uint32_t)names_ids[NAME_EVENT].size());
	file.WriteThreeBytes((uint32_t)names_ids[NAME_SITE].size());
	file.WriteThreeBytes((uint32_t)names_ids[NAME_ROUND].size());

	// *** Compatibility ***
	// even if maxFrequency is no longer used we still need to write these bytes
	unsigned maxFreq[NUM_NAME_TYPES] = {0};
	for (nameT nt : {NAME_PLAYER, NAME_EVENT, NAME_SITE, NAME_ROUND}) {
		auto it = std::max_element(freq[nt].begin(), freq[nt].end());
		maxFreq[nt] = (it == freq[nt].end()) ? 0 : *it;
		file.WriteThreeBytes(maxFreq[nt]);
	}
	// ***

	for (nameT nt : {NAME_PLAYER, NAME_EVENT, NAME_SITE, NAME_ROUND}) {
		const char* prevName = nullptr;
		size_t numNames = names_ids[nt].size();
		for (const auto& it : names_ids[nt]) {
			const char* name = it.first;
			idNumberT id = it.second;

			// *** Compatibility ***
			// Older code used a custom StrTree class with a peculiar sorting:
			// - the first char was interpreted as an unsigned char;
			// - the remaining part was compared with strComapare(),
			//   which converts the chars to ints, and is not consistent with
			//   the standard function strcmp().
			// The old StrTree class did also have unpredictable behaviors when
			// fed with names not sorted according to that criteria, for example
			// it could create Namebase objects with duplicate entries.
			// ***
			ASSERT(prevName == nullptr ||
			       static_cast<uint>(*prevName) < static_cast<uint>(*name) ||
			       (static_cast<uint>(*prevName) == static_cast<uint>(*name) &&
			        strCompare(prevName, name) < 0));

			// write idNumber in 2 bytes if possible, otherwise 3.
			if (numNames >= 65536) {
				file.WriteThreeBytes(id);
			} else {
				file.WriteTwoBytes(id);
			}

			// *** Compatibility ***
			// write these bytes even if they are not used anymore
			if (maxFreq[nt] >= 65536) {
				file.WriteThreeBytes(freq[nt][id]);
			} else if (maxFreq[nt] >= 256) {
				file.WriteTwoBytes(freq[nt][id]);
			} else {
				file.WriteOneByte(static_cast<byte>(freq[nt][id]));
			}
			// ***

			ASSERT(strlen(name) < 256);
			byte length = static_cast<byte>(strlen(name));
			file.WriteOneByte(length);
			byte prefix = 0;
			if (prevName) {
				prefix = (byte)strPrefix(name, prevName);
				file.WriteOneByte(prefix);
			}
			file.sputn(name + prefix, (length - prefix));
			prevName = name;
		}
	}
	return OK;
}

/**
 * An Index file starts with an header of 182 bytes containing:
 * - index_magic (8 bytes): identifies the file format.
 * - version (2 bytes): 300 or 400
 * - baseType (4 bytes): e.g. tournament, theory, etc.
 * - numGames (3 bytes): total number of games contained into the index.
 * - autoLoad (3 bytes): number of the game to load at start
 *                       (0=none, 1=1st, >numGames=last).
 * - description (108 bytes): a null-terminated string describing the database.
 * - flag1 description (9 bytes): a null-terminated string describing flag1.
 * - flag2 description (9 bytes): a null-terminated string describing flag2.
 * - flag3 description (9 bytes): a null-terminated string describing flag3.
 * - flag4 description (9 bytes): a null-terminated string describing flag4.
 * - flag5 description (9 bytes): a null-terminated string describing flag5.
 * - flag6 description (9 bytes): a null-terminated string describing flag6.
 */
constexpr char INDEX_MAGIC[8] = "Scid.si";

/// Read the header section of a SCIDv4 Index file into memory.
/// @param indexFile: file handle positioned at the start of the Index file.
/// @param fMode:     a valid file mode.
/// @param header:    reference to the object where the data will be stored.
/// @returns OK if successful or an error code.
template <typename FileT, typename HeaderT>
errorT readIndexHeader(FileT& indexFile, fileModeT fmode, HeaderT& header) {
	constexpr versionT SCID_VERSION = 400; // Current file format version = 4.0
	constexpr versionT SCID_OLDEST_VERSION = 300; // Oldest readable version

	char magic[8];
	indexFile.sgetn(magic, 8);
	if (!std::equal(std::begin(magic), std::end(magic), std::begin(INDEX_MAGIC),
	                std::end(INDEX_MAGIC))) {
		return ERROR_BadMagic;
	}

	header.version = indexFile.ReadTwoBytes();
	if (header.version < SCID_OLDEST_VERSION || header.version > SCID_VERSION) {
		return ERROR_FileVersion;
	}
	if (header.version != SCID_VERSION && fmode != FMODE_ReadOnly) {
		// Old versions must be opened readonly
		return ERROR_FileMode;
	}

	header.baseType = indexFile.ReadFourBytes();
	header.numGames = indexFile.ReadThreeBytes();
	header.autoLoad = indexFile.ReadThreeBytes();
	indexFile.sgetn(header.description, SCID_DESC_LENGTH + 1);
	header.description[SCID_DESC_LENGTH] = 0;
	if (header.version >= 400) {
		for (uint i = 0; i < CUSTOM_FLAG_MAX; i++) {
			indexFile.sgetn(header.customFlagDesc[i],
			                CUSTOM_FLAG_DESC_LENGTH + 1);
			header.customFlagDesc[i][CUSTOM_FLAG_DESC_LENGTH] = 0;
		}
	}
	return OK;
}

/// Write the header section of a SCIDv4 Index file.
/// @param indexFile: file handle of the Index file.
/// @param header:    reference to the object containing the header data.
/// @returns OK if successful or an error code.
template <typename FileT, typename HeaderT>
errorT writeIndexHeader(FileT& indexFile, HeaderT& Header) {
	if (indexFile.pubseekpos(0) != std::streampos(0))
		return ERROR_FileWrite;

	std::streamsize n = 0;
	n += indexFile.sputn(INDEX_MAGIC, 8);
	n += indexFile.WriteTwoBytes(Header.version);
	n += indexFile.WriteFourBytes(Header.baseType);
	n += indexFile.WriteThreeBytes(Header.numGames);
	n += indexFile.WriteThreeBytes(Header.autoLoad);
	n += indexFile.sputn(Header.description, SCID_DESC_LENGTH + 1);
	for (size_t i = 0; i < CUSTOM_FLAG_MAX; i++) {
		n += indexFile.sputn(Header.customFlagDesc[i],
		                     CUSTOM_FLAG_DESC_LENGTH + 1);
	}
	if (n != INDEX_HEADER_SIZE || indexFile.pubsync() == -1)
		return ERROR_FileWrite;

	return OK;
}

} // namespace

/**
 * Decode SCID4 (or SCID3) data into an IndexEntry object.
 * @param buf_it:  pointer to the buffer containing the data
 *                 (should contain INDEX_ENTRY_SIZE chars)
 * @param version: 400 for SCID4 or 300 for SCID3.
 * @param ie:      pointer to the IndexEntry object where the data will be
 *                 stored.
 */
void decodeIndexEntry(const char* buf_it, versionT version, IndexEntry* ie) {
	auto ReadOneByte = [&buf_it]() {
		uint8_t res = *buf_it++;
		return res;
	};
	auto ReadTwoBytes = [&ReadOneByte]() {
		uint16_t high = ReadOneByte();
		uint16_t res = (high << 8) | ReadOneByte();
		return res;
	};
	auto ReadFourBytes = [&ReadTwoBytes]() {
		uint32_t high = ReadTwoBytes();
		uint32_t res = (high << 16) | ReadTwoBytes();
		return res;
	};

	// Offset of the gamefile record (32 bits).
	ie->SetOffset(ReadFourBytes());

	// Length of gamefile record for this game: 17 bits are used so the max
	// length is 128 ko (131071).
	// Lower bits of the extra byte are used for custom flags: LxFFFFFF ( L =
	// length for long games, x = spare, F = custom flags)
	uint32_t len_Low = ReadTwoBytes();
	uint32_t len_flags = (version < 400) ? 0 : ReadOneByte();
	ie->SetLength(((len_flags & 0x80) << 9) | len_Low);
	uint32_t Flags = ReadTwoBytes();
	ie->clearFlags();
	ie->SetFlag(((len_flags & 0x3F) << 16) | Flags, true);

	// WhiteID and BlackID are 20-bit values, EventID and SiteID are
	// 19-bit values, and RoundID is an 18-bit value.
	// WhiteID high 4 bits = bits 4-7 of WhiteBlack_High.
	// BlackID high 4 bits = bits 0-3 of WhiteBlack_High.
	// EventID high 3 bits = bits 5-7 of EventSiteRnd_high.
	// SiteID  high 3 bits = bits 2-4 of EventSiteRnd_high.
	// RoundID high 2 bits = bits 0-1 of EventSiteRnd_high.
	uint32_t WhiteBlack_High = ReadOneByte();
	uint32_t WhiteID_Low = ReadTwoBytes();
	ie->SetWhite(((WhiteBlack_High & 0xF0) << 12) | WhiteID_Low);
	uint32_t BlackID_Low = ReadTwoBytes();
	ie->SetBlack(((WhiteBlack_High & 0x0F) << 16) | BlackID_Low);
	uint32_t EventSiteRnd_High = ReadOneByte();
	uint32_t EventID_Low = ReadTwoBytes();
	ie->SetEvent(((EventSiteRnd_High & 0xE0) << 11) | EventID_Low);
	uint32_t SiteID_Low = ReadTwoBytes();
	ie->SetSite(((EventSiteRnd_High & 0x1C) << 14) | SiteID_Low);
	uint32_t RoundID_Low = ReadTwoBytes();
	ie->SetRound(((EventSiteRnd_High & 0x03) << 16) | RoundID_Low);

	// Counters for comments, variations, etc. (4 bits each)
	// VarCounts also stores the result (4 bits).
	uint32_t varCounts = ReadTwoBytes();
	ie->SetRawVariationCount(varCounts & 0x0F);
	ie->SetRawCommentCount((varCounts >> 4) & 0x0F);
	ie->SetRawNagCount((varCounts >> 8) & 0x0F);
	ie->SetResult((varCounts >> 12) & 0x0F);

	// ECO code (16 bits)
	ie->SetEcoCode(ReadTwoBytes());

	// Date and EventDate are stored in four bytes.
	// Due to a compact encoding format, the EventDate
	// must be within a few years of the Date.
	uint32_t date_edate = ReadFourBytes();
	uint32_t date = date_edate & 0xFFFFF;
	ie->SetDate(date);
	uint32_t edate = date_edate >> 20;
	uint32_t eyear = date_GetYear(edate) & 0x07;
	if (eyear == 0) {
		edate = ZERO_DATE;
	} else {
		eyear += date_GetYear(date);
		eyear = (eyear < 4) ? 0 : eyear - 4;
		edate = DATE_MAKE(eyear, date_GetMonth(edate), date_GetDay(edate));
	}
	ie->SetEventDate(edate);

	// The two ELO ratings and rating types take 2 bytes each.
	uint16_t whiteElo = ReadTwoBytes();
	ie->SetWhiteElo(whiteElo & 0xFFF);
	ie->SetWhiteRatingType(whiteElo >> 12);
	uint16_t blackElo = ReadTwoBytes();
	ie->SetBlackElo(blackElo & 0xFFF);
	ie->SetBlackRatingType(blackElo >> 12);

	// material of the final position in the game,
	// and the StoredLineCode in the top 8 bits.
	uint32_t finalMatSig = ReadFourBytes();
	ie->SetFinalMatSig(finalMatSig & 0xFFFFFF);
	ie->SetStoredLineCode(finalMatSig >> 24);

	// Read the 9-byte homePawnData array:
	// The first byte of HomePawnData has high bits of the NumHalfMoves
	// counter in its top two bits:
	uint16_t NumHalfMoves = ReadOneByte();
	uint16_t pawnData0 = ReadOneByte();
	ie->SetNumHalfMoves(((pawnData0 & 0xC0) << 2) | NumHalfMoves);
	ie->SetHomePawnData(pawnData0 & 0x3F,
	                    reinterpret_cast<const unsigned char*>(buf_it));
}

errorT CodecSCID4::dyn_open(fileModeT fMode, const char* filename,
                            const Progress& progress, Index* idx,
                            NameBase* nb) {
	if (fMode == FMODE_WriteOnly || !filename || !idx || !nb)
		return ERROR;
	if (*filename == '\0')
		return ERROR_FileOpen;

	idx_ = idx;
	idx_->Init();
	nb_ = nb;
	filenames_.resize(3);
	filenames_[0] = std::string(filename) + ".si4";
	filenames_[1] = std::string(filename) + ".sn4";
	filenames_[2] = std::string(filename) + ".sg4";

	errorT err = gfile_.open(filenames_[2], fMode);
	if (err != OK)
		return err;

	const char* indexFilename = filenames_[0].c_str();
	if (fMode == FMODE_Create) {
		// Check that the file does not exists
		if (idxfile_.Open(indexFilename, FMODE_ReadOnly) == OK)
			err = ERROR_FileOpen;

		if (err == OK)
			err = idxfile_.Open(indexFilename, FMODE_Create);

		if (err == OK)
			err = writeIndexHeader(idxfile_, idx_->Header);

		if (err == OK) {
			err = namefileWrite(filenames_[1].c_str(), nb_->getNames(),
			                    idx_->calcNameFreq(*nb_));
		}
	} else {
		err = idxfile_.Open(indexFilename, fMode);

		if (err == OK)
			err = readIndexHeader(idxfile_, fMode, idx_->Header);

		if (err == OK)
			err = namefileRead(filenames_[1].c_str(), fMode, *nb_);

		if (err == OK)
			err = readIndex(progress);
	}

	return err;
}

errorT CodecSCID4::flush() {
	idx_->seqWrite_ = 0;
	errorT errHeader = OK;
	if (idx_->Header.dirty_) {
		errHeader = writeIndexHeader(idxfile_, idx_->Header);
		if (errHeader == OK)
			idx_->Header.dirty_ = false;
	}
	errorT errSync = (idxfile_.pubsync() != 0) ? ERROR_FileWrite : OK;
	errorT err = (errHeader == OK) ? errSync : errHeader;

	if (err == OK) {
		// *** Compatibility ***
		// Even if name's frequency is no longer used, it's necessary to
		// keep the compatibility with older Scid versions, forcing a
		// recalculation.
		err = namefileWrite(filenames_[1].c_str(), nb_->getNames(),
		                    idx_->calcNameFreq(*nb_));
	}
	errorT errGfile = (gfile_.pubsync() == 0) ? OK : ERROR_FileWrite;

	return (err == OK) ? errGfile : err;
}

/**
 * Reads the entire index file into memory.
 * Invalid name IDs are replaced with "?" if possible.
 * @param progress: a Progress object used for GUI communications.
 * @returns OK if successful or an error code.
 */
inline errorT CodecSCID4::readIndex(const Progress& progress) {
	gamenumT nUnknowIDs = 0;
	idNumberT maxID[NUM_NAME_TYPES];
	for (nameT nt = NAME_PLAYER; nt < NUM_NAME_TYPES; nt++) {
		maxID[nt] = nb_->GetNumNames(nt);
	}
	auto validateNameIDs = [&](IndexEntry* ie) {
		if (ie->GetWhite() >= maxID[NAME_PLAYER]) {
			auto unknown = dyn_addName(NAME_PLAYER, "?");
			if (unknown.first != OK)
				return false;
			ie->SetWhite(unknown.second);
			++nUnknowIDs;
		}
		if (ie->GetBlack() >= maxID[NAME_PLAYER]) {
			auto unknown = dyn_addName(NAME_PLAYER, "?");
			if (unknown.first != OK)
				return false;
			ie->SetBlack(unknown.second);
			++nUnknowIDs;
		}
		if (ie->GetEvent() >= maxID[NAME_EVENT]) {
			auto unknown = dyn_addName(NAME_EVENT, "?");
			if (unknown.first != OK)
				return false;
			ie->SetEvent(unknown.second);
			++nUnknowIDs;
		}
		if (ie->GetSite() >= maxID[NAME_SITE]) {
			auto unknown = dyn_addName(NAME_SITE, "?");
			if (unknown.first != OK)
				return false;
			ie->SetSite(unknown.second);
			++nUnknowIDs;
		}
		if (ie->GetRound() >= maxID[NAME_ROUND]) {
			auto unknown = dyn_addName(NAME_ROUND, "?");
			if (unknown.first != OK)
				return false;
			ie->SetRound(unknown.second);
			++nUnknowIDs;
		}
		return true;
	};

	auto version = idx_->Header.version;
	auto nGames = idx_->GetNumGames();
	idx_->entries_.resize(nGames);

	auto nBytes = (version < 400) ? OLD_INDEX_ENTRY_SIZE : INDEX_ENTRY_SIZE;
	for (gamenumT gNum = 0; idxfile_.sgetc() != EOF; ++gNum) {
		if (gNum == nGames)
			return ERROR_CorruptData;

		if ((gNum % 8192) == 0) {
			if (!progress.report(gNum, nGames))
				return ERROR_UserCancel;
		}

		char buf[INDEX_ENTRY_SIZE];
		if (idxfile_.sgetn(buf, nBytes) != nBytes)
			return ERROR_FileRead;

		IndexEntry& ie = idx_->entries_[gNum];
		decodeIndexEntry(buf, version, &ie);

		if (!validateNameIDs(&ie))
			return ERROR_CorruptData;

		nb_->AddElo(ie.GetWhite(), ie.GetWhiteElo());
		nb_->AddElo(ie.GetBlack(), ie.GetBlackElo());
	}
	progress.report(1, 1);

	if (nGames != idx_->GetNumGames())
		return ERROR_FileRead;

	idx_->nInvalidNameId_ = nUnknowIDs;
	return (nUnknowIDs == 0) ? OK : ERROR_NameDataLoss;
}
