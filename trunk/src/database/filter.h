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
#include <qbitarray.h>
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
	/** flag for which games to include in filter */
	enum WhichGames { AllGames, NoGames };
	
	/** construct filter of given size */
	Filter(int size, WhichGames whichGames = AllGames);
	
	/** construct filter of given size */
	Filter(Database* database, WhichGames whichGames = AllGames);

	/** construct filter from another filter */
	Filter(const Filter& filter);

	/** construct filter from a bit array */
	Filter(const QBitArray& bitArray);
	
	/** assignment operator */
	Filter operator=(const Filter& filter);

	/** destructor */
	~Filter();
	
	/** include a game in the filter */
	void add(int game);
	
	/** remove a game from the filter */
	void remove(int game);

   /** Sets pointer to the database for which the filter is */
	void setDatabase(Database* database);
	
	/** sets the value for a game */
	void set(int game, bool value);
	
	/** sets all games in the filter to the same value */
	void setAll(bool value);

	/** return true if the game is in the filter */
	bool contains(int game) const;
	
	/** returns the index of nth game in the filter (zero based)*/ 
	int gameIndex(int nth) const;

	/** return number of games in the filter */
	int count() const;
	
	/** return the size of the filter */
	int size() const;
	
	/** resizes the filter to the specified size */
	void resize(int size, Filter::WhichGames whichGames = AllGames);
	
	/** reverse the filter (complement set) */
	void reverse();
	
	/** intersect filter with another filter */
	void intersect(const Filter& filter);
	
	/** add the games of another filter to the filter */
	void add(const Filter& filter);
	
	/** remove the games of another filter from the filter */
	void remove(const Filter& filter);
	
	/** returns the filter as a bit array (returns copy in Qt3 due, implicitly shared in Qt4)*/
	QBitArray asBitArray() const;

   /* Executes search 'search' on database m_database, 
    * and sets this filter to contain the results */
   void executeSearch(const Search& search);
   /* Executes search 'search' on database m_database, 
    * and modifies this filter with the results */
   void executeSearch(const Search& search, Search::Operator searchOperator);
   /* Executes query 'query' on database m_database, 
    * and sets this filter to contain the results */
   void executeQuery(Query& query);

   /** Sets the state for leaf 'leaf' in m_triStateTree and return the state of the tree */
   TriStateTree::State setState(int leaf, TriStateTree::State state);
   /** Sets the state for leaf 'leaf' in m_triStateTree and return the state of the tree */
   TriStateTree::State setState(int leaf, bool state);
   /** Returns the state of the m_triStateTree */
   TriStateTree::State state() const;
   /** Returns the state of leaf 'leaf' in m_triStateTree */
   TriStateTree::State state(int leaf) const;
	
protected:
	/** returns the filter as a bit array (explicitly shared in Qt3, implicitly shared in Qt4)*/
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

