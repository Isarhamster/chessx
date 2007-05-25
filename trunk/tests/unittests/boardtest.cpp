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
#include <QStack>
#include <QString>

Board board;

class TestGame {
public:
	TestGame() {};
	TestGame(const QString& m, const QString& f) : moves(m), finalFen(f) {};
	QString moves, finalFen;
};
typedef QStack<TestGame> TestGameList;

static const char* game1 =
	"e4 e5 d4 f5 d5 c5 dxc6 dxc6 Bd2 Bd6 Nc3 Nf6 Qe2 O-O"
	" O-O-O f4 g4 fxg3 hxg3 Be6 Kb1 Kh8 Qh5 Nfd7 Qxh7#";
static const char* fen1 =
	"rn1q1r1k/pp1n2pQ/2pbb3/4p3/4P3/2N3P1/PPPB1P2/1K1R1BNR b - - 0 13";

TestGameList loadGames()
{
	TestGameList list;
	list.push(TestGame(game1,fen1));
	return list;
}

// Ensure that each move played in given games gets a unique hash,
// and that hash is identical as the game is undone one move at a
// time back to the starting position
void BoardTest::testReversableHash()
{
	TestGameList allgames(loadGames());

	while (!allgames.isEmpty()) {
		TestGame current(allgames.pop());

		quint64 previousHash = 0;
		int moveNum = 1;
		board.setStandardPosition();
		QStringList moves(current.moves.split(' '));
		QStringListIterator mi(moves);
		QStack<quint64> hash;
		QStack<Move> move;
		while (mi.hasNext()) {
			QString san(mi.next());
			hash.push(board.getHashValue());
			QVERIFY(hash.top() != previousHash);
			previousHash = hash.top();
			move.push(board.parseMove(san));
			board.doMove(move.top());
			if (move.top() == Black)
				++moveNum;
		}
		QVERIFY(board.toFen(moveNum) == current.finalFen);
		while (!move.isEmpty()) {
			board.undoMove(move.pop());
			QVERIFY(hash.pop() == board.getHashValue());
		}
	}
}

