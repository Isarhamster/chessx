/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 9/06/2008
    copyright            : (C) 2008 Aliaksandr Antonik
                           <forest.aa@gmail.com>
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
Unit tests for the PositionSearch class
*/

#ifndef POSITIONSEARCH_H
#define POSITIONSEARCH_H

#include <QtTest/QtTest>

class PositionSearchTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testSearch();
};

#endif

