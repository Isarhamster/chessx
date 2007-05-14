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

#ifndef PLAYERDATABASETEST_H
#define PLAYERDATABASETEST_H

#include <QtTest/QtTest>
#include "../../src/database/playerdatabase.h"

class PlayerDatabaseTest: public QObject
{

	Q_OBJECT

	PlayerDatabase pdb, newDb;
	QString db_name;

private slots:
	void initTestCase();
	void init();
	void cleanup();
	void cleanupTestCase();

	void testCreateDatabase();
	void testOpenDatabase();
	void testRemoveDatabase();
	void testPlayerCount();
	void testPlayerExists();
	void testNonExistingPlayer();
	void testCurrentPlayer();
	void testDatabaseClose();

};

#endif
