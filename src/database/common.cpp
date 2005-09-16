/***************************************************************************
                          common.cpp  -  common chess code
                             -------------------
    begin                : 27 August 2005
    copyright            : (C) 2005 William Hoggarth
														<whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "common.h"

Piece charToPiece(const QChar& letter)
{
	char pieceChar = letter.latin1();
	
	switch(pieceChar) {
		case 'K':
			return WhiteKing;	
		case 'Q':
			return WhiteQueen;
		case 'R':
			return WhiteRook;
		case 'B':
			return WhiteBishop;
		case 'N':
			return WhiteKnight;
		case 'P':
			return WhitePawn;
		case 'k':
			return BlackKing;	
		case 'q':
			return BlackQueen;
		case 'r':
			return BlackRook;
		case 'b':
			return BlackBishop;
		case 'n':
			return BlackKnight;
		case 'p':
			return BlackPawn;
		default:
			return InvalidPiece;
	}	
}
	
QChar pieceToChar(Piece piece)
{
	switch(piece) {
		case WhiteKing:
			return 'K';	
		case WhiteQueen:
			return 'Q';
		case WhiteRook:
			return 'R';
		case WhiteBishop:
			return 'B';
		case WhiteKnight:
			return 'N';
		case WhitePawn:
			return 'P';
		case BlackKing:	
			return 'k';
		case BlackQueen:
			return 'q';
		case BlackRook:
			return 'r';
		case BlackBishop:
			return 'b';
		case BlackKnight:
			return 'n';
		case BlackPawn:
			return 'p';
		default:
			return '?';
	}	
}
