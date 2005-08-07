/***************************************************************************
                          PlayerDialog - browser of player's database
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __PLAYERDIALOG_H__
#define __PLAYERDIALOG_H__

#include "playerdialogbase.h"


class PlayerDatabase;

class PlayerDialog : public PlayerDialogBase
{
  Q_OBJECT
public:
  PlayerDialog(PlayerDatabase* db, QWidget* parent = 0);
  ~PlayerDialog();

public slots:
  void findPlayers(const QString& s);
  void showPlayer(const QString& s);
  void showPlayer(QListViewItem* i);

private:
  PlayerDatabase* m_database;
};


#endif

