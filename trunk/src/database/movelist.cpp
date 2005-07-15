#include "movelist.h"

MoveList::MoveList()
{
  if(!initialized)initialize();
}

void MoveList::initialize() const
{
  if(initialized)return;
  initKnights();
  initBishops();
  initRooks();
  initKings();
  initialized=true;
}

Move MoveList::singleMove(Board& brd,QString& SAN) const
{
  int a,b,i;
  Move m;
  Piece p;
  Square to,tempsq;
  Coord fromX=8;
  Coord fromY=8;
  if(SAN.startsWith("O-O-O"))
  {
    m.setMove(brd.kingPosition(brd.toMove()),brd.kingPosition(brd.toMove())-2,Castling);
  }
  else if(SAN.startsWith("O-O"))
  {
    m.setMove(brd.kingPosition(brd.toMove()),brd.kingPosition(brd.toMove())+2,Castling);
  }
  else
  {
    if(SAN.at(0)=='N')p=(brd.toMove()==White)?WhiteKnight:BlackKnight;
    else if(SAN.at(0)=='B')p=(brd.toMove()==White)?WhiteBishop:BlackBishop;
    else if(SAN.at(0)=='R')p=(brd.toMove()==White)?WhiteRook:BlackRook;
    else if(SAN.at(0)=='Q')p=(brd.toMove()==White)?WhiteQueen:BlackQueen;
    else if(SAN.at(0)=='K')p=(brd.toMove()==White)?WhiteKing:BlackKing;
    else p=(brd.toMove()==White)?WhitePawn:BlackPawn;
    i=SAN.findRev("[=][NBRQ]",-1);
    if(i>-1)
    {
      if(SAN.at(i+1)=='Q')m.setType((brd.toMove()==White)?PromotionWhiteQueen:PromotionBlackQueen);
      else if(SAN.at(i+1)=='R')m.setType((brd.toMove()==White)?PromotionWhiteRook:PromotionBlackRook);
      else if(SAN.at(i+1)=='B')m.setType((brd.toMove()==White)?PromotionWhiteBishop:PromotionBlackBishop);
      else m.setType((brd.toMove()==White)?PromotionWhiteKnight:PromotionBlackKnight);
    }
    i=SAN.findRev("[a-h][1-8]",-1);
    if(i==-1)return Move();
    to=((SAN.at(i)).latin1()-'a')+8*((SAN.at(i+1)).latin1()-'1');
    i=SAN.findRev("[a-h]",i-1);
    if(i>-1)fromX=(SAN.at(i)).latin1()-'a';
    i=SAN.findRev("[1-8]",i-1);
    if(i>-1)fromY=(SAN.at(i)).latin1()-'1';
    i=0;
    switch(p)
    {
      case WhitePawn:
        if(fromX==8)
        {
          if(brd.at(to-8)==WhitePawn)m.setFrom(to-8);
          else m.setFrom(to-16);
        }
        else
        {
          m.setFrom(to&56+fromX-8);
          if(brd.at(to)==Empty)m.setType(EnPassant);
        }
        i++;
        break;
      case BlackPawn:
        if(fromX==8)
        {
          if(brd.at(to+8)==BlackPawn)m.setFrom(to+8);
          else m.setFrom(to+16);
        }
        else
        {
          m.setFrom(to&56+fromX+8);
          if(brd.at(to)==Empty)m.setType(EnPassant);
        }
        i++;
        break;
      case WhiteKnight:case BlackKnight:
        for(a=0;a<8;a++)
          if(knightData[to][a])
            if(brd.at(to+knightVectors[a])==p)
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
          for(b=0;b<bishopData[to][a];b++)
            do
            {
              tempsq+=bishopVectors[a];
              if(brd.at(tempsq)==p)
                if((fromX==8)||(fromX==(tempsq&7)))
                  if((fromY==8)||(fromY==(tempsq/8)))
                  {
                    m.setFrom(tempsq);
                    i++;
                  }
            }while(brd.at(tempsq)!=Empty);
        }
        break;
      case WhiteRook:case BlackRook:
        for(a=0;a<4;a++)
        {
          tempsq=to;
          for(b=0;b<rookData[to][a];b++)
            do
            {
              tempsq+=rookVectors[a];
              if(brd.at(tempsq)==p)
                if((fromX==8)||(fromX==(tempsq&7)))
                  if((fromY==8)||(fromY==(tempsq/8)))
                  {
                    m.setFrom(tempsq);
                    i++;
                  }
            }while(brd.at(tempsq)!=Empty);
        }
        break;
      case WhiteQueen:case BlackQueen:
        for(a=0;a<4;a++)
        {
          tempsq=to;
          for(b=0;b<bishopData[to][a];b++)
            do
            {
              tempsq+=bishopVectors[a];
              if(brd.at(tempsq)==p)
                if((fromX==8)||(fromX==(tempsq&7)))
                  if((fromY==8)||(fromY==(tempsq/8)))
                  {
                    m.setFrom(tempsq);
                    i++;
                  }
            }while(brd.at(tempsq)!=Empty);
          tempsq=to;
          for(b=0;b<rookData[to][a];b++)
            do
            {
              tempsq+=rookVectors[a];
              if(brd.at(tempsq)==p)
                if((fromX==8)||(fromX==(tempsq&7)))
                  if((fromY==8)||(fromY==(tempsq/8)))
                  {
                    m.setFrom(tempsq);
                    i++;
                  }
            }while(brd.at(tempsq)!=Empty);
        }
        break;
      case WhiteKing:case BlackKing:
        for(a=0;a<8;a++)
          if(kingData[to][a])
            if(brd.at(to+kingVectors[a])==p)
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
  }
  m.setTo(to);
  if(isLegal(brd,m))return m;
  else return Move();
}

