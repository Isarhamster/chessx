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
 * Defines the ICodecDatabase interface, which encapsulates the data
 * representation of databases.
 */

#ifndef CODEC_H
#define CODEC_H

#include "common.h"
#include <string>
#include <vector>

class Game;
class Index;
class IndexEntry;
class NameBase;
class Progress;

/**
 * This interface separates the logic of a database from its representation.
 * Ideally all the file I/O should be encapsulated in classes derived from this
 * interface.
 */
class ICodecDatabase {
public:
	virtual ~ICodecDatabase(){};

	enum Codec { MEMORY, SCID4, PGN };
	/**
	 * Creates a new object and calls the virtual function dyn_open().
	 * @param codec:    the type of the object to be created.
	 * @param fMode:    a valid file mode.
	 * @param filename: the full path of the database to be opened.
	 * @param progress: a Progress object used for GUI communications.
	 * @param idx:      valid pointer to the Index object for this database.
	 * @param nb:       valid pointer to the NameBase object for this database.
	 * @returns
	 * - on success: a valid pointer to the new object and OK.
	 * - on error:   nullptr and the error code.
	 */
	static std::pair<ICodecDatabase*, errorT> open(Codec codec, fileModeT fMode,
	                                               const char* filename,
	                                               const Progress& progress,
	                                               Index* idx, NameBase* nb);

	/**
	 * Returns the Codec type.
	 */
	virtual Codec getType() const = 0;

	/**
	 * Returns the full path of the files used by the database.
	 * The order of the filenames must be consistent for objects of the same
	 * Codec type.
	 */
	virtual std::vector<std::string> getFilenames() const = 0;

	/**
	 * Returns a vector of tag pairs containing extra information about the
	 * database (type, description, autoload, etc..)
	 */
	virtual std::vector<std::pair<const char*, std::string>>
	getExtraInfo() const = 0;

	/**
	 * Store an extra information about the database (type, description, etc..)
	 */
	virtual errorT setExtraInfo(const char* tagname, const char* new_value) = 0;

	/**
	 * Fetches the data of a game (excluding index info), encoded in native
	 * format.
	 * @param offset: offset of the requested game.
	 * @param length: length of the game data (in bytes).
	 * @returns
	 * - a pointer to the game data.
	 * - 0 (nullptr) on error.
	 */
	virtual const byte* getGameData(uint64_t offset, uint32_t length) = 0;

	/**
	 * Add a game to the database.
	 * @param srcIe:   valid pointer to the header data.
	 * @param srcNb:   valid pointer to the NameBase containing srcIe's names.
	 * @param srcData: valid pointer to a buffer containing the game data
	 *                 (encoded in native format).
	 * @param dataLen: length of the game data (in bytes).
	 * @returns OK if successful or an error code.
	 */
	virtual errorT addGame(const IndexEntry* srcIe, const NameBase* srcNb,
	                       const byte* srcData, size_t dataLen) = 0;

	/**
	 * Add a game to the database.
	 * @param game: valid pointer to a Game object with the data of the game.
	 * @returns OK if successful or an error code.
	 */
	virtual errorT addGame(Game* game) = 0;

	/**
	 * Replaces a game in the database.
	 * @param game:     valid pointer to a Game object with the new data.
	 * @param replaced: valid gamenumT of the game to be replaced
	 * @returns OK if successful or an error code.
	 */
	virtual errorT saveGame(Game* game, gamenumT replaced) = 0;

	/**
	 * Replaces a game's IndexEntry (which contains the header data of a game).
	 * @param ie:       reference to the IndexEntry with the new data.
	 * @param replaced: valid gamenumT of the game to be replaced
	 * @returns OK if successful or an error code.
	 */
	virtual errorT saveIndexEntry(const IndexEntry& ie, gamenumT replaced) = 0;

	/**
	 * Adds a name (player, event, site or round) to the database.
	 * @param nt:   nameT type of the name to add.
	 * @param name: the name to add.
	 * @returns
	 * - on success, a @e std::pair containing OK and the corresponding ID.
	 * - on failure, a @e std::pair containing an error code and 0.
	 */
	virtual std::pair<errorT, idNumberT> addName(nameT nt,
	                                             const char* name) = 0;

	/**
	 * Writes all pending output to the files.
	 * @returns OK if successful or an error code.
	 */
	virtual errorT flush() = 0;

private:
	/**
	 * Opens/Creates a database.
	 * This virtual function is called only once immediately after the class
	 * constructor.
	 * @param fMode:    a valid file mode.
	 * @param filename: the full path of the database to open.
	 * @param progress: a Progress object used for GUI communications.
	 * @param idx:      valid pointer to the Index object for this database.
	 * @param nb:       valid pointer to the NameBase object for this database.
	 * @returns
	 * - OK: the object is ready to be used.
	 * - ERROR_NameDataLoss: some names are corrupted and cannot be recovered,
	 *                       however the object can still be used.
	 * - Other error codes: the operation failed (the object must be destroyed).
	 */
	virtual errorT dyn_open(fileModeT fMode, const char* filename,
	                        const Progress& progress, Index* idx,
	                        NameBase* nb) = 0;
};

#endif
