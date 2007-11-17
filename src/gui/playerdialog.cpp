/***************************************************************************
                          PlayerDialog - browser of player's database
                             -------------------
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

#include <QComboBox>
#include <QSortFilterProxyModel>
#include <QtGlobal>
#include <QTextBrowser>

#include "database.h"
#include "search.h"
#include "game.h"
#include "playerdialog.h"
#include "playerdatabase.h"
#include "settings.h"

PlayerDialog::PlayerDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	m_filterModel = new QSortFilterProxyModel(this);
	m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui.playersView->setModel(m_filterModel);

	setObjectName("PlayerDialog");
	connect(ui.filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findPlayers(const QString&)));
	connect(ui.showButton, SIGNAL(clicked()), SLOT(showSelectedPlayer()));
	connect(ui.playersView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(showSelectedPlayer()));
	connect(ui.updatePlayerButton, SIGNAL(clicked()), SLOT(updatePlayer()));
	if (parent)
		connect(parent, SIGNAL(reconfigure()), SLOT(configure()));
	configure();
}

PlayerDialog::~PlayerDialog()
{}

void PlayerDialog::configure()
{
	/*
	AppSettings->beginGroup("/Players/");
	AppSettings->endGroup();
	*/
}

void PlayerDialog::findPlayers(const QString& s)
{
	m_filterModel->setFilterFixedString(s);
}

void PlayerDialog::showPlayer(const QString& player)
{
	if (!player.isEmpty()) {
		m_player.setName(player);
		m_player.update();
		ui.playerView->setText(QString("<h1>%1</h1><p>%2%3")
				.arg(m_player.name()).arg(m_player.formattedGameCount()).arg(m_player.formattedScore()));
		ui.tabs->setCurrentIndex(1);
	}
	else
		ui.playerView->setText(tr("<html><i>No player chosen.</i></html>"));
	show();
}

void PlayerDialog::showSelectedPlayer()
{
	if (ui.playersView->currentIndex().isValid())
		showPlayer(ui.playersView->currentIndex().data().toString());
}

void PlayerDialog::updatePlayer()
{
	showPlayer(m_player.name());
}


void PlayerDialog::setDatabase(Database* db)
{
	m_player.setDatabase(db);
	m_filterModel->setSourceModel(db->index()->tagValues(TagPlayerName));
	m_filterModel->sort(0);
}


