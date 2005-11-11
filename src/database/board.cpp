/***************************************************************************
                          board.cpp - board position
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
                           (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>
                           (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qregexp.h>
#include "board.h"

static const int castlingMask[64] = {
	~WhiteQueenside,	AllRights, AllRights, AllRights, ~WhiteBothSides,	AllRights, AllRights, ~WhiteKingside,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	AllRights, 				AllRights, AllRights, AllRights, AllRights, 				AllRights, AllRights, AllRights,
	~BlackQueenside,	AllRights, AllRights, AllRights, ~BlackBothSides,	AllRights, AllRights, ~BlackKingside };

static bool initialized = false;

static const int knightVectors[8]={-17,-15,-10,-6,6,10,15,17};
static const int bishopVectors[4]={-9,-7,7,9};
static const int rookVectors[4]={-8,-1,1,8};
static const int kingVectors[8]={-9,-8,-7,-1,1,7,8,9};

static int knightData[64][8];
static int bishopData[64][4];
static int rookData[64][4];
static int kingData[64][8];

static void initialize();
static void initKnights();
static void initBishops();
static void initRooks();
static void initKings();

void initialize()
{
  if(initialized)return;
  initKnights();
  initBishops();
  initRooks();
  initKings();
  initialized=true;
}

void initKnights()
{
  int x,y,a;
  for(x=0;x<8;x++)
    for(y=0;y<8;y++)
    {
      for(a=0;a<8;a++)
        knightData[x+y*8][a]=0;
      if(y>=2)
      {
        if(x>=1)knightData[x+y*8][0]=1;
        if(x<=6)knightData[x+y*8][1]=1;
      }
      if(y>=1)
      {
        if(x>=2)knightData[x+y*8][2]=1;
        if(x<=5)knightData[x+y*8][3]=1;
      }
      if(y<=6)
      {
        if(x>=2)knightData[x+y*8][4]=1;
        if(x<=5)knightData[x+y*8][5]=1;
      }
      if(y<=5)
      {
        if(x>=1)knightData[x+y*8][6]=1;
        if(x<=6)knightData[x+y*8][7]=1;
      }
    }
}

void initBishops()
{
  int x,y;
  for(x=0;x<8;x++)
    for(y=0;y<8;y++)
    {
      bishopData[x+y*8][0]=MIN(x,y);
      bishopData[x+y*8][1]=MIN(7-x,y);
      bishopData[x+y*8][2]=MIN(x,7-y);
      bishopData[x+y*8][3]=MIN(7-x,7-y);
    }
}

void initRooks()
{
  int x,y;
  for(x=0;x<8;x++)
    for(y=0;y<8;y++)
    {
      rookData[x+y*8][0]=y;
      rookData[x+y*8][1]=x;
      rookData[x+y*8][2]=7-x;
      rookData[x+y*8][3]=7-y;
    }
}

void initKings()
{
  int x,y,a;
  for(x=0;x<8;x++)
    for(y=0;y<8;y++)
    {
      for(a=0;a<8;a++)
        kingData[x+y*8][a]=0;
      if(y)
      {
        if(x)kingData[x+y*8][0]=1;
        kingData[x+y*8][1]=1;
        if(x<=6)kingData[x+y*8][2]=1;
      }
      if(x)kingData[x+y*8][3]=1;
      if(x<=6)kingData[x+y*8][4]=1;
      if(y<=6)
      {
        if(x)kingData[x+y*8][5]=1;
        kingData[x+y*8][6]=1;
        if(x<=6)kingData[x+y*8][7]=1;
      }
    }
}

Board::Board()
{
  clear();
	
	//set up move/piece data if required
	if(!initialized) {
		initialize();
	}
}

void Board::clear()
{
  m_toMove = White;
  memset(m_board, InvalidPiece, sizeof(m_board));
  memset(m_pieceType, Empty, sizeof(m_pieceType));
  memset(m_piecePosition, InvalidSquare, sizeof(m_piecePosition));
  memset(m_pieceCount, 0, sizeof(m_pieceCount));
	m_epSquare = NoEPSquare;
	m_castlingRights = AllRights;
	m_halfMoveClock = 0;
}

void Board::fromFEN(const QString& fen)
{
  clear();
	// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
	
	int index = 0;
	int col = 0;
	int row = 7;
	int pieceIndex = 1;
	
	//process piece placement
	while(fen[index] != ' ') {
	
		if(fen[index].isLetter()) {
			
			//add piece (indexes 0 & 16 reserved for black and wite kings)
			switch(fen[index].latin1()) {
				case 'K':
					restorePiece(row * 8 + col, WhiteKing, 16);
					break;
				case 'k':
					restorePiece(row * 8 + col, BlackKing, 0);
					break;
				default:
					restorePiece(row * 8 + col, charToPiece(fen[index]), pieceIndex++);
					if(pieceIndex == 16) {
						pieceIndex++;
					}
			}
			
			col++;
			if(col > 7) {
				col = 0;
				row--;
			}
			index++;
			continue;
		}
		
		if(fen[index].isNumber()) {
			col += fen[index].digitValue();
			if(col > 7) {
				col = 0;
				row--;
			}
			index++;
			continue;
		}
		
		//slashes get ignored
		index++;
	}
	index++;
	
	//side to move
	setToMove(fen[index].latin1() == 'w' ? White : Black);
	index += 2;
	
	//castling rights
	m_castlingRights = NoRights;
	while(fen[index] != ' ') {
		switch(fen[index].latin1()) {
			case 'K':
				m_castlingRights |= WhiteKingside;
				break;
			case 'Q':
				m_castlingRights |= WhiteQueenside;
				break;
			case 'k':
				m_castlingRights |= BlackKingside;
				break;
			case 'q':
				m_castlingRights |= BlackQueenside;
				break;
		}
		index++;
	}
	index++;
	
	//en passant square
	if(fen[index] != '-') {
		m_epSquare = (fen[index].latin1() - 'a') + 8 * (fen[index + 1].latin1() - '1');
	} else {
		m_epSquare = NoEPSquare;
	}
	
	//half move clock
	m_halfMoveClock = fen.section(" ", 4, 4).toInt();
	
	//full move number is not used
}

QString Board::toFEN() const
{
	QString fen = "";
	Piece piece;
	int empty = 0;
	
	//piece placement
	for(int row = 7; row >= 0; row--) {
		for(int col = 0; col < 8; col++) {
			piece = at(col, row);
			if(piece == Empty) {
				empty++;
			} else {
				if(empty != 0) {
					fen += QString::number(empty);
					empty = 0;
				}
				fen += pieceToChar(piece);
			}
		}
		if(empty != 0) {
			fen += QString::number(empty);
			empty = 0;
		}
		if(row != 0) {
			fen += '/';
		}
	}
	
	//side to move
	fen += toMove() == White ? " w " : " b ";
	
	//castling rights
	if(m_castlingRights == NoRights) {
		fen += "- ";
	} else {
		if(m_castlingRights & WhiteKingside) fen += 'K';
		if(m_castlingRights & WhiteQueenside) fen += 'Q';
		if(m_castlingRights & BlackKingside) fen += 'k';
		if(m_castlingRights & BlackQueenside) fen += 'q';
		fen += ' ';
	}
	
	//en passant square
	if(m_epSquare == NoEPSquare) {
		fen += "- ";
	} else {
		fen += 'a' + (m_epSquare & 7);
		fen += '1' + ((m_epSquare & 56) >> 3);
		fen += ' ';
	}
	
	//half move clock
	fen += QString::number(m_halfMoveClock);
	
	//full move number not used by board
	fen += " -";
	
	return fen;
}

void Board::setStandardPosition()
{
  // lazy way to implement it
  fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}  

QString Board::toASCII() const
{
	QString ascii;
	
	for(int row = 7; row >= 0; row--) {
		for(int col = 0; col < 8; col++) {
			if(at(col, row) == Empty) {
				ascii += ". ";
			} else {
				ascii += pieceToChar(at(col, row)) + QString(" ");
			}
		}
		ascii += '\n';
	}
	
	ascii += "\nTo Move: ";
	ascii += m_toMove == White ? "White" : "Black"; 
	
	ascii += ", EP Square: ";
	if(m_epSquare != NoEPSquare) {
		ascii += 'a' + (m_epSquare & 7);
		ascii += '1' + ((m_epSquare & 56) >> 3);
	} else {
		ascii += "none";
	}
	
	ascii += ", Castling Rights: ";
	if(m_castlingRights & WhiteKingside) ascii += "K";
	if(m_castlingRights & WhiteQueenside) ascii += "Q";
	if(m_castlingRights & BlackKingside) ascii += "k";
	if(m_castlingRights & BlackQueenside) ascii += "q";
	
	ascii += ", Half Move Clock: ";
	ascii += QString::number(m_halfMoveClock);
	
	return ascii;
}

Piece Board::at(Square s) const
{
  return (m_board[s] != InvalidPiece) ? m_pieceType[m_board[s]] : Empty;
}

Piece Board::at(Coord x, Coord y) const
{
  return at(8 * y + x);
}

Piece Board::atIndex(int i) const
{
  return m_pieceType[i];
}

Square Board::piecePosition(int index) const
{
  return m_piecePosition[index];
}

int Board::pieceCount(Piece p) const
{
  return m_pieceCount[p];
}

Square Board::kingPosition(Color c) const
{
  return (c == White) ? m_piecePosition[16] : m_piecePosition[0];
}

Color Board::toMove() const
{
  return m_toMove;
}

void Board::setToMove(Color c)
{
  m_toMove = c;
}

void Board::swapToMove()
{
  m_toMove = m_toMove == White ? Black : White;
}

Move Board::singleMove(const QString& SAN)
{
  int a,b,i,j;
  Move m;
  Piece p;
  Square to,tempsq;
  Coord fromX=8;
  Coord fromY=8;
  if(SAN.startsWith("O-O-O"))
  {
    m.setMove(kingPosition(toMove()),kingPosition(toMove())-2,Castling);
  }
  else if(SAN.startsWith("O-O"))
  {
    m.setMove(kingPosition(toMove()),kingPosition(toMove())+2,Castling);
  }
  else
  {
    if(SAN.at(0)=='N')p=(toMove()==White)?WhiteKnight:BlackKnight;
    else if(SAN.at(0)=='B')p=(toMove()==White)?WhiteBishop:BlackBishop;
    else if(SAN.at(0)=='R')p=(toMove()==White)?WhiteRook:BlackRook;
    else if(SAN.at(0)=='Q')p=(toMove()==White)?WhiteQueen:BlackQueen;
    else if(SAN.at(0)=='K')p=(toMove()==White)?WhiteKing:BlackKing;
    else p=(toMove()==White)?WhitePawn:BlackPawn;
    i=SAN.findRev(QRegExp("[=][NBRQ]"),-1);
    if(i>-1)
    {
      if(SAN.at(i+1)=='Q')m.setType((toMove()==White)?PromotionWhiteQueen:PromotionBlackQueen);
      else if(SAN.at(i+1)=='R')m.setType((toMove()==White)?PromotionWhiteRook:PromotionBlackRook);
      else if(SAN.at(i+1)=='B')m.setType((toMove()==White)?PromotionWhiteBishop:PromotionBlackBishop);
      else m.setType((toMove()==White)?PromotionWhiteKnight:PromotionBlackKnight);
    }
    i=SAN.findRev(QRegExp("[a-h][1-8]"),-1);
    if(i==-1)return Move();
    to=((SAN.at(i)).latin1()-'a')+8*((SAN.at(i+1)).latin1()-'1');
    j=SAN.findRev(QRegExp("[a-h]"),i-1);
    if(j>-1)fromX=(SAN.at(j)).latin1()-'a';
    j=SAN.findRev(QRegExp("[1-8]"),i-1);
    if(j>-1)fromY=(SAN.at(j)).latin1()-'1';
    i=0;
    switch(p)
    {
      case WhitePawn:
        if(fromX==8)
        {
          if(at(to-8)==WhitePawn)m.setFrom(to-8);
          else m.setFrom(to-16);
        }
        else
        {
					int from = (to & 56) + fromX;
          if(at(from - 8) == WhitePawn) {
						m.setFrom(from - 8);
					} else {
						m.setFrom(from - 16);
						m.setType(DoubleAdvance);
					}
          if(fromX != (to & 7) && at(to) == Empty) {
						m.setType(EnPassant);
					}
        }
        i++;
        break;
      case BlackPawn:
 				if(fromX==8)
        {
          if(at(to+8)==WhitePawn)m.setFrom(to+8);
          else m.setFrom(to+16);
        }
        else
        {
					int from = (to & 56) + fromX;
          if(at(from + 8) == BlackPawn) {
						m.setFrom(from + 8);
					} else {
						m.setFrom(from + 16);
						m.setType(DoubleAdvance);
					}
          if(fromX != (to & 7) && at(to) == Empty) {
						m.setType(EnPassant);
					}
        }
        i++;
        break;
      case WhiteKnight:case BlackKnight:
        for(a=0;a<8;a++)
          if(knightData[to][a])
            if(at(to+knightVectors[a])==p)
              if((fromX==8)||(fromX==((to+knightVectors[a])&7)))
                if((fromY==8)||(fromY==((to+knightVectors[a])/8)))
                {
                  m.setFrom(to+knightVectors[a]);
                  i++;
                }
        break;
      case WhiteBishop:case BlackBishop:
        for(a=0;a<4;a++)
        {
          tempsq=to;
          for(b=0;b<bishopData[to][a];b++) {
						tempsq+=bishopVectors[a];
            if(at(tempsq)==p)
              if((fromX==8)||(fromX==(tempsq&7)))
								if((fromY==8)||(fromY==(tempsq/8)))
                {
                  m.setFrom(tempsq);
                  i++;
                }
						if(at(tempsq)!=Empty) {
							break;
						}
					}
        }
        break;
      case WhiteRook:case BlackRook:
        for(a=0;a<4;a++)
        {
          tempsq=to;
          for(b=0;b<rookData[to][a];b++) {
						tempsq+=rookVectors[a];
						if(at(tempsq)==p)
							if((fromX==8)||(fromX==(tempsq&7)))
								if((fromY==8)||(fromY==(tempsq/8)))
								{
									m.setFrom(tempsq);
									i++;
								}
						if(at(tempsq)!=Empty) {
							break;
						}
					}
        }
        break;
      case WhiteQueen:case BlackQueen:
        for(a=0;a<4;a++)
        {
          tempsq=to;
          for(b=0;b<bishopData[to][a];b++) {
						tempsq+=bishopVectors[a];
            if(at(tempsq)==p)
              if((fromX==8)||(fromX==(tempsq&7)))
								if((fromY==8)||(fromY==(tempsq/8)))
                {
                  m.setFrom(tempsq);
                  i++;
                }
						if(at(tempsq)!=Empty) {
							break;
						}
					}
          tempsq=to;
          for(b=0;b<rookData[to][a];b++) {
						tempsq+=rookVectors[a];
						if(at(tempsq)==p)
							if((fromX==8)||(fromX==(tempsq&7)))
								if((fromY==8)||(fromY==(tempsq/8)))
								{
									m.setFrom(tempsq);
									i++;
								}
						if(at(tempsq)!=Empty) {
							break;
						}
					}
        }
        break;
      case WhiteKing:case BlackKing:
        for(a=0;a<8;a++)
          if(kingData[to][a])
            if(at(to+kingVectors[a])==p)
              if((fromX==8)||(fromX==((to+kingVectors[a])&7)))
                if((fromY==8)||(fromY==((to+kingVectors[a])/8)))
                {
                  m.setFrom(to+kingVectors[a]);
                  i++;
                  break;
                }
        break;
      default:return Move(); // just for clean compile - KMP
    }
    if(i!=1)return Move();
		m.setTo(to);
  }
  
  if(isLegal(m))return m;
  else return Move();
}

QString Board::moveToSAN(const Move& move)
{
	QString moveString = "";
	int from = move.from();
	int to = move.to();
	Piece piece = at(from);
	bool isPawn = ((piece == WhitePawn) || (piece == BlackPawn));
	
	//check for castling
	if(move.isCastling()) {
		if(from < to) {
			moveString = "O-O";
		} else {
			moveString = "O-O-O";
		}
	} else {
		//piece letter & disambiguation
		if(!isPawn) {
			moveString += pieceToChar(piece).upper();
			
			bool column = false;
			bool row = false;
			int vector, range, square;
			
			switch(piece) {
				
				case WhiteKnight:
					for(vector = 0; vector < 8; vector++) {
						if(knightData[to][vector]) {
							square = to + knightVectors[vector];
							if(at(square) == WhiteKnight && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case WhiteBishop:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= bishopData[to][vector]; range++) {
							square = + bishopVectors[vector];
							if(at(square) == WhiteBishop && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case WhiteRook:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= rookData[to][vector]; range++) {
							square = + rookVectors[vector];
							if(at(square) == WhiteRook && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case WhiteQueen:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= bishopData[to][vector]; range++) {
							square = + bishopVectors[vector];
							if(at(square) == WhiteQueen && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
						square = to;
						for(range = 1; range <= rookData[to][vector]; range++) {
							square = + rookVectors[vector];
							if(at(square) == WhiteQueen && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case BlackKnight:
					for(vector = 0; vector < 8; vector++) {
						if(knightData[to][vector]) {
							square = to + knightVectors[vector];
							if(at(square) == BlackKnight && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case BlackBishop:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= bishopData[to][vector]; range++) {
							square = + bishopVectors[vector];
							if(at(square) == BlackBishop && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case BlackRook:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= rookData[to][vector]; range++) {
							square = + rookVectors[vector];
							if(at(square) == BlackRook && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
					
				case BlackQueen:
					for(vector = 0 ; vector < 4; vector++) {
						square = to;
						for(range = 1; range <= bishopData[to][vector]; range++) {
							square = + bishopVectors[vector];
							if(at(square) == BlackQueen && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
						square = to;
						for(range = 1; range <= rookData[to][vector]; range++) {
							square = + rookVectors[vector];
							if(at(square) == BlackQueen && square != from && isLegal(Move(square, to))) {
								if((from & 7) != (square & 7)) {
									column = true;
								} else {
									row = true;
								}
							}
						}
					}
					break;
		
				default:
					break;
			}
			
			if(column) {
				moveString += 'a' + (from & 7);
			}
			
			if(row) {
				moveString += '1' + (from >> 3);
			}
		}
		
		//capture x
		if(at(to) != Empty) {
			if(isPawn) {
				moveString += 'a' + (from & 7);
			}
			moveString += 'x';
		}
		
		//destination square
		moveString += 'a' + (to & 7);
		moveString += '1' + (to >> 3);
	}
	
	HistoryItem historyItem = doMove(move);
	if(isCheck()) {
		moveString += '+';
	}
	undoMove(move, historyItem);
	
	return moveString;
}

Move Board::singleLANMove(QString& LAN)
{
	Move move;
	int from = (LAN[1].latin1() - '1') * 8 + (LAN[0].latin1() - 'a');
	int to = (LAN[3].latin1() - '1') * 8 + (LAN[2].latin1() - 'a');
	move.setFrom(from);
	move.setTo(to);
	
	//check for promotion piece
	if(LAN.length() == 5) {
		Piece piece;
		switch(LAN[4].latin1()) {
			case 'q':
				piece = m_toMove == White ? WhiteQueen : BlackQueen;
				break;
			case 'r':
				piece = m_toMove == White ? WhiteRook : BlackRook;
				break;
			case 'b':
				piece = m_toMove == White ? WhiteBishop : BlackBishop;
				break;
			case 'n':
				piece = m_toMove == White ? WhiteKnight : BlackKnight;
				break;
			default:
				piece = m_toMove == White ? WhiteQueen : BlackQueen;
				break;
		}
		move.setPromotionPiece(piece);
	} else {
		switch(at(from)) {
			//check for pawn double advance
			case WhitePawn:
				if(to - from == 16) {
					move.setType(DoubleAdvance);
				} else if(at(to) == Empty) {
					move.setType(EnPassant);
				}
				break;
			case BlackPawn:
				if(from - to == 16) {
					move.setType(DoubleAdvance);
				} else if(at(to) == Empty) {
					move.setType(EnPassant);
				}
				break;
			//check for castling move
			case WhiteKing:
			case BlackKing:
				if(from - to == 2 || to - from == 2) move.setType(Castling);
				break;
			default:
				break;
		}
	}
	
	if(isLegal(move)) return move;
  else return Move();
}

bool Board::isAttacked(Square sq,Color c) const
{
  int a,b,tempsq,piece;
  if(c==White)
  {
    if(sq&7)
      if(at(sq-9)==WhitePawn)return true;
    if((sq+1)&7)
      if(at(sq-7)==WhitePawn)return true;
    for(a=0;a<4;a++)
    {
      if(knightData[sq][a])
        if(at(sq+knightVectors[a])==WhiteKnight)return true;
      if(kingData[sq][a])
        if(at(sq+kingVectors[a])==WhiteKing)return true;
      tempsq=sq;
      for(b=0;b<bishopData[sq][a];b++) {
				tempsq+=bishopVectors[a];
        if(((piece=at(tempsq))==WhiteBishop)||(piece==WhiteQueen))return true;
				if(piece!=Empty) {
					break;
				}
			}	
      tempsq=sq;
      for(b=0;b<rookData[sq][a];b++) {
        tempsq+=rookVectors[a];
        if(((piece=at(tempsq))==WhiteRook)||(piece==WhiteQueen))return true;
        if(piece!=Empty) {
					break;
				}
			}
    }
    for(a=4;a<8;a++)
    {
      if(knightData[sq][a])
        if(at(sq+knightVectors[a])==WhiteKnight)return true;
      if(kingData[sq][a])
        if(at(sq+kingVectors[a])==WhiteKing)return true;
    }
    return false;
  }
  else
  {
    if(sq&7)
      if(at(sq+7)==BlackPawn)return true;
    if((sq+1)&7)
      if(at(sq+9)==BlackPawn)return true;
    for(a=0;a<4;a++)
    {
      if(knightData[sq][a])
        if(at(sq+knightVectors[a])==BlackKnight)return true;
      if(kingData[sq][a])
        if(at(sq+kingVectors[a])==BlackKing)return true;
      tempsq=sq;
      for(b=0;b<bishopData[sq][a];b++) {
        tempsq+=bishopVectors[a];
        if(((piece=at(tempsq))==BlackBishop)||(piece==BlackQueen))return true;
        if(piece!=Empty) {
					break;
				}
			}
      tempsq=sq;
      for(b=0;b<rookData[sq][a];b++) {
        tempsq+=rookVectors[a];
        if(((piece=at(tempsq))==BlackRook)||(piece==BlackQueen))return true;
        if(piece!=Empty) {
					break;
				}
			}
    }
    for(a=4;a<8;a++)
    {
      if(knightData[sq][a])
        if(at(sq+knightVectors[a])==BlackKnight)return true;
      if(kingData[sq][a])
        if(at(sq+kingVectors[a])==BlackKing)return true;
    }
    return false;
  }
}

bool Board::isLegal(const Move& m)
{
  int b;
  Square from,to,tempsq;
  Piece captured;
  Color color;
  from=m.from();
  to=m.to();
  captured=at(to);
  color=toMove();
  if(color==White)
  {
    if(captured >= WhiteKing && captured <= WhitePawn) {
			return false;
		}
    switch(at(from))
    {
      case WhitePawn:
        if(to-from==16)
        {
          if(from/8!=1)return false;
          if(at(from+8)!=Empty)return false;
          if(captured!=Empty)return false;
        }
        else if(to-from==8)
        {
          if(captured!=Empty)return false;
        }
        else if(to-from==7)
        {
					if(!(from&7))return false;
					if(to != m_epSquare) {
						if(captured<BlackKing)return false;
						if(captured>BlackPawn)return false;
					}
        }
        else if(to-from==9)
        {
					if(!((from+1)&7))return false;
					if(to != m_epSquare) {
						if(captured<BlackKing)return false;
          	if(captured>BlackPawn)return false;
					}
        }
        if(from/8==6)
        {
          if(!m.isPromotion())return false;
        }
        else
          if(m.isPromotion())return false;
        break;
      case WhiteKnight:
        if(abs(from/8-to/8)*abs((from&7)-(to&7))!=2)return false;
        break;
      case WhiteBishop:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteRook:
        if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteQueen:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteKing:
				if(m.isCastling()) {
					if(from == 4 && !isAttacked(4, Black)) {
						if(to == 2 && m_board[3] == InvalidPiece && m_board[2] == InvalidPiece && m_board[1] == InvalidPiece && 
								(m_castlingRights & WhiteQueenside) && !isAttacked(3, Black) && !isAttacked(2, Black)) break;
						if(to == 6 && m_board[5] == InvalidPiece && m_board[6] == InvalidPiece &&
								(m_castlingRights & WhiteKingside) && !isAttacked(5, Black) && !isAttacked(6, Black)) break;
					}
					return false;
				} else {
					if(abs(from/8-to/8)>1)return false;
					if(abs((from&7)-(to&7))>1)return false;
				}
        break;
      default:return false;
    }
  }
  else
  {
    if(captured>=BlackKing && captured<=BlackPawn) {
			return false;
		}
    switch(at(from))
    {
      case BlackPawn:
        if(to-from==-16)
        {
          if(from/8!=6)return false;
          if(at(from-8)!=Empty)return false;
          if(captured!=Empty)return false;
        }
        else if(to-from==-8)
        {
          if(captured!=Empty)return false;
        }
        else if(to-from==-9)
        {
					if(!(from&7))return false;
					if(to != m_epSquare) {
						if(captured<WhiteKing)return false;
						if(captured>WhitePawn)return false;
					}
        }
        else if(to-from==-7)
        {
					if(!((from+1)&7))return false;
					if(to != m_epSquare) {
						if(captured<WhiteKing)return false;
          	if(captured>WhitePawn)return false;
					}
        }
        if(from/8==1)
        {
          if(!m.isPromotion())return false;
        }
        else
          if(m.isPromotion())return false;
        break;
      case BlackKnight:
        if(abs(from/8-to/8)*abs((from&7)-(to&7))!=2)return false;
        break;
      case BlackBishop:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackRook:
        if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackQueen:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackKing:
				if(m.isCastling()) {
					if(from == 60 && !isAttacked(60, White)) {
						if(to == 58 && m_board[59] == InvalidPiece && m_board[58] == InvalidPiece && m_board[57] == InvalidPiece && 
								(m_castlingRights & BlackQueenside) && !isAttacked(59, White) && !isAttacked(58, White)) break;
						if(to == 62 && m_board[61] == InvalidPiece && m_board[62] == InvalidPiece &&
								(m_castlingRights & WhiteKingside) && !isAttacked(61, White) && !isAttacked(62, White)) break;
					}
					return false;
				} else {
					if(abs(from/8-to/8)>1)return false;
					if(abs((from&7)-(to&7))>1)return false;
				}
        break;
      default:return false;
    }
  }
	
	//see if move would leave the king in check
	HistoryItem historyItem = doMove(m);
	bool isLegal = !isAttacked(kingPosition(color),toMove());
	undoMove(m, historyItem);

  return isLegal;
}

HistoryItem Board::doMove(const Move& m)
{
	HistoryItem historyItem(m_board[m.to()], at(m.to()), m_castlingRights,
														m_epSquare, m_halfMoveClock++);
	m_epSquare = NoEPSquare;
	m_castlingRights &= castlingMask[m.from()] & castlingMask[m.to()];
	if(at(m.from()) == WhitePawn || at(m.from()) == BlackPawn){
		m_halfMoveClock = 0;
	}
	
  movePiece(m.from(), m.to());
  
	if (m.isSpecial())
  {
		if(m.isDoubleAdvance()) {
			m_epSquare = (m.from() + m.to()) >> 1;
		} else {
			if (m.isCastling())
				movePiece(m.castlingRookFrom(), m.castlingRookTo());
			else if (m.isEnPassant()) {
				historyItem.setIndex(m_board[m.enPassantSquare()]);
				historyItem.setPiece(at(m.enPassantSquare()));
				m_board[m.enPassantSquare()] = InvalidPiece;
			}
			else  /* promotion */
				promotePiece(m.to(), m.promotionPiece());
		}
  }
  swapToMove();
	
	return historyItem;
}

