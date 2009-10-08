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
	/** Empty constructor. Date is undefined. */
	PartialDate();
	/** Partial constructor. Only year is known. */
	PartialDate(int y);
	/** Partial constructor. Only year and month are known. */
	PartialDate(int y, int m);
	/** Full constructor. All is known. */
	PartialDate(int y, int m, int d);
	/** String constructor. Creates date from PGN date format (e.g. "1990.01.??"). */
	PartialDate(const QString& s);
	/** QDate constructor. */
	PartialDate(const QDate& d);
	/** Converts date to QDate. Undefined parts will be replaced with 0 for year, 1 for month and day */
	QDate asDate() const;
	/** Converts date to string. Uses PGN date format (e.g. "1990.01.??"). */
	QString asString() const;
	/** Converts date to string. Uses short format (e.g. "1990", "1990.01", "1990.01.15").
	Optionally saves just a part of date. */
	QString asShortString(int part = All) const;
	/** @return @p true if all date parts are defined. */
	bool isFull() const;
	/** @return year, @p 0 if undefined. */
	int year() const;
	/** @return month, @p 0 if undefined. */
	int month() const;
	/** @return day, @p 0 if undefined. */
	int day() const;
	/** Sets year. */
	void setYear(int y);
	/** Sets month. Only partial checking is done (m = 0..12) */
	void setMonth(int m);
	/** Sets day. Only partial checking is done (d = 0..31), so entering incorrect date is possible. */
	void setDay(int d);
	/** Sets date from string in PGN date format (e.g. "1990.01.??"). */
	void fromString(const QString& s);
	/** @return formatted date range (e. g. "1990.01.12-02.13", "1992-1997.11.12") */
	QString range(const PartialDate& d) const;

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
	QString numberToString(int d, QChar fill = '0') const;
};


#endif
