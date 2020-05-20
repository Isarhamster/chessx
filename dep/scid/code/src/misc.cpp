//////////////////////////////////////////////////////////////////////
//
//  FILE:       misc.cpp
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

#include "common.h"
#include "misc.h"
#include <stdio.h>
#include <ctype.h>     // For isspace() function.
#include <cmath>

//////////////////////////////////////////////////////////////////////
//   ECO Code Routines

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// eco_FromString():
//    Extract an ECO code from a string.
//
//    Eco code numbering: no eco = 0, A00 = 1, A01 = 132, etc.
//    That is, each basic ECO code = previous + 131.
//    The extra 130 subcodes are the extended code:
//    a, a1, a2, a3, a4, b, b1, .... z, z1, z2, z3, z4.  (130 in total).
//
//    Improvement, March 2000: now case-insensitive for first letter,
//    for example, a41 == A41.
ecoT
eco_FromString (const char * ecoStr)
{
    ecoT eco = ECO_None;
    // Get the basic Eco code from the first 3 characters: they MUST be in
    // the range "A00" to "E99" or the eco code will be considered empty.
    // Changed, June 1999: now accepts partial ECO codes, e.g. "C1" -> C10

    if (*ecoStr >= 'A'  &&  *ecoStr <= 'E') {
        eco = (*ecoStr - 'A') * 13100;
    } else if (*ecoStr >= 'a'  &&  *ecoStr <= 'e') {
        eco = (*ecoStr - 'a') * 13100;
    } else {
        return 0;
    }
    ecoStr++;
    if (! *ecoStr) { return eco + 1; }

    if (*ecoStr < '0'  ||  *ecoStr > '9') { return 0; }
    eco += (*ecoStr - '0') * 1310;
    ecoStr++;
    if (! *ecoStr) { return eco + 1; }

    if (*ecoStr < '0'  ||  *ecoStr > '9') { return 0; }
    eco += (*ecoStr - '0') * 131;
    ecoStr++;

    // Now check for the optional extended code: a, a1, ... z2, z3, z4.
    if (*ecoStr >= 'a'  &&  *ecoStr <= 'z') {
        eco++;
        eco += (*ecoStr - 'a') * 5;
        ecoStr++;
        if (*ecoStr >= '1'  &&  *ecoStr <= '4') {
            eco += *ecoStr - '0';
        }
    }
    return eco + 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// eco_ToString():
//      Convert an ECO code to its string representation.
void
eco_ToString (ecoT ecoCode, char * ecoStr, bool extensions)
{
    char * s = ecoStr;
    if (ecoCode == ECO_None) { *s = 0; return; }
    ecoCode--;
    
    // First the base code value:
    
    ecoT basicCode = ecoCode / 131;    // 131 = 26 * 5 + 1 subcodes.
    *s++ = basicCode / 100 + 'A';
    *s++ = (basicCode % 100) / 10 + '0';
    *s++ = (basicCode % 10) + '0';
    
    // Now the optional extensions:
    if (extensions) {
        ecoCode = ecoCode % 131;
        if (ecoCode > 0) {
            ecoCode--;
            *s++ = (ecoCode / 5) + 'a';
            ecoCode = ecoCode % 5;
            if (ecoCode > 0) { *s++ = (ecoCode + '0'); }
        }
        *s = 0;
    }
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// eco_BasicCode():
//    Converts an ECO code to its basic form, without any
//    Scid-specific extensions.
ecoT
eco_BasicCode (ecoT eco)
{
    if (eco == ECO_None) { return ECO_None; }

    eco--;
    eco /= 131;
    eco *= 131;
    return eco + 1;
}

/**
 * ecoReduce() - maps eco to a smaller set
 * @param eco: the eco value to convert (must be != 0)
 *
 * Scid ECO subcodes use 131 values for each canonical ECO.
 * For example A00 is divided in A00,A00a,A00a1,A00a2,A00a3,A00a4,A00b...A00z4
 * corresponding to eco values 1,2,3,4,5,6,7...131 (value 0 means no ECO).
 * This functions will map subECOs like A00a1...A00a4 into A00a, reducing
 * the 131 values to 27. The previous sequence will became 0,1,1,1,1,1,2...26
 */
ecoT eco_Reduce(ecoT eco) {
	ASSERT(eco != 0);

	eco--;
	ecoT res = (eco / 131) * 27;
	return res + static_cast<ecoT>(std::ceil((eco % 131) / 5.0));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// eco_LastSubCode():
//      Converts an ECO code to the deepest subcode it could contain.
//      Examples: B91a -> B91a4  and  B91 -> B91z4.
ecoT
eco_LastSubCode (ecoT eco)
{
    if (eco == ECO_None) { return ECO_None; }

    // if just a basic ECO code (1 letter, 2 digits), add the "z":
    eco--;
    if ((eco % 131) == 0) { eco += 126; }  // 126 = 5 * 25 + 1.

    // Now if no final digit, add the "4":
    if (((eco % 131) % 5) == 1) { eco += 4; }
    return eco + 1;
}

//////////////////////////////////////////////////////////////////////
//   String Routines

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strAppend():
//      Appends extra to the end of target, and returns a pointer
//      to the new END of the string target.
//
char *
strAppend (char * target, const char * extra)
{
    ASSERT (target != NULL  &&  extra != NULL);
    while (*target != 0)  { target++; }  // get to end of target string
    while (*extra != 0) {
        *target = *extra;
        target++;
        extra++;
    }
    *target = 0;
    return target;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strDuplicate(): Duplicates a string using new[] operator.
//
char *
strDuplicate (const char * original)
{
    ASSERT (original != NULL);
    char * newStr = new char [strLength(original) + 1];
    if (newStr == NULL)  return NULL;
    char *s = newStr;
    while (*original != 0) {
        *s = *original;
        s++; original++;
    }
    *s = 0;   // Add trailing '\0'.
    //printf ("Dup: %p: %s\n", newStr, newStr);
    return newStr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strPad():
//      Copies original string to target, but copies *exactly* 'width'
//      bytes. If the original is longer than specified width, not all of
//      original will be copied to target.  If original is shorter, then
//      target will be padded out to 'width' bytes with the padding char.
//
//      If the width is negative, no trimming or padding is done and
//      the result is just a regular string copy.
//
//      The return value is the length copied: always 'width' if
//      width is >= 0, or the length of original if 'width' is negative.
//
uint
strPad (char * target, const char * original, int width, char padding)
{
    ASSERT (target != NULL  &&  original != NULL);
    if (width < 0) {
        strCopy (target, original);
        return strLength (original);
    }
    int len = width;
    while (len > 0) {
        if (*original == 0) {
            break;
        }
        *target = *original;
        target++;
        original++;
        len--;
    }
    while (len--) { *target++ = padding; }
    *target = 0;
    return width;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strFirstChar():
//      Returns the pointer into the provided string where the
//      FIRST occurrence of matchChar is, or NULL if the string
//      does not contain matchChar at all.
//      Equivalent to strchr().
const char *
strFirstChar (const char * target, char matchChar)
{
    const char * s = target;
    while (*s != 0) {
        if (*s == matchChar) { return s; }
        s++;
    }
    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strLastChar():
//      Returns the pointer into the provided string where the
//      LAST occurrence of matchChar is, or NULL if the string
//      does not contain matchChar at all.
//      Equivalent to strrchr().
const char *
strLastChar (const char * target, char matchChar)
{
    const char * s = target;
    const char * last = NULL;
    while (*s != 0) {
        if (*s == matchChar) { last = s; }
        s++;
    }
    return last;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strStrip():
//      Removes all occurrences of the specified char from the string.
void
strStrip (char * str, char ch)
{
    char * s = str;
    while (*str != 0) {
        if (*str != ch) {
            if (s != str) { *s = *str; }
            s++;
        }
        str++;
    }
    *s = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimLeft():
//      Returns the pointer into the provided string where the first
//      character that does NOT equal a trimChar occurs.
const char *
strTrimLeft (const char * target, const char * trimChars)
{
    const char * s = target;
    while (*s != 0) {
        if (! strContainsChar (trimChars, *s)) { break; }
        s++;
    }
    return s;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimSuffix():
//      Trims the provided string in-place, at the last
//      occurrence of the provided suffix character.
//      Returns the number of characters trimmed.
//      E.g., strTrimSuffix ("file.txt", '.') would leave the
//      string as "file" and return 4.
uint
strTrimSuffix (char * target, char suffixChar)
{
    uint trimCount = 0;
    char * lastSuffixPtr = NULL;
    char * s = target;
    while (*s) {
        if (*s == suffixChar) {
            lastSuffixPtr = s;
            trimCount = 0;
        }
        trimCount++;
        s++;
    }
    if (lastSuffixPtr == NULL) { return 0; }
    *lastSuffixPtr = 0;
    return trimCount;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimDate():
//    Takes a date string ("xxxx.xx.xx" format) and trims
//    the day part if it is ".??", and also the month part
//    if it too is ".??".
void
strTrimDate (char * str)
{
    if (str[7] == '.'  &&  str[8] == '?'  &&  str[9] == '?') {
        str[7] = 0;
        if (str[4] == '.'  &&  str[5] == '?'  &&  str[6] == '?') {
            str[4] = 0;
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimMarkCodes():
//    Trims in-place all Scid-recognised board mark codes
//    in a comment string, such as "[%mark ...]" and "[%arrow ...]"
void
strTrimMarkCodes (char * str)
{
    char * in = str;
    char * out = str;
    bool inCode = false;
    char * startLocation = NULL;

    while (1) {
        char ch = *in;
        if (inCode) {
            // If we see end-of-string or code-starting '[', there is some
            // error so go back to the start of this code and treat it
            // normally.
            if (ch == 0  ||  ch == '[') {
                *out++ = *startLocation;
                inCode = false;
                in = startLocation;
            } else if (ch == ']') {
                // See a code-ending ']', so end the code.
                inCode = false;
            }
            // For all other characters in a code, just ignore it.
        } else {
            // Stop at end-of-string:
            if (ch == 0) { break; }
            // Look for the start of a code that is to be stripped:
            if (ch == '['  &&  in[1] == '%') {
                inCode = true;
                startLocation = in;
            } else {
                *out++ = ch;
            }
        }
        in++;
    }
    // Terminate the modified string:
    *out = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strTrimMarkup():
//    Trims in-place all HTML-like markup codes (<b>, </i>, etc)
//    from the provided string.
void
strTrimMarkup (char * str)
{
    char * in = str;
    char * out = str;
    bool inTag = false;

    while (*in != 0) {
        char ch = *in;
        if (inTag) {
            if (ch == '>') { inTag = false; }
        } else {
            if (ch == '<') { inTag = true; } else { *out++ = ch; }
        }
        in++;
    }
    *out = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strFirstWord:
//    Skips over all whitespace at the start of the
//    string to reach the first word.
const char *
strFirstWord (const char * str)
{
    ASSERT (str != NULL);
    while (*str != 0  &&  isspace(static_cast<unsigned char>(*str))) { str++; }
    return str;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strNextWord:
//    Skips over all successive non-whitespace characters
//    in the string, then all successive whitespace chars,
//    to reach the next word in the string.
const char *
strNextWord (const char * str)
{
    ASSERT (str != NULL);
    while (*str != 0  &&  !isspace(static_cast<unsigned char>(*str))) { str++; }
    while (*str != 0  &&  isspace(static_cast<unsigned char>(*str))) { str++; }
    return str;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strIsUnknownName():
//    Returns true if the string is an "unknown" name: the empty
//    string, "?" or "-". Used primarily to test if an event, site
//    or round name string contains information worth printing.
bool
strIsUnknownName (const char * str)
{
    if (str[0] == 0) { return true; }
    if (str[0] == '-'  &&  str[1] == 0) { return true; }
    if (str[0] == '?'  &&  str[1] == 0) { return true; }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strIsSurnameOnly():
//    Returns true if the name appears to be a surname only.
bool
strIsSurnameOnly (const char * name)
{
    uint capcount = 0;
    const char * s = name;
    while (*s != 0) {
        unsigned char c = *s;
        if (! isalpha(c)) { return false; }
        if (isupper(c)) {
            capcount++;
            if (capcount > 1) { return false; }
        }
        s++;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetBoolean():
//      Extracts a boolean value from a string.
//      True strings start with one of "TtYy1", false strings with
//      one of "FfNn0".
//      Returns false if the string does not contain a boolean value.
bool
strGetBoolean (const char * str)
{
    static const char * sTrue[] = {
        "true", "yes", "on", "1", "ja", "si", "oui", NULL
    };
    static const char * sFalse[] = {
        "false", "no", "off", "0", NULL
    };
    if (str[0] == 0) { return false; }

    bool matchedTrue = false;
    bool matchedFalse = false;

    const char ** next = sTrue;
    while (*next != NULL) {
        if (strIsCasePrefix (str, *next)  ||  strIsCasePrefix (*next, str)) {
           matchedTrue = true;
        }
        next++;
    }
    next = sFalse;
    while (*next != NULL) {
        if (strIsCasePrefix (str, *next)  ||  strIsCasePrefix (*next, str)) {
           matchedFalse = true;
        }
        next++;
    }
    if (matchedTrue  &&  !matchedFalse) { return true; }
    if (matchedFalse  &&  !matchedTrue) { return false; }

    // default: return false.
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetIntegers:
//    Extracts the specified number of signed integers in a
//    whitespace-separated string to an array.
void
strGetIntegers (const char * str, int * results, uint nResults)
{
    for (uint i=0; i < nResults; i++) {
        while (*str != 0  &&  isspace(static_cast<unsigned char>(*str))) { str++; }
        results[i] = strGetInteger (str);
        while (*str != 0  &&  !isspace(static_cast<unsigned char>(*str))) { str++; }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetUnsigneds:
//    Extracts the specified number of unsigned integers in a
//    whitespace-separated string to an array.
void
strGetUnsigneds (const char * str, uint * results, uint nResults)
{
    for (uint i=0; i < nResults; i++) {
        while (*str != 0  &&  isspace(static_cast<unsigned char>(*str))) { str++; }
        results[i] = strGetUnsigned (str);
        while (*str != 0  &&  !isspace(static_cast<unsigned char>(*str))) { str++; }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetResult:
//    Extracts a game result value from a string.
resultT
strGetResult (const char * str)
{
    switch (*str) {
    case '1':
        // Check for "1/2"-style draw result:
        if (str[1] == '/'  &&  str[2] == '2') {
            return RESULT_Draw;
        }
        return RESULT_White;
    case '=': return RESULT_Draw;
    case '0': return RESULT_Black;
    case '*': return RESULT_None;
    }
    return RESULT_None;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetFlag():
//    Extracts a flag (FLAG_YES, FLAG_NO or FLAG_BOTH) value from
//    a string. Defaults to FLAG_EMPTY.
flagT
strGetFlag (const char * str)
{
    char c = *str;
    switch (c) {
    case 'T':  case 't':
    case 'Y':  case 'y':
    case 'J':  case 'j':
    case 'O':  case 'o':
    case 'S':  case 's':
    case '1':
        return FLAG_YES;
    case 'F':  case 'f':
    case 'N':  case 'n':
    case '0':
        return FLAG_NO;
    case 'B':  case 'b':
    case '2':
        return FLAG_BOTH;
    }
    // default: return empty.
    return FLAG_EMPTY;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strGetSquare():
//   Extracts a square value from a string, such as "a2".
squareT
strGetSquare (const char * str)
{
    char chFyle = str[0];
    if (chFyle < 'a'  ||  chFyle > 'h') { return NULL_SQUARE; }
    char chRank = str[1];
    if (chRank < '1'  ||  chRank > '8') { return NULL_SQUARE; }
    return square_Make (fyle_FromChar(chFyle), rank_FromChar(chRank));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// strUniqueExactMatch():
//      Given a string <keyStr> and a null-terminated array of strings
//      <strTable>, returns the index of the unique match of the key
//      string in the string table. If no match was found, or there was
//      more than one match, -1 is returned.
//
//      If the flag <exact> is true, only complete matches are considered.
//      Otherwise, unique abbreviations are accepted.
//      Example: looking up "repl" in {"repeat", "replace", NULL} would
//      return 1 (matching "replace") but looking up "rep" would
//      return -1 because its match is ambiguous.
//
//      The array "strTable" does NOT need to be in any order, but the last
//      entry must be NULL.
int
strUniqueExactMatch (const char * keyStr, const char ** strTable, bool exact)
{
    int index = -1;
    int abbrevMatches = 0;
    const char * s1;
    const char * s2;
    const char ** entryPtr = strTable;
    
    // If keyStr or strTable are null, return no match:
    if (keyStr == NULL  ||  strTable == NULL) { return -1; }
    
    // Check each entry in turn:
    for (int i=0;  *entryPtr != NULL;  entryPtr++, i++) {
        // Check the key against this entry, character by character:
        for (s1 = keyStr, s2 = *entryPtr;  *s1 == *s2;  s1++, s2++) {
            // If *s1 is 0, we found an EXACT match, so return it now:
            if (*s1 == 0) {
                return i;
            }
        }
        // If *s1 == 0 now, key is an abbreviation of this entry:
        if (*s1 == 0) {
            index = i;
            abbrevMatches++;
        }
    }
    
    // If we reach here, there is no exact match.  If an exact match was
    // required, or there is not exactly one abbreviation, return no match:
    if (exact  ||  abbrevMatches != 1) {
        return -1;
    }
    // Otherwise, return the match found:
    return index;
}

//////////////////////////////////////////////////////////////////////
//  EOF: misc.cpp
//////////////////////////////////////////////////////////////////////

