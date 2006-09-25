/***************************************************************************
                          datebaseinfo  -  Single database information
                             -------------------
    begin                : 23 Aug 2006
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

#ifndef __DATABASEINFO_H__
#define __DATABASEINFO_H__

#include <qstring.h>

class Database;
class Filter;
class Game;

class DatabaseInfo
{
public:
  DatabaseInfo(const QString& fname = QString::null);
  ~DatabaseInfo();
  bool isValid() const;
  Database* database();
  Filter* filter();
  Game* currentGame();
  int currentIndex() const   {return m_index;}
  bool loadGame(int index);
private:
  Database* m_database;
  Filter* m_filter;
  Game* m_game;
  int m_index;
};

#endif

