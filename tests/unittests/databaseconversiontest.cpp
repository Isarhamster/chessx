/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005, 2006, 2007 Heinz R. Hopfgartner
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
Unit tests for the DatabaseConversion class.
*/

#include "databaseconversiontest.h"

#include "playerdatabase.h"
#include "databaseconversion.h"

void DatabaseConversionTest::init() {};
void DatabaseConversionTest::cleanup() {};

void DatabaseConversionTest::initTestCase()
{
    db_name = "./data/small/players_converted";
    source = "./data/small/ratings.ssp";
    picture_dir = "./data/small/playerphotos";
}

void DatabaseConversionTest::cleanupTestCase() {}

void DatabaseConversionTest::testConvertDatabase()
{
    DatabaseConversion converter;
    QVERIFY(converter.playerDatabaseFromScidRatings(source, db_name, picture_dir));
}
