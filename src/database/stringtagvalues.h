/***************************************************************************
 *   (C) 2006-2007 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>                *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __STRINGTAGVALUES_H__
#define __STRINGTAGVALUES_H__

#include <QString>
#include <QStringList>
#include <QDataStream>
#include <QHash>
#include <QBitArray>

#include "tagvalues.h"


/** @ingroup Core
StringTagValues class inherits from TagValues, and contains
a list of unique strings. The list represents all the values
for a given tag in the database.  It has the ability to dump,
and read it's data from a QDataStream.

@todo
*/

class StringTagValues : public TagValues
{

public:
	StringTagValues();
	~StringTagValues();

	/** Adds a value to the list, quickly if cache is enabled */
	int add(const QString& value);
	/** Returns a value from the list, given the index 'index' */
	QString value(int index) const;
	/** Clears all values from the list */
	void clear();
	/** @return the index of a value @p value . Returns @p -1 if value is not found. */
	int indexOf(const QString& value) const;
    /** When cache is enabled, adding values is very quick. It uses twice as
      * much RAM though. Useful for adding lots of values quickly.
      * Remember to disable afterwards to free extra memory.*/
    void setCacheEnabled(const bool enabled);
    /** Returns the number of values in the list */
	virtual int count() const;
	/** returns true if the list contains the string 'value' */
	virtual bool contains(const QString& value);
	/** Read the values for list from a data stream */
	void read(QDataStream& in);
	/** Write the current list values to a data stream */
	void write(QDataStream& out);
        /** Appends value at the end of out */
	void appendToStream(const QString& value, QDataStream& out);
	/** Returns a list of indexes containing value */
	QBitArray listContainingValue(const QString& value);
	/** Returns a list of indexes that falls in the specified range */
	QBitArray listInRange(const QString& minValue, const QString& maxValue);
	/** Returns a list of strings starting with given prefix. Used for completion. */
	QStringList listContainingPrefix(const QString& value, int max) const;

        /** Compacts tavalues with respect to the given vector where a zero
         *  in the vector means that the corresponding tagvalue has to be
         *  deleted. */	
	void compact(const QVector<bool>& qv);

private:
    typedef QHash <QString, int> TagCache;
    QStringList m_list;
    TagCache *m_cache;

};

#endif

