/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 27/11/2005
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
The main for the cppunit test case runner. The program runs all registered test cases.
There is no need to modify this file when adding new testcases.
*/

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <qdir.h>

void tidyUp();

int main(int argc, char* argv[])
{
    // Get the top level suite from the registry
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    
    // Adds the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite );
    
        
    // Change the default outputter to a compiler error format outputter
    runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
							 std::cerr ) );
    // Run the tests.
    bool wasSucessful = runner.run();

    tidyUp(); 

    // Return error code 1 if the one of test failed.
    return wasSucessful ? 0 : 1;
    
}

/** Removing files created during the test - but not properly cleaned up within the tests.
Used for convinience
*/
void tidyUp()
{
  QDir toRemove;
  toRemove.remove( "./data/small/change.cpd" );
  toRemove.remove( "./data/small/change.cpm" );
  toRemove.remove( "./data/small/players_converted.cpd" );
  toRemove.remove( "./data/small/players_converted.cpm" );
  toRemove.remove( "./spelling/spell.txt" );
  toRemove.remove( "./spelling/newspell.txt" );
  toRemove.remove( "./spelling/spell_1.txt" );
}
