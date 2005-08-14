/***************************************************************************
                          capture.h - infor,mation about capture/promotion
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include "common.h"
#include <qvaluestack.h>

class Capture {
public:
  /* Standard capture - index and type of captured piece. */
  Capture(int move, unsigned char index, Piece p);
  /* Promotion - index of pawn used for promotion. */
  Capture(int move, unsigned char index);
  /* @return move number */
  int move() const;
  /* Sets move number. */
  void setMove(int move);
  /* @return captured piece. */
  Piece piece() const;
  /* Sets captured piece. */
  void setPiece(Piece p);
  /* @return index of captured piece. */
  unsigned char index() const;
  /* Sets captured piece. */
  void setIndex(unsigned char index);
  /* Checks whether move is a promotion */
  bool isPromotion() const;
  /* Checks whether move is a capture */
  bool isCapture() const;
private:
  int m_move;
  unsigned char m_index;
  Piece m_piece;
};



class CaptureList : QValueStack<Capture> {
public:
  /** Checks whether the list contains entry for given move */
  bool hasMove(int move) const;
};

#endif
