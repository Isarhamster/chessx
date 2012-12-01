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

#include <QtDebug>

#include "indexitem.h"
#include "index.h"


IndexItem::IndexItem()
{
}

IndexItem::~IndexItem()
{
    m_mapTagIndexToValueIndex.clear();
}

void IndexItem::set(TagIndex tagIndex, ValueIndex valueIndex)
{
    m_mapTagIndexToValueIndex[tagIndex] = valueIndex;
}

ValueIndex IndexItem::valueIndex(TagIndex tagIndex) const
{
    if (m_mapTagIndexToValueIndex.contains(tagIndex))
    {
        return m_mapTagIndexToValueIndex[tagIndex];
    }
    return 0;
}

void IndexItem::write(QDataStream& out) const
{
    out << m_mapTagIndexToValueIndex;
}

void IndexItem::read(QDataStream& in)
{
    in >> m_mapTagIndexToValueIndex;
}

