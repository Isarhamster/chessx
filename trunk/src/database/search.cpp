/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtCore>

#include "search.h"
#include "database.h"
#include "filter.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* Search abstract base clase
 * ********************************/
Search::Search()
{
}

Search::~Search()
{
}

void Search::setDatabase(Database* database)
{
    m_database = database;
}

/* NullSearch Class
 * *********************/
NullSearch::NullSearch()
{
}

NullSearch::~NullSearch()
{
}

NullSearch* NullSearch::clone() const
{
    return new NullSearch;
}

Search::Type NullSearch::type() const
{
    return Search::NullSearch;
}

int NullSearch::matches(GameId)
{
    return false;
}

/* PositionSearch Class
 * ******************************/
PositionSearch::PositionSearch()
{
}

PositionSearch::PositionSearch(Database* db, const Board& position)
{
    setPosition(position);
    m_database = db;
}

PositionSearch* PositionSearch::clone() const
{
    return new PositionSearch(*this);
}

PositionSearch::~PositionSearch()
{
}

Search::Type PositionSearch::type() const
{
    return Search::PositionSearch;
}

Board PositionSearch::position() const
{
    return m_position;
}

void PositionSearch::setPosition(const Board& position)
{
    m_position = position;
}

int PositionSearch::matches(GameId index)
{
    m_database->loadGameMoves(index, m_game);
    return (m_game.findPosition(m_position) != NO_MOVE);
}

/* EloSearch class
 * **********************/
EloSearch::EloSearch(Database* database, int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlackElo)
{
    m_database = database;
    setEloSearch(minWhiteElo, maxWhiteElo, minBlackElo, maxBlackElo);
    initialize();
}

EloSearch* EloSearch::clone() const
{
    return new EloSearch(*this);
}

EloSearch::~EloSearch()
{
}

void EloSearch::initialize()
{
    m_matches = m_database->index()->listInRange(TagNameWhiteElo, m_minWhiteElo, m_maxWhiteElo);
    m_matches &= m_database->index()->listInRange(TagNameBlackElo, m_minBlackElo, m_maxBlackElo);
}

void EloSearch::setEloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlackElo)
{
    m_minWhiteElo = minWhiteElo;
    m_maxWhiteElo = maxWhiteElo;
    m_minBlackElo = minBlackElo;
    m_maxBlackElo = maxBlackElo;
    initialize();
}

Search::Type EloSearch::type() const
{
    return Search::EloSearch;
}

int EloSearch::maxWhiteElo() const
{
    return m_maxWhiteElo;
}

int EloSearch::minWhiteElo() const
{
    return m_minWhiteElo;
}

int EloSearch::maxBlackElo() const
{
    return m_maxBlackElo;
}

int EloSearch::minBlackElo() const
{
    return m_minBlackElo;
}

int EloSearch::matches(GameId index)
{
    return m_matches[index];
}

/* DateSearch class
 * **********************/
DateSearch::DateSearch()
{
    m_minDate = m_maxDate = PartialDate();
}

DateSearch::DateSearch(PartialDate minDate, PartialDate maxDate)
{
    Q_ASSERT(minDate < maxDate);

    m_minDate = minDate;
    m_maxDate = maxDate;
}

DateSearch* DateSearch::clone() const
{
    return new DateSearch(*this);
}

DateSearch::~DateSearch()
{}

Search::Type DateSearch::type() const
{
    return Search::DateSearch;
}

PartialDate DateSearch::minDate() const
{
    return m_minDate;
}

PartialDate DateSearch::maxDate() const
{
    return m_maxDate;
}

void DateSearch::setDateRange(PartialDate minDate, PartialDate maxDate)
{
    Q_ASSERT(minDate < maxDate);
    m_minDate = minDate;
    m_maxDate = maxDate;
}

int DateSearch::matches(GameId index)
{
    m_database->loadGameHeaders(index, m_game);
    PartialDate date(m_game.tag("Date"));

    return (date >= m_minDate && date <= m_maxDate);
}

/* TagSearch class
 * ***************/
TagSearch::TagSearch(Database* database, const QString& tag, const QString& value, bool partial)
{
    m_database = database;
    m_tagName = tag;
    m_value = value;
    m_bPartial = partial;
    initialize();
}

TagSearch::TagSearch(Database* database, const QString& tag, const QString& value, const QString& value2)
{
    m_database = database;
    m_tagName = tag;
    m_value = value;
    m_value2 = value2;
    m_bPartial = false;
    m_matches = m_database->index()->listInRange(m_tagName, m_value, m_value2);
}

TagSearch::TagSearch(Database* database, const QString& tag, int value, int value2)
{
    m_database = database;
    m_tagName = tag;
    m_value = QString::number(value);
    m_value2 = QString::number(value2);
    m_bPartial = false;
    m_matches = m_database->index()->listInRange(m_tagName, value, value2);
}

void TagSearch::initialize()
{
    if(m_bPartial)
    {
        m_matches = m_database->index()->listPartialValue(m_tagName, m_value);
    }
    else
    {
        m_matches = m_database->index()->listContainingValue(m_tagName, m_value);
    }
}

TagSearch* TagSearch::clone() const
{
    return new TagSearch(*this);
}

TagSearch::~TagSearch()
{
}

Search::Type TagSearch::type() const
{
    return Search::TagSearch;
}

QString TagSearch::tag() const
{
    return m_tagName;
}

QString TagSearch::value() const
{
    return m_value;
}

QString TagSearch::minValue() const
{
    return m_value;
}

QString TagSearch::maxValue() const
{
    return m_value2;
}

void TagSearch::setTag(const QString& tag)
{
    m_tagName = tag;
    initialize();
}

void TagSearch::setValue(const QString& value)
{
    m_value = value;
    initialize();
}

int TagSearch::matches(GameId index)
{
    return m_matches[index];
}

/* Number class
 * ***************/
NumberSearch::NumberSearch(Database* database, GameId start, GameId end)
{
    m_database = database;
    setRange(start, end);
}

NumberSearch::NumberSearch(Database* database, const QString& range)
{
    m_database = database;
    setRange(range);
}

NumberSearch* NumberSearch::clone() const
{
    return new NumberSearch(*this);
}

Search::Type NumberSearch::type() const
{
    return Search::NumberSearch;
}

void NumberSearch::setRange(GameId start, GameId end)
{
    m_start = start - 1;
    m_end = end - 1;
}

void NumberSearch::setRange(const QString& range)
{
    int sep = range.indexOf('-');
    if(sep != -1)
    {
        m_start = range.left(sep).toInt() - 1;
        m_end = range.mid(sep + 1).toInt() - 1;
    }
    else
    {
        m_start = m_end = range.toInt() - 1;
    }
}

int NumberSearch::matches(GameId index)
{
    return index >= m_start && index <= m_end;
}




/* FilterSearch class
 * **********************/
FilterSearch::FilterSearch() : m_filter(0)
{}
FilterSearch::FilterSearch(Filter* filter) : m_filter(filter)
{}
FilterSearch* FilterSearch::clone() const
{
    return new FilterSearch(*this);
}
FilterSearch::~FilterSearch()
{}
Search::Type FilterSearch::type() const
{
    return Search::FilterSearch;
}
bool FilterSearch::contains(GameId game) const
{
    return m_filter->contains(game);
}
Filter* FilterSearch::filter() const
{
    return m_filter;
}
void FilterSearch::setFilter(Filter* filter)
{
    m_filter = filter;
}
int FilterSearch::matches(GameId index)
{
    return m_filter->contains(index);
}
