/*
* Copyright (C) 2015 Fulvio Benini
* Copyright (c) 2001-2003  Shane Hudson (2nd part of the file)

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

#include "spellchk.h"
#include "date.h"
#include "filebuf.h"
#include "misc.h"


namespace {

enum InfoType {
	SPELL_SECTIONSTART,
	SPELL_NEWNAME, SPELL_ALIAS, SPELL_PREFIX, SPELL_INFIX, SPELL_SUFFIX,
	SPELL_BIO, SPELL_ELO,
	SPELL_EMPTY, SPELL_OLDBIO, SPELL_UNKNOWN
};

struct Parser {
	char* name;
	char* extra;
	enum InfoType type;

	Parser(char* line);
};

/**
 * Parser::Parser() - Parse a "spelling" line.
 *
 * Fill data members doing the following tasks:
 * - separate the optional comment (a comment starts with '#' and
 *   extend to the end of the line) from the name data.
 * - remove leading and trailing white-spaces.
 * - identify the type of data
 */
Parser::Parser(char* line) {
	ASSERT(line != 0);

	extra = strchr(line, '#');
	if (extra != NULL) {
		// Make [line, extra) a null terminated string
		*extra++ = 0;
	}
	name = (char*) strTrimLeft(line);
	strTrimRight(name);

	type = SPELL_UNKNOWN;
	switch (*name) {
		case 0:
			type = SPELL_EMPTY;
			break;
		case '>':
			type = SPELL_OLDBIO;
			break;
		case '=':
			type = SPELL_ALIAS;
			// Skip over "=" and spaces:
			name++;
			while (*name == ' ') { name++; }
			break;
		case '%':
			if (strIsPrefix("%Elo ", name)) {
				type = SPELL_ELO;
				name += 5; //Skip "%Elo "
			} else if (strIsPrefix("%Bio ", name)) {
				type = SPELL_BIO;
				name += 5; //Skip "%Bio "
			} else if (strIsPrefix("%Prefix ", name)) {
				type = SPELL_PREFIX;
			} else if (strIsPrefix("%Infix ", name)) {
				type = SPELL_INFIX;
			} else if (strIsPrefix("%Suffix ", name)) {
				type = SPELL_SUFFIX;
			}
			break;
		case '@':
			type = SPELL_SECTIONSTART;
			name++; //Skip '@'
			// Now check if there is a list of characters to exclude from
			// comparisons, e.g:   @PLAYER ", .-"
			// would indicate to exclude dots, commas, spaces and dashes.
			extra = strchr(name, '"');
			if (extra != NULL) {
				char* end = strchr(++extra, '"');
				if (end != NULL) {
					*end = 0;
				} else {
					extra = NULL;
				}
			}
			break;
		default:
			type = SPELL_NEWNAME;
			if (extra != NULL) {
				// Spelling files can provide player informations like titles/gender,
				// countries, highest elo, date of birth, date of death. For example:
				// Polgar, Judit           #gm+w HUN [2735] 1976
				strTrimRight(extra);
			}
	}
}

} // End of anonymous namespace


/**
 * class SpellChkLoader - load data into a SpellChecker object
 *
 * This class take parsed "spelling" data and store it into the right
 * data members of the associated SpellChecker object.
 * Reading from a "spelling" file is not stateless and the Parser object
 * cannot contain all the necessary data: a SpellChkLoader object keep track
 * of the current nameT section and the current correct name.
 * The SpellChkValidate object is used to log ignored data, usually
 * caused by typos like "@Eol" or "@Preffix".
 */
class SpellChkLoader {
	SpellChecker& sp_;
	SpellChecker::SpellChkValidate& validate_;
	nameT nt_;
	int32_t nameIdx_;

public:
	SpellChkLoader(SpellChecker& sp, SpellChecker::SpellChkValidate& v)
	: sp_(sp), validate_(v), nt_(NAME_INVALID), nameIdx_(-1) {
	}

