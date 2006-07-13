/***************************************************************************
                          search.cpp  -  Holds different types of searches
                             -------------------
    begin                : 06/12/2005
    copyright            : (C) 2005 Marius Roets
                           <saidinwielder@users.sourceforge.net>
													 (C) 2006 William Hoggarth
													 <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "search.h"

/** The Search Abstract base clase
 * ********************************/
Search::Search()
{
}
Search::~Search()
{
}

/** The NullSearch Class
 * *********************/
NullSearch::NullSearch()
{
}
NullSearch::~NullSearch()
{
}
Search::Type NullSearch::type() const
{
   return Search::NullSearch;
}

/** The Position Search Class
 * ******************************/
PositionSearch::PositionSearch()
{
}
PositionSearch::PositionSearch(const Board& position)
{
   setPosition(position);
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
   m_position.fromFEN(position.toFEN());
}

/** The EloSearch class
 * **********************/
EloSearch::EloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlacElo)
{
   setEloSearch(minWhiteElo, maxWhiteElo, minBlackElo, maxBlacElo);
}
EloSearch::~EloSearch()
{
}
void EloSearch::setEloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlacElo)
{
   m_minWhiteElo = minWhiteElo;
   m_maxWhiteElo = maxWhiteElo;
   m_minBlackElo = minBlackElo;
   m_maxBlackElo = maxBlacElo;
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
bool EloSearch::withinEloRange(int whiteElo, int blackElo) const
{
	return whiteElo >= m_minWhiteElo && whiteElo <= m_maxWhiteElo && blackElo >= m_minBlackElo && blackElo <= m_maxBlackElo;
}

/** The DateSearch class
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

DateSearch::~DateSearch()
{
}

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

bool DateSearch::withinDateRange(PartialDate date) const
{
	return (date >= m_minDate && date <= m_maxDate);
}

void DateSearch::setDateRange(PartialDate minDate, PartialDate maxDate)
{
	Q_ASSERT(minDate < maxDate);
	
	m_minDate = minDate;
	m_maxDate = maxDate;
}

void DateSearch::setMinDate(PartialDate minDate)
{
	Q_ASSERT(minDate < m_maxDate);
	m_minDate = minDate;
}

void DateSearch::setMaxDate(PartialDate maxDate)
{
	Q_ASSERT(maxDate > m_minDate);
	m_maxDate = maxDate;
}

/** The TagSearch class
 * **********************/
TagSearch::TagSearch(const QString& tag, const QString& value)
{
	m_tag = tag;
	m_value = value;
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
	return m_tag;
}

QString TagSearch::value() const
{
	return m_value;
}

void TagSearch::setTag(const QString& tag)
{
	m_tag = tag;
}

void TagSearch::setValue(const QString& value)
{
	m_value = value;
}

/** The FilterSearch class
 * **********************/
FilterSearch::FilterSearch() : m_filter(0)
{
}

FilterSearch::FilterSearch(const Filter& filter) : m_filter(filter)
{
}

FilterSearch::~FilterSearch()
{
}

Search::Type FilterSearch::type() const
{
	return Search::FilterSearch;
}
		
bool FilterSearch::contains(int game) const
{
	return m_filter.contains(game);
}
		
Filter FilterSearch::filter() const
{
	return m_filter;
}

void FilterSearch::setFilter(const Filter& filter)
{
	m_filter = filter;
}
