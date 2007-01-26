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

#include "ui_playerdialog.h"


class PlayerDatabase;

class PlayerDialog : public QDialog
{
  Q_OBJECT
public:
  PlayerDialog(PlayerDatabase* db, QWidget* parent = 0);
  virtual ~PlayerDialog();

public slots:
  /** Find players and show the dialog */
  void findPlayers(const QString& s);
  void showPlayer(const QString& s);
  void showPlayer(Q3ListViewItem* i);
  /* re-read configuration */
  void configure();
private:
  Ui::PlayerDialog ui;
  PlayerDatabase* m_database;
  bool m_showRating;
  int m_showLimit;
};


#endif

