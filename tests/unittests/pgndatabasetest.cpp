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

#include "pgndatabasetest.h"
#include <QTemporaryDir>

#include "resourcepath.h"

#include "pgndatabase.h"
#include "memorydatabase.h"
#include "gamex.h"
#include "filter.h"
#include "search.h"
#include "settings.h"

void PgnDatabaseTest::initTestCase()
{
    AppSettings = new Settings;
}

void PgnDatabaseTest::init() {}
void PgnDatabaseTest::cleanup() {}

void PgnDatabaseTest::cleanupTestCase() {}

void PgnDatabaseTest::testCreateDatabase()
{
    QTemporaryDir tmpDir;

    PgnDatabase* db = new PgnDatabase(false);
//    QVERIFY(db->open(tmpDir.path() + "pgndatabase_new1.pgn", false));
    QCOMPARE(db->count(), 0);
    delete db;
}

void PgnDatabaseTest::testLoad()
{
    const QString path(RESOURCE_PATH "game1.pgn");

    PgnDatabase* db = new PgnDatabase(false);
    QVERIFY(db->open(path, false));

    QCOMPARE(db->filename(), path);

    QVERIFY(db->parseFile());

    GameX game;
    QCOMPARE(2, db->count());
    QCOMPARE(db->loadGame(1, game), true);
    QCOMPARE(db->loadGame(0, game), true);
    QCOMPARE(db->loadGame(2, game), false);
    delete db;
}

void PgnDatabaseTest::testCopyGameIntoNewDB()
{
    auto src = new PgnDatabase(false);
    QVERIFY(src->open(RESOURCE_PATH "game1.pgn", false));
    QVERIFY(src->parseFile());

    QTemporaryDir tmpDir;
    auto dst = new MemoryDatabase();
//    dst->open(tmpDir.path() + "/pgndatabase/new.pgn", false);

    GameX game;
    QVERIFY(src->loadGame(1, game));
    dst->appendGame(game);
    QVERIFY(src->loadGame(0, game));
    dst->appendGame(game);

    // FIXME -- Test below is failing
//    QCOMPARE(dst->count(), 2);

    dst->remove(1);
    // FIXME -- Test below is failing
//    QCOMPARE(dst->count(), 1);
    dst->remove(0);
    // FIXME -- Test below is failing
//    QCOMPARE(dst->count(), 0);

    delete dst;
    delete src;
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


