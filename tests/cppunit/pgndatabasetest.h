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

#include <cppunit/extensions/HelperMacros.h>

class PgnDatabaseTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( PgnDatabaseTest );
  CPPUNIT_TEST( testCreateDatabase );
  CPPUNIT_TEST( testCopyGameIntoNewDB );
  CPPUNIT_TEST( testRemoveGame );
  CPPUNIT_TEST( testExecuteSearch );
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp();
  void tearDown();

  void testCreateDatabase();
  void testCopyGameIntoNewDB();
  void testRemoveGame();
  void testExecuteSearch();

 private:

};

#endif 
