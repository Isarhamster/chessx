/*
 * Copyright (C) 2018  Fulvio Benini.
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
 * Split input into PGN tokens and dispatch them to a "visiting" parser.
 */

#ifndef _PGN_LEXER_H
#define _PGN_LEXER_H

#include <algorithm>
#include <cassert>

namespace pgn_impl {
// "PGN character data is organized as tokens. A token is a contiguous
// sequence of characters that represents a basic semantic unit. Tokens
// may be separated from adjacent tokens by white space characters.
// (White space characters include space, newline, and tab characters.)
// Some tokens are self delimiting and do not require white space
// characters."

/**
 * Creates a 128 bits bitmap of PGN symbol characters.
 *
 * "A symbol token starts with a letter or digit character and is immediately
 * followed by a sequence of zero or more symbol continuation characters.
 * These continuation characters are letter characters ("A-Za-z"), digit
 * characters ("0-9"), the underscore ("_"), the plus sign ("+"), the octothorpe
 * sign ("#"), the equal sign ("="), the colon (":"),  and the hyphen ("-")."
 * @param elem: 0 for the lower 64 bits, 1 for the higher 64 bits.
 * @returns the requested half of the bitmap.
 */
constexpr unsigned long long init_symbol_map(unsigned elem) {
	return (elem == 0) ? 0x27ffb80800000000 : 0x47fffffe87ffffff;

	/* Requires gcc >= 6.2 or clang >= 3.5

	   unsigned long long res[2] = {0};
	   for (unsigned ch = 'A'; ch <= 'Z'; ++ch) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   for (unsigned ch = 'a'; ch <= 'z'; ++ch) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   for (unsigned ch = '0'; ch <= '9'; ++ch) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   const unsigned extra[] = {'_', '+', '#', '=', ':', '-'};
	   for (unsigned ch : extra) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   const unsigned drawresult_unclear[] = {'/', '~'};
	   for (unsigned ch : drawresult_unclear) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   const unsigned chess_variants[] = {',', '@'};
	   for (unsigned ch : chess_variants) {
	       res[ch / 64] |= (1ULL << (ch % 64));
	   }
	   return res[elem];
   */
}

/**
 * Checks if the given character is a PGN symbol.
 * @param ch: character to classify.
 * @returns true if @e ch is a PGN symbol character, false otherwise.
 */
inline bool is_PGNsymbol(unsigned char ch) {
	constexpr unsigned long long tok_map[] = {init_symbol_map(0),
	                                          init_symbol_map(1)};
	auto high = ch / 64;
	auto low = ch % 64;
	return high > 1 ? false : tok_map[high] & (1ULL << low);
}

/**
 * Checks if the given character is one of the 10 decimal digits: 0123456789.
 * @param ch: character to classify.
 * @returns true if the character is a numeric character, false otherwise.
 */
inline bool is_PGNdigit(unsigned char ch) { return ch >= '0' && ch <= '9'; }

/**
 * Checks if the given character is a white space ("white space characters
 * include space, newline, and tab characters").
 * @param ch: character to classify.
 * @returns true if the character is a white space, false otherwise.
 */
inline bool is_PGNwhitespace(unsigned char ch) {
	return (ch == ' ' || ch == '\r' || ch == '\t' || ch == '\v' || ch == '\n');
}

/**
 * Checks if a token is the game termination marker.
 *
 * "The game termination marker is a symbol that is one of the following four
 * values: "1-0" (White wins), "0-1" (Black wins), "1/2-1/2" (drawn game),
 * and "*" (game in progress, result unknown, or game abandoned)."
 * @param tok: the token to classify.
 * @returns
 * - '1' for "White wins",
 * - '0' for "Black wins",
 * - '/' for "drawn game",
 * - '*' for "result unknown",
 * - 0 (false) if it's not a termination marker.
 */
template <typename TView> char is_PGNtermination(TView tok) {
	auto n_chars = std::distance(tok.first, tok.second);
	if (n_chars == 3) {
		if (std::equal(tok.first, tok.first + 3, "1-0"))
			return '1';
		if (std::equal(tok.first, tok.first + 3, "0-1"))
			return '0';
		if (std::equal(tok.first, tok.first + 3, "1/2"))
			return '/';
		if (std::equal(tok.first, tok.first + 3, "1:0"))
			return '1';
		if (std::equal(tok.first, tok.first + 3, "0:1"))
			return '0';
	} else if (n_chars == 7) {
		if (std::equal(tok.first, tok.first + 7, "1/2-1/2") ||
		    std::equal(tok.first, tok.first + 7, "1/2:1/2"))
			return '/';
	}
	return 0;
}

/**
 * Read a token and dispatch it to a PGN parser.
 * The first char of the token is used to determine its termination.
 * @param ch:      the first char of the token.
 * @param input:   the input to get data from.
 * @param parser:  will receive the tokens via visitPGN_* functions.
 * @param section: -1 pregame, 0 for tag pair section, 1 for movetext section.
 * @returns the result of the invoked parser's function.
 */
template <typename TInput, typename TVisitor>
bool parse_token(char ch, TInput& input, TVisitor& parser, int& section) {
	switch (ch) {
	case ' ':  // self terminating
	case '\t': // self terminating
	case '\v': // self terminating
	case '\r': // self terminating
		return true;

	case '\n': // self terminating
		return parser.visitPGN_EndOfLine();

	case '.': // self terminating
		return true;

	case '<': // self terminating
		return true;

	case '>': // self terminating
		return true;

	case '*': // self terminating
		return parser.visitPGN_ResultFinal('*');

	case '(': // self terminating
		return parser.visitPGN_VariationStart();

	case ')': // self terminating
		return parser.visitPGN_VariationEnd();

	case '[': // --> ']', can span multiple lines
		if (section <= 0) {
			section = 0;
			auto skip_spaces = [&]() {
				auto spaces = input.read_while(is_PGNwhitespace);
				while (spaces.first != spaces.second) {
					if (*spaces.first++ == '\n')
						parser.visitPGN_EndOfLine();
				}
			};
			skip_spaces();
			auto tag = input.read_while(is_PGNsymbol);

			skip_spaces();
			auto value = input.read_until(']');

			// Remove the " char at the start and deal with the special case of
			// a ] char inside the string token.
			if (value.first != value.second && *value.first == '"') {
				auto is_terminated = [&]() {
					for (auto it = value.first; it != value.second; ++it) {
						if (*it == '"')
							return true;
						if (*it == '\\' && ++it == value.second)
							break;
					}
					return false;
				};
				++value.first;
				while (!input.last_column() && !is_terminated()) {
					value.second = input.read_until(']').second;
				}
			}
			// trim right
			while (value.first != value.second) {
				auto last_ch = *--value.second;
				if (last_ch == '"') {
					break;
				}
				if (!is_PGNwhitespace(last_ch)) {
					++value.second;
					break;
				}
				if (last_ch == '\n')
					parser.visitPGN_EndOfLine();
			}
			return parser.visitPGN_TagPair(tag, value);
		}
		input.sungetc();
		parser.visitPGN_inputUnexpectedPGNHeader();
		return false;

	case '{': // --> '}', can span multiple lines
		return parser.visitPGN_Comment(input.read_until('}'));

	case ';': // --> '\n'
		return parser.visitPGN_Comment(input.read_line());

	case '%': // --> '\n', only if "appearing in the first column of a line"
		if (input.first_column()) {
			return parser.visitPGN_Escape(input.read_line());
		}
		return parser.visitPGN_Unknown(
		    input.read_token([](char c) { return c == '%'; }));

	case '$': // terminated just prior to the first non-digit character
		return parser.visitPGN_NAG(input.read_token(is_PGNdigit));

	case '?': // Suffix annotations: "!", "?", "!!", "!?", "?!", and "??"
	case '!': // "At most one such suffix annotation may appear per move"
		return parser.visitPGN_Suffix(
		    input.read_token([](char c) { return c == '!' || c == '?'; }));
	}

	// "A symbol token is terminated just prior to the first non-symbol
	// character following the symbol character sequence."
	auto tok = input.read_token(is_PGNsymbol);
	bool epd = (section < 0 && std::count(tok.first, tok.second, '/') == 7);
	section = 1;

	if (epd) {
		tok.second = input.read_line().second;
		parser.visitPGN_EPD(tok);
		return false;
	}

	auto notdigit = std::find_if_not(tok.first, tok.second, is_PGNdigit);
	if (notdigit == tok.first)
		return parser.visitPGN_SANMove(tok);

	if (notdigit == tok.second)
		return parser.visitPGN_MoveNum(tok);

	if (auto result = is_PGNtermination(tok))
		return parser.visitPGN_ResultFinal(result);

	return parser.visitPGN_Unknown(tok);
}

class InputMemory {
	const char* const begin_;
	const char* const end_;
	const char* it_;

public:
	InputMemory(const char* begin, const char* end)
	    : begin_(begin), end_(end), it_(begin) {}