	errorT load(const Parser& data, bool* keepBuffer) {
		ASSERT(keepBuffer != 0);
		*keepBuffer = false;

		switch (data.type) {
			case SPELL_SECTIONSTART:
				nt_ = NameBase::NameTypeFromString(data.name);
				if (!NameBase::IsValidNameType(nt_)) return ERROR_CorruptData;
				if (data.extra != NULL) {
					sp_.excludeChars_[nt_] = data.extra;
				} else {
					sp_.excludeChars_[nt_].clear();
				}
				nameIdx_ = -1;
				return OK;
			case SPELL_NEWNAME:
			case SPELL_ALIAS:
			case SPELL_PREFIX:
			case SPELL_INFIX:
			case SPELL_SUFFIX:
				return nameSection(data, keepBuffer);
			case SPELL_BIO:
			case SPELL_ELO:
				return playerInfo(data, keepBuffer);
			case SPELL_EMPTY:
				return OK;
			case SPELL_OLDBIO:
			case SPELL_UNKNOWN:
				validate_.ignoredLine(data.name);
				return OK;
		}

		ASSERT(0);
		return ERROR_CorruptData;
	}

private:
	errorT nameSection(const Parser& data, bool* keepBuffer) {
		// Must be in a valid name section
		if (!NameBase::IsValidNameType(nt_)) return ERROR_CorruptData;

		switch (data.type) {
			case SPELL_NEWNAME:
				*keepBuffer = true;
				ASSERT(sp_.names_[nt_].size() < (1ULL << 31));
				nameIdx_ = static_cast<int32_t>(sp_.names_[nt_].size());
				sp_.names_[nt_].push_back(data.name);
				if (nt_ == NAME_PLAYER) {
					sp_.pInfo_.push_back(data.extra);
				}
				/* FALLTHRU */
			case SPELL_ALIAS:
				if (nameIdx_ == -1) {
					return ERROR_CorruptData;
				} else {
					sp_.idx_[nt_].push_back(SpellChecker::Idx(
						sp_.normalizeAndTransform(nt_, data.name),
						nameIdx_
						));
				}
				return OK;
			case SPELL_PREFIX:
				return sp_.general_[nt_].addPrefix(data.name);
			case SPELL_INFIX:
				return sp_.general_[nt_].addInfix(data.name);
			case SPELL_SUFFIX:
				return sp_.general_[nt_].addSuffix(data.name);
			default:
				ASSERT(0);
		}

		return ERROR_CorruptData;
	}

	errorT playerInfo(const Parser& data, bool* keepBuffer) {
		// SPELL_BIO and SPELL_ELO are valid only for a PLAYER name
		if (nt_ != NAME_PLAYER || nameIdx_ == -1) return ERROR_CorruptData;

		if (data.type == SPELL_BIO) {
			*keepBuffer = true;
			sp_.pInfo_[nameIdx_].bio_.push_back(data.name);
		} else {
			ASSERT(data.type == SPELL_ELO);
			// if necessary, add empty PlayerElo objects
			sp_.pElo_.resize(nameIdx_ + 1);
			sp_.pElo_[nameIdx_].AddEloData(data.name);
		}

		return OK;
	}
};


/**
 * SpellChecker::read() - Read a "spelling" file.
 *
 * This functions tries to open the @filename file and to load the data
 * into the SpellChecker object.
 * The object must be empty. In practice the requirement is to not call
 * this function twice, because this is the only non-const member function.
 * If the function fails (result != OK) the object state is undefined
 * and the only valid operation is to destroy the object.
 * If SPELLCHKVALIDATE is defined, it also creates a @filename.validate log.
 */
errorT SpellChecker::read(const char* filename, const Progress& progress)
{
	ASSERT(filename != NULL);
	ASSERT(staticStrings_ == NULL);

	// Open the file and get the file size.
	Filebuf file;
	std::streamsize fileSize = -1;
	if (file.open(filename, std::ios::in | std::ios::binary | std::ios::ate) != 0) {
		fileSize = file.pubseekoff(0, std::ios::cur, std::ios::in);
		file.pubseekoff(0, std::ios::beg, std::ios::in);
	}
	if (fileSize == -1) return ERROR_FileOpen;

	SpellChkValidate validate(filename, *this);

	// Parse the file lines
	staticStrings_ = (char*) malloc(fileSize + 1);
	char* bEnd = staticStrings_ + fileSize + 1;
	char* line = staticStrings_;
	size_t nRead;
	uint report_i = 0;
	std::streamsize report_done = 0;
	SpellChkLoader loader(*this, validate);
	while ((nRead = file.getline(line, std::distance(line, bEnd))) != 0) {
		report_done += nRead;
		if ((++report_i % 10000) == 0) {
			if (!progress.report(report_done, fileSize))
				return ERROR_UserCancel;
		}

		bool keepBuffer;
		errorT err = loader.load(Parser(line), &keepBuffer);
		if (err != OK) return err;

		if (keepBuffer) line += nRead;
	}
	if (report_done != fileSize || file.sgetc() != EOF) return ERROR_FileRead;

	// Success:
	if (pElo_.size() > 0) {
		// if necessary, add empty PlayerElo objects
		pElo_.resize(pInfo_.size());
		validate.checkEloData();
	}

	// Free unused memory
	const auto new_size = std::distance(staticStrings_, line);
	char* shrink = (char*)realloc(staticStrings_, new_size ? new_size : 1);
	if (shrink) {
		const auto offset = shrink - staticStrings_;
		staticStrings_ = shrink;
		if (offset) { // realloc() moved the memory: update the pointers.
			for (nameT i = 0; i < NUM_NAME_TYPES; i++) {
				for (auto& e : names_[i])
					e += offset;
			}
			for (auto& e : pInfo_) {
				e.comment_ += offset;
				for (auto& bio : e.bio_)
					bio += offset;
			}
		}
	}

	// Sort the index
	for (nameT i=0; i < NUM_NAME_TYPES; i++) {
		std::sort(idx_[i].begin(), idx_[i].end());
		validate.idxDuplicates(i);
	}
	return OK;
}


