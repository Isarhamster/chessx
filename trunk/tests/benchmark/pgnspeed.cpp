#include "pgndatabase.h"

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
     qDebug("Usage: pgnspeed <file>.\n");
     return -1;
   }
   PgnDatabase DB;
   DB.open(argv[1]);
   qDebug("%d games.\n", DB.count());
   return 0;
}