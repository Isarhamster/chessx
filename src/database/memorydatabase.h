/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MEMORYDATABASE_H__
#define MEMORYDATABASE_H__

#include <QMutex>
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
    /** Destructor */
    ~MemoryDatabase();
    /** Clear a memory database */
    virtual void clear() override;
    /** Returns whether the database is read-only or not */
    bool isReadOnly() const override;
    /** @return whether the database was modified. */
    bool isModified() const override;
    /** Set database dirty flag */
    void setModified(bool b) override;
    /** Set database dirty flag */
    void startTransaction(bool b) override;
    /** Adds a game to the database */
    bool appendGame(const GameX& game) override;
    /** Removes a game from the database */
    bool remove(GameId gameId) override;
    /** Undo the deletion of a game */
    bool undelete(GameId gameId) override;
    /** Saves a game at the given position, returns true if successful */
    bool replace(GameId gameId, GameX& game) override;

    /** Loads a game from the given position, returns true if successful */
    bool loadGame(GameId gameId, GameX& game) override;
    /** Loads only moves into a game from the given position */
    void loadGameMoves(GameId gameId, GameX& game) override;
    int findPosition(GameId index, const BoardX& position) override;

protected:
    void parseGame() override;
    bool hasIndexFile() const override { return false; }

private:
    bool parseFile() override;

private:
    QVector <GameX*> m_games;
    bool m_isModified {false};
    bool m_transaction {false};
    mutable QReadWriteLock m_mutex;
};

#endif	// MEMORYDATABASE_H__

