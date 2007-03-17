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
#include "database.h"
#include "filter.h"
#include <QtDebug>

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
bool NullSearch::matches(int )
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
   m_position.fromFEN(position.toFEN());
}
bool PositionSearch::matches(int index)
{
   m_database->loadGame(index, m_game);
   m_game.moveToStart();
   while (!m_game.atEnd()) {
      if (m_game.board() == m_position) {
         return true;
      }
      m_game.forward();
   }
   // Check the end position
   if (m_game.board() == m_position) {
      return true;
   }

   return false;
}

/* EloSearch class
 * **********************/
EloSearch::EloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlacElo)
{
   setEloSearch(minWhiteElo, maxWhiteElo, minBlackElo, maxBlacElo);
}
EloSearch* EloSearch::clone() const
{
	return new EloSearch(*this);
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
bool EloSearch::matches(int index)
{
   m_database->loadGameHeaders(index, m_game);
   int whiteElo = m_game.tag("WhiteElo").toInt(); 
   int blackElo = m_game.tag("BlackElo").toInt();
	return whiteElo >= m_minWhiteElo && whiteElo <= m_maxWhiteElo && blackElo >= m_minBlackElo && blackElo <= m_maxBlackElo;
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
void DateSearch::setDateRange(PartialDate minDate, PartialDate maxDate)
{
	Q_ASSERT(minDate < maxDate);
	m_minDate = minDate;
	m_maxDate = maxDate;
}
bool DateSearch::matches(int index)
{
   m_database->loadGameHeaders(index, m_game);
   PartialDate date(m_game.tag("Date"));

	return (date >= m_minDate && date <= m_maxDate);
}

/* TagSearch class
 * ***************/
TagSearch::TagSearch(Database* database, const QString& tag, const QString& value)
{
  m_database = database;
  m_tag = tag;
  m_value = value;
  //initialize();
}
void TagSearch::initialize()
{
   //m_matches = new QBitArray;
}
TagSearch* TagSearch::clone() const
{
	return new TagSearch(*this);
}
TagSearch::~TagSearch()
{
   //delete m_matches;
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
bool TagSearch::matches(int index)
{
   m_database->loadGameHeaders(index, m_game);
   return m_game.tag(m_tag).contains(m_value);
}

/* FilterSearch class
 * **********************/
FilterSearch::FilterSearch() : m_filter(0)
{
}
FilterSearch::FilterSearch(Filter* filter) : m_filter(filter)
{
}
FilterSearch* FilterSearch::clone() const
{
	return new FilterSearch(*this);
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
bool FilterSearch::matches(int index)
{
   return m_filter->contains(index);
}
