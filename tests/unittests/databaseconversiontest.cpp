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

#include <QTemporaryDir>

#include "resourcepath.h"

#include "databaseconversion.h"

void DatabaseConversionTest::testConvertDatabase()
{
    QTemporaryDir tmpDir(QDir::tempPath() + "/chessx_test_pdb");

    DatabaseConversion converter;
    QVERIFY(converter.playerDatabaseFromScidRatings(RESOURCE_PATH "small/ratings.ssp", tmpDir.filePath("converted"), tmpDir.filePath("photos")));
}
