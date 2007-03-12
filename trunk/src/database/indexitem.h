/***************************************************************************
                          indexitem.h - description
                             -------------------
    begin                : 
    copyright            : (C) 2006 Marius Roets <saidinwielder@sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __INDEXITEM_H__
#define __INDEXITEM_H__

#include "tagvalues.h"
#include "common.h"
#include <QList>
#include <QDataStream>

#define MINIMUM_ARRAY_SIZE 30

/** @ingroup Database  
 The IndexItem class holds one item that is listed in a index
 It contains essential game header information that is kept in
 memory for fast access. It is used in conjunction with the Tags
 class. For most items it stores the id that refers to the relevant
 tag item in the Tags instance. Where convenient, the actual value is
 stored instead of the id.

 @todo
*/

class IndexItem {
   public:
      IndexItem();
      ~IndexItem();

      /* Not necessary if using QByteArray */
      /** Allocates the needed space for the data structure that stores
       * the index values. It is currently assumed that this structure
       * can only grow, so passing a size smaller than the current size
       * has no effect. If clear is true, all values are set to zero,
       * otherwise only newly allocated space is set to zero */
      int allocate(const int size, bool clear=true);
      /** Adds the value 'index', which is a TagValues index to the
        * appropriate position 'offset'. 'size' is the size of the value. */
      TagIndex set(int offset, int size,TagIndex index);
      /** returns value of index stored at 'offset' with given 'size' */
      TagIndex index(int offset, int size);
      /** Write the data of the instance to a QDataStream */
      void write(QDataStream& out);
      /** Reads the data of the instance from a QDataStream.
       * All data is cleared first. */
      void read(QDataStream& in);

      /** A debugging function, used to dump the contents of the memory
       * structure that holds the values */
      QString output();

   private:

      /* Data structure */
      unsigned char* m_data;
      /** Size in bytes of the data structure */
      int m_size;
      /* or to be considered later */
      //QByteArray m_data

};

#endif	// __INDEXITEM_H__

