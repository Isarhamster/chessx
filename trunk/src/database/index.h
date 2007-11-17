/***************************************************************************
                          filename - description
                             -------------------
    begin                :
    copyright            : (C) 2006 Marius Roets <saidinwielder@sourceforge.net>
                           (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
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
	friend class CxdIndex;
   static const int defaultIndexItemSize;

	Index();
	~Index();

	// Storing tags //
	//
	/** Store the tag value for the given game, tag is given by tag id */
	void setTag(Tag tag, QString value, int gameId);

	/** Store the tag value for the given game, tag is given by name */
	void setTag(const QString& tagName, QString value, int gameId);

	/** Create new IndexItem in index and store all tags from game object into it */
        // this is only a dummy implementation at the moment
	GameId add(const Game& game);

   /** Creates new IndexItem in index and store all tags of CxdIndex::tags into it */
   GameId cxdAdd(const Game& game);

	/** Adds a empty indexitem */
	GameId add ();

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

	/** @return numerical value of tag for given game. */
	int gameTagIndex(Tag tag, int gameId) const;

	/** @return string value of tag for given game. */
	QString gameTagValue(Tag tag, int gameId) const;

	/** @return string value of given index for tag @p tag. Very fast. */
	QString indexToValue(Tag tag, int value) const;

	/** @return index of given string value for tag @p tag. With cache disabled requires searching all values. */
	int valueToIndex(Tag tag, const QString& value) const;

	/** Returns the Tag id associated with tag-name string */
	Tag tagFromString(const QString& tagName);

	/** Return TagValues for given tag (used as QAbstractItemModel for completion). */
	TagValues* tagValues(Tag tag);

private:
	int m_nbUsedIndexItems;

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

	/** Adds an IndexItem. item will be copied in a new item which will
   be dynamically allocated. This adds some overhead but in this
   way the caller of the function does not have to create item
   with @p new. To avoid this overhead use add(IndexItem*). */
	TagIndex add (const IndexItem& item);

	/** Adds new index item. @p item should be dynamically allocated.*/
   TagIndex add (IndexItem* item);

	/** Create index items */
	void createIndexItems();

	/** Compress the data for writing to disk. Is this necessary? */
	void pack();
	/** Uncompress the data when reading from disk. Is this necessary? */
	void unpack();

   /** Writes all tag name/tag value pairs for a given game in list.
	Currently all tags in the database are returned, if the game does
	not a have a value for a particular tag, a default value is returned. */
	void allGameTags(int gameId, QList<QPair<QString,QString> >&);


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

