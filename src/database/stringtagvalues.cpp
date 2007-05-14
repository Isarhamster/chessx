/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 20/02/2007
    copyright            : (C) 2007 Marius Roets
                           <saidinwielder@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "stringtagvalues.h"
#include <QtDebug>

StringTagValues::StringTagValues()
{
   m_list.append("?");
   m_cache = NULL;
   setCacheEnabled(false);
}
StringTagValues::~StringTagValues()
{
   setCacheEnabled(false);
}
int StringTagValues::add(const QString& value)
{
   if (m_cache) {
      int item = m_cache->value(value,-1);
      if (item >= 0) {
         return item;
      } else {
         m_cache->insert(value,m_list.count());
         m_list.append(value);
         return (m_list.count()-1);
      }
   } else {
      int position = m_list.indexOf(value);
      if (position > 0) { return position; }
      m_list.append(value);
      return (m_list.count()-1);
   }
}
QString StringTagValues::value(int index)
{
   Q_ASSERT( (index >= 0) && (index < m_list.count()) );
   return m_list[index];
}
void StringTagValues::clear()
{
   setCacheEnabled(false);
   m_list.clear();
}
int StringTagValues::indexOf(const QString& value)
{
   return m_list.indexOf(value);
}
void StringTagValues::setCacheEnabled(const bool enabled)
{
   if (enabled) {
      m_cache = new TagCache;
      for (int i = 0;i < m_list.count();++i) {
         m_cache->insert(m_list.at(i),i);
      }
   } else {
      if (m_cache) { delete m_cache; }
      m_cache = NULL;
   }
}
int StringTagValues::count()
{
   return m_list.count();
}
bool StringTagValues::contains(const QString& value)
{
   return m_list.contains(value);
}
void StringTagValues::read(QDataStream& in)
{
   int count;
   QString line;
   int lineNr = 0;

   clear();
   setCacheEnabled(true);

   in >> count;
   while (lineNr < count) {
      in >> line;
      add(line);
      lineNr++;
   }
   setCacheEnabled(false);
}
void StringTagValues::write(QDataStream& out)
{
   out << m_list.count();
   for (int i = 0; i < m_list.count(); ++i) {
      out << m_list[i];
   }

}
QBitArray StringTagValues::listContainingValue( const QString& value )
{
   QBitArray list(count(),false);
   for (int i = 0; i < count(); ++i) {
      list.setBit(i, m_list[i].contains(value, Qt::CaseInsensitive));
   }
   return list;
}
QBitArray StringTagValues::listInRange( const QString& minValue, const QString& maxValue )
{
   QBitArray list(count(),false);
   for (int i = 0; i < count(); ++i) {
      list.setBit(i, ((m_list[i] >= minValue) && (m_list[i] <= maxValue)));
   }
   return list;
}
