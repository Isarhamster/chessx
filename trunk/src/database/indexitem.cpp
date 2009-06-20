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

#include <QtDebug>

#include "indexitem.h"
#include "index.h"


IndexItem::IndexItem()
{
	allocate(Index::defaultIndexItemSize);
}
IndexItem::~IndexItem()
{
}

int IndexItem::allocate(const int size, bool clear)
{
	// We use malloc and realloc to be able to change the size of the array
	// C++ does not offer an alternative, apart from container classes.
	// The current suspicion is that using a container might be too slow,
	// but at some point we should do a test using QByteArray.
	m_data.resize(size);
	if (clear)
		m_data.fill(0);
	return m_data.size();
}

QString IndexItem::output()
{
	QString str = QString("size: %d\n").arg(m_data.size());
	for (int i = 0; i < m_data.size(); ++i)
		str += QString("%1 ").arg(quint8(m_data.at(i)));
	return str;
}

TagIndex IndexItem::set(int offset, int size, TagIndex index)
{
	// Could this test be skipped, abandonning safety for speed?
	if ((offset + size) > m_data.size())
		return static_cast<TagIndex>(-1);
	if (size == 1) {
		*(quint8*)(m_data.data() + offset) = (quint8) index;
	} else if (size == 2) {
		*(quint16*)(m_data.data() + offset) = (quint16) index;
	} else if (size == 4) {
		*(quint32*)(m_data.data() + offset) = (quint32) index;
	} else if (size == 3) {
		*(quint32*)(m_data.data() + offset) = ((quint32)index) | (*(m_data.data() + offset + 3) << 24);
	}
	return index;
}
TagIndex IndexItem::index(int offset, int size)
{
	// Could this test be skipped, abandonning safety for speed?
	if ((offset + size) > m_data.size())
		return 0;
	if (size == 1) {
		return *(quint8*)(m_data.constData() + offset);
	} else if (size == 2) {
		return *(quint16*)(m_data.constData() + offset);
	} else if (size == 4) {
		return *(quint32*)(m_data.constData() + offset);
	} else if (size == 3) {
		return (*(quint32*)(m_data.constData() + offset)) & 0x00FFFFFF;
	}
	return 0;
}

void IndexItem::write(QDataStream& out) const
{
	out << m_data;
}

void IndexItem::read(QDataStream& in)
{
	in >> m_data;	
}

