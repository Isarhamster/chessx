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

#ifndef PLAYERDATABASECHANGESTEST_H
#define PLAYERDATABASECHANGESTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <qstring.h>


class PlayerDatabaseChangesTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( PlayerDatabaseChangesTest );
  CPPUNIT_TEST( testAddPlayer );
  CPPUNIT_TEST( testFindPlayers );
  CPPUNIT_TEST( testRollback );
  CPPUNIT_TEST( testCompact );
  CPPUNIT_TEST( testDateOfBirth );
  CPPUNIT_TEST( testDateOfDeath );
  CPPUNIT_TEST( testCountry );
  CPPUNIT_TEST( testTitle );
  CPPUNIT_TEST( testPhoto );
  CPPUNIT_TEST( testBiography );
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp();
  void tearDown();

  void testAddPlayer();
  void testFindPlayers();
  void testRollback();
  void testCompact();
  void testDateOfBirth();
  void testDateOfDeath();
  void testCountry();
  void testTitle();
  void testPhoto();
  void testBiography();

 private:

  QString db_name;
};

#endif 
