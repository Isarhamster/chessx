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

	setObjectName("PlayerDialog");
	m_playerDatabase = db;
	m_database = 0;
	connect(ui.findButton, SIGNAL(clicked()), SLOT(findPlayers()));
	connect(ui.playerCombo, SIGNAL(activated(const QString&)), SLOT(showPlayer(const QString&)));
	if (parent)
		connect(parent, SIGNAL(reconfigure()), SLOT(configure()));
	configure();
}

PlayerDialog::~PlayerDialog()
{}

void PlayerDialog::configure()
{
	AppSettings->beginGroup("/Players/");
	m_showRating = AppSettings->value("rating", true).toBool();
	m_showLimit = AppSettings->value("count", 100).toInt();
	AppSettings->endGroup();
}

void PlayerDialog::findPlayers(const QString& s)
{
	// Capitalize first letter
	QString name = s;
	if (!name.isEmpty())
		name[0] = name[0].toUpper();
	QStringList players = m_playerDatabase->findPlayers(name, m_showLimit);
	ui.playerCombo->clear();
	if (players.count()) {
		QString player;
		foreach(player, players)
		ui.playerCombo->addItem(player);
		showPlayer(ui.playerCombo->itemText(0));
	}
	else
		showPlayer(s);
	ui.playerCombo->setEditText(s);
	show();
}

void PlayerDialog::findPlayers()
{
	findPlayers(ui.playerCombo->currentText());
}

void PlayerDialog::showPlayer(const QString& player)
{
	QString found;
	ui.playerView->setText(tr("<html><h2>%1</h2>%2</html>").arg(player).arg(databaseInfo(player)));
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

void PlayerDialog::showPlayer()
{
	showPlayer(ui.playerCombo->currentText());
}

void PlayerDialog::showPlayer(const QString& player, Database* db)
{
	m_database = db;
	showPlayer(player);
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
	if (m_showRating) {
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
	}
	if (!rating.isEmpty())
		rating.prepend(tr("<h2>Rating</h2>"));

	return tr("<h2>%1</h2>%2%3<br>Country: %4<br>Title: %5\n%6%7")
	       .arg(player).arg(image).arg(live).arg(country).arg(title).arg(bio).arg(rating);
}

void PlayerDialog::setDatabase(Database* db)
{
	m_database = db;
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
	unsigned bresults[5] = {0};
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
	if (!(results[4] + bresults[0]))
		return tr("<i>No games in current database.</i>");
	int wscore = 2 * results[WhiteWin] + results[Draw] + results[Unknown];
	QString white = tr("White: %1% +%2 =%3 -%4 %5/%6").arg(wscore / (0.02 * results[4]))
			.arg(results[WhiteWin]).arg(results[Draw]).arg(results[BlackWin])
			.arg(wscore / 2.0).arg(results[4]);
	int bscore = 2 * bresults[BlackWin] + bresults[Draw] + bresults[Unknown];
	QString black = tr("Black: %1% +%2 =%3 -%4 %5/%6").arg(bscore / (0.02 * bresults[4]))
			.arg(bresults[BlackWin]).arg(bresults[Draw]).arg(bresults[WhiteWin])
			.arg(bscore / 2.0).arg(bresults[4]);
	QString total = tr("Total: %1% +%2 =%3 -%4 %5/%6").arg((bscore + wscore) / (0.02 * (results[4] + bresults[4])))
			.arg(results[WhiteWin] + bresults[BlackWin]).arg(results[Draw] + bresults[Draw])
			.arg(results[BlackWin] + bresults[WhiteWin]).arg((wscore + bscore) / 2.0)
			.arg(results[4] + bresults[4]);
	QString daterange;
	if (dates[0] == dates[1])
		daterange = QString("(%1)").arg(dates[0].asString());
	else daterange = QString("(%1 -%2)").arg(dates[0].asString()).arg(dates[1].asString());
	return tr("Games in current database: %1 %2<br>%3<br>%4<br>%5<br>").arg((results[4] + bresults[4]))
			.arg(daterange).arg(white).arg(black).arg(total);
}

