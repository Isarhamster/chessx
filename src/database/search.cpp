/***************************************************************************
                          search.cpp  -  Holds different types of searches
                             -------------------
    begin                : 06/12/2005
    copyright            : (C) 2005 Marius Roets
                           <saidinwielder@users.sourceforge.net>
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
PositionSearch::PositionSearch(Board& position)
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
void PositionSearch::setPosition(Board& position)
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
