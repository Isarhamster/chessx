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
Unit tests for the Output class.
*/

#include "outputtest.h"
#include "output.h"
#include "qstring.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( OutputTest );

/*
void OutputTest::setUp()
{
}

void OutputTest::tearDown()
{
}
*/

void OutputTest::testOptions()
{
   // Need to pass template file path as parameter
   // because AppSettings is not defined here
   Output htmlOutput (Output::Html,"/home/mroets/data/chessx/qt3/data/templates/html-default.template");

   /* Set standard options */
   CPPUNIT_ASSERT( htmlOutput.setOption("ColumnStyle", true) );
   CPPUNIT_ASSERT( htmlOutput.setOption("TextWidth",60) );
   CPPUNIT_ASSERT( htmlOutput.setOption("VariationIndentLevel",8) );
   CPPUNIT_ASSERT( htmlOutput.setOption("VariationIndentSize",4 ));
   CPPUNIT_ASSERT( htmlOutput.setOption("CommentIndent",QString("Always")) );

   CPPUNIT_ASSERT( htmlOutput.setOption("MainLineMoveColor",QString("#000000")) );
   CPPUNIT_ASSERT( htmlOutput.setOption("VariationColor",QString("#0000ff")) );
   CPPUNIT_ASSERT( htmlOutput.setOption("CommentColor",QString("#00ff00")) );
   CPPUNIT_ASSERT( htmlOutput.setOption("NagColor",QString("#ff0000")) );
   CPPUNIT_ASSERT( htmlOutput.setOption("HeaderColor",QString("#0000ff")) );

   CPPUNIT_ASSERT( !htmlOutput.setOption("SomeNonExistentOption1",false));
   CPPUNIT_ASSERT( !htmlOutput.setOption("SomeNonExistentOption2",QString("somestring")));
   CPPUNIT_ASSERT( !htmlOutput.setOption("SomeNonExistentOption3",42));
   CPPUNIT_ASSERT( !htmlOutput.setOption("HeaderColor",QString("badcolor")) );
   CPPUNIT_ASSERT( !htmlOutput.setOption("CommentIndent",QString("badoption")) );
   CPPUNIT_ASSERT( !htmlOutput.setOption("VariationIndentLevel",5400) ); // out of range

   CPPUNIT_ASSERT( htmlOutput.getOptionAsBool("ColumnStyle") );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsInt("TextWidth") == 60 );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsInt("VariationIndentLevel") == 8 );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsInt("VariationIndentSize") == 4 );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("CommentIndent") == QString("Always") );

   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("MainLineMoveColor") == QString("#000000") );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("VariationColor") == QString("#0000ff") );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("CommentColor") == QString("#00ff00") );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("NagColor") == QString("#ff0000") );
   CPPUNIT_ASSERT( htmlOutput.getOptionAsString("HeaderColor") == QString("#0000ff") );

   // Now I could create some new options and test them too
}

