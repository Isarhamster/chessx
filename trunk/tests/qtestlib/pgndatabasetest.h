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

#ifndef PGNDATABASETEST_H
#define PGNDATABASETEST_H

#include <QtTest/QtTest>


class PgnDatabaseTest : public QObject
{

 Q_OBJECT

private slots:
  void initTestCase();
  void init();
  void cleanup();
  void cleanupTestCase();

  void testCreateDatabase();
  void testName();
  void testLoad();
  void testCopyGameIntoNewDB();
  void testRemoveGame();
  //  void testExecuteSearch();
  void testSave();

};

#endif 
