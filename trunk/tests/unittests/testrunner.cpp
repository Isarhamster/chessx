/***************************************************************************
            testrunner.cpp  -  runs all ChessX unit tests
                             -------------------
    begin                : 20/01/2007
    copyright            : (C) 2007 Heinz R. Hopfgartner 
                           <heinz.hopfgartner@gmx.at>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
   This is the main program which runs all the unit tests for ChessX.

Adding new test cases has to be done in the following way:
1. Create the test class (example: PlayerDatabaseTest)
2. #include the new test class within the testrunner
3. Add an instance of the new test class to the testrunner
   example: PlayerDatabaseTest playerDBtests;
4. Add the test execution to the testrunner
   example: QTest::qExec( &playerDBtests, argc, argv );
Do not forget to update the testrunner.pro - add the source files!
 */

#include <QtTest/QtTest>
#include "playerdatabasetest.h"
#include "pgndatabasetest.h"
#include "spellcheckertests.h"
#include "boardtest.cpp"
#include "databaseconversiontest.h"

int main(int argc, char *argv[])
{

  PlayerDatabaseTest playerDBtests;
  PgnDatabaseTest pgnDBtests;
  SpellCheckerTests spellCheckerTests;
  BoardTest boardTests;
  DatabaseConversionTest databaseConversionTests;

  QTest::qExec( &playerDBtests, argc, argv );
  QTest::qExec( &pgnDBtests, argc, argv );
  QTest::qExec( &spellCheckerTests, argc, argv );
  //QFATAL Error with this test - therefore commented
  // QTest::qExec( &boardTests, argc, argv );
  QTest::qExec( &databaseConversionTests, argc, argv );

  return 0;
}

