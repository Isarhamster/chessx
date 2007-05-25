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
Unit tests for the Board class.
*/

#ifndef BOARDTEST_H
#define BOARDTEST_H

#include <QtTest/QtTest>

class BoardTest : public QObject
{
	Q_OBJECT

private slots:
	void testIsValidFEN();
	void testCreateHash();
	void testValidate();
	void testReversableHash();
};

#endif
