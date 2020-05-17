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
 * Implements the CodecPgn class, which manages the databases encoded in PGN
 * format.
 */

#ifndef CODEC_PGN_H
#define CODEC_PGN_H

#include "codec_proxy.h"
#include "filebuf.h"
#include "pgnparse.h"
#include <algorithm>
#include <cstring>
#include <vector>

class CodecPgn : public CodecProxy<CodecPgn> {
	Filebuf file_;
	std::streamsize fileSize_ = 0;
	std::string filename_;
	std::vector<char> buf_;
	size_t nParsed_ = 0;
	size_t nRead_ = 0;
	PgnParseLog parseLog_;

public:
	Codec getType() const final { return ICodecDatabase::PGN; }

	std::vector<std::string> getFilenames() const final {
		return std::vector<std::string>(1, filename_);
	};

	errorT flush() final {
		errorT errFile = (file_.pubsync() == 0) ? OK : ERROR_FileWrite;
		errorT errProxy = CodecProxy<CodecPgn>::flush();
		return (errFile != OK) ? errFile : errProxy;
	}

	/**
	 * Opens/creates a PGN database.
	 * After successfully opening/creating the file, the object is ready for
	 * parseNext() calls.
	 * @param filename: full path of the pgn file to be opened.
	 * @param fmode:    valid file access mode.
	 * @returns OK in case of success, an @e errorT code otherwise.
	 */
	errorT open(const char* filename, fileModeT fmode) {
		ASSERT(filename && !file_.is_open());
		filename_ = filename;
		if (filename_.empty())
			return ERROR_FileOpen;

		errorT err_open = file_.Open(filename, fmode);
		if (err_open != OK)
			return err_open;

		buf_.resize(128 * 1024);
		nRead_ = nParsed_ = buf_.size();
		file_.pubsetbuf(nullptr, nRead_); // Optimization

		fileSize_ = file_.pubseekoff(0, std::ios::end);
		file_.pubseekpos(0);
		if (fileSize_ < 0)
			return ERROR_FileSeek;

		return OK;
	}

	/**
	 * Reads the next game.
	 * @param game: the Game object where the data will be stored.
	 * @returns
	 * - ERROR_NotFound if there are no more games to be read.
	 * - OK otherwise.
	 */
	errorT parseNext(Game& game) {
		const auto verge = 3 * (nRead_ / 4);
		if (nParsed_ > verge && nRead_ == buf_.size()) {
			nParsed_ -= verge;
			nRead_ -= verge;
			std::copy_n(buf_.data() + verge, nRead_, buf_.data());
			nRead_ += file_.sgetn(buf_.data() + nRead_, verge);
		}

		game.Clear();
		PgnVisitor visitor(game);
		auto parse = pgn::parse_game(
		    {buf_.data() + nParsed_, buf_.data() + nRead_}, visitor);

		bool eof = (nRead_ - nParsed_ == parse.first);
		if (eof && nRead_ == buf_.size()) {
			// Reached the end of input, but the file contains more bytes.
			if (nRead_ <= 128 * 1024 * 1024) {
				// Double the buffer size and retry.
				buf_.resize(nRead_ * 2);
				nRead_ += file_.sgetn(buf_.data() + nRead_, nRead_);
				return parseNext(game);
			}
			// Abort
			nRead_ = nParsed_ = 0;
			parseLog_.log.append("PGN parsing aborted.\n");
			return ERROR_NotFound;
		}

		nParsed_ += parse.first;
		parseLog_.logGame(parse.first, visitor);
		if (eof && !parse.second && *game.GetMoveComment() == '\0')
			return ERROR_NotFound;

		return OK;
	}

	/**
	 * Returns info about the parsing progress.
	 * @returns a pair<size_t, size_t> where first element is the quantity of
	 * data parsed and second one is the total amount of data of the database.
	 */
	std::pair<size_t, size_t> parseProgress() {
		return std::make_pair(parseLog_.n_bytes / 1024, fileSize_ / 1024);
	}

	/**
	 * Returns the list of errors produced by parseNext() calls.
	 */
	const char* parseErrors() { return parseLog_.log.c_str(); }

	/**
	 * Add a game into the database.
	 * The @e game is encoded in pgn format and appended at the end of @e file_.
	 * @param game: valid pointer to a Game object with the new data.
	 * @returns OK in case of success, an @e errorT code otherwise.
	 */
	errorT gameAdd(Game* game) {
		// buf_.clear();
		// auto moves_begin = encode(*game, buf_);
		// Split the range (moves_begin, buf_.size()) into lines
		// auto sz = static_cast<std::streamsize>(buf_.size());

		auto old_language = language;
		language = 0;
		game->SetPgnFormat(PGN_FORMAT_Plain);
		game->ResetPgnStyle(PGN_STYLE_TAGS | PGN_STYLE_VARS |
		                    PGN_STYLE_COMMENTS | PGN_STYLE_SCIDFLAGS);
		std::pair<const char*, unsigned> pgn = game->WriteToPGN(75, true);
		language = old_language;

		file_.pubseekpos(fileSize_);
		if (file_.sputn(pgn.first, pgn.second) == pgn.second) {
			fileSize_ += pgn.second;
			return OK;
		}
		return ERROR_FileWrite;
	}

	/**
	 * Encode a game into PGN format.
	 * @param game: the Game object to encode.
	 * @param dest: the container where the PGN Game will be appended.
	 * @returns the size of the tag pairs section.
	 */
	template <typename TCont> static size_t encode(Game& game, TCont& dest) {
		size_t tags_size = encodeTags(game, dest);
		dest.push_back('\n');

		game.MoveToStart();
		do {
			// TODO: comment, variations, etc..
			const char* next_move = game.GetNextSAN();
			dest.insert(dest.end(), next_move,
			            next_move + std::strlen(next_move));
			dest.push_back(' ');
		} while (game.MoveForwardInPGN() == OK);

		return tags_size;
	}

	template <typename TCont>
	static size_t encodeTags(Game& game, TCont& dest) {
		auto format_tag = [&dest](const char* tag, const char* value) {
			dest.push_back('[');
			dest.insert(dest.end(), tag, tag + std::strlen(tag));
			dest.push_back(' ');

			dest.push_back('"');
			auto value_begin = dest.size();
			dest.insert(dest.end(), value, value + std::strlen(value));
			pgn::escape_string(dest, value_begin);
			dest.push_back('"');

			dest.push_back(']');
			dest.push_back('\n');
		};
		auto format_tag_question_mark = [&format_tag](const char* tag,
		                                              const char* value) {
			format_tag(tag, (*value) ? value : "?");
		};

		size_t tags_size = dest.size();
		gamevisit::tags_STR(game, format_tag_question_mark);
		gamevisit::tags_extra(game, format_tag);
		return dest.size() - tags_size;
	}
};

#endif
