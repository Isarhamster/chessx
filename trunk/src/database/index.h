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
#include "search.h"
#include "game.h"

/** @ingroup Database
 * The Index class holds a list of IndexItem instances, typically one
 * for each game in the current database. This enables fast access to
 * game header information.
 *
 * @todo
 * pack() and unpack() needs to be implemented
 */

class Index : public QObject
{
    Q_OBJECT

public:
	static const int defaultIndexItemSize;

	Index();
	~Index();

    /** Adds an empty indexitem */
    GameId add ();
    int count() const;

	// Storing tags //
	//
	/** Store the tag value for the given game, tag is given by name */
    void setTag(const QString& tagName, const QString &value, int gameId);

	// Retrieving tags //
	//
    /** Restore all tags for gameId from Index into game object */
	void loadGameHeaders(GameId id, Game& game);

	/** Get the tag value for given game */
    QString tagValue(const QString&, GameId gameId) const;
    ValueIndex getValueIndex(const QString&) const;
    TagIndex getTagIndex(const QString& value) const;
    ValueIndex valueIndexFromTag(const QString& tagName, GameId gameId) const;
    ValueIndex valueIndexFromIndex(TagIndex tagIndex, GameId gameId) const;
    bool indexItemHasTag(TagIndex tagIndex, GameId gameId) const;
    QString tagValueName(ValueIndex getValueIndex) const;
    QStringList tagValues(const QString& tagName) const;
    /** Set the valid flag accordingly */
    void setValidFlag(const int& gameId, bool value);

    /** Get the valid flag accordingly */
    bool isValidFlag(const int& gameId) const;

    QStringList playerNames() const;

    // Searching tags //
	//
	/** Return a bit array to indicate which games in index have tag with matching value */
    QBitArray listContainingValue(const QString& tagName, const QString& value) const;

	/** Returns a bit array to indicate which games in index have a tag value in given range */
    QBitArray listInRange(const QString& tag, const QString& minValue, const QString& maxValue) const;

    /** Returns a bit array to indicate which games in index have a tag value which somewhat matches */
    QBitArray listPartialValue(const QString& tagName, const QString& value) const;

    // Utility //
	//

    /** Write the index to disk, using m_filename */
    bool write(QDataStream& out) const;

    /** Read the index from disk, using m_filename */
    bool read(QDataStream& in, volatile bool *breakFlag);

    /** Calculate missing data from the index file import */
    void calculateTagMap(volatile bool *breakFlag);
    void calculateReverseMaps(volatile bool *breakFlag);

    /** Clears the index, and frees all associated memory */
    void clear();

signals:
    void progress(int);

private:
	/** Return a pointer to the index item for the given game id */
	IndexItem* item(int gameId);

    /** Map an Index to a tagName */
    QHash<TagIndex, QString> m_tagNames;
    QHash<QString, TagIndex> m_tagNameIndex;
    TagIndex AddTagName(QString);

    /** Map an Index to a tagValue */
    QHash<ValueIndex, QString> m_tagValues;
    QHash<QString, ValueIndex> m_tagValueIndex;
    ValueIndex AddTagValue(QString);

    QList<IndexItem*> m_indexItems;

    /** Contains information which games are marked for deletion */
    QList<bool> m_validFlags;
    QMultiHash<TagIndex, int> m_mapTagToIndexItems;

    QString tagValue(TagIndex tagIndex, int gameId) const;
    QString tagName(TagIndex tagIndex) const;

};

#endif   // __INDEX_H__

