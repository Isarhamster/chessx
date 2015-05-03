/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>                *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __INDEX_H__
#define __INDEX_H__

#include <QList>
#include <QPair>
#include <QObject>

#include "indexitem.h"
#include "game.h"

#define VERSION_INDEX_1_2 1
#define VERSION_INDEX_1_3 2
#define VERSION_INDEX_CURRENT VERSION_INDEX_1_3

#define INDEX_FILE_MAGIC 0xce55

/** @ingroup Database
 * The Index class holds a list of IndexItem instances, typically one
 * for each game in the current database. This enables fast access to
 * game header information.
 *
 */

class Index : public QObject
{
    Q_OBJECT

public:
    Index();
    ~Index();

    /** Adds an empty indexitem */
    GameId add();

    /** @ret number of index items in the Index */
    int count() const;

    // Storing tags //
    //
    /** Store the tag value for the given game, tag is given by name */
    void setTag(const QString& tagName, const QString &value, GameId gameId);

    /** Set the valid flag accordingly */
    bool replaceTagValue(const QString& tagName, const QString& newValue, const QString& oldValue);

    // Retrieving tags //
    //
    /** Restore all tags for game @p id from Index into @p game object */
    void loadGameHeaders(GameId id, Game& game) const;

    /** Restore the value for @p tag for game @p id from Index into @p game object */
    void loadGameHeader(GameId id, Game& game, const QString& tag) const;

    /** Get the tag @p tagName for given game index @p gameId */
    QString tagValue(const QString& tagName, GameId gameId) const;

    /** @ret the value index number of a @p value  */
    ValueIndex getValueIndex(const QString&) const;

    /** @ret the value index number of a tags name @p value for a given game */
    ValueIndex valueIndexFromTag(const QString& tagName, GameId gameId) const;

    /** Get the list of tagValues for a given @p tagName */
    QStringList tagValues(const QString& tagName) const;

    /** Get the list of players (optimized query, as it reads white and black names w/o duplicates) */
    QStringList playerNames() const;

    // Validity of a game information
    //
    /** Set the valid flag accordingly */
    void setValidFlag(GameId gameId, bool value);

    /** Get the valid flag accordingly */
    bool isValidFlag(GameId gameId) const;

    // Searching tags //
    //
    /** Return a bit array to indicate which games in index have tag with matching value */
    QBitArray listContainingValue(const QString& tagName, const QString& value) const;

    /** Returns a bit array to indicate which games in index have a tag value in given range */
    QBitArray listInRange(const QString& tag, const QString& minValue, const QString& maxValue) const;

    /** Returns a bit array to indicate which games in index have a tag value in given range */
    QBitArray listInRange(const QString& tag, int minValue, int maxValue) const;

    /** Returns a bit array to indicate which games in index have a tag value which somewhat matches */
    QBitArray listPartialValue(const QString& tagName, const QString& value) const;

    // Utility //
    //

    /** Write the index to disk, using m_filename */
    bool write(QDataStream& out) const;

    /** Read the index from disk, using m_filename */
    bool read(QDataStream& in, volatile bool *breakFlag, short version);

    /** Clear all cached values */
    void clearCache();

    /** Build the tag caches */
    void calculateCache(volatile bool* breakFlag = 0);

    /** Calculate missing data from the index file import */
    void calculateTagMap(volatile bool *breakFlag);

    /** Calculate missing data from the index file import */
    void calculateReverseMaps(volatile bool *breakFlag);

    /** Clears the index, and frees all associated memory */
    void clear();

    /** Read delete flag */
    bool deleted(GameId gameId) const;

    /** Set delete flag */
    void setDeleted(GameId gameId, bool df);

    /** Remove a tag from the index, do not care for the value */
    void removeTag(const QString &tagName, GameId gameId);

signals:
    void progress(int);

private:
    /** Contains information which games are marked for deletion */
    QSet<GameId> m_deletedGames;

    /** Return a pointer to the index item for the given game id */
    IndexItem* item(GameId gameId);

    /** Map a tags index to a tagName */
    QHash<TagIndex, QString> m_tagNames;
    /** Map a tagName to an associated index */
    QHash<QString, TagIndex> m_tagNameIndex;
    /** Add a tag name to the index */
    TagIndex AddTagName(QString);

    /** Map an Index to a tagValue */
    QHash<ValueIndex, QString> m_tagValues;
    /** Map a Value to its associated index value */
    QHash<QString, ValueIndex> m_tagValueIndex;
    /** Add a tag value to the index */
    ValueIndex AddTagValue(QString);

    /** Hold the list of index items (=holds all game header information) */
    QList<IndexItem*> m_indexItems;

    /** Contains information which games are marked as valid */
    QSet<GameId> m_validFlags;

    /** Map tags to index items (which game has a specific header information) */
    QMultiHash<TagIndex, int> m_mapTagToIndexItems;

    /** Query the value of a tag given the tags index for a specific game */
    QString tagValue(TagIndex tagIndex, GameId gameId) const;

    /** Query the value of a tag given the tags index */
    QString tagName(TagIndex tagIndex) const;

    /** @ret the tag index number of a @p value  */
    TagIndex getTagIndex(const QString& value) const;

    /** Get the name of a @p valueIndex */
    QString tagValueName(ValueIndex valueIndex) const;

    /** @ret the value index number of a tags index @p value for a given game */
    ValueIndex valueIndexFromIndex(TagIndex tagIndex, GameId gameId) const;

    /** @ret true if a game @p gameId has a given tag index */
    bool indexItemHasTag(TagIndex tagIndex, GameId gameId) const;
};

#endif   // __INDEX_H__

