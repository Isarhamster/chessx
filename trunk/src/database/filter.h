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

#include <QBitArray>

#include "common.h"
#include "search.h"
#include "query.h"
#include "tristatetree.h"

class Database;

/**
   The Filter class represents a set of games. It is always associated with
   some Database object. On creation it has the same size as database,
   but it is not automatically resized when database size changes.
*/

class Filter
{
public:
  /** Possible operations on filter. */
  enum Operator {And, Or, Xor, Minus};
  /** Construct filter of given size. Add all games to the filter. */
  Filter(Database* database);
  /** Construct filter from another filter. */
  Filter(const Filter& filter);
  /** Assignment operator. */
  Filter operator=(const Filter& filter);
  /** Destructor. */
  ~Filter();
  /** @return a pointer to the database on which the filter is. */
  Database* database();
  /** Add or remove game @p game . Does nothing if the game is not in filter. */
  void set(int game, bool value);
  /** Set all games in the filter to the same value. */
  void setAll(bool value);
  /** @return true if the game is in the filter. */
  bool contains(int game) const;
  /** @return number of games in the filter. */
  int count() const;
  /** @return the size of the filter. */
  int size() const;
  /** @return first game in the filter or @p -1 if filter is empty. */
  int firstGame() const;
  /** @return last game in the filter or @p -1 if filter is empty. */
  int lastGame() const;
  /** @return next game in the filter or @p -1 if there is none. */
  int nextGame(int current) const;
  /** @return previous game in the filter or @p -1 if there is none. */
  int previousGame(int current) const;
  /** Resize the filter to the specified size. Keeps the current filter content.
  If the filter is increased, added game will be initialized to @p false (not in filter). */
  void resize(int newsize);
  /** Reverse the filter (complement set). */
  void reverse();
  /** Join filter with another filter, using one of possible operators @p op .
  Does nothing if filters have different sizes. */
  void join(const Filter& filter, Operator op);
  /** Executes search 'search' on database m_database,
     and sets this filter to contain the results. */
  void executeSearch(const Search& search);
   /** Executes search 'search' on database m_database,
      and modifies this filter with the results. */
   void executeSearch(const Search& search, Search::Operator searchOperator);
   /** Executes query 'query' on database m_database,
       and sets this filter to contain the results. */
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
  /** returns the filter as a implicitely shared QBitArray. */
  QBitArray bitArray() const;

  int m_count;
  QBitArray* m_bitArray;

  Database* m_database;
  TriStateTree m_triStateTree; 

};

#endif

