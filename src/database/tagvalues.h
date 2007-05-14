/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 20/02/2007
    copyright            : (C) 2007 Marius Roets
                           <saidinwielder@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef __TAGVALUES_H__
#define __TAGVALUES_H__

#include <QString>
#include <QStringList>
#include <QHash>
#include <QBitArray>

typedef quint32 TagIndex;

/** @ingroup Database
TagValues class holds a list of integer values. The list
represents all the values of a particular tag in the database.
For integer values, the actual values are not stored. The reasoning
is that you have to remember the index anyway, which is equivalent to
remembering the actual value. For this reason request for a value at
a particular index, actually returns the index.

@todo
*/
class TagValues
{

public:
	TagValues() {}
	virtual ~TagValues() {}

	/** Adds a string value to the list, quickly if cache is enabled */
	virtual int add(const QString& value) { return value.toInt(); }
	/** Adds a integer value to the list. Integer values are not stored */
	virtual int add(const int value) { return value; }
	/** Returns a value from the list, given the index 'index' */
	virtual QString value(const int index) { return QString::number(index); }
	/** Returns the index of a value 'value */
	virtual int indexOf(const QString& value) { return value.toInt(); }
	/** Returns the number of values in the list */
	virtual int count() { return 0; }
	/** returns true if the list contains the string 'value' */
	virtual bool contains(const QString&) { return true; }
	/** When cache is enabled, adding values is very quick. It uses twice as
	  * much RAM though. Useful for adding lots of values quickly.
	  * Remember to disable afterwards to free extra memory.*/
	virtual void setCacheEnabled(const bool) {}

	/** Read the values for list from a data stream */
	virtual void read(QDataStream&) {}
	/** Write the current list values to a data stream */
	virtual void write(QDataStream&) {}



private:
	// Contains no data


};

#endif
