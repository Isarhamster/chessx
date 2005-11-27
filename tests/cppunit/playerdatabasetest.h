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

#ifndef PLAYERDATABASETEST_H
#define PLAYERDATABASETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <qstring.h>

class PlayerDatabaseTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( PlayerDatabaseTest );
  CPPUNIT_TEST( testCreateDatabase );
  CPPUNIT_TEST( testOpenDatabase );
  CPPUNIT_TEST( testRemoveDatabase );
  CPPUNIT_TEST( testPlayerCount );
  CPPUNIT_TEST( testPlayerExists );
  CPPUNIT_TEST( testCurrentPlayerNotFound );
  CPPUNIT_TEST( testCurrentPlayer );
  CPPUNIT_TEST( testDatabaseClose );
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp();
  void tearDown();

  void testCreateDatabase();
  void testOpenDatabase();
  void testRemoveDatabase();
  void testPlayerCount();
  void testPlayerExists();
  void testCurrentPlayerNotFound();
  void testCurrentPlayer();
  void testDatabaseClose();

 private:

  QString db_name;
};

#endif 
