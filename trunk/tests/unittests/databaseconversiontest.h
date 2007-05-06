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

#ifndef DATABASECONVERSIONTEST_H
#define DATABASECONVERSIONTEST_H

#include <QtTest/QtTest>
#include <QString>

class DatabaseConversionTest : public QObject
{

Q_OBJECT

private slots:
  void initTestCase();
  void init();
  void cleanup();
  void cleanupTestCase();

  void testConvertDatabase();

 private:

  QString db_name;
  QString source;
  QString picture_dir;
};

#endif 