bool MoveList::isAttacked(Board& brd,Square sq,Color c) const
{
  int a,b,tempsq,piece;
  if(c==White)
  {
    if(sq&7)
      if(brd.at(sq-9)==WhitePawn)return true;
    if((sq+1)&7)
      if(brd.at(sq-7)==WhitePawn)return true;
    for(a=0;a<4;a++)
    {
      if(knightData[sq][a])
        if(brd.at(sq+knightVectors[a])==WhiteKnight)return true;
      if(kingData[sq][a])
        if(brd.at(sq+kingVectors[a])==WhiteKing)return true;
      tempsq=sq;
      for(b=0;b<bishopData[sq][a];b++)
        do
        {
          tempsq+=bishopVectors[a];
          if((piece=brd.at(tempsq)==WhiteBishop)||(piece==WhiteQueen))return true;
        }while(piece!=Empty);
      tempsq=sq;
      for(b=0;b<rookData[sq][a];b++)
        do
        {
          tempsq+=rookVectors[a];
          if((piece=brd.at(tempsq)==WhiteRook)||(piece==WhiteQueen))return true;
        }while(piece!=Empty);
    }
    for(a=4;a<8;a++)
    {
      if(knightData[sq][a])
        if(brd.at(sq+knightVectors[a])==WhiteKnight)return true;
      if(kingData[sq][a])
        if(brd.at(sq+kingVectors[a])==WhiteKing)return true;
    }
    return false;
  }
  else
  {
    if(sq&7)
      if(brd.at(sq+7)==BlackPawn)return true;
    if((sq+1)&7)
      if(brd.at(sq+9)==BlackPawn)return true;
    for(a=0;a<4;a++)
    {
      if(knightData[sq][a])
        if(brd.at(sq+knightVectors[a])==BlackKnight)return true;
      if(kingData[sq][a])
        if(brd.at(sq+kingVectors[a])==BlackKing)return true;
      tempsq=sq;
      for(b=0;b<bishopData[sq][a];b++)
        do
        {
          tempsq+=bishopVectors[a];
          if((piece=brd.at(tempsq)==BlackBishop)||(piece==BlackQueen))return true;
        }while(piece!=Empty);
      tempsq=sq;
      for(b=0;b<rookData[sq][a];b++)
        do
        {
          tempsq+=rookVectors[a];
          if((piece=brd.at(tempsq)==BlackRook)||(piece==BlackQueen))return true;
        }while(piece!=Empty);
    }
    for(a=4;a<8;a++)
    {
      if(knightData[sq][a])
        if(brd.at(sq+knightVectors[a])==BlackKnight)return true;
      if(kingData[sq][a])
        if(brd.at(sq+kingVectors[a])==BlackKing)return true;
    }
    return false;
  }
}

