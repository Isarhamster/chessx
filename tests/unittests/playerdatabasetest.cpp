/***************************************************************************
            playerdatabasetest.cpp  -  testcases for PlayerDatabase
                             -------------------
    begin                : 16/01/2007
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
   Unit tests for the PlayerDatabase class.
*/

#include "playerdatabasetest.h"

void PlayerDatabaseTest::initTestCase()
{
	db_name = QString("./data/small/players");
}

void PlayerDatabaseTest::init()
{
	pdb.open(db_name);
}

void PlayerDatabaseTest::cleanup()
{
	pdb.close();
}

void PlayerDatabaseTest::testCreateDatabase()
{
	QVERIFY(newDb.create(QString("./data/small/change")));
	newDb.close();
}

void PlayerDatabaseTest::testOpenDatabase()
{
	pdb.close();
	QVERIFY(pdb.open(db_name));
}

void PlayerDatabaseTest::testRemoveDatabase()
{
	QVERIFY(newDb.removeDatabase("./data/small/change"));
}

void PlayerDatabaseTest::testPlayerCount()
{
	uint count = 63;
	QCOMPARE(count, pdb.count());
}

void PlayerDatabaseTest::testPlayerExists()
{
	QVERIFY(pdb.exists("Thal, Olaf"));
}

void PlayerDatabaseTest::testNonExistingPlayer()
{
	QVERIFY(!pdb.exists("TRUSDFEADFA, WSDFASDF"));
}

void PlayerDatabaseTest::testCurrentPlayer()
{
	pdb.setCurrent("Thal, Olaf");
	QCOMPARE(QString("Thal, Olaf") , pdb.current());
}

void PlayerDatabaseTest::testDatabaseClose()
{
	pdb.close();
	// after closing - opening should be possible - no better idea for another test
	QVERIFY(pdb.open(db_name));
}

void PlayerDatabaseTest::cleanupTestCase()
{
	QDir toRemove;
	//TODO: das ist für die Änderungstest playerdb!
//   toRemove.remove( "./data/small/change.cpd" );
//   toRemove.remove( "./data/small/change.cpm" );
//   toRemove.remove( "./data/small/players_converted.cpd" );
//   toRemove.remove( "./data/small/players_converted.cpm" );
}



