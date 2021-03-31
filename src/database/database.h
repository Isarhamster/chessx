/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "filter.h"
#include "gamex.h"
#include "index.h"
#include "refcount.h"
#include "move.h"
#include "movedata.h"

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

    /** Flags to customize position search */
    enum PositionSearchOptions: unsigned
    {
        /** Default search */
        PositionSearch_Default = 0,

        /** Match only if position occurs at the game end */
        PositionSearch_GameEnd = 1,
    };

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
    virtual bool loadGame(GameId gameId, GameX& game);
    /** Load all tags for GameId from index into game object */
    virtual void loadGameHeaders(GameId gameId, GameX& game) const;
    /** Load a tag into a game */
    virtual void loadGameHeader(GameId gameId, GameX& game, const QString& tag) const;
    /** Get the tag for a game */
    virtual QString tagValue(GameId gameId, const QString &tag) const;
    virtual QString tagValue(GameId gameId, TagIndex tag) const;
    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(GameId index, GameX& game) = 0;
    /** Loads game moves and try to find a position */
    virtual int findPosition(GameId index, const BoardX& position) = 0;
    /** Perform batched position search */
    virtual void findPosition(const BoardX& position, PositionSearchOptions options, const QList<GameId>& games, QList<MoveId>& output, QMap<Move, MoveData>& stats);
    /** Saves a game at the given position, returns true if successful */
    virtual bool replace(GameId, GameX&);
    /** Adds a game to the database */
    virtual bool appendGame(const GameX&);
    /** Removes a game from the database */
    virtual bool remove(GameId);
    /** Remove all games from a database */
    virtual void clear();
    /** Undelete a game from the database */
    virtual bool undelete(GameId);
    /** Removes multiple games from the database as specified by the filter */
    virtual bool remove(const FilterX&);
    /** @return pointer to the index of the database */
    IndexX *index();
    /** @return const pointer to the index of the database */
    const IndexX *index() const;
    /** Returns the number of games in the database */
    virtual quint64 count() const;
    /** @return true if the database has been modified. */
    virtual bool isModified() const;
    /** Set / Reset the modification flag. */
    virtual void setModified(bool) { }
    virtual void startTransaction(bool) { }
    /** Get the Valid Flag for a given game id from the index */
    virtual bool getValidFlag(GameId gameId) const;
    /** Get the Valid Flag for a given game id from the index */
    virtual bool deleted(GameId gameId) const;
    /** Returns true, if the database is a clipboard database, false otherwise */
    virtual bool IsClipboard() const { return false; }
    /** Get a map of MoveData from a given board position */
    virtual unsigned int getMoveMapForBoard(const BoardX& , QMap<Move, MoveData> &) { return 0; }
protected:
    /** Copies all tags from @p game to the Index */
    void setTagsToIndex(const GameX& game, GameId id);

signals:
    /** Signal emitted when some progress is done. */
    void progress(int);
    void dirtyChanged(bool);

protected:
    IndexX m_index;
    bool m_utf8;
    QMutex m_mutex;
};

class DatabaseTransaction
{
public:
    DatabaseTransaction(Database* db) { m_db = db; if (db) db->startTransaction(true); }
    ~DatabaseTransaction() { if (m_db) m_db->startTransaction(false); }
private:
    Database* m_db;
};

#endif

