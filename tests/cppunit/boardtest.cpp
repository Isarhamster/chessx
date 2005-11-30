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
Unit tests for the Board class.
*/

#include "boardtest.h"
#include "board.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( BoardTest );

Board board;

/*
void BoardTest::setUp()
{
}

void BoardTest::tearDown()
{
}
*/

void BoardTest::testIsValidFEN()
{
   // ****** Starting position *******
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // ****** Test piece placement *******
   // invalid piece o
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/ppppppop/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // invalid number of files 9
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // invalid number of files 0
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/0/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // invalid number of ranks (extra /8)
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/8 b KQkq - 0 1") );
   // invalid number of files in second rank 
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/3pppp/8/8/8/8/3PPPPP/RNBQKBNR b KQkq - 0 1") );
   // ******* Test color to play *******
   // Invalid color W 
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR W KQkq - 0 1") );
   // Invalid color B 
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR B KQkq - 0 1") );
   // Invalid color k 
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR k KQkq - 0 1") );
   // ******* Test Castling ******* 
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b K - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Q - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQ - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b kq - 0 1") );
   // '-' should be alone, not with other characters
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k- - 0 1") );
   // '-' should be alone, not with other characters
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k-q - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1") );
   // ******* Test en passant square ******* 
   // e4 can never be an en passant target square
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e4 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e3 0 1") );
   // With e6 as en passant square, it should be white to move
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e6 0 1") );
   // No i6 (luckily)
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq i6 0 1") );
   // No A6 (must be lower case)
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq A6 0 1") );
   // With h6 as en passant square, it should be white to move
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq h6 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq h6 0 1") );
   // ******* Test half move clock ******* 
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 1 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 2 1") );
   // half move clock must be a number
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - a 1") );
   // half move clock must be larger than or equal to 0
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - -1 1") );

   // ******* Test full move count ******* 
   // full move count must be larger than 0
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 2") );
   // full move count must be a number
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 a") );
   // full move count must be larger than 0
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 -1") );

   // ******* Test some positions ******* 
   // Too many pawns
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/p7/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // Too many pieces
   CPPUNIT_ASSERT( !board.isValidFEN("rnbqkbnr/pppppppp/5nnn/8/8/5NNN/PPPPPPPP/RNBQKBNR b KQkq - 0 1") );
   // No white king
   CPPUNIT_ASSERT( !board.isValidFEN("k7/8/8/8/8/8/8/8 b KQkq - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("k7/8/8/8/8/8/8/K7 b KQkq - 0 1") );
   // Invalid check
   CPPUNIT_ASSERT( !board.isValidFEN("k7/8/8/r7/8/8/8/K7 b KQkq - 0 1") );
   CPPUNIT_ASSERT( board.isValidFEN("k7/8/8/r7/8/8/8/K7 w KQkq - 0 1") );
}

