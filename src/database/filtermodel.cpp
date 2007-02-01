/***************************************************************************
                          FilterModelBase.cpp - Provides a model for the Filter class
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

#include "filtermodel.h"

FilterModelBase::FilterModelBase(Filter*filter, QObject* parent)
         : QAbstractItemModel(parent), m_filter(filter)
{
   m_columnNames << "White"
                 << "Black"
                 << "Event"
                 << "Site"
                 << "Round"
                 << "Date"
                 << "Result"
                 << "ECO"
                 << "Length";
   m_game = new Game;
}
FilterModelBase::~FilterModelBase()
{
   delete m_game;
}

int FilterModelBase::rowCount(const QModelIndex& index) const
{
  if (index.isValid())
    return 0;
  return m_filter->count();
}

int FilterModelBase::columnCount(const QModelIndex&) const
{
   return m_columnNames.count();
}

QVariant FilterModelBase::data(const QModelIndex &index, int role) const
{
   if (!index.isValid())
      return QVariant();
   if (index.row() >= m_filter->count()) {
      return QVariant();
   }

   if (role == Qt::DisplayRole) 
   {
     m_filter->database()->loadHeaders(index.row(), *m_game);
     if (index.column() == 0)
       qDebug("Loading game %d: %s-%s", index.row(),
             qPrintable(m_game->tag(m_columnNames.at(0))), qPrintable(m_game->tag(m_columnNames.at(1))));
     return m_game->tag(m_columnNames.at(index.column()));
   }
   else return QVariant();
}

QVariant FilterModelBase::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (role != Qt::DisplayRole)
      return QVariant();

   if (orientation == Qt::Horizontal)
      return QString("%1").arg(m_columnNames.at(section));
   else
      return QString("%1").arg(section);
}

QModelIndex FilterModelBase::index(int row, int column, const QModelIndex& parent) const
{
  if (parent.isValid())
    return QModelIndex();
  return createIndex(row, column);
}

void FilterModelBase::setFilter(Filter* filter)
{
  m_filter = filter;
  reset();
}



FilterModel::FilterModel(Filter* filter, QObject* parent) : QSortFilterProxyModel(parent)
{
  m_filter = filter;
  FilterModelBase* source = new FilterModelBase(filter, parent);
  setSourceModel(source);
}


void FilterModel::setFilter(Filter* filter)
{
  m_filter = filter;
  qobject_cast<FilterModelBase*>(sourceModel())->setFilter(filter);
}

bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex& index) const
{
  qDebug("%d in filter? %s", source_row, m_filter->contains(source_row) ? "Yes" : "No");
  return m_filter->contains(source_row);
}

FilterModel::~FilterModel()
{
  delete sourceModel();
}

