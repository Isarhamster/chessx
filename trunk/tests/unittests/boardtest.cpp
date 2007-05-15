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

#include "boardtest.h"
#include "board.h"
#include "qstring.h"

Board board;

void BoardTest::initTestCase() {}
void BoardTest::init(){}
void BoardTest::cleanup(){}
void BoardTest::cleanupTestCase() {}

void BoardTest::testIsValidFEN()
{
	// ****** Starting position *******
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// ****** Test piece placement *******
	// invalid piece o
	QVERIFY(!board.isValidFEN("rnbqkbnr/ppppppop/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of files 9
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of files 0
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/0/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of ranks (extra /8)
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/8 b KQkq - 0 1"));
	// invalid number of files in second rank
	QVERIFY(!board.isValidFEN("rnbqkbnr/3pppp/8/8/8/8/3PPPPP/RNBQKBNR b KQkq - 0 1"));
	// ******* Test color to play *******
	// Invalid color W
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR W KQkq - 0 1"));
	// Invalid color B
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR B KQkq - 0 1"));
	// Invalid color k
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR k KQkq - 0 1"));
	// ******* Test Castling *******
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b K - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Q - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQ - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b kq - 0 1"));
	// '-' should be alone, not with other characters
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k- - 0 1"));
	// '-' should be alone, not with other characters
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k-q - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1"));
	// ******* Test en passant square *******
	// e4 can never be an en passant target square
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e4 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e3 0 1"));
	// With e6 as en passant square, it should be white to move
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e6 0 1"));
	// No i6 (luckily)
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq i6 0 1"));
	// No A6 (must be lower case)
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq A6 0 1"));
	// With h6 as en passant square, it should be white to move
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq h6 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq h6 0 1"));
	// ******* Test half move clock *******
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 1 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 2 1"));
	// half move clock must be a number
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - a 1"));
	// half move clock must be larger than or equal to 0
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - -1 1"));

	// ******* Test full move count *******
	// full move count must be larger than 0
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 2"));
	// full move count must be a number
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 a"));
	// full move count must be larger than 0
	QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 -1"));

// SBE -- FIXME -- MUST FIX AND REENABLE THESE TESTS
	/*
		// ******* Test some positions *******
		// Too many pawns
		QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/p7/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
		// Too many pieces
		QVERIFY(!board.isValidFEN("rnbqkbnr/pppppppp/5nnn/8/8/5NNN/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
		// No white king
		QVERIFY(!board.isValidFEN("k7/8/8/8/8/8/8/8 b KQkq - 0 1"));
		QVERIFY(board.isValidFEN("k7/8/8/8/8/8/8/K7 b KQkq - 0 1"));
		// Invalid check
		QVERIFY(!board.isValidFEN("k7/8/8/r7/8/8/8/K7 b KQkq - 0 1"));
		QVERIFY(board.isValidFEN("k7/8/8/r7/8/8/8/K7 w KQkq - 0 1"));
	*/
}

void BoardTest::testCreateHash()
{
	Board board2;
	board.setDebugName("Board1");
	board2.setDebugName("Board2");
	board.clear();
	board.setStandardPosition();
	board2.clear();
	board2.setStandardPosition();
	//printf ("%s - %s\n",QString::number(board.getHashValue(),16).latin1(),QString::number(board2.getHashValue(),16).latin1());
	QVERIFY(board == board2);
	/*board2.setAt(0,WhiteRook); board2.setAt(1,WhiteKnight); board2.setAt(2,WhiteBishop);
	board2.setAt(3,WhiteQueen); board2.setAt(4,WhiteKing); board2.setAt(5,WhiteBishop);
	board2.setAt(6,WhiteKnight); board2.setAt(7,WhiteRook); board2.setAt(8,WhitePawn);
	board2.setAt(9,WhitePawn); board2.setAt(10,WhitePawn); board2.setAt(11,WhitePawn);
	board2.setAt(12,WhitePawn); board2.setAt(13,WhitePawn); board2.setAt(14,WhitePawn);
	board2.setAt(15,WhitePawn); board2.setAt(48,BlackPawn); board2.setAt(49,BlackPawn);
	board2.setAt(50,BlackPawn); board2.setAt(51,BlackPawn); board2.setAt(52,BlackPawn);
	board2.setAt(53,BlackPawn); board2.setAt(54,BlackPawn); board2.setAt(55,BlackPawn);
	board2.setAt(56,BlackRook); board2.setAt(57,BlackKnight); board2.setAt(58,BlackBishop);
	board2.setAt(59,BlackQueen); board2.setAt(60,BlackKing); board2.setAt(61,BlackBishop);
	board2.setAt(62,BlackKnight); board2.setAt(63,BlackRook);*/
	//printf("Board1: Move 1\n");
	board.doMove(board.singleMove("e4"));
	//printf("Board2: Move 1\n");
	board2.doMove(board2.singleMove("e4"));
	QVERIFY(board == board2);
	//printf("Board1: Move 2\n");
	board.doMove(board.singleMove("b5"));
	//printf("Board2: Move 2\n");
	board2.doMove(board2.singleMove("f5"));
	QVERIFY(board != board2);
	//printf("Board1: Move 3\n");
	board.doMove(board.singleMove("e5"));
	//printf("Board2: Move 3\n");
	board2.doMove(board2.singleMove("e5"));
	QVERIFY(board != board2);
	//printf("Board1: Move 4\n");
	board.doMove(board.singleMove("f5"));
	//printf("Board2: Move 4\n");
	board2.doMove(board2.singleMove("b5"));
	QVERIFY(board != board2);

	//printf ("%s - %s\n",QString::number(h1,16).latin1(),QString::number(h2,16).latin1());
	board.clear();
	board.fromFEN("rnbqkbnr/p1ppp1pp/8/1p2Pp2/8/8/PPPP1PPP/RNBQKBNR b KQkq b6 0 1");
	//printf("%s\n",board.toASCII().latin1());
	//printf("%s\n",board2.toASCII().latin1());
	//board2.createHash();
	//printf("%s\n",board.toASCII().latin1());
	//printf("%s\n",board2.toASCII().latin1());
	QVERIFY(board != board2);
	board.fromFEN(board2.toFEN());
	QVERIFY(board == board2);
	board.setToMove(Black);
	QVERIFY(board != board2);
	board.setToMove(Black);
	QVERIFY(board != board2);
	board.swapToMove();
	QVERIFY(board == board2);
	board.swapToMove();
	QVERIFY(board != board2);
	board.swapToMove();
	board.doMove(board.singleMove("Bc4"));
	board.doMove(board.singleMove("Bb7"));
	board.doMove(board.singleMove("Nf3"));
	board.doMove(board.singleMove("Nf6"));
	//printf("*** Castling\n");
	board.doMove(board.singleMove("O-O"));
	//printf("%s\n",board.toASCII().latin1());
	//printf("%s\n",board.toFEN().latin1());
	board2.fromFEN(board.toFEN());
	QVERIFY(board == board2);
}
