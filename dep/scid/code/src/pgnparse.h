/*
 * Copyright (C) 2018  Fulvio Benini
 *
 * This file is part of SCID (Shane's Chess Information Database).
 *
 * SCID is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * SCID is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SCID. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** @file
 * Implements a parser that converts PGN text into SCID's Game objects.
 */

#ifndef SCID_PGNPARSE_H
#define SCID_PGNPARSE_H

#include "game.h"
#include "pgn_lexer.h"
#include <string>
#include <vector>

/**
 * This class implements a PGN "visitor" that invokes the appropriate member
 * functions of the associated Game object for each type of PGN token.
 * Errors are stored as <line number, error message> pairs; if the maximum
 * number of errors is reached tokens are ignored until the end of the Game.
 */
class PgnVisitor {
	Game& game;
	std::vector<std::pair<size_t, std::string>> errors_;
	size_t linenum_ = 0;
	int nErrorsAllowed_ = 2;

	using TView = std::pair<const char*, const char*>;
	friend struct PgnParseLog;

public:
	explicit PgnVisitor(Game& g) : game(g) {}

	bool visitPGN_inputEOF() {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		return logErr("Unexpected end of input (result missing ?).");
	}

	bool visitPGN_inputUnexpectedPGNHeader() {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		return logErr("Unexpected end of game: PGN header '[' seen "
		              "inside game (result missing ?).");
	}

	bool visitPGN_Comment(TView comment) {
		if (nErrorsAllowed_ < 0) { // Skip until the end of the game
			linenum_ += std::count(comment.first, comment.second, '\n');
			return true;
		}

		linenum_ += pgn::trim(comment);
		auto& str = game.accessMoveComment();
		auto prevSz = str.size();
		str.append(comment.first, comment.second);
		linenum_ += pgn::normalize(str, prevSz);
		return true;
	}

	bool visitPGN_EndOfLine() {
		++linenum_;
		return true;
	}

	bool visitPGN_EPD(TView line) {
		ASSERT(nErrorsAllowed_ >= 0);
		std::string tmp(line.first, line.second);
		if (game.SetStartFen(tmp.c_str()) != OK)
			return logErr("Failed to parse EPD record: ", line);

		int spaces = 0;
		auto opcode = std::find_if(line.first, line.second, [&](char ch) {
			return (ch == ' ') ? spaces++ == 4 : spaces == 4;
		});
		return visitPGN_Comment(std::make_pair(opcode, line.second));
	}

	bool visitPGN_Escape(TView) { return true; }

	bool visitPGN_MoveNum(TView) { return true; }

	bool visitPGN_NAG(TView token) {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		auto nag_code = game_parseNag(token);
		if (nag_code == 0 || game.AddNag(nag_code) != OK)
			return logErr("Invalid annotation symbol: ", token);

		return true;
	}

	bool visitPGN_ResultFinal(char resultCh) {
		auto result = RESULT_None;
		switch (resultCh) {
		case '0':
			result = RESULT_Black;
			break;
		case '1':
			result = RESULT_White;
			break;
		case '/':
			result = RESULT_Draw;
			break;
		default:
			ASSERT(resultCh == '*');
		}

		auto prev_result = game.GetResult();
		if (result != prev_result) {
			// Use the end-of-game result instead of the header tag result
			game.SetResult(result);
			if (prev_result != RESULT_None && nErrorsAllowed_ >= 0)
				logErr("Final result did not match the header tag.");
		}
		return false;
	}

	bool visitPGN_SANMove(TView tok) {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		simpleMoveT sm;
		auto err = game.GetCurrentPos()->ParseMove(&sm, tok.first, tok.second);
		if (err != OK) {
			if (err == ERROR_CastlingAvailability) {
				logWarning("Warning: illegal castling ", tok);
			} else {
				if (game_parseNag(tok)) // may be 'D', 'N' or a weird suffix
					return visitPGN_NAG(tok);

				return logFatalErr("Failed to parse the move: ", tok);
			}
		}
		return (game.AddMove(&sm) == OK)
		           ? true
		           : logFatalErr("Failed to add the move: ", tok);
	}

	bool visitPGN_Suffix(TView token) { return visitPGN_NAG(token); }

	bool visitPGN_TagPair(TView tag, TView value) {
		linenum_ += std::count(value.first, value.second, '\n');
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		auto tagLen = std::distance(tag.first, tag.second);
		auto valueLen = std::distance(value.first, value.second);
		if (tagLen == 0 || tagLen + valueLen > 240 ||
		    !parseTagPair(tag.first, tagLen, value)) // Failure
		{
			std::string err(tag.first, tag.second);
			err.append(" \"");
			err.append(value.first, value.second);
			err.push_back('"');
			logErr("Error parsing the tag pair: ",
			       {err.c_str(), err.c_str() + err.size()});
		}
		return true;
	}

	bool visitPGN_Unknown(TView token) {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		// Accept misspelled castling moves
		std::string tmp(token.first, token.second);
		if (tmp == "0-0" || tmp == "00") {
			tmp = "O-O";
			return visitPGN_SANMove({tmp.c_str(), tmp.c_str() + 3});
		}
		if (tmp == "0-0-0" || tmp == "000") {
			tmp = "O-O-O";
			return visitPGN_SANMove({tmp.c_str(), tmp.c_str() + 5});
		}

		return logErr("Unknown token: ", token);
	}

	bool visitPGN_VariationStart() {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		if (game.AddVariation() != OK)
			return logFatalErr("Failed to add a new variation.");

		return true;
	}