void Board::undoMove(const Move& m, const HistoryItem& historyItem)
{
	movePiece(m.to(), m.from());
  
	if (m.isCastling()) {
    movePiece(m.castlingRookTo(), m.castlingRookFrom());
	}
	else if(m.isPromotion()) {
		promotePiece(m.from(), m_toMove == White ? BlackPawn : WhitePawn);
	}
	
	if(historyItem.piece() != Empty) {
		//restore captured piece
		if (m.isEnPassant())
			restorePiece(m.enPassantSquare(), historyItem.piece(),
										historyItem.index());
		else {
			restorePiece(m.to(), historyItem.piece(), historyItem.index());
			if(m.isPromotion()) {
				promotePiece(m.from(), m_toMove == White ? BlackPawn : WhitePawn);
			}
		}
	}
	
  swapToMove();
	m_epSquare = historyItem.epSquare();
	m_castlingRights = historyItem.castlingRights();
	m_halfMoveClock = historyItem.halfMoveClock();
}


bool Board::isCheck()
{
	 return isAttacked(kingPosition(m_toMove), (m_toMove == White) ? Black : White);
}

bool Board::isCheckmate()
{
	return false;
}

void Board::movePiece(Square from, Square to)
{
  if (m_board[to] != InvalidPiece) // capture
  {
    m_pieceCount[m_pieceType[m_board[to]]]--;
    m_pieceType[m_board[to]] = Empty;
		m_halfMoveClock = 0;
  }
  m_board[to] = m_board[from];
	m_piecePosition[m_board[from]] = to;
  m_board[from] = InvalidPiece;
}

void Board::restorePiece(Square from, Piece piece, int index)
{
  m_pieceCount[piece]++;
  m_pieceType[index] = piece;
  m_piecePosition[index] = from;
  m_board[from] = index;
}

void Board::promotePiece(Square square, Piece promoted)
{
	m_pieceCount[m_pieceType[m_board[square]]]--;
	m_pieceCount[promoted]++;
	m_pieceType[m_board[square]] = promoted;
}
