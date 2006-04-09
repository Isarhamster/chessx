/***************************************************************************
                          database.h  -  abstract database class
                             -------------------
    begin                : 8 December 2005
    copyright            : (C) 2006 William Hoggarth 
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
 
#include "filter.h" 
#include "game.h"
#include "search.h"
#include "query.h"

/**
   The Database class is abstract, providing a common interface for all
   database types. There are methods for the loading and saving of games,
   and for performing searches and queries.
*/

class Database
{
	public:	
		/** Virtual destructor */
		virtual ~Database();
		//game retrieval & storage
		/** Loads and returns the game at the given position */
		virtual Game load(int game) = 0;
		/** Saves a game at the given position */
		virtual void save(int game, Game& game) = 0;
		/** Adds a game to the database */
		virtual void add(Game& game) = 0;
		/** Removes a game from the database */
		virtual void remove(int game) = 0;
		/** Removes multiple games from the database as specified by the filter */
		virtual void remove(const Filter& filter) = 0;
		/** Compacts the database */
		virtual void compact() = 0;
		/** Returns the number of games in the database */
		virtual int count() = 0;
		
		//capability enquiry (so we can start using a format before every last search type is supported)
		/** Checks if the database supports the given type of search */
		virtual bool supportedSearchType(Search::Type searchType) = 0;
		
		//basic search execution
		/** Executes a basic search, and returns the results in a filter */
		virtual Filter executeSearch(const Search& search) = 0;
		/** Executes a basic search combined with a filter */
		virtual Filter executeSearch(const Search& search, Search::Operator searchOperator, Filter filter) = 0;
		
		//complex query execution
		/** Executes a query, and returns the results in a filter */
		virtual Filter executeQuery(Query& query) = 0;
		/** Executes a query combined with a filter */
		virtual Filter executeQuery(const Query& query, Search::Operator searchOperator, Filter filter) = 0; 
};
