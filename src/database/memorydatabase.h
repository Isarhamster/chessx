/***************************************************************************
                          filename - description
                             -------------------
    begin                : 21/03/2007
    copyright            : (C) 2007 Marius Roets <saidinwielder@sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __MEMORYDATABASE_H__
#define __MEMORYDATABASE_H__

#include <QVector>
#include "pgndatabase.h"

/** @ingroup Database
   The MemoryDatabase class provides database access to PGN files.
   Games are stored in memory, and are editable.
   The class is derived from the PgnDatabase class, providing methods for the
   loading and saving of games, and for performing searches and queries.
*/

class MemoryDatabase : public PgnDatabase
{
public:
	/** Default constructor */
	MemoryDatabase();
	/** Destructor */
	~MemoryDatabase();
	/** Returns whether the database is read-only or not */
	bool isReadOnly() { return false; }
	/** Adds a game to the database */
	bool appendGame(Game& game);
	/** Removes a game from the database */
	bool remove(int index);
	/** Removes multiple games from the database as specified by the filter */
	bool remove(const Filter& filter);
	/** Saves a game at the given position, returns true if successful */
	bool replace(int index, Game& game);
	/** Removes all games from the database, return true if succesful */
	virtual bool clear();

	/** Loads a game from the given position, returns true if successful */
	bool loadGame(int index, Game& game);

private:
	QVector <Game*> m_games;
	bool parseFile();
};

#endif	// __MEMORYDATABASE_H__