	/// Reads one character and advances the input sequence by one character.
	char sbumpc() {
		assert(it_ != end_);
		return *it_++;
	};

	/// Makes the most recently extracted character available again.
	void sungetc() {
		assert(it_ != begin_ && it_ != end_);
		--it_;
	}

	/// Returns true if there are no chars available.
	bool eof() const { return it_ == end_; };

	/// Returns the number of chars read.
	std::size_t n_read() const { return std::distance(begin_, it_); }

	/// Returns true if the most recently extracted character was the first
	/// character of the line.
	bool first_column() const { return (n_read() < 2 || *(it_ - 2) == '\n'); };

	/// Returns true if the most recently extracted character was the last
	/// character of the line.
	bool last_column() const { return eof() || *it_ == '\n' || *it_ == '\r'; }

	/// Returns the range of chars: [curr_char, '\n').
	/// The '\n' char is left as the next character to extract.
	std::pair<const char*, const char*> read_line() {
		auto first = it_;
		it_ = std::find(it_, end_, '\n');
		return {first, it_};
	}

	/// Returns the range of chars: [curr_char, delim).
	/// The delim char is skipped.
	std::pair<const char*, const char*> read_until(char delim) {
		auto first = it_;
		it_ = std::find(it_, end_, delim);
		auto second = (it_ == end_) ? it_ : it_++;
		return {first, second};
	}

