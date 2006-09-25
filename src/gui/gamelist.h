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

#include <qwidget.h>

class QListView;
class QListViewItem;
class QScrollBar;
class DatabaseInfo;

class GameList : public QWidget
{
  Q_OBJECT
public:
  enum Column {Index, White, Black, Event, Site, Round, Date, Result, ECO, Length};
  GameList(QWidget* parent = 0, const char *name = 0);
  /** Set current database */
public slots:
  /** Set new database (or null for no database) */
  void setDatabase(DatabaseInfo* database);
  /** Scroll to show item @p index */
  void scrollList(int index);
  /** Read current configuration */
  void configure();
  /** Store current configuration */
  void saveConfig();
private slots:
  void itemSelected(QListViewItem* item);
signals:
  void selected(int);
private:
  /** Recalculate list after database/filter is changed */ 
  void updateList();
  /** Recalculate scrollbar after resize */ 
  void updateScrollbar();
  /** Recalculate visible items after resize */ 
  virtual void resizeEvent(QResizeEvent* event);
  /** Create visible list item from game @p index */
  void createItem(int index);
  virtual bool eventFilter(QObject* o, QEvent* e);
  QListView* m_list;
  QScrollBar* m_scroll;
  int m_count;
  int m_pageSize;
  int m_itemHeight;
  DatabaseInfo* m_database;
};

#endif
