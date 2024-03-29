/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "partialdate.h"
#include <QRegularExpression>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

PartialDate::PartialDate(int y, int m, int d) :
    m_year(y),
    m_month(m),
    m_day(d)
{
    m_bIsValid = !m_year || asDate().isValid();
}

PartialDate::PartialDate(const QString& s)
{
    m_year = 0;
    m_month = 0;
    m_day = 0;
    fromString(s);
}

PartialDate::PartialDate(const QDate& d)
{
    m_bIsValid = true;
    m_year = d.year();
    m_month = d.month();
    m_day = d.day();
}

PartialDate PartialDate::today()
{
    return PartialDate(QDate::currentDate());
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

PartialDate& PartialDate::fromString(const QString& s)
{
    m_bIsValid = false;
    QString test = s.trimmed();
    QRegularExpression regExp("^[\\?0-9]{4}([\\.\\/-]([\\?0-9]){1,2}){0,2}$");
    QRegularExpression sep("[\\./-]");
    if (test.isEmpty() || test == "?")
    {
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_bIsValid = true;
    }
    else if (test.indexOf(regExp)==0)
    {
        m_year = test.section(sep, 0, 0).toInt();
        m_month = test.section(sep, 1, 1).toInt();
        m_day = test.section(sep, 2, 2).toInt();
        m_bIsValid = !year() || asDate().isValid();
    }
    else
    {
        QRegularExpression regExpContinental("^(([\\?0-9]){1,2}[\\.\\/-]){0,2}[\\?0-9]{4}$");
        if(test.indexOf(regExpContinental)==0)
        {
            m_day = test.section(sep, 0, 0).toInt();
            m_month = test.section(sep, 1, 1).toInt();
            m_year = test.section(sep, 2, 2).toInt();
            m_bIsValid = !year() || asDate().isValid();
        }
        else
        {
            QRegularExpression regExpAmerican("^(([\\?0-9]){1,2}\\/){0,2}[\\?0-9]{4}$");
            if(test.indexOf(regExpAmerican)==0)
            {
                m_month = test.section(sep, 0, 0).toInt();
                m_day = test.section(sep, 1, 1).toInt();
                m_year = test.section(sep, 2, 2).toInt();
                m_bIsValid = !year() || asDate().isValid();
            }
        }
    }
    return *this;
}


QDate PartialDate::asDate() const
{
    return QDate(m_year, m_month ? m_month : 1, m_day ? m_day : 1);
}

bool PartialDate::isValid() const
{
    return m_bIsValid;
}

QString PartialDate::asString() const
{
    if(!m_year)
    {
        return "????.??.??";
    }
    QString s = QString("%1.%2.%3")
            .arg(m_year, 4)
            .arg(numberToString(m_month), numberToString(m_day));
    return s;
}

QString PartialDate::asShortString(int part) const
{
    if(!m_year)
    {
        return QString();
    }
    QString s;
    if(part & Year)
    {
        s = numberToString(m_year);
    }
    if(!m_month)
    {
        return s;
    }
    if(part & Month)
    {
        if(!s.isEmpty())
        {
            s += '.';
        }
        s += numberToString(m_month);
    }
    if(!m_day)
    {
        return s;
    }
    if(part & Day)
    {
        if(!s.isEmpty())
        {
            s += '.';
        }
        s += numberToString(m_day);
    }
    return s;
}

QString PartialDate::numberToString(int d, QChar fill) const
{
    if(!d)
    {
        return "??";
    }
    return d < 10 ? QString(fill) + QString::number(d) : QString::number(d);
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
    if(year() != d.year())
    {
        return asShortString() + "-" + d.asShortString();
    }
    if(month() != d.month())
        return QString("%1.%2-%3").arg(year()).arg(asShortString(Month | Day), d.asShortString(Month | Day));
    else if(day() != d.day())
        return QString("%1.%2.%3-%4").arg(year()).arg(month())
               .arg(asShortString(Day), d.asShortString(Day));
    else
    {
        return asShortString();
    }
}

