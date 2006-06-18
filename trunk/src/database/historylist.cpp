/***************************************************************************
                          historylist  -  description
                             -------------------
    begin                : 18 Jun 2006
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "historylist.h"
#include "settings.h"

#include <qstringlist.h>

HistoryList::HistoryList(int historySize) : m_count(0), m_unique(true)
{
  setSize(historySize);
}

HistoryList::~HistoryList()
{
}

void HistoryList::restore(const QString& group, const QString& key)
{
  AppSettings->beginGroup(group);
  QStringList list = AppSettings->readListEntry(key);
  AppSettings->endGroup();
  setItems(list);
}

void HistoryList::save(const QString& group, const QString& key) const
{
  AppSettings->beginGroup(group);
  AppSettings->writeEntry(key, items());
  AppSettings->endGroup();
}

void HistoryList::append(const QString& item)
{
  if (m_unique)
  {
    int duplicate = find(item);
    if (duplicate != -1)
      remove(duplicate);
  }
  if (count() == size())
  {
    for (int i = 0; i < count() - 1; i++)
      m_data[i] = m_data[i+1];
    m_data[count() - 1] = item;
  }
  else
  {
    m_data[m_count] = item;
    m_count++;
  }
}

void HistoryList::remove(int index)
{
  for (int i = index; i < count() - 1; i++)
    m_data[i] = m_data[i+1];
  m_count--;
}

void HistoryList::setSize(int newSize)
{
  m_data.resize(newSize);
}

int HistoryList::size() const
{
  return m_data.size();
}

int HistoryList::count() const
{
  return m_count;
}

void HistoryList::clear()
{
  m_count = 0;
}

QString HistoryList::item(int index) const
{
  return (index >= 0 && index < count()) ? m_data[m_count - index - 1] : QString::null;
}

QString HistoryList::operator[](int index) const
{
  return item(index);
}

int HistoryList::find(const QString& s) const
{
  for (int i = 0; i < count(); i++)
    if (m_data[i] == s)
      return i;
  return -1;
}

bool HistoryList::contains(const QString& s) const
{
  return find(s) != -1;
}

QStringList HistoryList::items() const
{
  QStringList list;
  for (int i = 0; i < count(); i++)
    list.append(m_data[i]);
  return list;
}

void HistoryList::setItems(const QStringList& list)
{
  clear();
  for (QStringList::ConstIterator iter = list.begin();
       iter != list.end(); iter++)
         append(*iter);
}

bool HistoryList::isUnique() const
{
  return m_unique;
}

void HistoryList::setUnique(bool value)
{
  m_unique = value;
}

