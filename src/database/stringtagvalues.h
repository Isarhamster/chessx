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
#ifndef __STRINGTAGVALUES_H__
#define __STRINGTAGVALUES_H__

#include <QString>
#include <QStringList>
#include <QDataStream>
#include <QHash>

#include "tagvalues.h"


/** @ingroup Database  
StringTagValues class inherits from TagValues, and contains 
a list of unique strings. The list represents all the values 
for a given tag in the database.  It has the ability to dump, 
and read it's data from a QDataStream.

@todo 
*/

class StringTagValues : public TagValues
{

   public:
      StringTagValues();
      ~StringTagValues();

      /** Adds a value to the list, quickly if cache is enabled */
      int add(const QString& value);
      /** Returns a value from the list, given the index 'index' */
      QString value(const int index);
      /** Clears all values from the list */
      void clear();
      /** Returns the index of a value 'value */
      int indexOf(const QString& value);
      /** When cache is enabled, adding values is very quick. It uses twice as
        * much RAM though. Useful for adding lots of values quickly. 
        * Remember to disable afterwards to free extra memory.*/
      void setCacheEnabled(const bool enabled);
      /** Returns the number of values in the list */
      virtual int count();
      /** returns true if the list contains the string 'value' */
      virtual bool contains(const QString& value);
      /** Read the values for list from a data stream */
      void read(QDataStream& in);
      /** Write the current list values to a data stream */
      void write(QDataStream& out);

   private:
      typedef QHash <QString, int> TagCache;
      QStringList m_list;
      TagCache *m_cache;
      // Considered
      QList<int> m_referenceCount;

};

#endif

