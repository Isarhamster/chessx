/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 30/04/2006
    copyright            : (C) 2006 Heinz R. Hopfgartner 
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
Unit tests for the PgnDatabase class.
*/

#include <qdir.h>

#include "pgndatabasetest.h"
#include "pgndatabase.h"
#include "game.h"
#include "filter.h"
#include "search.h"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PgnDatabaseTest );

void PgnDatabaseTest::setUp() {}

void PgnDatabaseTest::tearDown() {
    QDir toRemove;
    toRemove.remove( "./data/new.pgn" );
}

void PgnDatabaseTest::testCreateDatabase() {
    PgnDatabase* db = new PgnDatabase( "./data/game1.pgn" );
    CPPUNIT_ASSERT_EQUAL( 2 , db->count() );
    delete db;
}

void PgnDatabaseTest::testCopyGameIntoNewDB() {
    PgnDatabase* db = new PgnDatabase( "./data/game1.pgn" );
    Game game;
    bool success = db->load( 1, game );
    PgnDatabase* dbNew = new PgnDatabase( "./data/new.pgn" );
    dbNew->add
    ( game );
    success = db->load( 0, game );
    dbNew->add
    ( game );
    CPPUNIT_ASSERT_EQUAL( 2 , dbNew->count() );
    CPPUNIT_FAIL( "Game index 1 - is the second game in the db! Is this OK?" );
    delete dbNew, db;
}

void PgnDatabaseTest::testRemoveGame() {
    PgnDatabase* dbNew = new PgnDatabase( "./data/new.pgn" );
    dbNew->remove
    ( 1 );
    CPPUNIT_ASSERT_EQUAL( 1 , dbNew->count() );
    dbNew->remove
    ( 0 );
    CPPUNIT_ASSERT_EQUAL( 0 , dbNew->count() );
    delete dbNew;
}

void PgnDatabaseTest::testExecuteSearch() {
    PgnDatabase* db = new PgnDatabase( "./data/game1.pgn" );
    TagSearch search;
    search.setTag( "Black" );
    search.setValue( "Capablanca, Jose Raul" );
    Filter filter = db->executeSearch( search );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Black search: ", 2, filter.count() );

    TagSearch search1;
    search1.setTag( "Result" );
    search1.setValue( "1-0" );
    Filter filter1 = db->executeSearch( search1 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "Result search: ", 1, filter1.count() );
    delete db;
}

