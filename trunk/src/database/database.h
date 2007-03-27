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

#include <QString>
#include <QTextStream>

#include "filter.h" 
#include "game.h"
#include "search.h"
#include "query.h"
#include "index.h"
#include "output.h"

/** @defgroup Database */

/** @ingroup Database
   The Database class is abstract, providing a common interface for all
   database types. There are methods for the loading and saving of games,
   and for performing searches and queries.

   @todo
   Add name() to get just the database name
   Add isReadOnly() 
*/

class Database
{
   public:	
      /** Virtual destructor */
      virtual ~Database();

      //database operations
      /** Creates a database with the given filename */
      virtual bool create(const QString& ) { return false; }
      /** Opens the given database */
      virtual bool open(const QString& filename) = 0;
      /** File-based database name */
      virtual QString filename() const = 0;
      /** Closes the database */
      virtual void close() = 0;
      /** Returns whether the database is read-only or not */
      virtual bool isReadOnly() = 0;

      //game retrieval & storage
      /** Loads a game from the given position, returns true if successful */
      virtual bool loadGame(int index, Game& game) = 0;
      /** Loads only the header information into a game from the given position, returns true if successful */
      virtual bool loadGameHeaders(int index, Game& game);
      /** Saves a game at the given position, returns true if successful */
      virtual bool replace(int , Game&) { return false; }
      /** Adds a game to the database */
      virtual bool add(Game& ) { return false; }
      /** Removes a game from the database */
      virtual bool remove(int ) { return false; };
      /** Removes multiple games from the database as specified by the filter */
      virtual bool remove(const Filter& ) { return false; }
      /** Compacts the database */
      virtual void compact() {}

      /** Returns the number of games in the database */
      virtual int count() { return m_count; }

      /** Writes a single game at index to a text stream in PGN format */
      void saveToPGN(int index, Output& output, QTextStream& out);
      /** Exports the whole database to file filename in PGN format */
      void exportToPGN(QString filename);

   protected:
      Index m_index;
      int m_count;

};

#endif

