/*
 * Copyright (C) 2019  Fulvio Benini.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file
 * Defines a ByteBuffer class which will act like a std::string_view.
 * The caller must ensure that a ByteBuffer object does not outlive the
 * pointed-to data.
 */

#pragma once

#include "board_def.h"
#include "error.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <string_view>

/**
 * The Tag section is a list of <tag,value> variable length records:
 * tag_length: 1-byte
 * tag name: [0:240] bytes
 * value_length: 1-byte
 * value: [0:255] bytes
 * The section is terminated with 1-byte with value 0, meaning that un empty
 * tag name cannot be stored.
 * Some common tags are encoded in one byte, using a tag_length value over 240
 * and a tag name of 0 bytes.
 */
constexpr size_t MAX_TAG_LEN = 240;
constexpr std::string_view commonTags[] = {
    // 241, 242: Country
    "WhiteCountry", "BlackCountry",
    // 243: Annotator
    "Annotator",
    // 244: PlyCount
    "PlyCount",
    // 245: EventDate (plain text encoding)
    "EventDate",
    // 246, 247: Opening, Variation
    "Opening", "Variation",
    // 248-250: Setup and Source
    "Setup", "Source", "SetUp"
    // 252-254: spare for future use
    // 255: Reserved for compact EventDate encoding
};

template <typename SourceT, typename DestT>
void encodeTags(const SourceT& tagList, DestT& dest) {
	for (auto& tag : tagList) {
		if (tag.first.length() == 0)
			continue; // Cannot store empty tag names

		const auto it = std::find(commonTags, std::end(commonTags), tag.first);
		if (it != std::end(commonTags)) {
			// Common tags are stored with just their 1-byte value [241:250]
			const auto tagnum = std::distance(commonTags, it) + MAX_TAG_LEN + 1;
			dest.emplace_back(static_cast<unsigned char>(tagnum));
		} else {
			// Other tags are stored as 1-byte length [1:240] + the tag name.
			const auto tag_name = tag.first.data();
			const auto length = std::min(tag.first.length(), MAX_TAG_LEN);
			dest.emplace_back(static_cast<unsigned char>(length));
			dest.insert(dest.end(), tag_name, tag_name + length);
		}

		// The value is stored as 1-byte length [0:255] + the data.
		const auto value = tag.second.data();
		const auto valueLen = std::min<size_t>(tag.second.length(), 255);
		dest.emplace_back(static_cast<unsigned char>(valueLen));
		dest.insert(dest.end(), value, value + valueLen);
	}
	dest.emplace_back(0);
}

/**
 * The StartBoard section starts with a byte containing three flags:
 * 0-bit: true if the game doesn't start with the standard board
 * 1-bit: true if there are pawn to queen promotions
 * 2-bit: true if there are pawn to non-queen promotions.
 * If the 0-bit is true, the byte is followed by a null terminated string
 * containing the FEN of the start position.
 */
template <typename DestT>
void encodeStartBoard(bool promoFlag, bool underpromoFlag, const char* FEN,
                      DestT& dest) {
	char flags = 0;
	if (FEN) {
		flags += 1;
	}
	if (promoFlag) {
		flags += 2;
	}
	if (underpromoFlag) {
		flags += 4;
	}
	dest.emplace_back(flags);
	if (FEN) {
		const auto len = std::strlen(FEN) + 1; // Include the null char
		dest.insert(dest.end(), FEN, FEN + len);
	}
}

class ByteBuffer {
	const unsigned char* data_;
	const unsigned char* const end_;

public:
	ByteBuffer(const unsigned char* data, size_t length)
	    : data_(data), end_(data + length) {
		assert(data_ || data_ == end_);
	}

	explicit operator bool() const { return data_ != end_; }

