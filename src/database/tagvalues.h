/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>ccccc           *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __TAGVALUES_H__
#define __TAGVALUES_H__

#include <QString>
#include <QStringList>
#include <QHash>
#include <QBitArray>
#include <QAbstractItemModel>

typedef quint32 TagIndex;

/** @ingroup Core
TagValues class holds a list of integer values. The list
represents all the values of a particular tag in the database.
For integer values, the actual values are not stored. The reasoning
is that you have to remember the index anyway, which is equivalent to
remembering the actual value. For this reason request for a value at
a particular index, actually returns the index.

@todo
*/
class TagValues : public QAbstractItemModel
{

public:
    TagValues() {}
    virtual ~TagValues() {}

    /** Adds a string value to the list, quickly if cache is enabled */
    virtual int add(const QString& value)
    {
        return value.toInt();
    }
    /** Adds a integer value to the list. Integer values are not stored */
    virtual int add(const int value)
    {
        return value;
    }
    /** Returns a value from the list, given the index 'index' */
    virtual QString value(int index) const
    {
        return QString::number(index);
    }
    /** Returns the index of a value 'value */
    virtual int indexOf(const QString& value) const
    {
        return value.toInt();
    }
    /** Returns the number of values in the list */
    virtual int count() const
    {
        return 0;
    }
    /** returns true if the list contains the string 'value' */
    virtual bool contains(const QString&)
    {
        return true;
    }
    /** When cache is enabled, adding values is very quick. It uses twice as
      * much RAM though. Useful for adding lots of values quickly.
      * Remember to disable afterwards to free extra memory.*/
    virtual void setCacheEnabled(const bool) {}

    /** Read the values for list from a data stream */
    virtual void read(QDataStream&) {}
    /** Write the current list values to a data stream */
    virtual void write(QDataStream&) {}

    /** Compacts tavalues with respect to the given vector where a zero
         *  in the vector means that the corresponding tagvalue has to be
         *  deleted. */
    virtual void compact(const QVector<bool>&) {};

    /** Appends value at the end of out */
    virtual void appendToStream(const QString& value, QDataStream& out);

    /** Abstract item model functions. */
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex&) const;
private:
    // Contains no data


};

#endif
