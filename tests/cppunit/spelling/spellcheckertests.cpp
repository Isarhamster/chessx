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

#include<spelling/spellcheckertests.h>

#include<qstring.h> 
#include<qstringlist.h>


#include<iostream> 

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SpellCheckerTests );

  
void SpellCheckerTests::setUp()
{ 
  //not used
} 
    
void SpellCheckerTests::tearDown()
{ 
  //not used
} 

void SpellCheckerTests::testNoRule(){
  CPPUNIT_ASSERT_EQUAL( 0 , speller.count() );
}

void SpellCheckerTests::testImportSpellingFile(){
  const QString spelling_file = "./spelling/spelling.ssp"; 

  CPPUNIT_ASSERT( speller.import( spelling_file ) );
  speller.save( "./spelling/spell.txt" );
}

void SpellCheckerTests::testLoadSpellings(){
  CPPUNIT_ASSERT( speller.load( "./spelling/spell.txt" ) );
}

void SpellCheckerTests::testSaveSpellings(){
  speller.load( "./spelling/spell.txt" );
  bool saveIsOk = speller.save( "./spelling/spell_1.txt" );
  CPPUNIT_ASSERT( saveIsOk );
}

void SpellCheckerTests::testRemoveRules(){
  speller.clear();
  CPPUNIT_ASSERT_EQUAL( 0 , speller.count() );
}

void SpellCheckerTests::testCorrectPlayer(){
  Spellchecker::SpellingType spellingType = Spellchecker::Player;
  const QString typo = "Kortchnoi, Viktor L";
  const QString correct = "Korchnoi, Viktor L";

  speller.load( "./spelling/spell.txt" );
  CPPUNIT_ASSERT_EQUAL( correct , speller.correct( typo , spellingType ) );
}

void SpellCheckerTests::testCorrectSite(){
  Spellchecker::SpellingType spellingType = Spellchecker::Site;
  const QString typo = "Wien AUT";
  const QString correct = "Vienna AUT";
 
  speller.load( "./spelling/spell.txt" );
  CPPUNIT_ASSERT_EQUAL( correct , speller.correct( typo , spellingType ) );
}

void SpellCheckerTests::testCorrectEvent(){
  Spellchecker::SpellingType spellingType = Spellchecker::Event;
  const QString typo = "(open)";
  const QString correct = "Open";
  speller.load( "./spelling/spell.txt" );
  CPPUNIT_ASSERT_EQUAL( correct , speller.correct( correct , spellingType ) );
}

void SpellCheckerTests::testCorrectRound(){
  Spellchecker::SpellingType spellingType = Spellchecker::Round;
  const QString typo = "( 02 )";
  const QString correct = "2";
  speller.load( "./spelling/spell.txt" );
  CPPUNIT_ASSERT_EQUAL( correct , speller.correct( correct , spellingType ) );
}

void SpellCheckerTests::testFindSpellingsLiteral(){
  Spellchecker::SpellingType spellingType = Spellchecker::Player;
  Spellchecker::RuleType ruleType = Spellchecker::Literal;
  const QString correct = "Korchnoi, Viktor L";
  speller.load( "./spelling/spell.txt" );
  /**on reading the comment on findSpellings I think that it schould return only 13 
     it seems to me that the result includes also the correct spelling) */
  QStringList incorrect = speller.findSpellings( correct, ruleType, spellingType );
  size_t size = 13;

  //  for ( QStringList::Iterator it = incorrect.begin(); it != incorrect.end(); ++it ) {
  //  std::cout << "testFindSpellingsLiteral: " << ( *it ).ascii() << std::endl;
  //  } 

  CPPUNIT_ASSERT_EQUAL( size , incorrect.count() );
}


void SpellCheckerTests::testAddRuleSpelling(){
  newSpell.addRule( "Karpow, Anatoly", "Karpov, Anatoly", Spellchecker::Literal, Spellchecker::Player );
  newSpell.addRule( "Moskau", "Moscow RUS", Spellchecker::Literal, Spellchecker::Site );
  newSpell.addRule( "Aeroflot op", "Aeroflot Open", Spellchecker::Literal, Spellchecker::Site );
  newSpell.addRule( "02", "2", Spellchecker::Literal, Spellchecker::Round );

  newSpell.save( "./spelling/newspell.txt" );

  CPPUNIT_ASSERT_EQUAL( 4 , newSpell.count() );
}

void SpellCheckerTests::testRemoveRule(){
  newSpell.load( "./spelling/newspell.txt" );
  CPPUNIT_ASSERT( newSpell.removeRule( "02", Spellchecker::Literal, Spellchecker::Round ) );
  CPPUNIT_ASSERT_EQUAL( 3 , newSpell.count() );

  CPPUNIT_ASSERT( newSpell.removeRule( "Moskau", Spellchecker::Literal, Spellchecker::Site ) );
  CPPUNIT_ASSERT_EQUAL( 2 , newSpell.count() );
}

void SpellCheckerTests::testRemoveRuleFail(){
  newSpell.load( "./spelling/newspell.txt" );
  bool removeIsOk = newSpell.removeRule( "02", Spellchecker::Literal, Spellchecker::Round ); 
  CPPUNIT_ASSERT( removeIsOk );
}

void SpellCheckerTests::testFindSpellingsLiteralInNewSpell(){
  Spellchecker::SpellingType spellingType = Spellchecker::Player;
  Spellchecker::RuleType ruleType = Spellchecker::Literal;
  const QString correct = "Karpov, Anatoly";
  newSpell.load( "./spelling/newspell.txt" );
  QStringList incorrect = newSpell.findSpellings( correct, ruleType, spellingType ); 
  // for ( QStringList::Iterator it = incorrect.begin(); it != incorrect.end(); ++it ) {
  //  std::cout << "testFindSpellingsLiteralInNewSpell: " << (*it).ascii() << std::endl; }
  
    size_t size = 1;

    /**should return 1 */

    CPPUNIT_ASSERT_EQUAL( size , incorrect.count() );
}