	/// Decodes the tag pairs not stored into the index.
	/// @param fn: a function that should accept 2 parameters
	///            (string_view tag_name, string_view tag_value)
	///            and that will be called for each tag pair.
	template <typename FuncT> errorT decodeTags(FuncT fn) {
		if (data_ == end_)
			return ERROR_BufferRead;

		auto it = data_;
		for (;;) {
			const auto tagLen = *it++;
			if (tagLen == 0) {
				data_ = it;
				return OK; // Reached the end of the tags section
			}

			const char* tag = nullptr;
			size_t tagID = 0;
			if (tagLen > MAX_TAG_LEN) {
				// A common tag name, not explicitly stored
				tagID = tagLen - MAX_TAG_LEN - 1;
			} else {
				tag = reinterpret_cast<const char*>(it);
				it += tagLen;
			}

			if (it >= end_)
				return ERROR_Decode;

			// 255 was a special 3-bytes encoding of EventDate used in SCID2
			const auto valueLen = (tagLen != 255) ? *it++ : 3;
			const char* value = reinterpret_cast<const char*>(it);
			it += valueLen;
			if (it >= end_)
				return ERROR_Decode;

			if (tag) {
				fn(std::string_view(tag, tagLen),
				   std::string_view(value, valueLen));
			} else if (tagID < 10) {
				fn(commonTags[tagID], std::string_view(value, valueLen));
			}
		}
	}

	/// Decodes the start position.
	/// @returns OK on success and the FEN of the start position (nullptr for
	///          the standard starting position).
	/// To decode the moves the correct index should be assigned to each piece:
	/// they are assigned from left to right, but the king should always have
	/// index 0. It is therefore swapped with the piece occupying the index 0.
	/// For example with "rnb1k2Q/1p5p/p7/4p3/4q3/8/PPP2R1P/2K5 b" the black
	/// rook on A8 gets index 3, the black night on B8 gets index 1 .... and the
	/// white queen on H8 gets index 6, the pawn on A2 gets index 1 ...
	std::pair<errorT, const char*> decodeStartBoard() {
		if (data_ == end_)
			return {ERROR_Decode, nullptr};

		const auto flags = *data_++;
		if ((flags & 1) == 0)
			return {OK, nullptr};

		if (const auto FEN = GetTerminatedString())
			return {OK, FEN};

		return {ERROR_Decode, nullptr};
	}

	/// Reads a null-terminated string from the buffer.
	const char* GetTerminatedString() {
		const char* res = reinterpret_cast<const char*>(data_);
		data_ = std::find(data_, end_, 0);
		if (data_ == end_)
			return nullptr;

		++data_; // skip the null char
		return res;
	}

	/// Extract the next move.
	/// @returns a std::pair containing OK and the move value.
	///          Returns ERROR_EndOfMoveList when the end of the game is
	///          reached, an error otherwise.
	template <typename MoveFn, typename CommentFn, typename VariationFn,
	          typename NagFn>
	std::pair<errorT, unsigned char>
	nextMove(int varDepth, MoveFn acceptMove, CommentFn commentMarker,
	         VariationFn changeVar, NagFn addNag) {
		// The king has always index 0 and only 11 possible moves (8 destination
		// squares, 2 castle moves and the null move). The unused codes 11-15
		// represent special markers for nags, comments and variations.
		// Note: 2-bytes queen moves use the second byte as (64 + destination
		// square) to avoid interferences with these markers. However the byte
		// immediatly after the ENCODE_NAG can have any possible value.
		enum {
			ENCODE_NAG = 11,
			ENCODE_COMMENT,
			ENCODE_START_MARKER,
			ENCODE_END_MARKER,
			ENCODE_END_GAME
		};

		auto it = data_;
		for (; it != end_; ++it) {
			switch (*it) {
			case ENCODE_NAG:
				if (++it == end_) {
					return {ERROR_Decode, 0}; // ERROR: missing nag
				}
				if (!addNag(*it)) {
					return {ERROR_Decode, 0}; // ERROR: marker decoding
				}
				continue;

			case ENCODE_COMMENT:
				commentMarker();
				continue;

			case ENCODE_START_MARKER:
				++varDepth;
				if (!changeVar(true)) {
					return {ERROR_Decode, 0}; // ERROR: variation
				}
				continue;

			case ENCODE_END_MARKER:
				if (varDepth == 0) {
					return {ERROR_Decode, 0}; // ERROR: end marker in main line
				}
				--varDepth;
				if (!changeVar(false)) {
					return {ERROR_Decode, 0}; // ERROR: variation
				}
				continue;

			case ENCODE_END_GAME:
				if (varDepth != 0) {
					return {ERROR_Decode, 0}; // ERROR: unexpected end of game
				}
				data_ = ++it;
				return {ERROR_EndOfMoveList, 0}; // SUCCESS: end of game

			default:
				if (acceptMove(varDepth)) {
					data_ = it;
					return {OK, *data_++}; // SUCCESS
				}
			}
		}
		return {ERROR_Decode, 0}; // ERROR: missing ENCODE_END_GAME
	}

