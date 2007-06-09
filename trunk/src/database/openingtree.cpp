/***************************************************************************
                          OpeningTree
                             -------------------
    begin                : 9 Jun 2007
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

#include "openingtree.h"
#include "common.h"
#include "database.h"

MoveData::MoveData()
{
	count = 0;
	for (int  r = Unknown; r <= BlackWin; r++)
		result[r] = 0;
	rating = 0;
	rated = 0;
}

void MoveData::addGame(Game& g, Color c)
{
	if (move.isEmpty())
		move = g.moveToSan();
	count++;
	result[g.result()]++;
	unsigned elo = (c == White) ? g.tag("WhiteElo").toInt() : g.tag("BlackElo").toInt();
	if (elo > 1000) {
		rating += elo;
		rated++;
	}
}

int MoveData::percentage() const
{
	unsigned c = result[Unknown] + 2 * result[WhiteWin] + result[Draw];
	return c * 50 / count;
}

int MoveData::averageRating() const
{
	return rated ? rating / rated : 0;
}

bool operator<(const MoveData& m1, const MoveData& m2)
{
	return m1.count > m2.count || (m1.count == m2.count && m1.move < m2.move);
}



void OpeningTree::update(Filter& f, const Board& b)
{
	PositionSearch ps(f.database(), b);
	f.executeSearch(ps);
	Game g;
	QMap<Move, MoveData> moves;
	for (int i = 0; i < f.size(); i++)
		if (f.contains(i)) {
			f.database()->loadGame(i, g);
			g.moveToPly(f.gamePosition(i) - 1);
			if (!g.atEnd()) {
				moves[g.move()].addGame(g, b.toMove());
			}
		}
	m_moves.clear();
	for (QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
		m_moves.append(it.value());
	qSort(m_moves.begin(), m_moves.end());
}

QString OpeningTree::debug()
{
	QString s;
	for (int i = 0; i < m_moves.count(); i++)
		s.append(QString("%1. %2\t%3 games\t%4%\n")
				.arg(i+1).arg(m_moves[i].move).arg(m_moves[i].count).arg(m_moves[i].percentage()));
	return s;
}

