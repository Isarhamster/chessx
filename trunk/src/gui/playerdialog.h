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
class QSortFilterProxyModel;

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
	/** Show currently selected player */
	void showSelectedPlayer();
	/** Update current player using current database */
	void updatePlayer();
	/** Show information about given player from current database. */
	void showPlayer(const QString& player);
	/** Re-read configuration */
	void configure();
	/** Set database used for getting player information. */
	void setDatabase(Database* db);
private:
	/** Get formatted info about player from current database. */
	QString databaseInfo(const QString& player);
	/** Information about a player */
	QString formatPlayer(const QString& player);
	/** Format single color. */
	QString formatResult(int results[5]);
	Ui::PlayerDialog ui;
	PlayerDatabase* m_playerDatabase;
	Database* m_database;
	QString m_player;
	QSortFilterProxyModel* m_filterModel;
};


#endif

