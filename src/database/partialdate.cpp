/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "partialdate.h"

PartialDate::PartialDate() : m_year(0), m_month(0), m_day(0)
{}

PartialDate::PartialDate(int y) : m_year(y), m_month(0), m_day(0)
{}

PartialDate::PartialDate(int y, int m) : m_year(y), m_month(m), m_day(0)
{}

PartialDate::PartialDate(int y, int m, int d) : m_year(y), m_month(m), m_day(d)
{}

PartialDate::PartialDate(const QString& s)
{
	fromString(s);
}

PartialDate::PartialDate(const QDate& d)
{
	m_year = d.year();
	m_month = d.month();
	m_day = d.day();
}


bool PartialDate::isFull() const
{
	return m_year && m_month && m_day;
}

int PartialDate::year() const
{
	return m_year;
}

int PartialDate::month() const
{
	return m_month;
}

int PartialDate::day() const
{
	return m_day;
}

void PartialDate::setYear(int y)
{
	if (y >= 0)
		m_year = y;
}

void PartialDate::setMonth(int m)
{
	if (m >= 0 && m <= 12)
		m_month = m;
}

void PartialDate::setDay(int d)
{
	if (d >= 0 && d <= 31)
		m_day = d;
}

void PartialDate::fromString(const QString& s)
{
	m_year = s.section('.', 0, 0).toInt();
	m_month = s.section('.', 1, 1).toInt();
	m_day = s.section('.', 2, 2).toInt();
}


QDate PartialDate::asDate() const
{
	return QDate(m_year, m_month, m_day);
}

QString PartialDate::asString() const
{
	if (!m_year)
		return "????.??.??";
	QString s = QString("%1.%2.%3").arg(m_year, 4).arg(numberToString(m_month))
		    .arg(numberToString(m_day));
	return s;
}

QString PartialDate::asShortString(int part) const
{
	if (!m_year)
		return QString();
	QString s;
	if (part & Year)
		s = numberToString(m_year, 4);
	if (!m_month)
		return s;
	if (part & Month) {
		if (!s.isEmpty()) s += '.';
		s += numberToString(m_month);
	}
	if (!m_day)
		return s;
	if (part & Day) {
		if (!s.isEmpty()) s += '.';
		s += numberToString(m_day);
	}
	return s;
}

QString PartialDate::numberToString(int d, QChar fill) const
{
	if (!d)
		return QString(fill) + fill;
	return d < 10 ? fill + QString::number(d) : QString::number(d);
}

bool operator==(const PartialDate& d1, const PartialDate& d2)
{
	return d1.year() == d2.year() && d1.month() == d2.month() && d1.day() == d2.day();
}

bool operator<(const PartialDate& d1, const PartialDate& d2)
{
	return d2.year() > d1.year() ||
	       ((d2.year() == d1.year()) &&
		(d2.month() > d1.month() || (d2.month() == d1.month() && d2.day() > d1.day())));
}

bool operator>(const PartialDate& d1, const PartialDate& d2)
{
	return d1.year() > d2.year() ||
	       ((d1.year() == d2.year()) &&
		(d1.month() > d2.month() || (d1.month() == d2.month() && d1.day() > d2.day())));
}

bool operator>=(const PartialDate& d1, const PartialDate& d2)
{
	return d2 < d1 || d2 == d1;
}

bool operator<=(const PartialDate& d1, const PartialDate& d2)
{
	return d2 > d1 || d2 == d1;
}

bool operator!=(const PartialDate& d1, const PartialDate& d2)
{
	return d1.year() != d2.year() || d1.month() != d2.month() || d1.day() != d2.day();
}

QString PartialDate::range(const PartialDate& d) const
{
	if (year() != d.year())
		return asShortString() + "-" + d.asShortString();
	QString result = numberToString(year());
	if (month() != d.month())
		return QString("%1.%2-%3").arg(year()).arg(asShortString(Month | Day))
				.arg(d.asShortString(Month | Day));
	else if (day() != d.day())
		return QString("%1.%2.%3-%4").arg(year()).arg(month())
				.arg(asShortString(Day)).arg(d.asShortString(Day));
	else return asShortString();
}

