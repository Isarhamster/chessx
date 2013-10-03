/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/** @todo
	Removing from database is not implemented.
*/

class MemoryDatabase : public PgnDatabase
{
    Q_OBJECT
public:
    /** Default constructor */
    MemoryDatabase();
    /** Destructor */
    ~MemoryDatabase();
    /** Returns whether the database is read-only or not */
    virtual bool isReadOnly() const
    {
        return false;
    }
    /** @return whether the database was modified. */
    virtual bool isModified() const
    {
        return m_isModified;
    }
    /** @return whether the database was modified. */
    void setModified(bool b)
    {
        m_isModified = b;
    }
    /** Adds a game to the database */
    bool appendGame(const Game& game);
    /** Removes a game from the database */
    bool remove(int gameId);
    /** Undo the deletion of a game */
    bool undelete(int gameId);
    /** Saves a game at the given position, returns true if successful */
    bool replace(int index, Game& game);
    /** Removes all games from the database, return true if succesful */
    virtual bool clear();

    /** Loads a game from the given position, returns true if successful */
    bool loadGame(int index, Game& game);
    /** Loads only moves into a game from the given position */
    void loadGameMoves(int index, Game& game);

protected:
    virtual void parseGame();

private:
    QVector <Game*> m_games;
    bool parseFile();
    bool m_isModified;
};

#endif	// __MEMORYDATABASE_H__

