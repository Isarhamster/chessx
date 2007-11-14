/***************************************************************************
                          cxdindex.h  - handles loading and saving of 
					the index of a chessxdatabase
                             -------------------
   begin                : 18 October 2007
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

#ifndef __CXDINDEX_H__
#define __CXDINDEX_H__

#include "index.h"
#include "cxdsaxhandler.h"

#include <QFile>

/** @ingroup Database
  The CxdIndex class handles loading and saving operations for an index of
  a chessxdatabase which contains the tags to store in memory.

   @todo
   - implement handling of non-standard tags
   - implement compactification
   - perhaps: cut the ECO-code to the first three standard characters
     (i.e., cutting away the descriptive text) 
*/

class CxdIndex
{
  public:
  /** Number of standard tags for cxd file format. */
  static const int m_nbIndexTags=12;
  /** Number of files used to represent standard tags. */
  static const int m_nbTagFiles=10;

  /** Special Tags are tags which do not need to be saved in a game in standard way
   * the can be determined out of the game object as TagPlyCount,TagECO,TagFEN.
   * These Tags should always be at the end of CxdIndex::tags. */
  static const int m_nbSpecialTags=3;

  /** Lists the tags */
  static const Tag tags[m_nbIndexTags]; 

  /** Lists the basic tags which are saved in the tagfiles
    * (compared to cxdIndex::tags, m_mappedTags contains for
    * example only TagPlayerName instead of TagWhite and
    * TagBlack. */
  // m_mappedTags is not static to be reentrant
  Tag m_mappedTags[m_nbTagFiles];

  /** Default constructor */
  CxdIndex(Index& index);
  /** Destructor */
  ~CxdIndex();
  
  /** Opens a CxdIndex based on data contained in saxhandler. */
  bool open(SaxHandler& saxhandler);
  /** Closes files and sets m_isOpen to false. */
  void close();
  /** Creates files (corresponding to the data contained in saxhandler)
   *  for handling tags */
  bool create(SaxHandler& saxhandler);

  /** Appends a game to the index. */
  GameId appendGame(Game& game);


  private:
  bool m_isOpen;

  Index* m_index;
  SaxHandler* m_saxhandler;
  
  QFile m_indexFile;
  QDataStream m_indexDataStream;

  QFile m_tagFiles[m_nbTagFiles];
  // maps the indices of tags to the indices corresponding to the files of m_tagFiles.
  int m_toTagFileIndex[m_nbIndexTags];

  /** Returns the index of which corresponds in m_tagFiles to the given tag or -1
   *  if the index does not exists in tags.
   *  This is not a very efficient function but does not need to be as it is used
   *  only in non-bottleneck operations. */ 
  int getTagFileIndex(const Tag& tag);

  QDataStream m_tagDataStreams[m_nbTagFiles];

  void setFilenamesFromSaxHandler();
  void openFiles();

};


#endif

