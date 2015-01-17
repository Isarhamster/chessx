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

#include "common.h"
#include <QList>
#include <QDataStream>
#include <QHash>


/** @ingroup Database
 The IndexItem class holds one item that is listed in a index
 It contains essential game header information that is kept in
 memory for fast access. It is used in conjunction with the Tags
 class. For most items it stores the id that refers to the relevant
 tag item in the Tags instance. Where convenient, the actual value is
 stored instead of the id.

 @todo
 Index items seem to be implemented in an inefficient way, as all the tags
 appearing in database are stored for each game. The correct way will be
 to store only basic tags (let's say 7 standard tags, ratings and EventDate),
 using notation: tag name + tag value for others.
 When this is fixed, Output::writeAllTags() should be adjusted.
*/

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

    /** returns value of index stored at 'offset' with given 'size' */
    ValueIndex valueIndex(TagIndex tagIndex) const;
    bool hasTagIndex(TagIndex tagIndex) const;

    /** Write the data of the instance to a QDataStream */
    void write(QDataStream& out) const;
    /** Reads the data of the instance from a QDataStream.
     * All data is cleared first. */
    void read(QDataStream& in);
    const MapTagToValue& getTagMapping() const
    {
        return m_mapTagIndexToValueIndex;
    }

private:
    MapTagToValue m_mapTagIndexToValueIndex;
};

#endif	// __INDEXITEM_H__

