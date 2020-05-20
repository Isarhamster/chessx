
//////////////////////////////////////////////////////////////////////
//
//  FILE:       misc.h
//              Miscellaneous routines (File I/O, etc)
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2001-2003  Shane Hudson.  All rights reserved.
//              Copyright (C) 2015  Fulvio Benini
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_MISC_H
#define SCID_MISC_H

#include "common.h"
#include <algorithm>
#include <string>
#include <cstring>
#include <stdio.h>
#include <ctype.h>   // For isspace(), etc
#include <cstdlib>
#include <vector>

/**
 * class StrRange - parse a string interpreting its content as 1 or 2 integers
 *                  separated by whitespace.
 * The integers represent the min and max value of a range.
 * If only one integer is provided it will represent both the min and max value.
 */
class StrRange {
protected:
	long min_;
	long max_;

protected:
	StrRange()
	: min_(0), max_(0) {}

public:
	explicit StrRange(const char* range) {
		char* next;
		min_ = std::strtol(range, &next, 10);
		char* end;
		max_ = std::strtol(next, &end, 10);
		if (next == end) max_ = min_;
		if (min_ > max_) std::swap(min_, max_);
	}

	/// @returns true if @e val is >= min_ and <= max_
	bool inRange(long val) const {
		if (val < min_ || val > max_) return false;
		return true;
	}
};


class Progress {
public:
	struct Impl {
		virtual ~Impl() {}
		virtual bool report(size_t done, size_t total, const char* msg) = 0;
	};

	Progress(Impl* f = NULL) : f_(f) {}
	Progress(const Progress&) = delete;
	~Progress() { delete f_; }

