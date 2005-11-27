/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 15/09/2005
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


#ifndef __SPELLCHECKERTESTS_H 
#define __SPELLCHECKERTESTS_H  

#include<spellchecker.h> 
#include <cppunit/extensions/HelperMacros.h>


class SpellCheckerTests : public CppUnit::TestFixture 
{

  CPPUNIT_TEST_SUITE( SpellCheckerTests );
  CPPUNIT_TEST( testNoRule );
  CPPUNIT_TEST( testImportSpellingFile );
  CPPUNIT_TEST( testSaveSpellings );
  CPPUNIT_TEST( testRemoveRules );
  CPPUNIT_TEST( testLoadSpellings );
  CPPUNIT_TEST( testCorrectPlayer );
  CPPUNIT_TEST( testCorrectSite );
  CPPUNIT_TEST( testCorrectEvent );
  CPPUNIT_TEST( testCorrectRound );
  CPPUNIT_TEST( testFindSpellingsLiteral );
  CPPUNIT_TEST( testAddRuleSpelling );
  CPPUNIT_TEST( testRemoveRule );
  CPPUNIT_TEST( testRemoveRuleFail );
  CPPUNIT_TEST( testFindSpellingsLiteralInNewSpell );
  CPPUNIT_TEST_SUITE_END(); 

    
 public: 
  void setUp();
  void tearDown();

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
