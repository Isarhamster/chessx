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

#include "querytest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryTest );

Query query;

void QueryTest::setUp()
{
   Board b1;
   b1.fromFEN("r1bqkb1r/pppp1ppp/2n2n2/4P3/2Bp4/5N2/PPP2PPP/RNBQK2R w KQkq - 0 -");
   p1 = new PositionSearch(b1);
   b1.fromFEN("rnbqk2r/pp1p1ppp/4pn2/2p5/1bPP4/2N1P3/PP3PPP/R1BQKBNR w KQkq - 0 -");
   p2 = new PositionSearch(b1);
   e1 = new EloSearch();
   e2 = new EloSearch();
}
void QueryTest::tearDown()
{
   query.clear();
   delete p1;
   delete p2;
   delete e1;
   delete e2;
}

void QueryTest::setupList()
{
   query.clear();
   query.append(p1);
   query.append(e1);
   query.append(Search::And);
   query.append(p2);
   query.append(e2);
   query.append(Search::And);
   query.append(Search::Or);
}
void QueryTest::testAppend()
{
   setupList();
   CPPUNIT_ASSERT (query.count() == 7);
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
}
void QueryTest::testElementTypes()
{
   setupList();
   CPPUNIT_ASSERT( query.isElementSearch(0) );
   CPPUNIT_ASSERT( !query.isElementOperator(0) );
   CPPUNIT_ASSERT( query.isElementSearch(1) );
   CPPUNIT_ASSERT( !query.isElementOperator(1) );
   CPPUNIT_ASSERT( !query.isElementSearch(2) );
   CPPUNIT_ASSERT( query.isElementOperator(2) );
   CPPUNIT_ASSERT( query.isElementSearch(3) );
   CPPUNIT_ASSERT( !query.isElementOperator(3) );
   CPPUNIT_ASSERT( query.isElementSearch(4) );
   CPPUNIT_ASSERT( !query.isElementOperator(4) );
   CPPUNIT_ASSERT( !query.isElementSearch(5) );
   CPPUNIT_ASSERT( query.isElementOperator(5) );
   CPPUNIT_ASSERT( !query.isElementSearch(6) );
   CPPUNIT_ASSERT( query.isElementOperator(6) );
}
void QueryTest::testGetElements()
{
   setupList();
   CPPUNIT_ASSERT( query.search(0) == p1 );
   CPPUNIT_ASSERT( query.searchOperator(0) == Search::NullOperator );
   CPPUNIT_ASSERT( query.search(1) == e1 );
   CPPUNIT_ASSERT( query.searchOperator(1) == Search::NullOperator );
   CPPUNIT_ASSERT( query.search(2) == NULL );
   CPPUNIT_ASSERT( query.searchOperator(2) == Search::And );
   CPPUNIT_ASSERT( query.search(3) == p2 );
   CPPUNIT_ASSERT( query.searchOperator(3) == Search::NullOperator );
   CPPUNIT_ASSERT( query.search(4) == e2 );
   CPPUNIT_ASSERT( query.searchOperator(4) == Search::NullOperator );
   CPPUNIT_ASSERT( query.search(5) == NULL );
   CPPUNIT_ASSERT( query.searchOperator(5) == Search::And );
   CPPUNIT_ASSERT( query.search(6) == NULL );
   CPPUNIT_ASSERT( query.searchOperator(6) == Search::Or );
}
void QueryTest::testIsValid()
{
   setupList();
   CPPUNIT_ASSERT( query.isValid() );
   query.remove(0);
   CPPUNIT_ASSERT( !query.isValid() );
   setupList();
   query.set(3,Search::And);
   CPPUNIT_ASSERT( !query.isValid() );
   setupList();
   query.append(Search::Or);
   CPPUNIT_ASSERT( !query.isValid() );
   setupList();
   query.remove(5);
   CPPUNIT_ASSERT( query.isValid() ); // Valid but warning
}
void QueryTest::testSet()
{
   setupList();
   // case 1 : operator to operator
   CPPUNIT_ASSERT( query.searchOperator(2) == Search::And );
   query.set(2,Search::Or);
   CPPUNIT_ASSERT( query.searchOperator(2) == Search::Or );
   CPPUNIT_ASSERT( query.isElementOperator(2) );
   CPPUNIT_ASSERT (query.count() == 7);
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
   // case 2 : operand to operator
   CPPUNIT_ASSERT( query.search(3) == p2);
   CPPUNIT_ASSERT( query.isElementSearch(3) );
   query.set(3,Search::Or);
   CPPUNIT_ASSERT( query.searchOperator(3) == Search::Or );
   CPPUNIT_ASSERT( query.isElementOperator(3) );
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
   // case 3 : operand to operand
   CPPUNIT_ASSERT( query.search(4) == e2);
   CPPUNIT_ASSERT( query.isElementSearch(4) );
   query.set(4,p1);
   CPPUNIT_ASSERT( query.search(4) == p1 );
   CPPUNIT_ASSERT( query.isElementSearch(4) );
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
   // case 4 : operator to operand
   CPPUNIT_ASSERT( query.searchOperator(5) == Search::And);
   CPPUNIT_ASSERT( query.isElementOperator(5) );
   query.set(5,p2);
   CPPUNIT_ASSERT( query.search(5) == p2 );
   CPPUNIT_ASSERT( query.isElementSearch(5) );
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
}

void QueryTest::testRemove()
{
   setupList();
   CPPUNIT_ASSERT (query.count() == 7);
   CPPUNIT_ASSERT (query.remove(1));
   CPPUNIT_ASSERT (query.count() == 6);
   CPPUNIT_ASSERT( query.search(0) == p1 );
   CPPUNIT_ASSERT( query.searchOperator(1) == Search::And );
   CPPUNIT_ASSERT( query.search(2) == p2 );
   CPPUNIT_ASSERT( query.search(3) == e2 );
   CPPUNIT_ASSERT( query.searchOperator(4) == Search::And );
   CPPUNIT_ASSERT( query.searchOperator(5) == Search::Or );
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));
   CPPUNIT_ASSERT (query.remove(4));
   CPPUNIT_ASSERT (query.count() == 5);
   CPPUNIT_ASSERT( query.search(0) == p1 );
   CPPUNIT_ASSERT( query.searchOperator(1) == Search::And );
   CPPUNIT_ASSERT( query.search(2) == p2 );
   CPPUNIT_ASSERT( query.search(3) == e2 );
   CPPUNIT_ASSERT( query.searchOperator(4) == Search::Or );
   CPPUNIT_ASSERT (query.count() == (query.countOperators() + query.countOperands()));

}

