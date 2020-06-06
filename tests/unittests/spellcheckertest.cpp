/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 15/10/2005
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
   Test the speellchecker class.
*/

#include "spellcheckertest.h"

#include "resourcepath.h"
#include "spellchecker.h"

//void SpellCheckerTest::testNoRule()
//{
//    QCOMPARE(speller.count(), 0);
//}
//
//void SpellCheckerTest::testImportSpellingFile()
//{
//    const QString spelling_file = "./data/spelling.ssp";
//
//    QVERIFY(speller.import(spelling_file));
//    speller.save("./data/spell.txt");
//}
//
//void SpellCheckerTest::testLoadSpellings()
//{
//    QVERIFY(speller.load("./data/spell.txt"));
//}
//
//void SpellCheckerTest::testSaveSpellings()
//{
//    speller.load("./data/spell.txt");
//    bool saveIsOk = speller.save("./data/spell_1.txt");
//    QVERIFY(saveIsOk);
//}
//
//void SpellCheckerTest::testRemoveRules()
//{
//    speller.clear();
//    QCOMPARE(speller.count(), 0);
//}
//
//void SpellCheckerTest::testCorrectPlayer()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Player;
//    const QString typo = "Kortchnoi, Viktor L";
//    const QString correct = "Korchnoi, Viktor L";
//
//    speller.load("./data/spell.txt");
//    QCOMPARE(speller.correct(typo , spellingType), correct);
//}
//
//void SpellCheckerTest::testCorrectSite()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Site;
//    const QString typo = "Wien AUT";
//    const QString correct = "Vienna AUT";
//
//    speller.load("./data/spell.txt");
//    QCOMPARE(speller.correct(typo , spellingType), correct);
//}
//
//void SpellCheckerTest::testCorrectEvent()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Event;
//    const QString typo = "(open)";
//    const QString correct = "Open";
//    speller.load("./data/spell.txt");
//    QCOMPARE(speller.correct(correct , spellingType), correct);
//}
//
//void SpellCheckerTest::testCorrectRound()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Round;
//    const QString typo = "( 02 )";
//    const QString correct = "2";
//    speller.load("./data/spell.txt");
//    QCOMPARE(speller.correct(correct , spellingType), correct);
//}
//
//void SpellCheckerTest::testFindSpellingsLiteral()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Player;
//    Spellchecker::RuleType ruleType = Spellchecker::Literal;
//    const QString correct = "Korchnoi, Viktor L";
//    speller.load("./data/spell.txt");
//    QStringList incorrect = speller.findSpellings(correct, ruleType, spellingType);
//    QCOMPARE(incorrect.count(), 13);
//
//    //for ( QStringList::Iterator it = incorrect.begin(); it != incorrect.end(); ++it ) {
//    //  qDebug() << "testFindSpellingsLiteral:" << ( *it );
//    //}
//}
//
//
//void SpellCheckerTest::testAddRuleSpelling()
//{
//    newSpell.addRule("Karpow, Anatoly", "Karpov, Anatoly", Spellchecker::Literal, Spellchecker::Player);
//    newSpell.addRule("Moskau", "Moscow RUS", Spellchecker::Literal, Spellchecker::Site);
//    newSpell.addRule("Aeroflot op", "Aeroflot Open", Spellchecker::Literal, Spellchecker::Site);
//    newSpell.addRule("02", "2", Spellchecker::Literal, Spellchecker::Round);
//
//    newSpell.save("./data/newspell.txt");
//
//    QCOMPARE(newSpell.count(), 4);
//}
//
//void SpellCheckerTest::testRemoveRule()
//{
//    newSpell.load("./data/newspell.txt");
//    QVERIFY(newSpell.removeRule("02", Spellchecker::Literal, Spellchecker::Round));
//    QCOMPARE(newSpell.count(), 3);
//
//    QVERIFY(newSpell.removeRule("Moskau", Spellchecker::Literal, Spellchecker::Site));
//    QCOMPARE(newSpell.count(), 2);
//}
//
//void SpellCheckerTest::testRemoveRuleFail()
//{
//    newSpell.load("./data/newspell.txt");
//    bool removeIsOk = newSpell.removeRule("02", Spellchecker::Literal, Spellchecker::Round);
//    QVERIFY(removeIsOk);
//}
//
//void SpellCheckerTest::testFindSpellingsLiteralInNewSpell()
//{
//    Spellchecker::SpellingType spellingType = Spellchecker::Player;
//    Spellchecker::RuleType ruleType = Spellchecker::Literal;
//    const QString correct = "Karpov, Anatoly";
//    newSpell.load("./data/newspell.txt");
//    QStringList incorrect = newSpell.findSpellings(correct, ruleType, spellingType);
//    // for ( QStringList::Iterator it = incorrect.begin(); it != incorrect.end(); ++it ) {
//    //  std::cout << "testFindSpellingsLiteralInNewSpell: " << (*it).ascii() << std::endl; }
//
//    //     size_t size = 1;
//
//    /**should return 1 */
//
//    //    QCOMPARE( incorrect.count(), size );
//    QCOMPARE(incorrect.count(), 1);
//}

