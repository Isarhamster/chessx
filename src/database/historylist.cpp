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

HistoryList::HistoryList(int historySize) : m_unique(true)
{
  setSize(historySize);
}

HistoryList::~HistoryList()
{
}

void HistoryList::restore(const QString& group, const QString& key)
{
  AppSettings->beginGroup(group);
  QStringList list = AppSettings->value(key).toStringList();
  AppSettings->endGroup();
  setItems(list);
}

void HistoryList::save(const QString& group, const QString& key) const
{
  AppSettings->beginGroup(group);
  AppSettings->setValue(key, items());
  AppSettings->endGroup();
}

void HistoryList::append(const QString& item)
{
  if (m_unique)
    remove(item);
  if (count() == size())
    m_data.pop_back();
  m_data.prepend(item);
}

void HistoryList::remove(const QString& item)
{
  m_data.removeAll(item);
}

void HistoryList::setSize(int newSize)
{
  m_size = newSize;
}

int HistoryList::size() const
{
  return m_size;
}

int HistoryList::count() const
{
  return m_data.count();
}

void HistoryList::clear()
{
  m_data.clear();
}

QString HistoryList::item(int index) const
{
  return (index >= 0 && index < count()) ? m_data[index] : QString();
}

QString HistoryList::operator[](int index) const
{
  return item(index);
}

bool HistoryList::contains(const QString& s) const
{
  return m_data.contains(s);
}

QStringList HistoryList::items() const
{
  return m_data;
}

void HistoryList::setItems(const QStringList& list)
{
  clear();
  for (QStringList::ConstIterator iter = list.begin();
       iter != list.end(); ++iter)
    if (count() == size())
      break;
    else
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

