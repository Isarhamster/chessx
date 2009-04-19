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
	reset();
}

PlayerInfo::~PlayerInfo()
{
}

PlayerInfo::PlayerInfo(Database* db, const QString & player)
{
	setDatabase(db);
	setName(player);
	reset();
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
		int elo = index->gameTagValue(c == White ? TagWhiteElo : TagBlackElo, i).toInt();
		if (elo) {
			m_rating[0] = qMin(elo, m_rating[0]);
			m_rating[1] = qMax(elo, m_rating[1]);
		}
		PartialDate date(index->gameTagValue(TagDate, i));
		if (date.year() > 1000) {
			m_date[0] = qMin(date, m_date[0]);
			m_date[1] = qMax(date, m_date[1]);
		}
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

void PlayerInfo::reset()
{
	for (int c = White; c <= Black; c++) {
		for (int r = 0; r < 4; r++)
			m_result[c][r] = 0;
		m_count[c] = 0;
		m_eco[c].clear();
	}
	m_rating[0] = 99999;
	m_rating[1] = 0;
	m_date[0].setYear(9999);
	m_date[1].setYear(1);

}

QString PlayerInfo::formattedGameCount() const
{
	return QCoreApplication::translate("PlayerInfo", "Games in database <i>%1</i>: <b>%2</b><br>")
			.arg(m_database->name()).arg(m_count[White] + m_count[Black]);
}

QString PlayerInfo::formattedRating() const
{
	if (!m_rating[1])
		return QString();
	else if (m_rating[0] == m_rating[1])
		return QCoreApplication::translate("PlayerInfo", "Rating: <b>%1</b><br>").arg(m_rating[0]);
	else
		return QCoreApplication::translate("PlayerInfo", "Rating: <b>%1-%2</b><br>")
				.arg(m_rating[0]).arg(m_rating[1]);
}

QString PlayerInfo::formattedRange() const
{
	if (m_date[0].year() == 9999)	// No date
		return QCoreApplication::translate("PlayerInfo","Date: <b>????.??.??<b><br>");
	else if (m_date[0].year() < 1000)
		return QString();
	else
		return QCoreApplication::translate("PlayerInfo", "Date: <b>%1</b><br>").arg(m_date[0].range(m_date[1]));
}