	/// Returns the range of chars: [curr_char, cond == true].
	template <typename Cond>
	std::pair<const char*, const char*> read_while(Cond cond) {
		auto first = it_;
		it_ = std::find_if_not(it_, end_, cond);
		return {first, it_};
	}

	/// Returns the range of chars: [last_extracted_char, cond == true].
	/// cond is not applied to last_extracted_char.
	template <typename Cond>
	std::pair<const char*, const char*> read_token(Cond cond) {
		assert(it_ != begin_);
		auto first = it_ - 1;
		it_ = std::find_if_not(it_, end_, cond);
		return {first, it_};
	}
};

} // namespace pgn_impl

namespace pgn {

/**
 * Read a PGN game from memory, grouping characters in tokens and dispatching
 * them to a PGN parser.
 * @param input:  the memory range containing the PGN game.
 * @param parser: will receive the tokens via visitPGN_* functions.
 *                Parsing is aborted if it returns false.
 * @returns a std::pair containing the number of chars parsed, and true if at
 * least a tag-pair token or a symbol token was dispatched.
 */
template <typename TVisitor>
std::pair<std::size_t, bool> parse_game(pgn_impl::InputMemory input,
                                        TVisitor&& parser) {
	int section = -1;
	do {
		if (input.eof()) {
			if (section >= 0)
				parser.visitPGN_inputEOF();
			break;
		}
	} while (pgn_impl::parse_token(input.sbumpc(), input, parser, section));

	return {input.n_read(), section >= 0};
}

/**
 * Normalize white spaces and converts Latin-1 chars to UTF-8 sequences.
 *
 * The original PGN standard used a subset of ISO 8859/1 (Latin 1):
 * "Code value from 0 to 126 are the standard ASCII character set."
 * "Code value from 127 to 191 are not used for PGN data representation."
 * "Code value from 192 to 255 are mostly alphabetic printing characters with
 * various diacritical marks; their use is encouraged for those languages
 * that require such characters."
 * However this do not allow internationalization for comments and names
 * (players, sites, etc...); the common UTF-8 is a superior alternative.
 * @param unescape: if true converts \\ to \ and \" to ".
 * @param str: the string to be normalized.
 * @param pos: start of the substring of @e str that will be normalized.
 * @returns the number of '\n' chars in @e str.
 */
template <bool unescape = false, typename TString>
std::size_t normalize(TString& str, std::size_t pos) {
	std::size_t n_newlines = 0;
	for (std::size_t i = pos, n = str.size(); i < n; ++i) {
		unsigned char ch = str[i];
		// An invalid UTF-8 sequence is considered a Latin1 char and converted.
		if (ch > 0xBF) {
			unsigned char nxt = (i + 1 != n) ? str[i + 1] : 0;
			if (nxt < 0x80 || nxt > 0xBF) {
				str[i] = static_cast<unsigned char>(ch & 0xBF);
				str.insert(str.begin() + i, static_cast<unsigned char>(0xC3));
				++i;
				++n;
			}
		} else if (ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v') {
			if (ch == '\n')
				++n_newlines;

			// Tab and new line characters are removed if there is an adjacent
			// space, or converted to a normal space otherwise.
			if (i == pos ||                             // First char
			    (i + 1) == n ||                         // Last char
			    str[i - 1] == ' ' ||                    // Preceded by a space
			    pgn_impl::is_PGNwhitespace(str[i + 1])) // Followed by a space
			{
				str.erase(i, 1);
				--i;
				--n;
			} else {
				str[i] = ' ';
			}
		} else if (unescape && ch == '\\' && i + 1 != n) {
			// "A quote inside a string is represented by the backslash
			// immediately followed by a quote. A backslash inside a string is
			// represented by two adjacent backslashes."
			if (str[i + 1] == '\\' || str[i + 1] == '"') {
				str.erase(i, 1);
				--n;
			}
		}
	}
	return n_newlines;
}

/**
 * Escape quote and backslash chars according to the PGN standard:
 * "A quote inside a string is represented by the backslash immediately followed
 * by a quote. A backslash inside a string is represented by two adjacent
 * backslashes."
 * @param str: the string containing the chars to be escaped.
 * @param pos: start of the substring of @e str to be processed.
 */
template <typename TString> void escape_string(TString& str, std::size_t pos) {
	auto it = str.begin() + pos;
	while (true) {
		it = std::find_if(it, str.end(),
		                  [](char ch) { return ch == '\\' || ch == '\"'; });
		if (it != str.end())
			it = str.insert(it, '\\') + 2;
		else
			break;
	}
}

/**
 * Trim leading and trailing white spaces.
 * @param str: the string to trim.
 * @returns the number of '\n' chars in @e str.
 */
template <typename TView> std::size_t trim(TView& str) {
	std::size_t n_newlines = 0;
	auto is_space = [&n_newlines](char ch) {
		if (ch == '\n') {
			++n_newlines;
		} else if (ch != ' ' && ch != '\r' && ch != '\t' && ch != '\v') {
			return false;
		}
		return true;
	};
	str.first = std::find_if_not(str.first, str.second, is_space);

	using RevIt = std::reverse_iterator<decltype(str.first)>;
	str.second =
	    std::find_if_not(RevIt(str.second), RevIt(str.first), is_space).base();

	return n_newlines;
}

} // namespace pgn

#endif // _PGN_LEXER_H
