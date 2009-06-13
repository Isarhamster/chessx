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
	m_data = NULL;
	allocate(Index::defaultIndexItemSize);
}
IndexItem::~IndexItem()
{
	if (m_data)
		free(m_data);
}
int IndexItem::allocate(const int size, bool clear)
{
	// We use malloc and realloc to be able to change the size of the array
	// C++ does not offer an alternative, apart from container classes.
	// The current suspicion is that using a container might be too slow,
	// but at some point we should do a test using QByteArray.
	if (!m_data) {
		m_data = (unsigned char*) malloc(size);
	} else if (m_size != size) {
		m_data = (unsigned char*) realloc((void*)m_data, size);
	}
	if (m_data) {
		if (clear) {
			// Clear all the values to zero
			for (int i = 0;i < size; ++i) {
				*(quint8*)(m_data + i) = (quint8) 0;
			}
		} else {
			// Clear only newly allocated values to 0
			for (int i = m_size;i < size; ++i) {
				*(quint8*)(m_data + i) = (quint8) 0;
			}
		}
		m_size = size;
		return m_size;
	} else {
		m_size = 0;
		return 0;
	}
}
QString IndexItem::output()
{
	QString str;
	str = QString("size: ") + QString::number(m_size) + QString("\n");

	for (int i = 0;i < m_size; ++i) {
		str += QString("%1 ").arg((quint8)(*(m_data + i)), 2, 16, QChar('0'));
	}
	return str;
}
TagIndex IndexItem::set(int offset, int size, TagIndex index)
{
	// Could this test be skipped, abandonning safety for speed?
	if ((offset + size) > m_size)
		return static_cast<TagIndex>(-1);
	if (size == 1) {
		*(quint8*)(m_data + offset) = (quint8) index;
	} else if (size == 2) {
		*(quint16*)(m_data + offset) = (quint16) index;
	} else if (size == 4) {
		*(quint32*)(m_data + offset) = (quint32) index;
	} else if (size == 3) {
		*(quint32*)(m_data + offset) = ((quint32)index) | (*(m_data + offset + 3) << 24);
	}
	return index;
}
TagIndex IndexItem::index(int offset, int size)
{
	// Could this test be skipped, abandonning safety for speed?
	if ((offset + size) > m_size)
		return 0;
	if (size == 1) {
		return *(quint8*)(m_data + offset);
	} else if (size == 2) {
		return *(quint16*)(m_data + offset);
	} else if (size == 4) {
		return *(quint32*)(m_data + offset);
	} else if (size == 3) {
		return (*(quint32*)(m_data + offset)) & 0x00FFFFFF;
	}
	return 0;
}

void IndexItem::write(QDataStream& out) const
{
	out << m_size;
	for (int i = 0;i < m_size; ++i) {
		out << m_data[i];
	}
}

void IndexItem::read(QDataStream& in)
{
	in >> m_size;
	allocate(m_size);
	for (int i = 0;i < m_size; ++i) {
		in >> m_data[i];
	}
}

