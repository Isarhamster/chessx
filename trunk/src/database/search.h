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

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "board.h"
#include "partialdate.h"

/** The Search class is an abstract base class that represents a 
 * search on one criteria.
 */
struct Search 
{
   enum Type { NullSearch, PositionSearch, EloSearch, DateSearch }; //1 per subclass to allow static downcast
   enum Operator { NullOperator, Not, And, Or, Add, Remove }; //Add is effectively the same as Or

   Search();
   virtual ~Search()=0;
   virtual Type type() const = 0;
};

/** Undefined search */
class NullSearch : public Search
{
   public :
      NullSearch();
      virtual ~NullSearch();
      virtual Type type() const;
};
/** Defines a search for a given position */
class PositionSearch : public Search
{
   public :
      PositionSearch(Board& position);
      virtual ~PositionSearch();
      virtual Search::Type type() const;
      Board position() const;
      void setPosition(Board& position);
   private :
      Board m_position;
};
/** Defines a search based on the elo ratings of both players */
class EloSearch : public Search
{
   public :
      EloSearch(int minWhiteElo=0, int maxWhiteElo=4000, int minBlackElo=0, int maxBlacElo=4000);
      virtual ~EloSearch();
      virtual Type type() const;
      int minWhiteElo() const;
      int maxWhiteElo() const;
      int minBlackElo() const;
      int maxBlackElo() const;
      void setEloSearch(int minWhiteElo=0, int maxWhiteElo=4000, int minBlackElo=0, int maxBlacElo=4000);

   private :
      int m_minWhiteElo;
      int m_maxWhiteElo;
      int m_minBlackElo;
      int m_maxBlackElo;
};

/** Defines a search based on a date range */
class DateSearch : public Search
{
	public:
		DateSearch();
		DateSearch(PartialDate minDate, PartialDate maxDate);
		~DateSearch();
		Type type() const;
		
		PartialDate minDate();
		PartialDate maxDate();
		bool withinDateRange(PartialDate date);
		void setDateRange(PartialDate minDate, PartialDate maxDate);
		void setMinDate(PartialDate minDate);
		void setMaxDate(PartialDate maxDate);
	
	private:
		PartialDate m_minDate;
		PartialDate m_maxDate;
};

#endif // __SEARCH_H__
