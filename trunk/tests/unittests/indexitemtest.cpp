/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 26/02/2007
    copyright            : (C) 2006 Marius Roets
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
Unit tests for the IndexItem class
*/

#include <QtDebug>

#include "indexitemtest.h"
#include "indexitem.h"



void IndexItemTest::initTestCase() {}
void IndexItemTest::init(){}
void IndexItemTest::cleanup(){}
void IndexItemTest::cleanupTestCase() {}

void IndexItemTest::testIndexItemCompleteTest()
{
	IndexItem *item = new IndexItem();
	int val;

	item->set(0, 2, 0x45);
	item->set(2, 2, 0x0474);
	item->set(4, 4, 0x31323334);
	item->set(8, 3, 0x414243);
	item->set(11, 2, 0x6666);
	val = item->index(0, 2);
	QCOMPARE(val , 0x45);
	val = item->index(2, 2);
	QCOMPARE(val , 0x0474);
	val = item->index(4, 4);
	QCOMPARE(val , 0x31323334);
	val = item->index(8, 3);
	QCOMPARE(val , 0x414243);
	val = item->index(11, 2);
	QCOMPARE(val , 0x6666);

	item->set(2, 2, 0x49);
	item->set(8, 3, 0x444546);
	val = item->index(0, 2);
	QCOMPARE(val , 0x45);
	val = item->index(2, 2);
	QCOMPARE(val , 0x49);
	val = item->index(4, 4);
	QCOMPARE(val , 0x31323334);
	val = item->index(8, 3);
	QCOMPARE(val , 0x444546);
	val = item->index(11, 2);
	QCOMPARE(val , 0x6666);

	item->allocate(40, false);
	item->set(38, 2, 0x7777);
	val = item->index(38, 2);
	QCOMPARE(val , 0x7777);
	val = item->set(39, 2, 0x3333);
	QCOMPARE(val , -1);
	val = item->index(0, 2);
	QCOMPARE(val , 0x45);
	val = item->index(2, 2);
	QCOMPARE(val , 0x49);
	val = item->index(4, 4);
	QCOMPARE(val , 0x31323334);
	val = item->index(8, 3);
	QCOMPARE(val , 0x444546);
	val = item->index(11, 2);
	QCOMPARE(val , 0x6666);

	delete item;
}


