/***************************************************************************
                          gamelistitem.cpp  -  Game List Item
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

#include "gamelistitem.h"

int GameListItem::compare(QListViewItem* i, int col, bool ascending) const
{
  if (col == Index || col == Length)
    return ascending ? key(col, ascending).toInt() - i->key(col, ascending).toInt() :
        i->key(col, ascending).toInt() - key(col, ascending).toInt();
  else return QListViewItem::compare(i, col, ascending);
}

GameListItem::GameListItem(QListView * list) : QListViewItem(list)
{
}

