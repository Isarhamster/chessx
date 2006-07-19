/***************************************************************************
                          gamelist.h  -  Game List dialog
                             -------------------
    begin                : 18 Jul 2006
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

#ifndef __GAMELISTDIALOG_H__
#define __GAMELISTDIALOG_H__

#include "gamelistbase.h"

class Database;
class Filter;
class QListViewItem;

class GameListDialog : public GameListBase
{
  Q_OBJECT
  public:
    /** Create Game List dialog */
    GameListDialog(QWidget* parent = 0);
    /** Set filter */
    void setFilter(Filter* filter);
    /** Set filter */
    void setDatabase(Database* database);
    /** Destroy the dialog */
    ~GameListDialog();
    /** @return current page. */
    int page() const;
  public slots:
    /** Set current page */
    void gotoFirstPage();
    /** Set current page */
    void gotoLastPage();
    /** Set current page */
    void setPage(int page);
    /** Reset filter */
    void resetFilter();
    /** Set filter */
    void filter();
    /** Game highlighted */
    void gameHighlighted(QListViewItem* item);
    /** Game selected (double clicked) */
    void gameSelected(QListViewItem* item);
  signals:
    /** Game highlighted */
    void highlighted(int);
    /** Game selected (double clicked) */
    void selected(int);
  private:
    Filter* m_filter;
    Database* m_database;
    int m_pageLength;
};

#endif



