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
   Unit tests for methods tha change the database in the PlayerDatabase class.
*/

#include "playerdatabasechangestest.h"
#include "playerdatabase.h"
#include "partialdate.h"
#include <qimage.h>
#include <qfile.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PlayerDatabaseChangesTest );

PlayerDatabase change;

void PlayerDatabaseChangesTest::setUp()
{
  db_name = "./data/small/change";
  if( !QFile( "./data/small/change.cpd" ).exists() )
    change.create( db_name );
  else
    change.open( db_name );
}

void PlayerDatabaseChangesTest::tearDown()
{
  change.close();
}

void PlayerDatabaseChangesTest::testAddPlayer()
{
  CPPUNIT_ASSERT( change.add( "Hopfgartner, Heinz R" ) );
  change.commit();
}

void PlayerDatabaseChangesTest::testFindPlayers()
{
  QStringList qsl = change.findPlayers("Hop");
  CPPUNIT_ASSERT_EQUAL_MESSAGE( "findPlayers(Hop)", (size_t)1 , qsl.count() );
  qsl = change.findPlayers("HOP", 10, PlayerDatabase::IgnoreCase);
  CPPUNIT_ASSERT_EQUAL_MESSAGE( "findPlayers(HOP,10,IgnoreCase)", (size_t)1 , qsl.count() );
  qsl = change.findPlayers("HOP", 10, PlayerDatabase::RespectCase);
  CPPUNIT_ASSERT_EQUAL_MESSAGE( "findPlayers(HOP,10,RespectCase)", (size_t)0 , qsl.count() );
}

void PlayerDatabaseChangesTest::testRollback()
{
  uint count = 1;
  CPPUNIT_ASSERT_EQUAL_MESSAGE( "before adding" , count , change.count() );
  change.add( "Larsen,Bent" );
  change.rollback();
  change.commit();
  CPPUNIT_ASSERT_EQUAL_MESSAGE( "after adding" , count , change.count() );
}

void PlayerDatabaseChangesTest::testCompact()
{
  CPPUNIT_FAIL( "Method in PlayerDatabase not implemented" );
}

void PlayerDatabaseChangesTest::testDateOfBirth()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  PartialDate dob( 1968, 5, 30 );
  change.setDateOfBirth( dob );
  change.commit();
  PartialDate savedDob = change.dateOfBirth();
  CPPUNIT_ASSERT_EQUAL( dob.asString() , savedDob.asString() );
}

void PlayerDatabaseChangesTest::testDateOfDeath()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  PartialDate dod( 2080, 5, 31 );
  change.setDateOfDeath( dod );
  change.commit();
  PartialDate savedDod = change.dateOfDeath();
  CPPUNIT_ASSERT_EQUAL( dod.asString() , savedDod.asString() );
}

void PlayerDatabaseChangesTest::testCountry()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  QString country( "AUT" );
  change.setCountry( country );
  change.commit();
  QString savedCountry = change.country();
  CPPUNIT_ASSERT_EQUAL( country , savedCountry );
}


void PlayerDatabaseChangesTest::testTitle()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  QString title( "hgm" );
  change.setTitle( title );
  change.commit();
  QString savedTitle = change.title();
  CPPUNIT_ASSERT_EQUAL( title , savedTitle );
}

void PlayerDatabaseChangesTest::testPhoto()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  CPPUNIT_ASSERT( !change.hasPhoto() );
  QImage myPhoto( "./data/small/playerphotos/Hopfgartner, Heinz R.jpg" );
  change.setPhoto( myPhoto );
  change.commit();
  CPPUNIT_ASSERT( change.hasPhoto() );
}

void PlayerDatabaseChangesTest::testBiography()
{
  change.setCurrent( "Hopfgartner, Heinz R" );
  CPPUNIT_ASSERT( !change.hasBiography() );
  QString myBio("I am still trying to improve my chess." );
  change.setBiography( myBio );
  change.commit();
  CPPUNIT_ASSERT( change.hasBiography() );


  QString savedBio = change.biography();
  CPPUNIT_ASSERT_EQUAL( myBio , savedBio );

  QString appendBio( " Chess is fun!" );
  change.appendToBiography( appendBio );
  change.commit();
  QString newBio = myBio + appendBio;
  CPPUNIT_ASSERT_EQUAL( newBio , change.biography() );
}



