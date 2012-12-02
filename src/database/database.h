/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/** @defgroup Database Database - classes to manipulate chess game files*/

/** @ingroup Database
   The Database class is abstract, providing a common interface for all
   database types. There are methods for the loading and saving of games,
   and for performing searches and queries.

   @todo
   Add name() to get just the database name
   Add isReadOnly()
*/

class Database : public QObject
{    
	Q_OBJECT
public:
	/** Standard constructor. */
	Database();
    volatile bool m_break;
	/** Virtual destructor */
	virtual ~Database();

    /** Query file format */
    bool isUtf8() const { return m_utf8; }
    /** Set file format */
    void setUtf8(bool utf8) { m_utf8 = utf8; }

	//database operations
	/** Creates a database with the given filename */
	virtual bool create(const QString&) { return false; }
	/** Opens the given database */
    virtual bool open(const QString& filename, bool utf8) = 0;
    /** Opens the given database */
    virtual bool parseFile() = 0;
	/** File-based database name */
	virtual QString filename() const = 0;
	/** File-based database name for displayng (no path and extension. */
	virtual QString name() const;
	/** Closes the database */
	virtual void close() = 0;
	/** Returns whether the database is read-only or not */
	virtual bool isReadOnly() const {return true;}
	/** Removes all games from the database, return true if successful */
	virtual bool clear() { return false; }

	//game retrieval & storage
	/** Loads a game from the given position, returns true if successful */
	virtual bool loadGame(int index, Game& game) = 0;
	/** Load all tags for GameId from index into game object */
	virtual void loadGameHeaders(GameId id, Game& game) { m_index.loadGameHeaders(id, game); }
	/** Loads only moves into a game from the given position */
	virtual void loadGameMoves(int index, Game& game) = 0;
	/** Saves a game at the given position, returns true if successful */
	virtual bool replace(int , Game&) { return false; }
	/** Adds a game to the database */
    virtual bool appendGame(const Game&) { return false; }
	/** Removes a game from the database */
	virtual bool remove(int) { return false; };
	/** Removes multiple games from the database as specified by the filter */
	virtual bool remove(const Filter&) { return false; }
	/** Compacts the database */
	virtual void compact() {}
    /** @return pointer to the index of the database */
    Index *index();
    /** @return const pointer to the index of the database */
    const Index *index() const;
	/** Returns the number of games in the database */
    virtual int count() { return 0; }
	/** @return true if the database has been modified. By default database is read-only. */
	virtual bool isModified() const { return false; }
    /** Get the Valid Flag for a given game id from the index */
    virtual bool getValidFlag(GameId id) const { return m_index.isValidFlag(id); }
signals:
	/** Signal emitted when some progress is done. */
    void progress(int);

protected:
	Index m_index;
    bool m_utf8;
};

#endif

