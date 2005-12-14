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
Unit tests for the Query class.
*/

#ifndef QUERYTEST_H
#define QUERYTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <qstring.h>
#include "query.h"


class QueryTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( QueryTest );
   CPPUNIT_TEST( testAppend );
   CPPUNIT_TEST( testElementTypes );
   CPPUNIT_TEST( testGetElements );
   CPPUNIT_TEST( testSet );
   CPPUNIT_TEST( testRemove );
   CPPUNIT_TEST( testIsValid );
   CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();
      void tearDown();

      void testAppend();
      void testElementTypes();
      void testGetElements();
      void testIsValid();
      void testSet();
      void testRemove();

   private :
      PositionSearch *p1;
      PositionSearch *p2;
      EloSearch *e1;
      EloSearch *e2;

      void setupList();
};

#endif 
