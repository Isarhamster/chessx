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

using namespace chessx;

// Ensure that each move played in given games gets a unique hash,
// and that hash is identical as the game is undone one move at a
// time back to the starting position
void BoardTest::testReversableHash()
{
    QFETCH(QString, moves);
    QFETCH(QString, finalFen);

    BoardX board;
    board.setStandardPosition();
    QStack<quint64> hashStack;
    QStack<Move> moveStack;
    for (auto san: moves.split(' '))
    {
        auto m = board.parseMove(san);
        auto prevHash = board.getHashValue();
        board.doMove(m);
        auto currHash = board.getHashValue();

        // hash must change
        QVERIFY(currHash != prevHash);

        moveStack.push(m);
        hashStack.push(prevHash);
    }

    // final FEN must match predefined value
    if (!finalFen.isEmpty())
    {
        QCOMPARE(board.toFen(), finalFen);
    }

    // as we undo rewind the game back to the start
    // hashes must match with the ones computed during forward pass
    while (!moveStack.isEmpty())
    {
        auto m = moveStack.pop();
        auto h = hashStack.pop();

        board.undoMove(m);
        QCOMPARE(board.getHashValue(), h);
    }
}

void BoardTest::testReversableHash_data()
{
    QTest::addColumn<QString>("moves");
    QTest::addColumn<QString>("finalFen");

    QTest::newRow("game[0]")
        << "e4 e5 d4 f5 d5 c5 dxc6 dxc6 Bd2 Bd6 Nc3 Nf6 Qe2 O-O O-O-O f4 g4 fxg3 hxg3 Be6 Kb1 Kh8 Qh5 Nfd7 Qxh7#"
        << "rn1q1r1k/pp1n2pQ/2pbb3/4p3/4P3/2N3P1/PPPB1P2/1K1R1BNR b - - 0 13";
}

void BoardTest::testValidate()
{
    QFETCH(QString, fen);
    QFETCH(int, expected);

    BoardX board;
    QVERIFY(board.fromFen(fen));
    QCOMPARE(static_cast<int>(board.validate()), expected);
}

