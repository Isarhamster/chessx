/***************************************************************************
                          filename - description
                             -------------------
    begin                :
    copyright            : (C) 2006 Marius Roets <saidinwielder@sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __INDEX_H__
#define __INDEX_H__

#include <QList>
#include <QPair>
#include "indexitem.h"
#include "search.h"
#include "taglist.h"
#include "game.h"

/** @ingroup Database
 * The Index class holds a list of IndexItem instances, typically one
 * for each game in the current database. This enables fast access to
 * game header information.
 *
 * @todo
 * pack() and unpack() needs to be implemented
 */

class Index
{
public:
	Index();
	~Index();

	// Storing tags //
	//
	/** Store the tag value for the given game, tag is given by tag id */
	void setTag(Tag tag, QString value, int gameId);

	/** Store the tag value for the given game, tag is given by name */
	void setTag(const QString& tagName, QString value, int gameId);

	/** Create new gameId in index and store all tags from game object into it */
	GameId add(const Game& game);


	// Retrieving tags //
	//
	/** Restore all tags from gameId into game object */
	void loadGameHeaders(GameId id, Game& game);

	/** Get the tag value for given game */
	QString tagValue(Tag tag, int gameId);


	// Searching tags //
	//
	/** Return a bit array to indicate which games in index have tag with matching value */
	QBitArray listContainingValue(Tag tag, const QString& value);

	/** Returns a bit array to indicate which games in index have a tag value in given range */
	QBitArray listInRange(Tag tag, const QString& minValue, const QString& maxValue);


	// Utility //
	//
	/** Enables fast loading of many values */
	void setCacheEnabled(bool enabled);

	/** Returns the index of the value in the appropriate tag list */
	int valueIndex(Tag tag, int gameId);

	/** Returns the Tag id associated with tag-name string */
	Tag tagFromString(const QString& tagName);

private:
	/** Return a pointer to the index item for the given game id */
	IndexItem* item(int gameId);

	TagList m_tagList;
	QList<IndexItem*> m_indexItems;

	/** Clears the index, and frees all associated memory */
	void clear();

	// The following items are related to ItemIndex. These are needed
	// to know where in ItemIndex the values are stored.
	QMap <int, QPair<quint8 , quint8 > > m_tagIndexPosition;
	void setTagIndexPosition(int tag, quint8 offset, quint8 size);
	quint8 m_tagIndexSize;
	void reallocateIndexItems(bool clear = true);
	void calculateIndexSize();

	QString m_filename;

	/** Adds a index item */
	TagIndex add (const IndexItem& item);
	/** Adds a empty indexitem */
	TagIndex add ();
	/** Create index items */
	void createIndexItems();

	/** Compress the data for writing to disk. Is this necessary? */
	void pack();
	/** Uncompress the data when reading from disk. Is this necessary? */
	void unpack();

	/** Returns a map of all tag name/tag value pairs for a given game.
	 * Currently all tags in the database are returned, if the game does
	 * not a have a value for a particular tag, a default value is returned.
	 */
	QList<QPair<QString, QString> > allGameTags(int gameId);

// Unused:
//	/** Recreates Index, removing all unused TagValues */
//	bool compact();
//	/** Write the index to disk, using m_filename */
//	void write();
//	/** Read the index from disk, using m_filename */
//	void read();
//	/** Sets the filename for reading and saving index */
//	void setFilename(const QString& filename);

	/** Removes index item for game gameIndex */
	void remove (int gameIndex);

};

#endif   // __INDEX_H__