	bool report(size_t done, size_t total) const {
		return operator()(done, total);
	}
	bool operator()(size_t done, size_t total, const char* msg = NULL) const {
		if (f_) return f_->report(done, total, msg);
		return true;
	}

private:
	Impl* f_;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetFilterOp:
//    Converts a string value to a filter operation value.
enum filterOpT { FILTEROP_AND, FILTEROP_OR, FILTEROP_RESET };

inline filterOpT strGetFilterOp (const char * str)
{
    switch (*str) {
        // AND:
        case 'A': case 'a': case '0': return FILTEROP_AND;
        // OR:
        case 'O': case 'o': case '1': return FILTEROP_OR;
        // RESET:
        case 'R': case 'r': case '2': return FILTEROP_RESET;
    }
    // Default is RESET.
    return FILTEROP_RESET;
}

// ECO string routines
//
void eco_ToString (ecoT ecoCode, char * ecoStr, bool extensions = true);
inline void eco_ToBasicString (ecoT ecoCode, char * ecoStr) {
    eco_ToString (ecoCode, ecoStr, false);
}
inline void eco_ToExtendedString (ecoT ecoCode, char * ecoStr) {
    eco_ToString (ecoCode, ecoStr, true);
}
ecoT eco_FromString (const char * ecoStr);
ecoT eco_LastSubCode (ecoT ecoCode);
ecoT eco_BasicCode (ecoT ecoCode);
ecoT eco_Reduce(ecoT eco);

// String routines. Some are identical to ANSI standard functions, but
//      I have included them:
//      (a) to keep nice consistent naming conventions, e.g. strCopy.
//      (b) so stats can easily be kept by modifying the functions.
//      (c) so some can be made inline for speed if necessary.

inline uint32_t strStartHash(const char* str) {
	ASSERT(str != 0);
	const unsigned char* s = reinterpret_cast<const unsigned char*>(str);

	uint32_t tmp = static_cast<unsigned char>(tolower(*s));
	uint32_t result = tmp << 24;
	if (*s == '\0') return result;
	tmp = static_cast<unsigned char>(tolower(*++s));
	result += tmp << 16;
	if (*s == '\0') return result;
	tmp = static_cast<unsigned char>(tolower(*++s));
	result += tmp << 8;
	if (*s == '\0') return result;
	result += static_cast<unsigned char>(tolower(*++s));
	return result;
}

char * strDuplicate (const char * str);

char * strAppend (char * target, const char * extra);
uint   strPad (char * target, const char * orig, int length, char pad);
const char * strFirstChar (const char * target, char matchChar);
const char * strLastChar (const char * target, char matchChar);
void   strStrip (char * str, char ch);

const char * strTrimLeft (const char * target, const char * trimChars);
inline const char * strTrimLeft (const char * target) {
    return strTrimLeft (target, " \t\r\n");
}
uint   strTrimSuffix (char * target, char suffixChar);
void   strTrimDate (char * str);
void   strTrimMarkCodes (char * str);
void   strTrimMarkup (char * str);
const char * strFirstWord (const char * str);
const char * strNextWord (const char * str);

// strPlural:
//    Returns the empty string if its parameter is 1, or "s" otherwise.
inline const char *
strPlural (uint x) {
    return (x == 1 ? "" : "s");
}

bool   strIsUnknownName (const char * str);

// strIsSurnameOnly: returns true if a string appears to only
//    contain a surname.
bool   strIsSurnameOnly (const char * name);

bool   strGetBoolean (const char * str);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetInteger():
//    Extracts a signed base-10 value from a string.
//    Defaults to zero (as strtol does) for non-numeric strings.
inline int
strGetInteger(const char * str)
{
	return std::strtol(str, NULL, 10);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetUnsigned():
//    Extracts an unsigned base-10 value from a string.
//    Defaults to zero (as strtoul does) for non-numeric strings.
//
inline uint32_t strGetUnsigned(const char* str) {
	ASSERT(str != NULL);
	return static_cast<uint32_t>(std::strtoul(str, NULL, 10));
}

inline int strCaseCompare(const char* str1, const char* str2) {
	ASSERT(str1 != NULL && str2 != NULL);
	const unsigned char* s1 = reinterpret_cast<const unsigned char*>(str1);
	const unsigned char* s2 = reinterpret_cast<const unsigned char*>(str2);
	int c1, c2;
	do {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
		if (c1 == '\0')
			break;
	} while (c1 == c2);

	return c1 - c2;
}

inline int strCompareRound(const char* str1, const char* str2) {
	ASSERT(str1 != NULL && str2 != NULL);
	uint32_t a = strGetUnsigned(str1);
	uint32_t b = strGetUnsigned(str2);
	if (a == b)
		return strCaseCompare(str1, str2);
	return (a < b) ? -1 : 1;
}

inline bool strEqual(const char* str1, const char* str2) {
	ASSERT(str1 != NULL && str2 != NULL);
	return (std::strcmp(str1, str2) == 0);
}

void   strGetIntegers (const char * str, int * results, uint nResults);
void   strGetUnsigneds (const char * str, uint * results, uint nResults);
resultT strGetResult (const char * str);

typedef uint flagT;
const flagT FLAG_EMPTY = 0;
const flagT FLAG_YES = 1;
const flagT FLAG_NO = 2;
const flagT FLAG_BOTH = 3;
inline bool flag_Yes (flagT t) { return (t & FLAG_YES); }
inline bool flag_No (flagT t) { return (t & FLAG_NO); }
flagT  strGetFlag (const char * str);

squareT strGetSquare (const char * str);

inline uint
strTrimFileSuffix (char * target) { return strTrimSuffix (target, '.'); }

inline const char *
strFileSuffix (const char * target) { return strLastChar (target, '.'); }



int strUniqueExactMatch (const char * keyStr, const char ** strTable,
                         bool exact);

inline int strUniqueMatch (const char * keyStr, const char ** strTable) {
    return strUniqueExactMatch (keyStr, strTable, false);
}
inline int strExactMatch (const char * keyStr, const char ** strTable) {
    return strUniqueExactMatch (keyStr, strTable, true);
}

inline bool
strContainsChar (const char * str, char ch)
{
    while (*str) {
        if (*str == ch) { return true; }
        str++;
    }
    return false;
}

// WARNING: Avoid this function!
// Considering that the sign of a char is implementation-defined [3.9.1], the
// int conversion ("[4.7.3] If the destination type is signed, the value is
// unchanged if it can be represented in the destination type") can yield
// different results on different architectures or compilers.
// A better alternative is the standard function strcmp() that returns the
// difference between the values of the first pair of characters (both
// interpreted as unsigned char) that differ in the strings being compared.
inline int strCompare(const char* s1, const char* s2)
{
    ASSERT (s1 != NULL  &&  s2 != NULL);
    while (1) {
        if (*s1 != *s2) {
            return ((int) *s1) - ((int) *s2);
        }
        if (*s1 == 0)
            break;
        s1++; s2++;
    }
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strCopy(): same as strcpy().
//
inline void
strCopy (char * target, const char * original)
{
    ASSERT (target != NULL  &&  original != NULL);
    while (*original != 0) {
        *target = *original;
        target++;
        original++;
    }
    *target = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strPrefix():
//       Returns the length of the common prefix of two strings.
//
inline uint
strPrefix (const char * s1, const char * s2)
{
    ASSERT (s1 != NULL  &&  s2 != NULL);
    uint count = 0;
    while (*s1 == *s2) {
        if (*s1 == 0) { // seen end of string, strings are identical
            return count;
        }
        count++; s1++; s2++;
    }
    return count;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strIsPrefix():
//      Returns true if the prefix string is a prefix of longStr.
inline bool
strIsPrefix (const char * prefix, const char * longStr)
{
    while (*prefix) {
        if (*longStr == 0) { return false; }
        if (*prefix != *longStr) { return false; }
        prefix++;
        longStr++;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strIsCasePrefix():
//      Returns true if the prefix string is a case-insensitive
//      prefix of longStr.
inline bool
strIsCasePrefix (const char * prefix, const char * longStr)
{
    typedef unsigned char U;
    while (*prefix) {
        if (*longStr == 0) { return false; }
        if (tolower(U(*prefix)) != tolower(U(*longStr))) { return false; }
        prefix++;
        longStr++;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strIsAlphaPrefix():
//      Returns true if the prefix string is a prefix of longStr.
//      Unlike strIsPrexix(), this version is case-insensitive and
//      spaces are ignored.
//      Example: strIsAlphaPrefix ("smith,j", "Smith, John") == true.
inline bool
strIsAlphaPrefix (const char * prefix, const char * longStr)
{
    typedef unsigned char U;
    while (*prefix) {
        while (*prefix == ' ') { prefix++; }
        while (*longStr == ' ') { longStr++; }
        if (*longStr == 0) { return false; }
        if (tolower(U(*prefix)) != tolower(U(*longStr))) { return false; }
        prefix++;
        longStr++;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strContains():
//      Returns true if longStr contains an occurrence of keyStr,
//      case-sensitive and NOT ignoring any characters such as spaces.
inline bool
strContains (const char * longStr, const char * keyStr)
{
    while (*longStr) {
        if (strIsPrefix (keyStr, longStr)) { return true; }
        longStr++;
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strAlphaContains():
//      Returns true if longStr contains an occurrence of keyStr,
//      case-insensitive and ignoring spaces.
//      Example: strAlphaContains ("Smith, John", "th,j") == true.
//
inline bool
strAlphaContains (const char * longStr, const char * keyStr)
{
    while (*longStr) {
        if (strIsAlphaPrefix (keyStr, longStr)) { return true; }
        longStr++;
    }
    return false;
}

inline uint
strLength (const char * str)
{
    ASSERT(str != NULL);
    uint len = 0;
    while (*str != 0) { len++; str++; }
    return len;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimRight():
//      Trims the provided string in-place, removing the
//      end characters that match the trimChars.
//      Returns the number of characters trimmed.
//      E.g., strTrimRight("abcyzyz", "yz") would leave the string
//      as "abc".
inline void strTrimRight(char* target, const char* trimChars, size_t nTrimCh) {
	const char* endTrim = trimChars + nTrimCh;
	size_t iCh = strlen(target);
	for (; iCh > 0; --iCh) {
		if (std::find(trimChars, endTrim, target[iCh - 1]) == endTrim)
			break;
	}
	target[iCh] = '\0';
}
inline void strTrimRight(char* target) {
	return strTrimRight(target, " \t\r\n", 4);
}

#endif  // #ifdef SCID_MISC_H

//////////////////////////////////////////////////////////////////////
//  EOF: misc.h
//////////////////////////////////////////////////////////////////////

