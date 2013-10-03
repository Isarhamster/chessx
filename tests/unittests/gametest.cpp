/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 18/11/2007
    copyright            : (C) 2007 Marius Roets
                           <saidingwielder@sourceforge.net>
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
Unit tests for the Game class.
*/

#include <QtDebug>
#include "game.h"
#include "gametest.h"

void GameTest::initTestCase()
{
    m_game = new Game;
    m_game->clear();
}
void GameTest::init() {}
void GameTest::cleanup() {}
void GameTest::cleanupTestCase() {}

void GameTest::testEmptyGame()
{
    m_game->moveToStart();
    m_game->moveToId(2);
    QCOMPARE(m_game->forward(), 0);
    QCOMPARE(m_game->backward(), 0);
    QCOMPARE(m_game->moveByPly(2), 0);
    QCOMPARE(m_game->moveToPly(2), 0);
    QCOMPARE(m_game->plyCount(), 0);
    int moves, comments, nags;
    m_game->moveCount(&moves, &comments, &nags);
    QCOMPARE(moves, 0);
    QCOMPARE(comments, 0);
    QCOMPARE(nags, 0);
    QCOMPARE(m_game->moveToSan(), QString());
}
void GameTest::testAddMove_data()
{
    QTest::addColumn<QString> ("move");
    QTest::addColumn<int> ("node");

    QTest::newRow("1") << "e4" << 1;
    QTest::newRow("2") << "e5" << 2;
    QTest::newRow("3") << "Nf3" << 3;
    QTest::newRow("4") << "Nc6" << 4;
    QTest::newRow("5") << "Bc4" << 5;
    QTest::newRow("6") << "Nf6" << 6;
    QTest::newRow("7") << "Ng5" << 7;
    QTest::newRow("8") << "d5" << 8;
    QTest::newRow("9") << "exd5" << 9;
    QTest::newRow("10") << "Na5" << 10;
    QTest::newRow("11") << "Bb5+" << 11;
    QTest::newRow("12") << "c6" << 12;
    QTest::newRow("13") << "dxc6" << 13;
    QTest::newRow("14") << "bxc6" << 14;
    QTest::newRow("15") << "Be2" << 15;
    QTest::newRow("16") << "h6" << 16;
    QTest::newRow("17") << "Nf3" << 17;
    QTest::newRow("18") << "e4" << 18;
    QTest::newRow("19") << "Ne5" << 19;
    QTest::newRow("20") << "Bd6" << 20;
    QTest::newRow("21") << "d4" << 21;
    QTest::newRow("22") << "exd3" << 22;
    QTest::newRow("23") << "Nxd3" << 23;
    QTest::newRow("24") << "Qc7" << 24;
    QTest::newRow("25") << "h3" << 25;
}
void GameTest::testAddMove()
{
    QFETCH(QString, move);
    QFETCH(int, node);
    QCOMPARE(m_game->addMove(move), node);

}
void GameTest::testAddVariation_data()
{
    QTest::addColumn<QString> ("move");
    QTest::addColumn<int> ("node");
    QTest::addColumn<int> ("startnode");

    QTest::newRow("1") << "Nxd5" << 26 << 9;
    QTest::newRow("2") << "Nxf7" << 27 << 0;
    QTest::newRow("3") << "Kxf7" << 28 << 0;
    QTest::newRow("4") << "Qf3+" << 29 << 0;
    QTest::newRow("5") << "Ke6" << 30 << 0;
    QTest::newRow("6") << "Nc3" << 31 << 0;
    QTest::newRow("7") << "Nb4" << 32 << 0;

    QTest::newRow("8") << "d4" << 33 << 26;
    QTest::newRow("9") << "exd4" << 34 << 0;
    QTest::newRow("10") << "O-O" << 35 << 0;
    QTest::newRow("11") << "Be7" << 36 << 0;
    QTest::newRow("12") << "Nxf7" << 37 << 0;
    QTest::newRow("13") << "Kxf7" << 38 << 0;
    QTest::newRow("14") << "Qf3" << 39 << 0;
    QTest::newRow("15") << "Ke6" << 40 << 0;
    QTest::newRow("16") << "Nc3" << 41 << 0;
    QTest::newRow("17") << "dxc3" << 42 << 0;
    QTest::newRow("18") << "Re1" << 43 << 0;

    QTest::newRow("19") << "Be6" << 44 << 35;
    QTest::newRow("20") << "Re1" << 45 << 0;
    QTest::newRow("21") << "Be7" << 46 << 0;
    QTest::newRow("22") << "Rxe6" << 47 << 0;
    QTest::newRow("23") << "fxe6" << 48 << 0;
    QTest::newRow("24") << "Nxe6" << 49 << 0;
}
void GameTest::testAddVariation()
{
    QFETCH(QString, move);
    QFETCH(int, node);
    QFETCH(int, startnode);
    if(startnode)
    {
        m_game->moveToId(startnode);
        QCOMPARE(m_game->addVariation(move), node);
    }
    else
    {
        QCOMPARE(m_game->addMove(move), node);
    }
}
void GameTest::testDumpNodes()
{
    //m_game->dumpAllMoveNodes();
}
void GameTest::testAnnotation()
{
    QString a5 = "After5";
    QString a10 = "After10";
    QString a26 = "After26";
    QString b26 = "Before26";
    QString b10 = "Before10";
    QCOMPARE(m_game->setAnnotation(a5, 5, Game::AfterMove), true);
    QCOMPARE(m_game->setAnnotation(a10, 10, Game::AfterMove), true);
    QCOMPARE(m_game->setAnnotation(b10, 10, Game::BeforeMove), true);
    QCOMPARE(m_game->setAnnotation(a26, 26, Game::AfterMove), true);
    QCOMPARE(m_game->setAnnotation(b26, 26, Game::BeforeMove), true);
    QCOMPARE(m_game->annotation(5, Game::AfterMove), a5);
    QCOMPARE(m_game->annotation(6, Game::BeforeMove), a5);
    QCOMPARE(m_game->annotation(10, Game::AfterMove), a10);
    QCOMPARE(m_game->annotation(10, Game::BeforeMove), b10);
    QCOMPARE(m_game->annotation(26, Game::AfterMove), a26);
    QCOMPARE(m_game->annotation(26, Game::BeforeMove), b26);

}
void GameTest::testLineLimits_data()
{
    QTest::addColumn<int> ("node");
    QTest::addColumn<bool> ("isMainLine");
    QTest::addColumn<bool> ("atLineStart");
    QTest::addColumn<bool> ("atLineEnd");

    QTest::newRow("1") << 1 << true << true << false;
    QTest::newRow("2") << 2 << true << false << false;
    QTest::newRow("3") << 3 << true << false << false;
    QTest::newRow("4") << 4 << true << false << false;
    QTest::newRow("5") << 5 << true << false << false;
    QTest::newRow("6") << 6 << true << false << false;
    QTest::newRow("7") << 7 << true << false << false;
    QTest::newRow("8") << 8 << true << false << false;
    QTest::newRow("9") << 9 << true << false << false;
    QTest::newRow("10") << 10 << true << false << false;
    QTest::newRow("11") << 11 << true << false << false;
    QTest::newRow("12") << 12 << true << false << false;
    QTest::newRow("13") << 13 << true << false << false;
    QTest::newRow("14") << 14 << true << false << false;
    QTest::newRow("15") << 15 << true << false << false;
    QTest::newRow("16") << 16 << true << false << false;
    QTest::newRow("17") << 17 << true << false << false;
    QTest::newRow("18") << 18 << true << false << false;
    QTest::newRow("19") << 19 << true << false << false;
    QTest::newRow("20") << 20 << true << false << false;
    QTest::newRow("21") << 21 << true << false << false;
    QTest::newRow("22") << 22 << true << false << false;
    QTest::newRow("23") << 23 << true << false << false;
    QTest::newRow("24") << 24 << true << false << false;
    QTest::newRow("25") << 25 << true << false << true;

    QTest::newRow("1") << 26 << false << true << false;
    QTest::newRow("2") << 27 << false << false << false;
    QTest::newRow("2") << 28 << false << false << false;
    QTest::newRow("2") << 29 << false << false << false;
    QTest::newRow("2") << 30 << false << false << false;
    QTest::newRow("2") << 31 << false << false << false;
    QTest::newRow("2") << 32 << false << false << true;

    QTest::newRow("2") << 33 << false << true << false;
    QTest::newRow("2") << 34 << false << false << false;
    QTest::newRow("2") << 35 << false << false << false;
    QTest::newRow("2") << 36 << false << false << false;
    QTest::newRow("2") << 37 << false << false << false;
    QTest::newRow("2") << 38 << false << false << false;
    QTest::newRow("2") << 39 << false << false << false;
    QTest::newRow("2") << 40 << false << false << false;
    QTest::newRow("2") << 41 << false << false << false;
    QTest::newRow("2") << 42 << false << false << false;
    QTest::newRow("2") << 43 << false << false << true;

    QTest::newRow("2") << 44 << false << true << false;
    QTest::newRow("2") << 45 << false << false << false;
    QTest::newRow("2") << 46 << false << false << false;
    QTest::newRow("2") << 47 << false << false << false;
    QTest::newRow("2") << 48 << false << false << false;
    QTest::newRow("2") << 49 << false << false << true;
}
void GameTest::testLineLimits()
{
    QFETCH(int, node);
    QFETCH(bool, isMainLine);
    QFETCH(bool, atLineStart);
    QFETCH(bool, atLineEnd);
    QCOMPARE(m_game->isMainline(node), isMainLine);
    QCOMPARE(m_game->atLineStart(node), atLineStart);
    QCOMPARE(m_game->atLineEnd(node), atLineEnd);
}
void GameTest::testNavigation_data()
{
    QTest::addColumn<int> ("node");
    QTest::addColumn<int> ("pnode");
    QTest::addColumn<int> ("nnode");
    QTest::addColumn<bool> ("isMainLine");
    QTest::addColumn<bool> ("atLineStart");
    QTest::addColumn<bool> ("atLineEnd");

    QTest::newRow("0") << 0 << NO_MOVE << 1 << false << true << false;
    QTest::newRow("1") << 1 << 0 << 2 << true << true << false;
    QTest::newRow("2") << 2 << 1 << 3 << true << false << false;
    QTest::newRow("3") << 3 << 2 << 4 << true << false << false;
    QTest::newRow("4") << 4 << 3 << 5 << true << false << false;
    QTest::newRow("5") << 5 << 4 << 6 << true << false << false;
    QTest::newRow("6") << 6 << 5 << 7 << true << false << false;
    QTest::newRow("7") << 7 << 6 << 8 << true << false << false;
    QTest::newRow("8") << 8 << 7 << 9 << true << false << false;
    QTest::newRow("9") << 9 << 8 << 10 << true << false << false;
    QTest::newRow("10") << 10 << 9 << 11 << true << false << false;
    QTest::newRow("11") << 11 << 10 << 12 << true << false << false;
    QTest::newRow("12") << 12 << 11 << 13 << true << false << false;
    QTest::newRow("13") << 13 << 12 << 14 << true << false << false;
    QTest::newRow("14") << 14 << 13 << 15 << true << false << false;
    QTest::newRow("15") << 15 << 14 << 16 << true << false << false;
    QTest::newRow("16") << 16 << 15 << 17 << true << false << false;
    QTest::newRow("17") << 17 << 16 << 18 << true << false << false;
    QTest::newRow("18") << 18 << 17 << 19 << true << false << false;
    QTest::newRow("19") << 19 << 18 << 20 << true << false << false;
    QTest::newRow("20") << 20 << 19 << 21 << true << false << false;
    QTest::newRow("21") << 21 << 20 << 22 << true << false << false;
    QTest::newRow("22") << 22 << 21 << 23 << true << false << false;
    QTest::newRow("23") << 23 << 22 << 24 << true << false << false;
    QTest::newRow("24") << 24 << 23 << 25 << true << false << false;
    QTest::newRow("25") << 25 << 24 << NO_MOVE << true << false << true;

    QTest::newRow("26") << 26 << 9 << 27 << false << true << false;
    QTest::newRow("27") << 27 << 26 << 28 << false << false << false;
    QTest::newRow("28") << 28 << 27 << 29 << false << false << false;
    QTest::newRow("29") << 29 << 28 << 30 << false << false << false;
    QTest::newRow("30") << 30 << 29 << 31 << false << false << false;
    QTest::newRow("31") << 31 << 30 << 32 << false << false << false;
    QTest::newRow("32") << 32 << 31 << NO_MOVE << false << false << true;

    QTest::newRow("33") << 33 << 26 << 34 << false << true << false;
    QTest::newRow("34") << 34 << 33 << 35 << false << false << false;
    QTest::newRow("35") << 35 << 34 << 36 << false << false << false;
    QTest::newRow("36") << 36 << 35 << 37 << false << false << false;
    QTest::newRow("37") << 37 << 36 << 38 << false << false << false;
    QTest::newRow("38") << 38 << 37 << 39 << false << false << false;
    QTest::newRow("39") << 39 << 38 << 40 << false << false << false;
    QTest::newRow("40") << 40 << 39 << 41 << false << false << false;
    QTest::newRow("41") << 41 << 40 << 42 << false << false << false;
    QTest::newRow("42") << 42 << 41 << 43 << false << false << false;
    QTest::newRow("43") << 43 << 42 << NO_MOVE << false << false << true;

    QTest::newRow("44") << 44 << 35 << 45 << false << true << false;
    QTest::newRow("45") << 45 << 44 << 46 << false << false << false;
    QTest::newRow("46") << 46 << 45 << 47 << false << false << false;
    QTest::newRow("47") << 47 << 46 << 48 << false << false << false;
    QTest::newRow("48") << 48 << 47 << 49 << false << false << false;
    QTest::newRow("49") << 49 << 48 << NO_MOVE << false << false << true;
}
void GameTest::testNavigation()
{
    QFETCH(int, node);
    QFETCH(int, pnode);
    QFETCH(int, nnode);
    QFETCH(bool, atLineStart);
    if(node == 0)
    {
        m_game->moveToStart();
    }
    else
    {
        if(atLineStart)
        {
            m_game->moveToId(node);
        }
        else
        {
            m_game->forward();
        }
        QCOMPARE(m_game->currentMove(), node);
        QCOMPARE(m_game->nextMove(), nnode);
        QCOMPARE(m_game->previousMove(), pnode);
    }
}
void GameTest::testTags()
{
    QFETCH(QString, tagName);
    QFETCH(QString, tagValue);
    QFETCH(int, result);
    m_game->setTag(tagName, tagValue);
    QCOMPARE(m_game->tag(tagName), tagValue);

    QMap<QString, QString> tags = m_game->tags();
    QMap<QString, QString>::const_iterator i = tags.constBegin();
    while(i != tags.constEnd())
    {
        if(i.key() == tagName)
        {
            QCOMPARE(i.value(), tagValue);
        }
        ++i;
    }
    if(tagName == "Result")
    {
        m_game->setResult((Result)result);
        QCOMPARE(m_game->result(), (Result)result);
        QCOMPARE(m_game->tag(tagName), tagValue);
    }

    m_game->removeTag(tagName);
    tags = m_game->tags();
    i = tags.constBegin();
    while(i != tags.constEnd())
    {
        QVERIFY(i.key() != tagName);
        ++i;
    }
}
void GameTest::testTags_data()
{
    QTest::addColumn<QString> ("tagName");
    QTest::addColumn<QString> ("tagValue");
    QTest::addColumn<int> ("result");

    QTest::newRow("1") << "White" << "White Player" << 0;
    QTest::newRow("2") << "Black" << "Black Player" << 0;
    QTest::newRow("3") << "PlyCount" << "11" << 0;
    QTest::newRow("4") << "Result" << "1-0" << (int)WhiteWin;
    QTest::newRow("5") << "Result" << "0-1" << (int)BlackWin;
    QTest::newRow("6") << "Result" << "1/2-1/2" << (int)Draw;
    QTest::newRow("7") << "Result" << "*" << (int)Unknown;
    QTest::newRow("8") << "Result" << "1-0" << (int)WhiteWin;
}
void GameTest::testCounters()
{
    int moves, comments, nags;
    m_game->moveCount(&moves, &comments, &nags);
    QCOMPARE(moves, 25);
    QCOMPARE(comments, 5);
    QCOMPARE(nags, 0);
    m_game->moveToId(9);
    QCOMPARE(m_game->variationCount(), 1);
    QCOMPARE(m_game->ply(), 9);
    QCOMPARE(m_game->moveNumber(), 5);
    m_game->forward();
    QCOMPARE(m_game->moveNumber(), 5);
    m_game->backward();
    m_game->backward();
    QCOMPARE(m_game->moveNumber(), 4);
    QCOMPARE(m_game->plyCount(), 25);
    m_game->moveToId(29);
    QCOMPARE(m_game->variationNumber(), 26);
    QCOMPARE(m_game->variationCount(), 0);
}
void GameTest::testVariationManipulation()
{
    m_game->moveToId(47);
    m_game->truncateVariation();
    m_game->moveToId(44);
}
