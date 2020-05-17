/*
 * Copyright (C) 2014-2017  Fulvio Benini

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
 */

/** @file
 *  Extends the std:filebuf class with performance improvements.
 */

#ifndef FILEBUF_H
#define FILEBUF_H

#include "common.h"
#include <climits>
#include <fstream>

/**
 * Adds some helper functions to std::filebuf:
 * - getline()
 * - read and write of unsigned integers with size of 32/24/16/8 bit.
 */
class Filebuf : public std::filebuf {
public:
	/**
	 * Opens a file.
	 * @param filename: path to the file to be opened.
	 * @param fmode: open the file for reading, writing, or both.
	 * @returns OK on success, an @e errorT code on failure.
	 */
	errorT Open(const char* filename, fileModeT fmode) {
		std::ios::openmode mode = std::ios::binary;
		switch (fmode) {
		case FMODE_ReadOnly:
			mode |= std::ios::in;
			break;
		case FMODE_WriteOnly:
			mode |= std::ios::out;
			break;
		case FMODE_Both:
			mode |= std::ios::in | std::ios::out;
			break;
		case FMODE_Create:
			mode |= std::ios::in | std::ios::out | std::ios::trunc;
			break;
		default:
			return ERROR_FileMode;
		}

		return (open(filename, mode) != 0) ? OK : ERROR_FileOpen;
	}

	/**
	 * Equivalent to std::fstream::getline, but faster (no sentry [27.7.2.1.3]).
	 *
	 * Extracts characters from @e *this and stores them in successive locations
	 * of the array whose first element is pointed to by @p str, until  the end
	 * of line ('\\n' char) or the end of file condition occurs.
	 * The '\\n' char is not stored into @p str, and a null character, even in
	 * case of  errors, is appended.
	 *
	 * Typical usage: @code
	 * while (file.getline(buf, sizeof buf)) {
	 * // use buf
	 * }
	 * if (file.sgetc() != EOF) // error
	 * @endcode
	 * @param str: pointer to the character string to store the characters to
	 * @param count: size of character string pointed to by @p str
	 * @returns
	 * - the number of characters read, including the '\\n' char
	 * - 0 on failure or if the buffer is too small.
	 */
	size_t getline(char* str, size_t count) {
		ASSERT(str != 0);
		ASSERT(count != 0);

		size_t n = 0;
		for (int ch = sgetc(); ch != EOF; ch = snextc()) {
			++n;
			if (ch == '\n') {
				sbumpc();
				break;
			}
			if (n >= count) {
				n = 0; // Fail: buffer too small
				break;
			}
			*str++ = static_cast<char>(ch);
		}
		*str = 0;
		return n;
	}

	/**
	 * Reads a 8-bit unsigned integer.
	 * This function do not check for errors or EOF.
	 */
	byte ReadOneByte() { return static_cast<byte>(sbumpc()); }

	/**
	 * Reads a 16-bit unsigned integer.
	 * This function do not check for errors or EOF.
	 */
	uint16_t ReadTwoBytes() { return static_cast<uint16_t>(read<2>()); }

	/**
	 * Reads a 24-bit unsigned integer.
	 * This function do not check for errors or EOF.
	 */
	uint32_t ReadThreeBytes() { return read<3>(); }

	/**
	 * Reads a 32-bit unsigned integer.
	 * This function do not check for errors or EOF.
	 */
	uint32_t ReadFourBytes() { return read<4>(); }

	/**
	 * Writes a 8-bit unsigned integer.
	 * @returns the number of characters successfully written.
	 */
	int WriteOneByte(byte value) {
		int_type ch = sputc(static_cast<char_type>(value));
		return (ch != traits_type::eof()) ? 1 : 0;
	}

	/**
	 * Writes a 16-bit unsigned integer.
	 * @returns the number of characters successfully written.
	 */
	int WriteTwoBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}
	/**
	 * Writes a 24-bit unsigned integer.
	 * @returns the number of characters successfully written.
	 */
	int WriteThreeBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 16)) +
		       WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}
	/**
	 * Writes a 32-bit unsigned integer.
	 * @returns the number of characters successfully written.
	 */
	int WriteFourBytes(uint32_t value) {
		return WriteOneByte(static_cast<byte>(value >> 24)) +
		       WriteOneByte(static_cast<byte>(value >> 16)) +
		       WriteOneByte(static_cast<byte>(value >> 8)) +
		       WriteOneByte(static_cast<byte>(value));
	}

private:
	template <int nBytes> uint32_t read() {
		uint32_t res = 0;
		if (nBytes > 3)
			res += static_cast<uint32_t>(ReadOneByte()) << 24;
		if (nBytes > 2)
			res += static_cast<uint32_t>(ReadOneByte()) << 16;
		if (nBytes > 1)
			res += static_cast<uint32_t>(ReadOneByte()) << 8;
		return res + ReadOneByte();
	}
};

/**
 * Optimizes std::filebuf for random-access reading.
 */
class FilebufAppend : protected Filebuf {
	std::streamoff fileSz_;
	std::streamoff filePos_;

public:
	FilebufAppend() : fileSz_(0), filePos_(-1) {}

	/**
	 * Opens a file and store its size.
	 * @param filename: path to the file to be opened.
	 * @param fmode: open the file for reading, writing, or both.
	 * @returns OK on success, an @e errorT code on failure.
	 */
	errorT open(const std::string& filename, fileModeT fmode) {
		errorT res = Open(filename.c_str(), fmode);
		if (res != OK)
			return res;
		fileSz_ = pubseekoff(0, std::ios::end);
		if (fileSz_ == -1)
			return ERROR_FileSeek;
		return OK;
	}

	/**
	 * Returns the size of the file.
	 */
	unsigned long long size() const { return fileSz_; }

	/**
	 * Invokes std::filebuf::sync() to write all pending output to the file.
	 * @returns 0 in case of success, -1 in case of failure.
	 */
	int pubsync() { return sync(); }

	/**
	 * Writes, at the end of the file, @p count characters from the character
	 * array whose first element is pointed to by @p s.
	 * @returns OK in case of success, an error code otherwise.
	 */
	errorT append(const char_type* s, std::streamsize count) {
		assert(s != 0);

		if (filePos_ != -2) { // Seek to end of file, if necessary.
			filePos_ = seekpos(fileSz_);
			if (filePos_ == -1)
				return ERROR_FileSeek;
			filePos_ = -2;
		}

		std::streamsize n = xsputn(s, count);
		fileSz_ += n;
		return (n == count) ? OK : ERROR_FileWrite;
	}

	/**
	 * Invoke filebuf::xsgetn() and update @e filePos_.
	 */
	std::streamsize sgetn(char_type* s, std::streamsize count) {
		std::streamsize res = xsgetn(s, count);
		filePos_ += res;
		return res;
	}

	/**
	 * Repositions the internal buffer or invoke filebuf::seekpos().
	 * In the standard implementation, the buffer must be abandoned to ensure
	 * consistency when transitioning from reading to writing.
	 */
	std::streamoff pubseekpos(std::streamoff pos) {
		if (filePos_ < 0 || pos < filePos_)
			return filePos_ = seekpos(pos);

		if (filePos_ != pos) {
			const std::streamsize avail = egptr() - gptr();
			if (avail >= pos - filePos_) {
				ASSERT(pos - filePos_ <= INT_MAX);
				gbump(static_cast<int>(pos - filePos_));
				filePos_ = pos;
			} else {
				filePos_ = seekpos(pos);
			}
		}
		return filePos_;
	}
};

#endif // FILEBUF_H
