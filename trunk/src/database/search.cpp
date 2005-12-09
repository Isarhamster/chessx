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
Search::Type NullSearch::type()
{
   return Search::NullSearch;
}

/** The Position Search Class
 * ******************************/
PositionSearch::PositionSearch()
{
}
PositionSearch::~PositionSearch()
{
}
Search::Type PositionSearch::type()
{
   return Search::PositionSearch;
}
Board PositionSearch::position()
{
   return Board();
}

/** The EloSearch class
 * **********************/
EloSearch::EloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlacElo)
{
   m_minWhiteElo = minWhiteElo;
   m_maxWhiteElo = maxWhiteElo;
   m_minBlackElo = minBlackElo;
   m_maxBlackElo = maxBlacElo;
}
EloSearch::~EloSearch()
{
}
Search::Type EloSearch::type()
{
   return Search::EloSearch;
}
int EloSearch::maxWhiteElo()
{
   return m_maxWhiteElo;
}
int EloSearch::minWhiteElo()
{
   return m_minWhiteElo;
}
int EloSearch::maxBlackElo()
{
   return m_maxBlackElo;
}
int EloSearch::minBlackElo()
{
   return m_minBlackElo;
}
