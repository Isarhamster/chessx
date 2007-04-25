//////////////////////////////////////////////////////////////////////
//
//  FILE:       misc.h
//              Miscellaneous routines (File I/O, etc)
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2001-2003  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_MISC_H
#define SCID_MISC_H

#include "common.h"
#include "error.h"
#include <ctype.h>   // For isspace(), etc

namespace Guess
{

// Scid initialisation routine: this MUST be called before such things as
// computing chess moves are done, since it sets up piece movement tables.
//
void   scid_Init ();


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


// String routines. Some are identical to ANSI standard functions, but
//      I have included them:
//      (a) to keep nice consistent naming comventions, e.g. strCopy.
//      (b) so stats can easily be kept by modifying the functions.
//      (c) so some can be made inline for speed if necessary.
//
//      Currently, strLength() and strPrefix() are inline.
//      strCompare_INLINE() is an inline equivalent of strCompare().

// charIsSpace:
//   Return true if the char is whitespace, including ASCII-160 (a
//   non-breaking space, = 240 octal or A0 hex).
inline bool
charIsSpace (byte ch) {
    return (isspace(ch)  ||  ch == 160);
}

char * strDuplicate (const char * str);
int    strCompare (const char * s1, const char * s2);
int    strCaseCompare (const char * s1, const char * s2);
int    strCompareRound (const char * sleft, const char * sright);

inline bool
strEqual (const char * s1, const char * s2) {
    return (strCompare (s1, s2) == 0);
}

inline bool
strCaseEqual (const char * s1, const char * s2) {
    return (strCaseCompare (s1, s2) == 0);
}

void   strCopy (char * target, const char * original);
void   strCopyExclude (char * target, const char * original,
                       const char * excludeChars);
char * strAppend (char * target, const char * extra);
char * strAppend (char * target, uint u);
char * strAppend (char * target, int i);
char * strAppend (char * target, const char * s1, const char * s2);
char * strAppend (char * target, const char * s1, const char * s2);
char * strAppend (char * target, const char * s1, const char * s2,
                  const char * s3);
char * strAppend (char * target, const char * s1, const char * s2,
                  const char * s4);
uint   strPrefix (const char * s1, const char * s2);
uint   strPad (char * target, const char * orig, int length, char pad);
const char * strFirstChar (const char * target, char matchChar);
const char * strLastChar (const char * target, char matchChar);
void   strStrip (char * str, char ch);

static const char WHITESPACE[6] = " \t\r\n";
const char * strTrimLeft (const char * target, const char * trimChars);
inline const char * strTrimLeft (const char * target) {
    return strTrimLeft (target, WHITESPACE);
}
uint   strTrimRight (char * target, const char * trimChars);
inline uint strTrimRight (char * target) {
    return strTrimRight (target, WHITESPACE);
}
uint   strTrimSuffix (char * target, char suffixChar);
void   strTrimDate (char * str);
void   strTrimMarkCodes (char * str);
void   strTrimMarkup (char * str);
void   strTrimSurname (char * str, uint initials);
inline void strTrimSurname (char * str) { strTrimSurname (str, 0); }
const char * strFirstWord (const char * str);
const char * strNextWord (const char * str);

// strPlural:
//    Returns the empty string if its parameter is 1, or "s" otherwise.
inline const char *
strPlural (uint x) {
    return (x == 1 ? "" : "s");
}

uint   strSingleSpace (char * str);

bool   strIsAllWhitespace (const char * str);
bool   strIsUnknownName (const char * str);

// strIsPrefix: returns true if prefix is a prefix of longStr.
bool   strIsPrefix (const char * prefix, const char * longStr);

// strIsCasePrefix: like strIsPrefix, but case-insensitive.
bool   strIsCasePrefix (const char * prefix, const char * longStr);

// strIsAlphaPrefix: like strIsPrefix, but case-insensitive and space
//    characters are ignored.
bool   strIsAlphaPrefix (const char * prefix, const char * longStr);

// strIsSurnameOnly: returns true if a string appears to only
//    contain a surname.
bool   strIsSurnameOnly (const char * name);

// strAlphaContains: returns true if longStr contains keyStr,
//    case-insensitive and ignoring spaces. strContains is similar but
//    is case-sensitive and does not ignore spaces.
bool   strAlphaContains (const char * longStr, const char * keyStr);

bool   strContains (const char * longStr, const char * keyStr);
int    strContainsIndex (const char * longStr, const char * keyStr);

bool   strGetBoolean (const char * str);
int    strGetInteger (const char * str);
uint   strGetUnsigned (const char * str);
void   strGetIntegers (const char * str, int * results, uint nResults);
void   strGetUnsigneds (const char * str, uint * results, uint nResults);
void   strGetBooleans (const char * str, bool * results, uint nResults);
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

inline int
strCompare_INLINE (const char *s1, const char *s2)
{
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

inline uint
strLength (const char * str)
{
    ASSERT(str != NULL);
    uint len = 0;
    while (*str != 0) { len++; str++; }
    return len;
}


//////////////////////////////////////////////////////////////////////
//   MATH functions

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// log2(): Returns logarithm (base 2) of the integer x.
//      log2(0 or 1) = 0, log2(2 or 3) = 1,
//      log2(4/5/6/7) = 2, etc.
//
inline uint
log2 (uint x)
{
    uint result = 0;
    x = x >> 1;
    while (x) { result++; x = x >> 1; }
    return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// isPowerOf2():
//      Fast test for a power of two. Returns true (nonzero) only
//      if x is a power of two (0, 1, 2, 4, 8, 16, etc).
//
inline uint
isPowerOf2 (uint x)
{
    return ((x & (x-1)) == 0);
}


//////////////////////////////////////////////////////////////////////
//   FILE I/O Routines.

uint    fileSize (const char * name, const char * suffix);
uint    rawFileSize (const char * name);
uint    gzipFileSize (const char * name);

bool    fileExists (const char * fname, const char * suffix);
errorT  renameFile (const char * oldName, const char * newName,
                    const char * suffix);
errorT  removeFile (const char * fname, const char * suffix);
errorT  createFile (const char * fname, const char * suffix);


errorT  writeString (FILE * fp, char * str, uint length);
errorT  readString  (FILE * fp, char * str, uint length);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeOneByte(), readOneByte()

inline errorT
writeOneByte (FILE * fp, byte value)
{
    ASSERT(fp != NULL);
    return (putc(value, fp) == EOF) ? ERROR_FileWrite : OK;
}

inline byte
readOneByte (FILE * fp)
{
    ASSERT(fp != NULL);
    return (byte) getc(fp);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeTwoBytes(), readTwoBytes()

inline errorT
writeTwoBytes (FILE * fp, uint value)
{
    ASSERT(fp != NULL);
    int result;
    int v = (value >> 8)  & 255;    putc(v, fp);
    v = value & 255;                result = putc(v, fp);
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readTwoBytes (FILE *fp)
{
    ASSERT(fp != NULL);
    uint v = getc(fp);
    v = v << 8;    v += getc(fp);
    return v;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!
// writeThreeBytes(), readThreeBytes()

inline errorT
writeThreeBytes (FILE * fp, uint value)
{
    ASSERT(fp != NULL);
    int result;
    int v = (value >> 16)  & 255;   putc(v, fp);
    v = (value >> 8) & 255;         putc(v, fp);
    v = value & 255;                result = putc(v, fp);
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readThreeBytes (FILE * fp)
{
    ASSERT(fp != NULL);
    uint v = getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    return v;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeFourBytes(), readFourBytes()

inline errorT
writeFourBytes (FILE * fp, uint value)
{
    ASSERT(fp != NULL);
    int result;
    uint v = (value >> 24) & 255;   putc(v, fp);
    v = (value >> 16) & 255;        putc(v, fp);
    v = (value >>  8) & 255;        putc(v, fp);
    v = value & 255;                result = putc(v, fp);
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readFourBytes (FILE * fp)
{
    ASSERT(fp != NULL);
    uint v = getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    return v;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readCompactUint, writeCompactUint:
//   Read/write an unsigned int using a variable number
//   of bytes: 1 for 0-127, 2 for 128-16383, etc.

inline errorT
writeCompactUint (FILE * fp, uint value)
{
    ASSERT (fp != NULL);
    int result;
    while (true) {
        if (value < 128) {
            result = putc (value, fp);
            break;
        }
        putc ((value & 127) | 128, fp);
        value = value >> 7;
    }
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readCompactUint (FILE * fp)
{
    ASSERT (fp != NULL);
    uint v = 0;
    uint bitIndex = 0;
    while (true) {
        uint b = (uint) getc(fp);
        v = v | ((b & 127) << bitIndex);
        if (! (b & 128)) { break; }
        bitIndex += 7;
    }
    return v;
}

} // End namespace Guess

#endif  // #ifdef SCID_MISC_H

//////////////////////////////////////////////////////////////////////
//  EOF: misc.h
//////////////////////////////////////////////////////////////////////

