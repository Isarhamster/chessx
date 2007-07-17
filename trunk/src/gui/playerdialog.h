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
class Database;

/** @ingroup GUI
The PlayerDialog class is a dialog to browse player information. */
class PlayerDialog : public QDialog
{
	Q_OBJECT
public:
	PlayerDialog(PlayerDatabase* db, QWidget* parent = 0);
	virtual ~PlayerDialog();

public slots:
	/** Find players matching @p player . Put the list in the Players combo and show the first match. */
	void findPlayers(const QString& player);
	/** Find players matching current combo text. */
	void findPlayers();
	/** Show currently selected player */
	void showPlayer();
	/** Show given player. Do not change Player combo */
	void showPlayer(const QString& player);
	/** Show given player. Do not change Player combo. Set current database */
	void showPlayer(const QString& player, Database* db);
	/** Re-read configuration */
	void configure();
	/** Set database used for getting player information. */
	void setDatabase(Database* db);
private:
	/** Get formatted info about player from current database. */
	QString databaseInfo(const QString& player);
	/** Information about a player */
	QString formatPlayer(const QString& player);
	Ui::PlayerDialog ui;
	PlayerDatabase* m_playerDatabase;
	Database* m_database;
	bool m_showRating;
	int m_showLimit;

};


#endif

