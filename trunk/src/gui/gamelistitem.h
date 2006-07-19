/***************************************************************************
                          gamelistitem.h  -  Game List Item
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

#ifndef __GAMELISTITEM_H__
#define __GAMELISTITEM_H__

#include <qlistview.h>

class GameListItem : public QListViewItem
{
public:
  enum Column {Index, White, Black, Event, Site, Date, Result, ECO, Length};
  GameListItem(QListView* list);
  virtual int compare(QListViewItem* i, int col, bool ascending) const;
};

#endif

