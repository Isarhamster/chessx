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

#include "filter.h"
#include "game.h"
#include "index.h"
#include "refcount.h"

#include <QMutex>
#include <QString>
#include <QTextStream>

/** @defgroup Database Database - classes to manipulate chess game files*/

/** @ingroup Database
   The Database class is abstract, providing a common interface for all
   database types. There are methods for the loading and saving of games,
   and for performing searches and queries.
*/

class Database : public QObject
{
    Q_OBJECT
    SUPPORTS_REFCOUNTING
public:
    /** Standard constructor. */
    Database();
    volatile bool m_break;
    /** Virtual destructor */
    virtual ~Database();

    /** Query file format */
    bool isUtf8() const;
    /** Set file format */
    void setUtf8(bool utf8);

    QMutex* mutex() { return &m_mutex; }
    //database operations

    /** Opens the given database */
    virtual bool open(const QString& filename, bool utf8) = 0;
    /** Parse the database */
    virtual bool parseFile() = 0;
    /** File-based database name */
    virtual QString filename() const = 0;
    /** Determine if index files are supported */
    virtual bool hasIndexFile() const { return false; }

    /** Returns whether the database is read-only or not */
    virtual bool isReadOnly() const;
    /** File-based database name for display (no path and extension) */
    virtual QString name() const;

    //game retrieval & storage
    /** Loads a game at @p index, returns true if successful */
    virtual bool loadGame(GameId gameId, Game& game);
    /** Load all tags for GameId from index into game object */
    virtual void loadGameHeaders(GameId gameId, Game& game) const;
    virtual void loadGameHeader(GameId gameId, Game& game, const QString& tag) const;
    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(GameId index, Game& game) = 0;
    /** Loads game moves and try to find a position */
    virtual int findPosition(GameId index, const Board& position) = 0;
    /** Saves a game at the given position, returns true if successful */
    virtual bool replace(GameId, Game&);
    /** Adds a game to the database */
    virtual bool appendGame(const Game&);
    /** Removes a game from the database */
    virtual bool remove(GameId);
    /** Undelete a game from the database */
    virtual bool undelete(GameId);
    /** Removes multiple games from the database as specified by the filter */
    virtual bool remove(const Filter&);
    /** @return pointer to the index of the database */
    Index *index();
    /** @return const pointer to the index of the database */
    const Index *index() const;
    /** Returns the number of games in the database */
    virtual quint64 count() const;
    /** @return true if the database has been modified. */
    virtual bool isModified() const;
    /** Set / Reset the modification flag. */
    virtual void setModified(bool) { }
    /** Get the Valid Flag for a given game id from the index */
    virtual bool getValidFlag(GameId gameId) const;
    /** Get the Valid Flag for a given game id from the index */
    virtual bool deleted(GameId gameId) const;
    /** Returns true, if the database is a clipboard database, false otherwise */
    virtual bool IsClipboard() const { return false; }

protected:
    /** Copies all tags from @p game to the Index */
    void setTagsToIndex(const Game& game, GameId id);

signals:
    /** Signal emitted when some progress is done. */
    void progress(int);
    void dirtyChanged(bool);

protected:
    Index m_index;
    bool m_utf8;
    QMutex m_mutex;
};

#endif

