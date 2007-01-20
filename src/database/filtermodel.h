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

#include <QAbstractTableModel>
#include <QStringList>

#include "filter.h"
#include "game.h"

class FilterModel : public QAbstractTableModel
{
   Q_OBJECT

   public:
      FilterModel(Filter *filter, QObject *parent = 0);
      ~FilterModel();

      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      int columnCount(const QModelIndex& parent = QModelIndex()) const;
      QVariant data(const QModelIndex &index, int role) const;
      QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags ( const QModelIndex & index ) const;

      Filter* filter();

   private:
      Filter *m_filter;
      QStringList m_columnNames;
      Game *m_game;

};


#endif	// __FILTERMODEL_H__

