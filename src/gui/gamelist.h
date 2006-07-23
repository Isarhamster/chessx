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
class Database;

class GameList : public QWidget
{
  Q_OBJECT
public:
  enum Column {Index, White, Black, Event, Site, Date, Result, ECO, Length};
  GameList(QWidget* parent = 0, const char *name = 0);
  /** Set current database */
  void setDatabase(Database* database);
public slots:
  void scrollList(int page);
  void setItemCount(int count);
  void itemSelected(QListViewItem* item);
signals:
  void selected(int);
private:
  virtual void resizeEvent(QResizeEvent* event);
//  virtual void selectItem(int index);
  /** Add visible list item */
  void addItem(int index);
  virtual bool eventFilter(QObject* o, QEvent* e);
  QListView* m_list;
  QScrollBar* m_scroll;
  int m_count;
  int m_pageSize;
  int m_itemHeight;
  Database* m_database;
};

#endif
