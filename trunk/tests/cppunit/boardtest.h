/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 01/11/2005
    copyright            : (C) 2005 Marius Roets
                           <saidinwielder@sourceforge.net>
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
Unit tests for the Board class.
*/

#ifndef BOARDTEST_H
#define BOARDTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <qstring.h>


class BoardTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( BoardTest );
  CPPUNIT_TEST( testIsValidFEN );
  CPPUNIT_TEST( testCreateHash );
  CPPUNIT_TEST_SUITE_END();

 public:
  /*
  void setUp();
  void tearDown();
  */

  void testIsValidFEN();
  void testCreateHash();

 //private:

};

#endif 