	bool visitPGN_VariationEnd() {
		if (nErrorsAllowed_ < 0) // Skip until the end of the game
			return true;

		if (game.MoveExitVariation() != OK || game.MoveForward() != OK)
			return logFatalErr("Failed to exit from variation.");

		return true;
	}

private:
	bool logWarning(const char* str1, TView str2 = {nullptr, nullptr}) {
		errors_.emplace_back(linenum_, str1);
		if (std::distance(str2.first, str2.second) > 200) {
			errors_.back().second.append(str2.first, 200);
			errors_.back().second.append("...");
		} else {
			errors_.back().second.append(str2.first, str2.second);
		}
		return true;
	}

	bool logErr(const char* str1, TView str2 = {nullptr, nullptr}) {
		--nErrorsAllowed_;
		return logWarning(str1, str2);
	}

	bool logFatalErr(const char* str1, TView str2 = {nullptr, nullptr}) {
		nErrorsAllowed_ = 0;
		return logErr(str1, str2);
	}

	bool parseTagResult(TView str) {
		auto len = std::distance(str.first, str.second);
		if (len > 0 && *str.first == '*') {
			game.SetResult(RESULT_None);
			return true;
		}
		if (len >= 3) {
			if (std::equal(str.first, str.first + 3, "1-0")) {
				game.SetResult(RESULT_White);
				return true;
			}
			if (std::equal(str.first, str.first + 3, "0-1")) {
				game.SetResult(RESULT_Black);
				return true;
			}
			if (std::equal(str.first, str.first + 3, "1/2")) {
				game.SetResult(RESULT_Draw);
				return true;
			}
		}
		return logErr("Invalid Result tag: ", str);
	}

	bool parseTagPair(const char* tag, size_t tagLen, TView value) {
		switch (tagLen) {
		case 3:
			if (std::equal(tag, tag + 3, "ECO")) {
				std::string tmp{value.first, value.second};
				game.SetEco(eco_FromString(tmp.c_str()));
				return true;
			}
			if (std::equal(tag, tag + 3, "FEN")) {
				std::string tmp{value.first, value.second};
				return game.SetStartFen(tmp.c_str()) == OK;
			}
			break;
		case 4:
			if (std::equal(tag, tag + 4, "Date")) {
				game.SetDate(date_parsePGNTag(value));
				return true;
			}
			break;
		case 6:
			if (std::equal(tag, tag + 6, "Result"))
				return parseTagResult(value);
			break;
		case 7:
			if (std::equal(tag, tag + 7, "UTCDate") &&
			    game.GetDate() == ZERO_DATE) {
				// Add two tags: "UTCDate" and the standard "Date".
				game.SetDate(date_parsePGNTag(value));
			}
			break;
		case 9:
			if (std::equal(tag, tag + 9, "EventDate")) {
				game.SetEventDate(date_parsePGNTag(value));
				return true;
			}
			if (std::equal(tag, tag + 9, "ScidFlags")) {
				game.SetScidFlags(value.first,
				                  std::distance(value.first, value.second));
				return true;
			}
			break;
		}
		if (tagLen >= 8) {
			// Look for Rating Types: only the first Rating type found for
			// each player is added as the rating. Any extra ratings are
			// just added as normal tags.
			if (std::equal(tag, tag + 5, "White") && game.GetWhiteElo() == 0) {
				auto res = game.setRating(WHITE, tag + 5, tagLen - 5, value);
				if (res >= 0)
					return res;
			} else if (std::equal(tag, tag + 5, "Black") &&
			           game.GetBlackElo() == 0) {
				auto res = game.setRating(BLACK, tag + 5, tagLen - 5, value);
				if (res >= 0)
					return res;
			}
		}
		auto& str = game.accessTagValue(tag, tagLen);
		str.assign(value.first, value.second);
		linenum_ += pgn::normalize<true>(str, 0);
		return true;
	}
};

/**
 * Format and store errors.
 */
struct PgnParseLog {
	std::string log;
	unsigned long long n_bytes = 0;
	unsigned long long n_lines = 0;
	unsigned long long n_games = 0;

	/**
	 * Format and store errors occurred while parsing a Game.
	 * It also updates the byte, line, and game counters.
	 * @returns false if part of the game was ignored, true otherwise.
	 */
	bool logGame(size_t nBytes, const PgnVisitor& visitor) {
		++n_games;
		for (auto& e : visitor.errors_) {
			log += "(game " + std::to_string(n_games);
			log += ", line " + std::to_string(n_lines + e.first) + ") ";
			log += e.second;
			log += "\n";
		}
		n_lines += visitor.linenum_;
		n_bytes += nBytes;
		if (visitor.nErrorsAllowed_ < 0) {
			log += "(game " + std::to_string(n_games);
			log += ", line " + std::to_string(n_lines) + ") ";
			log += "End of game, ignored the part after the last error.\n";
			return false;
		}
		return true;
	}
};

/**
 * Convert PGN text into a SCID's Game object.
 * @param input:    the memory containing the PGN text.
 * @param inputLen: the number of chars in @e input.
 * @param game:     the Game object where the game will be stored.
 *                  The object is not automatically cleared so that moves can
 *                  be added to an already existing one.
 * @param log:      stores eventual parsing error.
 * @returns true if a game was parsed successfully (maybe with errors, but
 * without ignoring any part), false otherwise.
 */
inline bool pgnParseGame(const char* input, size_t inputLen, Game& game,
                         PgnParseLog& log) {
	struct VisitorNoEOF : public PgnVisitor {
		explicit VisitorNoEOF(Game& g) : PgnVisitor(g) {}
		bool visitPGN_inputEOF() { return true; }
	} visitor(game);

	auto parse = pgn::parse_game({input, input + inputLen}, visitor);
	if (!log.logGame(parse.first, visitor))
		return false;

	if (parse.first == inputLen && !parse.second &&
	    *game.GetMoveComment() == '\0')
		return false;

	return true;
}

#endif // idndef SCID_PGNPARSE_H
