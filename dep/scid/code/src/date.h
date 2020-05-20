//////////////////////////////////////////////////////////////////////
//
//  FILE:       date.h
//              Date format and inline date functions.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.9
//
//  Notice:     Copyright (c) 1999  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_DATE_H
#define SCID_DATE_H

#include "common.h"
#include <algorithm>
#include <cstdlib>

// DATE STORAGE FORMAT:
// In memory, dates are stored in a 32-bit (4-byte) uint, of which only
// the lowest 3 bytes need be used, with the lowest 5 bits for the
// day, the next highest 4 bits for the month and the highest bits for
// the year. This makes date comparisons easy: a bigger date value is
// a more recent date.  If a field is unknown, its value is set to zero.
// On disk, the date is stored in 3 bytes.


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CONSTANTS and MACROS:

const dateT ZERO_DATE = 0;

const uint  YEAR_SHIFT  = 9;
const uint  MONTH_SHIFT = 5;
const uint  DAY_SHIFT   = 0;

// DAY (31 days) 5 bits (32) , MONTH (12 months) 4 bits (16)

const uint YEAR_MAX = 2047;  // 2^11 - 1

#define DATE_MAKE(y,m,d) (((y) << YEAR_SHIFT) | ((m) << MONTH_SHIFT) | (d))


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// date_GetYear():
//      Get the year from a date.
inline uint
date_GetYear (dateT date)
{
    return (uint) (date >> YEAR_SHIFT);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// date_GetMonth():
//      Get the month from a date.
inline uint
date_GetMonth (dateT date)
{
    return (uint) ((date >> MONTH_SHIFT) & 15);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### date_GetDay():
//      Get the day of the month from a date.
inline uint
date_GetDay (dateT date)
{
    return (uint) (date & 31);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//### date_GetMonthDay():
//      Get the month and day; used to check for year-only dates. S.A
inline uint
date_GetMonthDay (dateT date)
{
    return (uint) (date & 511);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// date_DecodeToString(): convert date to PGN tag string.
inline void
date_DecodeToString (dateT date, char * str)
{
    ASSERT(str != NULL);
    uint year, month, day;

    year = date_GetYear (date);
    month = date_GetMonth (date);
    day = date_GetDay (date);

    if (year == 0) {
        *str++ = '?'; *str++ = '?'; *str++ = '?'; *str++ = '?';
    } else {
        *str++ = '0' + (year / 1000);
        *str++ = '0' + (year % 1000) / 100;
        *str++ = '0' + (year % 100) / 10;
        *str++ = '0' + (year % 10);
    }
    *str++ = '.';

    if (month == 0) {
        *str++ = '?'; *str++ = '?';
    } else {
        *str++ = '0' + (month / 10);
        *str++ = '0' + (month % 10);
    }
    *str++ = '.';

    if (day == 0) {
        *str++ = '?'; *str++ = '?';
    } else {
        *str++ = '0' + (day / 10);
        *str++ = '0' + (day % 10);
    }
    *str = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// date_EncodeFromString(): convert PGN tag string to date.
//      The date string format is: "yyyy.mm.dd".
inline dateT
date_EncodeFromString (const char * str)
{
    // Do checks on str's validity as a date string:
    ASSERT(str != NULL);

    dateT date;
    uint year, month, day;

    // convert year:
    year = std::strtoul(str, NULL, 10);
    if (year > YEAR_MAX) { year = 0; }
    date = year << YEAR_SHIFT;
    while (*str != 0  &&  *str != '.') { str++; }
    if (*str == '.') { str++; }

    // convert month:
    month = std::strtoul(str, NULL, 10);
    if (month > 12) { return date; }
    date |= (month << MONTH_SHIFT);
    while (*str != 0  &&  *str != '.') { str++; }
    if (*str == '.') { str++; }

    // convert day:
    day = std::strtoul(str, NULL, 10);
    if (day > 31) { return date; }
    date |= (day << DAY_SHIFT);

    return date;
}

/**
 * Creates a dateT object from a PGN tag value string.
 * "The Date tag value field always uses a standard ten character format:
 * "YYYY.MM.DD". If the any of the digit fields are not known, then question
 * marks are used in place of the digits."
 * @param str: pointer to the memory containing the tag value.
 * @param len: length of the tag value.
 * @returns the dateT object corresponding to @e str.
 */
inline dateT date_parsePGNTag(const char* str, size_t len) {
	char tmp[10] = {0};
	std::transform(str, str + std::min<size_t>(len, 10), tmp, [](char ch) {
		return (ch >= '0' && ch <= '9') ? ch - '0' : 0;
	});

	uint32_t year = tmp[0] * 1000 + tmp[1] * 100 + tmp[2] * 10 + tmp[3];
	uint32_t month = tmp[5] * 10 + tmp[6];
	uint32_t day = tmp[8] * 10 + tmp[9];

	if (year > YEAR_MAX)
		year = 0;

	if (month > 12)
		month = 0;

	constexpr unsigned char days[] = {31, 31, 28, 31, 30, 31, 30,
	                                  31, 31, 30, 31, 30, 31};
	if (day > days[month]) {
		if (day != 29 || year % 4 || (year % 100 == 0 && year % 400)) {
			day = 0;
		}
	}

	return (year << YEAR_SHIFT) | (month << MONTH_SHIFT) | (day << DAY_SHIFT);
}

inline dateT date_parsePGNTag(std::pair<const char*, const char*> str) {
	return date_parsePGNTag(str.first, std::distance(str.first, str.second));
}

#endif   // #ifndef SCID_DATE_H

//////////////////////////////////////////////////////////////////////
//  EOF: date.h
//////////////////////////////////////////////////////////////////////

