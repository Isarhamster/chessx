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
#include "resourcepath.h"
#include "playerdatabase.h"

void PlayerDatabaseTest::testBasics()
{
    QString path(RESOURCE_PATH "small/players");
    PlayerDatabase pdb;
    
    QVERIFY(pdb.open(path));

    QVERIFY(pdb.count() == 63);
    QVERIFY(pdb.exists("Thal, Olaf"));
    QVERIFY(!pdb.exists("TRUSDFEADFA, WSDFASDF"));

    pdb.setCurrent("Thal, Olaf");
    QVERIFY(pdb.current() == "Thal, Olaf");

    pdb.close();
    // after closing - opening should be possible - no better idea for another test
    QVERIFY(pdb.open(path));

    pdb.close();
}

void PlayerDatabaseTest::testCreate()
{
    QTemporaryDir tmpDir;
    PlayerDatabase pdb;
    auto path = tmpDir.path() + "/playerdatabase_create";
    QVERIFY(pdb.create(path));
    pdb.close();
    QVERIFY(pdb.removeDatabase(path));
}
