/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __FILTERMODEL_H__
#define __FILTERMODEL_H__

#include <QAbstractItemModel>
#include <QStringList>
#include <QPointer>

class Filter;

/** @ingroup Database
  The FilterModel class is an interface to Database used with Qt
  Model/View architecture
*/
class FilterModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    /** Constructs a FilterModel object using a pointer to a Filter */
    FilterModel(Filter* filter, QObject *parent = 0);
    ~FilterModel();

    /** Returns the number of rows in the model */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /** Returns the number of columns in the model */
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    /** Returns an item of data given the item 'index' */
    virtual QVariant data(const QModelIndex &index, int role) const;
    /** Returns the header information for header 'section' */
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    /** Return flags for items (especially drag drop hints) */
    Qt::ItemFlags flags(const QModelIndex &index) const;
    /** No tree - always return invalid parent */
    virtual QModelIndex parent(const QModelIndex&) const
    {
        return QModelIndex();
    }
    /** No tree - always return self */
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    /** No children */
    virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const
    {
        return !parent.isValid();
    }
    /** Associated filter */
    virtual Filter* filter();
    /** Changes current database. Resets any views. */
    virtual void setFilter(Filter* filter);
    /** Get the column tags. */
    const QStringList GetColumnTags()
    {
        return m_columnTags;
    }

    void updateColumns();
private:
    void addColumns(QStringList tags);
    void setupColumns();

    /** A pointer to filter on which the model opperates */
    QPointer<Filter> m_filter;
    /** The column names of the model */
    QStringList m_columnNames;
    /** Map of columns and database tags */
    QStringList m_columnTags;
};

#endif	// __FilterModelBase_H__

