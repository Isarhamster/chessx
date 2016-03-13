/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __PARTIALDATE_H__
#define __PARTIALDATE_H__

#include <QString>
#include <QDateTime>

/** @ingroup Database
   The PartialDate class represents a date, perhaps with missing month and day.
   All comparison operators should work.
*/

class PartialDate
{
public:
    enum {Year = 1, Month = 2, Day = 4, All = Year | Month | Day};
    /** Constructor */
    PartialDate(int y = 0, int m = 0, int d = 0);
    /** String constructor. Creates date from PGN date format (e.g. "1990.01.??"). */
    explicit PartialDate(const QString& s);
    /** QDate constructor. */
    explicit PartialDate(const QDate& d);
    /** Converts date to QDate. Undefined parts will be replaced with 0 for year, 1 for month and day */
    QDate asDate() const;
    /** Converts date to string. Uses PGN date format (e.g. "1990.01.??"). */
    QString asString() const;
    /** Converts date to string. Uses short format (e.g. "1990", "1990.01", "1990.01.15").
    Optionally saves just a part of date. */
    QString asShortString(int part = All) const;

    static PartialDate today();
    /** @return year, @p 0 if undefined. */
    int year() const;
    /** @return month, @p 0 if undefined. */
    int month() const;
    /** @return day, @p 0 if undefined. */
    int day() const;

    /** Sets date from string in PGN date format (e.g. "1990.01.??"). */
    PartialDate& fromString(const QString& s);
    /** @return formatted date range (e. g. "1990.01.12-02.13", "1992-1997.11.12") */
    QString range(const PartialDate& d) const;
    /** Test if PartialDate is valid */
    bool isValid() const;

    PartialDate(const PartialDate& rhs)
    {
        *this = rhs;
    }

    PartialDate& operator= (const PartialDate& rhs)
    {
        if(this != &rhs)
        {
            m_bIsValid = rhs.m_bIsValid;
            m_year     = rhs.m_year;
            m_month    = rhs.m_month;
            m_day      = rhs.m_day;
        }
        return *this;
    }

    friend bool operator==(const PartialDate& d1, const PartialDate& d2);
    friend bool operator>=(const PartialDate& d1, const PartialDate& d2);
    friend bool operator<=(const PartialDate& d1, const PartialDate& d2);
    friend bool operator<(const PartialDate& d1, const PartialDate& d2);
    friend bool operator>(const PartialDate& d1, const PartialDate& d2);
    friend bool operator!=(const PartialDate& d1, const PartialDate& d2);

private:
    short int m_year;
    unsigned char m_month;
    unsigned char m_day;
    bool m_bIsValid;
    QString numberToString(int d, QChar fill = '0') const;
};

const PartialDate PDMaxDate(9999);
const PartialDate PDMinDate(1);
const PartialDate PDInvalidDate(0, 0, 0);

#endif
