/***************************************************************************
                          gamelist.h  -  Game List window
                             -------------------
    begin                : Sun 23 Jul 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GAMELIST_H__
#define __GAMELIST_H__

#include <QTreeView>

class Filter;
class FilterModel;

/** The GameList class displays list of the games in current filter. */

class GameList : public QTreeView
{
  Q_OBJECT
public:
  GameList(Filter* filter, QWidget* parent = 0);
  ~GameList();
  /** Set current database */
public slots:
  /** Read current configuration */
  void configure();
  /** Store current configuration */
  void saveConfig();
  /** Change current filter/database */
  void setFilter(Filter* filter);
  /** Update filter (called after changing filter outside) */
  void updateFilter();
  /** Perform simple search */
  void simpleSearch(int tag);
  /** Select and show current game in the list */
  void selectGame(int index);
private slots:
  void itemSelected(const QModelIndex& index);
signals:
  void selected(int);
  void searchDone();
private:
  FilterModel* m_model;
};

#endif
