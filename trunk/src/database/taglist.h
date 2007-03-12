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
#ifndef __TAGLIST_H__
#define __TAGLIST_H__

#include <QString>
#include <QList>
#include <QMap>

#include "stringtagvalues.h"
#include "common.h"


/** @ingroup Database  
TagList holds a list of TagValues objects. It represents the
values of all the tags in a database. It has the ability to 
dump, and read it's data using QDataStream, and calling on
TagValues' read() and write() methods.

@todo 
*/

class TagList
{
   public:
      TagList();
      ~TagList();

      /** Adds a string value to a TagValues object, identified by tag */
      int add(Tag tag, const QString& value);
      /** Adds a integer value to a TagValues object, identified by tag */
      int add(Tag tag, int value);
      /** Adds a string value to a TagValues object, identified by tagName */
      int add(const QString& tagName, const QString& value);
      /** Adds a integer value to a TagValues object, identified by tagName */
      int add(const QString& tagName, int value);
      /** Gets the value for tag identified by tagName, stored at index
       * in the TagValues instance */
      QString tagValue(const QString& tagName, int index);
      /** Gets the value for tag identified by tag, stored at index
       * in the TagValues instance */
      QString tagValue(Tag tag, int index);
      /** Returns the TagValues instance stored at index */
      TagValues* tagValueList(int index);
      /** Returns the TagValues instance stored at index */
      TagValues* operator[] (int index);
      /** Returns the number of TagValues instances in the list */
      int count();
      /** Clears the list and frees all associated memory */
      void clear();
      /** Given a tag name tagName, looks up the Tag */
      int tagFromString(const QString& tagName);
      /** Given a Tag tag, looks up the tagName */
      QString stringFromTag(Tag tag);
      /** Writes the instance data to a QDataStream */
      void write(QDataStream& out);
      /** Clears the instance, and reads the instance data from a QDataStream */
      void read(QDataStream& in);
      /** Enables fast adding of tags, by calling setCacheEnabled for all
       * instances in the list. */
      void setCacheEnabled(bool enabled);
      /** Static member function to create a BasicTagMap. This map maps tags
       * that are colour dependant to a color independant list. Example:
       * White and Black are both mapped to PlayerName, so each name is stored
       * only once */
      static void createBasicTagMap();
      
   private:
      /** holds the TagValues instances */
      QList <TagValues*> m_list;
      /** Holds a map that maps the tag names to integer values, which will be 
       * the index for m_list*/
      QMap <QString, int> m_tagNameToInt;
      /** Indicates whether cache is currently enabled or not. If cache is enabled,
       * new TagValues instances added to the list, will have their cache enabled
       * as well */
      bool m_cacheEnabled;
      
};

#endif
