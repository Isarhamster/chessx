/***************************************************************************
                          common  -  description
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

#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char Square;
typedef unsigned char Coord;

enum Color {White, Black};
enum Piece {Empty, WhiteKing, WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight, WhitePawn, 
  BlackKing, BlackQueen, BlackRook, BlackBishop, BlackKnight, BlackPawn};

enum MoveType {StandardMove,
  PromotionWhiteQueen = WhiteQueen, PromotionWhiteRook = WhiteRook,
  PromotionWhiteBishop = WhiteBishop, PromotionWhiteKnight = WhiteKnight,
  PromotionBlackQueen = BlackQueen, PromotionBlackRook = BlackRook,
  PromotionBlackBishop = BlackBishop, PromotionBlackKnight = BlackKnight,
  Castling = 128, EnPassant};

#endif

