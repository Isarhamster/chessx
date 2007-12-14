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

#ifndef GAMETEST_H
#define GAMETEST_H

#include <QtTest/QtTest>
#include "game.h"


class GameTest : public QObject
{

	Q_OBJECT

private slots:
	void initTestCase();
	void init();
	void cleanup();
	void cleanupTestCase();

	void testAddMove_data();
	void testAddMove();
	void testAddVariation_data();
	void testAddVariation();
	void testAnnotation();
	void testLineLimits_data();
	void testLineLimits();
	void testNavigation_data();
	void testNavigation();
	void testTags();
	void testCounters();
	void testVariationManipulation();

   void testTags_data();
	//void testName();
	//void testLoad();
	//void testCopyGameIntoNewDB();
	//void testRemoveGame();
	//  void testExecuteSearch();
	//  void testSave();
private:
   Game *m_game;

};

#endif

/*    
      * bool isMainline(MoveId moveId = CURRENT_MOVE);
      * int currentMoveId() const;
      * QString annotation(MoveId moveId = CURRENT_MOVE,AnnotationPosition position = AfterMove) const;
      NagSet nags(MoveId moveId = CURRENT_MOVE) const;
      QString moveToSan(MoveId moveId = CURRENT_MOVE, MoveStringFlags flags = MoveOnly);
      * bool setAnnotation(QString annotation, MoveId moveId = CURRENT_MOVE, AnnotationPosition position=AfterMove);
      bool addNag(Nag nag, MoveId moveId = CURRENT_MOVE);
      bool setNags(NagSet nags, MoveId moveId = CURRENT_MOVE);
      * bool atLineStart(MoveId moveId = CURRENT_MOVE) const;
      * bool atLineEnd(MoveId moveId = CURRENT_MOVE) const;
      * void moveCount(int* moves, int* comments, int* nags);
      * int ply(MoveId moveId = CURRENT_MOVE) const;
      * int moveNumber(MoveId moveId = CURRENT_MOVE) const;
      * int plyCount() const;
      * int variationNumber(MoveId moveId = CURRENT_MOVE) const;
      * int variationCount(MoveId moveId = CURRENT_MOVE) const;
      * MoveId previousMove() const;
      * MoveId nextMove() const;
      * QList <MoveId> variations() const;

      * void moveToStart();
      * void moveToEnd();
      * int moveByPly(int diff);
      * void moveToId(MoveId moveId);
      * int forward(int count = 1);
      * int backward(int count = 1);

      * int addMove(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
      * int addMove(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
      * int addVariation(const Move& move, const QString& annotation = QString(), NagSet nags = NagSet());
      * int addVariation(const QString& sanMove, const QString& annotation = QString(), NagSet nags = NagSet());
      bool promoteVariation(int variation);
      bool removeVariation(int variation);
      |+* Removes all variations and mainline moves after the current position +|
      void truncateVariation();
      |+* @return value of the given tag +|
      * QString tag(const QString& tag) const;
      * QMap<QString, QString> tags() const;
      * void setTag(const QString& tag, const QString& value);
      * void removeTag(const QString& tag);
      * void setResult(Result result);
      * Result result() const;

      */

