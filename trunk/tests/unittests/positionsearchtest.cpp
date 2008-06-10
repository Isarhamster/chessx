
#include "positionsearchtest.h"
#include "game.h"
#include "pgndatabase.h"
#include "search.h"
#include "board.h"

void PositionSearchTest::initTestCase() {}
void PositionSearchTest::init(){}
void PositionSearchTest::cleanup(){}
void PositionSearchTest::cleanupTestCase() {}

void PositionSearchTest::testSearch() {
	PgnDatabase db;
	db.open("data/t1.pgn");

	Board board;
	board.setStandardPosition();
	Game game;
	db.loadGame(0, game);
	game.moveToStart();

	PositionSearch posSearch(&db,  board);
	QCOMPARE(posSearch.matches(0), 1);

	for (int i = 1; i <= 4; ++i) {
		game.forward();
		board.doMove(game.move());
		posSearch.setPosition(board);

		QCOMPARE(posSearch.matches(0), i+1);
	}

	board.setStandardPosition();
	board.doMove(board.parseMove("e2-e4"));
	posSearch.setPosition(board);
	Q_ASSERT(!posSearch.matches(0));
}