	/// Find the next move in the current line.
	/// Ignore variations, comments and nags.
	std::pair<errorT, unsigned char> nextLineMove() {
		return nextMove(
		    0, [](auto varDepth) { return varDepth == 0; },
		    [] {},                     // Ignore comments
		    [](auto) { return true; }, // Ignore variations
		    [](auto) { return true; }  // Ignore nags
		);
	}

	/// Decode a move encoded in SCID4 format.
	/// Excluding queens, the other chess pieces cannot reach more than 16
	/// target squares from any given position. This allow to store the target
	/// square of a move into 4 bits, as an index of all the reachable squares.
	/// @param movingPiece: the type (PAWN, BISHOP, etc.) of the piece to move.
	/// @param from: the square where is the piece to move.
	/// @param moveCode: the SCID4 encoding of the move (a 0-15 value).
	/// @returns a pair containing the destination square and the new type of
	///          the piece for promotions (INVALID_PIECE for normal moves).
	///          Special moves are returned as:
	///          - castle kingside: {from, KING}
	///          - castle queenside: {from, QUEEN}
	///          - null move: {from, PAWN}
	/// On error returns an invalid square (<0 or >63) or {from, INVALID_PIECE}.
	template <colorT toMove>
	std::pair<int, pieceT> decodeMove(pieceT movingPiece, squareT from,
	                                  unsigned char moveCode) {
		moveCode &= 0x0F;
		switch (movingPiece) {
		case PAWN: {
			static const pieceT promoPiece[] = {
			    INVALID_PIECE, INVALID_PIECE, INVALID_PIECE, QUEEN,
			    QUEEN,         QUEEN,         ROOK,          ROOK,
			    ROOK,          BISHOP,        BISHOP,        BISHOP,
			    KNIGHT,        KNIGHT,        KNIGHT,        INVALID_PIECE};
			static const int8_t sqdiff[] = {7, 8, 9, 7, 8, 9, 7, 8,
			                                9, 7, 8, 9, 7, 8, 9, 16};
			int to = (toMove == WHITE) ? from + sqdiff[moveCode]
			                           : from - sqdiff[moveCode];
			return {to, promoPiece[moveCode]};
		}
		case BISHOP: {
			int fylediff = square_Fyle(moveCode) - square_Fyle(from);
			int to = (moveCode >= 8) ? from - 7 * fylediff
			                         : from + 9 * fylediff;
			return {to, INVALID_PIECE};
		}
		case KNIGHT: {
			static const int8_t sqdiff[] = {0,  -17, -15, -10, -6, 6, 10, 15,
			                                17, 0,   0,   0,   0,  0, 0,  0};
			return {from + sqdiff[moveCode], INVALID_PIECE};
		}
		case QUEEN:
			if (moveCode == square_Fyle(from)) { // 2 BYTES MOVE
				int to = (data_ != end_) ? *data_++ : 0;
				return {to - 64, INVALID_PIECE};
			}
			/* FALLTHRU */
		case ROOK: {
			int to = (moveCode >= 8) // a vertical move
			             ? square_Make(square_Fyle(from), (moveCode - 8))
			             : square_Make(moveCode, square_Rank(from));
			return {to, INVALID_PIECE};
		}
		case KING:
			if (moveCode == 0) // NULL MOVE
				return {from, PAWN};

			if (moveCode <= 8) {
				static const int8_t sqdiff[] = {0, -9, -8, -7, -1, 1, 7, 8, 9};
				return {from + sqdiff[moveCode], INVALID_PIECE};
			}

			if (moveCode == 9) // CASTLE QUEENSIDE
				return {from, QUEEN};

			if (moveCode == 10) // CASTLE KINGSIDE
				return {from, KING};
		}

		return {from, INVALID_PIECE}; // decode error
	}
};
