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
#include "cxdassign.h"

// Just for tests
#include <iostream>
// ---------------------


/** @ingroup Database
   The CxdMoves class provides handling of files for saving and accessing
   the moves, nags and comments of a ChessX database.

   @todo
    - perhaps: add an auxiliary entry add the end of m_gameAFile containing
      position of end of file. This for example implementation of endPos
      function.
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

  /** Compactifies gamefile and gameaccess file. ql contains zero for all games
   * not to be deleted. ql is with respect to external indices. cxda is used
   * to map the external indices to internal ones.*/
  void compact(const QList<bool>& ql, const CxdAssign& cxda);

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

  qint64 startPos(const int& index);
  qint64 endPos(const int& index);

  qint64 appendToGameFile(Game& game);
  bool appendToGameAFile(const qint64& pos);
  void writeToGameAFile(const int& index, const qint64& pos);

// ATTENTION: All copy operations are only valid for copying content from
// higher indices/positions to lower ones.

// Copies the memory [sourceStart, sourceEnd[ to
// [destStart, destStart+(sourceEnd-sourceStart)[
  void copyMemInGameFile(const qint64& sourceStart, const qint64& sourceEnd,
			 const qint64& destStart);
// Copies entries in m_gameFile and m_gameAFile
  void copyInterval(const int& sourceFirst, const int& sourceLast,
	            const qint64& destPos, const int& destAPos);

};


#endif