//////////////////////////////////////////////////////////////////////
//
//  FILE:       spellchk.cpp
//              SpellChecker class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2001-2003  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

// Retrieve the list of Rating figures for given player (aka node) from the given (ssp) string
// The string is formatted as:
// [%Elo ]<year>:<<rating>|?>,...,<<rating>|?> [<year>:<<rating>|?>,...,<<rating>|?>...]
//
// The ratings are stored in a rating array for this player, in the order of appearance
// and without any assumption on the period that the rating refers to.
// This is accomplished by assuming that for all years the same number of rating figures
// could be given (see ELO_RATINGS_PER_YEAR above).
//
// The (external) algorithm to map ratings to actual periods must be able to cope with
// the holes that - as a consequence - will appear in the rating graph constructed here!
//
void PlayerElo::AddEloData(const char * str)
{
    while (1) {
        // Get the year in which the rating figures to follow were published
        //
        str = strTrimLeft (str);
        if (! isdigit(static_cast<unsigned char>(*str))) { break; }
        uint16_t year = strGetUnsigned (str);
        str += 4;
        if (*str != ':') { break; }
        str++;

        // Now read all the ratings for this year:
        //
        eloT elo = 0;
        while (1) {
            if (isdigit(static_cast<unsigned char>(*str))) {
                elo = strGetUnsigned (str);
                str += 4;
            } else if (*str == '?') {
                elo = 0;
                str++;
            } else if (*str == ' ') {
                break;
            } else {
                // Invalid data seen:
                return;
            }

            elo_.push_back(std::make_pair(year, elo));

            if (*str == ',') { str++; }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PlayerInfo::GetTitle:
//    Extract the first title appearing in the player
//    comment, and return it.
const char *
PlayerInfo::getTitle() const
{
    static const char * titles[] = {
        "GM", "IM", "FM",
        "WGM", "WIM", "WFM", "W",
        "CGM", "CIM", "HGM",
        NULL
    };
    const char ** titlePtr = titles;

    const char* comment = GetComment();
    if (*comment == 0) { return ""; }

    while (*titlePtr != NULL) {
        if (strIsCasePrefix (*titlePtr, comment)) { return *titlePtr; }
        titlePtr++;
    }
    return "";
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PlayerInfo::GetLastCountry:
//    Scan the player comment string for the country field (which
//    is the second field, after the title), then return the
//    last three letters in the country field, or the empty string
//    if the country field is less than 3 characters long.
const char *
PlayerInfo::getLastCountry() const
{
    static char country[4];
    country[0] = 0;

    const char* start = GetComment();
    if (*start == 0) { return ""; }

    // Skip over the title field:
    while (*start != ' '  &&  *start != 0) { start++; }
    while (*start == ' ') { start++; }

    const char * end = start;
    int length = 0;
    while (*end != ' '  &&  *end != 0) { end++; length++; }
    // Return the final three characters of the country field:
    if (length >= 3) {
        for (int i=0; i < 3; i++) { country[i] = start[length-3 + i]; }
        country[3] = 0;
    }
    return country;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PlayerInfo::GetPeakRating:
//    Scan the player comment string for the peak rating
//    field (which is contained in brackets), convert it
//    to an unsigned integer, and return it.
eloT
PlayerInfo::getPeakRating() const
{
    const char* s = GetComment();
    if (*s == 0) { return 0; }

    while (*s != '['  &&  *s != 0) { s++; }
    if (*s != '[') { return 0; }
    s++;
    return strGetUnsigned (s);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PlayerInfo::GetBirthdate:
//    Scan the player comment string for the birthdate
//    field, convert it to a date, and return it.
dateT
PlayerInfo::getBirthdate() const
{
    const char* s = GetComment();
    if (*s == 0) { return ZERO_DATE; }

    // Find the end-bracket character after the rating:
    while (*s != ']'  &&  *s != 0) { s++; }
    if (*s != ']') { return ZERO_DATE; }
    s++;
    // Now skip over any spaces:
    while (*s == ' ') { s++; }
    if (*s == 0) { return ZERO_DATE; }
    return date_EncodeFromString (s);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PlayerInfo::GetDeathdate:
//    Scan the player comment string for the deathdate
//    field, convert it to a date, and return it.
dateT
PlayerInfo::getDeathdate() const
{
    const char* s = GetComment();
    if (*s == 0) { return ZERO_DATE; }

    // Find the end-bracket character after the rating:
    while (*s != ']'  &&  *s != 0) { s++; }
    if (*s != ']') { return ZERO_DATE; }
    s++;
    // Now skip over any spaces:
    while (*s == ' ') { s++; }
    // Now skip over the birthdate and dashes:
    while (*s != 0  &&  *s != '-') { s++; }
    while (*s == '-') { s++; }
    if (*s == 0) { return ZERO_DATE; }
    return date_EncodeFromString (s);
}

//////////////////////////////////////////////////////////////////////
//  EOF: spellchk.cpp
//////////////////////////////////////////////////////////////////////