void BoardTest::testValidate_data()
{
    const char* desc = "";
    char buff[512];
    QTest::addColumn<int>("expected");
    QTest::addColumn<QString>("fen");

#define SECTION(text) desc = text
#define ROW(expected, fen) qsnprintf(buff, sizeof(buff), "%s [%d]", desc, __LINE__); QTest::newRow(buff) << static_cast<int>(expected) << fen

    SECTION("Life is good");
    ROW(Valid, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    SECTION("Too many pawns");
    ROW(TooManyBlackPawns, "rnbqkbnr/pppppppp/p7/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    SECTION("Too many pieces");
    ROW(TooManyWhite, "rnbqkbnr/pppppppp/8/8/8/5NNN/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    ROW(TooManyBlack, "rnbqkbnr/pppppppp/nnn5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    SECTION("Missing Kings");
    ROW(NoWhiteKing, "k7/8/8/8/8/8/8/8 b - - 0 1");
    ROW(NoBlackKing, "8/8/8/8/8/8/8/K7 b - - 0 1");

    SECTION("Invalid check");
    ROW(OppositeCheck, "k7/8/8/r7/8/8/8/K7 b - - 0 1");
    ROW(Valid, "k7/8/8/r7/8/8/8/K7 w - - 0 1");

    SECTION("Kings can't kiss");
    ROW(DoubleCheck, "8/8/8/3kK3/8/8/8/8 b - - 0 1");

    SECTION("Too many kings");
    ROW(TooManyKings, "k7/8/8/3K4/8/8/8/7K b - - 0 1");
    ROW(TooManyKings, "k7/8/8/3k4/8/8/8/7K b - - 0 1");

    SECTION("Pawn on first or eighth rank is a no no");
    ROW(PawnsOn18, "p7/8/8/k7/7K/8/8/8 b - - 0 1");
    ROW(PawnsOn18, "P7/8/8/k7/7K/8/8/8 b - - 0 1");
    ROW(PawnsOn18, "8/8/8/k7/7K/8/8/7p b - - 0 1");
    ROW(PawnsOn18, "8/8/8/k7/7K/8/8/7P b - - 0 1");

    SECTION("Can't castle without a rook");
    ROW(BadCastlingRights, "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    ROW(BadCastlingRights, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN1 b KQkq - 0 1");
    SECTION("Can't castle if king has moved");
    ROW(BadCastlingRights, "rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b Kkq - 0 1");
    ROW(BadCastlingRights, "rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b Qkq - 0 1");

    SECTION("End on a happy note");
    ROW(Valid, "rnbqkbnr/pppppppp/8/8/8/4P3/PPPPKPPP/RNBQ1BNR b kq - 0 1");

#undef ROW
#undef SECTION
}

void BoardTest::testIsValidFEN()
{
    QFETCH(bool, expected);
    QFETCH(QString, fen);

    BoardX board;
    QCOMPARE(board.isValidFen(fen), expected);
}

void BoardTest::testIsValidFEN_data()
{
    const char* desc;
    char buff[512];

    QTest::addColumn<bool>("expected");
    QTest::addColumn<QString>("fen");
#define SECTION(text) desc = text
#define ROW(expected, fen) qsnprintf(buff, sizeof(buff), "%s [%d]", desc, __LINE__); QTest::newRow(buff) << expected << fen
    
    SECTION("Starting position");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    // ****** Test piece placement *******
    SECTION("invalid piece o");
    ROW(false, "rnbqkbnr/ppppppop/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    SECTION("invalid number of files 9");
    ROW(false, "rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    SECTION("invalid number of files 0");
    ROW(false, "rnbqkbnr/pppppppp/8/0/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    SECTION("invalid number of ranks (extra /8)");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/8 b KQkq - 0 1");
    SECTION("invalid number of files in second rank");
    ROW(false, "rnbqkbnr/3pppp/8/8/8/8/3PPPPP/RNBQKBNR b KQkq - 0 1");

    // ******* Test color to play *******
    SECTION("Invalid color W");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR W KQkq - 0 1");
    SECTION("Invalid color B");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR B KQkq - 0 1");
    SECTION("Invalid color k");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR k KQkq - 0 1");

    // ******* Test Castling *******
    SECTION("Test Castling");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b K - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Q - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQ - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b kq - 0 1");
    SECTION("'-' should be alone, not with other characters");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k- - 0 1");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b k-q - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1");

    // ******* Test en passant square *******
    SECTION("en passant square should be on 3rd rank");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e4 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e3 0 1");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e5 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e6 0 1");
    SECTION("With e6 as en passant square, it should be white to move");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e6 0 1");
    SECTION("No i6 (luckily)");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq i6 0 1");
    SECTION("No A6 (must be lower case)");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq A6 0 1");
    SECTION("With h6 as en passant square, it should be white to move");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq h6 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq h6 0 1");

    // ******* Test half move clock *******
    SECTION("half move clock");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 1 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 2 1");
    SECTION("half move clock must be a number");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - a 1");
    SECTION("half move clock must be larger than or equal to 0");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - -1 1");

    // ******* Test full move count *******
    SECTION("full move count must be larger than 0");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    ROW(true, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 2");
    // Non-positive move counter are silently corrected since
    // [1c50830] Parse mal-formed FEN with - - - - at end
    // ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0");
    // ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 -1");
    SECTION("full move count must be a number");
    ROW(false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 a");
}

void BoardTest::testCreateHash()
{
    BoardX board1;
    BoardX board2;

    board1.clear();
    board2.clear();
    QVERIFY(board1 == board2);

    board1.setStandardPosition();
    QVERIFY(board1 != board2);
    board2.fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    QVERIFY(board1 == board2);

    // Create a move from algebraic notation, and do a few tests on it
    Move m(board1.parseMove("e4"));
    QVERIFY(m.isLegal());
    QVERIFY(m.isDoubleAdvance());
    QVERIFY(m == WhitePawn);
    QVERIFY(m != Black);

    // Play the move on board, and make sure it applies properly
    board1.doMove(m);
    QVERIFY(board1.toFen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    // Do same move on board 2
    board2.doMove(board2.parseMove("e4"));
    QVERIFY(board1 == board2);

    // See if desired move really is a black pawn
    QVERIFY(board1.parseMove("b5") == BlackPawn);

    // Play a few more moves
    board1.doMove(board1.parseMove("b5"));
    board2.doMove(board2.parseMove("f5"));
    QVERIFY(board1 != board2);
    board1.doMove(board1.parseMove("e5"));
    board2.doMove(board2.parseMove("e5"));
    QVERIFY(board1 != board2);
    board1.doMove(board1.parseMove("f5"));
    board2.doMove(board2.parseMove("b5"));
    QVERIFY(board1 != board2);

    // Getting back a FEN
    QVERIFY(board2.toFen() == "rnbqkbnr/p1ppp1pp/8/1p2Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq b6 0 3");

    // Setting a FEN
    board1.clear();
    QString testfen("rnbqkbnr/p1ppp1pp/8/1p2Pp2/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    QVERIFY(board1.fromFen(testfen));
    QCOMPARE(board1.toFen(), testfen);
    QVERIFY(board1.toMove() == Black);
    QVERIFY(board1 != board2);

    // Now restore board back to board2 game
    QVERIFY(board1.fromFen(board2.toFen()));
    QVERIFY(board1 == board2);

    // Side to move
    QVERIFY(board1.toMove() == White);
    QVERIFY(board2.toMove() == White);
    board1.setToMove(Black);
    QVERIFY(board1 != board2);
    board1.setToMove(Black);
    QVERIFY(board1 != board2);
    board1.swapToMove();
    QVERIFY(board1 == board2);
    board1.swapToMove();
    QVERIFY(board1 != board2);
    board1.swapToMove();

    board1.doMove(board1.parseMove("Bc4"));
    board1.doMove(board1.parseMove("Bb7"));
    board1.doMove(board1.parseMove("Nf3"));
    board1.doMove(board1.parseMove("Nf6"));
    board1.doMove(board1.parseMove("O-O"));
    board2.fromFen(board1.toFen());
    QVERIFY(board1 == board2);
}

// FIXME -- Add setAt hash testing
// FIXME -- Validate forward and backward hashing match
// FIXME -- Add tests for bad moves.. and bad parsing strings
