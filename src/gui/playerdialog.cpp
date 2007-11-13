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

PlayerDialog::PlayerDialog(PlayerDatabase* db, QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	m_filterModel = new QSortFilterProxyModel(this);
	m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui.playersView->setModel(m_filterModel);

	setObjectName("PlayerDialog");
	m_playerDatabase = db;
	m_database = 0;
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
	QString found;
	m_player = player;
	ui.playerView->setText(tr("<html><h2>%1</h2>%2</html>").arg(player)
			.arg(databaseInfo(player)));
	ui.tabs->setCurrentIndex(1);
	show();
	/*
	if (m_playerDatabase->exists(player))
		found = player;
	else {
		QStringList players = m_playerDatabase->findPlayers(player, 1);
		if (players.count())
			found = players[0];
	}
	if (found.isEmpty())
		ui.playerView->setText(tr("<html><i>No player found</i></html>"));
	else
		ui.playerView->setText(formatPlayer(found));
	*/
}

void PlayerDialog::showSelectedPlayer()
{
	if (ui.playersView->currentIndex().isValid())
		showPlayer(ui.playersView->currentIndex().data().toString());
}

void PlayerDialog::updatePlayer()
{
	showPlayer(m_player);
}


QString PlayerDialog::formatPlayer(const QString& player)
{
	m_playerDatabase->setCurrent(player);
	QString birth = m_playerDatabase->dateOfBirth().asShortString();
	QString death = m_playerDatabase->dateOfDeath().asShortString();
	QString live = death.isEmpty() ? tr("Born %1.").arg(birth) :
		       tr("Born: %1, died %2.").arg(birth).arg(death);
	QString country = m_playerDatabase->country();
	QString image;
	if (m_playerDatabase->hasPhoto()) {
		ui.playerView->document()->addResource(QTextDocument::ImageResource, QUrl("image.png"),
						       m_playerDatabase->photo());
		image = "<br><img src=\"image.png\"><br>";
	}
	QString bio;
	if (m_playerDatabase->hasBiography())
		bio = tr("<h3>Biography</h3>%1\n").arg(m_playerDatabase->biography());
	QString title = m_playerDatabase->title();

	// Rating
	QString rating;
	int elo = m_playerDatabase->highestElo();
	if (elo)
		rating = tr("Highest rating: %1<br>").arg(elo);
	int eloindex = m_playerDatabase->lastEloListIndex();
	elo = m_playerDatabase->elo(eloindex);
	if (elo)
		rating.append(tr("Last rating: %1 (%2)").arg(elo)
			      .arg(m_playerDatabase->eloListToDate(eloindex).asShortString()));
	rating.append(tr("<br>Rating history:<br><blockquote>"));
	int start = m_playerDatabase->firstEloListIndex();
	int end = m_playerDatabase->lastEloListIndex();
	for (int i = start; i <= end; i++) {
		int elo = m_playerDatabase->elo(i);
		if (!elo)
			continue;
		if (i != start)
			rating.append(", ");
		rating.append(QString("%1:&nbsp;%2").arg(m_playerDatabase->eloListToDate(i).asShortString()).arg(elo));
	}
	rating.append("</blockquote>\n");
	if (!rating.isEmpty())
		rating.prepend(tr("<h2>Rating</h2>"));

	return tr("<h2>%1</h2>%2%3<br>Country: %4<br>Title: %5\n%6%7")
	       .arg(player).arg(image).arg(live).arg(country).arg(title).arg(bio).arg(rating);
}

void PlayerDialog::setDatabase(Database* db)
{
	m_database = db;
	m_filterModel->setSourceModel(m_database->index()->tagValues(TagPlayerName));
	m_filterModel->sort(0);
}


QString formatResult(int results[5])
{
	int score = 2 * results[WhiteWin] + results[Draw] + results[Unknown];
	return QString(" %1% +%2 =%3 -%4 %5/%6").arg(score / (0.02 * results[4]), 0, 'f', 2)
	       .arg(results[WhiteWin]).arg(results[Draw]).arg(results[BlackWin])
	       .arg(score / 2.0).arg(results[4]);
}

int toResult(const QString& res)
{
	if (res == "1-0") return WhiteWin;
	else if (res == "1/2-1/2") return Draw;
	else if (res == "0-1") return BlackWin;
	else return Unknown;
}

QString PlayerDialog::databaseInfo(const QString& player)
{
	if (!m_database)
		return QString();
	int results[5] = {0};
	PartialDate dates[2] = {PartialDate("2999"), PartialDate("0")};
	int ratings[2] = {4000, 0};
	TagSearch ts(m_database, "White", player);
	Filter filter(m_database);
	filter.executeSearch(ts);
	Game game;
	for (int i = 0; i < filter.size(); i++)
		if (filter.contains(i))	{
			m_database->loadGameHeaders(i, game);
			results[toResult(game.tag("Result"))]++;
			results[4]++;
			int elo = game.tag("WhiteElo").toInt();
			if (elo) {
				ratings[0] = qMin(ratings[0], elo);
				ratings[1] = qMax(ratings[1], elo);
			}
			PartialDate date(game.tag("Date"));
			if (date.year()) {
				dates[0] = qMin(dates[0], date);
				dates[1] = qMax(dates[1], date);
			}
		}
	ts.setTag("Black");
	filter.executeSearch(ts);
	int bresults[5] = {0};
	for (int i = 0; i < filter.size(); i++)
		if (filter.contains(i))	{
			m_database->loadGameHeaders(i, game);
			bresults[toResult(game.tag("Result"))]++;
			bresults[4]++;
			int elo = game.tag("BlackElo").toInt();
			if (elo) {
				ratings[0] = qMin(ratings[0], elo);
				ratings[1] = qMax(ratings[1], elo);
			}
			PartialDate date(game.tag("Date"));
			if (date.year()) {
				dates[0] = qMin(dates[0], date);
				dates[1] = qMax(dates[1], date);
			}
		}
	qSwap(bresults[WhiteWin], bresults[BlackWin]);
	if (!(results[4] + bresults[4]))
		return tr("<i>No games in database %1.</i>").arg(m_database->name());


	QString white = tr("White:") + formatResult(results);
	QString black = tr("Black:") + formatResult(bresults);
	for (int i = 0 ; i < 5; i++)
		results[i] += bresults[i];
	QString total = tr("Total:") + formatResult(results);
	QString daterange;
	if (dates[0] == dates[1])
		daterange = QString("(%1)").arg(dates[0].asString());
	else daterange = QString("%1 - %2").arg(dates[0].asString()).arg(dates[1].asString());
	return tr("Games in database <i>%1</i>: <b>%2</b><br>"
			"Time span: <b>%3</b><br>%4<br>%5<br>%6<br>")
			.arg(m_database->name()).arg((results[4]))
			.arg(daterange).arg(total).arg(white).arg(black);
}

