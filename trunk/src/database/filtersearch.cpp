/***************************************************************************
                          search.h  -  Holds different types of searches
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

#include "filtersearch.h"

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
