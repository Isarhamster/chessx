/***************************************************************************
                          filtermodel.cpp - Provides a model for the Filter class
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
#include <QtDebug>

FilterModel::FilterModel(Filter *filter, QObject *parent)
         : QAbstractTableModel(parent), m_filter(filter)
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
FilterModel::~FilterModel()
{
   delete m_game;
}
Filter* FilterModel::filter()
{
   return m_filter;
}

int FilterModel::rowCount(const QModelIndex & /* parent */) const
{
   return m_filter->count();
}

int FilterModel::columnCount(const QModelIndex& /* parent */) const
{
   return m_columnNames.size();
}

Qt::ItemFlags FilterModel::flags ( const QModelIndex & index ) const
{
   if (!index.isValid()) {
      return 0;
   }
   if (index.row() >= m_filter->count()) {
      return 0;
   }
   return Qt::ItemIsSelectable|Qt::ItemIsEnabled;

}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
   qDebug() << "Just show me anything" << index.row() << index.column();
   if (!index.isValid()) {
      qDebug() << "Invalid index";
      return QVariant();
   }

   if (index.row() >= m_filter->count()) {
      qDebug() << "Index out of bounds";
      return QVariant();
   }
   qDebug() << "Good Index";
   qDebug() << index.row() << index.column();

   int gameId = m_filter->gameIndex(index.row());
   qDebug() <<  m_game->tag(m_columnNames.at(index.column()));

   if (role == Qt::DisplayRole) {
      m_filter->database()->loadHeaders(gameId,*m_game); 

      qDebug() <<  m_game->tag(m_columnNames.at(index.column()));
      return m_game->tag(m_columnNames.at(index.column()));
      
   } else {
      return QVariant();
   }
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (role != Qt::DisplayRole)
      return QVariant();

   if (orientation == Qt::Horizontal)
      return QString("%1").arg(m_columnNames.at(section));
   else
      return QString("%1").arg(section);
}


