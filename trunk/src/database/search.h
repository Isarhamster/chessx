/***************************************************************************
                          search.h  -  Holds different types of searches
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

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "board.h"

/** The Search class is an abstract base class that represents a 
 * search on one criteria.
 */
struct Search 
{
   enum Type { NullSearch, PositionSearch, EloSearch }; //1 per subclass to allow static downcast
   enum Operator { NullOperator, Not, And, Or, Add, Remove }; //Add is effectively the same as Or

   Search();
   virtual ~Search()=0;
   virtual Type type()=0;
};

/** Undefined search */
class NullSearch : public Search
{
   public :
      NullSearch();
      virtual ~NullSearch();
      virtual Type type();
};
/** Defines a search for a given position */
class PositionSearch : public Search
{
   public :
      PositionSearch(Board& position);
      virtual ~PositionSearch();
      virtual Search::Type type();
      Board position();
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
      virtual Type type();
      int minWhiteElo();
      int maxWhiteElo();
      int minBlackElo();
      int maxBlackElo();
      void setEloSearch(int minWhiteElo=0, int maxWhiteElo=4000, int minBlackElo=0, int maxBlacElo=4000);

   private :
      int m_minWhiteElo;
      int m_maxWhiteElo;
      int m_minBlackElo;
      int m_maxBlackElo;
};

#endif // __SEARCH_H__
