#include <qstring.h>
#include "common.h"
#include "board.h"

static bool initialized=false;

static const int knightVectors[8]={-17,-15,-10,-6,6,10,15,17};
static const int bishopVectors[4]={-9,-7,7,9};
static const int rookVectors[4]={-8,-1,1,8};
static const int kingVectors[8]={-9,-8,-7,-1,1,7,8,9};

static int knightData[64][8];
static int bishopData[64][4];
static int rookData[64][4];
static int kingData[64][8];

class MoveList
{
public:
  MoveList();
  void initialize() const;
  Move singleMove(Board& brd,QString& SAN) const;
  bool isAttacked(Board& brd,Square sq,Color c) const;
  bool isLegal(Board& brd,Move& m) const;
private:
  void initKnights() const;
  void initBishops() const;
  void initRooks() const;
  void initKings() const;
};
