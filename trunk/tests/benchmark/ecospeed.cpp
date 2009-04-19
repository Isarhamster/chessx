#include "game.h"

int main(int argc, char* argv[])
{
   Game::loadEcoFile("../../data/chessx.eco");
   qDebug("Done");
   return 0;
}