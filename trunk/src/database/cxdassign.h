/***************************************************************************
                          cxdassign.h  - makes the link between game indices
			                 and internally used indices.
                             -------------------
   begin                : 23 November 2007
   copyright            : (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 3 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License version 3 for more details.                 *
 *                                                                         *
 ***************************************************************************/

#ifndef __CXDASSIGN_H__
#define __CXDASSIGN_H__

#include "cxdsaxhandler.h"

#include <QFile>

/** @ingroup Database
  The CxdAssign class assigns game indices to indices used internally. Dataholders
  like the index class (and the .cxi file on disk) which have constant record size
  do not need internal indices which are different from the game indices. However
  dataholders without constant record size (like .cxg file) use different indices
  to speed up some operations.

   @todo
*/

class CxdAssign
{
  public:

  CxdAssign();


  /** Fills data into the object from a file specified in saxhandler. */
  bool open(SaxHandler& saxhandler);
  /** Closes files and sets m_isOpen to false*/
  void close();
  /** Creates files (corresponding to the data contained in saxhandler)
   *  for saving CxdAssign information */
  bool create(SaxHandler& saxhandler);

  /** Returns the internal id (idd) corresponding to id. No range-checking is
   *  done, so one must assure that id is valid.*/
  qint32 iid(const qint32& id) const;
  
  /** Appends a new iid at the end returning true when successful. */
  bool append(const qint32& iid);

  /** Replaces the internal index at position id by iid. id should be
    * in the range [0,...,nb_id()-1].*/
  bool replace(const qint32& id, const qint32& iid);
  

  /** Returns the number of id's used. */
  int nb_id() const;

  private:

  bool m_isOpen;

  // Appends iid to m_assignFile and returns the position where iid was written
  // in m_assignFile.
  bool appendToAssignFile(const qint32& iid);

  QFile m_assignFile;
  QDataStream m_assignDataStream;

  QList<qint32> m_assign;

};


#endif

