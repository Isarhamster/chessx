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

#ifndef __INDEXITEM_H__
#define __INDEXITEM_H__

#include <QList>
#include <QDataStream>
#include <QHash>


/** @ingroup Database
 The IndexItem class holds one item that is listed in a index
 It contains essential game header information that is kept in
 memory for fast access. It is used in conjunction with the Tags
 class. For most items it stores the id that refers to the relevant
 tag item in the Tags instance.
*/

typedef quint32 TagIndex;
typedef quint32 ValueIndex;

#define TagNoIndex 0xFFFFFFFF

typedef QHash<TagIndex, ValueIndex> MapTagToValue;

class IndexItem
{
public:
    IndexItem();
    ~IndexItem();

    /** Adds an index pair to the IndexItem */
    void set(TagIndex tagIndex, ValueIndex valueIndex);

    /** Remove an index */
    void remove(TagIndex tagIndex);

    /** @ret ValueIndex of a @p tagIndex */
    ValueIndex valueIndex(TagIndex tagIndex) const;

    /** @ret true iff the IndexItem has a value for @p tagIndex */
    bool hasTagIndex(TagIndex tagIndex) const;

    const QList<TagIndex> getTagIndices() const;

    /** Write the data of the instance to a QDataStream */
    void write(QDataStream& out) const;

    /** Reads the data of the instance from a QDataStream, existing data is cleared first. */
    void read(QDataStream& in);

    /** Compare two tag index items */
    bool isEqual(const IndexItem& rhs) const;

    /** Search and replace all values from @p valueIndex to @p newValueIndex */
    void replaceValue(QList<TagIndex> tags, ValueIndex valueIndex, ValueIndex newValueIndex);

private:
    MapTagToValue m_mapTagIndexToValueIndex;
};

#endif	// __INDEXITEM_H__

