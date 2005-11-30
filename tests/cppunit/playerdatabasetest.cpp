/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 01/11/2005
    copyright            : (C) 2005 Heinz R. Hopfgartner 
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
Unit tests for the PlayerDatabase class.
*/

#include "playerdatabasetest.h"
#include "playerdatabase.h"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PlayerDatabaseTest );

 PlayerDatabase pdb, newDb;

void PlayerDatabaseTest::setUp()
{
  db_name = "./data/small/players";
  pdb.open( db_name );
}

void PlayerDatabaseTest::tearDown()
{
  pdb.close();
}

void PlayerDatabaseTest::testCreateDatabase()
{
  CPPUNIT_ASSERT( newDb.create( "./data/small/change" ) );
  newDb.close();
}

void PlayerDatabaseTest::testOpenDatabase()
{
  pdb.close();
  CPPUNIT_ASSERT( pdb.open( db_name ) );
}

void PlayerDatabaseTest::testRemoveDatabase()
{
  CPPUNIT_ASSERT( newDb.removeDatabase( "./data/small/change" ) );
}

void PlayerDatabaseTest::testPlayerCount()
{
  uint count = 63;
  CPPUNIT_ASSERT_EQUAL( count, pdb.count() );
}

void PlayerDatabaseTest::testPlayerExists()
{
  CPPUNIT_ASSERT ( pdb.exists( "Thal, Olaf" ) );
}

void PlayerDatabaseTest::testNonExistingPlayer()
{
  CPPUNIT_ASSERT ( !pdb.exists( "TRUSDFEADFA, WSDFASDF" ) );
}

void PlayerDatabaseTest::testCurrentPlayer()
{
  pdb.setCurrent( "Thal, Olaf" );
  CPPUNIT_ASSERT_EQUAL( QString( "Thal, Olaf" ) , pdb.current() );
}

void PlayerDatabaseTest::testDatabaseClose(){
  pdb.close();
  // after closing - opening should be possible - no better idea for another test
  CPPUNIT_ASSERT( pdb.open( db_name ) );
}