bool MoveList::isLegal(Board& brd,Move& m) const // No castle/ep validation - KMP
{
  int b;
  Square from,to,tempsq;
  Piece captured;
  Color color;
  from=m.from();
  to=m.to();
  captured=brd.at(m.to());
  color=brd.toMove();
  if(color==White)
  {
    if(captured>=WhiteKing)return false;
    if(captured<=WhitePawn)return false;
    switch(brd.at(from))
    {
      case WhitePawn:
        if(to-from==16)
        {
          if(from/8!=1)return false;
          if(brd.at(from+8)!=Empty)return false;
          if(captured!=Empty)return false;
        }
        else if(to-from==8)
        {
          if(captured!=Empty)return false;
        }
        else if(to-from==7)
        {
          if(!(from&7))return false;
          if(captured<BlackKing)return false;
          if(captured>BlackPawn)return false;
        }
        else if(to-from==9)
        {
          if(!((from+1)&7))return false;
          if(captured<BlackKing)return false;
          if(captured>BlackPawn)return false;
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
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteRook:
        if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteQueen:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case WhiteKing:
        if(abs(from/8-to/8)>1)return false;
        if(abs((from&7)-(to&7))>1)return false;
        break;
      default:return false;
    }
  }
  else
  {
    if(captured>=BlackKing)return false;
    if(captured<=BlackPawn)return false;
    switch(brd.at(from))
    {
      case BlackPawn:
        if(to-from==-16)
        {
          if(from/8!=6)return false;
          if(brd.at(from-8)!=Empty)return false;
          if(captured!=Empty)return false;
        }
        else if(to-from==-8)
        {
          if(captured!=Empty)return false;
        }
        else if(to-from==-9)
        {
          if(!(from&7))return false;
          if(captured<BlackKing)return false;
          if(captured>BlackPawn)return false;
        }
        else if(to-from==-7)
        {
          if(!((from+1)&7))return false;
          if(captured<BlackKing)return false;
          if(captured>BlackPawn)return false;
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
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackRook:
        if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackQueen:
        if(from/8+(from&7)==to/8+(to&7))
        {
          b=(from>to)?-7:7;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8-(from&7)==to/8-(to&7))
        {
          b=(from>to)?-9:9;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if(from/8==to/8)
        {
          b=(from>to)?-1:1;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else if((from&7)==(to&7))
        {
          b=(from>to)?-8:8;
          for(tempsq=from+b;tempsq!=to;tempsq+=b)
            if(brd.at(tempsq)!=Empty)return false;
        }
        else return false;
        break;
      case BlackKing:
        if(abs(from/8-to/8)>1)return false;
        if(abs((from&7)-(to&7))>1)return false;
        break;
      default:return false;
    }
  }
  brd.doStandardMove(m);
  if(isAttacked(brd,brd.kingPosition(color),brd.toMove()))
  {
    brd.undoStandardMove(m,captured);
    return false;
  }
  brd.undoStandardMove(m,captured);
  return true;
}

void MoveList::initKnights() const
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

void MoveList::initBishops() const
{
  int x,y;
  for(x=0;x<8;x++)
    for(y=0;y<8;y++)
    {
      bishopData[x+y*8][0]=min(x,y);
      bishopData[x+y*8][1]=min(7-x,y);
      bishopData[x+y*8][2]=min(x,7-y);
      bishopData[x+y*8][3]=min(7-x,7-y);
    }
}

void MoveList::initRooks() const
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

void MoveList::initKings() const
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
