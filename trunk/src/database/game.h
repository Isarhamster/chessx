/***************************************************************************
                          board.h - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include <qvaluevector.h>

class Game
{
public:
  /** Creates a new 0-length game starting from position @p b (or startup position) */
  Game(const Board& b);
  /** Creates a game loaded from PGN. */
  Game(const QString& pgn);
  /** Creates a game loaded from DB format. */
  Game(const char* buffer);

 /* Game import/export */
 /** Loads the game from PGN. */
 bool fromPGN(const QString& pgn);
 /** Loads encoded game from a buffer. */
 bool fromBuffer(const char* db);
 /** Saves the game into PGN. */
 QString asPGN() const;
 /** Saves the game into database buffer. */
 void asBuffer(const char* buffer) const;
 
 /* Board */
 /** Sets the position to be used as a start position for the game. Removes all moves if present. */
 void setStartBoard(Board& b);
 /** returns the starting position for the game */
 Board startBoard() const;
 /** Returns current position */
 Board currentBoard() const;
 
 /* Game modification */
 /** adds a move at the end of the game */
 void addMove(const Move& m);
 /** truncates the game to @p p plies) */
 void truncate(int p);

 /* Move/ply information and browsing the game */
 /** @return number of plies */
 int plyCount() const;
 /** @return current ply */
 int currentPly() const;
 /** Sets current ply */
 void setCurrentPly(int p) const;
 /** @return move at ply @p p, or current move if no argument is specified. */
 Move move(int p = -1) const;
 /** @return number of moves in the game */
 int moveCount() const;
 /** Moves @p p plies forward in the game or to the end of game/variation
    @return true if all @p plies were done. */
 bool moveForward(int p = 1);
 /** Moves @p p plies backward in the game or to the beginning of the game/variation.  
   @return true if all @p plies were done. */
 bool moveBackward(int p = 1) const;

private:
 /** Writes encoded move into database buffer. @return number of bytes written.  */
 int encodeMove(const Move& m, const char* buffer) const;
 /** Decodes next move from database format. @return number of bytes read (0 if there is no move to read).  */
 int decodeMove(Move& m, const char* buffer) const;

 Board m_startBoard;
 Board m_currentBoard;
 int m_ply;
 int m_length;
 QValueVector <Move> m_moves;
};

#endif
