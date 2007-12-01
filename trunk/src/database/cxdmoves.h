/***************************************************************************
                          cxdmoves.h  - storing and retrieving moves
                                        for chessxdatabase 
                             -------------------
   begin                : 15 July 2007
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

#ifndef __CXDMOVES_H__
#define __CXDMOVES_H__

#include <QFile>
#include <QByteArray>

#include "game.h"
#include "cxdsaxhandler.h"
#include "cxdcfile.h"
#include "cxdcblock.h"

// Just for tests
#include <iostream>
// ---------------------


/** @ingroup Database
   The CxdMoves class provides handling of files for saving and accessing
   the moves, nags and comments of a ChessX database.

   @todo
   - serialize input/output operations of QIODevice by QDatastream
*/

class CxdMoves
{
  public:
  /** Default constructor */
  CxdMoves();

  /** Destructor */
  ~CxdMoves();
  
  /** Initializes CxdMoves object by using filenames saved in saxhandler. */
  bool open(const SaxHandler& saxhandler);
  /** Closes files and sets m_isOpen to false. */
  void close();
  /** Creates files for encoding moves and initializes the CxdMoves object
   *  corresponding to the data contained in saxhandler. */
  bool create(const SaxHandler& saxhandler);

  /** Loads moves, nags and comments into game */
  bool loadMoves(const int& index, Game& game);
  /** Appends game to move files */
  bool appendGame(Game& game);

  /** returns the name of the file containing the encoded moves of the games */
  QString gameFilename() const;
  /** returns the name of the game access file */
  QString gameAFilename() const;
 
  /** return the number of games saved in CxdMoves */
  int nb_games();

  private:
  bool m_isOpen;

  static const int gameARecordSize;
  QFile m_gameFile;
  CxdCFile m_gameACFile; 
  CxdCBlockT<qint64> m_gameACBlock;
//  QFile m_gameAFile;

  qint64 appendToGameFile(Game& game);
  bool appendToGameAFile(const qint64& pos);

};


#endif
