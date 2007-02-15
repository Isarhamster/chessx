/***************************************************************************
                          filtermodel.h - Provides a model for the Filter class
                                          in term of the Qt filter/model framework
                             -------------------
    begin                : 17/01/2007
    copyright            : (C) 2007 Marius Roets  <saidinwielder@user.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __FILTERMODEL_H__
#define __FILTERMODEL_H__

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QStringList>

#include "filter.h"
#include "database.h"
#include "game.h"

/** @ingroup Database  
  The DatabaseModel class is an interface to Database used with Qt
  Model/View architecture. 
*/
class DatabaseModel: public QAbstractItemModel
{
   Q_OBJECT

   public:
      /** Constructs a DatabaseModel object using a pointer to a Filter */
      DatabaseModel(Database* database, QObject *parent = 0);
      ~DatabaseModel();

      /** Returns the number of rows in the model */
      virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
      /** Returns the number of columns in the model */
      virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
      /** Returns an item of data given the item 'index' */
      virtual QVariant data(const QModelIndex &index, int role) const;
      /** Returns the header information for header 'section' */
      virtual QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const;
      /** Returns the appropriate display flags for item 'index' */
    //  virtual Qt::ItemFlags flags (const QModelIndex& index) const;
      /** No tree - always return invalid parent */
      virtual QModelIndex parent(const QModelIndex&) const  {return QModelIndex();}
      /** No tree - always return self */
      virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
      /** No children */
      virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const {return !parent.isValid();}
      /** Changes current database. Resets any views. */
      virtual void setDatabase(Database* database);

   private:
      /** A pointer to filter on which the model opperates */
      Database* m_database;
      /** The column names of the model */
      QStringList m_columnNames;
      /** A pointer to a game object, to hold the retrieved information 
       * about the game */
      Game* m_game;
};

/** @ingroup Database  
  The FilterModel class is an interface to Filter used with Qt
  Model/View architecture. It uses QSortFilterProxyModel
  class to filter items from DataModel class.
*/
class FilterModel : public QSortFilterProxyModel
{
  Q_OBJECT
  public:
    FilterModel(Filter*, QObject* parent = 0);
    ~FilterModel();
    virtual void setFilter(Filter* filter);
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
    /** @return filter used for getting data */
    Filter* filter()  {return m_filter;}
    /** Update filter after a change */
    void updateFilter();
  private:
    Filter* m_filter;
};




#endif	// __FilterModelBase_H__

