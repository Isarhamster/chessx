/***************************************************************************
 *   (C) 2006-2007 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>                *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "stringtagvalues.h"
#include <QtDebug>

StringTagValues::StringTagValues()
{
	m_list.append("?");
    m_cache = NULL;
    setCacheEnabled(true);
}

StringTagValues::~StringTagValues()
{
    setCacheEnabled(false);
}

int StringTagValues::add(const QString& value)
{
    if (m_cache)
    {
        int item = m_cache->value(value, -1);
        if (item >= 0)
        {
            return item;
        }
        else
        {
            m_cache->insert(value, m_list.count());
            m_list.append(value);
            return (m_list.count() - 1);
        }
    }
    else
    {
        int position = m_list.indexOf(value);
        if (position > 0) { return position; }
        m_list.append(value);
        return (m_list.count() - 1);
    }
}

QString StringTagValues::value(int index) const
{
	Q_ASSERT((index >= 0) && (index < m_list.count()));
	return m_list[index];
}

void StringTagValues::clear()
{
    setCacheEnabled(false);
    setCacheEnabled(true);
	m_list.clear();
}

int StringTagValues::indexOf(const QString& value) const
{
	return m_list.indexOf(value);
}

void StringTagValues::setCacheEnabled(bool enabled)
{
    if (enabled) {
        m_cache = new TagCache;
        for (int i = 0;i < m_list.count();++i) {
            m_cache->insert(m_list.at(i), i);
        }
    } else {
        if (m_cache) { delete m_cache; }
        m_cache = NULL;
    }
}

int StringTagValues::count() const
{
	return m_list.count();
}

bool StringTagValues::contains(const QString& value)
{
	return m_list.contains(value);
}

void StringTagValues::read(QDataStream& in)
{
    int n;
    in >> n;
	QString line;
    for (int i = 0; i < n; ++i)
    {
		in >> line;
		add(line);
	}
}

void StringTagValues::write(QDataStream& out)
{
    out << m_list.count();
	for (int i = 0; i < m_list.count(); ++i) {
		appendToStream(m_list[i],out);
	}
}

void StringTagValues::appendToStream(const QString& value, QDataStream& out)
{
	out << value;
}

QBitArray StringTagValues::listContainingValue(const QString& value)
{
	QBitArray list(count(), false);
	for (int i = 0; i < count(); ++i) {
		list.setBit(i, m_list[i].contains(value, Qt::CaseInsensitive));
	}
	return list;
}

QBitArray StringTagValues::listInRange(const QString& minValue, const QString& maxValue)
{
	QBitArray list(count(), false);
	for (int i = 0; i < count(); ++i) {
		list.setBit(i, ((m_list[i] >= minValue) && (m_list[i] <= maxValue)));
	}
	return list;
}

void StringTagValues::compact(const QVector<bool>& qv)
{
  // updating cache
  if(m_cache)
  {
    for(int i=0; i<qv.size(); ++i)
    {
      if(!qv[i]) m_cache->remove(m_list[i]);
    }
  }
}

