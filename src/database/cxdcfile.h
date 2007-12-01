/***************************************************************************
 * 	cxdcfile.h - Class for managing simple files with constant record size.
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#ifndef __CXDCFILE_H__
#define __CXDCFILE_H__

#include <QFile>
#include <QDataStream>

class CxdCBlock;

/** @ingroup Database
    The CxdCFile class allows to handle files with constant recordsize. This class
    allows to bundle code for such operations. Access to the data is provided through
    CxdCBlock or derived classes, allowing to give some semantics to the byteblocks
    accessed through the CxdCFile class. CxdCBlock objects that access data in
    a CxdCFile object should be listed in m_blocklist. This allows to check if
    different CxdCBlocks try to manage the same byteblocks and allows to apply
    cleanup operations to all CxdCBlocks associated with the CxdCFile object when
    closing. 
    @todo
*/

class CxdCFile
{
  friend class CxdCBlock;

  public:
   CxdCFile(const int& recordsize);
   ~CxdCFile();

/** Sets the file to be managed to the file corresponding to filename. Furthermore,
 *  this operation opens the file. */
   bool open(const QString& filename);
/** Closes file, makes cleanup operations and sets m_isOpen to false.*/
   void close();
/** Creates empty file corresponding to filename and sets it for administration.*/
   bool create(const QString& filename);

/** Seeks position in m_qf corresponding to gameId id.*/
   void seek(const int& id);
/** Compactifies file corresponding to ql. ql contains false for every entry which
 *  has to be deleted.*/
   void compact(QList<bool>& ql);

/** Returns the number of entries in the managed file.*/
   int nb_entries() const;

/** Appends nb default entries to m_qf.*/
   void appendEntries(const int& nb=1);

/** Adds cb to m_blocklist if there is no overlaping of managed blocks. If there
 *  is overlaping, nothing is done and zero is returned. */
   bool addBlock(CxdCBlock& cb);

/** Returns true if id is valid. */
   bool isInRange(const int& id) const;

/** returns m_isOpen */
   bool isOpen();

/** Returns true if file exists. */
   bool existsFile() const;

/** Returns the name of the underlying file. */
   QString fileName() const;

  private:
   bool m_isOpen;
   QFile m_qf;
   int m_recordsize;

   char* m_defaultentry;
   QList<CxdCBlock*> m_blocklist;

   bool doesOverlap(CxdCBlock& cb) const;

   /** Copies the entry corresponding from sourceId to destId. Range checking is only done
    *  in debug mode. */
   void copyentry(const int& sourceId, const int& destId);

}; 


#endif
