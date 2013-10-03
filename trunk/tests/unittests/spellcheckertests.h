/***************************************************************************
                          spellcheckertests.h  -  description
                             -------------------
    begin                : 15/09/2005
    copyright            : (C) 2005, 2006 Heinz R. Hopfgartner
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
   Test the speellchecker class.
*/


#ifndef __SPELLCHECKERTESTS_H
#define __SPELLCHECKERTESTS_H

#include<spellchecker.h>
#include <QtTest/QtTest>


class SpellCheckerTests : public QObject
{

    Q_OBJECT

private slots:

    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testNoRule();
    void testImportSpellingFile();
    void testSaveSpellings();
    void testRemoveRules();
    void testLoadSpellings();
    void testCorrectPlayer();
    void testCorrectSite();
    void testCorrectEvent();
    void testCorrectRound();
    void testFindSpellingsLiteral();
    void testAddRuleSpelling();
    void testRemoveRule();
    void testRemoveRuleFail();
    void testFindSpellingsLiteralInNewSpell();

private:

    /** used to import the scid spellchecker file */
    Spellchecker speller;

    /** used for a blank spellchecker file */
    Spellchecker newSpell;
};


#endif
