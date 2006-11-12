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
 
#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <qstring.h>
#include <qt34/qvaluevector.h>

#include "filter.h" 
#include "game.h"
#include "search.h"
#include "query.h"

class Filter;


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
		/** File-based database name */
		virtual QString name() const = 0;
		//game retrieval & storage
		/** Loads a game from the given position, returns true if successful */
		virtual bool load(int index, Game& game) = 0;
		/** Loads only the header information into a game from the given position, returns true if successful */
		virtual bool loadHeaders(int index, Game& game);
		/** Saves a game at the given position, returns true if successful */
		virtual bool save(int index, Game& game) = 0;
		/** Adds a game to the database */
		virtual void add(Game& game) = 0;
		/** Removes a game from the database */
		virtual void remove(int index) = 0;
		/** Removes multiple games from the database as specified by the filter */
		virtual void remove(const Filter& filter) = 0;
		/** Compacts the database */
		virtual void compact() = 0;
		/** Returns the number of games in the database */
		virtual int count() = 0;
		
		//capability enquiry (so we can start using a format before every last search type is supported)
		/** Checks if the database supports the given type of search */
		virtual bool supportedSearchType(Search::Type searchType) = 0;
		
      virtual void initSearch(Query& query, Filter* filter) = 0;
      //virtual QValueVector<QPair<FilterSearch, int> > initSearch(Query& query, Filter* filter) = 0;
      virtual void searchGame(int index) = 0;
      virtual void finalizeSearch() = 0;
		
		//move statistics (cf. tree window in Scid)
		typedef struct {
			Move move;
			QString eco;
			float frequency;
		} MoveStat;
		
		typedef QValueVector<MoveStat> MoveStatList;
		
		/** Returns move statistics for the given position */
		virtual MoveStatList moveStats(const MoveList& moveList) = 0;

};

#endif

