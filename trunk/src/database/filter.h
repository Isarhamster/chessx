/***************************************************************************
                          filter.h  -  holds search results in memory
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005 Ejner Borgbjerg
                           <ejner@users.sourceforge.net>
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

#ifndef __FILTER_H__
#define __FILTER_H__
#include "common.h"
#include <QBitArray>
#include "database.h"

#include "search.h"
#include "query.h"
#include "tristatetree.h"

class Database;

/**
   The Filter class represents a set of games, typically found when searching a database.
*/

class Filter
{
public:
  /** Flag for which games to include in filter */
  enum WhichGames { AllGames, NoGames };
  /** Construct filter of given size */
  Filter(int size, WhichGames whichGames = AllGames);
  /** Construct filter of given size */
  Filter(Database* database, WhichGames whichGames = AllGames);
  /** Construct filter from another filter */
  Filter(const Filter& filter);
  /** Construct filter from a bit array */
  Filter(const QBitArray& bitArray);
  /** Assignment operator. */
  Filter operator=(const Filter& filter);
  /** Destructor */
  ~Filter();
  /** Include a game in the filter. */
  void add(int game);
  /** Remove a game from the filter. */
  void remove(int game);
  /** Set pointer to the database for which the filter is */
  void setDatabase(Database* database);
  /** @return a pointer to the database on which the filter is */
  Database* database();
  /** Set the value for a game */
  void set(int game, bool value);
  /** Set all games in the filter to the same value */
  void setAll(bool value);
  /** @return true if the game is in the filter */
  bool contains(int game) const;
  /** @return the index of nth game in the filter (zero based)*/ 
  int gameIndex(int nth) const;
  /** @return number of games in the filter */
  int count() const;
  /** @return the size of the filter */
  int size() const;
  /** Resize the filter to the specified size */
  void resize(int size, Filter::WhichGames whichGames = AllGames);
  /** Reverse the filter (complement set) */
  void reverse();
  /** Intersect filter with another filter */
  void intersect(const Filter& filter);
  /** Add the games of another filter to the filter */
  void add(const Filter& filter);
  /** Remove the games of another filter from the filter */
  void remove(const Filter& filter);
  /** @return the filter as a bit array (returns copy in Qt3 due, implicitly shared in Qt4)*/
  QBitArray asBitArray() const;

  /** Executes search 'search' on database m_database, 
     and sets this filter to contain the results */
   void executeSearch(const Search& search);
   /** Executes search 'search' on database m_database,
      and modifies this filter with the results */
   void executeSearch(const Search& search, Search::Operator searchOperator);
   /** Executes query 'query' on database m_database,
       and sets this filter to contain the results */
   void executeQuery(Query& query);
   /** Set the state for leaf 'leaf' in m_triStateTree and return the state of the tree. */
   TriStateTree::State setState(int leaf, TriStateTree::State state);
   /** Set the state for leaf 'leaf' in m_triStateTree and return the state of the tree. */
   TriStateTree::State setState(int leaf, bool state);
   /** @return the state of the @p m_triStateTree . */
   TriStateTree::State state() const;
   /** @return the state of leaf 'leaf' in @p m_triStateTree . */
   TriStateTree::State state(int leaf) const;

protected:
  /** returns the filter as a implicitely shared QBitArray */
  QBitArray bitArray() const;

  int m_count;
  QBitArray* m_bitArray;

  // for the speeding up of sequential access
  mutable int m_lastNth;
  mutable int m_lastIndex;
  Database* m_database;
  TriStateTree m_triStateTree; 

};

#endif

