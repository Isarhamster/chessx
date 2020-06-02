
#include "positionsearchtest.h"
#include "pgndatabase.h"
#include "settings.h"
#include "positionsearch.h"

void PositionSearchTest::initTestCase() {}
void PositionSearchTest::init() {}
void PositionSearchTest::cleanup() {}
void PositionSearchTest::cleanupTestCase() {}

void PositionSearchTest::testSearch()
{
    // required by PgnDatabase::open() to check if indexing is enabled
    // TODO: remove
    AppSettings = new Settings;
    PgnDatabase db { false };
    db.open(QFINDTESTDATA("data/t1.pgn"), false);

    BoardX board;
    board.setStandardPosition();
    GameX game;
    db.loadGame(0, game);
    game.moveToStart();

    PositionSearch posSearch(&db, board);
    QCOMPARE(posSearch.matches(0), 1);

    for(int i = 1; i <= 4; ++i)
    {
        game.forward();
        board.doMove(game.move());
        posSearch.setPosition(board);

        QCOMPARE(posSearch.matches(0), i + 1);
    }

    board.setStandardPosition();
    board.doMove(board.parseMove("e2-e4"));
    posSearch.setPosition(board);
    Q_ASSERT(!posSearch.matches(0));
}

