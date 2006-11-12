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

#ifndef __FILTERSEARCH_H__
#define __FILTERSEARCH_H__

#include "filter.h"

/** Defined filter based search */
class FilterSearch : public Search
{
   public:
      FilterSearch();
      FilterSearch(const Filter& filter);
      ~FilterSearch();
      Type type() const;
      
      bool contains(int game) const;
      Filter filter() const;
      void setFilter(const Filter& filter);
   private:
      Filter m_filter;
}; 

#endif