void BoardTest::testValidate()
{
	// Life is good
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
	QVERIFY(board.validate() == Valid);

	// Too many pawns
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/p7/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.validate() == TooManyBlackPawns);

	// Too many pieces
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/5NNN/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.validate() == TooManyWhite);

	// Too many pieces
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/nnn5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.validate() == TooManyBlack);

	// Missing Kings
	QVERIFY(board.fromFen("k7/8/8/8/8/8/8/8 b - - 0 1"));
	QVERIFY(board.validate() == NoWhiteKing);
	QVERIFY(board.fromFen("8/8/8/8/8/8/8/K7 b - - 0 1"));
	QVERIFY(board.validate() == NoBlackKing);

	// Invalid check
	QVERIFY(board.fromFen("k7/8/8/r7/8/8/8/K7 b - - 0 1"));
	QVERIFY(board.validate() == OppositeCheck);
	QVERIFY(board.fromFen("k7/8/8/r7/8/8/8/K7 w - - 0 1"));
	QVERIFY(board.validate() == Valid);

	// Kings can't kiss
	QVERIFY(board.fromFen("8/8/8/3kK3/8/8/8/8 b - - 0 1"));
	QVERIFY(board.validate() == DoubleCheck);

	// Too many kings
	QVERIFY(board.fromFen("k7/8/8/3K4/8/8/8/7K b - - 0 1"));
	QVERIFY(board.validate() == TooManyKings);
	QVERIFY(board.fromFen("k7/8/8/3k4/8/8/8/7K b - - 0 1"));
	QVERIFY(board.validate() == TooManyKings);

	// Pawn on first or eighth rank is a no no
	QVERIFY(board.fromFen("p7/8/8/k7/7K/8/8/8 b - - 0 1"));
	QVERIFY(board.validate() == PawnsOn18);
	QVERIFY(board.fromFen("P7/8/8/k7/7K/8/8/8 b - - 0 1"));
	QVERIFY(board.validate() == PawnsOn18);
	QVERIFY(board.fromFen("8/8/8/k7/7K/8/8/7p b - - 0 1"));
	QVERIFY(board.validate() == PawnsOn18);
	QVERIFY(board.fromFen("8/8/8/k7/7K/8/8/7P b - - 0 1"));
	QVERIFY(board.validate() == PawnsOn18);

	// Can't castle without a rook or if king has moved
	QVERIFY(board.fromFen("1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.validate() == BadCastlingRights);
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN1 b KQkq - 0 1"));
	QVERIFY(board.validate() == BadCastlingRights);
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b Kkq - 0 1"));
	QVERIFY(board.validate() == BadCastlingRights);
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b Qkq - 0 1"));
	QVERIFY(board.validate() == BadCastlingRights);

	// End on a happy note
	QVERIFY(board.fromFen("rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b kq - 0 1"));
	QVERIFY(board.validate() == Valid);
}

void BoardTest::testIsValidFEN()
{
	// ****** Starting position *******
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// ****** Test piece placement *******
	// invalid piece o
	QVERIFY(!board.isValidFen("rnbqkbnr/ppppppop/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of files 9
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of files 0
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/0/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	// invalid number of ranks (extra /8)
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/8 b KQkq - 0 1"));
	// invalid number of files in second rank
	QVERIFY(!board.isValidFen("rnbqkbnr/3pppp/8/8/8/8/3PPPPP/RNBQKBNR b KQkq - 0 1"));
	// ******* Test color to play *******
	// Invalid color W
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR W KQkq - 0 1"));
	// Invalid color B
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR B KQkq - 0 1"));
	// Invalid color k
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR k KQkq - 0 1"));
	// ******* Test Castling *******
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b K - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Q - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQ - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b kq - 0 1"));
	// '-' should be alone, not with other characters
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k- - 0 1"));
	// '-' should be alone, not with other characters
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k-q - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1"));
	// ******* Test en passant square *******
	// e4 can never be an en passant target square
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e4 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e3 0 1"));
	// With e6 as en passant square, it should be white to move
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e6 0 1"));
	// No i6 (luckily)
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq i6 0 1"));
	// No A6 (must be lower case)
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq A6 0 1"));
	// With h6 as en passant square, it should be white to move
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq h6 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq h6 0 1"));
	// ******* Test half move clock *******
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 1 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 2 1"));
	// half move clock must be a number
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - a 1"));
	// half move clock must be larger than or equal to 0
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - -1 1"));

	// ******* Test full move count *******
	// full move count must be larger than 0
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));
	QVERIFY(board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 2"));
	// full move count must be a number
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 a"));
	// full move count must be larger than 0
	QVERIFY(!board.isValidFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 -1"));

}

void BoardTest::testCreateHash()
{
	Board board2;

	board.clear();
	board2.clear();
	QVERIFY(board == board2);

	board.setStandardPosition();
	QVERIFY(board != board2);
	board2.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	QVERIFY(board == board2);

	// Create a move from algebraic notation, and do a few tests on it
	Move m(board.parseMove("e4"));
	QVERIFY(m.isLegal());
	QVERIFY(m.isDoubleAdvance());
	QVERIFY(m == WhitePawn);
	QVERIFY(m != Black);

	// Play the move on board, and make sure it applies properly
	board.doMove(m);
	QVERIFY(board.toFen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

	// Do same move on board 2
	board2.doMove(board2.parseMove("e4"));
	QVERIFY(board == board2);

	// See if desired move really is a black pawn
	QVERIFY(board.parseMove("b5") == BlackPawn);

	// Play a few more moves
	board.doMove(board.parseMove("b5"));
	board2.doMove(board2.parseMove("f5"));
	QVERIFY(board != board2);
	board.doMove(board.parseMove("e5"));
	board2.doMove(board2.parseMove("e5"));
	QVERIFY(board != board2);
	board.doMove(board.parseMove("f5"));
	board2.doMove(board2.parseMove("b5"));
	QVERIFY(board != board2);

	// Getting back a FEN
	QVERIFY(board2.toFen(3) == "rnbqkbnr/p1ppp1pp/8/1p2Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 3");

	// Setting a FEN
	board.clear();
	QString testfen("rnbqkbnr/p1ppp1pp/8/1p2Pp2/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	QVERIFY(board.fromFen(testfen));
	QCOMPARE(board.toFen(), testfen);
	QVERIFY(board.toMove() == Black);
	QVERIFY(board != board2);

	// Now restore board back to board2 game
	QVERIFY(board.fromFen(board2.toFen()));
	QVERIFY(board == board2);

	// Side to move
	QVERIFY(board.toMove() == White);
	QVERIFY(board2.toMove() == White);
	board.setToMove(Black);
	QVERIFY(board != board2);
	board.setToMove(Black);
	QVERIFY(board != board2);
	board.swapToMove();
	QVERIFY(board == board2);
	board.swapToMove();
	QVERIFY(board != board2);
	board.swapToMove();

	board.doMove(board.parseMove("Bc4"));
	board.doMove(board.parseMove("Bb7"));
	board.doMove(board.parseMove("Nf3"));
	board.doMove(board.parseMove("Nf6"));
	board.doMove(board.parseMove("O-O"));
	board2.fromFen(board.toFen());
	QVERIFY(board == board2);
}

// FIXME -- Add setAt hash testing
// FIXME -- Validate forward and backward hashing match
// FIXME -- Add tests for bad moves.. and bad parsing strings