void SpellCheckerTest::testBasics()
{
    Spellchecker checker;
    QFETCH(bool, useSerialization);

    QCOMPARE(checker.count(), 0);

    checker.addRule("Karpow, Anatoly", "Karpov, Anatoly", Spellchecker::Literal, Spellchecker::Player);
    checker.addRule("Moskau", "Moscow RUS", Spellchecker::Literal, Spellchecker::Site);
    checker.addRule("Aeroflot op", "Aeroflot Open", Spellchecker::Literal, Spellchecker::Site);
    checker.addRule("02", "2", Spellchecker::Literal, Spellchecker::Round);

    // optionally save/load spell checker
    if (useSerialization)
    {
        // obtain temporary filename
        QTemporaryFile tmp("spellcheck-XXXXXX.txt");
        QVERIFY(tmp.open());
        tmp.close();

        QVERIFY(checker.save(tmp.fileName()));

        Spellchecker loaded;
        QVERIFY(loaded.load(tmp.fileName()));

        QCOMPARE(loaded.count(), checker.count());
        // use loaded instance for further checks
        qSwap(loaded, checker);
    }

    QCOMPARE(checker.count(), 4);
    // test findSpellings()
    {
        auto spellings = checker.findSpellings("Karpov, Anatoly", Spellchecker::Literal, Spellchecker::Player);
        QCOMPARE(spellings.count(), 1);
    }

    QVERIFY(checker.removeRule("02", Spellchecker::Literal, Spellchecker::Round));
    QCOMPARE(checker.count(), 3);
    QVERIFY(checker.removeRule("Moskau", Spellchecker::Literal, Spellchecker::Site));
    QCOMPARE(checker.count(), 2);

    checker.clear();
    QCOMPARE(checker.count(), 0);
}

void SpellCheckerTest::testBasics_data()
{
    QTest::addColumn<bool> ("useSerialization");

    QTest::newRow("with serialization") << true;
    QTest::newRow("without serialization") << false;
}

void SpellCheckerTest::testImport()
{
    Spellchecker checker;

    // import Scid spellcheck file
    QVERIFY(checker.import(RESOURCE_PATH "spelling.ssp"));

    // findSpellings()
    {
        auto spellings = checker.findSpellings("Korchnoi, Viktor L", Spellchecker::Literal, Spellchecker::Player);
        QCOMPARE(spellings.count(), 13);
    }

    // player
    {
        const QString mistake("Kortchnoi, Viktor L");
        const QString correct("Korchnoi, Viktor L");
        QCOMPARE(checker.correct(mistake, Spellchecker::Player), correct);
    }
    // site
    {
        const QString mistake("Wien AUT");
        const QString correct("Vienna AUT");
        QCOMPARE(checker.correct(mistake, Spellchecker::Site), correct);
    }
    // event
    {
        const QString mistake("(open)");
        const QString correct("Open");
        QCOMPARE(checker.correct(mistake, Spellchecker::Event), correct);
    }
    // round
    {
        const QString mistake("( 02 )");
        const QString correct("2");
        QCOMPARE(checker.correct(mistake, Spellchecker::Round), correct);
    }
}
