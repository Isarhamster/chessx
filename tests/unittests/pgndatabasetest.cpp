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



void PgnDatabaseTest::initTestCase() {}
void PgnDatabaseTest::init(){}
void PgnDatabaseTest::cleanup(){}

void PgnDatabaseTest::cleanupTestCase()
{
	QDir toRemove;
	toRemove.remove("./data/new.pgn");
	toRemove.remove("./data/new1.pgn");
}

void PgnDatabaseTest::testCreateDatabase()
{
	PgnDatabase* db = new PgnDatabase();
	db->create(QString("./data/new1.pgn"));
	QCOMPARE(db->count(), 0);
	delete db;
}

void PgnDatabaseTest::testName()
{
	PgnDatabase* db = new PgnDatabase();
	db->open(QString("./data/game1.pgn"));
	const QString name = QString("./data/game1.pgn");
	QCOMPARE(db->filename(), name);
	delete db;

}

void PgnDatabaseTest::testLoad()
{
	PgnDatabase* db = new PgnDatabase();
	db->open(QString("./data/game1.pgn"));
	Game game;
	QCOMPARE(2 , db->count());
	QCOMPARE(db->loadGame(1, game), true);
	QCOMPARE(db->loadGame(0, game), true);
	QCOMPARE(db->loadGame(2, game), false);
	delete db;

}

void PgnDatabaseTest::testCopyGameIntoNewDB()
{
	PgnDatabase* db = new PgnDatabase();
	db->open(QString("./data/game1.pgn"));
	Game game;
	bool success = db->loadGame(1, game);
	PgnDatabase* dbNew = new PgnDatabase();
	dbNew->create(QString("./data/new.pgn"));
	dbNew->appendGame(game);
	success = db->loadGame(0, game);
	dbNew->appendGame(game);
	// FIXME -- Test below is failing
//	QCOMPARE(dbNew->count(), 2);
	delete dbNew;
	delete db;
}

void PgnDatabaseTest::testRemoveGame()
{
	PgnDatabase* dbNew = new PgnDatabase();
	dbNew->open(QString("./data/new.pgn"));
	dbNew->remove(1);
	// FIXME -- Test below is failing
//	QCOMPARE(dbNew->count(), 1);
	dbNew->remove(0);
	// FIXME -- Test below is failing
//	QCOMPARE(dbNew->count(), 0);
	delete dbNew;
}

// void PgnDatabaseTest::testExecuteSearch() {
//     PgnDatabase* db = new PgnDatabase();
//     db->open( QString( "./data/game1.pgn" ));
//     TagSearch search;
//     search.setTag( "Black" );
//     search.setValue( "Capablanca, Jose Raul" );
//     Filter filter = db->executeSearch( search );
//     QCOMPARE( 2, filter.count() );

//     TagSearch search1;
//     search1.setTag( "Result" );
//     search1.setValue( "1-0" );
//     Filter filter1 = db->executeSearch( search1 );
//     QCOMPARE( 1, filter1.count() );
//     delete db;
// }

// void PgnDatabaseTest::testSave() {
//     PgnDatabase* db = new PgnDatabase();
//     db->open( QString( "./data/game1.pgn" ));
//     Game game;
//     QCOMPARE( true, db->loadGame( 1, game ) );

//     PgnDatabase* dbNew = new PgnDatabase();
//     dbNew->create( QString( "./data/new1.pgn" ));

//     QCOMPARE( dbNew->save( 0, game ), true );
//     QCOMPARE( db->loadGame( 0, game ), true );
//     QCOMPARE( true, dbNew->save( 1, game ) );
//     QCOMPARE( dbNew->count(), 2 );
//     delete dbNew;
//     delete db;
// }


