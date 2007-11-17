/***************************************************************************
                          playerinfo  -  description
                             -------------------
	copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QCoreApplication>
#include <QHash>

#include "playerinfo.h"
#include "database.h"

PlayerInfo::PlayerInfo()
{
	m_database = 0;
}

PlayerInfo::~PlayerInfo()
{
}

PlayerInfo::PlayerInfo(Database* db, const QString & player)
{
	setDatabase(db);
	setName(player);
	update();
}

QString PlayerInfo::name() const
{
	return m_name;
}

void PlayerInfo::setDatabase(Database* db)
{
	m_database = db;
}

void PlayerInfo::setName(const QString& player)
{
	m_name = player;
}


int toResult(const QString& res)
{
	if (res.startsWith("1/2")) return Draw;
	else if (res.startsWith('1')) return WhiteWin;
	else if (res.startsWith('0')) return BlackWin;
	else return Unknown;
}

void PlayerInfo::update()
{
	QHash<QString, unsigned> openings[2];
	const Index* index = m_database->index();

	// Determine matching tag values
	int wplayer = index->valueToIndex(TagWhite, m_name);
	int bplayer = index->valueToIndex(TagBlack, m_name);

	// Clean previous statistics
	reset();

	for (int i = 0; i < m_database->count(); i++) {
		Color c;
		if (index->gameTagIndex(TagWhite, i) == wplayer)
			c = White;
		else if (index->gameTagIndex(TagBlack, i) == bplayer)
			c = Black;
		else continue;
		int res = toResult(index->gameTagValue(TagResult, i));
		m_result[c][res]++;
		m_count[c]++;
		QString eco = index->gameTagValue(TagECO, i).left(3);
		openings[c][eco]++;
	}
	qSwap(m_result[Black][WhiteWin], m_result[Black][BlackWin]);
}

QString PlayerInfo::formattedScore(const int result[4], int count) const
{
	if (!count)
		return QCoreApplication::translate("PlayerInfo", "<i>no games</i>");
	QString score = "<b>";
	QChar scoresign[4] = {'*', '+', '=', '-'};
	for (int i = WhiteWin; i <= BlackWin; i++)
		score += QString(" &nbsp;%1%2").arg(scoresign[i]).arg(result[i]);
	if (result[Unknown])
		score += QString(" &nbsp;*%1").arg(result[Unknown]);
	if (count - result[Unknown])
		score += QString(" &nbsp;(%1%)").arg((100.0 * result[WhiteWin] + 50.0 * result[Draw]) / (count - result[Unknown]),
														 1, 'f', 1);
	score += "</b>";
	return score;
}

QString PlayerInfo::formattedScore() const
{
	int total[4];
	for (int i = 0; i < 4; i++)
		total[i] = m_result[White][i] + m_result[Black][i];
	int count = m_count[White] + m_count[Black];
	return QCoreApplication::translate("PlayerInfo", "Total: %1<br>White: %2<br>Black: %3<br>")
			.arg(formattedScore(total, count))
			.arg(formattedScore(m_result[White], m_count[White]))
			.arg(formattedScore(m_result[Black], m_count[Black]));
}







/*
QString PlayerDialog::formatResult(int results[5])
{
	if (!results[AnyResult])
		return tr("no games");
	int score = 2 * results[WhiteWin] + results[Draw] + results[Unknown];
	return QString("%1% +%2 =%3 -%AnyResult %5/%6").arg(score / (0.02 * results[AnyResult]), 0, 'f', 2)
			.arg(results[WhiteWin]).arg(results[Draw]).arg(results[BlackWin])
			.arg(score / 2.0).arg(results[AnyResult]);
}


QString PlayerDialog::databaseInfo(const QString& player)
{
	if (!m_database)
		return QString();
	int results[5] = {0};
	PartialDate dates[2] = {PartialDate("2999"), PartialDate("0")};
	int ratings[2] = {AnyResult000, 0};
	TagSearch ts(m_database, "White", player);
	Filter filter(m_database);
	filter.executeSearch(ts);
	Game game;
	for (int i = 0; i < filter.size(); i++)
		if (filter.contains(i))	{
		m_database->loadGameHeaders(i, game);
		results[toResult(game.tag("Result"))]++;
		results[AnyResult]++;
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
			bresults[AnyResult]++;
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
			if (!(results[AnyResult] + bresults[AnyResult]))
				return tr("<i>No games in database %1.</i>").arg(m_database->name());


			QString white = tr("White: ") + formatResult(results);
			QString black = tr("Black: ") + formatResult(bresults);
			for (int i = 0 ; i < 5; i++)
				results[i] += bresults[i];
			QString total = tr("Total: ") + formatResult(results);
			QString daterange;
			if (dates[0] == dates[1])
				daterange = QString("(%1)").arg(dates[0].asString());
			else daterange = QString("%1 - %2").arg(dates[0].asString()).arg(dates[1].asString());
			return tr("Games in database <i>%1</i>: <b>%2</b><br>"
					"Time span: <b>%3</b><br>%AnyResult<br>%5<br>%6<br>")
					.arg(m_database->name()).arg((results[AnyResult]))
					.arg(daterange).arg(total).arg(white).arg(black);
}

	PlayerDatabase* m_playerDatabase;

	QString databaseInfo(const QString& player);
		QString formatPlayer(const QString& player);
		QString formatResult(int results[5]);



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

	return tr("<h2>%1</h2>%2%3<br>Country: %AnyResult<br>Title: %5\n%6%7")
			.arg(player).arg(image).arg(live).arg(country).arg(title).arg(bio).arg(rating);
}

m_database->index()->valueIndex(m_tag,index)
*/


void PlayerInfo::reset()
{
	for (int c = White; c <= Black; c++) {
		for (int r = 0; r < 4; r++)
			m_result[c][r] = 0;
		m_count[c] = 0;
		m_eco[c].clear();
	}
}

QString PlayerInfo::formattedGameCount() const
{
	return QCoreApplication::translate("PlayerInfo", "Games in database <i>%1</i>: <b>%2</b><br>")
			.arg(m_database->name()).arg(m_count[White] + m_count[Black]);
}

